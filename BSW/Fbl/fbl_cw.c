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
 *  02.01.00   2016-07-25  Ach     ESCAN00090777    No changes
 *  02.01.81   2016-12-13  AWh     ESCAN00093277    Normal Fixed Addr with 11 Bit Rx: Differentiate CFand response addr
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

#include "fbl_inc.h"

/***********************************************************************************************************************
 *  VERSION
 **********************************************************************************************************************/

#if ( FBLWRAPPERCOM_CAN_VERSION != 0x0201u ) || \
    ( FBLWRAPPERCOM_CAN_RELEASE_VERSION != 0x81u )
# error "Error in fbl_cw.c: Source and header file are inconsistent!"
#endif

#if ( FBLWRAPPERCOM_CAN_VERSION != _FBLWRAPPERCOM_CAN_VERSION ) || \
    ( FBLWRAPPERCOM_CAN_RELEASE_VERSION != _FBLWRAPPERCOM_CAN_RELEASE_VERSION)
# error "Error in fbl_cw.c: Source and v_ver.h are inconsistent!"
#endif

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

#ifndef FBLDRVCAN_REFERENCE_IMPLEMENTATION
# define FBLDRVCAN_REFERENCE_IMPLEMENTATION  0x0100
#endif

/* To access the AllNode request databuffer (extended-addressing)   */
/* CAN Hardware access support for diagnostic                       */

#if defined( MULTIPLE_RECEIVE_BUFFER )
# define CANTPMSG_TPCI                    (data[0])
# define CANTPMSG_SID                     (data[1])
# define CANTPMSG_PARAM                   (data[2])

# define pCanData                         data
#endif

/* Defines for transport layer data buffer access, same defines also in FBL_TP.C */
#define kTpciPos                          0x00u
#define kSFDataPos                        0x01u

/* Defines to indicate the transmission state */
#define kFblCwTxStateIdle                 0x00u    /**< Idle state */
#define kFblCwTxStateRpInProgress         0x01u    /**< RP was forwarded, wait for the confirmation */
#define kFblCwTxStateTxInProgress         0x02u    /**< Final answer was sent, wait for the confirmation */
#define kFblCwTxStateRpPending            0x03u    /**< RP was not sent, try to send it again until timeout */
#define kFblCwTxStateTxPending            0x04u    /**< Final answer was not sent, try to send it again until timeout */

#define kFblCwCanRxMsgStatePhysical       0x00u
#define kFblCwCanRxMsgStateFunctional     0x01u
#define kFblCwCanRxMsgStateDiscard        0x02u

/* Flags indicate, which task is currently running */
#define kFblCwStateTaskRunning            FBL_BIT0
#define kFblCwTimerTaskRunning            FBL_BIT1

/* Flags used to control the call to the CW layer */
#define SetCwStateTaskRunning()           (cwTaskState |= kFblCwStateTaskRunning)
#define ClrCwStateTaskRunning()           (cwTaskState &= FblInvert8Bit(kFblCwStateTaskRunning))
#define GetCwStateTaskRunning()           ((cwTaskState & kFblCwStateTaskRunning) == kFblCwStateTaskRunning)

#define SetCwTimerTaskRunning()           (cwTaskState |= kFblCwTimerTaskRunning)
#define ClrCwTimerTaskRunning()           (cwTaskState &= FblInvert8Bit(kFblCwTimerTaskRunning))
#define GetCwTimerTaskRunning()           ((cwTaskState & kFblCwTimerTaskRunning) == kFblCwTimerTaskRunning)

/***********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

/** Communication wrapper initialized flag */
V_MEMRAM0 V_MEMRAM1 vuint8 V_MEMRAM2 fblCwInitFlag;

#if defined( FBL_ENABLE_NORMAL_FIXED_ADDRESSING )
/** CAN ID of received TP message */
V_MEMRAM0 V_MEMRAM1 vuint32 V_MEMRAM2 cwTpReceiveAddress;
/** Storage of base ID for dynamically defined TX IDs. Used to calculate dyncamic TX ID. */
V_MEMRAM0 V_MEMRAM1 vuint32 V_MEMRAM2 fblCwTxId[FBL_NUMBER_OF_TX_ID];
#endif /* FBL_ENABLE_NORMAL_FIXED_ADDRESSING */

/** CAN initialization structure */
V_MEMROM0 extern V_MEMROM1 tFblCanIdTable V_MEMROM2 kFblCanIdTable; /* PRQA S 3447, 3451 */ /* MD_FblCw_3447, MD_FblCw_3451 */
/** RAM copy of CAN initialization structure */
V_MEMRAM0 V_MEMRAM1 tFblCanIdTable V_MEMRAM2 fblCanIdTable;
/** CAN transmit buffers. */
V_MEMRAM0 V_MEMRAM1_NEAR tMsgObject V_MEMRAM2_NEAR fblCanTxObj[FBL_NUMBER_OF_TX_ID];
/** Diagnostic transmit object number (RAM). Used as index into fblCanTxObj list. */
V_MEMRAM0 V_MEMRAM1 vuintx V_MEMRAM2 fblCwDiagTransmitObject;

#if defined( FBL_ENABLE_STAY_IN_BOOT ) 
V_MEMRAM0 V_MEMRAM1 vuint8 V_MEMRAM2 fblCwStateStartMsg;
#endif

