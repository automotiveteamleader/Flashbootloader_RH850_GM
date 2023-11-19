/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief         Diagnostic module external interface
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
 *                                 ESCAN00082920    Fix compiler warning if SysService_SecModHis is not used
 *  02.02.00   2015-06-22  Ach     ESCAN00083496    No changes
 *  02.03.00   2015-07-02  Ach     ESCAN00083675    No changes
 *  02.03.01   2015-08-13  Ach     ESCAN00084514    Removed bus-type configuration check
 *  02.04.00   2015-11-10  Ach     ESCAN00086228    CheckSubparameter() sets incorrect error bit
 *                                 ESCAN00086236    Added configuration option for security access delay
 *                                                  flag initialization
 *  02.05.00   2016-02-26  Ach     ESCAN00086880    No changes
 *                                 ESCAN00088283    Added new state "Response pending allowed"
 *                                 ESCAN00088286    Added configuration options for startup response contents
 *                                 ESCAN00088288    Support non-kernelbuilder main module
 *  02.05.01   2016-02-29  Ach     ESCAN00088609    No changes
 *  02.06.00   2016-05-10  Rie     ESCAN00089943    Added additional RoutineControlTypes
 *  02.06.01   2016-05-20  Rie     ESCAN00090104    No changes
 *  02.06.02   2016-06-07  Shs     ESCAN00090324    No changes
 **********************************************************************************************************************/

#ifndef __FBL_DIAG_H__
#define __FBL_DIAG_H__

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

/* OEM specific interface additions */
#include "fbl_diag_oem.h"

/***********************************************************************************************************************
 *  VERSION
 **********************************************************************************************************************/

#define FBLDIAG_14229_CORE_VERSION           0x0206u
#define FBLDIAG_14229_CORE_RELEASE_VERSION   0x02u

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

/* Diagnostic base timing */
#if !defined( kDiagCallCycle )
# define kDiagCallCycle    1u
#endif

/* Maximum segment size of all memory devices */
#define FBL_DIAG_SEGMENT_SIZE                   FBL_MAX_SEGMENT_SIZE

/* Generic helper macros */
/* PRQA S 3453 2 */ /* MD_MSR_19.7 */
#define GET_ID_HIGH(id)                         ((vuint8)(((id) >> 8u) & 0xFFu))
#define GET_ID_LOW(id)                          ((vuint8)((id) & 0xFFu))

/***********************************************************************************************************************
*  Core state definitions
**********************************************************************************************************************/

/* Bootloader states */
#define kDiagStateIdxSessionDefault             0u
#define kDiagStateIdxSessionExtended            1u
#define kDiagStateIdxSessionProgramming         2u
#define kDiagStateIdxFunctionalRequest          3u
#define kDiagStateIdxDiagBufferLocked           4u
#define kDiagStateIdxDiagHandlerCalled          5u
#define kDiagStateIdxServiceInProgress          6u
#define kDiagStateIdxRcrRpAllowed               7u
#define kDiagStateIdxRcrRpInProgress            8u
#define kDiagStateIdxSuppressPosRspMsg          9u
#define kDiagStateIdxServiceHasSubfunction      10u
#define kDiagStateIdxResponseProcessing         11u
#define kDiagStateIdxTransferTypeFlash          12u
/* Reset handling */
#define kDiagStateIdxWaitEcuReset               13u
#define kDiagStateIdxEcuResetFctFinished        14u
#define kDiagStateIdxResetMsgConfirmed          15u
/* Generic download handling */
#define kDiagStateIdxPreconditionsChecked       16u
#define kDiagStateIdxMemDriverInitialized       17u

#define kDiagLastCoreStateIdx                   kDiagStateIdxMemDriverInitialized

/* Number of states (Core and OEM) */
#define kDiagNumberOfStates                     ( kDiagLastOemStateIdx + 1u )
#define kDiagNumberOfDispatchStates             kDiagNumberOfStates

