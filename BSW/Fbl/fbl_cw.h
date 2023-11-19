/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief         Communication wrapper layer for CAN
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
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  02.00.00   2016-06-24  Ach     ESCAN00080145    Wrong precopy functions are called for several configurations
 *                                 ESCAN00084071    Rework CAN ID configuration
 *  02.01.00   2016-07-25  Ach     ESCAN00090777    Added sleep mode defines kFblCanSleep*
 *  02.01.81   2016-12-13  AWh     ESCAN00093277    No changes
 **********************************************************************************************************************/

#ifndef __FBL_CW_H__
#define __FBL_CW_H__

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
#include "fbl_cw_cfg.h"

/***********************************************************************************************************************
 *  VERSION
 **********************************************************************************************************************/

/* ##V_CFG_MANAGEMENT ##CQProject : FblWrapperCom_Can CQComponent : Implementation */
#define FBLWRAPPERCOM_CAN_VERSION            0x0201u
#define FBLWRAPPERCOM_CAN_RELEASE_VERSION    0x81u

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

#include "fbl_hw.h"
#include "ftp_cfg.h"
#include "fbl_tp.h"

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

/* Default value for confirmation timeout */
#if defined( kFblCwTxPendingTimeout )
#else
# define kFblCwTxPendingTimeout           (100u / FBL_REPEAT_CALL_CYCLE)
#endif
#if defined( kFblCwMaxWaitTime )
#else
# define kFblCwMaxWaitTime                (10u / FBL_REPEAT_CALL_CYCLE)
#endif

/* Flag if a call to Timer or State Task is currently allowed */
/* This is used e.g. to prevent a call to the tasks if a      */
/* flash operation is currently in progress and the tasks     */
/* cannot be called, because they are complex and in ROM.     */

/* The following ifdef depends on the bus driver if it can/must be in RAM or not. */
#if defined( FBL_CW_ENABLE_TASK_CODE_IN_RAM )
/* All Task code is in RAM */
# define FblCwIsTimerTaskAllowed()        0x01u
# define FblCwIsStateTaskAllowed()        0x01u
#else
/* Task code is in ROM. Don't call this, if erase or write operation is in progress */
# define FblCwIsTimerTaskAllowed()        ((vuint8)(memDrvDeviceActive == kMioDeviceIdle))
# define FblCwIsStateTaskAllowed()        ((vuint8)(memDrvDeviceActive == kMioDeviceIdle))
#endif

/** This define indicates task locks are handled by ComWrapper */
#define FBLCW_LOCKS_TASKS

/* Call parameter type for reset cases */
/** This define will be used in case of regular ECU reset e.g. after 11 01 */
#define kFblCwResetEcuRegularCase         0x00u
/** This define will be used in case of non-regular reset of the diagnostic e.g.: Timeout, EcuReset with SuppressPosRsp */
#define kFblCwResetEcuTesterTimeout       0x01u

/* Macros for compatibility */
#define FblCwSetOfflineMode()
#define FblCwSetOnlineMode()

/* Re-Map CwTransmit to CwCanTransmit, because response type is not required here */
#define FblCwTransmit(a, b, c)            FblCwCanTransmit((a), (b)) /* PRQA S 3453 */ /* MD_MSR_19.7 */

# define FblCwRetransmitRP()              FblCanRetransmit()

/* Shutdown request from Diag-layer to Com-Wrapper */
/* Within CAN, a shutdown can be done immediately. */
/* No wait time from bus system necessary.         */
#define FblCwShutdownRequest(a)           FblSetShutdownRequest() /* PRQA S 3453 */ /* MD_MSR_19.7 */

/* Parameter CallType for FblCwTransmit */
#define kFblCwTxCallPosResponse           0x00u
#define kFblCwTxCallNegResponse           0x01u
#define kFblCwTxCallFailedResponse        0x02u
#define kFblCwTxCallRcrRp                 0x03u
#define kFblCwTxCallNoResponse            0x04u

/* Parameter for FblDiagConfirmation and FblDiagTxErrorIndication */
#define kFblCwMsgTypeNormal               0x00u
#define kFblCwMsgTypeRcrRp                0x03u

/* Length of the RCRP messages */
#define kFblCwRcrRpMsgLength              0x03u

