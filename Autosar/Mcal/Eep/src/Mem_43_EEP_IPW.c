/*==================================================================================================
* Project : RTD AUTOSAR 4.7
* Platform : CORTEXM
* Peripheral : IPV_FTFC
* Dependencies : 
*
* Autosar Version : 4.7.0
* Autosar Revision : ASR_REL_4_7_REV_0000
* Autosar Conf.Variant :
* SW Version : 2.0.0
* Build Version : S32K1_RTD_2_0_0_P04_D2404_ASR_REL_4_7_REV_0000_20240417
*
* Copyright 2020-2024 NXP Semiconductors
*
* NXP Confidential. This software is owned or controlled by NXP and may only be
* used strictly in accordance with the applicable license terms. By expressly
* accepting such terms or by downloading, installing, activating and/or otherwise
* using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms. If you do not agree to be
* bound by the applicable license terms, then you may not retain, install,
* activate or otherwise use the software.
==================================================================================================*/

/**
* @file
*
* @addtogroup MEM_43_EEP_DRIVER Mem_43_EEProm Driver
* @{
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
#include "Mem_43_EEP.h"
#include "Mem_43_EEP_IPW.h"
#include "Det.h"

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define MEM_43_EEP_IPW_VENDOR_ID_C                      43
#define MEM_43_EEP_IPW_AR_RELEASE_MAJOR_VERSION_C       4
#define MEM_43_EEP_IPW_AR_RELEASE_MINOR_VERSION_C       7
#define MEM_43_EEP_IPW_AR_RELEASE_REVISION_VERSION_C    0
#define MEM_43_EEP_IPW_SW_MAJOR_VERSION_C               2
#define MEM_43_EEP_IPW_SW_MINOR_VERSION_C               0
#define MEM_43_EEP_IPW_SW_PATCH_VERSION_C               0

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Version checks for OsIf.h */
    #if ((MEM_43_EEP_IPW_AR_RELEASE_MAJOR_VERSION_C != OSIF_AR_RELEASE_MAJOR_VERSION) || \
         (MEM_43_EEP_IPW_AR_RELEASE_MINOR_VERSION_C != OSIF_AR_RELEASE_MINOR_VERSION) \
        )
        #error "AutoSar Version Numbers of Mem_43_EEP_IPW.c and OsIf.h are different"
    #endif
#endif

/* Version checks for Mem_43_EEP.h */
#if (MEM_43_EEP_IPW_VENDOR_ID_C != MEM_43_EEP_VENDOR_ID)
    #error "Mem_43_EEP_IPW.c and Mem_43_EEP.h have different vendor ids"
#endif
#if ((MEM_43_EEP_IPW_AR_RELEASE_MAJOR_VERSION_C    != MEM_43_EEP_AR_RELEASE_MAJOR_VERSION) || \
     (MEM_43_EEP_IPW_AR_RELEASE_MINOR_VERSION_C    != MEM_43_EEP_AR_RELEASE_MINOR_VERSION) || \
     (MEM_43_EEP_IPW_AR_RELEASE_REVISION_VERSION_C != MEM_43_EEP_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of Mem_43_EEP_IPW.c and Mem_43_EEP.h are different"
#endif
#if ((MEM_43_EEP_IPW_SW_MAJOR_VERSION_C != MEM_43_EEP_SW_MAJOR_VERSION) || \
     (MEM_43_EEP_IPW_SW_MINOR_VERSION_C != MEM_43_EEP_SW_MINOR_VERSION) || \
     (MEM_43_EEP_IPW_SW_PATCH_VERSION_C != MEM_43_EEP_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Mem_43_EEP_IPW.c and Mem_43_EEP.h are different"
#endif

/* Version checks for Mem_43_EEP_IPW.h */
#if (MEM_43_EEP_IPW_VENDOR_ID_C != MEM_43_EEP_IPW_VENDOR_ID_H)
    #error "Mem_43_EEP_IPW.c and Mem_43_EEP_IPW.h have different vendor ids"
#endif
#if ((MEM_43_EEP_IPW_AR_RELEASE_MAJOR_VERSION_C    != MEM_43_EEP_IPW_AR_RELEASE_MAJOR_VERSION_H) || \
     (MEM_43_EEP_IPW_AR_RELEASE_MINOR_VERSION_C    != MEM_43_EEP_IPW_AR_RELEASE_MINOR_VERSION_H) || \
     (MEM_43_EEP_IPW_AR_RELEASE_REVISION_VERSION_C != MEM_43_EEP_IPW_AR_RELEASE_REVISION_VERSION_H) \
    )
    #error "AutoSar Version Numbers of Mem_43_EEP_IPW.c and Mem_43_EEP_IPW.h are different"
#endif
#if ((MEM_43_EEP_IPW_SW_MAJOR_VERSION_C != MEM_43_EEP_IPW_SW_MAJOR_VERSION_H) || \
     (MEM_43_EEP_IPW_SW_MINOR_VERSION_C != MEM_43_EEP_IPW_SW_MINOR_VERSION_H) || \
     (MEM_43_EEP_IPW_SW_PATCH_VERSION_C != MEM_43_EEP_IPW_SW_PATCH_VERSION_H) \
    )
    #error "Software Version Numbers of Mem_43_EEP_IPW.c and Mem_43_EEP_IPW.h are different"
#endif

/* Version checks for Det.h */
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    #if ((MEM_43_EEP_IPW_AR_RELEASE_MAJOR_VERSION_C != DET_AR_RELEASE_MAJOR_VERSION) || \
         (MEM_43_EEP_IPW_AR_RELEASE_MINOR_VERSION_C != DET_AR_RELEASE_MINOR_VERSION) \
        )
        #error "AutoSar Version Numbers of Mem_43_EEP_IPW.c and Det.h are different"
    #endif
#endif

/*==================================================================================================
*                           LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

#if ((MEM_43_EEP_ASYNC_WRITE_OPERATIONS_ENABLED == STD_ON) || \
     (MEM_43_EEP_ASYNC_ERASE_OPERATIONS_ENABLED == STD_ON))
typedef struct
{
    Mem_43_EEP_AddressType DestAddress;
    uint8 const * SrcAddress;
    Mem_43_EEP_LengthType Length;
    boolean InProgress;
} Mem_43_EEP_Ipw_xAsyncJobType;
#endif

/*==================================================================================================
*                                          LOCAL MACROS
==================================================================================================*/

