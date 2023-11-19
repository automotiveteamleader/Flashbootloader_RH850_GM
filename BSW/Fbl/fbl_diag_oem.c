/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief         OEM specific implementation of Diagnostic services supported in boot mode
 *                 Declaration of functions, variables, and constants
 *
 *  --------------------------------------------------------------------------------------------------------------------
 *  COPYRIGHT
 *  --------------------------------------------------------------------------------------------------------------------
 *  \par Copyright
 *  \verbatim
 *  Copyright (c) 2017 by Vector Informatik GmbH.                                                  All rights reserved.
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
 *  Dennis O'Donnell              Dod           Vector CANtech, Inc.
 *  Ralf Haegenlaeuer             HRf           Vector Informatik GmbH
 *  Sebastian Loos                Shs           Vector Informatik GmbH
 *  Wei Yang                      Wyg           Vector Informatik GmbH
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  01.00.00   2015-01-16  AWh     -                Creation
 *                         Dod     -                Update to be compatible with released fbl_hdr
 *                                 -                Update for SEC_ENABLE_WORKSPACE_EXTERNAL configuration
 *                         AWh     -                Reset calPartActive on Programming error
 *                                                  Allow for valid processing of 0-length Transfers
 *                                                  Introduce Err_Undefined
 *                                                  Set Programming failure if FblHdrPostFlashValidation fails
 *                                 -                Correct Shift to 16bit Did
 *                                 -                Correct Routine Id positive response format
 *                                 ESCAN00075381    Failing mem-driver init on erase routine results in wrong NRC
 *  02.00.00   2015-01-16  AWh     ESCAN00078613    Use FblDiag_14229_Core component
 *                                 ESCAN00078404    Bootloader only accepts data parameter F3 for service 28
 *                                 ESCAN00078614    Compile error: FBL_ENABLE_CALIBRATION_MODULES and
 *                                                  FBL_ENABLE_DEBUG_STATUS need to be set
 *                                 ESCAN00078804    FblEraseRoutine will always set Partition error if
 *                                                  FBL_ENABLE_CALIBRATION_MODULES is disabled
 *                                 ESCAN00078837    Remove Gap Fill support (now  completely in FblHdr)
 *                                 -                Rename FblHdrSetPartId to FblHdrSetParsedModPartId
 *                                 ESCAN00078915    Add Stay In boot feature, rename DiagSetError to FblDiagSetError
 *                                 ESCAN00078613    Remove Sleepmode macros and vars now in core, add unlock upon Init
 *                                                   in case of start from Appl
 *                                 ESCAN00079275    Erase Routine request is not answered when transitioning from
 *                                                   application to boot.
 *  03.00.00   2015-01-16  AWh     ESCAN00079564    Api Change to allow new FblSecHdr module supporting compression
 *                                 ESCAN00079615    Allow only one tester to enter non-default session
 *                                 ESCAN00079719    No changes
 *                                 ESCAN00079564    Corrections
 *                                 ESCAN00080449    Remove 0x27 0x07 subparam
 *  03.01.00   2015-06-12  AWh     ESCAN00082174    When Sending 0x10 0x02 within Default Session, the Fbl sends NRC 33
 *                                                   (now use FblDiag_14229_Core 2.xx branch )
 *                                 ESCAN00083131    Distinguish sent NRCs on 0x31 error states (do not only send NRC 72)
 *  03.02.00   2015-07-23  AWh     ESCAN00083469    Prepare component for Swcp process
 *                                 ESCAN00083865    Assemble flashdriver set error to Debug status information
 *                                 ESCAN00083883    No changes
 *  03.03.00   2015-12-04  AWh     ESCAN00085579    NRC31 instead of NRC72 is returned on reception of invalid Part Id
 *                                 ESCAN00085914    No changes
 *                                 ESCAN00085914    Allow redefinition of LibMem callback interface
 *  03.04.00   2016-02-29  HRf     ESCAN00088540    No changes
 *                                 ESCAN00088611    Support multiple diagnostic testers with FblWrapperCom_PduR
 *  03.05.00   2016-03-16  Shs     ESCAN00088916    Added support for PATAC SLP2
 *                                 ESCAN00089094    Wrong PEC on 0x36 in case data file size is reached
 *  04.00.00   2016-05-11  AWh     ESCAN00089529    Allow usage of FblDiag_SecHdr_Gm 3.xx.xx interface
 *  04.01.00   2016-11-10  AWh     ESCAN00090256    GB6000 changed requirements 2014_12_15 to 2016_03_02
 *                                 ESCAN00090901    GB6000 changed requirements per 2016_04_21 release
 *                                 ESCAN00091143    Changes to allow usage of non-Kb main and tool configured Cw
 *                                 ESCAN00091409    Add switch to allow using component unchanged in DemoAppl
 *                                 ESCAN00092312    Compiler error (IAR): Memory location definition and declaration for
 *                                                   variable errStatFileName does not match
 *                         Dod     ESCAN00092740    Test tool reports no response to routine updatePSI
 *  04.01.01   2016-11-12  AWh     ESCAN00092772    Correct V_GEN_GENERATOR_MSR use case, sleep default time 8s for GM
 *                                 ESCAN00092793    SLP6: ExtInitReflash response is wrong
 *  04.01.02   2016-11-22  AWh     ESCAN00092937    Required FBL_ENABLE_SERVICE_POSTHANDLER is not defined
 *  04.01.03   2016-12-14  Shs     ESCAN00093225    Added Support for PATAC SLP2 GEM Security Access and InitExtReflash
 *                                                   Routine
 *                                 ESCAN00093243    Posthandlers are not defined
 *                         AWh     ESCAN00093284    Allow for more flexible 11-bit Rx handling
 *  04.01.04   2017-02-09  AWh     ESCAN00093739    No changes
 *                                 ESCAN00093934    Fbl responds with NRC 7F on OTA routine received in 10 01 or 10 02
 *                                 ESCAN00093935    Patac GEM: RID 21E (ExtReflash) does not have status response byte
 *  04.02.00   2017-03-07  Wyg     ESCAN00094278    Make StayInBoot message configurable
 *  04.02.01   2017-03-20  AWh     ESCAN00094415    SBAT valid: Calibration file download is rejected
 *                         AWh     ESCAN00093957    No changes
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

/* PRQA S 0857 EOF */ /* MD_FblDiag_0857 */
/* Common Bootloader includes */
#include "fbl_inc.h"
/* Core Diagnostic interface and global symbols */
#include "fbl_diag_core.h"
#include "flashrom.h" /* To verify correct configuration */

/***********************************************************************************************************************
 *  VERSION
 **********************************************************************************************************************/

/* Diagnostic OEM module version check */
#if ( FBLDIAG_14229_GM_VERSION != 0x0402u ) || \
    ( FBLDIAG_14229_GM_RELEASE_VERSION != 0x01u )
# error "Error in fbl_diag_oem.c: Source and header file are inconsistent!"
#endif

#if(( FBLDIAG_14229_GM_VERSION != _FBLDIAG_OEM_VERSION ) || \
    ( FBLDIAG_14229_GM_RELEASE_VERSION != _FBLDIAG_OEM_RELEASE_VERSION ))
# error "Error in fbl_diag_oem.c: Source and v_ver.h are inconsistent!"
#endif

#if ( FBLDIAGHDR_GM_VERSION < 0x200u )
# error "Error in fbl_diag_oem.c: Wrong interface to fbl_hdr.c!"
#endif

#if defined( FLASHDRV_DECRYPTVALUE )
#else
# error "FLASHDRV_DECRYPTVALUE is required for FBL_ENABLE_FLASHDRV_ROM use case."
#endif

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

#if defined( FBL_ENABLE_STAY_IN_BOOT )
# if !defined( FBL_DIAG_STAY_IN_BOOT_ARRAY )
/** Default value of stay in boot message */
#  define FBL_DIAG_STAY_IN_BOOT_ARRAY {kDiagSidRoutineControl, kDiagSubStartRoutine, kDiagRoutineIdStayInBootHigh, kDiagRoutineIdStayInBootLow}
#  define FBL_DIAG_STAY_IN_BOOT_ROUTINECONTROL
# endif  /* FBL_DIAG_STAY_IN_BOOT_ARRAY */
#endif /* FBL_ENABLE_STAY_IN_BOOT */

/***********************************************************************************************************************
 *  State handling
 **********************************************************************************************************************/

/* Download sequence states */
#define SetDisableNormalCom()             SetDiagState(kDiagStateIdxDisableNormalCom)
#define ClrDisableNormalCom()             ClrDiagState(kDiagStateIdxDisableNormalCom)

#define SetSecurityUnlock()               SetDiagState(kDiagStateIdxSecurityUnlock)
#define ClrSecurityUnlock()               ClrDiagState(kDiagStateIdxSecurityUnlock)

/***********************************************************************************************************************
 *  Local constants
 **********************************************************************************************************************/

#define kDiagInitSequenceNum                             1u

/***********************************************************************************************************************
 *  Handler Indices
 **********************************************************************************************************************/

/* Service handler indices */
/* Defined in DiagCore: #define kServiceMainHandlerNoHandler                  -1)*/
/*  kServiceMainHandlerDefault (fbl_diag_core.h)         0u */  /* Idx to FblDiagDefaultMainHandler */
#define kServiceMainHandlerDummy                         1u     /* Idx to FblDiagDummyMainHandler */
#define kServiceMainHandlerDefaultSession                2u     /* Idx to FblDiagDefaultSessionMainHandler */
#define kServiceMainHandlerExtendedSession               3u     /* Idx to FblDiagExtendedSessionMainHandler */
#define kServiceMainHandlerProgrammingSession            4u     /* Idx to FblDiagProgrammingSessionMainHandler */
#define kServiceMainHandlerReadDataById                  5u     /* Idx to FblDiagReadDataByIdMainHandler */
#define kServiceMainHandlerSecAccessSeed                 6u     /* Idx to FblDiagSecAccessSeedMainHandler */
#define kServiceMainHandlerCommCtrl                      7u     /* Idx to FblDiagCommCtrlMainHandler */
#define kServiceMainHandlerRC_EraseMemory                8u     /* Idx to FblDiagRCEraseMemoryMainHandler */
#define kServiceMainHandlerRC_UpdatePSI                  9u     /* Idx to FblDiagRCUpdatePSIMainHandler */
#define kServiceMainHandlerRequestDL                    10u     /* Idx to FblDiagRequestDownloadMainHandler */
#define kServiceMainHandlerTransferData                 11u     /* Idx to FblDiagTransferDataMainHandler */
#define kServiceMainHandlerTransferExit                 12u     /* Idx to FblDiagReqTransferExitMainHandler */
#define kServiceMainHandlerTesterPresent                13u     /* Idx to FblDiagTesterPresentMainHandler */
#define kServiceMainHandlerControlDTC                   14u     /* Idx to FblDiagControlDTCMainHandler */
/* Optional Services */
#if defined( FBL_DIAG_STAY_IN_BOOT_ROUTINECONTROL )
# define kServiceMainHandlerRC_ForceBootMode            (kServiceMainHandlerControlDTC + 1u)        /* Idx to FblDiagRCStartForceBootModeMainHandler */
#else
# define kServiceMainHandlerRC_ForceBootMode             kServiceMainHandlerControlDTC              /* Introduce unused virtual Idx */
#endif /* FBL_DIAG_STAY_IN_BOOT_ROUTINECONTROL */
# if defined( FBL_DIAG_ENABLE_GM_ROUTINE_CHECKPROGDEP )
#  define kServiceMainHandlerRC_CheckProgDep            (kServiceMainHandlerRC_ForceBootMode + 1u) /* Idx to FblDiagRCCheckProgDepMainHandler */
# else
#  define kServiceMainHandlerRC_CheckProgDep             kServiceMainHandlerRC_ForceBootMode       /* Introduce unused virtual Idx */
# endif /* FBL_DIAG_ENABLE_GM_ROUTINE_CHECKPROGDEP */
# if defined( FBL_DIAG_ENABLE_GM_ROUTINE_SETPROGSTATEFLAG )
#  define kServiceMainHandlerRC_SetProgStateFlag        (kServiceMainHandlerRC_CheckProgDep + 1u)  /* Idx to FblDiagRCSetProgStateFlagMainHandler */
# else
#  define kServiceMainHandlerRC_SetProgStateFlag         kServiceMainHandlerRC_CheckProgDep        /* Introduce unused virtual Idx */
# endif /* FBL_DIAG_ENABLE_GM_ROUTINE_SETPROGSTATEFLAG */
# if defined( FBL_DIAG_ENABLE_GM_ROUTINE_INITEXTREFLASH )
#  define kServiceMainHandlerRC_InitExtReflash         (kServiceMainHandlerRC_SetProgStateFlag + 1u)  /* Idx to FblDiagRCInitExtReflashMainHandler */
# else
#  define kServiceMainHandlerRC_InitExtReflash          kServiceMainHandlerRC_SetProgStateFlag        /* Introduce unused virtual Idx */
# endif /* FBL_DIAG_ENABLE_GM_ROUTINE_INITEXTREFLASH */

/* Session support Routine Ids */
#if defined( FBL_DIAG_STAY_IN_BOOT_ROUTINECONTROL )
# define kDiagSessionStayInBoot kDiagSessionDefault /* Stay in Boot is supported in default session */
#else
# define kDiagSessionStayInBoot 0x0u /* Stay in Boot is not supported in any session */
#endif /* FBL_DIAG_STAY_IN_BOOT_ROUTINECONTROL */

