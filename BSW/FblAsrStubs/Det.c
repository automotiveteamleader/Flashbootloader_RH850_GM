
# include "ComStack_Types.h"
# include "Can_GeneralTypes.h"
#include "Det.h"

/*******************************************************************************
 * Global functions as an interface from EcuM to DET for initialization
 * and start
 *******************************************************************************/

/*******************************************************************************
 * Global function for BSW-Modules to report errors to the DET
 *******************************************************************************/
FUNC(Std_ReturnType, DET_CODE) Det_ReportError( uint16 ModuleId, uint8 InstanceId, uint8 ApiId, uint8 ErrorId ) /* PRQA S 1503, 850 */ /* MD_DET_1503, MD_DET_0850 */
{
   static volatile uint16 i = 0;
   while(i == 1u)
   {
      ;
   }

   return E_OK;
} /* PRQA S 2006, 6010, 6030 */ /* MD_DET_2006, MD_MSR_STPTH, MD_MSR_STCYC */