/*==================================================================================================
*                                        GLOBAL CONSTANTS
==================================================================================================*/

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

#if (FTFC_DISABLE_DEM_REPORT_ERROR_STATUS == STD_OFF)
    static Ftfc_Mem_Eep_Ip_ConfigType const * Mem_43_EEP_Ipw_pxFtfcConfig;
#endif

#if ((MEM_43_EEP_ASYNC_ERASE_OPERATIONS_ENABLED == STD_ON) || \
     (MEM_43_EEP_ASYNC_WRITE_OPERATIONS_ENABLED == STD_ON))
    static Mem_43_EEP_Ipw_xAsyncJobType Mem_43_EEP_Ipw_xAsyncJob;
#endif

#define MEM_43_EEP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Mem_43_EEP_MemMap.h"

/*==================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/
#define MEM_43_EEP_START_SEC_CODE
#include "Mem_43_EEP_MemMap.h"

#if (FTFC_DISABLE_DEM_REPORT_ERROR_STATUS == STD_OFF)
static void Mem_43_EEP_Ipw_ReportBrownOut (
    void
);
#endif

static void Mem_43_EEP_Ipw_ReportFailure (Ftfc_Mem_Eep_Ip_StatusType FtfcStatus, 
                                                                       uint32 InstanceId,
                                                                       uint32 ApiID
                                                          );

static Ftfc_Mem_Eep_Ip_PageSizeType Mem_43_EEP_Ipw_AlignedPageSize (Ftfc_Mem_Eep_Ip_AddressType FlexramAddress,
                                                         Ftfc_Mem_Eep_Ip_AddressType RamAddress,
                                                         Ftfc_Mem_Eep_Ip_LengthType Length
                                                        );
#define MEM_43_EEP_STOP_SEC_CODE
#include "Mem_43_EEP_MemMap.h"
/*==================================================================================================
*                                        GLOBAL FUNCTIONS
==================================================================================================*/

#define MEM_43_EEP_START_SEC_CODE
#include "Mem_43_EEP_MemMap.h"

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @brief   Initialize the FTFC IP
 * @details FTFC is the only EEPROM available on this platform.
 *
 * @return  FTFC initialization status
 * @retval  MEM_43_EEP_E_OK on success
 * @retval  MEM_43_EEP_E_FAILED on failure
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
Std_ReturnType Mem_43_EEP_Ipw_Init(Mem_43_EEP_ConfigType const * Config, uint32 InstanceIndex)
{
    Std_ReturnType RetVal = (Std_ReturnType)E_OK;
    Ftfc_Mem_Eep_Ip_StatusType FtfcStatus = FTFC_MEM_EEP_IP_STATUS_OK;

    /* reinitialize the global variables */
#if ((MEM_43_EEP_ASYNC_ERASE_OPERATIONS_ENABLED == STD_ON) || \
     (MEM_43_EEP_ASYNC_WRITE_OPERATIONS_ENABLED == STD_ON))
    Mem_43_EEP_Ipw_xAsyncJob.DestAddress = 0U;
    Mem_43_EEP_Ipw_xAsyncJob.SrcAddress = NULL_PTR;
    Mem_43_EEP_Ipw_xAsyncJob.Length = 0U;
    Mem_43_EEP_Ipw_xAsyncJob.InProgress = FALSE;
#endif

    /* Call Ip init function */
    FtfcStatus = Ftfc_Mem_Eep_Ip_Init(Config->pxFtfcIpConfig);

#if (FTFC_DISABLE_DEM_REPORT_ERROR_STATUS == STD_OFF)
    Mem_43_EEP_Ipw_pxFtfcConfig = Config->pxFtfcIpConfig;
    Mem_43_EEP_Ipw_ReportBrownOut();
#endif

    /* Check the status after finishing */
    if (FTFC_MEM_EEP_IP_STATUS_OK != FtfcStatus)
    {
        Mem_43_EEP_Ipw_ReportFailure(FtfcStatus, InstanceIndex, (uint32)MEM_43_EEP_INIT_ID);
        RetVal = (Std_ReturnType)E_NOT_OK;
    }

    return RetVal;
}