# if defined( FBL_DIAG_ENABLE_GM_ROUTINE_CHECKPROGDEP ) || defined( FBL_DIAG_ENABLE_GM_ROUTINE_SETPROGSTATEFLAG ) || defined( FBL_DIAG_ENABLE_GM_ROUTINE_INITEXTREFLASH )
#  define kDiagSessionOTA kDiagSessionDefault|kDiagSessionProgramming|kDiagSessionExtended /* OTA Routines are supported in all sessions */
# else
#  define kDiagSessionOTA 0x0u /* OTA Routines are not supported in any session */
# endif /* FBL_DIAG_ENABLE_GM_ROUTINE_CHECKPROGDEP || FBL_DIAG_ENABLE_GM_ROUTINE_SETPROGSTATEFLAG || FBL_DIAG_ENABLE_GM_ROUTINE_INITEXTREFLASH */

#define FBL_DIAG_GM_NO_TESTER_CONNECTED 0x00u  /* Init value for diagTesterAddr */

#if !defined( FBL_DIAG_ENABLE_GM_RESET_TESTER_IN_DEF_SESSION )
# define FBL_DIAG_DISABLE_GM_RESET_TESTER_IN_DEF_SESSION  /* Per default reset tester connection in default session. */
#endif /* FBL_DIAG_ENABLE_GM_RESET_TESTER_IN_DEF_SESSION */

/***********************************************************************************************************************
 *  TYPEDEFS AND STRUCTURES FOR INTERNAL USE
 **********************************************************************************************************************/

typedef enum tagDiagSequCnt{
   kDiagSequCntCorrect,
   kDiagSequCntIncorrect,
   kDiagSequCntRepeated
} tDiagSequCnt;

/***********************************************************************************************************************
 *  LOCAL DATA
 **********************************************************************************************************************/

/** Block sequence counter */
V_MEMRAM0 static V_MEMRAM1 vuint8 V_MEMRAM2 expectedSequenceCnt;
/** Current block sequence counter */
V_MEMRAM0 static V_MEMRAM1 vuint8 V_MEMRAM2 currentSequenceCnt;
#if defined( V_GEN_GENERATOR_MSR )
#else
/** Current Tester connected to */
V_MEMRAM0 static V_MEMRAM1 vuint8 V_MEMRAM2 diagTesterAddr;
#endif /* V_GEN_GENERATOR_MSR */

/*---------------------------------------------------------------------------*/
/*-- Subfunction table configuration ----------------------------------------*/
/*---------------------------------------------------------------------------*/

/*-- Subfunction: SessionControl --------------------------------------------*/
V_MEMROM0 static V_MEMROM1 vuint8 V_MEMROM2 kDiagSubFctTbl_DefSession[] = { kDiagSubDefaultSession };
V_MEMROM0 static V_MEMROM1 vuint8 V_MEMROM2 kDiagSubFctTbl_PrgSession[] = { kDiagSubProgrammingSession };
V_MEMROM0 static V_MEMROM1 vuint8 V_MEMROM2 kDiagSubFctTbl_ExtSession[] = { kDiagSubExtendedDiagSession };

V_MEMROM0 static V_MEMROM1 tSubFctInfoTable V_MEMROM2 kSubFctInfoTable_SessionCtrl[] =
{
   {
       kDiagSubFctTbl_DefSession
      ,(kDiagFunctionalRequestAllowed|kDiagSessionDefault|kDiagSessionProgramming|kDiagSessionExtended)
      ,kDiagRqlDiagnosticSessionControl
      ,kServiceMainHandlerDefaultSession
   }
  ,{
       kDiagSubFctTbl_PrgSession
      ,(kDiagFunctionalRequestAllowed|kDiagSecuredService|kDiagSessionProgramming|kDiagSessionExtended)
      ,kDiagRqlDiagnosticSessionControl
      ,kServiceMainHandlerProgrammingSession
   }
  ,{
       kDiagSubFctTbl_ExtSession
      ,(kDiagFunctionalRequestAllowed|kDiagSessionDefault|kDiagSessionExtended)
      ,kDiagRqlDiagnosticSessionControl
      ,kServiceMainHandlerExtendedSession
   }
};

/*-- Subfunction: TesterPresent ---------------------------------------------*/
V_MEMROM0 static V_MEMROM1 vuint8 V_MEMROM2 kDiagSubFctTbl_TesterPresent[] = { kDiagSubTesterPresent };
V_MEMROM0 static V_MEMROM1 tSubFctInfoTable V_MEMROM2 kSubFctInfoTable_TesterPresent[] =
{
   {
       kDiagSubFctTbl_TesterPresent
      ,(kDiagFunctionalRequestAllowed|kDiagSessionDefault|kDiagSessionProgramming|kDiagSessionExtended)
      ,kDiagRqlTesterPresent
      ,kServiceMainHandlerNoHandler
   }
};

/*-- Subfunction: ControlDTC ------------------------------------------------*/
V_MEMROM0 static V_MEMROM1 vuint8 V_MEMROM2 kDiagSubFctTbl_DtcControlOn[]  = { kDiagSubDtcOn };
V_MEMROM0 static V_MEMROM1 vuint8 V_MEMROM2 kDiagSubFctTbl_DtcControlOff[] = { kDiagSubDtcOff };

V_MEMROM0 static V_MEMROM1 tSubFctInfoTable V_MEMROM2 kSubFctInfoTable_ControlDTC[] =
{
   {
       kDiagSubFctTbl_DtcControlOn
      ,(kDiagFunctionalRequestAllowed|kDiagSessionExtended)
      ,kDiagRqlControlDTCSetting
      ,kServiceMainHandlerNoHandler
   }
  ,{
       kDiagSubFctTbl_DtcControlOff
      ,(kDiagFunctionalRequestAllowed|kDiagSessionExtended)
      ,kDiagRqlControlDTCSetting
      ,kServiceMainHandlerNoHandler
   }
};

/*-- Subfunction: CommunicationControl --------------------------------------*/
V_MEMROM0 static V_MEMROM1 vuint8 V_MEMROM2 kDiagSubFctTbl_CommCtrl_EnaRxEnaTx[] = { kDiagSubEnableRxAndTx };
V_MEMROM0 static V_MEMROM1 vuint8 V_MEMROM2 kDiagSubFctTbl_CommCtrl_DisaRxDisTx[] = { kDiagSubDisableRxAndTx };

V_MEMROM0 static V_MEMROM1 tSubFctInfoTable V_MEMROM2 kSubFctInfoTable_CommCtrl[] =
{
   {
       kDiagSubFctTbl_CommCtrl_EnaRxEnaTx
      ,(kDiagFunctionalRequestAllowed|kDiagSessionExtended)
      ,kDiagRqlCommunicationControl
      ,kServiceMainHandlerNoHandler
   }
  ,{
       kDiagSubFctTbl_CommCtrl_DisaRxDisTx
      ,(kDiagFunctionalRequestAllowed|kDiagSessionExtended)
      ,kDiagRqlCommunicationControl
      ,kServiceMainHandlerNoHandler
   }
};

/*-- Subfunction: RoutineControl --------------------------------------------*/
V_MEMROM0 static V_MEMROM1 vuint8 V_MEMROM2 kDiagSubFctTbl_EraseMemory[] = { kDiagSubStartRoutine, kDiagRoutineIdEraseMemoryHigh, kDiagRoutineIdEraseMemoryLow };
V_MEMROM0 static V_MEMROM1 vuint8 V_MEMROM2 kDiagSubFctTbl_UpdatePSI[] = { kDiagSubStartRoutine, kDiagRoutineIdUpdatePSIHigh, kDiagRoutineIdUpdatePSILow };
#if defined(FBL_DIAG_STAY_IN_BOOT_ROUTINECONTROL)
V_MEMROM0 static V_MEMROM1 vuint8 V_MEMROM2 kDiagSubFctTbl_RCStart_ForceBoot[] = { kDiagSubStartRoutine, kDiagRoutineIdStayInBootHigh, kDiagRoutineIdStayInBootLow };
#endif /*FBL_DIAG_STAY_IN_BOOT_ROUTINECONTROL*/
#  if defined( FBL_DIAG_ENABLE_GM_ROUTINE_CHECKPROGDEP )
V_MEMROM0 static V_MEMROM1 vuint8 V_MEMROM2 kDiagSubFctTbl_CheckProgDep [] = { kDiagSubStartRoutine, kDiagRoutineIdCheckProgDepHigh, kDiagRoutineIdCheckProgDepLow };
#  endif /* FBL_DIAG_ENABLE_GM_ROUTINE_CHECKPROGDEP */
#  if defined( FBL_DIAG_ENABLE_GM_ROUTINE_SETPROGSTATEFLAG )
V_MEMROM0 static V_MEMROM1 vuint8 V_MEMROM2 kDiagSubFctTbl_SetProgStateFlag [] = { kDiagSubStartRoutine, kDiagRoutineIdSetProgStateFlagHigh, kDiagRoutineIdSetProgStateFlagLow };
#  endif /* FBL_DIAG_ENABLE_GM_ROUTINE_SETPROGSTATEFLAG */
# if defined( FBL_DIAG_ENABLE_GM_ROUTINE_INITEXTREFLASH )
V_MEMROM0 static V_MEMROM1 vuint8 V_MEMROM2 kDiagSubFctTbl_InitExtReflash [] = { kDiagSubStartRoutine, kDiagRoutineIdInitExtReflashHigh, kDiagRoutineIdInitExtReflashLow };
# endif

V_MEMROM0 static V_MEMROM1 tSubFctInfoTable V_MEMROM2 kSubFctInfoTable_RoutineCtrl[] =
{

   /* Routines are not secured. Gm: "Securiy Level: Passed Preconditions for getting into boot".*/
   {
       kDiagSubFctTbl_EraseMemory
      ,(kDiagFunctionalRequestAllowed|kDiagSessionProgramming)
      ,kDiagRqlRoutineControlEraseMemory
      ,kServiceMainHandlerRC_EraseMemory
   }
  ,{
       kDiagSubFctTbl_UpdatePSI
      ,(kDiagFunctionalRequestAllowed|kDiagSessionProgramming)
      ,kDiagRqlRoutineControlUpdatePSI
      ,kServiceMainHandlerRC_UpdatePSI
   }
#if defined(FBL_DIAG_STAY_IN_BOOT_ROUTINECONTROL)
  ,{
      /* Force boot mode */
       kDiagSubFctTbl_RCStart_ForceBoot
      ,(kDiagSessionDefault|kDiagSessionExtended|kDiagSessionProgramming)
      ,kDiagRqlRoutineControlForceBoot
      ,kServiceMainHandlerRC_ForceBootMode
   }
#endif /* FBL_DIAG_STAY_IN_BOOT_ROUTINECONTROL */
#  if defined( FBL_DIAG_ENABLE_GM_ROUTINE_CHECKPROGDEP )
  ,{
       kDiagSubFctTbl_CheckProgDep
      ,(kDiagFunctionalRequestAllowed|kDiagSessionDefault|kDiagSessionExtended|kDiagSessionProgramming)
      ,kDiagRqlRoutineControlCheckProgDep
      ,kServiceMainHandlerRC_CheckProgDep
   }
#  endif /* FBL_DIAG_ENABLE_GM_ROUTINE_CHECKPROGDEP */
#  if defined( FBL_DIAG_ENABLE_GM_ROUTINE_SETPROGSTATEFLAG )
  ,{
       kDiagSubFctTbl_SetProgStateFlag
      ,(kDiagFunctionalRequestAllowed|kDiagSecuredService|kDiagSessionDefault|kDiagSessionExtended|kDiagSessionProgramming)
      ,kDiagRqlRoutineControlSetProgStateFlag
      ,kServiceMainHandlerRC_SetProgStateFlag
   }
#  endif /* FBL_DIAG_ENABLE_GM_ROUTINE_SETPROGSTATEFLAG */
# if defined( FBL_DIAG_ENABLE_GM_ROUTINE_INITEXTREFLASH )
  ,{
       kDiagSubFctTbl_InitExtReflash
      ,(kDiagFunctionalRequestAllowed|kDiagSessionDefault|kDiagSessionExtended|kDiagSessionProgramming)
      ,kDiagRqlRoutineControlInitExtReflash
      ,kServiceMainHandlerRC_InitExtReflash
   }
# endif /* FBL_DIAG_ENABLE_GM_ROUTINE_INITEXTREFLASH */
};