/* Macros for communication wrapper layer initialization */
#define FblCwGetInit()                    (fblCwInitFlag == 0x01u)
#define FblCwSetInit()                    (fblCwInitFlag = 0x01u)
#define FblCwClrInit()                    (fblCwInitFlag = 0x00u)

/***********************************************************************************************************************
 *  ADDRESS HANDLING DEFINES
 **********************************************************************************************************************/
#if defined( FBL_ENABLE_NORMAL_FIXED_ADDRESSING )
/* PRQA S 3453 3 */ /* MD_MSR_19.7 */
# define FblCwGetSourceAddress(id)        ((vuint8)((id) & 0xFFu))               /**< Extract source address from ID */
# define FblCwGetTargetAddress(id)        ((vuint8)(((id) >> 8u) & 0xFFu))       /**< Extract target address from ID */
# define FblCwTxSetTargetAddress(target)  FblCanSetTxId(fblCanTxObj[fblCwDiagTransmitObject], (MK_EXTID(GET_EXTID_RAW(fblCwTxId[fblCwDiagTransmitObject]) | (((vuint32)(target)) << 8u))))
#else
# define FblCwTxSetTargetAddress(targetAddress)
#endif /* FBL_ENABLE_NORMAL_FIXED_ADDRESSING */
#define FblCwResetResponseAddress()

#if defined( FBL_ENABLE_STAY_IN_BOOT ) 
/* Macros for start message window */
# define kFblCwCheckForStartMsg           0x01u
# define FblCwGetCheckForStartMsg()       (fblCwStateStartMsg == kFblCwCheckForStartMsg)
# define FblCwSetCheckForStartMsg()       (fblCwStateStartMsg = kFblCwCheckForStartMsg)
# define FblCwClrCheckForStartMsg()       (fblCwStateStartMsg = 0u)
#endif

/* FblCanTransmit() and FblCanMsgTransmitted() return values */
#define kFblCanTxOk                 0u  /* FblCanTransmit()   : Transmission request accepted. */
                                        /* FblCanTransmitted(): Transmission now completed.    */
#define kFblCanTxFailed             1u  /* FblCanTransmit()   : Transmission request failed.   */
                                        /* FblCanTransmitted(): No transmission in progress    */
#define kFblCanTxInProgress         2u  /* FblCanTransmit()   : Not allowed                    */
                                        /* FblCanTransmitted(): Transmission still in progress */

/* Default values for CAN configuration table. Will be overwritten during FblCwInit */
#define FBL_CAN_RX_ID0 0u
#define FBL_CAN_RX_ID1 0u
#define FBL_CAN_RX_ID2 0u
#define FBL_CAN_RX_ID3 0u
#define FBL_CAN_RX_ID4 0u
#define FBL_CAN_CODE_0 0u
#define FBL_CAN_CODE_1 0u

/* Compatibility defines */
#define kTpTxObj              fblCwDiagTransmitObject
#define tpCanTxData           fblCanTxObj[fblCwDiagTransmitObject].msgObject.DataFld
#define kFblTpTxHandle        (&(fblCanTxObj[fblCwDiagTransmitObject]))

/* FblCanSleep() return values */
#define kFblCanSleepOk              0u
#define kFblCanSleepFailed          1u
#define kFblCanSleepNotSupported    2u

/***********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

typedef tTpDataType tCwDataLengthType;

/***********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

#define FBL_CW_FULL_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
void FblCwInit( void );
void FblCwDeinit( void );
void FblCwSetTxInit( void );
void FblCwIdleTask( void );

#define FBL_CW_TASK_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
void V_API_NEAR FblCwTimerTask( void );
void V_API_NEAR FblCwStateTask( void );
#define FBL_CW_TASK_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

void FblCwCanTransmit( const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * data, tCwDataLengthType length );
void FblCwTransmitRP( const vuint8 * data );
void FblCwDiscardReception( void );
void FblCwSetRxBlock( void );
void FblCwResetRxBlock( void );
#if defined( FBL_ENABLE_STAY_IN_BOOT ) 
tFblResult FblCwCheckStartMessage( void );
# if defined( MULTIPLE_RECEIVE_BUFFER )
vuint8 FblCheckOwnStartMsg( pChipDataPtr data );
# else
#  error "Unsupported receive buffer configuration selected."
# endif
#endif

void FblCwTpDataInd( tCwDataLengthType rxDataLen );

void FblCwTpConfirmation( vuint8 state );
void FblCwTpRxStartIndication( void );
void FblCwTpErrorIndication( vuint8 errorCode );
#if defined( MULTIPLE_RECEIVE_BUFFER )
void FblCwPrecopy( pChipDataPtr data );
#endif

#if defined( MULTIPLE_RECEIVE_BUFFER )
void FblHandleRxMsg( vuint8 fblRxCanMsgHdl, pChipDataPtr canDataPtr );
#endif

#if defined( FBL_ENABLE_NORMAL_FIXED_ADDRESSING )
tFblResult FblCwPrepareResponseAddress( void );
tFblResult FblCwSaveResponseAddress( void );
#else /* ! FBL_ENABLE_NORMAL_FIXED_ADDRESSING */
# define FblCwPrepareResponseAddress() kFblOk
# define FblCwSaveResponseAddress() kFblOk
#endif /* ! FBL_ENABLE_NORMAL_FIXED_ADDRESSING */

