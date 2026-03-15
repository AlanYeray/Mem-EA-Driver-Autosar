/*==================================================================================================
*   Project              : RTD AUTOSAR 4.7
*   Platform             : CORTEXM
*   Peripheral           : IPV_FTFC
*   Dependencies         : 
*
*   Autosar Version      : 4.7.0
*   Autosar Revision     : ASR_REL_4_7_REV_0000
*   Autosar Conf.Variant :
*   SW Version           : 2.0.0
*   Build Version        : S32K1_RTD_2_0_0_P04_D2404_ASR_REL_4_7_REV_0000_20240417
*
*   Copyright 2020-2024 NXP Semiconductors
*
*   NXP Confidential. This software is owned or controlled by NXP and may only be
*   used strictly in accordance with the applicable license terms. By expressly
*   accepting such terms or by downloading, installing, activating and/or otherwise
*   using the software, you are agreeing that you have read, and that you agree to
*   comply with and are bound by, such license terms. If you do not agree to be
*   bound by the applicable license terms, then you may not retain, install,
*   activate or otherwise use the software.
==================================================================================================*/

/**
*   @file Ftfc_Mem_Eep_Ip.c
*
*   @addtogroup FTFC_MEM_EEP_IP
*   @{
*/

#ifdef __cplusplus
extern "C"{
#endif

/*==================================================================================================
*                                          INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/

#include "OsIf.h"
#include "Mcal.h"
#include "SchM_Mem_43_EEP.h"
#include "Ftfc_Mem_Eep_Ip.h"

/*==================================================================================================
*                                 SOURCE FILE VERSION INFORMATION
==================================================================================================*/

#define FTFC_MEM_EEP_IP_VENDOR_ID_C                      43
#define FTFC_MEM_EEP_IP_AR_RELEASE_MAJOR_VERSION_C       4
#define FTFC_MEM_EEP_IP_AR_RELEASE_MINOR_VERSION_C       7
#define FTFC_MEM_EEP_IP_AR_RELEASE_REVISION_VERSION_C    0
#define FTFC_MEM_EEP_IP_SW_MAJOR_VERSION_C               2
#define FTFC_MEM_EEP_IP_SW_MINOR_VERSION_C               0
#define FTFC_MEM_EEP_IP_SW_PATCH_VERSION_C               0

/*==================================================================================================
*                                       FILE VERSION CHECKS
==================================================================================================*/
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Version checks for OsIf.h */
    #if ((FTFC_MEM_EEP_IP_AR_RELEASE_MAJOR_VERSION_C != OSIF_AR_RELEASE_MAJOR_VERSION) || \
        (FTFC_MEM_EEP_IP_AR_RELEASE_MINOR_VERSION_C != OSIF_AR_RELEASE_MINOR_VERSION) \
        )
        #error "Ftfc_Mem_Eep_Ip.c and OsIf.h are for different AUTOSAR versions!"
    #endif
#endif

#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Version checks for Mcal.h */
    #if ((FTFC_MEM_EEP_IP_AR_RELEASE_MAJOR_VERSION_C != MCAL_AR_RELEASE_MAJOR_VERSION) || \
        (FTFC_MEM_EEP_IP_AR_RELEASE_MINOR_VERSION_C != MCAL_AR_RELEASE_MINOR_VERSION) \
        )
        #error "Ftfc_Mem_Eep_Ip.c and Mcal.h are for different AUTOSAR versions!"
    #endif
#endif

#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Version checks for "SchM_Mem_43_EEP.h" */
    #if ((FTFC_MEM_EEP_IP_AR_RELEASE_MAJOR_VERSION_C != SCHM_MEM_43_EEP_AR_RELEASE_MAJOR_VERSION) || \
        (FTFC_MEM_EEP_IP_AR_RELEASE_MINOR_VERSION_C != SCHM_MEM_43_EEP_AR_RELEASE_MINOR_VERSION) \
        )
        #error "Ftfc_Mem_Eep_Ip.c and "SchM_Mem_43_EEP.h" are for different AUTOSAR versions!"
    #endif
#endif

/* Version checks for Ftfc_Mem_Eep_Ip.h */
#if (FTFC_MEM_EEP_IP_VENDOR_ID_C != FTFC_MEM_EEP_IP_VENDOR_ID_H)
    #error "Ftfc_Mem_Eep_Ip.c and Ftfc_Mem_Eep_Ip.h have different vendor IDs!"
#endif
#if ((FTFC_MEM_EEP_IP_AR_RELEASE_MAJOR_VERSION_C    != FTFC_MEM_EEP_IP_AR_RELEASE_MAJOR_VERSION_H) || \
     (FTFC_MEM_EEP_IP_AR_RELEASE_MINOR_VERSION_C    != FTFC_MEM_EEP_IP_AR_RELEASE_MINOR_VERSION_H) || \
     (FTFC_MEM_EEP_IP_AR_RELEASE_REVISION_VERSION_C != FTFC_MEM_EEP_IP_AR_RELEASE_REVISION_VERSION_H) \
    )
    #error "Ftfc_Mem_Eep_Ip.c and Ftfc_Mem_Eep_Ip.h are for different AUTOSAR versions!"
#endif
#if ((FTFC_MEM_EEP_IP_SW_MAJOR_VERSION_C != FTFC_MEM_EEP_IP_SW_MAJOR_VERSION_H) || \
     (FTFC_MEM_EEP_IP_SW_MINOR_VERSION_C != FTFC_MEM_EEP_IP_SW_MINOR_VERSION_H) || \
     (FTFC_MEM_EEP_IP_SW_PATCH_VERSION_C != FTFC_MEM_EEP_IP_SW_PATCH_VERSION_H) \
    )
    #error "Ftfc_Mem_Eep_Ip.c and Ftfc_Mem_Eep_Ip.h have different SW versions!"
#endif

/*==================================================================================================
*                                          LOCAL MACROS
==================================================================================================*/

/** Switch FlexRAM function between RAM and emulated EEPROM. */
#define FTFC_MEM_EEP_IP_FLASH_CMD_SETRAM_U8         0x81U

#if (FTFC_MEM_EEP_IP_QUICK_WRITE == STD_ON) 
/** Minimum number of bytes allowed for a quick write. */
#define FTFC_MEM_EEP_IP_QUICK_WRITES_LENGTH_MIN     16U

/** Maximum number of bytes allowed for a quick write. */
#define FTFC_MEM_EEP_IP_QUICK_WRITES_LENGTH_MAX     512U
#endif

#define FTFC_MEM_EEP_IP_NO_BO_DETECTED_U8               0x00    /**< No EEPROM issues detected */
#define FTFC_MEM_EEP_IP_BO_DURING_MAINTENANCE_U8        0x01    /**< Quick write maintenance has to be completed. */
#define FTFC_MEM_EEP_IP_BO_DURING_QUICK_WRITES_U8       0x02    /**< Quick writes were discarded due to a reset. */
#define FTFC_MEM_EEP_IP_BO_DURING_NORMAL_WRITES_U8      0x04    /**< A normal write was interrupted by reset. */

/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

/** FCCOB registers and their position in the IP_EEPROM->FCCOB[] array */
enum
{
    FTFC_MEM_EEP_IP_FCCOB_3,    FTFC_MEM_EEP_IP_FCCOB_2,    FTFC_MEM_EEP_IP_FCCOB_1,    FTFC_MEM_EEP_IP_FCCOB_0,
    FTFC_MEM_EEP_IP_FCCOB_7,    FTFC_MEM_EEP_IP_FCCOB_6,    FTFC_MEM_EEP_IP_FCCOB_5,    FTFC_MEM_EEP_IP_FCCOB_4,
    FTFC_MEM_EEP_IP_FCCOB_11,   FTFC_MEM_EEP_IP_FCCOB_10,   FTFC_MEM_EEP_IP_FCCOB_9,    FTFC_MEM_EEP_IP_FCCOB_8
};

/** Input params written in the FCCOB registers for the flash command: Set FlexRAM Function */
enum
{
    FTFC_MEM_EEP_IP_FCCOB_SETRAM_WRITE_CMD_SETRAM               = FTFC_MEM_EEP_IP_FCCOB_0,    /**< Flash command: SETRAM (0x81) */
    FTFC_MEM_EEP_IP_FCCOB_SETRAM_WRITE_FR_FCC                   = FTFC_MEM_EEP_IP_FCCOB_1,    /**< FlexRAM Function Control Code */
    FTFC_MEM_EEP_IP_FCCOB_SETRAM_WRITE_QUICKWRITES_LENGTH_MSB   = FTFC_MEM_EEP_IP_FCCOB_4,    /**< Number of FlexRAM bytes allocated for EEPROM quick writes [15:8] */
    FTFC_MEM_EEP_IP_FCCOB_SETRAM_WRITE_QUICKWRITES_LENGTH_LSB   = FTFC_MEM_EEP_IP_FCCOB_5     /**< Number of FlexRAM bytes allocated for EEPROM quick writes [7:0] */
};

