#ifndef EA_H
#define EA_H


#include "Std_Types.h"
#include "MemAcc.h"

/**
* @file           MemIf_Types.h
* @requirements   BSW00374, BSW00379, BSW00318
* @implements     DESIGN001, DESIGN002
*/
#define EA_VENDOR_ID                          0
#define EA_MODULE_ID                          0
#define EA_AR_RELEASE_MAJOR_VERSION           0
#define EA_AR_RELEASE_MINOR_VERSION           0
#define EA_AR_RELEASE_REVISION_VERSION        0
#define EA_SW_MAJOR_VERSION                   0
#define EA_SW_MINOR_VERSION                   0
#define EA_SW_PATCH_VERSION                   0

/*==================================================================================================
                                       DEFINES AND MACROS
==================================================================================================*/
#define EA_DEV_ERROR_DETECT (STD_OFF)


/* This configuration enables the SubAddressArea retirement feature */
#define EA_SUBADDRESSAREA_RETIREMENT                     (STD_OFF)

/* Arbitrary value by NVRAM Manager (NvM) */
#define NVM_DATASET_SELECTION_BITS                         (4) //4 bits reservados para datasets
#define NUM_CONFIGURED_BLOCKS                              (3) //3 bloques configurados    

//SWS_Ea_00198, SWS_Ea_00197
//#define EA_ALING_READ_REQUEST  // Habilita la verificación de alineación
#define EaMinimumReadPageSize                               (4) // Tamaño de página mínimo = 4 bytes
#define EaBufferAlignmentValue                              (2)  //bytes   

//config autosar (.arxml) or  Ea_Cfg.h
#define EA_MAX_BLOCKS                                       (2) // arbitrary value


//Error codes
#define EA_E_INVALID_CANCEL                                 0x01u     // Error por cancelación inválida 

//#define EA_BUFFER_ALIGNMENT_REQUIRED

//#define EA_ENABLE_INTERNAL_JOBS                           (STD_OFF) //garbage collection" tras una escritura.

#define Ea_NvMJobNotificaction                              (STD_OFF)

#if( Ea_NvMJobNotificaction == STD_ON)
/* Job end notification routine provided by the upper layer module */
/** @implements   EaNvmJobEndNotification_Object */
#define Ea_NvMJobEndNotification                            NvM_JobEndNotification
/* Job error notification routine provided by the upper layer module */
/** @implements   EaNvmJobErrorNotification_Object */
#define Ea_NvMJobErrorNotification                          NvM_JobErrorNotification

#endif /*#if (Ea_NvMJobNotificaction == STD_OFF)*/

/**
* @brief          Mem_43_EEP Config Type
* @details        Configuration data structure of the Ea module
* @implements     Mem_43_EEP_ConfigType_typedef
*
*/
typedef struct 
{
  uint16 const *Ea_Config;
}Ea_ConfigType;

/**
* @brief          Block Config Type
* @details        Configuration data structure of the Ea module
* @implements     
*
*/
typedef struct 
{
    uint16 BlockNumber;  // Bloque lógico (ej: 0x100)
    uint32 PhysicalAddr;  // Dirección física (ej: 0x0000F200)
    uint16 Size;        //Tamaño en bytes
}Ea_BlockAddressType;

/**
* @brief          Block Config Type
* @details        Configuration data structure of the Ea module
* @implements     
*
*/
typedef struct {
    uint16  BlockNumber; // Bloque lógico (ej: 0x100)
    uint16  BlockSize;  //Tamaño en bytes 
    boolean ImmediateData; 
}Ea_BlockConfigType;





/**
 * En el entorno AUTOSAR, la herramienta de configuración (ej: EB Tresos, DaVinci Configurator) genera automáticamente tablas de mapeo basadas en:

    Definición de bloques lógicos: Cada bloque (ej: 0x100) tiene un tamaño y propiedades asignadas.
    Memoria física disponible: Se especifican las direcciones base y tamaños de la EEPROM/Flash emulada.
    Ejemplo de configuración en .arxml:
        <EEPROMS>
            <LOGICAL_BLOCK ID="0x100" SIZE="64" PHYSICAL_ADDRESS="0x0000F200"/>
            <LOGICAL_BLOCK ID="0x101" SIZE="128" PHYSICAL_ADDRESS="0x0000F240"/>
        </EEPROMS>
 */

const Ea_BlockAddressType Ea_ConfigTable[] =
{
    {0x100, 0x0000F200, 64},  // Bloque 0x100 → Dirección 0x0000F200
    {0x101, 0x0000F240, 128}, // Bloque 0x101 → Dirección 0x0000F240
};


/**
* @brief          Block Config example
* @details        Configuration blocks
* @implements     
*
*/
const Ea_BlockConfigType Ea_ConfiguredBlocks[NUM_CONFIGURED_BLOCKS] = {
    {0x0001, 32, TRUE},  // Bloque 1, 32 bytes, datos inmediatos
    {0x0005, 100, FALSE}, // Bloque 5, 100 bytes, no datos inmediatos
    {0x0017, 38, FALSE}   // Bloque 17, 38 bytes, no datos inmediatos
};

/* Configuración de bloques EA -> Áreas MemAcc */
typedef struct {
    uint16 BlockNumber;                   // Número de bloque lógico EA
    MemAcc_AddressAreaIdType AddressArea; // Área física asociada
} Ea_BlockToAreaMapping;

const Ea_BlockToAreaMapping Ea_BlockMap[] =
{
    {0x0001, 0},  // Bloque 1 → Área 0 (EEPROM principal)
    {0x0002, 0},  // Bloque 2 → Área 0 
    {0x0003, 1}   // Bloque 3 → Área 1 (EEPROM secundaria)
}

#define EA_NUM_BLOCKS (sizeof(Ea_BlockMap) / sizeof(Ea_BlockToAreaMapping))      

#endif

