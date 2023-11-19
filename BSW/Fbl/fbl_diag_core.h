/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief         Diagnostic services supported in boot mode
 *                 Declaration of functions, variables, and constants
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
 *  Sebastian Loos                Shs           Vector Informatik GmbH
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  02.00.00   2015-03-02  Rie     ESCAN00078948    General rework
 *                         Ach     ESCAN00079649    No changes
 *  02.01.00   2015-06-03  Ach     ESCAN00083232    Use gap fill feature from FblLib_Mem
 *                                 ESCAN00083244    No changes
 *                                 ESCAN00082920    No changes
 *  02.02.00   2015-06-22  Ach     ESCAN00083496    Renamed block number get function encapsulation
 *  02.03.00   2015-07-02  Ach     ESCAN00083675    No changes
 *  02.03.01   2015-08-13  Ach     ESCAN00084514    No changes
 *  02.04.00   2015-11-10  Ach     ESCAN00086228    CheckSubparameter() sets incorrect error bit
 *                                 ESCAN00086236    Added configuration option for security access delay
 *                                                  flag initialization
 *  02.05.00   2016-02-26  Ach     ESCAN00086880    No changes
 *                                 ESCAN00088283    Added new state "Response pending allowed"
 *                                 ESCAN00088286    Added configuration options for startup response contents
 *                                 ESCAN00088288    Support non-kernelbuilder main module
 *  02.05.01   2016-02-29  Ach     ESCAN00088609    No changes
 *  02.06.00   2016-05-10  Rie     ESCAN00089943    Added additional RoutineControlTypes, moved MISRA justifications to header
 *  02.06.01   2016-05-20  Rie     ESCAN00090104    No changes
 *  02.06.02   2016-06-07  Shs     ESCAN00090324    No changes
 **********************************************************************************************************************/

#ifndef __FBL_DIAG_CORE_H__
#define __FBL_DIAG_CORE_H__

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  VERSION
 **********************************************************************************************************************/

/* ##V_CFG_MANAGEMENT ##CQProject : FblDiag_14229_Core CQComponent : Implementation */
#define FBLDIAG_14229_COREINT_VERSION           0x0206u
#define FBLDIAG_14229_COREINT_RELEASE_VERSION   0x02u

/* Interface version */
/** Major interface version identifies incompatible changes */
#define FBL_DIAGCORE_API_VERSION_MAJOR    0x02u
/** Minor interface version identifies backward compatible changes */
#define FBL_DIAGCORE_API_VERSION_MINOR    0x06u
/** Release interface version identifies cosmetic changes */
#define FBL_DIAGCORE_API_VERSION_RELEASE  0x00u

/***********************************************************************************************************************
 *  Default Configurations
 **********************************************************************************************************************/
#if !defined( FBL_DIAG_ENABLE_CORE_READ_SECACCESSDELAY_FLAG ) && \
    !defined( FBL_DIAG_DISABLE_CORE_READ_SECACCESSDELAY_FLAG )
# define FBL_DIAG_ENABLE_CORE_READ_SECACCESSDELAY_FLAG
#endif

 /***********************************************************************************************************************
 *  Session handling
 **********************************************************************************************************************/

/* Session handling control bits */
#define kDiagSessionDefault                  FBL_BIT0
#define kDiagSessionExtended                 FBL_BIT1
#define kDiagSessionProgramming              FBL_BIT2
#define kDiagLengthSpecialCheck              FBL_BIT4
#define kDiagSecuredService                  FBL_BIT5     /**< Indicates if service or subfunction requires security access */
#define kDiagServiceIsSubfunction            FBL_BIT6     /**< Indicates if service supports subfunctions request is allowed for this service */
#define kDiagFunctionalRequestAllowed        FBL_BIT7     /**< Indicates if a function request is allowed for this service */

/***********************************************************************************************************************
 *  Service handling
 **********************************************************************************************************************/