/* Diagnostic handling states */
#define GetDiagDefaultDiagSession()             GetDiagState( kDiagStateIdxSessionDefault )
#define GetDiagProgrammingSession()             GetDiagState( kDiagStateIdxSessionProgramming )
#define GetDiagExtendedSession()                GetDiagState( kDiagStateIdxSessionExtended )
#define GetDiagBufferLocked()                   GetDiagState( kDiagStateIdxDiagBufferLocked )
#define GetDiagHandlerCalled()                  GetDiagState( kDiagStateIdxDiagHandlerCalled )
#define GetRcrRpAllowed()                       GetDiagState( kDiagStateIdxRcrRpAllowed )
#define GetRcrRpInProgress()                    GetDiagState( kDiagStateIdxRcrRpInProgress )
#define GetResponseProcessing()                 GetDiagState( kDiagStateIdxResponseProcessing )
#define GetSuppressPosRspMsg()                  GetDiagState( kDiagStateIdxSuppressPosRspMsg )
#define GetServiceHasSubfunction()              GetDiagState( kDiagStateIdxServiceHasSubfunction )
#define GetServiceInProgress()                  GetDiagState( kDiagStateIdxServiceInProgress )
#define GetFunctionalRequest()                  GetDiagState( kDiagStateIdxFunctionalRequest )
# define GetTransferTypeFlash()                 GetDiagState( kDiagStateIdxTransferTypeFlash )
/* Reset handling states */
#define GetWaitEcuReset()                       GetDiagState( kDiagStateIdxWaitEcuReset )
#define GetEcuResetFctFinished()                GetDiagState( kDiagStateIdxEcuResetFctFinished )
#define GetResetMsgConfirmed()                  GetDiagState( kDiagStateIdxResetMsgConfirmed )
/* Generic download states */
#define GetPreconditionsChecked()               GetDiagState( kDiagStateIdxPreconditionsChecked )
#define GetMemDriverInitialized()               GetDiagState( kDiagStateIdxMemDriverInitialized )

/***********************************************************************************************************************
 *  State handling framework
 **********************************************************************************************************************/

#if defined( FBL_DEF_ENABLE_NON_KB_MAIN )
#else
/* State array settings */
/* PRQA S 0883 1 */ /* MD_FblDiag_0883 */
# if defined( C_CPUTYPE_8BIT )
typedef vuint8 tFblStateBitmap;
#  define FBL_STATE_BITS               8u
#  define kFblStateMaskNone            0x00u
#  define kFblStateMaskAll             0xFFu
#  define FblInvertStates(x)           FblInvert8Bit(x)  /* PRQA S 3453 */ /* MD_MSR_19.7 */
# elif defined( C_CPUTYPE_16BIT )
typedef vuint16 tFblStateBitmap;
#  define FBL_STATE_BITS               16u
#  define kFblStateMaskNone            0x0000u
#  define kFblStateMaskAll             0xFFFFu
#  define FblInvertStates(x)           FblInvert16Bit(x) /* PRQA S 3453 */ /* MD_MSR_19.7 */
#elif defined( C_CPUTYPE_32BIT )
typedef vuint32 tFblStateBitmap;
#  define FBL_STATE_BITS               32u
#  define kFblStateMaskNone            0x00000000ul
#  define kFblStateMaskAll             0xFFFFFFFFul
#  define FblInvertStates(x)           FblInvert32Bit(x) /* PRQA S 3453 */ /* MD_MSR_19.7 */
# else
#  error "Bit count of CPU not supported."
# endif