# if defined( FBL_CW_ENABLE_PHYSICAL_REQUEST_HANDLER )
void FblCwProcessPhysicalRequest(vuint8 fblRxCanMsgHdl, pChipDataPtr canDataPtr);
# endif /* FBL_CW_ENABLE_PHYSICAL_REQUEST_HANDLER */
# if defined( FBL_CW_ENABLE_PHYSICAL_RANGE_HANDLER )
void FblCwProcessPhysicalRangeRequest(vuint8 fblRxCanMsgHdl, pChipDataPtr canDataPtr);
# endif /* FBL_CW_ENABLE_PHYSICAL_RANGE_HANDLER */
# if defined( FBL_CW_ENABLE_FUNCTIONAL_REQUEST_HANDLER )
void FblCwProcessFunctionalRequest(vuint8 fblRxCanMsgHdl, pChipDataPtr canDataPtr);
# endif /* FBL_CW_ENABLE_FUNCTIONAL_REQUEST_HANDLER */
# if defined( FBL_CW_ENABLE_FUNCTIONAL_RANGE_HANDLER )
void FblCwProcessFunctionalRangeRequest(vuint8 fblRxCanMsgHdl, pChipDataPtr canDataPtr);
# endif /* FBL_CW_ENABLE_FUNCTIONAL_REQUEST_HANDLER */

#define FBL_CW_FULL_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/***********************************************************************************************************************
 *  GLOBAL DATA DECLARATIONS
 **********************************************************************************************************************/

V_MEMRAM0 extern V_MEMRAM1_NEAR tMsgObject V_MEMRAM2_NEAR fblCanTxObj[FBL_NUMBER_OF_TX_ID];
V_MEMRAM0 extern V_MEMRAM1 tFblCanIdTable V_MEMRAM2 fblCanIdTable;
V_MEMRAM0 extern V_MEMRAM1 vuint8 V_MEMRAM2 fblCwInitFlag;

#if defined( FBL_ENABLE_NORMAL_FIXED_ADDRESSING )
V_MEMRAM0 extern V_MEMRAM1 vuint32 V_MEMRAM2 cwTpReceiveAddress;
V_MEMRAM0 extern V_MEMRAM1 vuint32 V_MEMRAM2 fblCwTxId[FBL_NUMBER_OF_TX_ID];
#endif /* FBL_ENABLE_NORMAL_FIXED_ADDRESSING */

V_MEMRAM0 extern V_MEMRAM1 vuintx V_MEMRAM2 fblCwDiagTransmitObject;

#if defined( FBL_ENABLE_STAY_IN_BOOT ) 
V_MEMRAM0 extern V_MEMRAM1 vuint8 V_MEMRAM2 fblCwStateStartMsg;
#endif

/***********************************************************************************************************************
 *  CONFIGURATION CHECKS
 **********************************************************************************************************************/

#if ( FBL_DIAG_BUFFER_LENGTH > 4095u )
# error "Error in FBL_CFG.H: DiagBuffer size larger than supported by TP"
#endif

#if ! defined( FBL_TP_ENABLE_CONFIRMATION_INTERRUPT ) || \
    ! defined( FBL_ENABLE_CAN_CONFIRMATION )
# error "Error in fbl_cfg.h/fbl_tp.h: Unsupported confirmation handling"
#endif

#endif /* __FBL_CW_H__ */

/***********************************************************************************************************************
 *  END OF FILE: FBL_CW.H
 **********************************************************************************************************************/