/*-- Subfunction: SecurityAccess --------------------------------------------*/
V_MEMROM0 static V_MEMROM1 vuint8 V_MEMROM2 kDiagSubFctTbl_SecAccess_SeedService[] = { kDiagSubRequestSeedService };
V_MEMROM0 static V_MEMROM1 vuint8 V_MEMROM2 kDiagSubFctTbl_SecAccess_SeedAssembly[] = { kDiagSubRequestSeedAssembly };
#if defined( FBL_DIAG_ENABLE_GM_REQUEST_SEED_OTA )
V_MEMROM0 static V_MEMROM1 vuint8 V_MEMROM2 kDiagSubFctTbl_SecAccess_SeedOta[] = { kDiagSubRequestSeedOta };
#endif
V_MEMROM0 static V_MEMROM1 tSubFctInfoTable V_MEMROM2 kSubFctInfoTable_SecAccess[] =
{
   {
       kDiagSubFctTbl_SecAccess_SeedService
      ,(kDiagFunctionalRequestAllowed|kDiagSessionProgramming|kDiagSessionExtended)
      ,kDiagRqlSecurityAccessSeed
      ,kServiceMainHandlerSecAccessSeed
   }
  ,{
       kDiagSubFctTbl_SecAccess_SeedAssembly
      ,(kDiagFunctionalRequestAllowed|kDiagSessionProgramming|kDiagSessionExtended)
      ,kDiagRqlSecurityAccessSeed
      ,kServiceMainHandlerSecAccessSeed
   }
#if defined( FBL_DIAG_ENABLE_GM_REQUEST_SEED_OTA )
  ,{
       kDiagSubFctTbl_SecAccess_SeedOta
      ,(kDiagFunctionalRequestAllowed|kDiagSessionProgramming|kDiagSessionExtended)
      ,kDiagRqlSecurityAccessSeed
      ,kServiceMainHandlerSecAccessSeed
   }
#endif
};

/***********************************************************************************************************************
 *  PROTOTYPES
 **********************************************************************************************************************/

/*-- Diagnostic service helper functions ------------------------------------*/
static void FblDiagSetNrcFromPecResult( void );
static tDiagSequCnt FblDiagHandleSequenceCounter( void );
static void FblDiagSessionControlParamInit(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen);

/***********************************************************************************************************************
 *    Diagnostic main handler service functions
 **********************************************************************************************************************/

/*-- Dummy Handler ----------------------------------------------------------*/
static tFblResult FblDiagDummyMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen);

/*-- Session handling -------------------------------------------------------*/
static tFblResult FblDiagDefaultSessionMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen);
static tFblResult FblDiagExtendedSessionMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen);
static tFblResult FblDiagProgrammingSessionMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen);
static tFblResult FblDiagTesterPresentMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen);

/*-- DID handling -----------------------------------------------------------*/
static tFblResult FblDiagReadDataByIdMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen);

/*-- Security access --------------------------------------------------------*/
static tFblResult FblDiagSecAccessSeedMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen);

/*-- Communication control / DTC handling -----------------------------------*/
static tFblResult FblDiagCommCtrlMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen);
static tFblResult FblDiagControlDTCMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen);

/*-- Routine control --------------------------------------------------------*/
static tFblResult FblDiagRCEraseMemoryMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen);
static tFblResult FblDiagRCUpdatePSIMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen);
#  if defined( FBL_DIAG_ENABLE_GM_ROUTINE_CHECKPROGDEP )
static tFblResult FblDiagRCCheckProgDepMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen);
#  endif /* FBL_DIAG_ENABLE_GM_ROUTINE_CHECKPROGDEP */
#  if defined( FBL_DIAG_ENABLE_GM_ROUTINE_SETPROGSTATEFLAG )
static tFblResult FblDiagRCSetProgStateFlagMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen);
#  endif /* FBL_DIAG_ENABLE_GM_ROUTINE_SETPROGSTATEFLAG */
# if defined( FBL_DIAG_ENABLE_GM_ROUTINE_INITEXTREFLASH )
static tFblResult FblDiagRCInitExtReflashMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen);
# endif /* FBL_DIAG_ENABLE_GM_ROUTINE_INITEXTREFLASH */
#if defined(FBL_DIAG_STAY_IN_BOOT_ROUTINECONTROL)
  static tFblResult FblDiagRCStartForceBootModeMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen);
#endif /* FBL_DIAG_STAY_IN_BOOT_ROUTINECONTROL */

/*-- Data transfer ----------------------------------------------------------*/
static tFblResult FblDiagRequestDownloadMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen);
static tFblResult FblDiagTransferDataMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen);
static tFblResult FblDiagReqTransferExitMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen);

/***********************************************************************************************************************
 *  EXTERNAL DATA
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  Diagnostic handler function call table
 **********************************************************************************************************************/

/* Service handler function table. Indices are defined above */
V_MEMROM0 V_MEMROM1 tFblDiagMainHandler V_MEMROM2 kServiceMainHandlerFctTable[] =       /*  0 */   { FblDiagDefaultMainHandler
                                                                                        /*  1 */    ,FblDiagDummyMainHandler
                                                                                        /*  2 */    ,FblDiagDefaultSessionMainHandler
                                                                                        /*  3 */    ,FblDiagExtendedSessionMainHandler
                                                                                        /*  4 */    ,FblDiagProgrammingSessionMainHandler
                                                                                        /*  5 */    ,FblDiagReadDataByIdMainHandler
                                                                                        /*  6 */    ,FblDiagSecAccessSeedMainHandler
                                                                                        /*  7 */    ,FblDiagCommCtrlMainHandler
                                                                                        /*  8 */    ,FblDiagRCEraseMemoryMainHandler
                                                                                        /*  9 */    ,FblDiagRCUpdatePSIMainHandler
                                                                                        /* 10 */    ,FblDiagRequestDownloadMainHandler
                                                                                        /* 11 */    ,FblDiagTransferDataMainHandler
                                                                                        /* 12 */    ,FblDiagReqTransferExitMainHandler
                                                                                        /* 13 */    ,FblDiagTesterPresentMainHandler
                                                                                        /* 14 */    ,FblDiagControlDTCMainHandler
#if defined( FBL_DIAG_STAY_IN_BOOT_ROUTINECONTROL )
                                                                                    /* 14 + w */    ,FblDiagRCStartForceBootModeMainHandler
#endif /* FBL_DIAG_STAY_IN_BOOT_ROUTINECONTROL */
#  if defined( FBL_DIAG_ENABLE_GM_ROUTINE_CHECKPROGDEP )
                                                                                    /* 14 + x */    ,FblDiagRCCheckProgDepMainHandler
#  endif /* FBL_DIAG_ENABLE_GM_ROUTINE_CHECKPROGDEP */
#  if defined( FBL_DIAG_ENABLE_GM_ROUTINE_SETPROGSTATEFLAG )
                                                                                    /* 14 + y */    ,FblDiagRCSetProgStateFlagMainHandler
#  endif /* FBL_DIAG_ENABLE_GM_ROUTINE_SETPROGSTATEFLAG */
# if defined( FBL_DIAG_ENABLE_GM_ROUTINE_INITEXTREFLASH )
                                                                                    /* 14 + z */    ,FblDiagRCInitExtReflashMainHandler
# endif /* FBL_DIAG_ENABLE_GM_ROUTINE_INITEXTREFLASH */
};

/***********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

#if defined( FBL_ENABLE_DEBUG_STATUS )
/* The below Debug status relevant variables are initialized inside ApplFblInitErrStatus */
/* Tp error related */
V_MEMRAM0 V_MEMRAM1 vuint8 V_MEMRAM2 errStatTpError; /**< Allow reporting of Tp error (requires using Error indication to set this error) */

/* Device driver related */
V_MEMRAM0 V_MEMRAM1 vuint8 V_MEMRAM2 errStatHaveDriver; /**< Status flag to indicate Driver involved. */
V_MEMRAM0 V_MEMRAM1 tFblAddress V_MEMRAM2 errStatAddress; /**< Addr where Device Driver Error occurred. */

/* Module related Info */
V_MEMRAM0 V_MEMRAM1 tDebugFileName V_MEMRAM2 errStatFileName; /**< file name information for Debug Status */
V_MEMRAM0 V_MEMRAM1 tDebugLineNbr V_MEMRAM2 errStatLineNumber; /**< line number information for Debug Status */
#endif

#if defined( FBL_ENABLE_SERVICE_POSTHANDLER )
/** Posthandler function table configuration */
/* Index into the PostHandler table */
# define kServicePostHandlerDefault 0ul
/* Start codeseg to be executed in RAM */
# define FBLDIAG_START_SEC_RAMCONST_UNSPECIFIED
# include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */
/* The handler table */
V_MEMROM0 V_MEMROM1 tFblDiagPostHandler V_MEMROM2 kServicePostHandlerFctTable[] =
{
   FblDiagDefaultPostHandler
};
/* End section to execute code from RAM */
# define FBLDIAG_STOP_SEC_RAMCONST_UNSPECIFIED
# include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */
#endif /* FBL_ENABLE_SERVICE_POSTHANDLER */

/*---------------------------------------------------------------------------*/
/*-- Main service table configuration ---------------------------------------*/
/*---------------------------------------------------------------------------*/
/* PRQA S 3218 1 */ /* MD_FblDiag_3218 */
V_MEMROM0 V_MEMROM1 tServiceInfoTable V_MEMROM2 kServiceInfoTable[] =
{
  { /* 00:Definition for StartDiagnosticSession (0x10) */
    kDiagSidDiagnosticSessionControl
   ,(kDiagFunctionalRequestAllowed|kDiagServiceIsSubfunction|kDiagSessionDefault|kDiagSessionExtended|kDiagSessionProgramming)
   ,(kDiagRqlDiagnosticSessionControl)
   ,(ARRAY_SIZE(kSubFctInfoTable_SessionCtrl))
   ,(ARRAY_SIZE(kDiagSubFctTbl_DefSession))
   ,kSubFctInfoTable_SessionCtrl
   ,kServiceMainHandlerDummy
#if defined( FBL_ENABLE_SERVICE_POSTHANDLER )
   ,kServicePostHandlerDefault
#endif
  }
 ,{ /* 01:Definition for CommunicationControl (0x28) */
    kDiagSidCommunicationControl
   ,(kDiagFunctionalRequestAllowed|kDiagServiceIsSubfunction|kDiagSessionExtended)
   ,(kDiagRqlCommunicationControl)
   ,(ARRAY_SIZE(kSubFctInfoTable_CommCtrl))
   ,(ARRAY_SIZE(kDiagSubFctTbl_CommCtrl_EnaRxEnaTx))
   ,kSubFctInfoTable_CommCtrl
   ,kServiceMainHandlerCommCtrl
#if defined( FBL_ENABLE_SERVICE_POSTHANDLER )
   ,kServicePostHandlerDefault
#endif
  }
 ,{ /* 02:Definition for SecurityAccess (0x27) */
    kDiagSidSecurityAccess
   ,(kDiagFunctionalRequestAllowed|kDiagServiceIsSubfunction|kDiagSessionProgramming|kDiagSessionExtended)
   ,kDiagRqlSecurityAccessSeed
   ,(ARRAY_SIZE(kSubFctInfoTable_SecAccess))
   ,(ARRAY_SIZE(kDiagSubFctTbl_SecAccess_SeedService))
   ,kSubFctInfoTable_SecAccess
   ,kServiceMainHandlerDummy
#if defined( FBL_ENABLE_SERVICE_POSTHANDLER )
   ,kServicePostHandlerDefault
#endif
  }
 ,{ /* 03:Definition for RoutineControl (0x31) */
    kDiagSidRoutineControl
   ,(kDiagFunctionalRequestAllowed|kDiagServiceIsSubfunction|kDiagSessionProgramming|kDiagSessionOTA|kDiagSessionStayInBoot)
   ,(kDiagRqlRoutineControl)
   ,(ARRAY_SIZE(kSubFctInfoTable_RoutineCtrl))
   ,(ARRAY_SIZE(kDiagSubFctTbl_EraseMemory))
   ,kSubFctInfoTable_RoutineCtrl
   ,kServiceMainHandlerDummy
#if defined( FBL_ENABLE_SERVICE_POSTHANDLER )
   ,kServicePostHandlerDefault
#endif
  }
 ,{ /* 04:Definition for RequestDownload (0x34) */
    kDiagSidRequestDownload
   ,(kDiagFunctionalRequestAllowed|kDiagSessionProgramming)
   ,(kDiagRqlRequestDownload)
   ,0u
   ,0u
   ,kDiagSubFctNoInfoTable
   ,kServiceMainHandlerRequestDL
#if defined( FBL_ENABLE_SERVICE_POSTHANDLER )
   ,kServicePostHandlerDefault
#endif
  }
 ,{ /* 05:Definition for TransferData (0x36) */
    kDiagSidTransferData
   ,(kDiagFunctionalRequestAllowed|kDiagLengthSpecialCheck|kDiagSessionProgramming)
   ,(kDiagRqlTransferData + 0x01u) /* UDS: minimum length shall be 2 (1 data byte min.) excluding SID */
   ,0u
   ,0u
   ,kDiagSubFctNoInfoTable
   ,kServiceMainHandlerTransferData
#if defined( FBL_ENABLE_SERVICE_POSTHANDLER )
   ,kServicePostHandlerDefault
#endif
  }
 ,{ /* 06:Definition for TransferDataExit (0x37) */
    kDiagSidRequestTransferExit
   ,(kDiagFunctionalRequestAllowed|kDiagSessionProgramming)
   ,(kDiagRqlRequestTransferExit)
   ,0u
   ,0u
   ,kDiagSubFctNoInfoTable
   ,kServiceMainHandlerTransferExit
#if defined( FBL_ENABLE_SERVICE_POSTHANDLER )
   ,kServicePostHandlerDefault
#endif
  }
 ,{ /* 07:Definition for TesterPresent (0x3E) */
    kDiagSidTesterPresent
   ,(kDiagFunctionalRequestAllowed|kDiagServiceIsSubfunction|kDiagSessionDefault|kDiagSessionProgramming|kDiagSessionExtended)
   ,(kDiagRqlTesterPresent)
   ,(ARRAY_SIZE(kSubFctInfoTable_TesterPresent))
   ,(ARRAY_SIZE(kDiagSubFctTbl_TesterPresent))
   ,kSubFctInfoTable_TesterPresent
   ,kServiceMainHandlerTesterPresent
#if defined( FBL_ENABLE_SERVICE_POSTHANDLER )
   ,kServicePostHandlerDefault
#endif
  }
 ,{ /* 08:Definition for ReadDataById (0x22) */
    kDiagSidReadDataByIdentifier
   ,(kDiagFunctionalRequestAllowed|kDiagLengthSpecialCheck|kDiagSessionDefault|kDiagSessionExtended|kDiagSessionProgramming)
   ,(kDiagRqlReadDataByIdentifier)
   ,0u
   ,0u
   ,kDiagSubFctNoInfoTable
   ,kServiceMainHandlerReadDataById
#if defined( FBL_ENABLE_SERVICE_POSTHANDLER )
   ,kServicePostHandlerDefault
#endif
  }
 ,{ /* 09:Definition for ControlDTC (0x85) */
    kDiagSidControlDTCSetting
   ,(kDiagFunctionalRequestAllowed|kDiagServiceIsSubfunction|kDiagSessionExtended)
   ,(kDiagRqlControlDTCSetting)
   ,(ARRAY_SIZE(kSubFctInfoTable_ControlDTC))
   ,(ARRAY_SIZE(kDiagSubFctTbl_DtcControlOn))
   ,kSubFctInfoTable_ControlDTC
   ,kServiceMainHandlerControlDTC
#if defined( FBL_ENABLE_SERVICE_POSTHANDLER )
   ,kServicePostHandlerDefault
#endif
  }
 };