/* State mask build macros */
/* PRQA S 3453 10 */ /* MD_MSR_19.7 */
# define FBL_STATE_MASK(bit)                 ((tFblStateBitmap)(1ul << ((bit) % FBL_STATE_BITS)))
# define FBL_STATE_MULTIMASK(min, max)       ((((kFblStateMaskAll) >> ((min) % FBL_STATE_BITS)) << ((min) % FBL_STATE_BITS)) ^ (((kFblStateMaskAll) >> (((max) + 1) % FBL_STATE_BITS)) << (((max) + 1) % FBL_STATE_BITS)))
# define FBL_STATE_MASKLONG(bit)             (1ul << (bit))
# define FBL_STATE_MULTIMASKLONG(min, max)   ((((~0ul) >> (min)) << (min)) ^ (((~0ul) >> ((max) + 1)) << ((max) + 1)))
# define FBL_STATE_INDEX(bit)                ((bit) / FBL_STATE_BITS)
# define FBL_STATE_SET(arr, bit)             ((arr)[FBL_STATE_INDEX(bit)] |= FBL_STATE_MASK(bit))
# define FBL_STATE_CLR(arr, bit)             ((arr)[FBL_STATE_INDEX(bit)] &= FblInvertStates(FBL_STATE_MASK(bit)))
# define FBL_STATE_MULTICLR(arr, min, max)   ((arr)[FBL_STATE_INDEX(min)] &= FblInvertStates(FBL_STATE_MULTIMASK((min), (max))))
# define FBL_STATE_TEST(arr, bit)            (((arr)[FBL_STATE_INDEX(bit)] & FBL_STATE_MASK(bit)) == FBL_STATE_MASK(bit))

#endif /* FBL_DEF_ENABLE_NON_KB_MAIN */

#define FBL_DIAG_STATECHECK_ARRAYSIZE        (((kDiagNumberOfDispatchStates - 1u) / FBL_STATE_BITS) + 1u)

/* State access macros */
#define GetDiagState(state)                  FBL_STATE_TEST(fblDiagStates, (state)) /* PRQA S 3453 */ /* MD_MSR_19.7 */

/* Diagnostic state handling */
/* Control macros */
#define FBL_DIAG_STATE_CHECK_COUNT  0u
#define FBL_DIAG_STATE_ADDCLR_COUNT 0u
#define FBL_DIAG_STATE_COUNT        (FBL_DIAG_STATE_CHECK_COUNT + FBL_DIAG_STATE_ADDCLR_COUNT)

/* State array build macros */
#define FBL_DIAG_STATE_ARRAYENTRY(mask, idx)    ((tFblStateBitmap)(((mask) >> ((idx) * FBL_STATE_BITS)) & kDiagStateMaskAll))  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#if (FBL_DIAG_STATECHECK_ARRAYSIZE == 1)
# define FBL_STATE_BUILDARRAYENTRIES(mask)      FBL_STATE_ARRAYENTRY(mask, 0) /* PRQA S 3453 */ /* MD_MSR_19.7 */
#endif
#if (FBL_DIAG_STATECHECK_ARRAYSIZE == 2)
# define FBL_STATE_BUILDARRAYENTRIES(mask)      FBL_STATE_ARRAYENTRY(mask, 0), FBL_STATE_ARRAYENTRY(mask, 1)   /* PRQA S 3453 */ /* MD_MSR_19.7 */
#endif
#if (FBL_DIAG_STATECHECK_ARRAYSIZE == 3)
# define FBL_STATE_BUILDARRAYENTRIES(mask)      FBL_STATE_ARRAYENTRY(mask, 0), FBL_STATE_ARRAYENTRY(mask, 1), FBL_STATE_ARRAYENTRY(mask, 2)  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#endif
#if (FBL_DIAG_STATECHECK_ARRAYSIZE == 4)
# define STATE_BUILDARRAYENTRIES(mask)          FBL_STATE_ARRAYENTRY(mask, 0), FBL_STATE_ARRAYENTRY(mask, 1), FBL_STATE_ARRAYENTRY(mask, 2), FBL_STATE_ARRAYENTRY(mask, 3) /* PRQA S 3453 */ /* MD_MSR_19.7 */
#endif
#if (FBL_DIAG_STATECHECK_ARRAYSIZE > 4)
# error "Invalid array size for state check!"
#endif

