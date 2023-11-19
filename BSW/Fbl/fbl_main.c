/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief         Main module of FBL
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
 *  Achim Strobelt                Ach           Vector Informatik GmbH
 *  Marco Riedl                   Rie           Vector Informatik GmbH
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  03.00.00   2016-02-26  Ach     -                First implementation, based on FblKbMain_Frame 01.52.00
 *  03.00.01   2016-02-26  Ach     ESCAN00088625    Application is not started after start message time out
 *  03.01.00   2016-05-13  Rie     ESCAN00089848    Added call to SecM_InitPowerOn
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

#include "fbl_inc.h"

/***********************************************************************************************************************
 *  VERSION
 **********************************************************************************************************************/

#if ( FBL_MAIN_VERSION != 0x0301u ) || \
    ( FBL_MAIN_RELEASE_VERSION != 0x00u )
# error "Error in fbl_main.c: Source and header file are inconsistent!"
#endif

#if ( FBL_MAIN_VERSION != _FBL_MAIN_VERSION ) || \
    ( FBL_MAIN_RELEASE_VERSION != _FBL_MAIN_RELEASE_VERSION)
# error "Error in fbl_main.c: Source and v_ver.h are inconsistent!"
#endif

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

/* FblStart feature */
#if defined( FBL_ENABLE_FBL_START )
# if !defined( FblStartReset )
#  define FblStartReset() ApplFblReset()
# endif
#endif /* FBL_ENABLE_FBL_START */

#if defined( FBL_ENABLE_STAY_IN_BOOT )
# if !defined( kFblMainStartMessageDelay )
#  define kFblMainStartMessageDelay 20u      /**< Start message delay default value: 20ms */
# endif
# define kFblMainStartMessageReceived 0u     /**< Indicates that a startup message has been received */
# define kFblMainStartMessageTimeout  1u     /**< No startup message has been received before timeout */
#endif

/* Main state handling */
#define FblMainSetStartFromReset()                 SetFblMainState( kFblMainStateStartFromReset )
#define FblMainSetStartFromAppl()                  SetFblMainState( kFblMainStateStartFromAppl )
#define FblMainSetStartFromXcp()                   SetFblMainState( kFblMainStateStartFromXcp )
#define FblMainSetApplicationValid()               SetFblMainState( kFblMainStateApplicationValid )
#define FblMainSetWaitStartMessage()   {                                               \
                                       SetFblMainState( kFblMainStateWaitStartMessage );    \
                                       startMessageDelay = kFblMainStartMessageDelay + 1u;   \
                                    }  /* PRQA S 3458 */ /* MD_MSR_19.4 */
#define FblMainSetStartMessageReceived()           SetFblMainState( kFblMainStateStartMessageReceived )

#define FblMainClrStartFromReset()                 ClrFblMainState( kFblMainStateStartFromReset )
#define FblMainClrStartFromAppl()                  ClrFblMainState( kFblMainStateStartFromAppl )
#define FblMainClrStartFromXcp()                   ClrFblMainState( kFblMainStateStartFromXcp )
#define FblMainClrApplicationValid()               ClrFblMainState( kFblMainStateApplicationValid )
#define FblMainClrWaitStartMessage()               ClrFblMainState( kFblMainStateWaitStartMessage )
#define FblMainClrStartMessageReceived()           ClrFblMainState( kFblMainStateStartMessageReceived )

/***********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

#define FBLHEADER_START_SEC_CONST
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/** FBL header data for access from application software */
V_MEMROM0 V_MEMROM1 tFblHeader V_MEMROM2 FblHeader =
{
   kGENyVersion
#if defined( FBL_MTAB_MAGIC_FLAG )
   ,(tFblHeaderLogicalBlockTablePtr)(&FblLogicalBlockTable)
#else
   ,V_NULL
#endif
#if defined( FBL_ENABLE_COMMON_DATA )
   ,(tFblCommonDataPtr)(&fblCommonData)
#endif
#if defined( FBL_ENABLE_FBL_START )
   ,(tFblStrtFct)FblStart
#endif
};
#define FBLHEADER_STOP_SEC_CONST
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if defined( FBL_ENABLE_FBL_START )
# define FBL_MAIN_MAGICFLAG_START_SEC_VAR
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/** Reprogramming request storage variable */
V_MEMRAM0 volatile V_MEMRAM1 vuint8 V_MEMRAM2 fblStartMagicFlag[kFblNoOfMagicBytes];
# if defined( FBL_MAIN_ENABLE_FBLSTART_PARAM )
V_MEMRAM0 volatile V_MEMRAM1 tFblStartParamter V_MEMRAM2 fblStartParameter;
# endif /* FBL_MAIN_ENABLE_FBLSTART_PARAM */
# define FBL_MAIN_MAGICFLAG_STOP_SEC_VAR
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
#endif /* FBL_ENABLE_FBL_START */