/** Size of service table */
V_MEMROM0 V_MEMROM1 vuintx V_MEMROM2 kSizeOfServiceTable = ARRAY_SIZE(kServiceInfoTable);
/** Size of main handler table */
V_MEMROM0 V_MEMROM1 vuintx V_MEMROM2 kSizeOfMainHandlerTable = ARRAY_SIZE(kServiceMainHandlerFctTable);

#if defined( FBL_ENABLE_STAY_IN_BOOT )
/** Stay in boot message definition */
V_MEMROM0 V_MEMROM1 vuint8 V_MEMROM2 fblDiagStayInBootMsg[] = FBL_DIAG_STAY_IN_BOOT_ARRAY; /* PRQA S 3684 */ /* MD_FblDiag_3684 */
/** Size of stay in boot message */
V_MEMROM0 V_MEMROM1 vuintx V_MEMROM2 kSizeOfStayInBootMsg = ARRAY_SIZE(fblDiagStayInBootMsg);
#endif  /* FBL_ENABLE_STAY_IN_BOOT */

/***********************************************************************************************************************
 *  Service support functions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  FblDiagOemInitPowerOn
 **********************************************************************************************************************/
/*! \brief       Initialize module variables
 **********************************************************************************************************************/
void FblDiagOemInitPowerOn(void)
{
   /* Sleep timer not running during startup */
   FblSleepCounterClear();

   FblHdrInitPowerOn();

}

/***********************************************************************************************************************
 *  FblDiagOemInit
 **********************************************************************************************************************/
/*! \brief       Initialize diagnostic module
 *  \details     Sets the diagnostic module to default session and initializes depending modules.
 **********************************************************************************************************************/
void FblDiagOemInit(void)
{
#if defined( FBL_ENABLE_DEBUG_STATUS )
   /* Initialize error status */
   ApplFblInitErrStatus();
#endif

#if defined( V_GEN_GENERATOR_MSR )
#else
   diagTesterAddr = FBL_DIAG_GM_NO_TESTER_CONNECTED;
#endif /* V_GEN_GENERATOR_MSR */

   if (FblMainGetStartFromAppl())
   {
#if defined( V_GEN_GENERATOR_MSR )
      /* Tester connection is set via FblCwPrepareResponseAddress() */
#else
      /* This fixes tester address to the one passed from application. */
      (void) ApplFblReadTpTargetAddr(&diagTesterAddr);
#endif /* V_GEN_GENERATOR_MSR */

      SetSecurityUnlock();
      SetDisableNormalCom();
   }
   else
   {
      /* Call Header Module Init, call it later in case of start from application to allow for a response pending */
      FblHdrInit(HDR_INTERFACE_TYPE_ALL_EXPLICIT);
   }
}
/* PRQA S 0277 EOF */ /* MD_FblDiag_0277 */
/* PRQA S 2006 EOF */ /* MD_CBD_14.7 */

#if defined( V_GEN_GENERATOR_MSR )
#else
/***********************************************************************************************************************
 *  FblDiagGetTesterAddr
 **********************************************************************************************************************/
/*! \brief       This routine will return the currently known tester address
 *  \detail      It is used to allow for correct 11-bit according to GB6000 2016 october version
**********************************************************************************************************************/
vuint8 FblDiagGetTesterAddr(void)
{
   vuint8 testerAddr = FBL_DIAG_RESP_TARGADDR_11BIT_INIT;

   if (FBL_DIAG_GM_NO_TESTER_CONNECTED != diagTesterAddr)
   {
      testerAddr = diagTesterAddr;
   }

   return testerAddr;
}
#endif

/***********************************************************************************************************************
 *  FblDiagProcessServiceNrc
 **********************************************************************************************************************/
/*! \brief       This routine maps service check errors to NRCs
 *  \details     The checks done in CheckSubServiceTable() are mapped to NRCs in this function. This function is
 *               called for services without sub functions and/sub params.
 *  \pre         Service available (serviceInfo initialized)
 *  \param[in]   subFctResult Error map from CheckSubServiceTable()
 *  \param[in]   pServiceInfo Diagnostic service details
 **********************************************************************************************************************/