#define DIAG_STATE_BUILDARRAY(mask)            { DIAG_STATE_BUILDARRAYENTRIES(mask) }

/* State masks and access macros */
#define kDiagStateMaskAllLong             FBL_STATE_MULTIMASKLONG(0, ((FBL_DIAG_STATECHECK_ARRAYSIZE * FBL_STATE_BITS) - 1))

#define kDiagStateMask(state)             FBL_STATE_MASKLONG(state)   /* PRQA S 3453 */ /* MD_MSR_19.7 */

#define SetDiagState(state)               FBL_STATE_SET(fblDiagStates, state)  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define ClrDiagState(state)               FBL_STATE_CLR(fblDiagStates, state)  /* PRQA S 3453 */ /* MD_MSR_19.7 */

/***********************************************************************************************************************
 *  Service handling
 **********************************************************************************************************************/

/* Diagnostic services */
#define kDiagSidDiagnosticSessionControl                 0x10u    /**< Service ID - Diagnostic session control */
#define kDiagSidEcuReset                                 0x11u    /**< Service ID - ECU reset */
#define kDiagSidReadDTCInformation                       0x19u    /**< Service ID - Read DTC information */
#define kDiagSidReadDataByIdentifier                     0x22u    /**< Service ID - Read data by Identifier */
#define kDiagSidSecurityAccess                           0x27u    /**< Service ID - Security Access */
#define kDiagSidCommunicationControl                     0x28u    /**< Service ID - Communication Control */
#define kDiagSidWriteDataByIdentifier                    0x2Eu    /**< Service ID - Write data by Identifier */
#define kDiagSidRoutineControl                           0x31u    /**< Service ID - Routine Control */
#define kDiagSidRequestDownload                          0x34u    /**< Service ID - Request Download */
#define kDiagSidTransferData                             0x36u    /**< Service ID - Transfer Data */
#define kDiagSidRequestTransferExit                      0x37u    /**< Service ID - Request Transfer Exit */
#define kDiagSidTesterPresent                            0x3Eu    /**< Service ID - Tester Present */
#define kDiagSidControlDTCSetting                        0x85u    /**< Service ID - Control DTC */
#define kDiagSidLinkControl                              0x87u    /**< Service ID - Link Control */

/* Response identifier service definition */
#define kDiagRidNegativeResponse                         0x7Fu    /**< Service ID - Negative response */

/* Diagnostic subservices */
#define kDiagSuppressPosRspMsgIndicationBit              0x80u

/* DiagnosticSessionControl */
#define kDiagSubDefaultSession                           0x01u    /**< Subservice ID - Default session */
#define kDiagSubProgrammingSession                       0x02u    /**< Subservice ID - Programming session */
#define kDiagSubExtendedDiagSession                      0x03u    /**< Subservice ID - Extended session */

/* TesterPresent */
#define kDiagSubTesterPresent                            0x00u    /**< Subservice ID - Zero subfunction */

/* ECUReset */
#define kDiagSubHardReset                                0x01u    /**< Subservice ID - Hard reset */
#define kDiagSubKeyOffOnReset                            0x02u    /**< Subservice ID - Key Off/On reset */
#define kDiagSubSoftReset                                0x03u    /**< Subservice ID - Soft reset */

/* ReadDTCInformation */
#define kDiagSubReportDTCByStatusMask                    0x02u    /**< Subservice ID - Report DTC by status mask */

/* CommunicationControl */
#define kDiagSubEnableRxAndTx                            0x00u    /**< ControlType - Enable Rx and Disable Tx */
#define kDiagSubEnableRxAndDisableTx                     0x01u    /**< ControlType - Enable Rx and Disable Tx */
#define kDiagSubDisableRxAndTx                           0x03u    /**< ControlType - Disable Rx and Tx */
#define kDiagSubNormalCommunication                      0x01u    /**< Communication type - normal communication */
#define kDiagSubNmCommunication                          0x02u    /**< Communication type - network management communication */
#define kDiagSubNmAndNormalCommunication                 0x03u    /**< Communication type - network management and normal communication */