/***********************************************************************************************************************
 *  LOCAL DATA
 **********************************************************************************************************************/

#if defined( FBL_ENABLE_STAY_IN_BOOT )
V_MEMRAM0 static V_MEMRAM1 vuint16 V_MEMRAM2 startMessageDelay;
#endif

V_MEMRAM0 V_MEMRAM1_NEAR tFblStateBitmap V_MEMRAM2_NEAR fblMainStates[FBL_STATE_INDEX(kFblMainNumberOfStates - 1u) + 1u];

/***********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
static void FblCheckForApplication(void);
#if defined( FBL_ENABLE_STAY_IN_BOOT )
static void FblPostProcessStartMsg( vuintx startMsgResult );
static void FblRepeatCheckForStartMsg(void);
#endif
static void FblInitPowerOn( void );
static void FblInit( vuint8 initposition );

static void FblStartApplication( void );
static void FblDeinit( void );

static void FblRepeat( void );

/***********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  FblCheckForApplicationStart
 **********************************************************************************************************************/
/*! \brief       Checks if the application should be started
 *  \details     Evaluates the programming request flag and the application validity
 **********************************************************************************************************************/
static void FblCheckForApplication(void)
{
#if defined( FBL_NO_JSR_APPL )
#else
   /* Check if application software should be started */
   switch (ApplFblExtProgRequest())
   {
      case kProgRequest:
      {
         FblMainSetStartFromAppl();
         break;
      }
      case kProgRequestXcp:
      {
         FblMainSetStartFromXcp();
         break;
      }
      default:
      {
# if defined( FBL_ENABLE_STAY_IN_BOOT ) && \
     defined( FBL_MAIN_ENABLE_APPL_CHECK_AFTER_STARTUP_MSG )
         /* Invoke start message check on every startup */
         FblMainSetWaitStartMessage(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
# else
         /* Check for valid application */
         if (ApplFblIsValidApp() == kApplValid)
         {
            FblMainSetApplicationValid();
#  if defined( FBL_ENABLE_STAY_IN_BOOT )
            /* Only wait for start message if there is a valid application */
            FblMainSetWaitStartMessage(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
#  else
            /* Start application directly */
            FblStartApplication();
#  endif /* FBL_ENABLE_STAY_IN_BOOT */
         }
         else
         {
            FblMainSetStartFromReset();
         }
# endif
         break;
      }
   }
#endif /* FBL_NO_JSR_APPL */
}

#if defined( FBL_ENABLE_STAY_IN_BOOT )
/***********************************************************************************************************************
 *  FblPostProcessStartMsg
 **********************************************************************************************************************/
/*! \brief       Post-process result of start message reception
 *  \details     This function post-processes the result of the startup message check - either
 *               the Bootloader is prepared or the application is started.
 *  \param[in]   startMsgResult Indicates if a start message has been received or not
 **********************************************************************************************************************/
static void FblPostProcessStartMsg(vuintx startMsgResult)
{
   switch (startMsgResult)
   {
      case kFblMainStartMessageReceived:
      {
         FblMainSetStartMessageReceived();
         break;
      }
      case kFblMainStartMessageTimeout:
      {
# if defined( FBL_ENABLE_STAY_IN_BOOT )
#  if defined( FBL_MAIN_ENABLE_APPL_CHECK_AFTER_STARTUP_MSG )
         /* Check for valid application */
         if (ApplFblIsValidApp() == kApplValid)
#  else
         if (FblMainGetApplicationValid())
#  endif
         {
            /* Start application. Function never returns. */
            FblStartApplication();
         }
# endif
         break;
      }
      default:
      {
         assertFblInternal(0u, kFblSysAssertParameterOutOfRange); /* PRQA S 2214 */ /* MD_FblMain_2214 */
         break;
      }
   }

}
#endif

/***********************************************************************************************************************
 *  FblInitPowerOn
 **********************************************************************************************************************/
/*! \brief       Pre-initialization of Bootloader variables
 *  \details     Initializes all variables needed to control the Bootloader
 **********************************************************************************************************************/
static void FblInitPowerOn(void)
{
   vuintx i;

   /* Disable global interrupt handling */
   FblInterruptDisable();  /* PRQA S 1006 */ /* MD_FblMain_1006 */ /* PRQA S 3109 */ /* MD_MSR_14.3 */

   /* Check if bootloader header address is correctly configured in generation tool */
   /* PRQA S 0306 1 */ /* MD_FblMain_FblHeader */ /* PRQA S 2214 1 */ /* MD_FblMain_2214 */
   assertFblGen(FblHeaderLocal == FblHeaderTable, kFblSysAssertParameterOutOfRange);

   /* Initialization of main variables */
   for (i = 0u; i < (sizeof(fblMainStates)/sizeof(fblMainStates[0])); i++)
   {
      fblMainStates[i] = (tFblStateBitmap)0x00u;
   }
#if defined( FBL_ENABLE_STAY_IN_BOOT )
   startMessageDelay = 0u;
#endif

   /* Power on initialization of core modules */
   ClrWDInit();
   FblCwClrInit();
   MemDriver_InitPowerOn();   /* PRQA S 3109 */ /* MD_MSR_14.3 */
   FblDiagInitPowerOn();

#if defined( FBL_ENABLE_SECMOD_VECTOR )
   (void)SecM_InitPowerOn(V_NULL);
#endif /* FBL_ENABLE_SECMOD_VECTOR */
}

/***********************************************************************************************************************
 *  FblInit
 **********************************************************************************************************************/
/*! \brief       Do initializations after reset or start out of application
 **********************************************************************************************************************/
static void FblInit( vuint8 initposition )
{
   /* User initialization before core initializations */
   ApplFblStartup(kFblInitPreCallback | initposition);

   switch (initposition)
   {
      /* Initialization steps the application validity is checked */
      case kFblInitBaseInitialization:
      {
#if defined( FBL_ENABLE_PRE_WDINIT )
         /* Initialize watchdog handling */
         FblInitWatchdog();
#endif /* FBL_MAIN_ENABLE_PRE_WDINIT */
#if defined( FBL_ENABLE_PRE_TIMERINIT )
         /* Initialize Timer */
         FblTimerInit();
#endif /* FBL_MAIN_ENABLE_PRE_TIMERINIT */
         break;
      }

      /* Initialization steps after the application validity is checked */
      case kFblInitFblCommunication:
      {
#if !defined( FBL_ENABLE_PRE_WDINIT )
         /* Initialize watchdog handling */
         FblInitWatchdog();
#endif /* FBL_MAIN_ENABLE_PRE_WDINIT */
#if !defined( FBL_ENABLE_PRE_TIMERINIT )
         /* Initialize Timer */
         FblTimerInit();
#endif /* FBL_ENABLE_PRE_TIMERINIT */

         /* Initialize communication wrapper */
         FblCwInit();

         /* Initialize diagnostic module */
         FblDiagInit();
         break;
      }

      /* Initialization steps executed after the Bootloader is invoked */
      case kFblInitFblFinalize:
      {

         /* Diagnostic layer - init steps after NV-memory init */
         FblDiagPostInit();
         break;
      }

      default:
      {
         assertFblInternal(0u, kFblSysAssertParameterOutOfRange); /* PRQA S 2214 */ /* MD_FblMain_2214 */
         break;
      }
   }

   /* User initialization after core initialization */
   ApplFblStartup(kFblInitPostCallback | initposition);
}

/***********************************************************************************************************************
 *  FblStartApplication
 **********************************************************************************************************************/
/*! \brief       This function is called for leaving bootloader and enter application software.
 *  \pre         Only call if application is present and valid
 **********************************************************************************************************************/
static void FblStartApplication( void )
{
#if defined( FBL_ENABLE_STAY_IN_BOOT )
   FblDeinit();
#else
# if defined( FBL_ENABLE_PRE_WDINIT )
#  if defined( FBL_WATCHDOG_ON )
   /* Synchronize watchdog before application start */
   ApplFblWDLong();
#  endif /* FBL_WATCHDOG_ON */
# endif /* FBL_ENABLE_PRE_WDINIT */
# if defined( FBL_ENABLE_PRE_TIMERINIT )
   /* Stop timer to avoid a timer interrupt
      after application start */
   FblTimerStopp();  /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0303 */ /* MD_FblMain_0303_0306 */
# endif /* FBL_ENABLE_PRE_TIMERINIT */
#endif

   /* Start application code */
   ApplFblStartApplication();
}

/***********************************************************************************************************************
 *  FblDeinit
 **********************************************************************************************************************/
/*! \brief       Do necessary deinitializations
 **********************************************************************************************************************/
static void FblDeinit( void )
{
   /* Deinitialize communication wrapper */
   FblCwDeinit();
#if defined( FBL_WATCHDOG_ON )
   /* Synchronize watchdog before reset */
   ApplFblWDLong();
#endif /* FBL_WATCHDOG_ON */
   FblTimerStopp();  /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0303,0306 */ /* MD_FblMain_0303_0306 */
}

#if defined( FBL_ENABLE_STAY_IN_BOOT )
/***********************************************************************************************************************
 *  FblRepeatCheckForStartMsg
 **********************************************************************************************************************/
/*! \brief       This function waits during the acceptance window if the start message is received.
 *  \pre         Hardware has to be initialized.
 **********************************************************************************************************************/
static void FblRepeatCheckForStartMsg( void )
{
   /* Set COM wrapper flag for start message window */
   FblCwSetCheckForStartMsg();
   while (FblMainGetWaitStartMessage())
   {
      /* Check if 1ms hardware timer expired */
      if ((FblLookForWatchdog() & FBL_TM_TRIGGERED) == FBL_TM_TRIGGERED)
      {
         /* Handle timer each millisecond */
         startMessageDelay--;
         if (startMessageDelay == 0)
         {
            /* Start message wait time expired */
            FblMainClrWaitStartMessage();
            FblPostProcessStartMsg(kFblMainStartMessageTimeout);
         }
      }

      if (FblCwCheckStartMessage() == kFblOk)
      {
         /* Startup message has been received */
         FblMainClrWaitStartMessage();
         FblPostProcessStartMsg(kFblMainStartMessageReceived);
      }
   }
   /* Start message window is closed now */
   FblCwClrCheckForStartMsg();
}
#endif

/***********************************************************************************************************************
 *  FblRepeat
 **********************************************************************************************************************/
/*! \brief       This function contains the main loop of the FBL.
 *  \details     In this loop, the Bus is polled and the watchdog is triggered.
 *               This function is used by the boot procedure for the start delay and for the real operation.
 *  \pre         Hardware has to be initialized before.
 *  \return      The return value is used by the boot procedure to determine whether to stay in FBL or start the appl.
 **********************************************************************************************************************/
static void FblRepeat( void )
{
   /* This loop will be left only for reset */
   while (!FblMainGetFblShutdownRequest())
   {
      /* Check if 1ms hardware timer expired */
      if ((FblLookForWatchdog() & FBL_TM_TRIGGERED) == FBL_TM_TRIGGERED)
      {
         /* Run 1ms based tasks */
          FblDiagTimerTask();

# if defined( FBL_ENABLE_APPL_TIMER_TASK )
            /* Call cyclic function of fbl application module (if configured) */
            ApplFblTask();
# endif /* FBL_ENABLE_APPL_TIMER_TASK */
      }
      /* Run non-timer relevant operations */
      FblDiagStateTask();
      FblCwIdleTask();

#if defined( FBL_MEM_ENABLE_PIPELINING )
      if (FblMemTaskIsPending())
      {
         FblMemTask();
      }
#endif /* FBL_MEM_ENABLE_PIPELINING */

# if defined( FBL_ENABLE_APPL_STATE_TASK )
      /* Call background function of fbl application module (if configured) */
      ApplFblStateTask();
# endif /* FBL_ENABLE_APPL_STATE_TASK */

   }
}

/***********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  main
 **********************************************************************************************************************/
/*! \brief       If a valid application program is available, the application will be started.
 *  \details     Otherwise the FBL initializes the hardware and waits for a download request.
 **********************************************************************************************************************/
int V_API_NEAR main( void ) /* PRQA S 5013 */ /* MD_FblMain_5013 */
{
   /* Initialize variables */
   FblInitPowerOn();

   /* base initialization step - valid for both application and Bootloader */
   FblInit(kFblInitBaseInitialization);

   /* Check if a programming session request is pending or for a valid application (if
      allowed at this point). This function may not return but start the application. */
   FblCheckForApplication();

   /* Bootloader initialization step 1 - hardware and basic communication */
   FblInit(kFblInitFblCommunication);

#if defined( FBL_ENABLE_STAY_IN_BOOT )
   /* Check for start message */
   FblRepeatCheckForStartMsg();
#endif

   /* Bootloader initialization step 2 - full diagnostics initialization */
   FblInit(kFblInitFblFinalize);

   /* Bootloader execution */
   FblRepeat();

   /* Deinitialize and reset bootloader */
   FblDeinit();
   ApplFblReset();

   return 0; /* Return statement is never reached */
}

#if defined( FBL_ENABLE_FBL_START )
/***********************************************************************************************************************
 *  FblStart
 **********************************************************************************************************************/
/*! \brief       Transition from application into flash boot loader
 *  \pre         Valid application program running
 *  \param[in]   pFblStartParam Pointer to OEM specific parameter structure
 *  \param[in]   length Length of OEM specific parameter structure [Bytes]
 **********************************************************************************************************************/
# if defined( FBL_MAIN_ENABLE_FBLSTART_PARAM )
void FblStart( V_MEMRAM1 void V_MEMRAM2 V_MEMRAM3 * pFblStartParam, vuint8 length )
# else
void FblStart( void )
# endif /* FBL_MAIN_ENABLE_FBLSTART_PARAM */
{
# if defined( FBL_MAIN_ENABLE_FBLSTART_PARAM )
   vuintx i;
# endif

   FblInterruptDisable();  /* PRQA S 1006 */ /* MD_FblMain_1006 */ /* PRQA S 3109 */ /* MD_MSR_14.3 */

   /* Restore bootloader context - e.g. small data area base registers */
   FblHwRestoreFblContext();

# if defined( FBL_MAIN_ENABLE_FBLSTART_PARAM )
   /* Copy parameter to reset safe variable */
   for (i = 0u; i < length; i++)
   {
      ((vuint8*)&fblStartParameter)[i] = ((vuint8*)pFblStartParam)[i];
   }
# endif /* FBL_MAIN_ENABLE_FBLSTART_PARAM */

   /* Set FblStart magic flag
      Please pay attention: The FblStart magic flag does not use a shared memory area so that the buffer may overlap
      with the application stack. */
   FblSetFblStartMagicFlag();

   /* Generate reset */
   FblStartReset();
}

#endif /* FBL_ENABLE_FBL_START */

/***********************************************************************************************************************
 *  MISRA DEVIATIONS
 **********************************************************************************************************************/

/* Module specific MISRA deviations:

   MD_FblMain_0303_0306:
     Reason: Casting of integer to pointer type is required due to memory mapped registers.
     Risk: The size of integer required to hold the result of a pointer cast is implementation defined.
     Prevention: The size of the respective integer data type which holds the address value is adapted on a hardware
                 specific basis.

   MD_FblMain_1006:
      Reason: Interrupts are enabled/disabled using assembler code.
      Risk: The code is not portable for different compilers.
      Prevention: The assembler code provided by each FblHw module has to be tested.

   MD_FblMain_2214:
      Reason: Assertion macros have to be disabled in production code and are used only for development.
      Risk: Assertion code may be enabled in production code unintentionally. If a assertion condition is unexpectedly
         false, the code is active. A problem may occur if the Macro is additionally changed from single statement
         to multiple statement.
      Prevention: Macro is not allowed to be changed without review. Development code is automatically disabled via
         configuration of project state "Production".

   MD_FblMain_5013:
      Reason: The Vector standard API for main function follows ANSI C typical definition of main function returning int.
      Risk: No identifiable risk.
      Prevention: No prevention required.

   MD_FblMain_FblHeader:
      Reason: A memory address is compared to a configured value. The configured value is interpreted as an integer.
      Risk: The comparison may fail due to different data types.
      Prevention: If this check fails, the configuration checks done by assertions have to verified manually.

*/

/***********************************************************************************************************************
 *  END OF FILE: FBL_MAIN.C
 **********************************************************************************************************************/