void FblDiagProcessServiceNrc(V_MEMROM1 tServiceInfoTable V_MEMROM2 V_MEMROM3* pServiceInfo, vuint16 serviceCheckResult)
{
#if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   /* Parameters not used: avoid compiler warning */
   (void)pServiceInfo; /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif

   /* Session support check */
   if ((serviceCheckResult & kServiceSessionFailure) == kServiceSessionFailure)
   {
      DiagNRCServiceNotSupportedInActiveSession();
   }
   /* Minimum length check */
   else if ((serviceCheckResult & kServiceMinLenFailure) == kServiceMinLenFailure)
   {
      DiagNRCIncorrectMessageLengthOrInvalidFormat();
   }
   /* Max length check */
   else if ((serviceCheckResult & kServiceLenFailure) == kServiceLenFailure)
   {
      DiagNRCIncorrectMessageLengthOrInvalidFormat();
   }
   else
   {
      /* This point shouldn't be reached. If it is reached, check your configuration. */
      assertFblInternal(0u, kFblAssertInternalIlegalState);  /* PRQA S 2214 */ /* MD_FblDiag_2214 */
#if ( FBL_PRODUCTION == FBL_PROJECT_STATE )
      DiagNRCGeneralReject(); /* This should not be reached if configured correctly. */
#endif
   }
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/***********************************************************************************************************************
 *  FblDiagProcessRcNrc
 **********************************************************************************************************************/
/*! \brief       This routine maps service check errors to NRCs
 *  \details     The checks done in CheckSubServiceTable() are mapped to NRCs in this function. This function is
 *               called for Routine Control service.
 *  \pre         Service available (serviceInfo initialized)
 *  \param[in]   subFctResult Error map from CheckSubServiceTable()
 *  \param[in]   pServiceInfo Diagnostic service details
 **********************************************************************************************************************/
void FblDiagProcessRcNrc(V_MEMROM1 tServiceInfoTable V_MEMROM2 V_MEMROM3* pServiceInfo, vuint16 subFctResult)
{
#if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   /* Parameters not used: avoid compiler warning */
   (void)pServiceInfo;  /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif

   /* Check service level */
   if ((subFctResult & (kServiceSessionFailure)) == kServiceSessionFailure)
   {
      /* Service not supported in active session */
      DiagNRCServiceNotSupportedInActiveSession();
   }
   else if ((subFctResult & kServiceMinLenFailure) == kServiceMinLenFailure)
   {
      /* Minimum length check failed */
      DiagNRCIncorrectMessageLengthOrInvalidFormat();
   }
   /* Check subfunction level */
   else if ((subFctResult & (kCheckFunctionNotFound | kCheckParameterNotFound )) != 0u )
   {
      /* Failure returned, but no error set. */
      /* Means, subfunction not supported.   */
#if defined( FBL_ENABLE_USERSUBFUNCTION )
      /* Subfunction not found in check */
      /* Call user handler to manage its own subfunction */
      ApplDiagUserSubFunction(&DiagBuffer[kDiagFmtSubparam], DiagDataLength);

      /* Indicate, that a user callback was called */
      SetDiagHandlerCalled();
#else
      /* Special handling of routine control requests - routine ID has to be checked before */
      if ((subFctResult & kCheckParameterNotFound) == kCheckParameterNotFound)
      {
         /* Routine ID is checked before subfunction */
         DiagNRCRequestOutOfRange();
      }
      else
      {
         /* Subfunction not supported */
         DiagNRCSubFunctionNotSupported();
      }
#endif /* FBL_ENABLE_USERSUBFUNCTION */
   }
   else if ((subFctResult & kSubServiceTableMismatch) == kSubServiceTableMismatch)
   {
      /* Subfunction not supported. */
      DiagNRCRequestOutOfRange();
   }
   /* Security access on service level -> No secured services for GM SLP6. Keep it here to show potential order */
   else if ((subFctResult & kSubServiceSecAccFailure) == kSubServiceSecAccFailure)
   {
      /* Security level not correct */
      DiagNRCSecurityAccessDenied();
   }
   else if ((subFctResult & kSubServiceLenFailure) == kSubServiceLenFailure)
   {
      /* Length check of subservice failed */
      DiagNRCIncorrectMessageLengthOrInvalidFormat();
   }
   else if ((subFctResult & kSubServiceSessionFailure) == kSubServiceSessionFailure)
   {
      DiagNRCRequestOutOfRange();
   }
   else
   {
      /* This point shouldn't be reached. If it is reached, check your configuration. */
      assertFblInternal(0u, kFblAssertInternalIlegalState);  /* PRQA S 2214 */ /* MD_FblDiag_2214 */
#if ( FBL_PRODUCTION == FBL_PROJECT_STATE )
      DiagNRCGeneralReject(); /* This should not be reached if configured correctly. */
#endif
   }
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/***********************************************************************************************************************
 *  FblDiagProcessSubfunctionNrc
 **********************************************************************************************************************/
/*! \brief       This routine maps service check errors to NRCs
 *  \details     The checks done in CheckSubServiceTable() are mapped to NRCs in this function. This function is
 *               called for services with sub functions.
 *  \pre         Service available (serviceInfo initialized)
 *  \param[in]   subFctResult Error map from CheckSubServiceTable()
 *  \param[in]   pServiceInfo Diagnostic service details
 **********************************************************************************************************************/
void FblDiagProcessSubfunctionNrc(V_MEMROM1 tServiceInfoTable V_MEMROM2 V_MEMROM3* pServiceInfo, vuint16 subFctResult)
{
#if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   /* Parameters not used: avoid compiler warning */
   (void)pServiceInfo; /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif

   /* Check service level */
   /* Session support check */
   if ((subFctResult & kServiceSessionFailure) == kServiceSessionFailure)
   {
      DiagNRCServiceNotSupportedInActiveSession();
   }
   /* Minimum length check */
   else if ((subFctResult & kServiceMinLenFailure) == kServiceMinLenFailure)
   {
      DiagNRCIncorrectMessageLengthOrInvalidFormat();
   }
   /* Security access on service level */
   else if ((subFctResult & kServiceSecAccFailure) == kServiceSecAccFailure)
   {
      DiagNRCSecurityAccessDenied();
   }
   /* Check subfunction level */
   else if ((subFctResult &  kSubServiceFunctionNotFound) == kSubServiceFunctionNotFound)
   {
      /* Failure returned, but no error set. */
      /* Means, subfunction not supported.   */
#if defined( FBL_ENABLE_USERSUBFUNCTION )
      /* Call user handler to manage its own subfunction */
      ApplDiagUserSubFunction(&DiagBuffer[kDiagFmtSubparam], DiagDataLength);

      /* Indicate, that a user callback was called */
      SetDiagHandlerCalled();
#else
      /* Subfunction not supported */
      DiagNRCSubFunctionNotSupported();
#endif
   }
   else if ((subFctResult & kSubServiceSessionFailure) == kSubServiceSessionFailure)
   {
      /* For subfunctions send 0x7E */
      DiagNRCSubfunctionNotSupportedInActiveSession();
   }
   else if ((subFctResult & kSubServiceSecAccFailure) == kSubServiceSecAccFailure)
   {
      /* Security level not correct */
      DiagNRCSecurityAccessDenied();
   }
   else if ((subFctResult & kSubServiceTableMismatch) == kSubServiceTableMismatch)
   {
      /* Subfunction not supported. */
      DiagNRCRequestOutOfRange();
   }
   else if ((subFctResult & kSubServiceLenFailure) == kSubServiceLenFailure)
   {
      /* Length is incorrect. Set neg. response and exit */
      DiagNRCIncorrectMessageLengthOrInvalidFormat();
   }
   else
   {
      /* This point shouldn't be reached. If it is reached, check your configuration. */
      assertFblInternal(0u, kFblAssertInternalIlegalState);  /* PRQA S 2214 */ /* MD_FblDiag_2214 */
#if ( FBL_PRODUCTION == FBL_PROJECT_STATE )
      DiagNRCGeneralReject(); /* This should not be reached if configured correctly. */
#endif
   }
}  /* PRQA S 6080 */ /* MD_MSR_STMIF */

/***********************************************************************************************************************
 *  FblDiagProcessSubparamNrc
 **********************************************************************************************************************/
/*! \brief       This routine maps service check errors to NRCs
 *  \details     The checks done in CheckSubServiceTable() are mapped to NRCs in this function. This function is
 *               called for services with sub parameters.
 *  \pre         Service available (serviceInfo initialized)
 *  \param[in]   subFctResult Error map from CheckSubServiceTable()
 *  \param[in]   pServiceInfo Diagnostic service details
 **********************************************************************************************************************/
void FblDiagProcessSubparamNrc(V_MEMROM1 tServiceInfoTable V_MEMROM2 V_MEMROM3* pServiceInfo, vuint16 subFctResult)
{
#if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   /* Parameters not used: avoid compiler warning */
   (void)pServiceInfo; /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif

   /* Check for services with sub parameters  */
   /* Now processing the individual errors */
   if ((subFctResult & kSubServiceLenFailure) == kSubServiceLenFailure)
   {
      /* Length is incorrect. Set negative response and exit */
      DiagNRCIncorrectMessageLengthOrInvalidFormat();
   }
   else
   {
      /* Failure returned, but no error set. */
      /* Means, subfunction not supported.   */
#if defined( FBL_ENABLE_USERSUBFUNCTION )
      if ((subFctResult & (kSubServiceParameterNotFound | kSubServiceFunctionNotFound | kSubServiceTableMismatch)) !=0u)
      {
         /* Call user handler to manage its own subfunction */
         ApplDiagUserSubFunction(&DiagBuffer[kDiagFmtSubparam], DiagDataLength);

         /* Indicate, that a user callback was called */
         SetDiagHandlerCalled();
      }
      else
#endif
      {
         /* Not a subfunction */
         DiagNRCRequestOutOfRange();
      }
   }
}

/***********************************************************************************************************************
 *  FblDiagSetNrcFromPecResult
 **********************************************************************************************************************/
/*! \brief       Standard UDS sequence counter handling
 *  \return      tDiagSequCnt - information if transfer was in (in)correct order or repeated
 **********************************************************************************************************************/
static void FblDiagSetNrcFromPecResult( void )
{
   switch ( FblGetExtPecErr() )
   {
      case HdrErr_LengthExceeded: /* FblHdrTransferDataProcess */
      case HdrErr_ExtNrcTransferDataSuspended: /* FblHdrTransferDataProcess */
      {
         DiagNRCTransferDataSuspended();
         break;
      }
      case HdrErr_ExtNrcRequestOutOfRange: /* FblHdrTransferDataProcess */
      case HdrErr_ExtTransferTooSmall: /* FblHdrTransferDataProcess */
      case HdrErr_SwRegion: /* FblHdrTransferDataProcess */
      {
         DiagNRCRequestOutOfRange();
         break;
      }
      case HdrErr_ExtNrcRequestSequenceError:
      case HdrErr_MoreDataExpected: /* FblHdrTransferDataEnd */
      case HdrErr_ExtTransferDataEndMsgOutOfSequence: /* FblHdrTransferDataEnd */
      case HdrErr_ExtTransferDataMsgOutOfSequence: /* FblHdrTransferDataProcess */
      {
         DiagNRCRequestSequenceError();
         break;
      }
      case HdrErr_ExtUpdatePsiMsgOutOfSequence: /* FblHdrUpdatePSI */
      case HdrErr_ExtNrcConditionsNotCorrect: /* General error: FBL_MEM_STATUS_Failed */
      case HdrErr_ExtFlashWriteDriverNotReady: /* Currently not used */
      case HdrErr_ExtEraseDriverNotReady: /* Currently not used */
      case HdrErr_ExtRequestDownloadMsgOutOfSequence: /* FblHdrRequestDownload */
      case HdrErr_SwNotPresent: /* FblHdrEraseRoutine */
#if defined( FBL_ENABLE_SYSTEM_CHECK )
      case HdrErr_ExtValueOutOfRange: /* FblHdrTransferDataProcess */
#endif
#if defined( FBL_HDR_ENABLE_MULTIPLE_DOWNLOAD_SOURCES )
      case HdrErr_ExtDownloadOngoing: /* FblHdrRequestDownload */
#endif
      {
         DiagNRCConditionsNotCorrect();
         break;
      }
      case HdrErr_ExtMemoryNotErased: /* FblHdrRequestDownload */
      {
         DiagNRCUploadDownloadNotAccepted();
         break;
      }
      default:
      {
         DiagNRCGeneralProgrammingFailure();
         break;
      }
   } /* switch ( FblGetExtPecErr() ) */
}

/***********************************************************************************************************************
 *  FblDiagHandleSequenceCounter
 **********************************************************************************************************************/
/*! \brief       Standard UDS sequence counter handling
 *  \return      tDiagSequCnt - information if transfer was in (in)correct order or repeated
 **********************************************************************************************************************/
static tDiagSequCnt FblDiagHandleSequenceCounter( void )
{
   tDiagSequCnt retVal;

   /* Check if the requested sequence number is expected */
   if (DiagBuffer[kDiagFmtSubparam] != expectedSequenceCnt)
   {
      /* Check if sequence number corresponds to a retransmission of the last message */
      if (DiagBuffer[kDiagFmtSubparam] == currentSequenceCnt)
      {
         /* Repetition of last transferData request */
         DiagProcessingDone(kDiagRslTransferData);
         retVal = kDiagSequCntRepeated;
      }
      else /* Sequence number is not for a retry */
      {
         /* This is a wrong sequence */
         FblSetPecErr(HdrErr_ExtTransferDataSequenceCounter); /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblDiag_Debug_0310 */
         DiagNRCWrongBlockSequenceCounter();
         retVal = kDiagSequCntIncorrect;
      }
   }
   else /* Sequence number is equal to the one expected */
   {
      retVal = kDiagSequCntCorrect;
   }

   return retVal;
}

/***********************************************************************************************************************
 *  Oem specific Support functions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  FblDiagCheckSpecialLength
 **********************************************************************************************************************/
/*! \brief       This function performs a special length check for certain service requests
 *  \details     The checks done within this routine are used to perform non-constant length checks, e.g.
 *               minimum length checks for services without sub functions or length checks depending on
 *               parameters submitted with the diagnostic request.
 *  \pre         Service available (serviceInfo initialized)
 *  \param[in]   pbDiagData Diagnostic request
 *  \param[in]   diagReqDataLen Length of diagnostic request
 *  \param[in]   pServiceInfo Describes the received service
 *  \return      kFblOk: Request length correct, kFblFailed: Incorrect request length
 **********************************************************************************************************************/
/* PRQA S 3673 3 */ /* MD_FblDiag_3673 */
vuint8 FblDiagCheckSpecialLength(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen,
                                 V_MEMROM1 tServiceInfoTable V_MEMROM2 V_MEMROM3* pServiceInfo)
{
   vuint8 result = kFblOk;

#if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   /* Parameters not used: avoid compiler warning */
   (void)pbDiagData; /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif

   switch (pServiceInfo->serviceId)
   {
      case kDiagSidReadDataByIdentifier:
      {
         /* Check diagnostic request length for 2-byte alignment*/
         if ((diagReqDataLen & 0x01u) == 0x01u)
         {
            result = kFblFailed;
         }
         break;
      }
      case kDiagSidTransferData:
      {
         if (diagReqDataLen > (tTpDataType)(kDiagTransferDataBufferLen - 1u))
         {
            /* Requested transfer length is larger than indicated data length */
            result = kFblFailed;
         }
         break;
      }
      default:
         break;
   }

   return result;
}

#if defined( FBL_ENABLE_STAY_IN_BOOT )
/***********************************************************************************************************************
 *  FblDiagCheckStartMsg
 **********************************************************************************************************************/
/*! \brief       Checks and evaluates the contents of the start message
 *  \pre         Start message reception has to be verified by ComWrapper
 *  \param[in]   pData Pointer to received data
 *  \param[in]   length Length of received data
 *  \return      Check result. Return kFblOk if the start message has been received.
 **********************************************************************************************************************/
vuint8 FblDiagCheckStartMsg(const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 *pData, vuintx length)
{
   vuint8 result;
   vuintx index;

   /* Initialize variables */
   result = kFblOk;

   if ( kSizeOfStayInBootMsg != pData[0u] )
   {
      result = kFblFailed;
   }
   else
   {
      /* Check complete message contents for Stay-In-Boot */
      for (index = 0u; index < kSizeOfStayInBootMsg; index++)
      {
         /* Check complete message contents for Stay-In-Boot */
         if (fblDiagStayInBootMsg[index] != pData[index + 1u])
         {
            result = kFblFailed;
            break;
         }
      }
   }

   if (result == kFblOk)
   {
#if defined( FBL_DEF_ENABLE_NON_KB_MAIN )
      /* State handling now in main. */
#else
      fblMode |= FBL_START_WITH_PING;
#endif
   }

   return result;
}
#endif /* FBL_ENABLE_STAY_IN_BOOT */

/***********************************************************************************************************************
 *  FblDiagSessionControlParamInit
 **********************************************************************************************************************/
/*! \brief         Initialization of session control request response.
 *  \param[in,out] pbDiagData Pointer to data in the diagBuffer (without SID).
 *  \param[in]     diagReqDataLen Data length (without SID).
 **********************************************************************************************************************/
static void FblDiagSessionControlParamInit(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen)
{
#if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   /* Parameters not used: avoid compiler warning */
   (void)diagReqDataLen; /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif

   /* Reset internal states */
   ClrSecurityUnlock();
   ClrMemDriverInitialized();

   /* Prepare diagnostic response */
   pbDiagData[kDiagLocFmtSubparam + 1u] = GetDiagRsParamSessControl1();
   pbDiagData[kDiagLocFmtSubparam + 2u] = GetDiagRsParamSessControl2();
   pbDiagData[kDiagLocFmtSubparam + 3u] = GetDiagRsParamSessControl3();
   pbDiagData[kDiagLocFmtSubparam + 4u] = GetDiagRsParamSessControl4();
}

#if defined( FBL_ENABLE_DEBUG_STATUS )
/***********************************************************************************************************************
 *  FblErrDebug
 **********************************************************************************************************************/
/*! \brief       This function saves the extended error status.
 *  \details     The status may be retrieved with a Read-Data-By-Identifier (0x1A) service request.
 *  \param[in]   error Extended error status, see macros kDiagErr<condition>
 *  \param[in]   fileName pointer to string containing name of file the error was detected in
 *  \param[in]   lineNumber source line where error was detected at
 **********************************************************************************************************************/
void FblErrDebug( vuint16 error, tDebugFileName fileName, tDebugLineNbr lineNumber )
{
   /* Do not overwrite already set error */
   if (0u == errStatErrorCode)
   {
      FblErrStatSetError(error);
      errStatHaveDriver = 0u;
      errStatFileName = fileName;
      errStatLineNumber = lineNumber;
   }
}

/***********************************************************************************************************************
 *  FblErrDebugDriver
 **********************************************************************************************************************/
/*! \brief       This routine is called when an memory-device related error is detected.
 *               The routine saves that error-code and address that the error occurred at.
 *  \details     The address may be retrieved with a Read-Data-By-Identifier (0x1A) service request.
 *  \param[in]   addr Address that an memory error occurred at
 *  \param[in]   error Error code returned from device-driver
 **********************************************************************************************************************/
void FblErrDebugDriver( tFblAddress addr, vuint16 error )
{
   FblErrStatSetFlashDrvError(error);
   errStatAddress=(addr);
   errStatHaveDriver = 1u;
}
#endif /* FBL_ENABLE_DEBUG_STATUS */

/***********************************************************************************************************************
 *  Diagnostic main handler service functions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  FblDiagDefaultMainHandler
 **********************************************************************************************************************/
/*! \brief         Default main handler.
 *  \details       This default main handler is called if no service main handler could be found and calls a user
 *                 callback function. Normally, a service specific main handler should be called.
 *  \param[in,out] pbDiagData Pointer to the data in the diagBuffer (without SID).
 *  \param[in]     diagReqDataLen Length of data (without SID).
 *  \return        Result - Always kFblOk.
 **********************************************************************************************************************/
/* PRQA S 3673 1 */ /* MD_FblDiag_3673 */
tFblResult FblDiagDefaultMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen)
{
   ApplDiagUserService(pbDiagData, diagReqDataLen);
   return kFblOk;
}

/***********************************************************************************************************************
 *  FblDiagDummyMainHandler
 **********************************************************************************************************************/
/*! \brief         Dummy main handler.
 *  \details       This dummy main handler is called if no service main handler could be found. Normally,
 *                 a service specific main handler should be called.
 *  \param[in,out] pbDiagData Pointer to the data in the diagBuffer (without SID).
 *  \param[in]     diagReqDataLen Length of data (without SID).
 *  \return        Result - Always kFblOk.
 **********************************************************************************************************************/
/* PRQA S 3673 1 */ /* MD_FblDiag_3673 */
static tFblResult FblDiagDummyMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen)
{
#if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   /* Parameters not used: avoid compiler warning */
   (void)diagReqDataLen; /* PRQA S 3112 */ /* MD_MSR_14.2 */
   (void)pbDiagData; /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif

   /* This function should not be called.  */
   assertFblInternal(0u, kFblAssertInternalIlegalState);  /* PRQA S 2214 */ /* MD_FblDiag_2214 */

   return kFblOk;
}