/***********************************************************************************************************************
 *  LOCAL DATA
 **********************************************************************************************************************/

#if ( TpCallCycle > 1 )
V_MEMRAM0 static V_MEMRAM1 vuint8  V_MEMRAM2 tpCycleCounter;
#endif
V_MEMRAM0 static V_MEMRAM1 vuint8  V_MEMRAM2 cwTxState;
V_MEMRAM0 static V_MEMRAM1 vuint16 V_MEMRAM2 cwTxTimer;
V_MEMRAM0 static V_MEMRAM1 tCwDataLengthType V_MEMRAM2 cwTxPendingLength;
V_MEMRAM0 static V_MEMRAM1 vuint8  V_MEMRAM2 cwCanRxMsgState;     /* 0 = Physical; 1 = Functional */

/* Variable tracks which tasks are running */
V_MEMRAM0 static V_MEMRAM1 vuint8  V_MEMRAM2 cwTaskState;

#if defined( FBL_CW_ENABLE_NORMAL_FIXED_ALLOWING_11BIT_RX )
V_MEMRAM0 static V_MEMRAM1 vuint8 V_MEMRAM2 fblCwStateNFix11BitReceived;
#endif /* FBL_CW_ENABLE_NORMAL_FIXED_ALLOWING_11BIT_RX */

#define FBL_CW_FULL_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/***********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

static void FblCwInitRxIdentifier(void);

/***********************************************************************************************************************
 *  USE-CASE SPECIFIC MESSAGE HANDLING FUNCTIONS
 **********************************************************************************************************************/

#if defined( FBL_CW_ENABLE_PHYSICAL_REQUEST_HANDLER )
/***********************************************************************************************************************
 *  FblCwProcessPhysicalRequest
 **********************************************************************************************************************/
/*! \brief       Physical request handling function (Normal Addressing)
 *  \pre         Physical request has been received
 *  \param[in]   fblRxCanMsgHdl RX CAN message handle
 *  \param[in]   canDataPtr In case of multiple receive buffer configuration: Pointer to CAN object.
 **********************************************************************************************************************/
/* PRQA S 3206 1 */ /* MD_FblCw_3206 */
void FblCwProcessPhysicalRequest(vuint8 fblRxCanMsgHdl, pChipDataPtr canDataPtr)
{
   {
      /* Discard messages if a diagnostic response is currently processed */
      if (kFblCwCanRxMsgStateDiscard != cwCanRxMsgState)
      {
# if defined( FBL_CW_ENABLE_NORMAL_FIXED_ALLOWING_11BIT_RX )
         /* FBL_CW_FIXED_CF_TARGET_ADDR below will be overwritten for Final response in FblCwTpDataInd */
         FblCwTxSetTargetAddress(FBL_CW_FIXED_FLOWCONTROL_TARGET_ADDR);
         fblCwStateNFix11BitReceived = 1u;
# endif /* FBL_CW_ENABLE_NORMAL_FIXED_ALLOWING_11BIT_RX */
         cwCanRxMsgState = kFblCwCanRxMsgStatePhysical;
         (void)FblTpPrecopy(canDataPtr);
      }
   }
}
#endif /* FBL_CW_ENABLE_PHYSICAL_REQUEST_HANDLER */

#if defined( FBL_CW_ENABLE_PHYSICAL_RANGE_HANDLER )
/***********************************************************************************************************************
 *  FblCwProcessPhysicalRangeRequest
 **********************************************************************************************************************/
/*! \brief       Physical request handling function (Addressing modes with range handling)
 *  \pre         Physical request has been received
 *  \param[in]   fblRxCanMsgHdl RX CAN message handle
 *  \param[in]   canDataPtr In case of multiple receive buffer configuration: Pointer to CAN object.
 **********************************************************************************************************************/
/* PRQA S 3206 1 */ /* MD_FblCw_3206 */
void FblCwProcessPhysicalRangeRequest(vuint8 fblRxCanMsgHdl, pChipDataPtr canDataPtr)
{
# if defined( FBL_ENABLE_NORMAL_FIXED_ADDRESSING )
   /* Discard messages if a diagnostic response is currently processed */
   if (kFblCwCanRxMsgStateDiscard != cwCanRxMsgState)
   {
      if (ApplFblCheckTesterSourceAddr(FblCwGetSourceAddress(CanRxActualId)) == kFblOk)
      {
         /* Prepare response ID */
         cwTpReceiveAddress = CanRxActualId;
         FblCwTxSetTargetAddress(FblCwGetSourceAddress(CanRxActualId));

         cwCanRxMsgState = kFblCwCanRxMsgStatePhysical;
         (void)FblTpPrecopy(canDataPtr);
      }
   }
# endif /* FBL_ENABLE_NORMAL_FIXED_ADDRESSING */
}
#endif /* FBL_CW_ENABLE_PHYSICAL_RANGE_HANDLER */

#if defined( FBL_CW_ENABLE_FUNCTIONAL_REQUEST_HANDLER )
/***********************************************************************************************************************
 *  FblCwProcessFunctionalRequest
 **********************************************************************************************************************/