/* Default pointer values */
#define kDiagSubFctNoInfoTable            (tSubFctInfoTable *)V_NULL
#define kDiagSubFctNoMainHandler          (tFblDiagMainHandler)V_NULL

/***********************************************************************************************************************
 *  State handling
 **********************************************************************************************************************/

#define kDiagStateNone                    0x00ul

/* Diagnostic session states handling */
#define ClrDiagSession()                  FBL_STATE_MULTICLR(fblDiagStates, kDiagStateIdxSessionDefault, kDiagStateIdxSessionProgramming)

/* Session control macros */
#define kDiagStateSessionDefault          kDiagStateMask(kDiagStateIdxSessionDefault)
#define SetDiagDefaultSession()                    \
{                                                  \
   ClrDiagSession();                               \
   SetDiagState(kDiagStateIdxSessionDefault);      \
}  /* PRQA S 3458 */ /* MD_MSR_19.4 */

#define kDiagStateSessionExtended         kDiagStateMask(kDiagStateIdxSessionExtended)

#define SetDiagExtendedSession()                   \
{                                                  \
   ClrDiagSession();                               \
   SetDiagState(kDiagStateIdxSessionExtended);     \
}  /* PRQA S 3458 */ /* MD_MSR_19.4 */

#define kDiagStateSessionProgramming      kDiagStateMask(kDiagStateIdxSessionProgramming)
#define SetDiagProgrammingSession()                \
{                                                  \
   ClrDiagSession();                               \
   SetDiagState(kDiagStateIdxSessionProgramming);  \
}  /* PRQA S 3458 */ /* MD_MSR_19.4 */

/* Bit masks to handle session checks */
#define kDiagStateSessionMask             (kDiagStateSessionDefault|kDiagStateSessionExtended|kDiagStateSessionProgramming)
#define GetCurrentSession()               (vuint8)(fblDiagStates[0] & kDiagStateSessionMask)

/* Helper macro to get number of array entries */
#define ARRAY_SIZE(arr)                   (sizeof(arr) / sizeof((arr)[0])) /* PRQA S 3453 */ /* MD_MSR_19.7 */