/***********************************************************************************************************************
 *  FblDiagDefaultSessionMainHandler
 **********************************************************************************************************************/
/*! \brief         Default session request service handler.
 *  \param[in,out] pbDiagData Pointer to data in diagBuffer (without SID).
 *  \param[in]     diagReqDataLen Data length (without SID).
 *  \return        Result: Always kFblOk.
 **********************************************************************************************************************/
static tFblResult FblDiagDefaultSessionMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen)
{
   /* PRQA S 2214 2 */ /* MD_FblDiag_2214 */
   assertFblInternal(diagServiceCurrent == kDiagSidDiagnosticSessionControl, kFblAssertWrongServiceHandler);
   assertFblInternal(pbDiagData[kDiagLocFmtSubparam ] == kDiagSubDefaultSession, kFblAssertWrongServiceHandler);

   {
      /* Request bootloader reset */
      SetWaitEcuReset(); /* PRQA S 3109 */ /* MD_CBD_14.3 */

      /* Prepare response and reset states */
      FblDiagSessionControlParamInit(pbDiagData, diagReqDataLen);
      /* Send response */
      DiagProcessingDone(kDiagRslDiagnosticSessionControl);
      SetEcuResetFctFinished();
   }
   return kFblOk;
}

/***********************************************************************************************************************
 *  FblDiagExtendedSessionMainHandler
 **********************************************************************************************************************/
/*! \brief         Extended session request service handler.
 *  \param[in,out] pbDiagData Pointer to data in diagBuffer (without SID).
 *  \param[in]     diagReqDataLen Data length (without SID).
 *  \return        Result: Always kFblOk.
 **********************************************************************************************************************/
static tFblResult FblDiagExtendedSessionMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen)
{
   /* PRQA S 2214 2 */ /* MD_FblDiag_2214 */
   assertFblInternal(diagServiceCurrent == kDiagSidDiagnosticSessionControl, kFblAssertWrongServiceHandler);
   assertFblInternal(pbDiagData[kDiagLocFmtSubparam] == kDiagSubExtendedDiagSession, kFblAssertWrongServiceHandler);

   /* Change to extended session */
   SetDiagExtendedSession(); /* PRQA S 3109 */ /* MD_CBD_14.3 */

   /* Prepare response and reset states */
   FblDiagSessionControlParamInit(pbDiagData, diagReqDataLen);
   /* Send response */
   DiagProcessingDone(kDiagRslDiagnosticSessionControl);

   return kFblOk;
}

/***********************************************************************************************************************
 *  FblDiagProgrammingSessionMainHandler
 **********************************************************************************************************************/
/*! \brief         Programming session request service handler.
 *  \param[in,out] pbDiagData Pointer to data in diagBuffer (without SID).
 *  \param[in]     diagReqDataLen Data length (without SID).
 *  \return        Result: kFblOk if reprogramming conditions are fulfilled, kFblFailed if not.
 **********************************************************************************************************************/
static tFblResult FblDiagProgrammingSessionMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen)
{
   tFblResult result = kFblOk;

   /* PRQA S 2214 2 */ /* MD_FblDiag_2214 */
   assertFblInternal( diagServiceCurrent==kDiagSidDiagnosticSessionControl, kFblAssertWrongServiceHandler );
   assertFblInternal( pbDiagData[kDiagLocFmtSubparam]==kDiagSubProgrammingSession, kFblAssertWrongServiceHandler );

   /* Check of programming preconditions done? */
   if( kFblOk != ApplFblCheckProgConditions() )
   {
      /* Check of programming preconditions failed */
      FblErrDebugStatus(kDiagErrProgConditionsWrong); /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblDiag_Debug_0310 */
      DiagNRCConditionsNotCorrect();
      result = kFblFailed;
   }
   else
   {
      if (FblMainGetStartFromAppl())
      {
         /* Call Header Module Init late in case of start from application to allow for a response pending
          * message during initialization parsing of the SBA ticket.*/
         FblHdrInit(HDR_INTERFACE_TYPE_ALL_EXPLICIT);

         /* FblHdrInit() destroys all diagData elements behinds preamble, restore required elements. */
         pbDiagData[kDiagLocFmtSubparam] = kDiagSubProgrammingSession;
      }

      /* Check of programming preconditions succeeded - switch to programming session */
      SetDiagProgrammingSession(); /* PRQA S 3109 */ /* MD_CBD_14.3 */
      /* Clear when entering Programming Session */
      FblClrPecErr(); /* PRQA S 3109 */ /* MD_CBD_14.3 */
      /* Prepare response and reset states */
      FblDiagSessionControlParamInit(pbDiagData, diagReqDataLen);

#if defined( FBL_USE_FBL_AS_DEMOAPPL )
      /* Attention: Intended for Vector Demo Application project only!
       * Typically provide demonstration through fbl_jmpToBoot.c (prototype through fbl_jmpToBoot.h to include in fbl_inc.h) */
      JmpToBoot_OnProgSession();
#endif /* FBL_USE_FBL_AS_DEMOAPPL */

   }

   DiagProcessingDone(kDiagRslDiagnosticSessionControl);
   return result;
}

/***********************************************************************************************************************
 *  FblDiagSecAccessSeedMainHandler
 **********************************************************************************************************************/
/*! \brief         Request Seed request main handler.
 *  \param[in,out] pbDiagData Pointer to the data in the diagBuffer (without SID)
 *  \param[in]     diagReqDataLen Length of data (without SID)
 *  \return        kFblOk: service processed successfully (goto next state), kFblFailed: Service processing failed.
 **********************************************************************************************************************/
static tFblResult FblDiagSecAccessSeedMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen)
{
   tFblResult result = kFblOk;
   vuintx i;

#if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   /* Parameters not used: avoid compiler warning */
   (void)diagReqDataLen; /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif

   /* PRQA S 2214 2 */ /* MD_FblDiag_2214 */
   assertFblInternal( diagServiceCurrent==kDiagSidSecurityAccess,  kFblAssertWrongServiceHandler );
   assertFblInternal( ((pbDiagData[kDiagLocFmtSubparam ] & 0x01u) == 0x01u),     kFblAssertWrongServiceHandler );

   /* ECU to be unlocked upon request in boot */
   for(i = 0u; i < kSecSeedLength; i++)
   {
      pbDiagData[kDiagLocFmtSeedKeyStart + i] = 0x00u;
   }

   SetSecurityUnlock();

   DiagProcessingDone(kDiagRslSecurityAccessSeed);
   return result;
}

/***********************************************************************************************************************
 *  FblDiagCommCtrlMainHandler
 **********************************************************************************************************************/
/*! \brief         Communication Control request main handler.
 *  \param[in,out] pbDiagData Pointer to the data in the diagBuffer (without SID)
 *  \param[in]     diagReqDataLen Length of data (without SID)
 *  \return        kFblOk: service processed successfully (goto next state), kFblFailed: Service processing failed.
 **********************************************************************************************************************/
/* PRQA S 3673 1 */ /* MD_FblDiag_3673 */
static tFblResult FblDiagCommCtrlMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen)
{
   tFblResult result;

#if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   /* Parameters not used: avoid compiler warning */
   (void)diagReqDataLen; /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif

   /* PRQA S 2214 1 */ /* MD_FblDiag_2214 */
   assertFblInternal(diagServiceCurrent == kDiagSidCommunicationControl, kFblAssertWrongServiceHandler);

   if ((pbDiagData[kDiagLocFmtSubparam + 0x01u] & kDiagSubRequestAllowedMask) != kDiagSubRequestAllowedMask)
   {
      FblErrDebugStatus(kDiagErrCommCtrlParam); /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblDiag_Debug_0310 */
      DiagNRCRequestOutOfRange();
      result = kFblFailed;
   }
   else
   {
      SetDisableNormalCom();
      /* Simply transmit a positive response message with subfunction parameter */
      DiagProcessingDone(kDiagRslCommunicationControl);
      result = kFblOk;
   }

   return result;
}

/***********************************************************************************************************************
 *  FblDiagRCEraseMemoryMainHandler
 **********************************************************************************************************************/
/*! \brief         Routine Control - Erase Memory.
 *  \details       This is where the "programming sequence" starts.
 *                 Retries upon error are performed on Erase Memory routine.
 *  \pre           Memory driver initialized.
 *  \param[in,out] pbDiagData Pointer to the data in the diagBuffer (without SID)
 *  \param[in]     diagReqDataLen Length of data (without SID)
 *  \return        kFblOk: service processed successfully (goto next state), kFblFailed: Service processing failed.
 **********************************************************************************************************************/
static tFblResult FblDiagRCEraseMemoryMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen)
{
   tFblResult result = kFblFailed;
   tFlashErrorCode flashErrorCode;
   vuint8 partId;

#if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   /* Parameters not used: avoid compiler warning */
   (void)diagReqDataLen; /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif

   /* PRQA S 2214 4 */ /* MD_FblDiag_2214 */
   assertFblInternal(diagServiceCurrent==kDiagSidRoutineControl, kFblAssertWrongServiceHandler);
   assertFblInternal(pbDiagData[kDiagLocFmtSubfunction]==kDiagSubStartRoutine, kFblAssertWrongServiceHandler);
   assertFblInternal(pbDiagData[kDiagLocFmtRoutineIdHigh]==kDiagRoutineIdEraseMemoryHigh, kFblAssertWrongServiceHandler);
   assertFblInternal(pbDiagData[kDiagLocFmtRoutineIdLow]==kDiagRoutineIdEraseMemoryLow, kFblAssertWrongServiceHandler);

   /* Send response pending for flash driver initialization */
   DiagExRCRResponsePending(kForceSendRpIfNotInProgress);

   /* Turn on programming voltage (if necessary) */
   ApplFblSetVfp();

   /* Copy memory driver code into RAM buffer */
   FblCwSetOfflineMode();
   flashErrorCode = MemDriver_InitSync(V_NULL);
   FblCwSetOnlineMode();

   if (flashErrorCode != kFlashOk)
   {
      FblErrDebugStatus(kDiagErrFlashcodeInitFailed); /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblDiag_Debug_0310 */
      FblSetPecErr(HdrErr_Undefined); /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblDiag_Debug_0310 */
      DiagNRCConditionsNotCorrect();
   }
   else
   {
      /* Set Memory driver initialization flag */
      SetMemDriverInitialized();
      /* Prepare LibMem for new Programming sequence; Restore bytes behind preamble after DiagMemInit */

       partId = pbDiagData[kDiagLocFmtRoutinePartInfo];

      (void) FblMemInit();
      pbDiagData = FblDiagMemGetActiveBuffer();
      pbDiagData[kDiagLocFmtRoutineIdHigh] = 0xFFu;
      pbDiagData[kDiagLocFmtRoutineIdLow] = 0x00u;
      pbDiagData[kDiagLocFmtRoutinePartInfo] = partId;

      result = FblHdrEraseRoutine(partId, HDR_DOWNLOAD_SOURCE_DIAG);
      if (kFblOk != result)
      {
         FblDiagSetNrcFromPecResult();
      }
   }

   DiagProcessingDone(kDiagRslRoutineControlEraseMemory);
   return result;
}  /* PRQA S 6010, 6030, 6050 */ /* MD_FblDiag_6010, MD_FblDiag_6030, MD_FblDiag_6050 */

/***********************************************************************************************************************
 *  FblDiagRCUpdatePSIMainHandler
 **********************************************************************************************************************/
/*! \brief         Routine Control - Update PSI.
 *  \param[in,out] pbDiagData Pointer to the data in the diagBuffer (without SID)
 *  \param[in]     diagReqDataLen Length of data (without SID)
 *  \return        kFblOk: service processed successfully (goto next state), kFblFailed: Service processing failed.
 **********************************************************************************************************************/
static tFblResult FblDiagRCUpdatePSIMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen)  /* PRQA S 3673 */ /* MD_FblDiag_3673 */
{
   tFblResult retVal;
   vuint8 locPartId = pbDiagData[kDiagLocFmtRoutinePartInfo];

#if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   /* Parameters not used: avoid compiler warning */
   (void)diagReqDataLen; /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif

#if defined( FBL_HDR_ENABLE_EXPLICIT_UPDATEPSI )
   retVal = FblHdrUpdatePSI(locPartId);
   if (kFblOk != retVal)
   {
      FblDiagSetNrcFromPecResult();
   }
#else
   if (   (!GetTransferDataSucceeded())
       || (FblHdrGetParsedModPartId() != locPartId) )
   {
      FblSetPecErr(HdrErr_ExtUpdatePsiMsgOutOfSequence); /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblDiag_Debug_0310 */
      DiagNRCConditionsNotCorrect();
      retVal = kFblFailed;
   }
   /* do not set the PSI but return if the PSI was set to support GM Global B and PATAC SLP2 procedure*/
   else if (PSI_PART_PRESENT != ApplFblChkPSIState(locPartId))
   {
      retVal = kFblFailed;
   }
   else
   {
      retVal = kFblOk;
   }

   /* Always clear upon PSI Update (failed/Ok) */
   ClrTransferDataSucceeded();
#endif

   DiagProcessingDone(kDiagRslRoutineControlUpdatePSI);
   return retVal;
}  /* PRQA S 6010, 6030, 6050 */ /* MD_FblDiag_6010, MD_FblDiag_6030, MD_FblDiag_6050 */