/*! \brief       Functional request handling function (Normal Addressing)
 *  \pre         Functional request has been received
 *  \param[in]   fblRxCanMsgHdl RX CAN message handle
 *  \param[in]   canDataPtr In case of multiple receive buffer configuration: Pointer to CAN object.
 **********************************************************************************************************************/
/* PRQA S 3206 1 */ /* MD_FblCw_3206 */
void FblCwProcessFunctionalRequest(vuint8 fblRxCanMsgHdl, pChipDataPtr canDataPtr)
{
   /* Discard messages if a diagnostic response is currently processed */
   if (kFblCwCanRxMsgStateDiscard != cwCanRxMsgState)
   {
      /* cwCanRxMsgState will be set in FblCwPrecopy if the functional request is accepted */
      FblCwPrecopy(canDataPtr);
   }
}
#endif /* FBL_CW_ENABLE_FUNCTIONAL_REQUEST_HANDLER */

#if defined( FBL_CW_ENABLE_FUNCTIONAL_RANGE_HANDLER )
/***********************************************************************************************************************
 *  FblCwProcessFunctionalRangeRequest
 **********************************************************************************************************************/
/*! \brief       Functional request handling function (Addressing modes with range handling)
 *  \pre         Functional request has been received
 *  \param[in]   fblRxCanMsgHdl RX CAN message handle
 *  \param[in]   canDataPtr In case of multiple receive buffer configuration: Pointer to CAN object.
 **********************************************************************************************************************/
/* PRQA S 3206 1 */ /* MD_FblCw_3206 */
void FblCwProcessFunctionalRangeRequest(vuint8 fblRxCanMsgHdl, pChipDataPtr canDataPtr)
{
# if defined( FBL_ENABLE_NORMAL_FIXED_ADDRESSING )
         if (kFblCwCanRxMsgStateDiscard != cwCanRxMsgState)
         {
            if ((ApplFblCheckTesterSourceAddr(FblCwGetSourceAddress(CanRxActualId)) == kFblOk)
#  if defined( FBL_CW_ENABLE_FUNC_ID_CHECK )
                 && (ApplFblCheckFuncTargetAddr(FblCwGetTargetAddress(CanRxActualId)) != kFblOk)
#  endif /* FBL_ENABLE_FUND_ID_CHECK */
               )
            {
               /* Prepare response ID*/
               cwTpReceiveAddress = CanRxActualId;
               FblCwTxSetTargetAddress(FblCwGetSourceAddress(CanRxActualId));

               /* cwCanRxMsgState will be set in FblCwPrecopy if the functional request is accepted */
               FblCwPrecopy(canDataPtr);
            }
         }
# endif /* FBL_ENABLE_NORMAL_FIXED_ADDRESSING */
}
#endif /* FBL_CW_ENABLE_FUNCTIONAL_RANGE_HANDLER */

/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  FblCwTransmitRP
 **********************************************************************************************************************/
/*! \brief       Transmit given data buffer with RCR-RP
 *  \pre         CW is initialized
 *  \param[in]   data Pointer to the data to be transmitted
 **********************************************************************************************************************/
void FblCwTransmitRP( const vuint8 * data )
{
   /* Transmission if ComWrapper is idle */
   if (cwTxState == kFblCwTxStateIdle)
   {
      tpCanTxData[kTpciPos]     = kFblCwRcrRpMsgLength;
      tpCanTxData[kSFDataPos]   = data[0];
      tpCanTxData[kSFDataPos+1] = data[1];
      tpCanTxData[kSFDataPos+2] = data[2];
#if defined( FBL_TP_ENABLE_TX_FRAME_PADDING )
      tpCanTxData[kSFDataPos+3] = kFblTpFillPattern;
      tpCanTxData[kSFDataPos+4] = kFblTpFillPattern;
      tpCanTxData[kSFDataPos+5] = kFblTpFillPattern;
      tpCanTxData[kSFDataPos+6] = kFblTpFillPattern;
#endif
#if defined( FBL_TP_ENABLE_VARIABLE_TX_DLC )
      /* Set DLC value for next TP message */
      FblCanSetTxDLC(fblCanTxObj[fblCwDiagTransmitObject], 4u);
#endif

      /* To dispatch confirmation */
      cwTxState = kFblCwTxStateRpInProgress;
      if (FblTpTransmitSF(kFblCwRcrRpMsgLength) != kTpSuccess)
      {
         /* Transmission request failed. Retry in timer task, observed by timeout */
         cwTxTimer = kFblCwTxPendingTimeout;
         cwTxState = kFblCwTxStateRpPending;

         /* Store request length for retransmission */
         cwTxPendingLength = (tCwDataLengthType)kFblCwRcrRpMsgLength;
      }
   }
}

#define FBL_CW_TASK_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/***********************************************************************************************************************
 *  FblCwTimerTask
 **********************************************************************************************************************/
/*! \brief       Communication wrapper timer task function
 *  \pre         CW is initialized.
 **********************************************************************************************************************/