/* RoutineControl, routineControlType */
#define kDiagSubStartRoutine                             0x01u
#define kDiagSubStopRoutine                              0x02u
#define kDiagSubRequestRoutineResults                    0x03u

/***********************************************************************************************************************
 *  Error handling
 **********************************************************************************************************************/

/* Negative Response Codes */
#define kDiagErrorNone                                   0x00u
#define kDiagNrcGeneralReject                            0x10u
#define kDiagNrcServiceNotSupported                      0x11u
#define kDiagNrcSubFunctionNotSupported                  0x12u
#define kDiagNrcIncorrectMessageLengthOrInvalidFormat    0x13u
#define kDiagNrcResponseTooLong                          0x14u
#define kDiagNrcBusyRepeatRequest                        0x21u
#define kDiagNrcConditionsNotCorrect                     0x22u
#define kDiagNrcRequestSequenceError                     0x24u
#define kDiagNrcRequestOutOfRange                        0x31u
#define kDiagNrcSecurityAccessDenied                     0x33u
#define kDiagNrcInvalidKey                               0x35u
#define kDiagNrcExceedNumberOfAttempts                   0x36u
#define kDiagNrcRequiredTimeDelayNotExpired              0x37u
#define kDiagNrcUploadDownloadNotAccepted                0x70u
#define kDiagNrcTransferDataSuspended                    0x71u
#define kDiagNrcGeneralProgrammingFailure                0x72u
#define kDiagNrcWrongBlockSequenceCounter                0x73u
#define kDiagNrcIllegalByteCountInBlockTransfer          0x75u
#define kDiagNrcRcrResponsePending                       0x78u
#define kDiagNrcSubfunctionNotSupportedInActiveSession   0x7Eu
#define kDiagNrcServiceNotSupportedInActiveSession       0x7Fu
#define kDiagNrcVoltageTooHigh                           0x92u
#define kDiagNrcVoltageTooLow                            0x93u

/* Diagnostic exception setting macros */
#define DiagNRCGeneralReject()                           FblDiagSetError(kDiagNrcGeneralReject)
#define DiagNRCServiceNotSupported()                     FblDiagSetError(kDiagNrcServiceNotSupported)
#define DiagNRCSubFunctionNotSupported()                 FblDiagSetError(kDiagNrcSubFunctionNotSupported)
#define DiagNRCIncorrectMessageLengthOrInvalidFormat()   FblDiagSetError(kDiagNrcIncorrectMessageLengthOrInvalidFormat)
#define DiagNRCResponseTooLong()                         FblDiagSetError(kDiagNrcResponseTooLong)
#define DiagNRCBusyRepeatRequest()                       FblDiagSetError(kDiagNrcBusyRepeatRequest)
#define DiagNRCConditionsNotCorrect()                    FblDiagSetError(kDiagNrcConditionsNotCorrect)
#define DiagNRCRequestSequenceError()                    FblDiagSetError(kDiagNrcRequestSequenceError)
#define DiagNRCRequestOutOfRange()                       FblDiagSetError(kDiagNrcRequestOutOfRange)
#define DiagNRCSecurityAccessDenied()                    FblDiagSetError(kDiagNrcSecurityAccessDenied)
#define DiagNRCInvalidKey()                              FblDiagSetError(kDiagNrcInvalidKey)
#define DiagNRCExceedNumberOfAttempts()                  FblDiagSetError(kDiagNrcExceedNumberOfAttempts)
#define DiagNRCRequiredTimeDelayNotExpired()             FblDiagSetError(kDiagNrcRequiredTimeDelayNotExpired)
#define DiagNRCUploadDownloadNotAccepted()               FblDiagSetError(kDiagNrcUploadDownloadNotAccepted)
#define DiagNRCTransferDataSuspended()                   FblDiagSetError(kDiagNrcTransferDataSuspended)
#define DiagNRCGeneralProgrammingFailure()               FblDiagSetError(kDiagNrcGeneralProgrammingFailure)
#define DiagNRCWrongBlockSequenceCounter()               FblDiagSetError(kDiagNrcWrongBlockSequenceCounter)
#define DiagNRCIllegalByteCountInBlockTransfer()         FblDiagSetError(kDiagNrcIllegalByteCountInBlockTransfer)
#define DiagNRCRcrResponsePending()                      FblDiagSetError(kDiagNrcRcrResponsePending)
#define DiagNRCSubfunctionNotSupportedInActiveSession()  FblDiagSetError(kDiagNrcSubfunctionNotSupportedInActiveSession)
#define DiagNRCServiceNotSupportedInActiveSession()      FblDiagSetError(kDiagNrcServiceNotSupportedInActiveSession)
#define DiagNRCVoltageTooHigh()                          FblDiagSetError(kDiagNrcVoltageTooHigh)
#define DiagNRCVoltageTooLow()                           FblDiagSetError(kDiagNrcVoltageTooLow)