/* Diagnostic handling states */
#define ClrDiagDefaultSession()           ClrDiagState( kDiagStateIdxSessionDefault )
#define ClrDiagProgrammingSession()       ClrDiagState( kDiagStateIdxSessionProgramming )
#define ClrDiagExtendedSession()          ClrDiagState( kDiagStateIdxSessionExtended )
#define SetDiagBufferLocked()             SetDiagState( kDiagStateIdxDiagBufferLocked )
#define ClrDiagBufferLocked()             ClrDiagState( kDiagStateIdxDiagBufferLocked )
#define SetDiagHandlerCalled()            SetDiagState( kDiagStateIdxDiagHandlerCalled )
#define ClrDiagHandlerCalled()            ClrDiagState( kDiagStateIdxDiagHandlerCalled )
#define SetRcrRpAllowed()                 SetDiagState( kDiagStateIdxRcrRpAllowed )
#define ClrRcrRpAllowed()                 ClrDiagState( kDiagStateIdxRcrRpAllowed )
#define SetRcrRpInProgress()              SetDiagState( kDiagStateIdxRcrRpInProgress )
#define ClrRcrRpInProgress()              ClrDiagState( kDiagStateIdxRcrRpInProgress )
#define SetResponseProcessing()           SetDiagState( kDiagStateIdxResponseProcessing )
#define ClrResponseProcessing()           ClrDiagState( kDiagStateIdxResponseProcessing )
#define SetSuppressPosRspMsg()            SetDiagState( kDiagStateIdxSuppressPosRspMsg )
#define ClrSuppressPosRspMsg()            ClrDiagState( kDiagStateIdxSuppressPosRspMsg )
#define SetServiceHasSubfunction()        SetDiagState( kDiagStateIdxServiceHasSubfunction )
#define ClrServiceHasSubfunction()        ClrDiagState( kDiagStateIdxServiceHasSubfunction )
#define SetServiceInProgress()            SetDiagState( kDiagStateIdxServiceInProgress )
#define ClrServiceInProgress()            ClrDiagState( kDiagStateIdxServiceInProgress )
#define SetFunctionalRequest()            SetDiagState( kDiagStateIdxFunctionalRequest )
#define ClrFunctionalRequest()            ClrDiagState( kDiagStateIdxFunctionalRequest )
#define SetTransferTypeFlash()            SetDiagState( kDiagStateIdxTransferTypeFlash )
#define ClrTransferTypeFlash()            ClrDiagState( kDiagStateIdxTransferTypeFlash )
/* Reset handling states */
#define SetWaitEcuReset()                       \
{                                               \
   SetDiagState( kDiagStateIdxWaitEcuReset );   \
   ResetEcuResetTimeout();                      \
}  /* PRQA S 3458 */ /* MD_MSR_19.4 */
#define ClrWaitEcuReset()                       \
{                                               \
   ClrDiagState( kDiagStateIdxWaitEcuReset );   \
   StopEcuResetTimeout();                       \
}  /* PRQA S 3458 */ /* MD_MSR_19.4 */
#define SetEcuResetFctFinished()          SetDiagState( kDiagStateIdxEcuResetFctFinished )
#define ClrEcuResetFctFinished()          ClrDiagState( kDiagStateIdxEcuResetFctFinished )
#define SetResetMsgConfirmed()            SetDiagState( kDiagStateIdxResetMsgConfirmed )
#define ClrResetMsgConfirmed()            ClrDiagState( kDiagStateIdxResetMsgConfirmed )
/* Generic download states */
#define SetPreconditionsChecked()         SetDiagState( kDiagStateIdxPreconditionsChecked )
#define ClrPreconditionsChecked()         ClrDiagState( kDiagStateIdxPreconditionsChecked )
#define SetMemDriverInitialized()         SetDiagState( kDiagStateIdxMemDriverInitialized )
#define ClrMemDriverInitialized()         ClrDiagState( kDiagStateIdxMemDriverInitialized )

/***********************************************************************************************************************
 *  Timeout handling
 **********************************************************************************************************************/

/* Diagnostic session timing */
#define kDiagSessionTimingP2                 ( kFblDiagTimeP2 * FBL_REPEAT_CALL_CYCLE )
#define kDiagSessionTimingP2StarRaw          ( kFblDiagTimeP2Star * FBL_REPEAT_CALL_CYCLE )
#define kDiagSessionTimingP2Star             ( kDiagSessionTimingP2StarRaw  / 10u )

/* Threshold timer for P2 to trigger an RCR-RP */
#if !defined( kFblDiagP2MinThreshold )
# define kFblDiagP2MinThreshold              ( kDiagSessionTimingP2 / 2u )
#endif

/* Threshold timer for P2-Star to trigger an RCR-RP */
#if !defined( kFblDiagP2StarMinThreshold )
# define kFblDiagP2StarMinThreshold          ( kDiagSessionTimingP2StarRaw / 2u )
#endif

/* Security access delay */

/* ECU shutdown handling */
/* Maximum wait time for ECU reset request */
#if defined( kCwMaxWaitTimeEcuResetRequest )
# define kFblDiagMaxWaitTimeEcuResetRequest     (kCwMaxWaitTimeEcuResetRequest / FBL_REPEAT_CALL_CYCLE)
#else
# define kFblDiagMaxWaitTimeEcuResetRequest     kFblDiagTimeP2Star
#endif

/* ECU reset handling */
#if !defined( kFblDiagSubReset )
# define kFblDiagSubReset                       kDiagSubHardReset
#endif

/* Macros to access the ecuReset timer value */
#define TimeoutEcuResetValue()                  ecuResetTimeout
#define DecTimeoutEcuResetValue()              (ecuResetTimeout--)
#define ResetEcuResetTimeout()                 (ecuResetTimeout = (vuint16)(kFblDiagMaxWaitTimeEcuResetRequest/kDiagCallCycle))
#define StopEcuResetTimeout()                  (ecuResetTimeout = 0)

