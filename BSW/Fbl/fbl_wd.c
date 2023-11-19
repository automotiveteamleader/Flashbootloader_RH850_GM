/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief        Watchdog functions
 *
 *  --------------------------------------------------------------------------------------------------------------------
 *  COPYRIGHT
 *  --------------------------------------------------------------------------------------------------------------------
 *  \par Copyright
 *  \verbatim
 *  Copyright (c) 2016 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 */
/**********************************************************************************************************************/

/***********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  --------------------------------------------------------------------------------------------------------------------
 *  Andreas Wenckebach            AWh           Vector Informatik GmbH
 *  Ralf Haegenlaeuer             HRf           Vector Informatik GmbH
 *  Thomas Bezold                 TBe           Vector Informatik GmbH
 *  Alexander Starke              ASe           Vector Informatik GmbH
 *  Marco Riedl                   Rie           Vector Informatik GmbH
 *  Torben Stoessel               TnS           Vector Informatik GmbH
 *  Achim Strobelt                Ach           Vector Informatik GmbH
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  02.00.00   2013-09-20  HRf     ESCAN00070534    RCRRP message is not processed as expected
 *  02.01.00   2013-10-01  AWh     ESCAN00070817    Allow to configure external Wd Event Timer handling (P2)
 *  02.02.00   2013-10-09  HRf     ESCAN00070988    Change GetCwIdleTaskState() to FblCwGetIdleTaskState() to follow
 *                                                  fbl_cw nomenclature
 *  02.03.00   2014-01-20  AWh     ESCAN00071053    Unused macros for FBL_ENABLE_GENERIC_WDTIMER_EXT_HANDLING
 *                         TBe     ESCAN00071969    No changes
 *                         ASe     ESCAN00072907    Fixed handling of DSP5683X with Metrowerks compiler
 *  02.04.00   2014-06-13  ASe     ESCAN00076257    Modified watchdog copy handling for C_COMP_IAR_RL78_AFCAN
 *  02.05.00   2014-10-17  Ach     ESCAN00078355    Added void wrapper for FblLookForWatchdog()
 *  02.06.00   2015-04-17  Rie     ESCAN00082413    Added MemMap support for FblLookForWatchdogVoid
 *  02.07.00   2015-06-12  AWh     ESCAN00083379    Gm SLP6: Rcrrp shall be at P2Star/2 inst. of shortly before timeout
 *  02.08.00   2015-09-18  AWh     ESCAN00083585    No changes
 *                         AWh     ESCAN00083951    Remove FBL_WD_STATE_TASK and FBL_WD_TIMER_TASK comment filters
 *  02.09.00   2015-10-23  AWh     ESCAN00085794    Gm SLP4: Swcp filtering not successful
 *                         CB      ESCAN00085818    Removed out-dated filters for Tasking- and Keil-compiler
 *  02.09.01   2015-12-10  TnS     ESCAN00086954    Compiler warning: variable set but never used
 *                         TnS     ESCAN00059743    Watchdog trigger function is not executed any more after flash
 *                                                  erase function was called (FBL_ENABLE_INTERRUPT is set)
 *  02.09.02   2015-12-10  AWh     ESCAN00087120    No changes
 *  02.10.00   2016-02-23  Ach     ESCAN00088299    Call ApplFblWdInit() from FblInitWatchdog() in case FblMain 3.xx is used
 *                         TnS     ESCAN00088434    Added comments
 *  02.11.00   2016-05-31  AWh     ESCAN00090236    Timer Interrupt: Unnecessary watchdog event might occur in case
 *                                                   isrTimer init value is chosen to small
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

#include "fbl_inc.h"

/***********************************************************************************************************************
 *  VERSION
 **********************************************************************************************************************/

#if ( FBLWD_VERSION != 0x0211u ) || \
    ( FBLWD_RELEASE_VERSION != 0x00u )
# error "Error in FBL_WD.C: Source and header file are inconsistent!"
#endif

#if ( FBLWD_VERSION != _FBLWD_VERSION ) || \
    ( FBLWD_RELEASE_VERSION != _FBLWD_RELEASE_VERSION )
# error "Error in FBL_WD.C: Source and v_ver.h are inconsistent!"
#endif

# if defined( FBLCW_LOCKS_TASKS )
# else
#   error "ComWrapper must handle task locks internally. Please use a newer version."
# endif

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

/* PRQA S 3453 FblWd_3453 */ /* MD_MSR_19.7 */
/* PRQA S 3458 FblWd_3458 */ /* MD_MSR_19.4 */

#define TRIGGER_WATCHDOG()          ApplFblWDTrigger()

#   define kFblGenericEventMargin           (kFblDiagTimeP2Star/2u)
#     define FblWdRetransmit()         FblCwRetransmitRP()
#   define PerformGenericEventAction()  {\
                                           if (GetRcrRpInProgress()                                \
                                               )                                                   \
                                           {                                                       \
                                               FblWdRetransmit();                                  \
                                               SetP2Timer(kFblDiagTimeP2Star);                     \
                                           }                                                       \
                                        }

/* PRQA L:FblWd_3453 */
/* PRQA L:FblWd_3458 */