/** Returned values read from the FCCOB registers for the flash command: Set FlexRAM Function */
enum
{
    FTFC_MEM_EEP_IP_FCCOB_SETRAM_READ_BROWNOUT_CODE             = FTFC_MEM_EEP_IP_FCCOB_5,    /**< Brown-out (BO) Detection Codes */
    FTFC_MEM_EEP_IP_FCCOB_SETRAM_READ_QWRITES_MAINTENANCE_MSB   = FTFC_MEM_EEP_IP_FCCOB_6,    /**< Number of EEPROM quick write records requiring maintenance [15:8] */
    FTFC_MEM_EEP_IP_FCCOB_SETRAM_READ_QWRITES_MAINTENANCE_LSB   = FTFC_MEM_EEP_IP_FCCOB_7,    /**< Number of EEPROM quick write records requiring maintenance [7:0] */
    FTFC_MEM_EEP_IP_FCCOB_SETRAM_READ_SECTOR_ERASE_COUNT_MSB    = FTFC_MEM_EEP_IP_FCCOB_8,    /**< EEPROM sector erase count [15:8] */
    FTFC_MEM_EEP_IP_FCCOB_SETRAM_READ_SECTOR_ERASE_COUNT_LSB    = FTFC_MEM_EEP_IP_FCCOB_9     /**< EEPROM sector erase count [7:0] */
};

/** FLEXRAM Function Control Codes */
typedef enum
{
    FLEXRAM_FCC_MODE_EEERAM_NORMAL      = 0x00,     /**< Make FlexRAM available for emulated EEPROM */
    FLEXRAM_FCC_MODE_EEERAM_QWRITES     = 0x55,     /**< Make FlexRAM available for EEPROM quick writes */
    FLEXRAM_FCC_GET_FLEXRAM_STATUS      = 0x77,     /**< EEPROM quick write status query */
    FLEXRAM_FCC_COMPLETE_MAINTENANCE    = 0xAA,     /**< Complete interrupted EEPROM quick write process */
    FLEXRAM_FCC_MODE_RAM                = 0xFF      /**< Make FlexRAM available as RAM */
} Ftfc_Mem_Eep_Ip_FlexramFccType;

/** Data about the last asynchronous job */
typedef struct
{
    /** updated at the beginning of a new async job with OsIf_GetCounter() and with each OsIf_GetElapsed() call */
    uint32 TicksStarted;
    /** here OsIf_GetElapsed() stores the time elapsed since TicksStarted */
    uint32 TicksElapsed;
    /** the status of the last asynchronous job may be: ok, pending or failed */
    Ftfc_Mem_Eep_Ip_StatusType Result;
} Ftfc_Mem_Eep_Ip_AsyncJobType;

/*==================================================================================================
*                                        GLOBAL CONSTANTS
==================================================================================================*/

#define MEM_43_EEP_START_SEC_CONST_32
#include "Mem_43_EEP_MemMap.h"

    /** The contents of an erased EEPROM record.
     *  This variable needs to be aligned on LONGWORD bytes. */
    static uint32 const Ftfc_Mem_Eep_Ip_u32EraseValue = 0xFFFFFFFFU;

#define MEM_43_EEP_STOP_SEC_CONST_32
#include "Mem_43_EEP_MemMap.h"

/*==================================================================================================
*                                        GLOBAL VARIABLES
==================================================================================================*/

/*==================================================================================================
*                                         LOCAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                         LOCAL VARIABLES
==================================================================================================*/

#define MEM_43_EEP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Mem_43_EEP_MemMap.h"

    static Ftfc_Mem_Eep_Ip_ConfigType const * Ftfc_Mem_Eep_Ip_pxConfiguration;
    static Ftfc_Mem_Eep_Ip_BrownOutCodeType Ftfc_Mem_Eep_Ip_eBrownOutCode;
    static Ftfc_Mem_Eep_Ip_AsyncJobType Ftfc_Mem_Eep_Ip_xAsyncJob;

#define MEM_43_EEP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Mem_43_EEP_MemMap.h"

/*==================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/

#define MEM_43_EEP_START_SEC_CODE
#include "Mem_43_EEP_MemMap.h"

    static Ftfc_Mem_Eep_Ip_PageSizeType Ftfc_Mem_Eep_Ip_AlignedPageSize (Ftfc_Mem_Eep_Ip_AddressType FlexramAddress,
                                                                 Ftfc_Mem_Eep_Ip_AddressType RamAddress,
                                                                 Ftfc_Mem_Eep_Ip_LengthType Length
                                                                );

    static boolean Ftfc_Mem_Eep_Ip_IsFtfcReady (void);

    static Ftfc_Mem_Eep_Ip_StatusType Ftfc_Mem_Eep_Ip_CmdSetFlexramFunction (Ftfc_Mem_Eep_Ip_FlexramFccType FlexramFuncControlCode,
                                                                     uint16 QuickWritesLength
                                                                    );

    static Ftfc_Mem_Eep_Ip_StatusType Ftfc_Mem_Eep_Ip_CheckFstatErrors (void);

    static Ftfc_Mem_Eep_Ip_StatusType Ftfc_Mem_Eep_Ip_AcEflashCmd (void);

    static Ftfc_Mem_Eep_Ip_StatusType Ftfc_Mem_Eep_Ip_AcEflashWrite (Ftfc_Mem_Eep_Ip_AddressType DestAddress,
                                                             uint8 const * pu8SrcAddress,
                                                             Ftfc_Mem_Eep_Ip_PageSizeType PageSize,
                                                             boolean Async
                                                            );

#define MEM_43_EEP_STOP_SEC_CODE
#include "Mem_43_EEP_MemMap.h"


#if (FTFC_MEM_EEP_IP_LOAD_AC_INTO_RAM == STD_ON)
#define MEM_43_EEP_START_SEC_RAMCODE
#else
#define MEM_43_EEP_START_SEC_CODE
#endif
#include "Mem_43_EEP_MemMap.h"

    static Ftfc_Mem_Eep_Ip_StatusType Ftfc_Mem_Eep_Ip_StartFlashCommand (void);

    static Ftfc_Mem_Eep_Ip_StatusType Ftfc_Mem_Eep_Ip_WriteIntoFlexram (Ftfc_Mem_Eep_Ip_AddressType DestAddress,
                                                                uint8 const * pu8SrcAddress,
                                                                Ftfc_Mem_Eep_Ip_PageSizeType PageSize,
                                                                boolean Async
                                                               );

    static Ftfc_Mem_Eep_Ip_StatusType Ftfc_Mem_Eep_Ip_WaitCcifTimeout (uint32 TimeoutCounter);

#if (FTFC_MEM_EEP_IP_LOAD_AC_INTO_RAM == STD_ON)
#define MEM_43_EEP_STOP_SEC_RAMCODE
#else
#define MEM_43_EEP_STOP_SEC_CODE
#endif
#include "Mem_43_EEP_MemMap.h"

/*==================================================================================================
*                                        GLOBAL FUNCTIONS
==================================================================================================*/