/* Access macros for user diagnostic call-backs */
/* PRQA S 3453 2 */ /* MD_MSR_19.7 */
#define DiagPutResponseSId(a)    (DiagBuffer[0] = (a))
#define DiagGetRequestSId()      (DiagBuffer[0])

/***********************************************************************************************************************
 *  Data processing
 **********************************************************************************************************************/

#define kDiagSubNoEncryption              0x00u
#define kDiagSubNoCompression             0x00u
#define kDiagSubNoDataProcessing          ((kDiagSubNoCompression << 4u) | kDiagSubNoEncryption)

#define kDiagFmtEncryptionMask            0x0Fu
#define kDiagFmtCompressionMask           0xF0u

/* PRQA S 3453 2 */ /* MD_MSR_19.7 */
#define FblDiagGetCompressionMode(m)      (((m) & kDiagFmtCompressionMask) >> 4u)
#define FblDiagGetEncryptionMode(m)       ((m) & kDiagFmtEncryptionMask)

/***********************************************************************************************************************
 *  Response processing
 **********************************************************************************************************************/

/* Parameter to the function FblDiagEcuReset() */
#define kDiagResetPutResponse                0x00u
#define kDiagResetNoResponse                 0x01u

/* Parameters for busy response handling function */
#define kNotForceSendResponsePending         0x00u
#define kForceSendResponsePending            0x01u
#define kForceSendRpIfNotInProgress          0x02u

/* Sequence counter retry init number */
#if !defined( kDiagInitDataRetries )
# define kDiagInitDataRetries                0x03u
#endif

/* Read memory function for flash driver validation */
#define FblReadRam                           FblReadBlock

/***********************************************************************************************************************
 *  Timeout handling
 **********************************************************************************************************************/

/* Tester present timeout handling */
#if !defined( TESTER_PRESENT_TIMEOUT )
/** S3 timeout */
# define TESTER_PRESENT_TIMEOUT                 5000u
#endif

#define TimeoutTesterValue()                    testerPresentTimeout
#define DecTimeoutTesterValue()                 ( testerPresentTimeout-- )
#define ResetTesterTimeout()                    ( testerPresentTimeout = (vuint16)( TESTER_PRESENT_TIMEOUT / kDiagCallCycle ))
#define StopTesterTimeout()                     ( testerPresentTimeout = 0u )

#if defined( FBL_ENABLE_SLEEPMODE )
/* Sleep mode handling */
# define kFblSleepDelayTime                     ((vuint32)( FBL_SLEEP_TIME / FBL_REPEAT_CALL_CYCLE ))
# define FblSleepCounterClear()                 ( sleepCounter = 0x00u )
# define FblSleepCounterReload()                ( sleepCounter = kFblSleepDelayTime )
# define FblDecSleepCounter()                   ( sleepCounter-- )
# define FblGetSleepCounter()                   ((vuint32) sleepCounter )
#endif

