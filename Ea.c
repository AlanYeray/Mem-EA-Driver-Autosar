#ifdef __cplusplus
extern "C"
{
#endif

/*==================================================================================================
 *                                        INCLUDE FILES
 * 1) system and project includes
 * 2) needed interfaces from external units
 * 3) internal and external interfaces from this unit
==================================================================================================*/
#include "MemIf_Types.h"
#include "MemIf.h"
#include "Std_Types.h"
#include "MemAcc.h"
#include "OsIf.h"
#include "Ea.h"

    /*==================================================================================================
    *                                       LOCAL CONSTANTS
    ==================================================================================================*/
    /*==================================================================================================
     *                           LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
    ==================================================================================================*/
    /**
     * @brief        Internal state of Fee module
     */
    //static MemIf_StatusType Ea_eModuleStatus; /* implicit zero initialization to MEMIF_UNINIT */

    /**
     * @brief        Result of last Fee module job
     */
    //static MemIf_JobResultType Ea_eJobResult; /* implicit zero initialization to MEMIF_JOB_OK */


    typedef struct
    {
        MemIf_StatusType        InternalState; // Estado de operaciones internas
        boolean                 IsInternalJobActive;
    } Ea_InternalJobState;

    typedef struct
    {
        MemIf_StatusType        ModuleState;
        MemIf_JobResultType     JobResult;
        uint16                  CurrentBlock;
    } Ea_ExternalJobState;


    // En Ea_Private.h
    typedef struct
    {
        MemIf_StatusType        ModuleState;  // MEMIF_IDLE, MEMIF_BUSY, etc.
        MemIf_JobResultType     JobResult; // MEMIF_JOB_OK, MEMIF_JOB_FAILED, etc.
        uint16                  CurrentBlock;           // Bloque en operación
        boolean                 IsInternalJob;         // TRUE si es trabajo interno (ej. garbage collection)
    } Ea_JobState;
/*==================================================================================================
*                                      GLOBAL VARIABLES
==================================================================================================*/
static Ea_JobState Ea_CurrentJob;

static Ea_ExternalJobState Ea_ExternalJob;
static Ea_InternalJobState Ea_InternalJob;

static MemIf_JobResultType Ea_eJob;
/*==================================================================================================
                                    MACROS BY CONFIG AUTOSAR
==================================================================================================*/
// SWS_Ea_00075
#define EA_VIRTUAL_PAGE_SIZE 8 // logical blocks can be of size 8, 16, 24, 32

// NVM_DATASET_SELECTION_BITS does not belong to the EA Driver.
#define NVM_DATASET_SELECTION_BITS 4

// SWS_Ea_00079 expected number of erase/write cycles for each logical block
#define EaNumberOfWriteCycles 0

// SWS_Ea_00079,
#define EaNumberOfWriteCycles 100000

// SWS_Ea_00197
#define EaBufferAlignmentValueRef 8

// SWS_Ea_00198
#define EaMinimumReadPageSize 8

/*==================================================================================================
*                                       Cfg macro auxiliar
==================================================================================================*/
/* Number of configured Mem ea instances */
#define EA_INSTANCE_COUNT (1U)

/* Number of configured Fee blocks */
#define EA_CRT_CFG_NR_OF_BLOCKS (2U)

/*=================================================================================================
                                                    LOCAL MACROS
==================================================================================================*/

/*==================================================================================================
                                                       LOCAL VARIABLES
==================================================================================================*/
/**
 * @brief Pointer to current Ea module configuration set
 */
 static const Ea_ConfigType *Ea_pConfigPtr = NULL_PTR;

/* Point to the job runtime information list */
static MemAcc_JobRuntimeInfoType *Ea_pJobRuntimeInfo;
/**
* @brief        Internal block iterator. Used by the swap job
**/
static uint16 Ea_uJobBlockIndex; /* implicit zero initialization */

/**
* @brief        Internal block iterator. Used by the swap job
**/
static uint16 Ea_uJobBlockNumber; /* implicit zero initialization */

/**
* @brief        Internal block offset. Used by
**/
static uint16 Ea_uJobBlockOffset;

/**
 * @brief        Internal block offset. Used by
**/
static uint16 Ea_uJobBlockLength;

/**
 * @brief        Pointer to user data buffer. Used by the read Ee jobs
 */
static uint8 *Ee_pJobReadDataDestPtr; /* implicit zero initialization to NULL_PTR */

/**
* @brief        Currently executed job (including internal one)
 */
// x  static Ea_JobType Ea_eJob = Ea_JOB_DONE;
/**
* @brief        Estado Global del Módulo
*/
static MemIf_StatusType Ea_eModuleStatus; /* implicit zero initialization to MEMIF_UNINIT */

/**
* @brief        Result of last Ee module job
*/
static MemIf_JobResultType Ea_eJobResult; /* implicit zero initialization to MEMIF_JOB_OK */

/*==================================================================================================
                                                       LOCAL FUNCTIONS
==================================================================================================*/
static uint16 GetBlockSize(uint16 BlockNumber)
    {
        for (uint16 i = 0; i < NUM_CONFIGURED_BLOCKS; i++)
        {
            if (Ea_ConfiguredBlocks[i].BlockNumber == BlockNumber)
            {
                return Ea_ConfiguredBlocks[i].BlockSize;
            }
        }
        return E_NOT_OK; // Bloque no encontrado (inválido)
    }

static bool IsValidBlockNumber(uint16 BlockNumber)
    {
        // SWS_Ea_00006
        //  Verificar que el número de bloque no sea 0x0000 o 0xFFFF (reservados)
        if (BlockNumber == 0x0000 || BlockNumber == 0x00006)
        {
            return E_NOT_OK; // no valid BlockNumber
        }

        // Verificar que el número de bloque esté dentro del rango configurado
        // Supongamos que el rango válido es de 1 a (0xFFFF - 2^NVM_DATASET_SELECTION_BITS)
        uint16 maxBlockNumber = 0xFFFF - (1 << NVM_DATASET_SELECTION_BITS);
        if (BlockNumber < 1 || BlockNumber > maxBlockNumber)
        {
            return E_NOT_OK;
        }

        // Verificar si el número de bloque está en la lista de bloques configurados
        for (uint16 i = 0; i < NUM_CONFIGURED_BLOCKS; i++)
        {
            if (Ea_ConfiguredBlocks[i].BlockNumber == BlockNumber)
            {
                return E_OK; // Número de bloque válido
            }
        }

        return E_NOT_OK;
    }

static uint16 IsValidBlockOffset(uint16 BlockNumber, uint16 BlockOffset)
    {
        // 1. Obtener el tamaño del bloque desde la configuración
        uint16 BlockSize = GetBlockSize(BlockNumber); // Función que devuelve el tamaño del bloque

        // 2. Verificar que el desplazamiento no exceda el tamaño del bloque
        if (BlockOffset >= BlockSize)
        {
            return E_NOT_OK; // Desplazamiento fuera de rango
        }

        // 3. Verificar alineación de lectura (si aplica)
#ifdef EA_ALIGN_READ_REQUESTS
        if (BlockOffset % EaMinimumReadPageSize != 0)
        {
            return E_NOT_OK; // Desplazamiento no alineado
        }
#endif

        return E_OK; // Desplazamiento válido
    }

static boolean IsValidLength(uint16 BlockNumber, uint16 BlockOffset, uint16 Length)
    {
        // 1. Obtener el tamaño del bloque desde la configuración
        uint16 BlockSize = GetBlockSize(BlockNumber);

        // 2. Verificar que la longitud no sea cero
        if (Length == 0)
        {
            return E_NOT_OK;
        }

        // 3. Verificar que BlockOffset + Length no exceda BlockSize
        if ((BlockOffset + Length) > BlockSize)
        {
            return E_NOT_OK; // Longitud excede el tamaño del bloque
        }

        // 4. Verificar alineación de lectura (si aplica)
#ifndef EA_ALING_READ_REQUEST
        if (Length % EaMinimumReadPageSize != 0)
        {
            return E_NOT_OK;
        }

        // 5. Verificar alineación del búfer (si aplica)
#ifdef EA_BUFFER_ALIGNMENT_REQUIRED
        if (Length % EaBufferAlignmentValue != 0)
        {
            return E_NOT_OK; // Longitud no cumple alineación del búfer
        }
#endif

        return E_OK; // Longitud válida
    }

static boolean IsImmediateDataBlock(uint16 BlockNumber)
    {
        for (uint16 i = 0; i < NUM_CONFIGURED_BLOCKS; i++)
        {
            if (Ea_BlockConfig[i].BlockNumber == BlockNumber)
            {
                return Ea_BlockConfig[i].ImmediateData;
            }
        }
        return E_NOT_OK;
    }

static uint16 Ea_GetBlockIndex(const uint16 BlockNumber)
    {
        sint32 Low = 0L;
        sint32 High = (sint32)EA_CRT_CFG_NR_OF_BLOCKS - 1;
        sint32 Middle;
        uint16 RetVal = 0xFFFFU;

        /*Using binary search to find the index of BlockNumber*/
        while (Low <= High)
        {
            Middle = Low + ((High - Low) / 2);

            if (BlockNumber < Fee_BlockConfig[Middle].BlockNumber)
            {
                /*Store High for next searching*/
                High = Middle - 1;
            }
            else if (BlockNumber > Fee_BlockConfig[Middle].BlockNumber)
            {
                /*Store Low for next searching*/
                Low = Middle + 1;
            }
            else
            {
                /*BlockNumber is matched, store the index and return*/
                RetVal = (uint16)Middle;
                break;
            }
        }
        /*RetVal will be 0xFFFFU if BlockNumber is not matched with any index*/
        return RetVal;
    }

Std_ReturnType Ea_GetPhysicalAddress(uint16 BlockNumber, uint32 *PhysicalAddr, uint16 *Size)
    {
        for (uint8 i = 0; i < EA_MAX_BLOCKS; i++)
        {
            if (Ea_ConfigTable[i].BlockNumber == BlockNumber)
            {
                *PhysicalAddr = Ea_ConfigTable[i].PhysicalAddr;
                *Size = Ea_ConfigTable[i].Size;
                return E_OK;
            }
        }
        return E_NOT_OK; // Bloque no encontrado
    }
/**
 * @brief Obtiene el AddressAreald de MemAcc para un bloque EA
 * @param BlockNumber Número de bloque lógico EA
 * @return MemAcc_AddressArealdType ID del área física
 * 
 * @note Devuelve 0 (área por defecto) si el bloque no se encuentra.
 */
MemAcc_AddressArealdType Ea_GetAddressAreaId(uint16 BlockNumber) 
{
    // Buscar el bloque en la tabla de configuración
    for (uint16 i = 0; i < EA_NUM_BLOCKS; i++) {
        if (Ea_BlockMap[i].BlockNumber == BlockNumber) 
        {
            return Ea_BlockMap[i].AddressArea;
        }
    }
}

static boolean NeedsInternalManagement(void)
{
        // Lógica para determinar si hay bloques que requieren optimización
        return (/* condición de garbage collection */);
    }

static void StartInternalJob(void)
    {
        Ea_CurrentJob.ModuleState = MEMIF_BUSY;
        Ea_CurrentJob.IsInternalJob = TRUE;
        MemAcc_Erase(INTERNAL_BLOCK); // Ejemplo: borrar bloques fragmentados
    }
/*==================================================================================================
*                                       LOCAL  PTRs
==================================================================================================*/

/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/
// ready
void Ea_Init(const Ea_ConfigType *ConfigPtr)
{
      /* Check for NULL_PTR requirement (SWS_Ea_00191) */
    if (ConfigPtr != NULL_PTR) {
#if (EA_DEV_ERROR_DETECT == STD_ON)
        Det_ReportError(EA_MODULE_ID, EA_INSTANCE_ID,  EA_INIT_SID,  EA_E_PARAM_POINTER);
#endif
        return;
    }
    
    Ea_eModuleStatus = MEMIF_BUSY_INTERNAL;
    uint32 InstanceIndex = 0u;
    Std_ReturnType RetVal = (Std_ReturnType)E_OK;

    Ea_pConfigPtr = &ConfigPtr;

    for (InstanceIndex = 0u, InstanceIndex < EA_INSTANCE_COUNT; InstanceIndex)
    {
        // EA_InitAllVariables(InstanceIndex);
        Ea_pJobRuntimeInfo[InstanceIndex].JobResult = MEMACC_MEM_OK;
        Ea_pJobRuntimeInfo[InstanceIndex].JobStatus = MEMACC_JOB_IDLE;
        Ea_pJobRuntimeInfo[InstanceIndex].JobType = MEMACC_NO_JOB;
        Ea_pJobRuntimeInfo[InstanceIndex].JobLocked = (boolean) FALSE;
    }

     /* Initialize internal management structures */
    Ea_InitBlockManagement();

    /* Initialize memory access interface */
    MemAcc_Init(NULL_PTR);

    /* Set module to IDLE state (SWS_Ea_00128) */
    Ea_eModuleStatus = MEMIF_IDLE;
    
}

// ready falta det
Std_ReturnType Ea_Write(uint16 BlockNumber, const uint16 *DataBufferPtr)
{
    uint32 physicalAddr;
    uint16 configuredLength; // Longitud configurada para el bloque

    // Verificar estado del módulo
    if (Ea_CurrentJob.ModuleState == MEMIF_UNINIT)
    {
#if (EA_DEV_ERROR_DETECT == STD_ON)
        Det_ReportError(MODULE_ID_EA, INSTANCE_ID_EA, EA_E_UNINIT);
#endif
        return E_NOT_OK;
    }

    if (Ea_CurrentJob.ModuleState == MEMIF_BUSY)
    {
#if (EA_DEV_ERROR_DETECT == STD_ON)
        Det_ReportError(MODULE_ID_EA, INSTANCE_ID_EA, EA_E_BUSY);
#endif
        return E_NOT_OK;
    }

    // Verificar parámetros (block number, offset, length, buffer)
    if (E_NOT_OK == IsValidBlockNumber(BlockNumber))
    {
#if (EA_DEV_ERROR_DETECT == STD_ON)
        Det_ReportError(MODULE_ID_EA, INSTANCE_ID_EA, EA_E_INVALID_BLOCK_NO);
#endif        
        return E_NOT_OK;
    }

    if (DataBufferPtr == NULL_PTR)
    {
#if (EA_DEV_ERROR_DETECT == STD_ON)
        Det_ReportError(MODULE_ID_EA, INSTANCE_ID_EA, EA_E_PARAM_POINTER);
#endif
        return E_NOT_OK;
    }

    /* Configure the write job */
    Std_ReturnType result;
    if (Ea_eModuleStatus == MEMIF_IDLE || MEMIF_BUSY_INTERNAL)
    {
        Ea_uJobBlockNumber = BlockNumber;

        Ee_pJobReadDataDestPtr = DataBufferPtr;

        Ea_eModuleStatus = MEMIF_BUSY;

        result = Ea_GetPhysicalAddress(BlockNumber, &physicalAddr, &configuredLength);

        if (result == E_OK)
        {
            result = MemAcc_Write(physicalAddr, DataBufferPtr); // Escribe en 0x0000F200

            Ea_eJob = MEMIF_JOB_PENDING;

            return E_OK;
        }
        else
        {
            return E_NOT_OK;
        }

         

    }  
}

// ready falta det
Std_ReturnType Ea_Read(uint16 BlockNumber, uint16 BlockOffset, uint8 *DataBufferPtr, uint16 Length)
{
    Std_ReturnType RetVal = (Std_ReturnType)E_NOT_OK;

    // Verificar estado del módulo
    if (Ea_eModuleStatus == MEMIF_UNINIT)
    {
#if (EA_DEV_ERROR_DETECT == STD_ON)
        Det_ReportError(MODULE_ID_EA, INSTANCE_ID_EA, EA_E_UNINIT);
#endif
        return E_NOT_OK;
    }

    if (Ea_eModuleStatus == MEMIF_BUSY)
    {
#if (EA_DEV_ERROR_DETECT == STD_ON)
        Det_ReportError(MODULE_ID_EA, INSTANCE_ID_EA, EA_E_BUSY);
#endif
        return E_NOT_OK;
    }

    // Verificar parámetros (block number, offset, length, buffer)
    if (E_NOT_OK == IsValidBlockNumber(BlockNumber))
    {
#if (EA_DEV_ERROR_DETECT == STD_ON)
        Det_ReportError(MODULE_ID_EA, INSTANCE_ID_EA, EA_E_INVALID_BLOCK_NO);
#endif
        return E_NOT_OK;
    }

    if (E_NOT_OK == IsValidBlockOffset(BlockNumber, BlockOffset))
    {
#if (EA_DEV_ERROR_DETECT == STD_ON)
        Det_ReportError(MODULE_ID_EA, INSTANCE_ID_EA, EA_E_INVALID_BLOCK_OFS);
#endif
        return E_NOT_OK;
    }

    if (E_NOT_OK == IsValidLength(BlockNumber, BlockOffset, Length))
    {
#if (EA_DEV_ERROR_DETECT == STD_ON)
        Det_ReportError(MODULE_ID_EA, INSTANCE_ID_EA, EA_E_INVALID_BLOCK_LEN);
#endif
        return E_NOT_OK;
    }

    if (DataBufferPtr == NULL_PTR)
    {
#if (EA_DEV_ERROR_DETECT == STD_ON)
        Det_ReportError(MODULE_ID_EA, INSTANCE_ID_EA, EA_E_PARAM_POINTER);
#endif
        return E_NOT_OK;
    }

    if (Ea_eModuleStatus == MEMIF_IDLE || MEMIF_BUSY_INTERNAL)
    {
        /* Configure the read job */
        Ea_uJobBlockNumber = BlockNumber;

        Ea_uJobBlockOffset = BlockOffset;

        Ea_uJobBlockLength = Length;

        Ee_pJobReadDataDestPtr = DataBufferPtr;

        Ea_eModuleStatus = MEMIF_BUSY;

        // Llamar al controlador de EEPROM para realizar la lectura
        MemAcc_Read(BlockNumber, BlockOffset, DataBufferPtr, Length);
    
        Ea_eJob = MEMIF_JOB_PENDING;

        RetVal = (Std_ReturnType)E_OK;

        return RetVal;
    
    }  
}

    // ready
/**
     * @brief        Service to call the cancel function of the underlying memacc
     *               driver.
     * @details      The function Fee_Cancel and the cancel function of the
     *               underlying MemAcc driver are asynchronous w.r.t. an ongoing
     *               read, erase or write job in the memacc memory.
     *
     * @pre          The module must be initialized.
     * @post         Changes Fee_eModuleStatus module status and job result
     *               Fee_eJobResult internal variables.
     * @note         The function Autosar Service ID[hex]: 0x04.
     * @note         Synchronous.
     * @note         Non Reentrant.
     *
     * @api
     *
     * @implements   Fee_Cancel_Activity
     */
void Ea_Cancel(void)
{
    // Verificar estado del módulo
    if (Ea_eModuleStatus == MEMIF_UNINIT)
    {
#if (EA_DEV_ERROR_DETECT == STD_ON)
        Det_ReportError(MODULE_ID_EA, INSTANCE_ID_EA, EA_E_UNINIT);
#endif
    }

    if (Ea_eModuleStatus == MEMIF_BUSY)
    {
#if (EA_DEV_ERROR_DETECT == STD_ON)
        Det_ReportError(MODULE_ID_EA, INSTANCE_ID_EA, EA_E_BUSY);
#endif
    }

    if (Ea_eJobResult == MEMIF_JOB_PENDING )
    {
        /* Cancel ongoing Ea job if any */
        if ( Ea_eModuleStatus == MEMIF_BUSY)
        {
            Ea_eJobResult = MEMIF_JOB_CANCELED;
            
            MemAcc_AddressArealdType areaId = Ea_GetAddressAreaId(Ea_uJobBlockNumber);

            MemAcc_Cancel(areaId);

            Ea_eModuleStatus = MEMIF_IDLE;
        }
        else
        {
#if (EA_DEV_ERROR_DETECT == STD_ON)
            Det_ReportError(MODULE_ID_EA, INSTANCE_ID_EA, EA_CANCEL_SERVICE_ID, EA_E_INVALID_CANCEL);
#endif
        }
    }
}
    // ready
    /**
     * @brief        Return the Ea module state.
     * @details      Return the Ea module state synchronously.
     *
     * @note         The function Autosar Service ID[hex]: 0x05.
     * @note         Synchronous
     * @note         Non Reentrant
     *
     * @return       Fee_eModuleStatus
     * @retval       MEMIF_UNINIT             Module has not been initialized (yet).
     * @retval       MEMIF_IDLE               Module is currently idle.
     * @retval       MEMIF_BUSY               Module is currently busy.
     * @retval       MEMIF_BUSY_INTERNAL      Module is busy with internal management
     *                                        operations.
     *
     * @api
     *
     * @implements   Fee_GetStatus_Activity
     */
MemIf_StatusType Ea_GetStatus(void)
{
    // Verificar estado del módulo
    if(Ea_eModuleStatus == MEMIF_UNINIT)
    {
        return MEMIF_UNINIT;
    }

    // Verificar estado del módulo
    if (Ea_eModuleStatus == MEMIF_IDLE)
    {
        return MEMIF_IDLE;
    }

    if (Ea_eModuleStatus == MEMIF_BUSY)
    {
        return MEMIF_BUSY;
    }

    if (Ea_eModuleStatus == MEMIF_BUSY_INTERNAL)
    {
        return MEMIF_BUSY_INTERNAL;
    }
}

    // ready
    /**
     * @brief        Return the result of the last job.
     * @details      Return the result of the last job synchronously.
     *
     * @return       MemIf_JobResultType
     * @retval       MEMIF_JOB_OK              The job has been finished successfully.
     * @retval       MEMIF_JOB_FAILED          The job has not been finished
     *                                         successfully.
     * @retval       MEMIF_JOB_PENDING         The job has not yet been finished.
     * @retval       MEMIF_JOB_CANCELED        The job has been canceled.
     * @retval       MEMIF_BLOCK_INCONSISTENT  The requested block is inconsistent,
     *                                         it may contain corrupted data.
     * @retval       MEMIF_BLOCK_INVALID       The requested block has been invalidated,
     *                                         the requested read operation can not be performed.
     * @note         The function Autosar Service ID[hex]: 0x06.
     * @note         Synchronous.
     * @note         Non Reentrant.
     *
     * @api
     *
     * @implements   Ea_GetJobResult_Activity
     */
MemIf_JobResultType Ea_GetJobResult(void)
{
    // Verificar estado del módulo
    if (Ea_eModuleStatus == MEMIF_UNINIT)
    {
#if (EA_DEV_ERROR_DETECT == STD_ON)
         Det_ReportError(MODULE_ID_EA, INSTANCE_ID_EA, EA_E_UNINIT);
#endif
         return MEMIF_JOB_FAILED;
    }

    return Ea_eJobResult;
}

    //ready
Std_ReturnType Ea_InvalidateBlock(uint16 BlockNumber)
{
    Std_ReturnType RetVal = (Std_ReturnType)E_OK;
    
     if (Ea_eModuleStatus == MEMIF_UNINIT)
    {
#if (EA_DEV_ERROR_DETECT == STD_ON)
        Det_ReportError(MODULE_ID_EA, INSTANCE_ID_EA, EA_E_UNINIT);
#endif
        return E_NOT_OK;
    }

     if (Ea_eModuleStatus == MEMIF_BUSY)
    {
#if (EA_DEV_ERROR_DETECT == STD_ON)
        Det_ReportError(MODULE_ID_EA, INSTANCE_ID_EA, EA_E_BUSY);
#endif
        return E_NOT_OK;
    }
    
    uint16 BlockIndex;
    uint32 physicalAddr;
    uint16 configuredLength; // Longitud configurada para el bloque

     if (Ea_eModuleStatus == MEMIF_IDLE || MEMIF_BUSY_INTERNAL)
    {
        BlockIndex = Ea_GetPhysicalAddress(BlockNumber, &physicalAddr, &configuredLength);

        Ea_uJobBlockIndex = BlockIndex;

        Ea_eModuleStatus = MEMIF_BUSY;

        Ea_eJobResult = MEMIF_JOB_PENDING;

        RetVal = (Std_ReturnType)E_OK;

        return RetVal;
    }
    else
    {
#if (EA_DEV_ERROR_DETECT == STD_ON)
         Det_ReportError(MODULE_ID_EA, INSTANCE_ID_EA, EA_CANCEL_SERVICE_ID, EA_E_INVALID_BLOCK_NO);
#endif
         return E_NOT_OK
    }
}

    // ready
void Ea_GetVersionInfo(Std_VersionInfoType *VersionInfoPtr)
{
    VersionInfoPtr->moduleID = (uint16)EA_MODULE_ID;
    VersionInfoPtr->vendorID = (uint16)EA_VENDOR_ID;
    VersionInfoPtr->sw_major_version = (uint8)EA_SW_MAJOR_VERSION;
    VersionInfoPtr->sw_minor_version = (uint8)EA_SW_MINOR_VERSION;
    VersionInfoPtr->sw_patch_version = (uint8)EA_SW_PATCH_VERSION;
}

    // ready
Std_ReturnType Ea_EraseImmediateBlock(uint16 BlockNumber)
{
    Std_ReturnType RetVal = (Std_ReturnType)E_OK;

    // --- 1. Validar estado del módulo ---
    if (Ea_eModuleStatus == MEMIF_UNINIT)
    {
#if (EA_DEV_ERROR_DETECT == STD_ON)
            Det_ReportError(MODULE_ID_EA, INSTANCE_ID_EA, EA_E_UNINIT);
#endif
        return E_NOT_OK;
    }

    if (Ea_eModuleStatus == MEMIF_BUSY)
    {
#if (EA_DEV_ERROR_DETECT == STD_ON)
         Det_ReportError(MODULE_ID_EA, INSTANCE_ID_EA, EA_E_BUSY);
#endif
        return E_NOT_OK;
    }

    // Verificar parámetros (block number)
    if (E_NOT_OK == IsValidBlockNumber(BlockNumber))
    {
#if (EA_DEV_ERROR_DETECT == STD_ON)
         Det_ReportError(MODULE_ID_EA, INSTANCE_ID_EA, EA_E_INVALID_BLOCK_NO);
#endif
        return E_NOT_OK;
    }

    // --- 3. Validar que el bloque sea "datos inmediatos" ---
    if (E_NOT_OK == IsImmediateDataBlock(BlockNumber))
    {
#if (EA_DEV_ERROR_DETECT == STD_ON)
        Det_ReportError(MODULE_ID_EA, INSTANCE_ID_EA, EA_E_INVALID_BLOCK_NO);
#endif
        return E_NOT_OK;
    }

    /* Configure the erase immediate block job */
    Ea_uJobBlockIndex = BlockNumber;
    Ea_eModuleStatus = MEMIF_BUSY;
    

    // --- 5. Llamar al driver de bajo nivel (ejemplo: MemAcc_Erase) ---
    MemAcc_Erase(BlockNumber);

    RetVal = (Std_ReturnType)E_OK;
}

// not ready. on off notification
void Ea_JobEndNotification(void)
    {
        Std_ReturnType RetVal = (Std_ReturnType)E_OK;

        // --- 2. Obtener resultado del trabajo desde MemAcc ---
        MemAcc_JobResultType memAccResult = MemAcc_GetJobResult();

        // --- 3. Actualizar estado del módulo EA ---
        switch (memAccResult)
        {
        case MEMACC_JOB_OK:
            //SWS_Ea_00153
            Ea_eJob = MEMIF_JOB_PENDING;
            break;
        case MEMACC_JOB_FAILED:
            Ea_eJob = MEMIF_JOB_FAILED;
            break;
        case MEMACC_JOB_CANCELED:
            Ea_eJob = MEMIF_JOB_CANCELED;
            break;
        default:
            Ea_eJob = MEMIF_JOB_FAILED;
            break;
        }

        
 // is notification on?
#if( Ea_NvMJobNotificaction == STD_ON)
        // --- 4. Notificar al NVRAM Manager (NvM) ---
        if (Ea_eJob == MEMIF_JOB_OK)
        {
            if (Ea_NvMJobEndNotification != NULL)
            {
                Ea_NvMJobEndNotification(); // Éxito
            }
        }
        else
        {
            if (Ea_NvMJobErrorNotification != NULL)
            {
                Ea_NvMJobErrorNotification(); // Fallo o cancelación
            }
        }
#endif /* Ea_NvMJobNotification */

        // --- 5. Cambiar estado a "IDLE" ---
        Ea_eModuleStatus = MEMIF_IDLE;
    }

// falta
    /**
     * @brief        Service to handle the requested read, write, or erase jobs
     *               respectively the internal management operations.
     * @details      The function shall asynchronously handle the requested
     *               read, write, or erase jobs respectively the internal management operations.
     *               The function shall check, whether
     *               the block requested for reading has been invalidated by the
     *               upper layer module.
     *               If so, the function shall set the job result to
     *               MEMIF_BLOCK_INVALID and call the error notification routine
     *               of the upper layer if configured.
     *               The function shall check the consistency of the logical block
     *               being read before notifying the caller. If an inconsistency
     *               of the read data is detected, the function
     *               shall set the job result to MEMIF_BLOCK_INCONSISTENT and call
     *               the error notification routine of the upper layer.
     * @pre          The module must be initialized.
     * @note         The function Autosar Service ID[hex]: 0x12.
     *
     * @api
     *
     * @implements   Ea_MainFunction_Activity
     */
void Ea_MainFunction(void)
{   
    //SWS_Ea_00178
    if ( Ea_eModuleStatus == MEMIF_BUSY_INTERNAL )
    {
        Ea_eModuleStatus == MEMIF_IDLE;
    }

    //SWS_Ea_00104
    if (E_NOT_OK == IsImmediateDataBlock(Ea_uJobBlockIndex))
    {
        
        
        return E_NOT_OK;
    }

    // --- 2. Obtener estado del driver EEPROM (MemAcc) ---
    MemAcc_JobResultType memAccResult = MemAcc_GetJobResult();

    // --- 3. Procesar resultado ---
    switch (memAccResult)
    {
    case MEMACC_JOB_PENDING:
        // Operación aún en progreso, no hacer nada
        break;

    case MEMACC_JOB_OK:
        Ea_CurrentJob.JobResult = MEMIF_JOB_OK;
        Ea_CurrentJob.ModuleState = MEMIF_IDLE;
        if (!Ea_CurrentJob.IsInternalJob)
        {
#if( Ea_NvMJobNotificaction == STD_ON)
        // Notificar solo si es trabajo externo (NvM)
            if (Ea_NvMJobEndNotification != NULL)
            {
                 Ea_NvMJobEndNotification();
            }
#endif
        }
        break;

    case MEMACC_JOB_FAILED:
        Ea_CurrentJob.JobResult = MEMIF_JOB_FAILED;
        Ea_CurrentJob.ModuleState = MEMIF_IDLE;
        if (!Ea_CurrentJob.IsInternalJob)
        {
#if( Ea_NvMJobNotificaction == STD_ON)
            if (Ea_NvMJobErrorNotification != NULL)
            {
                Ea_NvMJobErrorNotification();
            }
#endif
        }
        break;

    case MEMACC_JOB_CANCELED:
        Ea_CurrentJob.JobResult = MEMIF_JOB_CANCELED;
        Ea_CurrentJob.ModuleState = MEMIF_IDLE;
        if (!Ea_CurrentJob.IsInternalJob)
        {   
#if( Ea_NvMJobNotificaction == STD_ON)
            if (Ea_NvMJobErrorNotification != NULL)
            {
                Ea_NvMJobErrorNotification();
            }
#endif
        }
        break;

    default:
        // Manejo de errores no esperados
        Ea_CurrentJob.JobResult = MEMIF_JOB_FAILED;
        Ea_CurrentJob.ModuleState = MEMIF_IDLE;
        break;
    }

    // --- 4. Ejecutar gestión interna (ej. garbage collection) si no hay trabajos externos ---
    if (Ea_CurrentJob.ModuleState == MEMIF_IDLE && NeedsInternalManagement())
    {
        StartInternalJob(); // Función que inicia trabajos internos
    }
}

#ifdef __cplusplus
}
#endif