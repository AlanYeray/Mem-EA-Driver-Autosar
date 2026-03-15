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

#ifndef FTFC_MEM_EEP_IP_H
#define FTFC_MEM_EEP_IP_H

/**
*   @file Ftfc_Mem_Eep_Ip.h
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
#include "Ftfc_Mem_Eep_Ip_Cfg.h"
#include "Ftfc_Mem_Eep_Ip_Types.h"

/*==================================================================================================
*                                 SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define FTFC_MEM_EEP_IP_VENDOR_ID_H                    43
#define FTFC_MEM_EEP_IP_AR_RELEASE_MAJOR_VERSION_H     4
#define FTFC_MEM_EEP_IP_AR_RELEASE_MINOR_VERSION_H     7
#define FTFC_MEM_EEP_IP_AR_RELEASE_REVISION_VERSION_H  0
#define FTFC_MEM_EEP_IP_SW_MAJOR_VERSION_H             2
#define FTFC_MEM_EEP_IP_SW_MINOR_VERSION_H             0
#define FTFC_MEM_EEP_IP_SW_PATCH_VERSION_H             0

/*==================================================================================================
*                                       FILE VERSION CHECKS
==================================================================================================*/

/* Version checks for Ftfc_Mem_Eep_Ip_Cfg.h */
#if (FTFC_MEM_EEP_IP_VENDOR_ID_H != FTFC_MEM_EEP_IP_CFG_VENDOR_ID_H)
    #error "Ftfc_Mem_Eep_Ip.h and Ftfc_Mem_Eep_Ip_Cfg.h have different vendor IDs!"
#endif
#if ((FTFC_MEM_EEP_IP_AR_RELEASE_MAJOR_VERSION_H    != FTFC_MEM_EEP_IP_CFG_AR_RELEASE_MAJOR_VERSION_H) || \
     (FTFC_MEM_EEP_IP_AR_RELEASE_MINOR_VERSION_H    != FTFC_MEM_EEP_IP_CFG_AR_RELEASE_MINOR_VERSION_H) || \
     (FTFC_MEM_EEP_IP_AR_RELEASE_REVISION_VERSION_H != FTFC_MEM_EEP_IP_CFG_AR_RELEASE_REVISION_VERSION_H) \
    )
    #error "Ftfc_Mem_Eep_Ip.h and Ftfc_Mem_Eep_Ip_Cfg.h are for different AUTOSAR versions!"
#endif
#if ((FTFC_MEM_EEP_IP_SW_MAJOR_VERSION_H != FTFC_MEM_EEP_IP_CFG_SW_MAJOR_VERSION_H) || \
     (FTFC_MEM_EEP_IP_SW_MINOR_VERSION_H != FTFC_MEM_EEP_IP_CFG_SW_MINOR_VERSION_H) || \
     (FTFC_MEM_EEP_IP_SW_PATCH_VERSION_H != FTFC_MEM_EEP_IP_CFG_SW_PATCH_VERSION_H)    \
    )
    #error "Ftfc_Mem_Eep_Ip.h and Ftfc_Mem_Eep_Ip_Cfg.h have different SW versions!"
#endif

/* Version checks for Ftfc_Mem_Eep_Ip_Types.h */
#if (FTFC_MEM_EEP_IP_VENDOR_ID_H != FTFC_MEM_EEP_IP_TYPES_VENDOR_ID_H)
    #error "Ftfc_Mem_Eep_Ip.h and Ftfc_Mem_Eep_Ip_Types.h have different vendor IDs!"
#endif
#if ((FTFC_MEM_EEP_IP_AR_RELEASE_MAJOR_VERSION_H    != FTFC_MEM_EEP_IP_TYPES_AR_RELEASE_MAJOR_VERSION_H) || \
     (FTFC_MEM_EEP_IP_AR_RELEASE_MINOR_VERSION_H    != FTFC_MEM_EEP_IP_TYPES_AR_RELEASE_MINOR_VERSION_H) || \
     (FTFC_MEM_EEP_IP_AR_RELEASE_REVISION_VERSION_H != FTFC_MEM_EEP_IP_TYPES_AR_RELEASE_REVISION_VERSION_H) \
    )
    #error "Ftfc_Mem_Eep_Ip.h and Ftfc_Mem_Eep_Ip_Types.h are for different AUTOSAR versions!"