/***********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

typedef tFblResult (*tFblDiagPreHandler )( vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen );
typedef tFblResult (*tFblDiagMainHandler)( vuint8 *pbDiagData, tCwDataLengthType diagReqDataLen );
typedef void       (*tFblDiagPostHandler)( vuint8 postParam );

typedef tFblAddress  tFblDiagAddr;
typedef vuint8       tFblDiagNrc;

/***********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

/* Initialization functions */
void        FblDiagInitPowerOn( void );
void        FblDiagInit( void );
#if defined( FBL_DEF_ENABLE_NON_KB_MAIN )
void        FblDiagPostInit( void );
#endif /* FBL_DEF_ENABLE_NON_KB_MAIN */
/* Error handling interface */
void        FblDiagSetError( vuint8 errorCode );
void        DiagProcessingDone( tCwDataLengthType dataLength );
/* Response pending handling functions */
vuint8      FblRealTimeSupport( void );
void        DiagExRCRResponsePending( vuint8 forceSend );
/* Diagnostic task functions */
void        FblDiagTimerTask( void );
void        FblDiagStateTask( void );
/* Diagnostic response after reset handling */
void        FblDiagInitStartFromAppl( void );

/* Memory handling functions */
vsint16     FblMemSegmentNrGet( tFblAddress address );
tFblLength  FblReadProm( tFblAddress address, vuint8 *buffer, tFblLength length );
V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * FblDiagMemGetActiveBuffer(void);

/* Callback functions from FblLib_Mem */
tFblResult  FblDiagMemPreWrite( void );
tFblResult  FblDiagMemPostWrite( void );

/* Start section to execute code from RAM */
#define FBLDIAG_RAMCODE_START_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* Communication wrapper API */
void        FblDiagRxStartIndication( void );
void        FblDiagRxIndication( vuint8* pbDiagBuffer, tCwDataLengthType rxDataLength );
vuint8*     FblDiagRxGetPhysBuffer( tCwDataLengthType rxDataLength );
vuint8*     FblDiagRxGetFuncBuffer( tCwDataLengthType rxDataLength );
void        FblDiagTxConfirmation( vuint8 cwMsgType );
void        FblDiagTxErrorIndication( vuint8 cwMsgType );
void        FblDiagRxErrorIndication( void );

/* End section to execute code from RAM */
#define FBLDIAG_RAMCODE_STOP_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/***********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

V_MEMRAM0 extern V_MEMRAM1       vuint8            V_MEMRAM2 V_MEMRAM3 * V_MEMRAM1 V_MEMRAM2 DiagBuffer;
V_MEMRAM0 extern V_MEMRAM1       vsint16           V_MEMRAM2      memSegment;
V_MEMRAM0 extern V_MEMRAM1_NEAR  tCwDataLengthType V_MEMRAM2_NEAR DiagDataLength;
V_MEMRAM0 extern V_MEMRAM1_NEAR  vuint16           V_MEMRAM2_NEAR testerPresentTimeout;
V_MEMRAM0 extern V_MEMRAM1_NEAR  vuint8            V_MEMRAM2_NEAR diagServiceCurrent;
V_MEMRAM0 extern V_MEMRAM1_NEAR  tFblStateBitmap   V_MEMRAM2_NEAR fblDiagStates[FBL_STATE_INDEX(kDiagNumberOfStates - 1) + 1];

#if defined( FBL_ENABLE_SLEEPMODE )
V_MEMRAM0 extern V_MEMRAM1       vuint32           V_MEMRAM2      sleepCounter;
#endif

/***********************************************************************************************************************
 *  CONFIGURATION CHECKS
 **********************************************************************************************************************/

#if !(defined(FBL_ENABLE_SYSTEM_CHECK) || defined(FBL_DISABLE_SYSTEM_CHECK))
# error "Error in fbl_cfg.h: Usage of system check not defined"
#endif