/* Security functions */

/* Error handling */
/* Access macros */
#define DiagClrError()                    ( diagErrorCode = kDiagErrorNone )  /**< Clear error status */
#define DiagGetError()                    ( diagErrorCode )                   /**< Get error (NRC) code */

#if defined ( FBL_DIAG_ENABLE_CORE_SEGMENTHANDLING )
/* Segment handling macros */
#define kSwmOutOfRange           0xFFu
#define FblDiagSegmentInit()     (segmentCount = 0)   /**< Initialize segment counter */
#endif /* FBL_DIAG_ENABLE_CORE_SEGMENTHANDLING */

/* Response state handling */
/* Access macros */
#define DiagSetNoResponse()               ( diagResponseFlag |= kDiagPutNoResponse )   /**< Do not send a response */
#define DiagGetNoResponse()               ( diagResponseFlag & kDiagPutNoResponse )    /**< Check if a response should be sent */

#define kDiagResponseIdle                 0x00u             /**< Response not yet configured */
#define kDiagResponseRequested            FBL_BIT0          /**< Response should be sent */
#define kDiagPutNoResponse                FBL_BIT1          /**< No response should be sent */
#define kDiagPutPosResponse               FBL_BIT2          /**< Positive response requested */
#define kDiagPutNegResponse               FBL_BIT3          /**< Negative response requested - superseeds positive response request */
#define kDiagRepeatResponse               FBL_BIT7          /**< Response should be repeated */

/* Parameter for post-handler::diagPostParam  */
#define kDiagPostPosResponse              0x00u             /**< No error - positive response */
#define kDiagPostNegResponse              0x01u             /**< NRC set - negative response */
#define kDiagPostFailedResponse           0x02u             /**< Retry to send response */
#define kDiagPostRcrRp                    0x03u             /**< RCRRP sent */
#define kDiagPostNoResponse               0x04u             /**< No response to be sent */

/* Service handling macros */
/* Return values of service checks - used as bit mask*/

#define kServiceFailure                   0x4000u           /**< Indication of failed service check */
#define kSubServiceFailure                0x8000u           /**< Indication of failed sub-service check */
#define kCheckValid                       0x0000u           /**< Subfunction found and valid. Proceed to process. */

#define kServiceValid                     kCheckValid
#define kSubServiceValid                  kCheckValid

#define kCheckLenFailure                  0x0001u           /**< Wrong length */
#define kCheckSessionFailure              0x0002u           /**< Wrong session */
#define kCheckParameterNotFound           0x0004u           /**< Parameter not found (subparameter with index > 0) */
#define kCheckFunctionNotFound            0x0008u           /**< Subfunction not found (subparameter with index 0) */
#define kCheckTableMismatch               0x0010u           /**< No table entry found at all */
#define kCheckSecAccFailure               0x0020u           /**< Security level not correct */
#define kCheckNoFuncSupport               0x0040u           /**< Not supported functionally */
#define kCheckNoPhysSupport               0x0080u           /**< Not supported physically */
#define kCheckMinLenFailure               0x0100u           /**< Minimum length check failed */
#define kCheckParameterFound              0x1000u           /**< Requested parameter is supported */
#define kCheckFunctionFound               0x2000u           /**< Requested sub function is supported */

#define kServiceLenFailure                (kServiceFailure | kCheckLenFailure)            /**< Wrong length for service */
#define kServiceMinLenFailure             (kServiceFailure | kCheckMinLenFailure)
#define kServiceSessionFailure            (kServiceFailure | kCheckSessionFailure)        /**< Wrong session for service */
#define kServiceParameterNotFound         (kServiceFailure | kCheckParameterNotFound)     /**< Parameter not found */
#define kServiceFunctionNotFound          (kServiceFailure | kCheckFunctionNotFound)      /**< Subfunction not found */
#define kServiceTableMismatch             (kServiceFailure | kCheckTableMismatch)         /**< No table entry found at all */
#define kServiceSecAccFailure             (kServiceFailure | kCheckSecAccFailure)         /**< Function security level not correct */
#define kServiceNoFuncSupport             (kServiceFailure | kCheckNoFuncSupport)         /**< Function not supported functionally */
#define kServiceNoPhysSupport             (kServiceFailure | kCheckNoPhysSupport)         /**< Function not supported physically */

