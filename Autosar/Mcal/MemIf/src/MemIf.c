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
#include "Fee.h"
#include "MemIf.h"

    /*==================================================================================================
     *                                      GLOBAL VARIABLES
    ==================================================================================================*/
    /*==================================================================================================
     *                           LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
    ==================================================================================================*/

    /*==================================================================================================
                                           LOCAL VARIABLES
    ==================================================================================================*/
#if (MEMIF_NUMBER_OF_DEVICES == 2)

    typedef Std_ReturnType (*MemIf_WriteFctType)(uint16 BlockNumber, const uint8 *DataBufferPtr);
    typedef Std_ReturnType (*MemIf_ReadFctType)(uint16 BlockNumber, uint16 BlockOffset, uint8∗ DataBufferPtr, uint16 Length);
    typedef Std_ReturnType (*MemIf_CancelFctType)(void);
    typedef Std_ReturnType (*MemIf_GetStatusFctType)(void);
    typedef Std_ReturnType (*MemIf_GetJobResultFctType)(void);
    typedef Std_ReturnType (*MemIf_InvalidateBlockFctType)(uint16 BlockNumber);
    typedef Std_ReturnType (*MemIf_GetVersionInfoFctType)(uint16 BlockNumber);
    typedef Std_ReturnType (*MemIf_EraseImmediateBlockFctType)(uint16 BlockNumber);

#endif
    /*==================================================================================================
                                           LOCAL CONSTANTS
    ==================================================================================================*/
    /*==================================================================================================
    *                                       LOCAL  PTRs
    ==================================================================================================*/

#if (MEMIF_NUMBER_OF_DEVICES == 2)
    /* Definir punteros a las funciones de los módulos subyacentes */
    static MemIf_ReadFctType MemIf_ReadFctPtr[] = {
        &Fee_Read,
        //&Ea_Read,
    };

    static MemIf_WriteFctType MemIf_WriteFctPtr[] = {
        &Fee_Write, /* Si DeviceIndex es 0, usa FEE */
        //&Ea_Write, /* Si DeviceIndex es 1, usa Ea */
    };

    static MemIf_CancelFctType MemIf_CancelFctPtr[] = {
        &Fee_Cancel,
        //&Ea_Cancel
    };

    static MemIf_GetStatusFctType MemIf_GetStatusFctPtr[] = {
        &Fee_GetStatus
        // &Ea_GetStatus
    };

    static MemIf_GetJobResultFctType MemIf_GetJobResultFctPtr[] = {
        &Fee_GetJobResult,
        //&Ea_GetJobResult
    };

    static MemIf_InvalidateBlockFctType MemIf_InvalidateBlockFctPtr[] = {
        &Fee_InvalidateBlock
        //&Ea_InvalidateBlock
    };

    // MemIf_GetVersionInfo

    static MemIf_EraseImmediateBlockFctType MemIf_EraseImmediateBlockFctPtr[] = {
        &Fee_EraseImmediateBlock
        //&Ea_EraseImmediateBlock
    };

#endif

/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/
#if (MEMIF_NUMBER_OF_DEVICES == 2)

    Std_ReturnType MemIf_Read(uint16 DeviceIndex, uint16 BlockNumber, uint16 BlockOffset, uint8 *DataBufferPtr, uint16 Length)
    {

        if (DeviceIndex >= MEMIF_NUMBER_OF_DEVICES - 1)
        {
            /* Manejo de error si el índice está fuera del rango */
            return E_NOT_OK;
        }

        return MemIf_ReadFctPtr[DeviceIndex](BlockNumber, BlockOffset, DataBufferPtr, Length);
    }


    Std_ReturnType MemIf_Write(uint16 DeviceIndex, uint16 BlockNumber, const uint8 *DataBufferPtr)
    {
        if (DeviceIndex >= MEMIF_NUMBER_OF_DEVICES - 1)
        {
            /* Manejo de error si el índice está fuera del rango */
            return E_NOT_OK;
        }

        return MemIf_WriteFctPtr[DeviceIndex](BlockNumber, DataBufferPtr);
    }

 
    void MemIf_Cancel(uint16 DeviceIndex)
    {
        MemIf_CancelFctPtr[DeviceIndex]();
    }


    MemIf_StatusType MemIf_GetStatus(uint16 DeviceIndex)
    {
        return MemIf_GetStatusFctPtr[DeviceIndex]();
    }

   
    MemIf_JobResultType MemIf_GetJobResult(uint16 DeviceIndex)
    {
        return MemIf_GetJobResultFctPtr[DeviceIndex]();
    }

 
    Std_ReturnType MemIf_InvalidateBlock(uint16 DeviceIndex, uint16 BlockNumber)
    {
        if (DeviceIndex >= MEMIF_NUMBER_OF_DEVICES - 1)
        {
            /* Manejo de error si el índice está fuera del rango */
            return E_NOT_OK;
        }

        return MemIf_InvalidateBlocFctkPtr[DeviceIndex](BlockNumber);
    }

    /**
     * @brief        The function to erase a logical block.
     *
     * @details      Invokes the "EraseImmediateBlock"" function of the underlying memory abstraction module selected
                    by the parameter DeviceIndex.
     *
     * @param[in]    DeviceIndex    Number of driver device
     * @param[in]    BlockNumber    Number of block to read
     *
     * @return        E_NOT_OK else it shall
                    return the value of thse called function of the underlying module.
     * @api
    */
    Std_ReturnType MemIf_EraseImmediateBlock(uint16 DeviceIndex, uint16 BlockNumber)
    {
        if (DeviceIndex >= MEMIF_NUMBER_OF_DEVICES - 1)
        {
            /* Manejo de error si el índice está fuera del rango */
            return E_NOT_OK;
        }

        return MemIf_EraseImmediateBlockFctPtr[DeviceIndex](BlockNumber);
    }
#endif // MEMIF_NUMBER_OF_DEVICES > 1

    /**
     * @brief        Returns version information
     *
     * @param[out]    VersionInfoPtr    Pointer to standard version information structure.
     * @api
     */
    void MemIf_GetVersionInfo(Std_VersionInfoType *VersionInfoPtr)
    {
        VersionInfoPtr->moduleID = (uint16)MEMIF_MODULE_ID;
        VersionInfoPtr->vendorID = (uint16)MEMIF_VENDOR_ID;
        VersionInfoPtr->sw_major_version = (uint8)MEMIF_SW_MAJOR_VERSION;
        VersionInfoPtr->sw_minor_version = (uint8)MEMIF_SW_MINOR_VERSION;
        VersionInfoPtr->sw_patch_version = (uint8)MEMIF_SW_PATCH_VERSION;
    }

#ifdef __cplusplus
}
#endif