void V_API_NEAR FblCwTimerTask( void )
{
   /* Check if communication wrapper already initialized */
   if (FblCwGetInit())
   {
      if (!GetCwTimerTaskRunning())
      {
         SetCwTimerTaskRunning();

#if ( TpCallCycle > 1 )
         tpCycleCounter++;
         if (tpCycleCounter >= TpCallCycle)
         {
#endif
            /* Call transport layer cyclic function */
            FblTpTask();
#if ( TpCallCycle > 1 )
            tpCycleCounter = 0u;
         }
#endif
         /* Check for pending operation */
         if (cwTxTimer != 0u)
         {
            /* decrement timer */
            cwTxTimer--;

            /* Evaluation of the txState only necessary if the timer is running */
            switch (cwTxState)
            {
               case kFblCwTxStateIdle:         /* Intentional fall-through */
               case kFblCwTxStateRpInProgress: /* Intentional fall-through */
               case kFblCwTxStateTxInProgress:
               {
                  /* Timer is irrelevant in these states */
                  cwTxTimer = 0u;
                  break;
               }
               case kFblCwTxStateRpPending:
               {
                  /* Check if the timer is expired */
                  if (cwTxTimer == 0u)
                  {
                     cwTxState = kFblCwTxStateIdle;
                     FblDiagTxErrorIndication(kFblCwMsgTypeRcrRp);
                  }
                  else
                  {
                     /* Change state before call to TpTransmit because indication may be done within the function */
                     cwTxState = kFblCwTxStateRpInProgress;
                     if (FblTpTransmitSF((vuint8)cwTxPendingLength) != kTpSuccess)
                     {
                        /* Revert to previous state */
                        cwTxState = kFblCwTxStateRpPending;
                     }
                     else
                     {
                        /* Now successful. Stop the timer */
                        cwTxTimer = 0u;
                     }
                  }
                  break;
               }
               case kFblCwTxStateTxPending:
               {
                  /* Check if the timer is expired */
                  if (cwTxTimer == 0u)
                  {
                     cwTxState = kFblCwTxStateIdle;
                     FblDiagTxErrorIndication(kFblCwMsgTypeNormal);
                  }
                  else
                  {
                     /* Change state before call to TpTransmit             */
                     /* because indication may be done within the function */
                     cwTxState = kFblCwTxStateTxInProgress;
                     if (FblTpTransmit(cwTxPendingLength) != kTpSuccess)
                     {
                        /* revert to previous state */
                        cwTxState = kFblCwTxStateTxPending;
                     }
                     else
                     {
                        /* Now successful. Stop the timer */
                        cwTxTimer = 0u;
                     }
                  }
                  break;
               }
               default:
               {
                  /* Illegal state */
                  cwTxState = kFblCwTxStateIdle;
                  cwTxTimer = 0u;
                  break;
               }
            }
         }

         ClrCwTimerTaskRunning(); /* PRQA S 0277 */ /* MD_FblCw_0277 */
      }
   }
} /* PRQA S 6030, 6080 */ /* MD_MSR_STCYC, MD_MSR_STMIF */

/***********************************************************************************************************************
 *  FblCwStateTask
 **********************************************************************************************************************/
/*! \brief       Polling of CAN receive messages and CAN message confirmation
 *  \pre         CW is initialized
 **********************************************************************************************************************/
void V_API_NEAR FblCwStateTask( void )
{
   if (FblCwGetInit())
   {
      if (!GetCwStateTaskRunning())
      {
         SetCwStateTaskRunning();

         (void)FblCanMsgTransmitted();

# if defined( FBL_CW_ENABLE_TASK_CODE_IN_RAM ) || \
     defined( FBL_CW_ENABLE_RECEPTION_IN_STATE_TASK )
         /* Polling of CAN receive messages */
         if (FblCanReceive() != 0u) /* PRQA S 0303 */ /* MD_FblCw_0303 */
         {
            /*-----------------------------------------------------------------*/
            /*--- CAN messages has been received. Check for new data.          */
            /*-----------------------------------------------------------------*/
            /* Process the RxMsg if the ID is valid                            */
            /* and call FblCanHandleRxMsg with a handle                        */
#  if defined( FBL_ENABLE_STAY_IN_BOOT ) 
            /* In case of stay-in-boot or multiple platforms, the message is   */
            /* processed in function FblCwCheckStartMessage()                  */
            if (!FblCwGetCheckForStartMsg())
#  endif
            {
               FblCanProcessRxMsg();
            }
         }
# endif /* FBL_CW_ENABLE_TASK_CODE_IN_RAM || FBL_CW_ENABLE_RECEPTION_IN_STATE_TASK */
         ClrCwStateTaskRunning(); /* PRQA S 0277 */ /* MD_FblCw_0277 */
      }
   }
}

# define FBL_CW_TASK_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/***********************************************************************************************************************
 *  FblCwIdleTask
 **********************************************************************************************************************/
/*! \brief       Communication wrapper idle task function
 *  \details     The function polls the CAN, it also executes some pending actions, which could not fulfilled in the
 *               event function.
 *               Note, that the order of polling is a design decision: Error-Task, TX-Task, RX-Task.
 *  \pre         CW is initialized
 **********************************************************************************************************************/