#define kSubServiceLenFailure             (kSubServiceFailure | kCheckLenFailure)         /**< Wrong length for subparameter */
#define kSubServiceMinLenFailure          (kSubServiceFailure | kCheckMinLenFailure)
#define kSubServiceSessionFailure         (kSubServiceFailure | kCheckSessionFailure)     /**< Wrong session for subparameter */
#define kSubServiceParameterNotFound      (kSubServiceFailure | kCheckParameterNotFound)  /**< Parameter not found (subparameter with index > 0) */
#define kSubServiceFunctionNotFound       (kSubServiceFailure | kCheckFunctionNotFound)   /**< Subfunction not found (subparameter with index 0) */
#define kSubServiceTableMismatch          (kSubServiceFailure | kCheckTableMismatch)      /**< No table entry found at all */
#define kSubServiceSecAccFailure          (kSubServiceFailure | kCheckSecAccFailure)      /**< Sub function security level not correct */
#define kSubServiceNoFuncSupport          (kSubServiceFailure | kCheckNoFuncSupport)      /**< Sub function not supported functionally */
#define kSubServiceNoPhysSupport          (kSubServiceFailure | kCheckNoPhysSupport)      /**< Sub function not supported physically */

#define kSubServiceParameterFound         (kSubServiceFailure | kCheckParameterFound)     /**< Requested parameter is supportede */
#define kSubServiceFunctionFound          (kSubServiceFailure | kCheckFunctionFound)      /**< Requested sub function is supported */

/* Index definition for service handler table (kServiceMainHandlerFctTable) */
#define kServiceMainHandlerNoHandler      -1
#define kServiceMainHandlerDefault         0

#define FblDiagGetSubParamTabIndex()       subParTabIdx

/***********************************************************************************************************************
 *  DATA TYPE DEFINITIONS
 **********************************************************************************************************************/

/** Structure definition for subfunction parameter check */
typedef struct tagSubFctInfoTable
{
   /** Pointer to subfunction ID table */
   V_MEMROM1 vuint8 V_MEMROM2 V_MEMROM3  *subFctParamIdTable;
   /** Session support, length check and security level configuration */
   vuint8                     sessionSupport;
   /** Service length. If special length check is not set, fixed check against this value. */
   tCwDataLengthType          serviceLen;
   /** Selects main handler function */
   vsintx                     mainInstanceHandlerIdx;
} tSubFctInfoTable;

/** This structure is used for the diagnostic state machine */
typedef struct tagServiceInfoTable
{
   /** Service ID */
   vuint8                     serviceId;
   /** Session support, length check and security level configuration */
   vuint8                     sessionSupport;
   /** Service length. If special length check is not set, fixed check against this value.
       Value is treated as minimum length if subfunctions are present. */
   tCwDataLengthType          serviceLen;
   /** Length of subfunction table */
   vsintx                     subFunctionInfoLen;
   /** Length of subfunction ID */
   vsintx                     subFctParamIdTableLen;
   /** Pointer to subfunction table */
   V_MEMROM1 tSubFctInfoTable V_MEMROM2 V_MEMROM3  *subFctInfo;
#if defined( FBL_ENABLE_SERVICE_PREHANDLER )
   /** Index to prehandler list */
   vuintx                     preHandlerIdx;
#endif
   /** Index to mainhandler list */
   vuintx                     mainHandlerIdx;
#if defined( FBL_ENABLE_SERVICE_POSTHANDLER )
   /** Index to posthandler list */
   vuintx                     postHandlerIdx;
#endif
} tServiceInfoTable;