/**
* @brief         Mem_43_EEP_Ipw_DeInit
* @details       DeInitialize MEM_43_EEP module
*
* @param[in]     Config SD init configuration
*
* @return        status
* @pre           N/A
*/
Std_ReturnType Mem_43_EEP_Ipw_DeInit(Mem_43_EEP_ConfigType const * Config, uint32 InstanceIndex)
{
    Std_ReturnType Status = (Std_ReturnType)E_OK;
    (void)Config;
    (void)InstanceIndex;

    return Status;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @brief Check parameters for the Read job.
 *
 * @param[in] SrcAddress
 * @param[in] pu8DestAddress
 * @param[in] Length
 *
 * @return Ok / not ok to proceed with the read operation.
 * @retval E_OK
 * @retval E_NOT_OK
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
Std_ReturnType Mem_43_EEP_Ipw_CheckReadParams(Mem_43_EEP_AddressType SrcAddress,
                                        Mem_43_EEP_DataType  const * pu8DestAddress,
                                        Mem_43_EEP_LengthType Length, uint32 InstanceIndex
                                       )
{
    Std_ReturnType RetVal;

    /* Check the validity of Source Address */
    if (SrcAddress >= FTFC_MEM_EEP_IP_EEP_SIZE)
    {
    #if (MEM_43_EEP_DEV_ERROR_DETECT == STD_ON)
        (void) Det_ReportError((uint16)MEM_43_EEP_MODULE_ID, (uint8)InstanceIndex, MEM_43_EEP_READ_ID, MEM_43_EEP_E_PARAM_ADDRESS);
    #endif
        RetVal = (Std_ReturnType)E_NOT_OK;
    }
    /* Check the validity of Length */
    else if ((0U == Length) || ((SrcAddress + Length) > FTFC_MEM_EEP_IP_EEP_SIZE))
    {
    #if (MEM_43_EEP_DEV_ERROR_DETECT == STD_ON)
        (void) Det_ReportError((uint16)MEM_43_EEP_MODULE_ID, (uint8)InstanceIndex, MEM_43_EEP_READ_ID, MEM_43_EEP_E_PARAM_LENGTH);
    #endif
        RetVal = (Std_ReturnType)E_NOT_OK;
    }
    /* Check the validity of destination address */
    else if (NULL_PTR == pu8DestAddress)
    {
    #if (MEM_43_EEP_DEV_ERROR_DETECT == STD_ON)
        (void) Det_ReportError((uint16)MEM_43_EEP_MODULE_ID, (uint8)InstanceIndex, MEM_43_EEP_READ_ID, MEM_43_EEP_E_PARAM_POINTER);
    #endif
        RetVal = (Std_ReturnType)E_NOT_OK;
    }
    else
    {
        RetVal = (Std_ReturnType)E_OK;
    }

#if (MEM_43_EEP_DEV_ERROR_DETECT == STD_OFF)
(void)InstanceIndex;
#endif

    return RetVal;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @brief Synchronous read.
 *
 * @param[out] pu8DestAddress
 *
 * @return Synchronous read op status.
 * @retval MEM_43_EEP_E_OK
 * @retval MEM_43_EEP_E_FAILED
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Std_ReturnType Mem_43_EEP_Ipw_Read(Mem_43_EEP_AddressType SrcAddress,
                             Mem_43_EEP_DataType * pu8DestAddress,
                             Mem_43_EEP_LengthType TransfLength, 
                             uint32 InstanceIndex
                            )
{
    Std_ReturnType RetVal = (Std_ReturnType)E_NOT_OK;
    Ftfc_Mem_Eep_Ip_StatusType FtfcStatus = FTFC_MEM_EEP_IP_STATUS_OK;
    (void)InstanceIndex;

    /* Call Ip read function */
    FtfcStatus = Ftfc_Mem_Eep_Ip_Read(SrcAddress, pu8DestAddress, TransfLength);

    /* Check status of read operation */
    if (FTFC_MEM_EEP_IP_STATUS_OK == FtfcStatus)
    {
        RetVal = (Std_ReturnType)E_OK;
    }

    return RetVal;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @brief  Check the params given for a Compare job.
 *
 * @param[in] SrcAddress
 * @param[in] pu8DestAddress
 * @param[in] Length
 *
 * @return Ok / not ok to proceed to the compare operation with the given params.
 * @retval E_OK
 * @retval E_NOT_OK
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
Std_ReturnType Mem_43_EEP_Ipw_CheckCompareParams(Mem_43_EEP_AddressType SrcAddress,
                                          const Mem_43_EEP_DataType * pu8DestAddress,
                                          Mem_43_EEP_LengthType Length, uint32 InstanceIndex
                                         )
{
    Std_ReturnType retVal;

    /* Check the validity of source address */
    if (SrcAddress >= FTFC_MEM_EEP_IP_EEP_SIZE)
    {
    #if (MEM_43_EEP_DEV_ERROR_DETECT == STD_ON)
        (void) Det_ReportError ((uint16)MEM_43_EEP_MODULE_ID, (uint8)InstanceIndex, MEM_43_EEP_HWSPECIFICSERVICE_ID, MEM_43_EEP_E_PARAM_ADDRESS);
    #endif
        retVal = (Std_ReturnType)E_NOT_OK;
    }
    /* Check the validity of length */
    else if ((0U == Length) || ((SrcAddress + Length) > FTFC_MEM_EEP_IP_EEP_SIZE))
    {
    #if (MEM_43_EEP_DEV_ERROR_DETECT == STD_ON)
        (void) Det_ReportError ((uint16)MEM_43_EEP_MODULE_ID, (uint8)InstanceIndex, MEM_43_EEP_HWSPECIFICSERVICE_ID, MEM_43_EEP_E_PARAM_LENGTH);
    #endif
        retVal = (Std_ReturnType)E_NOT_OK;
    }
    /* Check the validity of destination address */
    else if (NULL_PTR == pu8DestAddress)
    {
    #if (MEM_43_EEP_DEV_ERROR_DETECT == STD_ON)
        (void) Det_ReportError ((uint16)MEM_43_EEP_MODULE_ID, (uint8)InstanceIndex, MEM_43_EEP_HWSPECIFICSERVICE_ID, MEM_43_EEP_E_PARAM_POINTER);
    #endif
        retVal = (Std_ReturnType)E_NOT_OK;
    }
    else
    {
        retVal = (Std_ReturnType)E_OK;
    }

#if (MEM_43_EEP_DEV_ERROR_DETECT == STD_OFF)
(void)InstanceIndex;
#endif

    return retVal;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @brief Compare an EEPROM area with a given buffer.
 *
 * @param[in] SrcAddress
 * @param[in] pu8DestAddress
 * @param[in] Length
 *
 * @return the comparison result:
 * @retval MEM_43_EEP_COMPARE_E_OK                 memory contents match
 * @retval MEM_43_EEP_COMPARE_E_BLOCK_INCONSISTENT memory contents do not match
 * @retval MEM_43_EEP_COMPARE_E_FAILED             compare operation failed
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
Mem_43_EEP_CompareCheckType Mem_43_EEP_Ipw_CompareSync(Mem_43_EEP_AddressType SrcAddress,
                                   const Mem_43_EEP_DataType *pu8DestAddress,
                                   Mem_43_EEP_LengthType TransfLength, 
                                   uint32 InstanceIndex
                                  )
{
    Mem_43_EEP_CompareCheckType RetVal  = MEM_43_EEP_COMPARE_E_OK;
    Ftfc_Mem_Eep_Ip_StatusType FtfcStatus  = FTFC_MEM_EEP_IP_STATUS_OK;
    (void)InstanceIndex;

    /* Call Ip compare function */
    FtfcStatus = Ftfc_Mem_Eep_Ip_Compare(SrcAddress, pu8DestAddress, TransfLength);

    /* Check the status after compare operation */
    switch (FtfcStatus)
    {
        case FTFC_MEM_EEP_IP_STATUS_OK:
            RetVal = MEM_43_EEP_COMPARE_E_OK;
        break;

        case FTFC_MEM_EEP_IP_STATUS_BLOCK_INCONSISTENT:
            RetVal = MEM_43_EEP_COMPARE_E_BLOCK_INCONSISTENT;
        break;

        default:
            RetVal = MEM_43_EEP_COMPARE_E_FAILED;
        break;
    }

    return RetVal;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @brief       Check if the parameters are ok and the HLD can start the blank check job.
 *
 * @param[in]   TargetAddress
 * @param[in]   Length
 *
 * @return      Ok / Not ok to start the blanck check job.
 * @retval      E_OK
 * @retval      E_NOT_OK
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
Std_ReturnType Mem_43_EEP_Ipw_CheckBlankCheckParams(Mem_43_EEP_AddressType TargetAddress,
                                                 Mem_43_EEP_LengthType Length, uint32 InstanceIndex)
{
    Std_ReturnType RetVal;

    /* Check the validity of Source Address */
    if (TargetAddress >= FTFC_MEM_EEP_IP_EEP_SIZE)
    {
    #if (MEM_43_EEP_DEV_ERROR_DETECT == STD_ON)
        (void) Det_ReportError ((uint16)MEM_43_EEP_MODULE_ID, (uint8)InstanceIndex, MEM_43_EEP_BLANKCHECK_ID, MEM_43_EEP_E_PARAM_ADDRESS );
    #endif
        RetVal = (Std_ReturnType)E_NOT_OK;
    }
    /* Check the validity of Length */
    else if ((0U == Length) || ((TargetAddress + Length) > FTFC_MEM_EEP_IP_EEP_SIZE))
    {
    #if (MEM_43_EEP_DEV_ERROR_DETECT == STD_ON)
        (void) Det_ReportError ((uint16)MEM_43_EEP_MODULE_ID, (uint8)InstanceIndex, MEM_43_EEP_BLANKCHECK_ID, MEM_43_EEP_E_PARAM_LENGTH );
    #endif
        RetVal = (Std_ReturnType)E_NOT_OK;
    }
    else
    {
        RetVal = (Std_ReturnType)E_OK;
    }

#if (MEM_43_EEP_DEV_ERROR_DETECT == STD_OFF)
(void)InstanceIndex;
#endif

    return RetVal;
}

/**
* @brief         Mem_43_EEP_Ipw_BlankCheck
* @details       Compare data for the given transfer length with 0
*
* @param[in]     EepromAddrIt   Address to start comparing
* @param[in]     BlankLength   Transfer length to compare
*
* @return        status
* @pre           The driver needs to be initialized.
*/
Mem_43_EEP_BlankCheckType Mem_43_EEP_Ipw_BlankCheck(Mem_43_EEP_AddressType EepromAddrIt,
                                      Mem_43_EEP_LengthType BlankLength, uint32 InstanceIndex
                                      )
{
    (void)InstanceIndex;
    uint8 BlankValues[MEM_43_EEP_MAX_READ_BLOCK_SIZE];
    Mem_43_EEP_BlankCheckType BlankCheckRetVal;
    Ftfc_Mem_Eep_Ip_StatusType FtfcStatus  = FTFC_MEM_EEP_IP_STATUS_OK;

    /* Fill temp array*/
    for(Mem_43_EEP_LengthType i = 0U; i < BlankLength; i++)
    {
        BlankValues[i] = 255U;
    }

    /* Call Ip compare function */
    FtfcStatus = Ftfc_Mem_Eep_Ip_Compare((Ftfc_Mem_Eep_Ip_AddressType)EepromAddrIt, BlankValues, (Ftfc_Mem_Eep_Ip_LengthType)BlankLength);

    /* Check the status after compare operation */
    switch (FtfcStatus)
    {
        case FTFC_MEM_EEP_IP_STATUS_OK:
            BlankCheckRetVal = MEM_43_EEP_BLANKCHECK_E_OK;
        break;

        case FTFC_MEM_EEP_IP_STATUS_BLOCK_INCONSISTENT:
            BlankCheckRetVal = MEM_43_EEP_BLANKCHECK_E_INCONSISTENT;
        break;

        default:
            BlankCheckRetVal = MEM_43_EEP_BLANKCHECK_E_NOT_OK;
        break;
    }

    return BlankCheckRetVal;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @brief       Check if the parameters are ok and the HLD can start the erase job.
 *
 * @param[in]   Address
 * @param[in]   Length
 *
 * @return      Ok / Not ok to start the erase job.
 * @retval      E_OK
 * @retval      E_NOT_OK
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
Std_ReturnType Mem_43_EEP_Ipw_CheckEraseParams (Mem_43_EEP_AddressType Address,
                                         Mem_43_EEP_LengthType Length,
                                         uint32 InstanceIndex
                                        )
{
    Std_ReturnType retVal;

    /* Check the address */
    if (Address >= FTFC_MEM_EEP_IP_EEP_SIZE)
    {
    #if (MEM_43_EEP_DEV_ERROR_DETECT == STD_ON)
        (void)Det_ReportError((uint16)MEM_43_EEP_MODULE_ID, (uint8)InstanceIndex, MEM_43_EEP_ERASE_ID, MEM_43_EEP_E_PARAM_ADDRESS);
    #endif
        retVal = (Std_ReturnType)E_NOT_OK;
    }
    /* Check the length */
    else if ((0U == Length) || ((Address + Length) > FTFC_MEM_EEP_IP_EEP_SIZE))
    {
    #if (MEM_43_EEP_DEV_ERROR_DETECT == STD_ON)
        (void)Det_ReportError((uint16)MEM_43_EEP_MODULE_ID, (uint8)InstanceIndex, MEM_43_EEP_ERASE_ID, MEM_43_EEP_E_PARAM_LENGTH);
    #endif
        retVal = (Std_ReturnType)E_NOT_OK;
    }
    else
    {
        retVal = (Std_ReturnType)E_OK;
    }

#if (MEM_43_EEP_DEV_ERROR_DETECT == STD_OFF)
(void)InstanceIndex;
#endif

    return retVal;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @brief Erase starting from @p Address up to Length bytes.
 *
 * @param[in] address
 * @param[in] length
 *
 * @return Erase status
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
Mem_43_EEP_ReturnType Mem_43_EEP_Ipw_Erase (Mem_43_EEP_AddressType Address,
                              Mem_43_EEP_LengthType Length, uint32 InstanceIndex
                             )
{
    Mem_43_EEP_AddressType EraseAdd = Address;
    Mem_43_EEP_LengthType LengthRemaining = Length;

    Mem_43_EEP_ReturnType RetVal = MEM_43_EEP_E_FAILED;
    Ftfc_Mem_Eep_Ip_StatusType FtfcStatus = FTFC_MEM_EEP_IP_STATUS_OK;

#if (MEM_43_EEP_ASYNC_ERASE_OPERATIONS_ENABLED == STD_ON)
    Ftfc_Mem_Eep_Ip_PageSizeType PageSize = FTFC_MEM_EEP_IP_PAGE_BYTE;

    if (FALSE == Mem_43_EEP_Ipw_xAsyncJob.InProgress)
    {
        Mem_43_EEP_Ipw_xAsyncJob.DestAddress = EraseAdd;
        Mem_43_EEP_Ipw_xAsyncJob.SrcAddress = NULL_PTR;
        Mem_43_EEP_Ipw_xAsyncJob.Length = LengthRemaining;

        PageSize = Mem_43_EEP_Ipw_AlignedPageSize(Mem_43_EEP_Ipw_xAsyncJob.DestAddress, 0U, Mem_43_EEP_Ipw_xAsyncJob.Length);
        FtfcStatus = Ftfc_Mem_Eep_Ip_Erase(Mem_43_EEP_Ipw_xAsyncJob.DestAddress, PageSize, TRUE);

        if (FTFC_MEM_EEP_IP_STATUS_PENDING == FtfcStatus)
        {
            Mem_43_EEP_Ipw_xAsyncJob.DestAddress += (Ftfc_Mem_Eep_Ip_AddressType)PageSize;
            Mem_43_EEP_Ipw_xAsyncJob.Length -= (Ftfc_Mem_Eep_Ip_LengthType)PageSize;

            Mem_43_EEP_Ipw_xAsyncJob.InProgress = TRUE;
            RetVal = MEM_43_EEP_E_PENDING;
        }
    }
    else
    {
        FtfcStatus = Ftfc_Mem_Eep_Ip_GetJobResult();
        switch (FtfcStatus)
        {
            case FTFC_MEM_EEP_IP_STATUS_PENDING:
                RetVal = MEM_43_EEP_E_PENDING;
            break;

            case FTFC_MEM_EEP_IP_STATUS_OK:
                if (Mem_43_EEP_Ipw_xAsyncJob.Length > 0U)
                {
                    PageSize = Mem_43_EEP_Ipw_AlignedPageSize(Mem_43_EEP_Ipw_xAsyncJob.DestAddress, 0U, Mem_43_EEP_Ipw_xAsyncJob.Length);
                    FtfcStatus = Ftfc_Mem_Eep_Ip_Erase(Mem_43_EEP_Ipw_xAsyncJob.DestAddress, PageSize, TRUE);

                    if (FTFC_MEM_EEP_IP_STATUS_PENDING == FtfcStatus)
                    {
                        Mem_43_EEP_Ipw_xAsyncJob.DestAddress += (Ftfc_Mem_Eep_Ip_AddressType)PageSize;
                        Mem_43_EEP_Ipw_xAsyncJob.Length -= (Ftfc_Mem_Eep_Ip_LengthType)PageSize;
                        RetVal = MEM_43_EEP_E_PENDING;
                    }
                    else
                    {
                        Mem_43_EEP_Ipw_xAsyncJob.InProgress = FALSE;
                    }
                }
                else
                {
                    Mem_43_EEP_Ipw_xAsyncJob.InProgress = FALSE;
                    RetVal = MEM_43_EEP_E_OK;
                }
            break;

            default:
                Mem_43_EEP_Ipw_xAsyncJob.InProgress = FALSE;
            break;
        }
    }

#else /* MEM_43_EEP_ASYNC_ERASE_OPERATIONS_ENABLED */

    Ftfc_Mem_Eep_Ip_PageSizeType PageSize;

    while ((LengthRemaining > 0U) && (FTFC_MEM_EEP_IP_STATUS_OK == FtfcStatus))
    {
        PageSize = Mem_43_EEP_Ipw_AlignedPageSize(EraseAdd, 0U, LengthRemaining);
        FtfcStatus = Ftfc_Mem_Eep_Ip_Erase(EraseAdd, PageSize, FALSE);
        LengthRemaining -= (Mem_43_EEP_LengthType)PageSize;
        EraseAdd += (Mem_43_EEP_AddressType)PageSize;
    }

    if (FTFC_MEM_EEP_IP_STATUS_OK == FtfcStatus)
    {
        RetVal = MEM_43_EEP_E_OK;
    }

#endif /* MEM_43_EEP_ASYNC_ERASE_OPERATIONS_ENABLED */

    /* Repor the status after finishing */
    Mem_43_EEP_Ipw_ReportFailure(FtfcStatus, InstanceIndex, (uint32)MEM_43_EEP_ERASE_ID);
    return RetVal;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @brief       Check the parameters given for a Write job.
 *
 * @param[in]   DestAddress
 * @param[in]   pu8SrcAddress
 * @param[in]   Length
 *
 * @return Ok / not ok to start the write job with those parameters.
 * @retval E_OK
 * @retval E_NOT_OK
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
Std_ReturnType Mem_43_EEP_Ipw_CheckWriteParams (Mem_43_EEP_AddressType DestAddress,
                                         uint8 const * pu8SrcAddress,
                                         Mem_43_EEP_LengthType Length,
                                         uint32 InstanceIndex
                                        )
{
    Std_ReturnType retVal;

    /* Check the validity of destination address */
    if (DestAddress >= FTFC_MEM_EEP_IP_EEP_SIZE)
    {
    #if (MEM_43_EEP_DEV_ERROR_DETECT == STD_ON)
        (void) Det_ReportError ((uint16)MEM_43_EEP_MODULE_ID, (uint8)InstanceIndex, MEM_43_EEP_WRITE_ID, MEM_43_EEP_E_PARAM_ADDRESS);
    #endif
        retVal = (Std_ReturnType)E_NOT_OK;
    }
    /* Check the validity of length */
    else if ((0U == Length) || ((DestAddress + Length) > FTFC_MEM_EEP_IP_EEP_SIZE))
    {
    #if (MEM_43_EEP_DEV_ERROR_DETECT == STD_ON)
        (void) Det_ReportError ((uint16)MEM_43_EEP_MODULE_ID, (uint8)InstanceIndex, MEM_43_EEP_WRITE_ID, MEM_43_EEP_E_PARAM_LENGTH);
    #endif
        retVal = (Std_ReturnType)E_NOT_OK;
    }
    /* Check the validity of source address */
    else if (NULL_PTR == pu8SrcAddress)
    {
    #if (MEM_43_EEP_DEV_ERROR_DETECT == STD_ON)
        (void) Det_ReportError ((uint16)MEM_43_EEP_MODULE_ID, (uint8)InstanceIndex, MEM_43_EEP_WRITE_ID, MEM_43_EEP_E_PARAM_POINTER);
    #endif
        retVal = (Std_ReturnType)E_NOT_OK;
    }
    else
    {
        retVal = (Std_ReturnType)E_OK;
    }

#if (MEM_43_EEP_DEV_ERROR_DETECT == STD_OFF)
(void)InstanceIndex;
#endif

    return retVal;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @brief Write operation.
 *
 * @param[in] DestAddress
 * @param[in] pu8SrcAddress
 * @param[in] Length
 *
 * @return The write call may end in one of the following states:
 * @retval MEM_43_EEP_E_OK       job jas finished successfully
 * @retval MEM_43_EEP_E_PENDING  async: the job is in progress
 * @retval MEM_43_EEP_E_FAILED   an error has occurred
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
Mem_43_EEP_ReturnType Mem_43_EEP_Ipw_Write (Mem_43_EEP_AddressType DestAddress,
                              uint8 const * pu8SrcAddress,
                              Mem_43_EEP_LengthType Length,
                              uint32 InstanceIndex
                             )
{
    Mem_43_EEP_ReturnType RetVal = MEM_43_EEP_E_FAILED;
    Ftfc_Mem_Eep_Ip_StatusType FtfcStatus = FTFC_MEM_EEP_IP_STATUS_OK;
    Mem_43_EEP_AddressType DestAdd = DestAddress;
    uint8 const * pu8SrcAdd = pu8SrcAddress;
    Mem_43_EEP_LengthType LengthRemaining = Length;

#if (MEM_43_EEP_ASYNC_WRITE_OPERATIONS_ENABLED == STD_ON)
    Ftfc_Mem_Eep_Ip_PageSizeType PageSize = FTFC_MEM_EEP_IP_PAGE_BYTE;

    if (FALSE == Mem_43_EEP_Ipw_xAsyncJob.InProgress)
    {
        Mem_43_EEP_Ipw_xAsyncJob.DestAddress = DestAdd;
        Mem_43_EEP_Ipw_xAsyncJob.SrcAddress = pu8SrcAdd;
        Mem_43_EEP_Ipw_xAsyncJob.Length = LengthRemaining;

        PageSize = Mem_43_EEP_Ipw_AlignedPageSize(Mem_43_EEP_Ipw_xAsyncJob.DestAddress, (Mem_43_EEP_AddressType)Mem_43_EEP_Ipw_xAsyncJob.SrcAddress, Mem_43_EEP_Ipw_xAsyncJob.Length);
        FtfcStatus = Ftfc_Mem_Eep_Ip_Write(Mem_43_EEP_Ipw_xAsyncJob.DestAddress, Mem_43_EEP_Ipw_xAsyncJob.SrcAddress, PageSize, TRUE);

        if (FTFC_MEM_EEP_IP_STATUS_PENDING == FtfcStatus)
        {
            Mem_43_EEP_Ipw_xAsyncJob.DestAddress += (Ftfc_Mem_Eep_Ip_AddressType)PageSize;
            Mem_43_EEP_Ipw_xAsyncJob.SrcAddress = (uint8 const *)((Mem_43_EEP_AddressType)Mem_43_EEP_Ipw_xAsyncJob.SrcAddress + (uint8)PageSize);
            Mem_43_EEP_Ipw_xAsyncJob.Length -= (Ftfc_Mem_Eep_Ip_LengthType)PageSize;

            Mem_43_EEP_Ipw_xAsyncJob.InProgress = TRUE;
            RetVal = MEM_43_EEP_E_PENDING;
        }
    }
    else
    {
        FtfcStatus = Ftfc_Mem_Eep_Ip_GetJobResult();
        switch (FtfcStatus)
        {
            case FTFC_MEM_EEP_IP_STATUS_PENDING:
                RetVal = MEM_43_EEP_E_PENDING;
            break;

            case FTFC_MEM_EEP_IP_STATUS_OK:
                if (Mem_43_EEP_Ipw_xAsyncJob.Length > 0U)
                {
                    PageSize = Mem_43_EEP_Ipw_AlignedPageSize(Mem_43_EEP_Ipw_xAsyncJob.DestAddress, (Mem_43_EEP_AddressType)Mem_43_EEP_Ipw_xAsyncJob.SrcAddress, Mem_43_EEP_Ipw_xAsyncJob.Length);
                    FtfcStatus = Ftfc_Mem_Eep_Ip_Write(Mem_43_EEP_Ipw_xAsyncJob.DestAddress, Mem_43_EEP_Ipw_xAsyncJob.SrcAddress, PageSize, TRUE);

                    if (FTFC_MEM_EEP_IP_STATUS_PENDING == FtfcStatus)
                    {
                        Mem_43_EEP_Ipw_xAsyncJob.DestAddress += (Ftfc_Mem_Eep_Ip_AddressType)PageSize;
                        Mem_43_EEP_Ipw_xAsyncJob.SrcAddress = (uint8 const *)((Mem_43_EEP_AddressType)Mem_43_EEP_Ipw_xAsyncJob.SrcAddress + (uint8)PageSize);
                        Mem_43_EEP_Ipw_xAsyncJob.Length -= (Ftfc_Mem_Eep_Ip_LengthType)PageSize;
                        RetVal = MEM_43_EEP_E_PENDING;
                    }
                    else
                    {
                        Mem_43_EEP_Ipw_xAsyncJob.InProgress = FALSE;
                    }
                }
                else
                {
                    Mem_43_EEP_Ipw_xAsyncJob.InProgress = FALSE;
                    RetVal = MEM_43_EEP_E_OK;
                }
            break;

            default:
                Mem_43_EEP_Ipw_xAsyncJob.InProgress = FALSE;
            break;
        }
    }

#else /* MEM_43_EEP_ASYNC_WRITE_OPERATIONS_ENABLED */

    Ftfc_Mem_Eep_Ip_PageSizeType PageSize;

    while ((LengthRemaining > 0U) && (FTFC_MEM_EEP_IP_STATUS_OK == FtfcStatus))
    {
        PageSize = Mem_43_EEP_Ipw_AlignedPageSize(DestAdd, (Mem_43_EEP_AddressType)pu8SrcAdd, LengthRemaining);
        FtfcStatus = Ftfc_Mem_Eep_Ip_Write(DestAdd, pu8SrcAdd, PageSize, FALSE);
        LengthRemaining -= (Mem_43_EEP_LengthType)PageSize;
        DestAdd += (Mem_43_EEP_AddressType)PageSize;
        pu8SrcAdd = (uint8*)((Mem_43_EEP_AddressType)pu8SrcAdd + (Mem_43_EEP_AddressType)PageSize);
    }

    if (FTFC_MEM_EEP_IP_STATUS_OK == FtfcStatus)
    {
        RetVal = MEM_43_EEP_E_OK;
    }

#endif /* MEM_43_EEP_ASYNC_WRITE_OPERATIONS_ENABLED */

    /* Report the status of write operation */
    Mem_43_EEP_Ipw_ReportFailure(FtfcStatus, InstanceIndex, (uint32)MEM_43_EEP_WRITE_ID);
    return RetVal;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @brief  Cancel the current operation.
 *
 * @return the status of the IP
 * @retval E_OK       the FTFC operation finished
 * @retval E_NOT_OK  a timeout has occured while waiting for the HW IP
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
Std_ReturnType Mem_43_EEP_Ipw_Cancel(uint32 InstanceIndex)
{
    (void)InstanceIndex;
    Std_ReturnType Status = (Std_ReturnType)E_OK;
    uint32 Timeout = OsIf_MicrosToTicks(FTFC_MEM_EEP_IP_ABORT_TIMEOUT, FTFC_MEM_EEP_IP_TIMEOUT_TYPE);
    uint32 CurrentTicks = OsIf_GetCounter(FTFC_MEM_EEP_IP_TIMEOUT_TYPE);
    uint32 ElapsedTicks = 0U;

    /* wait until FlexRam is ready or abort if timeout is reached */
    while (FTFC_MEM_EEP_IP_STATUS_PENDING == Ftfc_Mem_Eep_Ip_GetJobResult())
    {
        ElapsedTicks += OsIf_GetElapsed(&CurrentTicks, FTFC_MEM_EEP_IP_TIMEOUT_TYPE);
        if (ElapsedTicks >= Timeout)
        {
            Status =  (Std_ReturnType)E_NOT_OK;
            break;
        }
    }

#if ((MEM_43_EEP_ASYNC_ERASE_OPERATIONS_ENABLED == STD_ON) || \
     (MEM_43_EEP_ASYNC_WRITE_OPERATIONS_ENABLED == STD_ON))
    Mem_43_EEP_Ipw_xAsyncJob.InProgress = FALSE;
#endif

    return Status;
}

#if (FTFC_MEM_EEP_IP_QUICK_WRITE == STD_ON)

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @brief       Check the parameters given for a QuickWrite job.
 *
 * @param[in]   QuickWriteConfig
 * @param[in]   InstanceIndex
 *
 * @return Ok / not ok to start the quick write job with those parameters.
 * @retval E_OK
 * @retval E_NOT_OK
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
Std_ReturnType Mem_43_EEP_Ipw_CheckQuickWriteParams (Mem_43_EEP_QuickWriteConfigType QuickWriteConfig,
                                                                        uint32 InstanceIndex
                                                                        )
{
    Std_ReturnType retVal = E_NOT_OK;
    /* Both the lowest two bits are zero: 4 bytes aligned */
    Mem_43_EEP_AddressType CheckAddress = (QuickWriteConfig.targetAddress | (uint32)(QuickWriteConfig.sourceDataPtr)) & 3U;
    /* Check the validity of destination address */
    if ((QuickWriteConfig.targetAddress >= FTFC_MEM_EEP_IP_EEP_SIZE) || (CheckAddress != 0U))
    {
    #if (MEM_43_EEP_DEV_ERROR_DETECT == STD_ON)
        (void) Det_ReportError((uint16)MEM_43_EEP_MODULE_ID, (uint8)InstanceIndex, MEM_43_EEP_HWSPECIFICSERVICE_ID, MEM_43_EEP_E_PARAM_ADDRESS);
    #endif
    }
    /* Check the validity of length */
    else if ((0U == QuickWriteConfig.length) || ((QuickWriteConfig.targetAddress + QuickWriteConfig.length) > FTFC_MEM_EEP_IP_EEP_SIZE) ||
        (QuickWriteConfig.u16QuickWritesLength < 16U) || (QuickWriteConfig.u16QuickWritesLength > 512U) ||
        ((QuickWriteConfig.u16QuickWritesLength % 4U) != 0U) || ((QuickWriteConfig.length % QuickWriteConfig.u16QuickWritesLength) != 0U))
    {
    #if (MEM_43_EEP_DEV_ERROR_DETECT == STD_ON)
        (void) Det_ReportError((uint16)MEM_43_EEP_MODULE_ID, (uint8)InstanceIndex, MEM_43_EEP_HWSPECIFICSERVICE_ID, MEM_43_EEP_E_PARAM_LENGTH);
    #endif
    }
    /* Check the validity of source add */
    else if (NULL_PTR == QuickWriteConfig.sourceDataPtr)
    {
    #if (MEM_43_EEP_DEV_ERROR_DETECT == STD_ON)
        (void) Det_ReportError((uint16)MEM_43_EEP_MODULE_ID, (uint8)InstanceIndex, MEM_43_EEP_HWSPECIFICSERVICE_ID, MEM_43_EEP_E_PARAM_POINTER);
    #endif
    }
    else
    {
        retVal = E_OK;
    }

#if (MEM_43_EEP_DEV_ERROR_DETECT == STD_OFF)
(void)InstanceIndex;
#endif

    return retVal;
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @brief Quick Write operation.
 *
 * @param[in] DestAddress
 * @param[in] pu8SrcAddress
 * @param[in] Length
 *
 * @return The quick write call may end in one of the following states:
 * @retval MEM_43_EEP_E_OK
 * @retval MEM_43_EEP_E_FAILED
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
Mem_43_EEP_ReturnType Mem_43_EEP_Ipw_QuickWrite (Mem_43_EEP_QuickWriteConfigType QuickWriteConfig,
                                                                                   uint32 InstanceIndex)
{
    Mem_43_EEP_ReturnType RetVal = MEM_43_EEP_E_OK;
    Ftfc_Mem_Eep_Ip_StatusType FtfcStatus = FTFC_MEM_EEP_IP_STATUS_OK;

    FtfcStatus = Ftfc_Mem_Eep_Ip_QuickWrite(QuickWriteConfig.targetAddress, QuickWriteConfig.sourceDataPtr, QuickWriteConfig.u16QuickWritesLength);

    /* Check status after operation finish  */
    if (FTFC_MEM_EEP_IP_STATUS_OK != FtfcStatus)
    {
        RetVal = MEM_43_EEP_E_FAILED;
        Mem_43_EEP_Ipw_ReportFailure(FtfcStatus, InstanceIndex, (uint32)MEM_43_EEP_HWSPECIFICSERVICE_ID);
    }

    return RetVal;
}

#endif /* MEM_43_EEP_SERVICE_QUICKWRITES_API */

/*==================================================================================================
*                                        LOCAL FUNCTIONS
==================================================================================================*/

#if (FTFC_DISABLE_DEM_REPORT_ERROR_STATUS == STD_OFF)

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @brief   Reports the brownout code to DEM.
 * @details The brownout code is read at init.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
static void Mem_43_EEP_Ipw_ReportBrownOut (void)
{
    switch (Ftfc_Mem_Eep_Ip_GetBrownOutCode())
    {
#if (FTFC_DEM_EVENT_BO_MAINTENANCE == STD_ON)
        case FTFC_MEM_EEP_IP_BO_DURING_MAINTENANCE:
            if ((uint32)STD_ON == Mem_43_EEP_Ipw_pxFtfcConfig->E_BOMaintenanceCfg.state)
            {
                (void) Dem_SetEventStatus((Dem_EventIdType)Mem_43_EEP_Ipw_pxFtfcConfig->E_BOMaintenanceCfg.id, DEM_EVENT_STATUS_FAILED);
            }
        break;
#endif
#if (FTFC_DEM_EVENT_BO_QUICK_WRITE == STD_ON)
        case FTFC_MEM_EEP_IP_BO_DURING_QUICK_WRITES:
            if ((uint32)STD_ON == Mem_43_EEP_Ipw_pxFtfcConfig->E_BOQuickWriteCfg.state)
            {
                (void) Dem_SetEventStatus((Dem_EventIdType)Mem_43_EEP_Ipw_pxFtfcConfig->E_BOQuickWriteCfg.id, DEM_EVENT_STATUS_FAILED);
            }
        break;
#endif
#if (FTFC_DEM_EVENT_BO_NORMAL_WRITE == STD_ON)
        case FTFC_MEM_EEP_IP_BO_DURING_NORMAL_WRITES:
            if ((uint32)STD_ON == Mem_43_EEP_Ipw_pxFtfcConfig->E_BONormalWriteCfg.state)
            {
                (void) Dem_SetEventStatus((Dem_EventIdType)Mem_43_EEP_Ipw_pxFtfcConfig->E_BONormalWriteCfg.id, DEM_EVENT_STATUS_FAILED);
            }
        break;
#endif
        case FTFC_MEM_EEP_IP_NO_BO_DETECTED:
            /* Nothing to report */
        break;

        default:
            /* corrupted value */
        break;
    }
}

#endif /* (FTFC_DISABLE_DEM_REPORT_ERROR_STATUS == STD_OFF) */

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @brief Report failures coming from IPL to DET / DEM.
 *
 * @param[in] FtfcStatus
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
static void Mem_43_EEP_Ipw_ReportFailure (Ftfc_Mem_Eep_Ip_StatusType FtfcStatus,
                                                                        uint32 InstanceId,
                                                                       uint32 ApiID
                                                          )
{
    switch (FtfcStatus)
    {
        case FTFC_MEM_EEP_IP_STATUS_TIMEOUT:
            (void) Det_ReportRuntimeError((uint16)MEM_43_EEP_MODULE_ID, (uint8)InstanceId, (uint8)ApiID, (uint8)MEM_43_EEP_E_TIMEOUT);
        break;

    #if (FTFC_MEM_EEP_IP_HIGH_TEMP_CHIP == STD_ON)
        case FTFC_MEM_EEP_IP_STATUS_FAILED_MGSTAT:
        #if (FTFC_DISABLE_DEM_REPORT_ERROR_STATUS == STD_OFF)
        #if (FTFC_DEM_EVENT_BO_MGSTAT_BLOCK == STD_ON)
            if ((uint32)STD_ON == Mem_43_EEP_Ipw_pxFtfcConfig->E_MGSTAT_BlockCfg.state)
            {
                (void) Dem_SetEventStatus((Dem_EventIdType)Mem_43_EEP_Ipw_pxFtfcConfig->E_MGSTAT_BlockCfg.id, DEM_EVENT_STATUS_FAILED);
            }
        #endif
        #endif
        break;
    #endif

        default:
            /* nothing to report */
        break;
    }
}

/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * @brief Compute the page size, considering the addresses alignment and the remaining bytes.
 *
 * @param[in]  FlexramAddress the FlexRAM address
 * @param[in]  RamAddress the RAM address
 * @param[in]  Length how many bytes are left to process
 *
 * @return the page size
 * @retval FTFC_MEM_EEP_IP_PAGE_BYTE     for unaligned operations
 * @retval FTFC_MEM_EEP_IP_PAGE_WORD     16-bit aligned operations
 * @retval FTFC_MEM_EEP_IP_PAGE_LONGWORD 32-bit aligned operations
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
static Ftfc_Mem_Eep_Ip_PageSizeType Mem_43_EEP_Ipw_AlignedPageSize (Ftfc_Mem_Eep_Ip_AddressType FlexramAddress,
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

#define MEM_43_EEP_STOP_SEC_CODE
#include "Mem_43_EEP_MemMap.h"

#ifdef __cplusplus
}
#endif

/** @}*/