#if !(defined(FBL_ENABLE_DEBUG_STATUS) || defined(FBL_DISABLE_DEBUG_STATUS))
# error "Error in fbl_cfg.h: Usage of debug support not defined"
#endif

#if !(defined(FBL_ENABLE_APPL_TASK) || defined(FBL_DISABLE_APPL_TASK))
# error "Error in fbl_cfg.h: Usage of application task function not defined"
#endif

#if !(defined(FBL_ENABLE_USERSUBFUNCTION) || defined(FBL_DISABLE_USERSUBFUNCTION))
# error "Error in fbl_cfg.h: Usage of diag user-subfunction call not defined"
#endif

#if !(defined(FBL_ENABLE_DATA_PROCESSING) || defined(FBL_DISABLE_DATA_PROCESSING))
# error "Error in fbl_cfg.h: Usage of data processing interface not defined"
#endif

#if !(defined(FBL_WATCHDOG_ON) || defined(FBL_WATCHDOG_OFF))
# error "Error in fbl_cfg.h: Usage of watchdog support not defined"
#endif

#if defined( FBL_WATCHDOG_ON) && !defined(FBL_WATCHDOG_TIME )
# error "Error in fbl_cfg.h: Watchdog trigger period not defined"
#endif

#if !(defined(FBL_ENABLE_FBL_START) || defined(FBL_DISABLE_FBL_START))
# error "Error in fbl_cfg.h: Usage of FblStart function not defined"
#endif

#if defined ( FBL_DIAG_ENABLE_CORE_SEGMENTHANDLING )
# if !defined(SWM_DATA_MAX_NOAR)
#  error "Error in fbl_cfg.h: Number of supported download segments (SWM_DATA_MAX_NOAR) not defined"
# endif
#endif

#if !defined(FBL_HEADER_ADDRESS)
# error "Error in fbl_cfg.h: Start address of bootblock header not defined"
#endif

#if !defined(FBL_DIAG_BUFFER_LENGTH)
# error "Error in fbl_cfg.h: FBL_DIAG_BUFFER_LENGTH not defined"
#endif

#if ( FBL_DIAG_BUFFER_LENGTH < (FBL_MAX_SEGMENT_SIZE+2) )
# error "Error in fbl_cfg.: DiagBuffer size too small - modify FBL_DIAG_BUFFER_LENGTH"
#endif

#if defined( SEC_SECURITY_CLASS )
# if ( SEC_SECURITY_CLASS == SEC_CLASS_C ) || ( SEC_SECURITY_CLASS == SEC_CLASS_CCC )
#  if ( FBL_DIAG_BUFFER_LENGTH < (kSecSigLength+4) )
#   error "Error in fbl_cfg.h: DiagBuffer size too small - modify FBL_DIAG_BUFFER_LENGTH"
#  endif
# endif
#endif

#if defined( FBL_DIAG_ENABLE_OEM_SEGMENTNRGET) || \
    defined( FBL_DIAG_ENABLE_OEM_READPROM )
   /* Currently, these switches must be used together */
# if defined( FBL_DIAG_ENABLE_OEM_SEGMENTNRGET) && \
     defined( FBL_DIAG_ENABLE_OEM_READPROM )
# else
#  error "FBL_DIAG_ENABLE_OEM_SEGMENTNRGET and FBL_DIAG_ENABLE_OEM_READPROM must be used together."
# endif
#endif

#if !defined(FBL_DIAG_TIME_P3MAX)
# error "Error in fbl_cfg.h: P2Star time not defined"
#endif

#if !defined(FLASH_SIZE)
# error "Error in fbl_cfg.h: Size of flash code buffer not defined"
#endif

#endif /* __FBL_DIAG_H__ */

/***********************************************************************************************************************
 *  END OF FILE: FBL_DIAG.H
 **********************************************************************************************************************/