/***********************************************************************************************************************
 *  LOCAL DATA - visible in core and OEM layer, but not in other bootloader modules
 **********************************************************************************************************************/

V_MEMRAM0 extern V_MEMRAM1_NEAR vuint8          V_MEMRAM2_NEAR diagResponseFlag;
V_MEMRAM0 extern V_MEMRAM1 vuint16              V_MEMRAM2      ecuResetTimeout;
V_MEMRAM0 extern V_MEMRAM1 vuint8               V_MEMRAM2      diagErrorCode;
#if defined( FBL_DIAG_ENABLE_CORE_SEGMENTHANDLING )
V_MEMRAM0 extern V_MEMRAM1 vuint8               V_MEMRAM2      segmentCount;
#endif /* FBL_DIAG_ENABLE_CORE_SEGMENTHANDLING */
V_MEMRAM0 extern V_MEMRAM1 vuint8               V_MEMRAM2      subParTabIdx;
V_MEMRAM0 extern V_MEMROM1 tServiceInfoTable    V_MEMROM2 V_MEMROM3 * V_MEMRAM1 V_MEMRAM2 serviceInfo;
/* Data - implemented in OEM layer */
/* PRQA S 3684 2 */ /* MD_FblDiag_3864 */
V_MEMROM0 extern V_MEMROM1 tFblDiagMainHandler  V_MEMROM2      kServiceMainHandlerFctTable[];
V_MEMROM0 extern V_MEMROM1 tServiceInfoTable    V_MEMROM2      kServiceInfoTable[];
V_MEMROM0 extern V_MEMROM1 vuintx V_MEMROM2 kSizeOfServiceTable;
V_MEMROM0 extern V_MEMROM1 vuintx V_MEMROM2 kSizeOfMainHandlerTable;
#if defined( FBL_DIAG_ENABLE_DYNAMIC_P2_HANDLING )
V_MEMRAM0 extern V_MEMRAM1 vuint16 V_MEMRAM2 fblDiagTimeP2Max;    /**< P2 base value */
#endif /* FBL_DIAG_ENABLE_DYNAMIC_P2_HANDLING */
#if defined( FBL_ENABLE_SERVICE_POSTHANDLER )
/* Start section to execute code from RAM */
# define FBLDIAG_START_SEC_RAMCONST_UNSPECIFIED
# include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */
/* PRQA S 3684 1 */ /* MD_FblDiag_3864 */
V_MEMROM0 extern V_MEMROM1 tFblDiagPostHandler  V_MEMROM2      kServicePostHandlerFctTable[];
/* End section to execute code from RAM */
# define FBLDIAG_STOP_SEC_RAMCONST_UNSPECIFIED
# include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */
#endif /* FBL_ENABLE_SERVICE_POSTHANDLER */

/***********************************************************************************************************************
 *  PROTOTYPES
 **********************************************************************************************************************/
#if defined( FBL_DIAG_ENABLE_CORE_SEGMENTHANDLING )
vuint8 FblDiagSegmentNext(void);
#endif /* FBL_DIAG_ENABLE_CORE_SEGMENTHANDLING */
#if defined( FBL_DIAG_ENABLE_CORE_RANGECHECKS )
tFblResult FblCheckAddressRange(tFblAddress address, tFblAddress rangeStart, tFblLength rangeLength);
tFblResult FblCheckRangeContained(tFblAddress address, tFblLength length, tFblAddress rangeStart, tFblLength rangeLength);
#endif /* FBL_DIAG_ENABLE_CORE_RANGECHECKS */
#if defined( FBL_DIAG_ENABLE_CORE_GETBLOCKNR )
# if defined( FBL_DIAG_ENABLE_ADDR_BASED_DOWNLOAD )
tFblResult FblGetBlockNrFromAddress(tFblAddress blockAddress, tFblLength blockLength, vuint8* pLogicalBlock);
# else
tFblResult FblGetBlockNrFromIndex(vuint8 blockIndex, vuint8* pLogicalBlock);
# endif /* FBL_DIAG_ENABLE_ADDR_BASED_DOWNLOAD */
#endif /* FBL_DIAG_ENABLE_CORE_GETBLOCKNR */
void FblDiagEcuReset( void );