void FblCwIdleTask( void )
{
   /* Check for CAN errors */
   FblCanErrorTask();

#if defined( FBL_CW_ENABLE_TASK_CODE_IN_RAM ) || \
    defined( FBL_CW_ENABLE_RECEPTION_IN_STATE_TASK )
#else
   /* Polling of CAN receive messages */
   if (FblCanReceive() != 0u) /* PRQA S 0303 */ /* MD_FblCw_0303 */
   {
      /*-----------------------------------------------------------------*/
      /*--- CAN messages has been received. Check for new data.          */
      /*-----------------------------------------------------------------*/
      /* Process the RxMsg if the ID is valid                            */
      /* and call FblCanHandleRxMsg with a handle                        */
# if defined( FBL_ENABLE_STAY_IN_BOOT ) 
      /* In case of stay-in-boot or multiple platforms, the message is   */
      /* processed in function FblCwCheckStartMessage()                  */
      if (!FblCwGetCheckForStartMsg())
# endif
      {
         FblCanProcessRxMsg();
      }
   }
#endif /* FBL_ENABLE_COM_RX_INTERRUPT || FBL_CW_ENABLE_TASK_CODE_IN_RAM || FBL_CW_ENABLE_RECEPTION_IN_STATE_TASK */

}

/***********************************************************************************************************************
 *  FblCwInitRxIdentifier
 **********************************************************************************************************************/
/*! \brief       Initialization of receive CAN identifiers
 **********************************************************************************************************************/
static void FblCwInitRxIdentifier(void)
{
   vuintx i;
   vuintx idIndex;

   for (i = 0u; i < FBL_NUMBER_OF_RX_ID; i++)
   {
      idIndex = 0u;

      /* Calculate ID position */
#if defined( FBL_CW_ENABLE_MULTIPLE_NODES )
      if (fblCanIdRxConfiguration[i].nrOfNodes > 1u)
      {
         idIndex += fblEcuHandle;
      }
#endif
      /* Set Rx Id */
      fblCanIdTable.CanRxId[fblCanIdRxConfiguration[i].fblCanIdTableIndex] = fblCanIdRxConfiguration[i].idList[idIndex];
   }
}

/***********************************************************************************************************************
 *  FblCwInit
 **********************************************************************************************************************/
/*! \brief       Initialization of communication layer
 **********************************************************************************************************************/
void FblCwInit(void)
{
   /* Init local variables */
#if ( TpCallCycle > 1 )
   tpCycleCounter = 0u;
#endif
   cwTxTimer = 0u;
   cwTxState = kFblCwTxStateIdle;
   cwCanRxMsgState = kFblCwCanRxMsgStatePhysical;

   /* Copy CAN initialization data into RAM */
   fblCanIdTable = kFblCanIdTable;

#if defined( FBL_CW_ENABLE_MULTIPLE_NODES ) || \
    defined( FBL_ENABLE_CAN_CONFIGURATION )
   /* Callback function for ECU-specific CAN initialization */
   ApplFblCanParamInit();
#endif

   /* Prepare CAN identifiers */
   FblCwInitRxIdentifier();

#if defined( FBL_DEF_ENABLE_NON_KB_MAIN )
   /* Activate Transceiver */
   ApplTrcvrNormalMode();
#endif /* FBL_DEF_ENABLE_NON_KB_MAIN */
   /* Initialize CAN interface */
   FblCanInit();

   /* Initialize Transport Layer */
   FblTpInitPowerOn();

#if defined( FBL_DEF_ENABLE_NON_KB_MAIN )
#endif /* FBL_DEF_ENABLE_NON_KB_MAIN */

   cwTaskState = 0u;

#if defined( FBL_ENABLE_STAY_IN_BOOT ) 
   fblCwStateStartMsg = 0u;
#endif

   FblCwSetInit();
}

/***********************************************************************************************************************
 *  FblCwDeinit
 **********************************************************************************************************************/
/*! \brief       Deinitialization of communication layer
 **********************************************************************************************************************/
void FblCwDeinit( void )
{
#if ( FBLDRVCAN_REFERENCE_IMPLEMENTATION >= 0x0102 )
   FblCanStop();
#endif
   FblCwClrInit();
}

/***********************************************************************************************************************
 *  FblCwSetTxInit
 **********************************************************************************************************************/
/*! \brief       Set transmit init parameter
 *  \pre         fblEcuHandle has to be configured by user callback (Multiple Nodes configuration)
 **********************************************************************************************************************/
void FblCwSetTxInit( void )
{
   vuintx i;
   vuintx idIndex;
   vuintx txObjectIndex;

   /* Initialize configured TX objects */
   for (i = 0u; i < FBL_NUMBER_OF_TX_ID; i++)
   {
      idIndex = 0u;

#if defined( FBL_CW_ENABLE_MULTIPLE_NODES )
      if (fblCanIdTxConfiguration[i].nrOfNodes > 1u)
      {
         idIndex += fblEcuHandle;
      }
#endif

      txObjectIndex = fblCanIdTxConfiguration[i].txObject;

      /* Prepare send objects */
      FblCanSetTxId(fblCanTxObj[txObjectIndex], fblCanIdTxConfiguration[i].idList[idIndex]);
      FblCanSetTxDLC(fblCanTxObj[txObjectIndex], 8u);
      FblCanSetTxConfirmation(fblCanTxObj[txObjectIndex], fblCanIdTxConfiguration[i].fblCwConfirmationHandler);

#if defined( FBL_ENABLE_NORMAL_FIXED_ADDRESSING )
      /* Store TX base ID if dynamic TX IDs are used */
      fblCwTxId[i] = fblCanIdTxConfiguration[i].idList[idIndex];
#endif /* FBL_ENABLE_NORMAL_FIXED_ADDRESSING */
   }

   /* Set transmit object index */
   fblCwDiagTransmitObject = fblCanIdTxConfiguration[kFblCwDiagTxObject].txObject;
}

