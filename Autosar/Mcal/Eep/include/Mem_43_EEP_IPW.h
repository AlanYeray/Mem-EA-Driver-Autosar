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

#ifndef MEM_43_EEP_IPW_H
#define MEM_43_EEP_IPW_H

/**
* @file
*
* @addtogroup MEM_43_EEP_DRIVER Mem_43_EEProm Driver
* @{
*/

#ifdef __cplusplus
extern "C" {
#endif

/*==================================================================================================
                                         INCLUDE FILES
 1) system and project includes
 2) needed interfaces from external units
 3) internal and external interfaces from this unit
==================================================================================================*/
#include "Mem_43_EEP_InternalTypes.h"
#include "Ftfc_Mem_Eep_Ip.h"

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define MEM_43_EEP_IPW_VENDOR_ID_H                    43
#define MEM_43_EEP_IPW_AR_RELEASE_MAJOR_VERSION_H     4
#define MEM_43_EEP_IPW_AR_RELEASE_MINOR_VERSION_H     7
#define MEM_43_EEP_IPW_AR_RELEASE_REVISION_VERSION_H  0
#define MEM_43_EEP_IPW_SW_MAJOR_VERSION_H             2
#define MEM_43_EEP_IPW_SW_MINOR_VERSION_H             0
#define MEM_43_EEP_IPW_SW_PATCH_VERSION_H             0

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/

/* Check if header file and Mem_43_EEP_InternalTypes.h header file are of the same vendor */
#if (MEM_43_EEP_IPW_VENDOR_ID_H != MEM_43_EEP_INTERNALTYPES_VENDOR_ID)
    #error "Mem_43_EEP_IPW.h and Mem_43_EEP_InternalTypes.h have different vendor ids"
#endif
/* Check if header file and Mem_43_EEP_InternalTypes.h header file are of the same Autosar version */
#if ((MEM_43_EEP_IPW_AR_RELEASE_MAJOR_VERSION_H     != MEM_43_EEP_INTERNALTYPES_AR_RELEASE_MAJOR_VERSION) || \
     (MEM_43_EEP_IPW_AR_RELEASE_MINOR_VERSION_H     != MEM_43_EEP_INTERNALTYPES_AR_RELEASE_MINOR_VERSION) || \
     (MEM_43_EEP_IPW_AR_RELEASE_REVISION_VERSION_H  != MEM_43_EEP_INTERNALTYPES_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of Mem_43_EEP_IPW.h and Mem_43_EEP_InternalTypes.h are different"
#endif
/* Check if header file and Mem_43_EEP_InternalTypes.h header file are of the same software version */
#if ((MEM_43_EEP_IPW_SW_MAJOR_VERSION_H  != MEM_43_EEP_INTERNALTYPES_SW_MAJOR_VERSION) || \
     (MEM_43_EEP_IPW_SW_MINOR_VERSION_H  != MEM_43_EEP_INTERNALTYPES_SW_MINOR_VERSION) || \
     (MEM_43_EEP_IPW_SW_PATCH_VERSION_H  != MEM_43_EEP_INTERNALTYPES_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Mem_43_EEP_IPW.h and Mem_43_EEP_InternalTypes.h are different"
#endif


/* Version checks for Ftfc_Mem_Eep_Ip.h */
#if (MEM_43_EEP_IPW_VENDOR_ID_H != FTFC_MEM_EEP_IP_VENDOR_ID_H)
    #error "Mem_43_EEP_IPW.h and Ftfc_Mem_Eep_Ip.h have different vendor IDs!"
#endif
#if ((MEM_43_EEP_IPW_AR_RELEASE_MAJOR_VERSION_H    != FTFC_MEM_EEP_IP_AR_RELEASE_MAJOR_VERSION_H) || \
     (MEM_43_EEP_IPW_AR_RELEASE_MINOR_VERSION_H    != FTFC_MEM_EEP_IP_AR_RELEASE_MINOR_VERSION_H) || \
     (MEM_43_EEP_IPW_AR_RELEASE_REVISION_VERSION_H != FTFC_MEM_EEP_IP_AR_RELEASE_REVISION_VERSION_H) \
    )
    #error "Mem_43_EEP.h and Ftfc_Mem_Eep_Ip.h are for different AUTOSAR versions!"
#endif
#if ((MEM_43_EEP_IPW_SW_MAJOR_VERSION_H != FTFC_MEM_EEP_IP_SW_MAJOR_VERSION_H) || \
     (MEM_43_EEP_IPW_SW_MINOR_VERSION_H != FTFC_MEM_EEP_IP_SW_MINOR_VERSION_H) || \
     (MEM_43_EEP_IPW_SW_PATCH_VERSION_H != FTFC_MEM_EEP_IP_SW_PATCH_VERSION_H) \
    )
    #error "Mem_43_EEP_IPW.h and Ftfc_Mem_Eep_Ip.h have different SW versions!"
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
*                                  STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
*                                  GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
*                                       FUNCTION PROTOTYPES
==================================================================================================*/
#define MEM_43_EEP_START_SEC_CODE
#include "Mem_43_EEP_MemMap.h"

Std_ReturnType Mem_43_EEP_Ipw_Init(Mem_43_EEP_ConfigType const * Config, uint32 InstanceIndex);

Std_ReturnType Mem_43_EEP_Ipw_DeInit(Mem_43_EEP_ConfigType const * Config, uint32 InstanceIndex);
Std_ReturnType Mem_43_EEP_Ipw_CheckReadParams(Mem_43_EEP_AddressType SrcAddress,
                                        Mem_43_EEP_DataType  const * pu8DestAddress,
                                        Mem_43_EEP_LengthType Length, uint32 InstanceIndex
                                       );
Std_ReturnType Mem_43_EEP_Ipw_Read(Mem_43_EEP_AddressType SrcAddress,
                             Mem_43_EEP_DataType * pu8DestAddress,
                             Mem_43_EEP_LengthType TransfLength, uint32 InstanceIndex
                            );
Std_ReturnType Mem_43_EEP_Ipw_CheckCompareParams(Mem_43_EEP_AddressType SrcAddress,
                                          const Mem_43_EEP_DataType * pu8DestAddress,
                                          Mem_43_EEP_LengthType Length, uint32 InstanceIndex
                                         );

Mem_43_EEP_CompareCheckType Mem_43_EEP_Ipw_CompareSync(Mem_43_EEP_AddressType SrcAddress,
                                   const Mem_43_EEP_DataType *pu8DestAddress,
                                   Mem_43_EEP_LengthType TransfLength, uint32 InstanceIndex
                                  );

Std_ReturnType Mem_43_EEP_Ipw_CheckEraseParams(Mem_43_EEP_AddressType Address,
                                        Mem_43_EEP_LengthType Length, uint32 InstanceIndex
                                       );

Mem_43_EEP_ReturnType Mem_43_EEP_Ipw_Erase(Mem_43_EEP_AddressType Address,
                             Mem_43_EEP_LengthType Length, uint32 InstanceIndex
                            );

Std_ReturnType Mem_43_EEP_Ipw_CheckWriteParams(Mem_43_EEP_AddressType DestAddress,
                                        const Mem_43_EEP_DataType * pu8SrcAddress,
                                        Mem_43_EEP_LengthType Length, uint32 InstanceIndex
                                       );
Mem_43_EEP_ReturnType Mem_43_EEP_Ipw_Write(Mem_43_EEP_AddressType DestAddress,
                             const Mem_43_EEP_DataType *pu8SrcAddress,
                             Mem_43_EEP_LengthType Length, uint32 InstanceIndex
                            );
Std_ReturnType Mem_43_EEP_Ipw_CheckBlankCheckParams(Mem_43_EEP_AddressType TargetAddress,
                                                 Mem_43_EEP_LengthType Length, uint32 InstanceIndex
                                                 );

Mem_43_EEP_BlankCheckType Mem_43_EEP_Ipw_BlankCheck(Mem_43_EEP_AddressType EepromAddrIt,
                                      Mem_43_EEP_LengthType BlankLength, uint32 InstanceIndex
                                      );

Std_ReturnType Mem_43_EEP_Ipw_Cancel(uint32 InstanceIndex);

#if (FTFC_MEM_EEP_IP_QUICK_WRITE == STD_ON)
Std_ReturnType Mem_43_EEP_Ipw_CheckQuickWriteParams (Mem_43_EEP_QuickWriteConfigType QuickWriteConfig,
                                                                        uint32 InstanceIndex
                                                                        );
Mem_43_EEP_ReturnType Mem_43_EEP_Ipw_QuickWrite (Mem_43_EEP_QuickWriteConfigType QuickWriteConfig,
                                                                                    uint32 InstanceIndex
                                                                                    );
#endif

#define MEM_43_EEP_STOP_SEC_CODE
#include "Mem_43_Eep_MemMap.h"

#ifdef __cplusplus
}
#endif

#endif

/** @}*/