/* Functions to be implemented in OEM layer */
/* Service handling functions */
tFblResult FblDiagDefaultMainHandler(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen);
/* Support functions */
void FblDiagProcessRcNrc(V_MEMROM1 tServiceInfoTable V_MEMROM2 V_MEMROM3* pServiceInfo, vuint16 subFctResult);
void FblDiagProcessSubfunctionNrc(V_MEMROM1 tServiceInfoTable V_MEMROM2 V_MEMROM3* pServiceInfo, vuint16 subFctResult);
void FblDiagProcessSubparamNrc(V_MEMROM1 tServiceInfoTable V_MEMROM2 V_MEMROM3* pServiceInfo, vuint16 subFctResult);
void FblDiagProcessServiceNrc(V_MEMROM1 tServiceInfoTable V_MEMROM2 V_MEMROM3* pServiceInfo, vuint16 serviceCheckResult);
vuint8 FblDiagCheckSpecialLength(vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen,
                                 V_MEMROM1 tServiceInfoTable V_MEMROM2 V_MEMROM3* pServiceInfo);
#if defined( FBL_DIAG_ENABLE_OEM_INITPOWERON )
void FblDiagOemInitPowerOn(void);
#endif /* FBL_DIAG_ENABLE_OEM_INITPOWERON */
#if defined( FBL_DIAG_ENABLE_OEM_INIT )
void FblDiagOemInit(void);
#endif /* FBL_DIAG_ENABLE_OEM_INIT */
#if defined( FBL_DIAG_ENABLE_OEM_POSTINIT )
void FblDiagOemPostInit(void);
#endif /* FBL_DIAG_ENABLE_OEM_POSTINIT */
#if defined( FBL_DIAG_ENABLE_OEM_SPS_REQUEST )
void FblDiagOemPrepareSpsRequest(V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * pDiagBuffer, V_MEMRAM1 tCwDataLengthType V_MEMRAM2 V_MEMRAM3 *pDiagDataLength);
#endif /* FBL_DIAG_ENABLE_OEM_SPS_REQUEST */
#if defined( FBL_DIAG_ENABLE_OEM_DEINIT )
void FblDiagOemDeinit(void);
#endif /* FBL_DIAG_ENABLE_OEM_DEINIT */
#if defined( FBL_DIAG_ENABLE_OEM_STATETASK )
void FblDiagOemStateTask(void);
#endif /* FBL_DIAG_ENABLE_OEM_STATETASK */
#if defined( FBL_DIAG_ENABLE_OEM_TIMERTASK )
void FblDiagOemTimerTask(void);
#endif /* FBL_DIAG_ENABLE_OEM_TIMERTASK */

#if defined( FBL_DIAG_ENABLE_OEM_SESSION_TIMEOUT )
void FblDiagSessionTimeout(void);
#endif

#if defined( FBL_ENABLE_SERVICE_POSTHANDLER )
/* Diagnostic post-handler service functions */
/* Start section to execute code from RAM */
# define FBLDIAG_RAMCODE_START_SEC_CODE
# include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */
extern void FblDiagDefaultPostHandler( vuint8 postParam );
/* End section to execute code from RAM */
# define FBLDIAG_RAMCODE_STOP_SEC_CODE
# include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */
#endif /* FBL_ENABLE_SERVICE_POSTHANDLER */

#endif /* __FBL_DIAG_CORE_H__ */