/***********************************************************************************************************************
 *  FblCwDiscardReception
 **********************************************************************************************************************/
/*! \brief       Received messages are discarded
 **********************************************************************************************************************/
void FblCwDiscardReception( void )
{
   /* Discard all incoming messages */
   cwCanRxMsgState = kFblCwCanRxMsgStateDiscard;

   /* Handle all rx messages */
   while ((FblCanReceive() != 0u)) /* PRQA S 0303 */ /* MD_FblCw_0303 */
   {
      FblCanProcessRxMsg();
   }

   /* Reset to further process RX CAN messages */
   cwCanRxMsgState = kFblCwCanRxMsgStatePhysical;
}

/***********************************************************************************************************************
 *  FblCwSetRxBlock
 **********************************************************************************************************************/
/*! \brief       Reset transport layer
 **********************************************************************************************************************/
void FblCwSetRxBlock( void )
{
   /* Block DiagBuffer to refuse reception on TP */
   FblTpSetRxBlock();
}

/***********************************************************************************************************************
 *  FblCwResetRxBlock
 **********************************************************************************************************************/
/*! \brief       Reset transport layer
 **********************************************************************************************************************/
void FblCwResetRxBlock( void )
{
   cwTxTimer = 0u;
   cwTxState = kFblCwTxStateIdle;

   /* No response via TP - reset all functions */
   FblTpResetRxBlock();

}

/***********************************************************************************************************************
 *  FblCwCanTransmit
 **********************************************************************************************************************/
/*! \brief       Schedule data for transmission.
 *  \pre         CW is initialized
 *  \param[in]   data Pointer to transmit data
 *  \param[in]   length Length of data
 **********************************************************************************************************************/
void FblCwCanTransmit( const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * data, tCwDataLengthType length )
{
   vuint16 maxWaitTime;
#if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   (void)data; /* PRQA S 3112 */ /* MD_MSR_14.2  */
#endif

   /* Wait in case response pending is in progress */
   maxWaitTime = kFblCwMaxWaitTime;

   /* Retransmit response in FblCwStateTask if necessary */
   while ((maxWaitTime > 0u) && (cwTxState != kFblCwTxStateIdle))
   {
      if (FblLookForWatchdog() != FBL_NO_TRIGGER)
      {
         maxWaitTime--;
      }
   }
   /* To dispatch confirmation */
   cwTxState = kFblCwTxStateTxInProgress;
   if (FblTpTransmit(length) != kTpSuccess)
   {
      cwTxTimer = kFblCwTxPendingTimeout;
      cwTxState = kFblCwTxStateTxPending;

      /* Temporary store length */
      cwTxPendingLength = length;
   }
}

#if defined( FBL_ENABLE_STAY_IN_BOOT ) 
/***********************************************************************************************************************
 *  FblCwCheckStartMessage
 **********************************************************************************************************************/
/*! \brief       Check the "stay in boot" start message
 *  \pre         Hardware has to be initialized before
 **********************************************************************************************************************/
tFblResult FblCwCheckStartMessage( void )
{
   tFblResult result;

   /* Initialize variable */
   result = kFblFailed;

   if (FblCanReceive() != 0u)
   {
      if (FblCanStartMsg() == kFblOk)
      {
         result = kFblOk;
      }
   }

   return result;
}

# if defined( MULTIPLE_RECEIVE_BUFFER )
/***********************************************************************************************************************
 *  FblCheckOwnStartMsg
 **********************************************************************************************************************/
/*! \brief       This function checks if a Force Boot Mode message has been received.
 *  \pre         CAN hardware has to be initialized before call. May only be called after a special message was received.
 *  \param[in]   data Pointer to received data in CAN cell
 *  \return      kFblOk if it is the Force Boot Mode message
 **********************************************************************************************************************/
/* PRQA S 3673 1 */ /* MD_FblCw_3673 */
vuint8 FblCheckOwnStartMsg( pChipDataPtr data )
{
   vuint8 messageBuffer[8];   /**< Start message buffer - 8 Bytes max. on CAN */
   vuintx i;

   for (i = 0u; i < 8u; i++)
   {
      /* Copy message from CAN cell to buffer */
      messageBuffer[i] = data[i];
   }

   return FblDiagCheckStartMsg(messageBuffer, 8u);
}
# endif
#endif

/***********************************************************************************************************************
 *  CALLBACK FUNCTIONS FROM TP LAYER
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  FblCwTpDataInd
 **********************************************************************************************************************/
/*! \brief       Indication function for a transport layer message
 *  \param[in]   rxDataLen Number of received bytes
 **********************************************************************************************************************/
void FblCwTpDataInd( tCwDataLengthType rxDataLen )
{

# if defined( FBL_CW_ENABLE_NORMAL_FIXED_ALLOWING_11BIT_RX )
   if ( 1u == fblCwStateNFix11BitReceived )
   {
     FblCwTxSetTargetAddress(FBL_CW_FIXED_RESPONSE_TARGET_ADDR);
   }
# endif /* FBL_CW_ENABLE_NORMAL_FIXED_ALLOWING_11BIT_RX */

   if (rxDataLen > 0u)
   {
      FblDiagRxIndication(DiagBuffer, rxDataLen);
   }
}