#  if defined( FBL_DIAG_ENABLE_GM_ROUTINE_CHECKPROGDEP )
/***********************************************************************************************************************
 *  FblDiagRCCheckProgDepMainHandler
 **********************************************************************************************************************/
/*! \brief         Routine Control - Check Programming Dependencies.
 *  \param[in,out] pbDiagData Pointer to the data in the diagBuffer (without SID)
 *  \param[in]     diagReqDataLen Length of data (without SID)
 *  \return        kFblOk: service processed successfully, kFblFailed: Service processing failed.
 **********************************************************************************************************************/
static tFblResult FblDiagRCCheckProgDepMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen)  /* PRQA S 3673 */ /* MD_FblDiag_3673 */
{
   tFblResult retVal;

#if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   /* Parameters not used: avoid compiler warning */
   (void)diagReqDataLen; /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif

   /* PRQA S 2214 4 */ /* MD_FblDiag_2214 */
   assertFblInternal(diagServiceCurrent==kDiagSidRoutineControl, kFblAssertWrongServiceHandler);
   assertFblInternal(pbDiagData[kDiagLocFmtSubfunction]==kDiagSubStartRoutine, kFblAssertWrongServiceHandler);
   assertFblInternal(pbDiagData[kDiagLocFmtRoutineIdHigh]==kDiagRoutineIdCheckProgDepHigh, kFblAssertWrongServiceHandler);
   assertFblInternal(pbDiagData[kDiagLocFmtRoutineIdLow]==kDiagRoutineIdCheckProgDepLow, kFblAssertWrongServiceHandler);

   /* Do nothing. Just accept this service. It is executed in application, the service has to be positively responded in Fbl. */

   /* callback below need to be introduced if unavailable and response bytes need to be defined.
    * Prototype need to be "tFblResult ApplFblCheckProgDependencies(V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * responseBytes);"
    */
   retVal = ApplFblCheckProgDependencies( &pbDiagData[kDiagLocFmtRoutineCheckProgDepStatus] );

   DiagProcessingDone(kDiagRslRoutineControlCheckProgDep);
   return retVal;
}
#  endif /* FBL_DIAG_ENABLE_GM_ROUTINE_CHECKPROGDEP */

#  if defined( FBL_DIAG_ENABLE_GM_ROUTINE_SETPROGSTATEFLAG )
/***********************************************************************************************************************
 *  FblDiagRCSetProgStateFlagMainHandler
 **********************************************************************************************************************/
/*! \brief         Routine Control - Set Programming State Flag
 *  \detail        This function will not do anything in Fbl. Make sure that reception from tester is accepted by Fbl.
 *                 The function is expected to be required in case the Fbl runs on a gateway.
 *  \param[in,out] pbDiagData Pointer to the data in the diagBuffer (without SID)
 *  \param[in]     diagReqDataLen Length of data (without SID)
 *  \return        kFblOk: service processed successfully, kFblFailed: Service processing failed.
 **********************************************************************************************************************/
static tFblResult FblDiagRCSetProgStateFlagMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen)  /* PRQA S 3673 */ /* MD_FblDiag_3673 */
{
    tFblResult retVal;
#if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   /* Parameters not used: avoid compiler warning */
   (void)diagReqDataLen; /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif

   /* PRQA S 2214 4 */ /* MD_FblDiag_2214 */
   assertFblInternal(diagServiceCurrent==kDiagSidRoutineControl, kFblAssertWrongServiceHandler);
   assertFblInternal(pbDiagData[kDiagLocFmtSubfunction]==kDiagSubStartRoutine, kFblAssertWrongServiceHandler);
   assertFblInternal(pbDiagData[kDiagLocFmtRoutineIdHigh]==kDiagRoutineIdSetProgStateFlagHigh, kFblAssertWrongServiceHandler);
   assertFblInternal(pbDiagData[kDiagLocFmtRoutineIdLow]==kDiagRoutineIdSetProgStateFlagLow, kFblAssertWrongServiceHandler);

   /* Do nothing. Just accept this service. It is executed in application, the service  has to be positively responded in Fbl. */

   /* Unclear if status record is used; Default: unused; change kDiagRslRoutineControlSetProgStateFlag to value different kDiagRslRoutineControl
    * in order to allow to set status bytes from external callback. To allow this also define ApplFblProgStateFlag to function callback
    */
#if defined( ApplFblProgStateFlag )
   retVal = ApplFblProgStateFlag( &pbDiagData[kDiagLocFmtRoutineSetProgStateFlagStatus] );
#else
   retVal = kFblOk;
#endif /* ApplFblProgStateFlag */

   DiagProcessingDone(kDiagRslRoutineControlSetProgStateFlag);
   return retVal;
}
#  endif /* FBL_DIAG_ENABLE_GM_ROUTINE_SETPROGSTATEFLAG */

# if defined( FBL_DIAG_ENABLE_GM_ROUTINE_INITEXTREFLASH )
/***********************************************************************************************************************
 *  FblDiagRCInitExtReflashMainHandler
 **********************************************************************************************************************/
/*! \brief         Routine Control - Diag Init Extended Reflash
 *  \param[in,out] pbDiagData Pointer to the data in the diagBuffer (without SID)
 *  \param[in]     diagReqDataLen Length of data (without SID)
 *  \return        kFblOk: service processed successfully, kFblFailed: Service processing failed.
 **********************************************************************************************************************/
static tFblResult FblDiagRCInitExtReflashMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen)  /* PRQA S 3673 */ /* MD_FblDiag_3673 */
{
    tFblResult retVal;
#if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   /* Parameters not used: avoid compiler warning */
   (void)diagReqDataLen; /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif

   /* PRQA S 2214 4 */ /* MD_FblDiag_2214 */
   assertFblInternal(diagServiceCurrent==kDiagSidRoutineControl, kFblAssertWrongServiceHandler);
   assertFblInternal(pbDiagData[kDiagLocFmtSubfunction]==kDiagSubStartRoutine, kFblAssertWrongServiceHandler);
   assertFblInternal(pbDiagData[kDiagLocFmtRoutineIdHigh]==kDiagRoutineIdInitExtReflashHigh, kFblAssertWrongServiceHandler);
   assertFblInternal(pbDiagData[kDiagLocFmtRoutineIdLow]==kDiagRoutineIdInitExtReflashLow, kFblAssertWrongServiceHandler);

   /* Do nothing. Just accept this service with status success. It is executed in application, the service  has to be positively responded in Fbl. */
   pbDiagData[kDiagLocFmtRoutineInitExtReflash] = 0x00u;
#if defined( ApplFblInitExtReflash )
   retVal = ApplFblInitExtReflash( &pbDiagData[kDiagLocFmtRoutineInitExtReflash] );
#else
   retVal = kFblOk;
#endif /* ApplFblProgStateFlag */

   DiagProcessingDone(kDiagRslRoutineControlInitExtReflash);
   return retVal;
}
# endif /* FBL_DIAG_ENABLE_GM_ROUTINE_INITEXTREFLASH */

#if defined( FBL_DIAG_STAY_IN_BOOT_ROUTINECONTROL )
/***********************************************************************************************************************
 *  FblDiagRCStartForceBootModeMainHandler
 **********************************************************************************************************************/
/*! \brief         Check for Force Boot Mode message
 *  \param[in,out] pbDiagData Pointer to the data in the diagBuffer (without SID)
 *  \param[in]     diagReqDataLen Length of data (without SID)
 *  \return        kFblOk: service processed successfully (goto next state), kFblFailed: Service processing failed.
 **********************************************************************************************************************/
/* PRQA S 3673 1 */ /* MD_FblDiag_3673 */
static tFblResult FblDiagRCStartForceBootModeMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen)
{
   /* PRQA S 2214 4 */ /* MD_FblDiag_2214 */
   assertFblGen(diagServiceCurrent == kDiagSidRoutineControl, kFblAssertWrongServiceHandler);
   assertFblGen(pbDiagData[kDiagLocFmtSubfunction] == kDiagSubStartRoutine, kFblAssertWrongServiceHandler);
   assertFblGen(pbDiagData[kDiagLocFmtSubfunction + 1u] == kDiagRoutineIdStayInBootHigh, kFblAssertWrongServiceHandler);
   assertFblGen(pbDiagData[kDiagLocFmtSubfunction + 2u] == kDiagRoutineIdStayInBootLow, kFblAssertWrongServiceHandler);

#if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   /* Parameters not used: avoid compiler warning */
   (void)pbDiagData; /* PRQA S 3112 */ /* MD_MSR_14.2 */
   (void)diagReqDataLen; /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif

   /* Nothing to do here - just send positive response */
   DiagProcessingDone(kDiagRslRoutineControlStayInBoot);

   return kFblOk;
}
#endif /* FBL_DIAG_STAY_IN_BOOT_ROUTINECONTROL */

/***********************************************************************************************************************
 *  FblDiagRequestDownloadMainHandler
 **********************************************************************************************************************/
/*! \brief         Request download service function
 *  \param[in,out] pbDiagData Pointer to the data in the diagBuffer (without SID)
 *  \param[in]     diagReqDataLen Length of data (without SID)
 *  \return        kFblOk: service processed successfully (goto next state), kFblFailed: Service processing failed.
 **********************************************************************************************************************/
/* PRQA S 3673 1 */ /* MD_FblDiag_3673 */
static tFblResult FblDiagRequestDownloadMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen)
{
   tFblResult result = kFblFailed;

#if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   /* parameters not used, satisfies lint */
   (void)diagReqDataLen; /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif

   /* Only one addressAndLengthFormatIdentifier is supported */
   if (pbDiagData[kDiagLocFmtFormatOffset] != kDiagRequestDownloadALFI)
   {
      /* ALFI is invalid, send ROOR according to ISO-14229-1.2 */
      FblErrDebugStatus(kDiagErrAlfi); /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblDiag_Debug_0310 */
      FblSetPecErr(HdrErr_Undefined); /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblDiag_Debug_0310 */
      DiagNRCRequestOutOfRange();
   }
   else
   {
      /* Init expected sequence counter for TransferData */
      expectedSequenceCnt = kDiagInitSequenceNum;
      /* Init current sequence counter for TransferData */
      currentSequenceCnt = kDiagInitSequenceNum;

      result = FblHdrRequestDownload(HDR_DOWNLOAD_SOURCE_DIAG);
      if( kFblOk != result )
      {
         FblDiagSetNrcFromPecResult();
      }

      /* Reset FblLib_Mem states on RequestDownload to allow Libmem handling on newly received module in any case. */
      (void) FblMemInit();
      pbDiagData = FblDiagMemGetActiveBuffer();

      /* Prepare response: Transmit 2byte Block Length either aligned or unaligned */
      pbDiagData[kDiagLocFmtSubparam] = (kDiagRslRequDldMaxBlockLenParameter << 4u);
      pbDiagData[kDiagLocFmtSubparam + 1u] = GetDiagRsParamRequDlMaxBlockLen1();
      pbDiagData[kDiagLocFmtSubparam + 2u] = GetDiagRsParamRequDlMaxBlockLen2();
   }

   DiagProcessingDone(kDiagRslRequestDownload);
   return result;
}  /* PRQA S 2006, 6010, 6030, 6050 */ /* MD_MSR_14.7, MD_FblDiag_6010, MD_FblDiag_6030, MD_FblDiag_6050 */

/***********************************************************************************************************************
 *  FblDiagTransferDataMainHandler
 **********************************************************************************************************************/
/*! \brief         TransferData service function;
 *  \pre           TransferData must be enabled by RequestDownload service
 *  \param[in,out] pbDiagData Pointer to the data in the diagBuffer (without SID)
 *  \param[in]     diagReqDataLen Length of data (without SID)
 *  \return        kFblOk: service processed successfully (goto next state), kFblFailed: Service processing failed.
 **********************************************************************************************************************/
