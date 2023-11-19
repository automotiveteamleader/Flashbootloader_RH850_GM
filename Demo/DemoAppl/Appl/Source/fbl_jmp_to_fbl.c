
/* These includes from the bootloader header files are required in your application. */
#if defined( USE_LEGACY_FBL_APPL_INTERFACE )
/* provide fbl_def.h from D01 */
# include "fbl_def_legacy_D01.h"
# include "fbl_cfg.h"
#else

/* use current deliveries fbl_def.h */
# include "fbl_def.h"
# include "fbl_mtab.h"
# include "fbl_main.h"
# include "comdat.h"
# include "fbl_cfg.h"
#endif

/* This include is not required in your application. */
#include "fbl_inc.h"


void JumpToBootDemonstration ( void )
{
#if defined( USE_LEGACY_FBL_APPL_INTERFACE )
   tCanInitTable  canInitTable;

   /* Set tp target address in CAN initialization table for use by Fbl */
   canInitTable.TpTargetAddress = FblCwGetSourceAddress(CanRxActualId);
#else
   /* Check comdat provided data is where it is expected, before access */
   assertFblInternal(FblGetComDat_IsPresent(), kFblAssertConditionCorrect);
   /* Check comdat provided data is compatible, before access */
   assertFblInternal(FblGetComDat_IsCompatible(), kFblAssertConditionCorrect);

   /* Prepare Fbl required elements of fblCommonDataAccessRam before transition to the Fbl
    * fblCommonDataAccessRam contains shared Ram data between application and Fbl.
    *
    * BE SURE TO KEEP THAT RAM AREA UNUSED / WITHOUT CONFLICT IN YOUR APPLICATION.
    *
    */

   /*  OPTION COM WRAPPER CAN IN FBL */
   /*  Set fblCommonDataAccessRam.targetAddress to Tester Id used in application.
   */
   FblCwSaveResponseAddress();

   /* Optionally pass sbat address */
   /* fblCommonDataAccessRam.pSbat = (tFblSbatPtr) sbaBlk0; */
   fblCommonDataAccessRam.reProgChannel = FBL_REPR_CH_UDS;
#endif

   /* ----------------------------------------------------------------*/
   /* -- Start flash boot loader.                                   --*/
   /* -- Send out response pending before (recommended).            --*/
   /* ----------------------------------------------------------------*/
   DiagExRCRResponsePending(kForceSendResponsePending);

#if defined( USE_LEGACY_FBL_APPL_INTERFACE )
   CallFblStart(&canInitTable);
#else
   CallFblStart();
#endif

   /* Shall never get reached */
   while(1)
   {
      ;
   }
   /* -----------------------------------------------------------------*/
   /* -- !! NOTE: Code should never reach here  !!!   --*/
   /* ----------------------------------------------------------------*/
}