/***********************************************************************************************************************
 *  FblCwTpConfirmation
 **********************************************************************************************************************/
/*! \brief       TP message transmission completed
 *  \details     This function is called by the TP to confirm a message has been sent
 *               successful or terminated with an error. The message transmission state
 *               is handed over to the diagnostic module.
 *  \param[in]   state TP confirmation state: kTpBusy, kTpSuccess, kTpErrTxFCNotExpected,
 *               kTpTimeoutFC, kTpErrTxOverrun, kTpErrTxFSInvalid, kTpCanTxFailed
 **********************************************************************************************************************/
void FblCwTpConfirmation( vuint8 state )
{
   vuint8 resetState;

   /* TpConfirm called from TP. This can have several reasons */
   if ((state != kTpErrTxFCNotExpected) && (state != kTpBusy) && (state != kTpFailed))
   {
      /* State is reset per default */
      resetState = (vuint8)1u;

      if (state == kTpSuccess)
      {
         switch (cwTxState)
         {
            case kFblCwTxStateIdle:
            {
               break;
            }
            case kFblCwTxStateRpInProgress:
            {
               FblDiagTxConfirmation(kFblCwMsgTypeRcrRp);
               break;
            }
            case kFblCwTxStateTxInProgress:
            {
               FblDiagTxConfirmation(kFblCwMsgTypeNormal);
               break;
            }
            case kFblCwTxStateRpPending: /* Intentional fall-through */
            case kFblCwTxStateTxPending:
            {
               /* Confirmation while another request is pending.           */
               /* Bus is now probably available for next transmission.     */
               /* This can be issued at next. No action required at least. */
               resetState = (vuint8)0u;
               break;
            }
            default:
            {
               /* Illegal state */
               break;
            }
         }
      }
      else
      {
         /* Evaluate the parameter type and indicate the error */
         switch (cwTxState)
         {
            case kFblCwTxStateRpInProgress: /* Intentional fall-through */
            case kFblCwTxStateRpPending:
            {
               FblDiagTxErrorIndication(kFblCwMsgTypeRcrRp);
               break;
            }
            default:
            {
               FblDiagTxErrorIndication(kFblCwMsgTypeNormal);
               break;
            }
         }
#if defined( FBL_CW_ENABLE_CANCEL_REQUEST )
         if (state == kTpCanTxFailed)
         {
            /* Reset CAN channel if TxTimeout occurs */
            FblCanInit();
         }
#endif /* FBL_CW_ENABLE_CANCEL_REQUEST */
      }

      /* Current TX operation completed */
      if (resetState != (vuint8)0u)
      {
         cwTxState = kFblCwTxStateIdle;
         cwTxTimer = 0u;
      }
   }
}

/***********************************************************************************************************************
 *  FblCwTpRxStartIndication
 **********************************************************************************************************************/
/*! \brief       Start of TP frame has been received.
 *  \details     Indicate start of TP frame to ComWrapper
 **********************************************************************************************************************/
void FblCwTpRxStartIndication( void )
{
    /* Dummy call of FblDiagRxGetPhysBuffer to clear functional requests and make it conform to the CW-specification.
     * The GetDiagBufferLocked concept is not applicable for CAN, the FblTp Module copies the received data into the
     * DiagBuffer independently of it is locked or not. */

   if (cwCanRxMsgState == kFblCwCanRxMsgStatePhysical)
   {
      (void)FblDiagRxGetPhysBuffer(gbTpRxLength);
   }

   FblDiagRxStartIndication();
}

/***********************************************************************************************************************
 *  FblCwTpErrorIndication
 **********************************************************************************************************************/
/*! \brief       TP-layer error indication function
 *  \details     Indicates that an error occurred during reception of a multi-frame message.
 *               In case of error codes kTpErrRxWrongSN or kTpErrRxTimeout, the diagnostic module is informed.
 *  \param[in]   errorCode Details the error that has occurred during reception.
 **********************************************************************************************************************/
void FblCwTpErrorIndication( vuint8 errorCode )
{
   if ((errorCode == kTpErrRxWrongSN) || (errorCode == kTpErrRxTimeout))
   {
      /* Clear all other flags for service management */

      /* Important: Indicate only those errors, that aborts reception or service transmission */
      FblDiagRxErrorIndication();
   }
}

/***********************************************************************************************************************
 *  CALLBACK FUNCTIONS FROM CAN LAYER
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  FblCwPrecopy
 **********************************************************************************************************************/
/*! \brief       Function to support functional(broadcast) requests
 *  \param[in]   data Receive data value (not used in each case)
 **********************************************************************************************************************/