#define MEM_43_EEP_START_SEC_CODE
#include "Mem_43_EEP_MemMap.h"

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @brief   Initialize the module.
 * @details Set FlexRAM to work as EEERAM.
 *          Query EEPROM quick write status and complete the maintenance, if needed.
 *
 * Out of reset with the FSTAT[CCIF] bit clear, the partition settings (EEESIZE, DEPART)
 * are read from the data flash IFR and the emulated EEPROM file system is initialized
 * accordingly. The emulated EEPROM file system locates all valid EEPROM data records
 * in EEPROM backup and copies the newest data to FlexRAM.
 *
 * CCIF is cleared throughout the reset sequence. Completion of the reset sequence is marked by
 * setting CCIF which enables flash user commands.
 *
 * @param[in]  pConfig pointer stored in Ftfc_Mem_Eep_Ip_pxConfiguration
 *
 * @return the initialization result
 * @retval FTFC_MEM_EEP_IP_STATUS_OK             initialization successful
 * @retval FTFC_MEM_EEP_IP_STATUS_TIMEOUT        a flash cmd timeout has occurred
 * @retval FTFC_MEM_EEP_IP_STATUS_FAILED         a flash command failed to execute
 * @retval FTFC_MEM_EEP_IP_STATUS_FAILED_MGSTAT  one or more MGSTAT 1/2/3 bits were set
 *
 * @implements Ftfc_Mem_Eep_Ip_Init_Activity
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
Ftfc_Mem_Eep_Ip_StatusType Ftfc_Mem_Eep_Ip_Init (Ftfc_Mem_Eep_Ip_ConfigType const * pConfig)
{
    Ftfc_Mem_Eep_Ip_StatusType Status = FTFC_MEM_EEP_IP_STATUS_OK;
    uint8 BrownoutCode = 0U;

    FTFC_DEV_ASSERT(NULL_PTR != pConfig);

    /* Overwrite all global variables */
    Ftfc_Mem_Eep_Ip_pxConfiguration = pConfig;
    Ftfc_Mem_Eep_Ip_eBrownOutCode = FTFC_MEM_EEP_IP_NO_BO_DETECTED;
    Ftfc_Mem_Eep_Ip_xAsyncJob.TicksStarted = 0U;
    Ftfc_Mem_Eep_Ip_xAsyncJob.TicksElapsed = 0U;
    Ftfc_Mem_Eep_Ip_xAsyncJob.Result = FTFC_MEM_EEP_IP_STATUS_OK;

    if (0U == (IP_EEPROM->FCNFG & FCNFG_EEERDY_MASK))
    {
        /* When the memory partitioning is done without the option to load the EEPROM data
         * to FLEXRAM at reset, a FLEXRAM_FCC_MODE_EEERAM_NORMAL command is needed (so EEERDy gets set).
         * Also, the FLEXRAM_FCC_GET_FLEXRAM_STATUS cmd is not allowed unless the EEERDY bit is set. */
        Status = Ftfc_Mem_Eep_Ip_CmdSetFlexramFunction(FLEXRAM_FCC_MODE_EEERAM_NORMAL, 0U);
    }

    if (FTFC_MEM_EEP_IP_STATUS_OK == Status)
    {
        Status = Ftfc_Mem_Eep_Ip_CmdSetFlexramFunction(FLEXRAM_FCC_GET_FLEXRAM_STATUS, 0U);
        if (FTFC_MEM_EEP_IP_STATUS_OK == Status)
        {
            BrownoutCode = IP_EEPROM->FCCOB[FTFC_MEM_EEP_IP_FCCOB_SETRAM_READ_BROWNOUT_CODE];
            switch (BrownoutCode)
            {
                case FTFC_MEM_EEP_IP_BO_DURING_MAINTENANCE_U8:
                    Ftfc_Mem_Eep_Ip_eBrownOutCode = FTFC_MEM_EEP_IP_BO_DURING_MAINTENANCE;
                break;

                case FTFC_MEM_EEP_IP_BO_DURING_QUICK_WRITES_U8:
                    Ftfc_Mem_Eep_Ip_eBrownOutCode = FTFC_MEM_EEP_IP_BO_DURING_QUICK_WRITES;
                break;

                case FTFC_MEM_EEP_IP_BO_DURING_NORMAL_WRITES_U8:
                    Ftfc_Mem_Eep_Ip_eBrownOutCode = FTFC_MEM_EEP_IP_BO_DURING_NORMAL_WRITES;
                break;

                case FTFC_MEM_EEP_IP_NO_BO_DETECTED_U8:
                    Ftfc_Mem_Eep_Ip_eBrownOutCode = FTFC_MEM_EEP_IP_NO_BO_DETECTED;
                break;

                default:
                    Status = FTFC_MEM_EEP_IP_STATUS_FAILED;
                break;
            }

            if (FTFC_MEM_EEP_IP_NO_BO_DETECTED != Ftfc_Mem_Eep_Ip_eBrownOutCode)
            {
                Status = Ftfc_Mem_Eep_Ip_CmdSetFlexramFunction(FLEXRAM_FCC_COMPLETE_MAINTENANCE, 0U);
            }
        }
    }

    return Status;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @brief  Read @p Length bytes from EEPROM @p SrcAddress to @p pu8DestAddress.
 *
 * @param[in]  SrcAddress EEPROM address to read from
 * @param[out] pu8DestAddress buffer to store the read data
 * @param[in]  Length how many bytes to read
 *
 * @return the read operation status
 * @retval FTFC_MEM_EEP_IP_STATUS_OK      the requested bytes were copied into the destination buffer
 * @retval FTFC_MEM_EEP_IP_STATUS_FAILED  FTFC not ready
 * @retval FTFC_MEM_EEP_IP_STATUS_FAILED  a read was attempted on an invalid page size
 *
 * @implements Ftfc_Mem_Eep_Ip_Read_Activity
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
Ftfc_Mem_Eep_Ip_StatusType Ftfc_Mem_Eep_Ip_Read (Ftfc_Mem_Eep_Ip_AddressType SrcAddress,
                                         uint8 * pu8DestAddress,
                                         Ftfc_Mem_Eep_Ip_LengthType Length
                                        )
{
    Ftfc_Mem_Eep_Ip_StatusType Status = FTFC_MEM_EEP_IP_STATUS_OK;
    Ftfc_Mem_Eep_Ip_AddressType FlexramAddress = FTFC_MEM_EEP_IP_FLEXRAM_ADDRESS + SrcAddress;
    Ftfc_Mem_Eep_Ip_PageSizeType PageSize;
    uint8 * DestAddressPtr = pu8DestAddress;
    Ftfc_Mem_Eep_Ip_LengthType LengthRemaining = Length;

    FTFC_DEV_ASSERT(pu8DestAddress != NULL_PTR);
    FTFC_DEV_ASSERT(SrcAddress < FTFC_MEM_EEP_IP_EEP_SIZE);
    FTFC_DEV_ASSERT((SrcAddress + LengthRemaining) <= FTFC_MEM_EEP_IP_EEP_SIZE);
    FTFC_DEV_ASSERT(LengthRemaining > 0U);

    MCAL_FAULT_INJECTION_POINT(EEP_FIP_01_EEP_AFTER_READ_SYNC);

    if (Ftfc_Mem_Eep_Ip_IsFtfcReady())
    {
        while ((LengthRemaining > 0U) && (FTFC_MEM_EEP_IP_STATUS_OK == Status))
        {
            PageSize = Ftfc_Mem_Eep_Ip_AlignedPageSize(FlexramAddress, (Ftfc_Mem_Eep_Ip_AddressType)DestAddressPtr, LengthRemaining);

        #if (FTFC_MEM_EEP_IP_HIGH_TEMP_CHIP == STD_ON)
            switch (PageSize)
            {
                case FTFC_MEM_EEP_IP_PAGE_LONGWORD:
                    *((uint32 *)(uint32)DestAddressPtr) = *((uint32 *)FlexramAddress);
                break;

                case FTFC_MEM_EEP_IP_PAGE_WORD:
                {
                    uint32 Offset = FlexramAddress % FTFC_MEM_EEP_IP_PAGE_LONGWORD;
                    uint32 PageContents = *(uint32 *)(FlexramAddress - Offset);
                    *((uint16 *)(uint32)DestAddressPtr) = (uint16)(PageContents >> (Offset * 8U));
                }
                break;

                case FTFC_MEM_EEP_IP_PAGE_BYTE:
                {
                    uint32 Offset = FlexramAddress % FTFC_MEM_EEP_IP_PAGE_LONGWORD;
                    uint32 PageContents = *(uint32 *)(FlexramAddress - Offset);
                    *DestAddressPtr = (uint8)(PageContents >> (Offset * 8U));
                }
                break;

                default:
                    Status = FTFC_MEM_EEP_IP_STATUS_FAILED;
                break;
            }

        #else /* FTFC_MEM_EEP_IP_HIGH_TEMP_CHIP */

            switch (PageSize)
            {
                case FTFC_MEM_EEP_IP_PAGE_LONGWORD:
                    *((uint32 *)(uint32)DestAddressPtr) = *((uint32 *)FlexramAddress);
                break;

                case FTFC_MEM_EEP_IP_PAGE_WORD:
                    *((uint16 *)(uint32)DestAddressPtr) = *((uint16 *)FlexramAddress);
                break;

                case FTFC_MEM_EEP_IP_PAGE_BYTE:
                    *DestAddressPtr = *((uint8 *)FlexramAddress);
                break;

                default:
                    Status = FTFC_MEM_EEP_IP_STATUS_FAILED;
                break;
            }
        #endif /* FTFC_MEM_EEP_IP_HIGH_TEMP_CHIP */

            FlexramAddress += (Ftfc_Mem_Eep_Ip_AddressType)PageSize;
            DestAddressPtr = &DestAddressPtr[PageSize];
            LengthRemaining -= (Ftfc_Mem_Eep_Ip_LengthType)PageSize;
        }
    }
    else
    {
        Status = FTFC_MEM_EEP_IP_STATUS_FAILED;
    }

    return Status;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @brief   Compare the first @p Length bytes of @p pu8DestAddress to the contents found at @p SrcAddress.
 *
 * @param[in] pu8DestAddress pointer to the data buffer
 * @param[in] SrcAddress where the contents in EEPROM are stored
 * @param[in] Length how many bytes to compare
 *
 * @return the comparison result
 * @retval FTFC_MEM_EEP_IP_STATUS_OK                  the contents match
 * @retval FTFC_MEM_EEP_IP_STATUS_BLOCK_INCONSISTENT  the contents do not match
 * @retval FTFC_MEM_EEP_IP_STATUS_FAILED              FTFC not ready
 * @retval FTFC_MEM_EEP_IP_STATUS_FAILED              a read was attempted on an invalid page size
 *
 * @implements Ftfc_Mem_Eep_Ip_Compare_Activity
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
Ftfc_Mem_Eep_Ip_StatusType Ftfc_Mem_Eep_Ip_Compare (Ftfc_Mem_Eep_Ip_AddressType SrcAddress,
                                            uint8 const * pu8DestAddress,
                                            Ftfc_Mem_Eep_Ip_LengthType Length
                                           )
{
    Ftfc_Mem_Eep_Ip_StatusType Status = FTFC_MEM_EEP_IP_STATUS_OK;
    Ftfc_Mem_Eep_Ip_AddressType FlexramAddress = FTFC_MEM_EEP_IP_FLEXRAM_ADDRESS + SrcAddress;
    Ftfc_Mem_Eep_Ip_AddressType RamAddress = (Ftfc_Mem_Eep_Ip_AddressType)pu8DestAddress;
    Ftfc_Mem_Eep_Ip_PageSizeType PageSize;
    Ftfc_Mem_Eep_Ip_LengthType LengthRemaining = Length;

    FTFC_DEV_ASSERT(pu8DestAddress != NULL_PTR);
    FTFC_DEV_ASSERT(SrcAddress < FTFC_MEM_EEP_IP_EEP_SIZE);
    FTFC_DEV_ASSERT((SrcAddress + LengthRemaining) <= FTFC_MEM_EEP_IP_EEP_SIZE);
    FTFC_DEV_ASSERT(LengthRemaining > 0U);

    MCAL_FAULT_INJECTION_POINT(EEP_FIP_02_EEP_AFTER_COMPARE_SYNC);

    if (Ftfc_Mem_Eep_Ip_IsFtfcReady())
    {
        while ((LengthRemaining > 0U) && (FTFC_MEM_EEP_IP_STATUS_OK == Status))
        {
            PageSize = Ftfc_Mem_Eep_Ip_AlignedPageSize(FlexramAddress, RamAddress, LengthRemaining);

        #if (FTFC_MEM_EEP_IP_HIGH_TEMP_CHIP == STD_ON)
            switch (PageSize)
            {
                case FTFC_MEM_EEP_IP_PAGE_LONGWORD:
                    if (*((uint32 *)RamAddress) != *((uint32 *)FlexramAddress))
                    {
                        Status = FTFC_MEM_EEP_IP_STATUS_BLOCK_INCONSISTENT;
                    }
                break;

                case FTFC_MEM_EEP_IP_PAGE_WORD:
                {
                    uint32 Offset = FlexramAddress % FTFC_MEM_EEP_IP_PAGE_LONGWORD;
                    uint32 PageContents = *(uint32 *)(FlexramAddress - Offset);
                    if (*((uint16 *)RamAddress) != (uint16)(PageContents >> (Offset * 8U)))
                    {
                        Status = FTFC_MEM_EEP_IP_STATUS_BLOCK_INCONSISTENT;
                    }
                }
                break;

                case FTFC_MEM_EEP_IP_PAGE_BYTE:
                {
                    uint32 Offset = FlexramAddress % FTFC_MEM_EEP_IP_PAGE_LONGWORD;
                    uint32 PageContents = *(uint32 *)(FlexramAddress - Offset);
                    if (*((uint8 *)RamAddress) != (uint8)(PageContents >> (Offset * 8U)))
                    {
                        Status = FTFC_MEM_EEP_IP_STATUS_BLOCK_INCONSISTENT;
                    }
                }
                break;

                default:
                    Status = FTFC_MEM_EEP_IP_STATUS_FAILED;
                break;
            }

        #else /* FTFC_MEM_EEP_IP_HIGH_TEMP_CHIP */

            switch (PageSize)
            {
                case FTFC_MEM_EEP_IP_PAGE_LONGWORD:
                    if (*((uint32 *)RamAddress) != *((uint32 *)FlexramAddress))
                    {
                        Status = FTFC_MEM_EEP_IP_STATUS_BLOCK_INCONSISTENT;
                    }
                break;

                case FTFC_MEM_EEP_IP_PAGE_WORD:
                    if (*((uint16 *)RamAddress) != *((uint16 *)FlexramAddress))
                    {
                        Status = FTFC_MEM_EEP_IP_STATUS_BLOCK_INCONSISTENT;
                    }
                break;

                case FTFC_MEM_EEP_IP_PAGE_BYTE:
                    if (*((uint8 *)RamAddress) != *((uint8 *)FlexramAddress))
                    {
                        Status = FTFC_MEM_EEP_IP_STATUS_BLOCK_INCONSISTENT;
                    }
                break;

                default:
                    Status = FTFC_MEM_EEP_IP_STATUS_FAILED;
                break;
            }
        #endif /* FTFC_MEM_EEP_IP_HIGH_TEMP_CHIP */

            FlexramAddress += (Ftfc_Mem_Eep_Ip_AddressType)PageSize;
            RamAddress += (Ftfc_Mem_Eep_Ip_AddressType)PageSize;
            LengthRemaining -= (Ftfc_Mem_Eep_Ip_LengthType)PageSize;
        }
    }
    else
    {
        Status = FTFC_MEM_EEP_IP_STATUS_FAILED;
    }

    return Status;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @brief   Write @p PageSize bytes from @p pu8SrcAddress buffer to EEPROM at @p offset DestAddress
 *
 * @param[out] DestAddress EEPROM offset
 * @param[in]  pu8SrcAddress buffer containing the data to be written
 * @param[in]  PageSize must be a valid PageSize: 1, 2 or 4 bytes for FTFC and only 4 bytes for FTFM
 * @param[in]  Async choose between a synchronous and an asynchronous job
 *
 * @return the write operation result
 * @retval FTFC_MEM_EEP_IP_STATUS_FAILED         FTFC not ready
 * @retval FTFC_MEM_EEP_IP_STATUS_FAILED         an invalid page alignment was given
 * @retval FTFC_MEM_EEP_IP_STATUS_FAILED         sync: some of the FSTAT error bits were set
 * @retval FTFC_MEM_EEP_IP_STATUS_TIMEOUT        sync: timeout occurred while waiting for CCIF
 * @retval FTFC_MEM_EEP_IP_STATUS_OK             sync: page successfully written to EFLASH
 * @retval FTFC_MEM_EEP_IP_STATUS_PENDING        async: the page was written to FlexRAM, but the status of
 *                                           the EFLASH record shall be interrogated with GetJobResult
 *
 * @implements Ftfc_Mem_Eep_Ip_Write_Activity
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
Ftfc_Mem_Eep_Ip_StatusType Ftfc_Mem_Eep_Ip_Write (Ftfc_Mem_Eep_Ip_AddressType DestAddress,
                                          uint8 const * pu8SrcAddress,
                                          Ftfc_Mem_Eep_Ip_PageSizeType PageSize,
                                          boolean Async
                                         )
{
    Ftfc_Mem_Eep_Ip_StatusType Status = FTFC_MEM_EEP_IP_STATUS_FAILED;
    Ftfc_Mem_Eep_Ip_AddressType DestAddr = DestAddress;

    FTFC_DEV_ASSERT(pu8SrcAddress != NULL_PTR);
    FTFC_DEV_ASSERT(DestAddr < FTFC_MEM_EEP_IP_EEP_SIZE);
    FTFC_DEV_ASSERT((DestAddr + (Ftfc_Mem_Eep_Ip_AddressType)PageSize) <= FTFC_MEM_EEP_IP_EEP_SIZE);
    FTFC_DEV_ASSERT(PageSize == Ftfc_Mem_Eep_Ip_AlignedPageSize(DestAddr, \
                                                            (Ftfc_Mem_Eep_Ip_AddressType)pu8SrcAddress, \
                                                            (Ftfc_Mem_Eep_Ip_LengthType)PageSize));

    /* Convert the EEPROM location to a FlexRAM address */
    DestAddr += FTFC_MEM_EEP_IP_FLEXRAM_ADDRESS;

    SchM_Enter_Mem_43_EEP_MEM_EXCLUSIVE_AREA_07();
    if (Ftfc_Mem_Eep_Ip_IsFtfcReady())
    {
    #if (FTFC_MEM_EEP_IP_HIGH_TEMP_CHIP == STD_ON)
        if (PageSize != FTFC_MEM_EEP_IP_PAGE_LONGWORD)
        {
            uint32 Offset = DestAddr % FTFC_MEM_EEP_IP_PAGE_LONGWORD;

            if ((Offset + (uint32)PageSize) <= (uint32)FTFC_MEM_EEP_IP_PAGE_LONGWORD)
            {
                /* Read the current data */
                uint32 *PageAddress  = (uint32 *)((uint32)DestAddr - Offset);
                uint32  PageContents = *PageAddress;
                uint8  *ContentsPtr  = (uint8 *)(&PageContents);

                /* Modify the desired bytes */
                uint32 It;
                for (It = 0U; It < (uint32)PageSize; It++)
                {
                    ContentsPtr[It + Offset] = pu8SrcAddress[It];
                }

                /* Write the new data */
                Status = Ftfc_Mem_Eep_Ip_AcEflashWrite((Ftfc_Mem_Eep_Ip_AddressType)PageAddress, ContentsPtr, PageSize, Async);
            }
            else
            {
                /* the request crosses the 4-byte aligned page boundary */
                Status = FTFC_MEM_EEP_IP_STATUS_FAILED;
            }
        }
        else
    #endif /* FTFC_MEM_EEP_IP_HIGH_TEMP_CHIP */
        {
            Status = Ftfc_Mem_Eep_Ip_AcEflashWrite(DestAddr, pu8SrcAddress, PageSize, Async);
        }
        SchM_Exit_Mem_43_EEP_MEM_EXCLUSIVE_AREA_07();

        if ((FTFC_MEM_EEP_IP_STATUS_PENDING == Status) && (TRUE == Async))
        {
            /* In async mode the user can skip the waiting part, but he must make sure that
            * the DFLASH partition is not accessed (for r/w) until the CCIF flag is set. */
            SchM_Enter_Mem_43_EEP_MEM_EXCLUSIVE_AREA_10();
            Ftfc_Mem_Eep_Ip_xAsyncJob.Result = FTFC_MEM_EEP_IP_STATUS_PENDING;
            Ftfc_Mem_Eep_Ip_xAsyncJob.TicksStarted = OsIf_GetCounter(FTFC_MEM_EEP_IP_TIMEOUT_TYPE);
            Ftfc_Mem_Eep_Ip_xAsyncJob.TicksElapsed = 0U;
            SchM_Exit_Mem_43_EEP_MEM_EXCLUSIVE_AREA_10();
        }
    }
    else
    {
        SchM_Exit_Mem_43_EEP_MEM_EXCLUSIVE_AREA_07();
    }

    return Status;
}

#if (FTFC_MEM_EEP_IP_QUICK_WRITE == STD_ON) 
/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @brief   QuickWrite API
 * @details
 *
 * For configurations with interleaved flash blocks for EEPROM backup, quick writes
 * should be restricted to either within the first half of EEERAM or within the second half
 * of EEERAM. Otherwise, FSTAT[ACCERR] will be returned.
 *
 * Once a quick write is started, user must finish the entire quick write activity prior to
 * starting another FTFC or CSE command.
 *
 * @param[out] DestAddress EEPROM offset needs to be 4-bytes aligned
 * @param[in]  pu8SrcAddress
 * @param[in]  Length
 *
 * @return quick write status
 * @retval FTFC_MEM_EEP_IP_STATUS_FAILED         FTFC not ready
 * @retval FTFC_MEM_EEP_IP_STATUS_OK             everything ok
 * @retval FTFC_MEM_EEP_IP_STATUS_FAILED         invalid page size/alignment
 * @retval FTFC_MEM_EEP_IP_STATUS_FAILED         some of the FSTAT error bits were set
 * @retval FTFC_MEM_EEP_IP_STATUS_FAILED_MGSTAT  one or more MGSTAT 1/2/3 bits were set
 * @retval FTFC_MEM_EEP_IP_STATUS_TIMEOUT        timeout occurred while waiting for CCIF
 *
 * @implements Ftfc_Mem_Eep_Ip_QuickWrite_Activity
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
Ftfc_Mem_Eep_Ip_StatusType Ftfc_Mem_Eep_Ip_QuickWrite (Ftfc_Mem_Eep_Ip_AddressType DestAddress,
                                               uint8 const * pu8SrcAddress,
                                               Ftfc_Mem_Eep_Ip_LengthType Length
                                              )
{
    Ftfc_Mem_Eep_Ip_StatusType Status;
    Ftfc_Mem_Eep_Ip_AddressType SrcAddress = (Ftfc_Mem_Eep_Ip_AddressType)pu8SrcAddress;
    Ftfc_Mem_Eep_Ip_AddressType DestAddr = DestAddress;
    Ftfc_Mem_Eep_Ip_LengthType LengthRemaining = Length;

    /* Quick write mode only accepts 4-byte aligned writes */
    FTFC_DEV_ASSERT((DestAddr % (Ftfc_Mem_Eep_Ip_AddressType)FTFC_MEM_EEP_IP_PAGE_LONGWORD) == 0U);
    FTFC_DEV_ASSERT((SrcAddress % (Ftfc_Mem_Eep_Ip_AddressType)FTFC_MEM_EEP_IP_PAGE_LONGWORD) == 0U);

    /* FSTAT[ACCERR] is Set if FlexRAM Function Control Code is set to make FlexRAM available for
     * EEPROM quick writes but the number of bytes allocated for quick write is less than 16,
     * more than 512, or not divisible by 4 (only 32-bit quick writes are allowed) */
    FTFC_DEV_ASSERT(LengthRemaining >= FTFC_MEM_EEP_IP_QUICK_WRITES_LENGTH_MIN);
    FTFC_DEV_ASSERT(LengthRemaining <= FTFC_MEM_EEP_IP_QUICK_WRITES_LENGTH_MAX);
    FTFC_DEV_ASSERT((LengthRemaining % (Ftfc_Mem_Eep_Ip_LengthType)FTFC_MEM_EEP_IP_PAGE_LONGWORD) == 0U);

    Status = Ftfc_Mem_Eep_Ip_CmdSetFlexramFunction(FLEXRAM_FCC_MODE_EEERAM_QWRITES, (uint16)LengthRemaining);
    if (FTFC_MEM_EEP_IP_STATUS_OK == Status)
    {
        while ((LengthRemaining > 0U) && (FTFC_MEM_EEP_IP_STATUS_OK == Status))
        {
            Status = Ftfc_Mem_Eep_Ip_Write(DestAddr, (uint8 *)SrcAddress, FTFC_MEM_EEP_IP_PAGE_LONGWORD, FALSE);
            LengthRemaining -= (Ftfc_Mem_Eep_Ip_LengthType)FTFC_MEM_EEP_IP_PAGE_LONGWORD;
            DestAddr += (Ftfc_Mem_Eep_Ip_AddressType)FTFC_MEM_EEP_IP_PAGE_LONGWORD;
            SrcAddress += (Ftfc_Mem_Eep_Ip_AddressType)FTFC_MEM_EEP_IP_PAGE_LONGWORD;
        }

        if (FTFC_MEM_EEP_IP_STATUS_OK == Status)
        {
            Status = Ftfc_Mem_Eep_Ip_CmdSetFlexramFunction(FLEXRAM_FCC_MODE_EEERAM_NORMAL, 0U);
        }
        else
        {
            (void) Ftfc_Mem_Eep_Ip_CmdSetFlexramFunction(FLEXRAM_FCC_MODE_EEERAM_NORMAL, 0U);
        }
    }

    return Status;
}
#endif

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @brief   Erase API
 * @details Invokes a write with ERASED_CELL_VALUE.
 *
 * @return the erase operation result
 * @retval FTFC_MEM_EEP_IP_STATUS_FAILED         FTFC not ready
 * @retval FTFC_MEM_EEP_IP_STATUS_FAILED         an invalid page size/alignment was given
 * @retval FTFC_MEM_EEP_IP_STATUS_FAILED         sync: some of the FSTAT error bits were set
 * @retval FTFC_MEM_EEP_IP_STATUS_TIMEOUT        sync: timeout occurred while waiting for CCIF
 * @retval FTFC_MEM_EEP_IP_STATUS_OK             sync: page successfully written to EFLASH
 * @retval FTFC_MEM_EEP_IP_STATUS_PENDING        async: the page was written to FlexRAM, but the status of
 *                                           the EFLASH record shall be interrogated with GetJobResult
 *
 * @implements Ftfc_Mem_Eep_Ip_Erase_Activity
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
Ftfc_Mem_Eep_Ip_StatusType Ftfc_Mem_Eep_Ip_Erase (Ftfc_Mem_Eep_Ip_AddressType Address,
                                          Ftfc_Mem_Eep_Ip_PageSizeType PageSize,
                                          boolean Async
                                         )
{
    return Ftfc_Mem_Eep_Ip_Write(Address, (uint8 const *) &Ftfc_Mem_Eep_Ip_u32EraseValue, PageSize, Async);
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @brief  Interrogate the result of the last async job, considering the timeout and FSTAT errors.
 *
 * @return the result of the last async job
 * @retval FTFC_MEM_EEP_IP_STATUS_OK            the job finished successfully
 * @retval FTFC_MEM_EEP_IP_STATUS_FAILED        FSTAT error bits were set
 * @retval FTFC_MEM_EEP_IP_STATUS_PENDING       the job is still waiting for CCIF
 * @retval FTFC_MEM_EEP_IP_STATUS_TIMEOUT       a timeout has occurred while waiting for CCIF
 *
 * @implements Ftfc_Mem_Eep_Ip_GetJobResult_Activity
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
Ftfc_Mem_Eep_Ip_StatusType Ftfc_Mem_Eep_Ip_GetJobResult (void)
{
    uint32 Timeout = 0U;

    SchM_Enter_Mem_43_EEP_MEM_EXCLUSIVE_AREA_06();

    if (FTFC_MEM_EEP_IP_STATUS_PENDING == Ftfc_Mem_Eep_Ip_xAsyncJob.Result)
    {
        if (Ftfc_Mem_Eep_Ip_IsFtfcReady())
        {
            Ftfc_Mem_Eep_Ip_xAsyncJob.Result = Ftfc_Mem_Eep_Ip_CheckFstatErrors();
            Ftfc_Mem_Eep_Ip_xAsyncJob.TicksElapsed = 0U;
        }
        else
        {
            Timeout = OsIf_MicrosToTicks(FTFC_MEM_EEP_IP_ASYNC_WRITE_TIMEOUT, FTFC_MEM_EEP_IP_TIMEOUT_TYPE);
            Ftfc_Mem_Eep_Ip_xAsyncJob.TicksElapsed += OsIf_GetElapsed(&Ftfc_Mem_Eep_Ip_xAsyncJob.TicksStarted, FTFC_MEM_EEP_IP_TIMEOUT_TYPE);
            if (Ftfc_Mem_Eep_Ip_xAsyncJob.TicksElapsed >= Timeout)
            {
                Ftfc_Mem_Eep_Ip_xAsyncJob.Result = FTFC_MEM_EEP_IP_STATUS_TIMEOUT;
                Ftfc_Mem_Eep_Ip_xAsyncJob.TicksElapsed = 0U;
            }
        }
    }

    SchM_Exit_Mem_43_EEP_MEM_EXCLUSIVE_AREA_06();

    return Ftfc_Mem_Eep_Ip_xAsyncJob.Result;
}

#if (FTFC_DISABLE_DEM_REPORT_ERROR_STATUS == STD_OFF)
/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @brief  Getter for Ftfc_Mem_Eep_Ip_eBrownOutCode.
 *
 * @return the brownout code read after reset
 * @retval 0x04 normal write was interrupted
 * @retval 0x02 quick write was interrupted before writing all bytes to flash
 * @retval 0x01 quick write was interrupted before maintenance completed
 *
 * @implements Ftfc_Mem_Eep_Ip_GetBrownOutCode_Activity
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
Ftfc_Mem_Eep_Ip_BrownOutCodeType Ftfc_Mem_Eep_Ip_GetBrownOutCode (void)
{
    return Ftfc_Mem_Eep_Ip_eBrownOutCode;
}
#endif

#define MEM_43_EEP_STOP_SEC_CODE
#include "Mem_43_EEP_MemMap.h"

/*==================================================================================================
*                                        LOCAL FUNCTIONS
==================================================================================================*/

#define MEM_43_EEP_START_SEC_CODE
#include "Mem_43_EEP_MemMap.h"

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @brief Compute the page size, considering the addresses alignment and the remaining bytes.
 *
 * @param[in]  FlexramAddress the FlexRAM address
 * @param[in]  RamAddress the RAM address
 * @param[in]  Length how many bytes are left to process
 *
 * @return the page size that FlexRAM will accept given the address/buffer alignments
 * @retval FTFC_MEM_EEP_IP_PAGE_BYTE     for unaligned operations
 * @retval FTFC_MEM_EEP_IP_PAGE_WORD     16-bit aligned operations
 * @retval FTFC_MEM_EEP_IP_PAGE_LONGWORD 32-bit aligned operations
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
static Ftfc_Mem_Eep_Ip_PageSizeType Ftfc_Mem_Eep_Ip_AlignedPageSize (Ftfc_Mem_Eep_Ip_AddressType FlexramAddress,
                                                             Ftfc_Mem_Eep_Ip_AddressType RamAddress,
                                                             Ftfc_Mem_Eep_Ip_LengthType Length
                                                            )
{
    Ftfc_Mem_Eep_Ip_PageSizeType PageSize = FTFC_MEM_EEP_IP_PAGE_BYTE;

#if (FTFC_MEM_EEP_IP_ALIGNED_RAM_ACCESS == STD_OFF)
    (void) RamAddress; /* avoid compiler warning */
#endif

    if ((0U == (FlexramAddress % (Ftfc_Mem_Eep_Ip_AddressType)FTFC_MEM_EEP_IP_PAGE_LONGWORD))
    #if (FTFC_MEM_EEP_IP_ALIGNED_RAM_ACCESS == STD_ON)
        && (0U == (RamAddress  % (Ftfc_Mem_Eep_Ip_AddressType)FTFC_MEM_EEP_IP_PAGE_LONGWORD))
    #endif
    )
    {
        if (Length >= (Ftfc_Mem_Eep_Ip_LengthType)FTFC_MEM_EEP_IP_PAGE_LONGWORD)
        {
            /* 4 bytes aligned and length >= 4 bytes: 4 bytes operation */
            PageSize = FTFC_MEM_EEP_IP_PAGE_LONGWORD;
        }
        else if (Length >= (Ftfc_Mem_Eep_Ip_LengthType)FTFC_MEM_EEP_IP_PAGE_WORD)
        {
            /* 4 bytes aligned and 4bytes > length >= 2 bytes : 2 bytes operation */
            PageSize = FTFC_MEM_EEP_IP_PAGE_WORD;
        }
        else
        {
            /* 4 bytes aligned and length < 2 bytes : 1 byte operation */
            PageSize = FTFC_MEM_EEP_IP_PAGE_BYTE;
        }
    }
    else if ((0U == (FlexramAddress % (Ftfc_Mem_Eep_Ip_AddressType)FTFC_MEM_EEP_IP_PAGE_WORD))
        #if (FTFC_MEM_EEP_IP_ALIGNED_RAM_ACCESS == STD_ON)
             && (0U == (RamAddress  % (Ftfc_Mem_Eep_Ip_AddressType)FTFC_MEM_EEP_IP_PAGE_WORD))
        #endif
            )
    {
        if (Length >= (Ftfc_Mem_Eep_Ip_LengthType)FTFC_MEM_EEP_IP_PAGE_WORD)
        {
            /* 2 bytes aligned and length >= 2 bytes : 2 bytes operation */
            PageSize = FTFC_MEM_EEP_IP_PAGE_WORD;
        }
        else
        {
            /* 2 bytes aligned and length < 2 bytes : 1 byte operation */
            PageSize = FTFC_MEM_EEP_IP_PAGE_BYTE;
        }
    }
    else
    {
        /* unaligned operation */
        PageSize = FTFC_MEM_EEP_IP_PAGE_BYTE;
    }

    return PageSize;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @brief   Interrogate the FTFC status.
 *
 * @details If CCIF is zero, the previous command execution is still active, a new command write
 *          sequence cannot be started, and all writes to the FCCOB registers are ignored.
 *
 * @details The CCIF and EEERDY flag will remain negated until all EEPROM maintenance activities
 *          have been completed.
 *
 * @return FTFC status
 * @retval TRUE  FTFC ready
 * @retval FALSE FTFC not ready
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
static boolean Ftfc_Mem_Eep_Ip_IsFtfcReady (void)
{
    boolean Status = FALSE;
    volatile uint32 EepReadyFlag = (uint32)(IP_EEPROM->FCNFG) & (uint32)FCNFG_EEERDY_MASK;

    MCAL_FAULT_INJECTION_POINT(EEP_FIP_FTFx_EEERDY_BIT);

    if (0U != EepReadyFlag)
    {
        if (0U != (IP_EEPROM->FSTAT & FSTAT_CCIF_MASK))
        {
            Status = TRUE;
        }
    }

    return Status;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @brief   Construct the SETRAM flash command (Set FlexRAM Function)
 *
 * @details First, set up all required FCCOB fields and then initiate the command's execution
 * by writing a 1 to the FSTAT[CCIF] bit.
 *
 * @param[in] FlexramFuncControlCode the control code for the SETRAM command
 * @param[in] QuickWritesLength only used for FLEXRAM_FCC_MODE_EEERAM_QWRITES
 *
 * @return flash command execution status
 * @retval FTFC_MEM_EEP_IP_STATUS_OK             the command has completed successfully
 * @retval FTFC_MEM_EEP_IP_STATUS_TIMEOUT        a timeout has occurred
 * @retval FTFC_MEM_EEP_IP_STATUS_FAILED         the flash command execution failed
 * @retval FTFC_MEM_EEP_IP_STATUS_FAILED_MGSTAT  one or more MGSTAT 1/2/3 bits were set (FTFM only)
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
static Ftfc_Mem_Eep_Ip_StatusType Ftfc_Mem_Eep_Ip_CmdSetFlexramFunction (Ftfc_Mem_Eep_Ip_FlexramFccType FlexramFuncControlCode,
                                                                 uint16 QuickWritesLength
                                                                )
{
    Ftfc_Mem_Eep_Ip_StatusType Status = FTFC_MEM_EEP_IP_STATUS_FAILED;

    /* If CCIF is zero:                                 *
     * - the previous command execution is still active *
     * - a new command write sequence cannot be started *
     * - all writes to the FCCOB registers are ignored. */
    SchM_Enter_Mem_43_EEP_MEM_EXCLUSIVE_AREA_08();
    if (0U != (IP_EEPROM->FSTAT & FSTAT_CCIF_MASK))
    {
        /* The individual registers that make up the FCCOB data set can be written in any order. */

        IP_EEPROM->FCCOB[FTFC_MEM_EEP_IP_FCCOB_SETRAM_WRITE_CMD_SETRAM] = (uint8)FTFC_MEM_EEP_IP_FLASH_CMD_SETRAM_U8;
        IP_EEPROM->FCCOB[FTFC_MEM_EEP_IP_FCCOB_SETRAM_WRITE_FR_FCC] = (uint8)FlexramFuncControlCode;

        if (FLEXRAM_FCC_MODE_EEERAM_QWRITES == FlexramFuncControlCode)
        {
            IP_EEPROM->FCCOB[FTFC_MEM_EEP_IP_FCCOB_SETRAM_WRITE_QUICKWRITES_LENGTH_MSB] = (uint8)(QuickWritesLength >> 8U);
            IP_EEPROM->FCCOB[FTFC_MEM_EEP_IP_FCCOB_SETRAM_WRITE_QUICKWRITES_LENGTH_LSB] = (uint8) QuickWritesLength;
        }

        /* This clears the CCIF bit, which locks all FCCOB parameter fields and
         * they cannot be changed by the user until the command completes (CCIF returns to 1). */
        Status = Ftfc_Mem_Eep_Ip_AcEflashCmd();
    }
    SchM_Exit_Mem_43_EEP_MEM_EXCLUSIVE_AREA_08();

    return Status;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @brief   Check the FSTAT error flags.
 *
 * @details If the parameter check fails, the FSTAT[ACCERR] (access error) flag is set.
 * If the protection check fails, the FSTAT[FPVIOL] (protection error) flag is set.
 * Run-time errors are reported in the FSTAT[MGSTAT0] bit.
 *
 * @return the kind of error detected:
 * @retval FTFC_MEM_EEP_IP_STATUS_OK            no error flags were set
 * @retval FTFC_MEM_EEP_IP_STATUS_FAILED        FSTAT error bits were set
 * @retval FTFC_MEM_EEP_IP_STATUS_FAILED_MGSTAT MGSTAT1/2/3 bits were set (FTFM only)
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
static Ftfc_Mem_Eep_Ip_StatusType Ftfc_Mem_Eep_Ip_CheckFstatErrors (void)
{
    Ftfc_Mem_Eep_Ip_StatusType Status = FTFC_MEM_EEP_IP_STATUS_OK;

#if (FTFC_MEM_EEP_IP_HIGH_TEMP_CHIP == STD_ON)
    uint8 ErrorFlagsHT = FSTAT_MGSTAT1_MASK | FSTAT_MGSTAT2_MASK | FSTAT_MGSTAT3_MASK;
#endif
    uint8 ErrorFlags = FSTAT_MGSTAT0_MASK | FSTAT_FPVIOL_MASK | FSTAT_ACCERR_MASK | FSTAT_RDCOLERR_MASK;
    uint8 FlashStatus = IP_EEPROM->FSTAT;

    if (0U != (FlashStatus & ErrorFlags))
    {
        Status = FTFC_MEM_EEP_IP_STATUS_FAILED;
    }

#if (FTFC_MEM_EEP_IP_HIGH_TEMP_CHIP == STD_ON)
    if (0U != (FlashStatus & ErrorFlagsHT))
    {
        Status = FTFC_MEM_EEP_IP_STATUS_FAILED_MGSTAT;
        
    }
#endif

    return Status;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @brief   Implements the steps needed to be followed when accessing the EFLASH.
 *
 * @details Clear the FSTAT errors, call the notification functions, run the AC and check for errors.
 *
 * Do not attempt to read a flash block while the FTFC is running a command (CCIF = 0)
 * on that same block.
 *
 * @return the EFLASH operation result
 * @retval FTFC_MEM_EEP_IP_STATUS_OK             cmd executed successfully
 * @retval FTFC_MEM_EEP_IP_STATUS_TIMEOUT        timeout occurred while waiting for CCIF to be set
 * @retval FTFC_MEM_EEP_IP_STATUS_FAILED         some of the FSTAT error bits were set
 * @retval FTFC_MEM_EEP_IP_STATUS_FAILED_MGSTAT  one or more MGSTAT 1/2/3 bits were set (FTFM only)
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
static Ftfc_Mem_Eep_Ip_StatusType Ftfc_Mem_Eep_Ip_AcEflashCmd (void)
{
    Ftfc_Mem_Eep_Ip_StatusType Status = FTFC_MEM_EEP_IP_STATUS_OK;

    /* When set, the Access Error (ACCERR) and Flash Protection Violation (FPVIOL) bits in this
     * register prevent the launch of any more commands until the flag is cleared. */
    IP_EEPROM->FSTAT = (FSTAT_ACCERR_MASK | FSTAT_FPVIOL_MASK); /* W1C */

    if (Ftfc_Mem_Eep_Ip_pxConfiguration->startEepromAccessNotifPtr != NULL_PTR)
    {
        Ftfc_Mem_Eep_Ip_pxConfiguration->startEepromAccessNotifPtr();
    }

    Status = Ftfc_Mem_Eep_Ip_StartFlashCommand();

    if (Ftfc_Mem_Eep_Ip_pxConfiguration->finishedEepromAccessNotifPtr != NULL_PTR)
    {
        Ftfc_Mem_Eep_Ip_pxConfiguration->finishedEepromAccessNotifPtr();
    }

    if (FTFC_MEM_EEP_IP_STATUS_OK == Status)
    {
        Status = Ftfc_Mem_Eep_Ip_CheckFstatErrors();
    }

    return Status;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @brief   Implements the steps needed to be followed when accessing the EFLASH.
 *
 * @details Clear the FSTAT errors, call the notification functions, run the AC and check for errors.
 *
 * @param[in] DestAddress
 * @param[in] pu8SrcAddress
 * @param[in] PageSize
 * @param[in] Async
 *
 * @return the write operation result
 * @retval FTFC_MEM_EEP_IP_STATUS_OK             sync: page successfully written to EFLASH
 * @retval FTFC_MEM_EEP_IP_STATUS_FAILED         sync: some of the FSTAT error bits were set
 * @retval FTFC_MEM_EEP_IP_STATUS_FAILED_MGSTAT  sync: one or more MGSTAT 1/2/3 bits were set
 * @retval FTFC_MEM_EEP_IP_STATUS_TIMEOUT        sync: timeout occurred while waiting for CCIF
 * @retval FTFC_MEM_EEP_IP_STATUS_PENDING        async: the page was written to FlexRAM, but the status of
 *                                           the EFLASH record shall be interrogated with GetJobResult
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
static Ftfc_Mem_Eep_Ip_StatusType Ftfc_Mem_Eep_Ip_AcEflashWrite (Ftfc_Mem_Eep_Ip_AddressType DestAddress,
                                                         uint8 const * pu8SrcAddress,
                                                         Ftfc_Mem_Eep_Ip_PageSizeType PageSize,
                                                         boolean Async
                                                        )
{
    Ftfc_Mem_Eep_Ip_StatusType Status = FTFC_MEM_EEP_IP_STATUS_OK;

    /* When set, the Access Error (ACCERR) and Flash Protection Violation (FPVIOL) bits in this
     * register prevent writes to the FlexRAM (when EEERDY is set) until the flag is cleared */
    IP_EEPROM->FSTAT = (FSTAT_ACCERR_MASK | FSTAT_FPVIOL_MASK); /* W1C */

    if (Ftfc_Mem_Eep_Ip_pxConfiguration->startEepromAccessNotifPtr != NULL_PTR)
    {
        Ftfc_Mem_Eep_Ip_pxConfiguration->startEepromAccessNotifPtr();
    }

    /* When configured for emulated EEPROM use, writes to an unprotected location in FlexRAM invoke
     * the emulated EEPROM file system to program a new EEPROM data record in the emulated EEPROM
     * backup memory in a round-robin fashion. This is why we can't write into FlexRAM
     * while fetching code from the DFLASH partition. */
    Status = Ftfc_Mem_Eep_Ip_WriteIntoFlexram(DestAddress, pu8SrcAddress, PageSize, Async);

    if (Ftfc_Mem_Eep_Ip_pxConfiguration->finishedEepromAccessNotifPtr != NULL_PTR)
    {
        Ftfc_Mem_Eep_Ip_pxConfiguration->finishedEepromAccessNotifPtr();
    }

    if ((FTFC_MEM_EEP_IP_STATUS_OK == Status) && (FALSE == Async))
    {
        /* the FSTAT register is ready to be interrogated only after
        * the FlexRAM write has successfully completed */
        Status = Ftfc_Mem_Eep_Ip_CheckFstatErrors();
    }

    return Status;
}

#define MEM_43_EEP_STOP_SEC_CODE
#include "Mem_43_EEP_MemMap.h"

#if (FTFC_MEM_EEP_IP_LOAD_AC_INTO_RAM == STD_ON)
#define MEM_43_EEP_START_SEC_RAMCODE
#else
#define MEM_43_EEP_START_SEC_CODE
#endif
#include "Mem_43_EEP_MemMap.h"

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @brief   Start the previously constructed Flash Command and wait for it to complete.
 *
 * @details This function must not run from the DFLASH partition shared with EFLASH.
 *
 * Before launching a command, the ACCERR and FPVIOL bits in the FSTAT register  must be zero and
 * the CCIF flag must read 1 to verify that any previous command has completed. If CCIF is zero,
 * the previous command execution is still active, a new command write sequence cannot be started,
 * and all writes to the FCCOB registers are ignored.
 *
 * Once all relevant command parameters have been loaded, the user launches the command
 * by clearing the FSTAT[CCIF] bit by writing a '1' to it.
 *
 * Do not attempt to read a flash block while the FTFC is running a command (CCIF = 0)
 * on that same block.

 * @return the cmd execution result
 * @retval FTFC_MEM_EEP_IP_STATUS_OK       successful cmd execution
 * @retval FTFC_MEM_EEP_IP_STATUS_TIMEOUT  a timeout has occurred while waiting for the CCIF flag
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
static Ftfc_Mem_Eep_Ip_StatusType Ftfc_Mem_Eep_Ip_StartFlashCommand (void)
{
    /* W1C the CCIF bit to start the command */
    IP_EEPROM->FSTAT = FSTAT_CCIF_MASK;

    /* The CCIF flag remains zero until the FTFC command completes */
    return Ftfc_Mem_Eep_Ip_WaitCcifTimeout(FTFC_MEM_EEP_IP_ABORT_TIMEOUT);
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @brief   Write a page into FlexRAM.
 * @details This function must not run from the DFLASH partition shared with EFLASH.
 *
 * @param[in] DestAddress
 * @param[in] pu8SrcAddress
 * @param[in] PageSize
 * @param[in] Async
 *
 * @return the result of the write request
 * @retval FTFC_MEM_EEP_IP_STATUS_PENDING  async: GetJobResult shall be used to check the FSTAT errors
 * @retval FTFC_MEM_EEP_IP_STATUS_OK       sync: the write has succeeded
 * @retval FTFC_MEM_EEP_IP_STATUS_TIMEOUT  sync: a timeout has occured while waiting for the CCIF flag
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
static Ftfc_Mem_Eep_Ip_StatusType Ftfc_Mem_Eep_Ip_WriteIntoFlexram (Ftfc_Mem_Eep_Ip_AddressType DestAddress,
                                                            uint8 const * pu8SrcAddress,
                                                            Ftfc_Mem_Eep_Ip_PageSizeType PageSize,
                                                            boolean Async
                                                           )
{
    Ftfc_Mem_Eep_Ip_StatusType Status = FTFC_MEM_EEP_IP_STATUS_PENDING;
    Ftfc_Mem_Eep_Ip_AddressType SrcAdd = (Ftfc_Mem_Eep_Ip_AddressType)pu8SrcAddress;

#if (FTFC_MEM_EEP_IP_HIGH_TEMP_CHIP == STD_ON)

    *(uint32 *)DestAddress = *(uint32 const *)SrcAdd;
    (void) PageSize; /* Page size is always LONGWORD for FTFM */

#else /* FTFC_MEM_EEP_IP_HIGH_TEMP_CHIP */

    switch (PageSize)
    {
        case FTFC_MEM_EEP_IP_PAGE_BYTE:
            *(uint8 *)DestAddress = *(uint8 const *)SrcAdd;
        break;

        case FTFC_MEM_EEP_IP_PAGE_WORD:
            *(uint16 *)DestAddress = *(uint16 const *)SrcAdd;
        break;

        case FTFC_MEM_EEP_IP_PAGE_LONGWORD:
            *(uint32 *)DestAddress = *(uint32 const *)SrcAdd;
        break;

        default:
            /* page size checked in the upper call stack */
        break;
    }

#endif /* FTFC_MEM_EEP_IP_HIGH_TEMP_CHIP */

    MCAL_FAULT_INJECTION_POINT(EEP_FIP_FTFx_CHECK_BO);

    /* When a write occurs the FlexRAM is not accessible until the CCIF bit is set. */
    if (FALSE == Async)
    {
        Status = Ftfc_Mem_Eep_Ip_WaitCcifTimeout(FTFC_MEM_EEP_IP_SYNC_WRITE_TIMEOUT);
    }

    return Status;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @brief   Wait for the CCIF flag to be set or exit with a given timeout.
 *
 * @details This function must not run from the DFLASH partition shared with EFLASH.
 *
 * @param[in] TimeoutCounter given as a dummy counter that is decremented in a while loop
 *
 * @return the status of the flash operation
 * @retval FTFC_MEM_EEP_IP_STATUS_OK       the flash cmd/write has completed
 * @retval FTFC_MEM_EEP_IP_STATUS_TIMEOUT  a timeout has occurred
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
static Ftfc_Mem_Eep_Ip_StatusType Ftfc_Mem_Eep_Ip_WaitCcifTimeout (uint32 TimeoutCounter)
{
    Ftfc_Mem_Eep_Ip_StatusType Status = FTFC_MEM_EEP_IP_STATUS_OK;
    uint32 counter = TimeoutCounter;
    volatile uint32 CommandCompleteFlag = (uint32)(IP_EEPROM->FSTAT) & (uint32)FSTAT_CCIF_MASK;

    MCAL_FAULT_INJECTION_POINT(FTFC_MEM_EEP_IP_WAITCCIFTIMEOUT_FAULT);

    while (0U == CommandCompleteFlag)
    {
        --counter;
        if (0U == counter)
        {
            Status = FTFC_MEM_EEP_IP_STATUS_TIMEOUT;
            break;
        }

        /* feed the watchdog */
        if (NULL_PTR != Ftfc_Mem_Eep_Ip_pxConfiguration->acCallBackPtr)
        {
            Ftfc_Mem_Eep_Ip_pxConfiguration->acCallBackPtr();
        }

        CommandCompleteFlag = (uint32)(IP_EEPROM->FSTAT) & (uint32)FSTAT_CCIF_MASK;
    }

    return Status;
}

#if (FTFC_MEM_EEP_IP_LOAD_AC_INTO_RAM == STD_ON)
#define MEM_43_EEP_STOP_SEC_RAMCODE
#else
#define MEM_43_EEP_STOP_SEC_CODE
#endif
#include "Mem_43_EEP_MemMap.h"

#ifdef __cplusplus
}
#endif

/** @} */

