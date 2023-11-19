/*******************************************************************************
 * Global function for BSW-Modules to report errors to the DET
 *******************************************************************************/
#define DET_CODE
FUNC(Std_ReturnType, DET_CODE) Det_ReportError( uint16 ModuleId, uint8 InstanceId, uint8 ApiId, uint8 ErrorId ); /* PRQA S 850 */ /* MD_DET_0850 */
