#ifndef MEMIF_H
#define MEMIF_H


/**
 * @brief        The function read a block
 *
 * @details      Invokes the "Read" function of the underlying memory abstraction module selected 
                by the parameter DeviceIndex. 
 *
 * @param[in]    DeviceIndex    Number of driver device
 * @param[in]    BlockNumber    Number of logical block, also denoting
*                                start address of that block in emulated
*                                EEPROM.
 * @param[in]    BlockOffset    Read address Offset inside the block. 
 * @param[in]    DataBufferPtr  Pointer to selected configuration array
 * @param[in]    Length         Pointer to selected configuration structure.
 * 
 *
 * @return        E_NOT_OK else it shall 
                return the value of the called function of the underlying module. 
 * @api
 */
Std_ReturnType MemIf_Read( uint16 DeviceIndex,  uint16 BlockNumber, uint16 BlockOffset, uint8* DataBufferPtr, uint16 Length );

/**
 * @brief        The function write a block
 *
 * @details      Invokes the "Write" function of the underlying memory abstraction module selected 
                by the parameter DeviceIndex. 
 *
 * @param[in]    DeviceIndex    Number of driver device
 * @param[in]    BlockNumber    Number of logical block, also denoting
*                                start address of that block in emulated
*                                EEPROM.
 * @param[in]    DataBufferPtr  Pointer to selected configuration array
 * 
 *
 * @return        E_NOT_OK else it shall 
                return the value of the called function of the underlying module. 
 *
 * 
 *
 * @api
 */
Std_ReturnType MemIf_Write( uint16 DeviceIndex, uint16 BlockNumber, const uint8* DataBufferPtr );
 
/**
 * @brief        The function cancels the job
 *
 * @details      Invokes the "Cancel" function of the underlying memory abstraction module selected 
                by the parameter DeviceIndex. 
 *
 * @param[in]    DeviceIndex    Number of driver device
 *
 * 
 *
 * @api
 */
void MemIf_Cancel( uint16 DeviceIndex ) ;

/**
 * @brief        The function obtains status
 *
 * @details      Invokes the "GetStatus" function of the underlying memory abstraction module selected 
                by the parameter DeviceIndex. 
 *
 * @param[in]    DeviceIndex    Number of driver device
 *
 * @return      MemIf_StatusType
 *
 * @api
 */
MemIf_StatusType MemIf_GetStatus ( uint16 DeviceIndex );

/**
 * @brief        The function obtains status of work performed
 *
 * @details      Invokes the "GetJobResult" function of the underlying memory abstraction module selected 
                by the parameter DeviceIndex. 
 *
 * @param[in]    DeviceIndex    Number of driver device
 *
 * @return       In case development error detection is enabled for the Memory 
                Abstraction Interface and a development error is detected according to 
                SWS_MemIf_00022 the function shall return MEMIF_JOB_FAILED else 
                it shall return the value of the called function of the underlying module.
 *
 * @api
 */
MemIf_JobResultType MemIf_GetJobResult ( uint16 DeviceIndex ); 

/**
 * @brief        The function mark a block as invalid
 *
 * @details      Invokes the "InvalidateBlock" function of the underlying memory abstraction module selected 
                by the parameter DeviceIndex. 
 *
 * @param[in]    DeviceIndex    Number of driver device
 * @param[in]    BlockNumber    Number of logical block, also denoting
*                                start address of that block in emulated
*                                EEPROM.
 * @param[in]    DataBufferPtr  Pointer to selected configuration array
 * 
 *
 * @return        E_NOT_OK else it shall 
                return the value of the called function of the underlying module. 
 * @api
 */
Std_ReturnType MemIf_InvalidateBlock ( uint16 DeviceIndex, uint16 BlockNumber );

/**
 * @brief        Returns version information 
 *
 * @param[out]    VersionInfoPtr    Pointer to standard version information structure. 
 * @api
 */
void MemIf_GetVersionInfo ( Std_VersionInfoType* VersionInfoPtr );

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
Std_ReturnType MemIf_EraseImmediateBlock ( uint16 DeviceIndex, uint16 BlockNumber );

/*==================================================================================================
                                        LOCAL MACROS
==================================================================================================*/
// Number of drivers/devices availables (Fee and Ea)
#define MEMIF_NUMBER_OF_DEVICES 1

#if( MEMIF_NUMBER_OF_DEVICES < 2)

    //MEMIF_NUMBER_OF_DEVICES IS 1
    #define MemIf_Write(DeviceIndex, BlockNumber, DataBufferPtr)                     Fee_Write(BlockNumber, DataBufferPtr)
    #define MemIf_Read(DeviceIndex, BlockNumber, BlockOffset, DataBufferPtr, Length) Fee_Read(BlockNumber, BlockOffset, DataBufferPtr, Length)
    #define MemIf_Cancel(void)                                                       Fee_Cancel()
    #define MemIf_GetJobResult(void)                                                 Fee_GetJobResult()
    #define MemIf_GetStatus(DeviceIndex)                                             Fee_GetStatus()
    #define MemIf_InvalidateBlock(DeviceIndex, BlockNumber)                          Fee_InvalidateBlock(BlockNumber)
    #define MemIf_EraseImmadiateBlock(BlockNumber)                                   Fee_EraseImmediateBlock(BlockNumber)

#endif

#endif /* MEMIF_TYPES_H */

/** @} */