/* module specific MISRA deviations:

   MD_FblDiag_0277:
      Reason: Overflow is used to define maximum possible value.
      Risk: No identifiable risk.
      Prevention: No prevention required.

   MD_FblDiag_0306:
      Reason: Address conversion between integer values and pointers is required to allow for hardware independent
      configuration and address range checks.
      Risk: The size of integer required to hold the result of a pointer cast is implementation defined.
      Prevention: The size of the respective integer data type which holds the address value is adapted on a hardware
      specific basis.

   MD_FblDiag_0313:
      Reason: Signature/checksum verification functions are stored as void pointers as actual type is not known at
       configuration time.
      Risk: Function signature not compatible with expected type.
      Prevention: Integrator has to take care the configured functions match with the expected signature.

   MD_FblDiag_0781:
      Reason: The variable is being used as a structure/union member as well as being a label, tag or ordinary
       identifier. This way it is more clear to the reader which purpose the variable has.
      Risk: No identifiable risk.
      Prevention: No prevention required.

   MD_FblDiag_0883:
      Reason: Different defines are set depending on a hardware dependent define to optimize data structures
       for the used hardware platform. #elif is chosen to make sure only one setting is selected.
      Risk: No identifiable risk because repeated inclusion protection is done at teh begin of the file.
      Prevention: No prevention required.

   MD_FblDiag_2214:
      Reason: Assertion macros have to be disabled in production code and are used only for development.
      Risk: Assertion code may be enabled in production code unintentionally. If a assertion condition is unexpectedly
         false, the code is active. A problem may occur if the Macro is additionally changed from single statement
         to multiple statement.
      Prevention: Macro is not allowed to be changed without review. Development code is automatically disabled via
         configuration of project state "Production".

   MD_FblDiag_3109:
      Reason: Multi-line macros encapsulated in curly brackets to reduce side-effects. Results in empty statement after
       the closing bracket.
      Risk: No identifiable risk.
      Prevention: No prevention required.

   MD_FblDiag_319x:
      Reason: Value assignment is done but the value is modified again before use or not used. The assignment is done to keep
       DiagData and the local pointer pbDiagData consistent.
      Risk: Adds a unused assignment which could result in a compiler warning.
      Prevention: Ignore compiler warning if observed.

   MD_FblDiag_3199:
      Reason: Value assignment is done but assigned value is not used in all configurations.
      Risk: Adds an possibly unused assignment which could result in a compiler warning.
      Prevention: Ignore compiler warning if observed.

   MD_FblDiag_3201:
      Reason: Function calls are not used in all configurations because if statement may be always true depending on
       the module's configuration.
      Risk: Unused statements could result in a compiler warning, slightly increased resource consumption because
       of 2 unused statements.
      Prevention: Ignore compiler warning if observed.

   MD_FblDiag_3218:
      Reason: Configuration constants/tables are kept at a central location for a better overview and maintenance.
      Risk: Scope is larger than required (whole file instead of one function). Some other function could access the variable.
      Prevention: Take care that this deviation is used for constant data only (which cannot be modified).

   MD_FblDiag_3425:
      Reason: The WrapNv call uses the ternary operator to remap the return value of IO_E_OK to kFblOk. As both of
       them are defined to the value zero the remapping has no side affects.
      Risk: No identifiable risk.
      Prevention: No prevention required.

   MD_FblDiag_3673:
     Reason: The prototype is not adapted because future code additions might use the respective parameter as output.
     Risk: The parameter value might be changed unintentionally, thus causing malfunction in the calling instance.
     Prevention: All changes to the affected code parts have to be reviewed by the module responsible.

   MD_FblDiag_3864:
      Reason: Symbol definition of array is done without array size. This is done because the arrays are implemente
       by the OEM layer, but the interface has to be defined by the core layer.
      Risk: The compiler is not able to check array access bounds.
      Prevention: Array accesses have to be verified by code inspection.

   MD_FblDiag_3689:
      Reason: A signed variable is used as array index variable.
      Risk: A negative value may be used as index. Data from outside the array will be returned.
      Prevention: Value of index variable has to be checked before accessing the array.

*/

/***********************************************************************************************************************
 *  END OF FILE: FBL_DIAG_CORE.H
 **********************************************************************************************************************/