#if defined( MULTIPLE_RECEIVE_BUFFER )
void FblCwPrecopy( pChipDataPtr data )
{
#endif
   vuint8 * pFuncBuffer;

#if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   (void)data; /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif

#if defined( FBL_TP_ENABLE_FIX_RX_DLC_CHECK )
# if defined( FBL_TP_ENABLE_VARIABLE_RX_DLC )
# else
   if (CanRxActualDLC != 8u)
   {
      return;
   }
# endif
#endif
   if ((pCanData[0u] != 0u) && (pCanData[0u] < CanRxActualDLC))
   {
      /* Try to get the functional request buffer */
      pFuncBuffer = FblDiagRxGetFuncBuffer(pCanData[0u]);

      /* Physical buffer is available. Service can be processed. */
      if (NULL != pFuncBuffer)
      {
         cwCanRxMsgState = kFblCwCanRxMsgStateFunctional;
         (void)FblTpPrecopy(data);
      }
   }

}

/***********************************************************************************************************************
 *  FblHandleRxMsg
 **********************************************************************************************************************/
/*! \brief       Processing of filtered CAN receive messages. After a message was received, TpPrecopy is called.
 *  \pre         CAN hardware has to be initialized before call
 *  \param[in]   fblRxCanMsgHdl RX CAN message handle
 *  \param[in]   canDataPtr In case of multiple receive buffer configuration: Pointer to CAN object.
 **********************************************************************************************************************/
#if defined( MULTIPLE_RECEIVE_BUFFER )
void FblHandleRxMsg( vuint8 fblRxCanMsgHdl, pChipDataPtr canDataPtr )
#endif
{
   vuint8 i;

   /* Confirmation handling before receive message is processed */
   (void)FblCanMsgTransmitted();

#if defined( FBL_CW_ENABLE_NORMAL_FIXED_ALLOWING_11BIT_RX )
   fblCwStateNFix11BitReceived = 0u;
#endif /* FBL_CW_ENABLE_NORMAL_FIXED_ALLOWING_11BIT_RX */

   /* Check for message handler */
   i = 0u;
   while (i < FBL_NUMBER_OF_RX_ID)
   {
      if (fblCanIdRxConfiguration[i].rxObject == fblRxCanMsgHdl)
      {
         fblCanIdRxConfiguration[i].fblCwIndicationHandler(fblRxCanMsgHdl, canDataPtr);
         break;
      }
      i++;
   }
}

#if defined( FBL_ENABLE_NORMAL_FIXED_ADDRESSING )
/***********************************************************************************************************************
 *  FblCwPrepareResponseAddress
 **********************************************************************************************************************/
/*! \brief       Read TP target address from NV-memory and prepare response address.
 *  \return      kFblOk if setting response address was successful, kFblFailed otherwise
 **********************************************************************************************************************/
tFblResult FblCwPrepareResponseAddress( void )
{
   vuint8 buffer;
   tFblResult result;

   /* Read transport layer target address */
   result = (tFblResult)ApplFblReadTpTargetAddr(&buffer);

   /* Set target address for response */
   FblCwTxSetTargetAddress(buffer);

   return result;
}

/***********************************************************************************************************************
 *  FblCwSaveResponseAddress
 **********************************************************************************************************************/
/*! \brief       Save TP source address to NV-memory
 *  \return      kFblOk if setting response address was successful, kFblFailed otherwise
 **********************************************************************************************************************/
tFblResult FblCwSaveResponseAddress( void )
{
   vuint8 buffer;

   /* Set target address for response */
   buffer = FblCwGetSourceAddress(cwTpReceiveAddress);

   /* Read transport layer target address */
   return (tFblResult)ApplFblWriteTpTargetAddr(&buffer);
}
#endif /* FBL_ENABLE_NORMAL_FIXED_ADDRESSING */

#define FBL_CW_FULL_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* Module specific MISRA deviations:

   MD_FblCw_0277:
     Reason: Usage of FblInvert.. macros implies usage of ~ operator and appropriate casts. This includes a cast from negative integer to unsigned value.
             The integer is only an intermediate type that occurs because of integer promotion while using ~ operator, though only a bit mask is operated on.
     Risk: No risk.
     Prevention: No prevention defined.

   MD_FblCw_0303:
     Reason: Casting of integer to pointer type is required through memory mapped register.
     Risk: No risk.
     Prevention: No prevention.

   MD_FblCw_3206:
      Reason: Depending on the interface implementation of the CAN driver, different variables are used to define the
              message box parameter.
      Risk: Not all possible configurations are reviewed and tested.
      Prevention: Correctness has to be verified during integration, e.g. by verifying that communication is working.

   MD_FblCw_3447:
     Reason: The current design of the hw-layer does not offer kFblCanIdTable declared extern in header file for all hardware platforms.
     Risk: Reduced Maintainability.
     Prevention: No prevention. This will have to be improved in future design.

   MD_FblCw_3451:
     Reason: The current design of the hw-layer does not offer kFblCanIdTable declared extern in header file for all hardware platforms.
             If a hardware platform implements the extern declaration, there are 2 extern declarations.
     Risk: Reduced Maintainability.
     Prevention: No prevention. This will have to be improved in future design.

   MD_FblCw_3673:
     Reason: The prototype is not adapted because future code additions might use the respective parameter as output.
     Risk: The parameter value might be changed unintentionally, thus causing malfunction in the calling instance.
     Prevention: All changes to the affected code parts have to be reviewed by the module responsible.

*/

/***********************************************************************************************************************
 *  END OF FILE: FBL_CW.C
 **********************************************************************************************************************/