/***********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

vuint16 P2Timer;        /**< P2 timeout timer, mapped to wdGenericEventTimer in header file */
vuint8  WDInitFlag;     /**< Watchdog initialized flag */

#if defined( FBL_WATCHDOG_ON )
tWdTime WDTimer;           /**< Counts timer events until next watchdog triggering */
#endif

/***********************************************************************************************************************
 *  EXTERNAL DATA
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL DATA
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  FblInitWatchdog
 **********************************************************************************************************************/
/*! \brief      Polling of watchdog during longer operations
 *  \return     Reports if initialization was successful.
 **********************************************************************************************************************/
# define INITWATCHDOG_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
void V_API_NEAR FblInitWatchdog( void )
{

   wdGenericEventTimer = 0x00u;
   SetWDInit();

#if defined( FBL_DEF_ENABLE_NON_KB_MAIN )
   ApplFblWDInit();
#endif /* FBL_DEF_ENABLE_NON_KB_MAIN */
}

# define INITWATCHDOG_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

# define LOOKFORWATCHDOG_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/***********************************************************************************************************************
 *  FblLookForWatchdogVoid
 **********************************************************************************************************************/
/*! \brief       Polling of watchdog during longer operations.
 *  \details     This function executes watchdog and timer handling code from RAM. Depending on the Bootloader
 *               configuration, additionally some communication tasks can be handled by this function. The return
 *               value of FblLookForWatchdog() is not returned by this function so it could be used by libraries which
 *               expect a void return value.
 *  \pre         Watchdog code was copied to RAM, watchdog handling is initialized.
 **********************************************************************************************************************/
void V_API_NEAR FblLookForWatchdogVoid( void )
{
   (void)FblLookForWatchdog();
}
# define LOOKFORWATCHDOG_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

# define LOOKFORWATCHDOG_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/***********************************************************************************************************************
 *  FblLookForWatchdog
 **********************************************************************************************************************/
/*! \brief       Polling of watchdog during longer operations.
 *  \details     This function executes watchdog and timer handling code from RAM. Depending on the Bootloader
 *               configuration, additionally some communication tasks can be handled by this function.
 *  \pre         Watchdog code was copied to RAM, watchdog handling is initialized.
 *  \return      FBL_NO_TRIGGER if no timer/watchdog expired, FBL_TM_TRIGGERED if the millisecond timer expired,
 *               FBL_WD_TRIGGERED if watchdog handling code was called.
 **********************************************************************************************************************/
vuint8 V_API_NEAR FblLookForWatchdog( void )
{
   vuint8 retValue = FBL_NO_TRIGGER;

   if (GetWDInit()) /* Check if watchdog already initialized */
   {
      if (FblTimerGet() != 0x00u)   /* PRQA S 0303 *//* MD_FblSfr_MemoryMappedRegister */
      {
         FblTimerReset(); /* PRQA S 0303 *//* MD_FblSfr_MemoryMappedRegister */
         retValue |= FBL_TM_TRIGGERED;

         if (wdGenericEventTimer > 0x00u)
         {
            wdGenericEventTimer--;
            if (wdGenericEventTimer < kFblGenericEventMargin)
            {
               /* PRQA S 0303 1 *//* MD_FblSfr_MemoryMappedRegister */
               PerformGenericEventAction(); /* PRQA S 3109 *//* MD_MSR_14.3 */
            }
         }

# if defined( FBL_WATCHDOG_ON )
         if (WDTimer != 0x00u)
         {
            WDTimer--;
            if (WDTimer == 0x00u)
            {
               TRIGGER_WATCHDOG();
               WDTimer = FBL_WATCHDOG_TIME;
               retValue |= FBL_WD_TRIGGERED;
            }
         }
# endif /* FBL_WATCHDOG_ON */
         /* Check if the FBL or FD (erase/write) is active */
         if (0u != FblCwIsTimerTaskAllowed())
         {
            /* the FBL is active, this task can now be called e.g. from the FLASH */
            FblCwTimerTask();
         }
# if defined( FBL_WD_TIMER_TASK )
         /* Execute configurable timer task
            Task is not active until timer and watchdog have been initialized
            Has to be executed from RAM or a lock mechanism needs to be implemented in macro to prevent execution while flash
            operations are active  */
         FBL_WD_TIMER_TASK();
# endif /* FBL_WD_TIMER_TASK */
      } /* if (FblTimerGet() != 0x00u) */
   } /* if (GetWDInit()) */

   /* Check if the FBL or FD (erase/write) is active */
   if (0u != FblCwIsStateTaskAllowed())
   {
      /* the FBL is active, this task can now be called e.g. from FLASH */
      FblCwStateTask();
   }
# if defined( FBL_WD_STATE_TASK )
   /* Execute configurable state task
      Task is active immediately and will be executed on every function call
      Has to be executed from RAM or a lock mechanism needs to be implemented in macro to prevent execution while flash
      operations are active  */
   FBL_WD_STATE_TASK();
# endif /* FBL_WD_STATE_TASK */

   return retValue;
}  /* PRQA S 6080 */ /* MD_MSR_STMIF */

# define LOOKFORWATCHDOG_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/***********************************************************************************************************************
 *  END OF FILE: FBL_WD.C
 **********************************************************************************************************************/