#endif
#if ((FTFC_MEM_EEP_IP_SW_MAJOR_VERSION_H != FTFC_MEM_EEP_IP_TYPES_SW_MAJOR_VERSION_H) || \
     (FTFC_MEM_EEP_IP_SW_MINOR_VERSION_H != FTFC_MEM_EEP_IP_TYPES_SW_MINOR_VERSION_H) || \
     (FTFC_MEM_EEP_IP_SW_PATCH_VERSION_H != FTFC_MEM_EEP_IP_TYPES_SW_PATCH_VERSION_H)    \
    )
    #error "Ftfc_Mem_Eep_Ip.h and Ftfc_Mem_Eep_Ip_Types.h have different SW versions!"
#endif

/*==================================================================================================
*                                            CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/

/*==================================================================================================
*                                              ENUMS
==================================================================================================*/

/*==================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
*                                  GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
*                                       FUNCTION PROTOTYPES
==================================================================================================*/
#define MEM_43_EEP_START_SEC_CODE
#include "Mem_43_EEP_MemMap.h"

/**
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
**/
Ftfc_Mem_Eep_Ip_StatusType Ftfc_Mem_Eep_Ip_Init (Ftfc_Mem_Eep_Ip_ConfigType const * pConfig);

/**
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
**/
Ftfc_Mem_Eep_Ip_StatusType Ftfc_Mem_Eep_Ip_Read (Ftfc_Mem_Eep_Ip_AddressType SrcAddress,
                                         uint8 * pu8DestAddress,
                                         Ftfc_Mem_Eep_Ip_LengthType Length
                                        );

/**
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
**/
Ftfc_Mem_Eep_Ip_StatusType Ftfc_Mem_Eep_Ip_Compare (Ftfc_Mem_Eep_Ip_AddressType SrcAddress,
                                            uint8 const * pu8DestAddress,
                                            Ftfc_Mem_Eep_Ip_LengthType Length
                                           );

/**
 * @brief   Write @p PageSize bytes from @p pu8SrcAddress buffer to EEPROM at @p offset DestAddress.
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
**/
Ftfc_Mem_Eep_Ip_StatusType Ftfc_Mem_Eep_Ip_Write (Ftfc_Mem_Eep_Ip_AddressType DestAddress,
                                          uint8 const * pu8SrcAddress,
                                          Ftfc_Mem_Eep_Ip_PageSizeType PageSize,
                                          boolean Async
                                         );

#if (FTFC_MEM_EEP_IP_QUICK_WRITE == STD_ON) 
/**
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
**/
Ftfc_Mem_Eep_Ip_StatusType Ftfc_Mem_Eep_Ip_QuickWrite (Ftfc_Mem_Eep_Ip_AddressType DestAddress,
                                               uint8 const * pu8SrcAddress,
                                               Ftfc_Mem_Eep_Ip_LengthType Length
                                              );
#endif

/**
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
**/
Ftfc_Mem_Eep_Ip_StatusType Ftfc_Mem_Eep_Ip_Erase (Ftfc_Mem_Eep_Ip_AddressType Address,
                                          Ftfc_Mem_Eep_Ip_PageSizeType PageSize,
                                          boolean Async
                                         );

/**
 * @brief  Interrogate the result of the last async job, considering the timeout and FSTAT errors.
 *
 * @return the result of the last async job
 * @retval FTFC_MEM_EEP_IP_STATUS_OK            the job finished successfully
 * @retval FTFC_MEM_EEP_IP_STATUS_FAILED        FSTAT error bits were set
 * @retval FTFC_MEM_EEP_IP_STATUS_PENDING       the job is still waiting for CCIF
 * @retval FTFC_MEM_EEP_IP_STATUS_TIMEOUT       a timeout has occurred while waiting for CCIF
 *  
**/
Ftfc_Mem_Eep_Ip_StatusType Ftfc_Mem_Eep_Ip_GetJobResult (void);

#if (FTFC_DISABLE_DEM_REPORT_ERROR_STATUS == STD_OFF)
/**
 * @brief  Getter for Ftfc_Mem_Eep_Ip_eBrownOutCode.
 *
 * @return the brownout code read after reset
 * @retval 0x04 normal write was interrupted
 * @retval 0x02 quick write was interrupted before writing all bytes to flash
 * @retval 0x01 quick write was interrupted before maintenance completed
 *  
**/
Ftfc_Mem_Eep_Ip_BrownOutCodeType Ftfc_Mem_Eep_Ip_GetBrownOutCode (void);
#endif

#define MEM_43_EEP_STOP_SEC_CODE
#include "Mem_43_EEP_MemMap.h"

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* FTFC_MEM_EEP_IP_H */