/* PRQA S 3673 1 */ /* MD_FblDiag_3673 */
static tFblResult FblDiagTransferDataMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen)
{
   tFblResult result = kFblFailed;
   tTpDataType transferPayloadSize; /**< payload in transferData */
   tDiagSequCnt sequCnt;

   /* PRQA S 2214 1 */ /* MD_FblDiag_2214 */
   assertFblInternal(diagServiceCurrent==kDiagSidTransferData,  kFblAssertWrongServiceHandler);

   /* In order to allow for intended NRC order, check partition download status early. */
   if (FblHdrStateIsTransferEndExpected())
   {
      FblSetPecErr(HdrErr_LengthExceeded); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblDiag_Debug_0310 */
      DiagNRCRequestSequenceError();
   }
   else if (!FblHdrStateIsTransferExpected())
   {
      FblSetPecErr(HdrErr_ExtTransferDataMsgOutOfSequence); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblDiag_Debug_0310 */
      DiagNRCRequestSequenceError();
   }
   else
   {
      sequCnt = FblDiagHandleSequenceCounter();
      if (kDiagSequCntIncorrect == sequCnt)
      {
         /* Do nothing. All states are already set inside FblDiagHandleSequenceCounter(); */
      }
      else if (kDiagSequCntRepeated == sequCnt)
      {
          result = kFblOk;
      }
      else
      {
         /* Minimum length check guarantees diagReqDataLen > kDiagLocFmtDataOffset */
         assertFblInternal(diagReqDataLen >= kDiagLocFmtDataOffset, kFblAssertInternalIlegalState); /* PRQA S 2214 */ /* MD_FblDiag_2214 */

         transferPayloadSize = diagReqDataLen - kDiagLocFmtDataOffset; /* PRQA S 3382, 3297 */ /* MD_FblDiag_3382, MD_FblDiag_3297 */

         /* Put RCRRP message in CAN transmit buffer so that FblCanRetransmit() can be called from WD during device-driver
          * erase/write operations; Libmem during Pipelined Verification action may already have sent one. */
         if (!GetRcrRpInProgress())
         {
            DiagExRCRResponsePending(kForceSendResponsePending);
         }

         result = FblHdrTransferDataProcess(&pbDiagData[kDiagLocFmtDataOffset], transferPayloadSize);
         if (kFblOk != result)
         {
            FblDiagSetNrcFromPecResult();
         }
         else
         {
            currentSequenceCnt = expectedSequenceCnt; /* Memorize current counter */
            expectedSequenceCnt++;  /* Sequence counter value of next transferData request */
         }
      }
   }

   (void) FblDiagMemGetActiveBuffer();
   DiagProcessingDone(kDiagRslTransferData);
   return result;
}  /* PRQA S 6050 */ /* MD_MSR_STCAL */ /* PRQA S 6080 */ /* MD_MSR_STMIF */ /* PRQA S 2006 */ /* MD_MSR_14.7 */

/***********************************************************************************************************************
 *  FblDiagReqTransferExitMainHandler
 **********************************************************************************************************************/
/*! \brief         RequestTransferExit service function
 *  \param[in,out] pbDiagData Pointer to the data in the diagBuffer (without SID)
 *  \param[in]     diagReqDataLen Length of data (without SID)
 *  \return        kFblOk: service processed successfully (goto next state), kFblFailed: Service processing failed.
 **********************************************************************************************************************/
/* PRQA S 3673 1 */ /* MD_FblDiag_3673 */
static tFblResult FblDiagReqTransferExitMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen)
{
   tFblResult retVal;
   /* PRQA S 2214 1 */ /* MD_FblDiag_2214 */
   assertFblInternal(diagServiceCurrent==kDiagSidRequestTransferExit,  kFblAssertWrongServiceHandler );

#if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   /* parameters not used, satisfies lint */
   (void)pbDiagData; /* PRQA S 3112 */ /* MD_MSR_14.2 */
   (void)diagReqDataLen; /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif

   retVal = FblHdrTransferDataEnd();
   if (kFblOk != retVal)
   {
      FblDiagSetNrcFromPecResult();
   }

   DiagProcessingDone(kDiagRslRequestTransferExit);
   return retVal;
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */

/***********************************************************************************************************************
 *  FblDiagTesterPresentMainHandler
 **********************************************************************************************************************/
/*! \brief         FblTesterPresent service function.
 *  \param[in,out] pbDiagData Pointer to the data in the diagBuffer (without SID)
 *  \param[in]     diagReqDataLen Length of data (without SID)
 *  \return        kFblOk: service processed successfully (goto next state), kFblFailed: Service processing failed.
 **********************************************************************************************************************/
/* PRQA S 3673 1 */ /* MD_FblDiag_3673 */
static tFblResult FblDiagTesterPresentMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen)
{
   /* PRQA S 2214 2 */ /* MD_FblDiag_2214 */
   assertFblInternal(diagServiceCurrent == kDiagSidTesterPresent, kFblAssertWrongServiceHandler);
   assertFblInternal(pbDiagData[kDiagLocFmtSubparam] == kDiagSubTesterPresent, kFblAssertWrongServiceHandler);

#if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   /* Parameters not used: avoid compiler warning */
   (void)pbDiagData; /* PRQA S 3112 */ /* MD_MSR_14.2 */
   (void)diagReqDataLen; /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif

   DiagProcessingDone(kDiagRslTesterPresent);

   return kFblOk;
}

/***********************************************************************************************************************
 *  FblDiagControlDTCMainHandler
 **********************************************************************************************************************/
/*! \brief         ControlDTCSetting service function.
 *  \param[in,out] pbDiagData Pointer to the data in the diagBuffer (without SID)
 *  \param[in]     diagReqDataLen Length of data (without SID)
 *  \return        kFblOk: service processed successfully (goto next state), kFblFailed: Service processing failed.
 **********************************************************************************************************************/
/* PRQA S 3673 1 */ /* MD_FblDiag_3673 */
static tFblResult FblDiagControlDTCMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen)
{
#if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   /* parameters not used, satisfies lint */
   (void)pbDiagData; /* PRQA S 3112 */ /* MD_MSR_14.2 */
   (void)diagReqDataLen; /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif

   /* PRQA S 2214 1 */ /* MD_FblDiag_2214 */
   assertFblInternal(diagServiceCurrent==kDiagSidControlDTCSetting, kFblAssertWrongServiceHandler);

   DiagProcessingDone(kDiagRslControlDTCSetting);
   return kFblOk;
}

/***********************************************************************************************************************
 *  FblDiagReadDataByIdMainHandler
 **********************************************************************************************************************/
/*! \brief         ReadDataByIdentifier service function
 *  \param[in,out] pbDiagData Pointer to the data in the diagBuffer (without SID)
 *  \param[in]     diagReqDataLen Length of data (without SID)
 *  \return        kFblOk: service processed successfully (goto next state), kFblFailed: Service processing failed.
 *********************************************************************************************************************/
static tFblResult FblDiagReadDataByIdMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen)
{
#if defined( FBL_ENABLE_DEBUG_STATUS )
   vuint16 diagDid;
#endif

   /* PRQA S 2214 1 */ /* MD_FblDiag_2214 */
   assertFblInternal( diagServiceCurrent==kDiagSidReadDataByIdentifier, kFblAssertWrongServiceHandler );

#if defined( FBL_ENABLE_DEBUG_STATUS )
   diagDid = (vuint16)pbDiagData[0u];
   diagDid = (vuint16)((diagDid << 8u) | (vuint16)(pbDiagData[1u]));

   if (kDiagDidReadLastError == diagDid)
   {
      tTpDataType i, j;

      pbDiagData[kDiagLocFmtDidResp + 0u] = errStatLastServiceId;
      pbDiagData[kDiagLocFmtDidResp + 1u] = (vuint8)(errStatErrorCode >>  8u);
      pbDiagData[kDiagLocFmtDidResp + 2u] = (vuint8)(errStatErrorCode);
      if (0u != errStatHaveDriver)
      {
         pbDiagData[kDiagLocFmtDidResp + 3u] = (vuint8)(errStatAddress >> 16u);
         pbDiagData[kDiagLocFmtDidResp + 4u] = (vuint8)(errStatAddress >>  8u);
         pbDiagData[kDiagLocFmtDidResp + 5u] = (vuint8)(errStatAddress);

         i = (tTpDataType) (kDiagLocFmtDidResp + 6u);
      }
      else if (kDiagErrTPFailed == errStatErrorCode)
      {
         pbDiagData[kDiagLocFmtDidResp + 3u] = errStatTpError;
         i = (tTpDataType) (kDiagLocFmtDidResp + 4u);
      }
      else
      {
         i = (tTpDataType) (kDiagLocFmtDidResp + 3u);
      }

      if (errStatFileName != 0u)
      {
         for (j = (tTpDataType)0u; (errStatFileName[j] != (vuint8)0u) && (i < (tTpDataType)FBL_DIAG_BUFFER_LENGTH); j++)
         {
            (void)FblRealTimeSupport();

            pbDiagData[i] = errStatFileName[j];
            i++;
         }

         pbDiagData[i] = (vuint8)0u;
         i++;

         for (j = (tTpDataType)sizeof(vuint16); (j > (tTpDataType)0u) && (i < (tTpDataType)FBL_DIAG_BUFFER_LENGTH); j--)
         {
            pbDiagData[i] = (vuint8)((vuint16)errStatLineNumber >> ( (j - 1u) << 3) );
            i++;
         }
      }

      DiagProcessingDone( (kDiagRslReadDataByIdentifier + i) - (vuint8)2u);
      ApplFblInitErrStatus();
      return kFblOk;
   }
#endif

   ApplFblReadDataByIdentifier(pbDiagData, diagReqDataLen);
   return kFblOk;
}

#ifdef FBL_ENABLE_SERVICE_POSTHANDLER
/* Start section to execute code from RAM */
# define FBLDIAG_RAMCODE_START_SEC_CODE
# include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */
/***********************************************************************************************************************
 *  FblDiagDefaultPostHandler
 **********************************************************************************************************************/
/*! \brief       Default diagnostics service post handler. Resets FblCw assignment of the connection address in case
 *                of default diagnostic session.
 *  \param[in]   postParam Parameter indicating service response.
 **********************************************************************************************************************/
void FblDiagDefaultPostHandler( vuint8 postParam )
{
#if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   /* Parameters not used: avoid compiler warning */
   (void)postParam; /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif

   /* Check session and reset FblCw assignment of the connection address in case of default session */
   if( GetDiagDefaultDiagSession() )
   {
# if defined( FBL_DIAG_ENABLE_GM_RESET_TESTER_IN_DEF_SESSION )
#  if defined( V_GEN_GENERATOR_MSR )
      FblCwResetResponseAddress();
#  else
      diagTesterAddr = FBL_DIAG_GM_NO_TESTER_CONNECTED;
#  endif /* V_GEN_GENERATOR_MSR */
# endif /* FBL_DIAG_ENABLE_GM_RESET_TESTER_IN_DEF_SESSION */
   }
}
/* Stop section to execute code from RAM */
# define FBLDIAG_RAMCODE_STOP_SEC_CODE
# include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */
#endif /* FBL_ENABLE_SERVICE_POSTHANDLER */

#if defined( V_GEN_GENERATOR_MSR )
#else
/***********************************************************************************************************************
 *  FblDiagCheckTesterSourceAddr
 **********************************************************************************************************************/
/*! \brief         Checks if ID is allowed to be received in current context
 *  \details       Check if ID is equal to previous received Tester id in case data indication is in progress.
 *  \param[in]     sourceId ID of received frame
 *  \return        kFblOk: source Id valid, request is going to be processed
 *  \return        kFblFailed: Either source Id invalid or Ecu is busy.
 *********************************************************************************************************************/
tFblResult FblDiagCheckTesterSourceAddr(vuint8 sourceId)
{
   tFblResult retVal = kFblOk;

   if( GetDiagBufferLocked() )
   {
      /* Do nothing, normal processing */
   }
   else if( FBL_DIAG_GM_NO_TESTER_CONNECTED == diagTesterAddr )
   {
      /* This is again written to the same value in Cw in case of Range-IDs. */
      FblCwTxSetTargetAddress((vuint16)sourceId);
      diagTesterAddr = sourceId;
   }
   else if (sourceId != diagTesterAddr)
   {
      retVal = kFblFailed;
   }
   else
   {
      /* Do nothing, normal processing. */
   }

   return retVal;
}
#endif /* V_GEN_GENERATOR_MSR */

#define FBLDIAG_RAMCODE_START_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */
/***********************************************************************************************************************
 *  FblDummyFunction
 **********************************************************************************************************************/
/*! \brief         Dummy function used for all Diag Core functions not required in Gm SLP6
 *********************************************************************************************************************/
void FblDummyFunction(void)
{

}
# define FBLDIAG_RAMCODE_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/***********************************************************************************************************************
 *  END OF DIAGNOSTIC SERVICE MANAGEMENT HANDLERS
 **********************************************************************************************************************/

/* module specific MISRA deviations:

   MD_FblDiag_2214:
     Reason: This is an assertion macro used only for debugging in development (FBL_ENABLE_ASSERTION).
     Risk: No risk.
     Prevention: No prevention defined.

   MD_FblDiag_Debug_0310
     Reason: The compiler macro __FILE__ is casted to (vuint8 *) object type. The definition of __FILE__ is compiler
             platform specific.
     Risk: The file name information is only used for analysis purpose. Moreover the code is only available
           if Debug Status configuration (FBL_ENABLE_DEBUG_STATUS is set), which is only the case in GENy project
           state "Integration". For GENy project state "Production" the code is no more executed.
     Prevention: Change to Project State "Production" in GENy to disable Debug Status.

   MD_FblDiag_3673:
     Reason: Standard Api that cannot easily be changed.
     Risk: Decreased readability, function may be compiled with less efficient code.
     Prevention: No prevention defined.

   MD_FblDiag_3382:
     Reason: The minimum length check of fbl_diag_core checks this value to be large enough and does not allow for wraparound here
     Risk: Minim length check wrongly configured.
     Prevention: Assertion guards this expression.

   MD_FblDiag_3297:
     Reason: The minimum length check of fbl_diag_core checks this value to be large enough and does not allow for wraparound here
     Risk: Minim length check wrongly configured.
     Prevention: Assertion guards this expression.

   MD_FblDiag_3684:
     Reason: Ping message is configurable and variable. User only concentrate on definition of message.
     Risk: There is a development feature.
     Prevention: No prevention defined

*/

/***********************************************************************************************************************
 *  END OF FILE: FBL_DIAG_OEM.C
 **********************************************************************************************************************/

