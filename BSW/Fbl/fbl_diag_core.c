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
 *                         Ach     ESCAN00079649    Move OEM deinit call to a place before actual deinitialization
 *  02.01.00   2015-06-03  Ach     ESCAN00083232    Use gap fill feature from FblLib_Mem
 *                                 ESCAN00083244    FblMemSegmentNrGet() local variables declared but not used
 *                                 ESCAN00082920    No changes
 *  02.02.00   2015-06-22  Ach     ESCAN00083496    Renamed block number get function encapsulation
 *  02.03.00   2015-07-02  Ach     ESCAN00083675    Suppress NRCs 7E and 7F for 14229:2013 configurations
 *  02.03.01   2015-08-13  Ach     ESCAN00084514    No changes
 *  02.04.00   2015-11-10  Ach     ESCAN00086228    CheckSubparameter() sets incorrect error bit
 *                                 ESCAN00086236    Added configuration option for security access delay
 *                                                  flag initialization
 *  02.05.00   2016-02-26  Ach     ESCAN00086880    Added task lock macros
 *                                 ESCAN00088283    Added new state "Response pending allowed"
 *                                 ESCAN00088286    Added configuration options for startup response contents
 *                                 ESCAN00088288    Support non-kernelbuilder main module
 *  02.05.01   2016-02-29  Ach     ESCAN00088609    GetMainStartMessageReceived() not defined
 *  02.06.00   2016-05-10  Rie     ESCAN00089943    Added additional RoutineControlTypes, moved MISRA justifications to header
 *  02.06.01   2016-05-20  Rie     ESCAN00090104    Fixed function call to FblDiagOemPrepareSdsResponse
 *  02.06.02   2016-06-07  Shs     ESCAN00090324    Initial Response Pending on 0x36 is sent out late.
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
/* Common Bootloader includes */
#include "fbl_inc.h"
#include "fbl_diag_core.h"

/***********************************************************************************************************************
 *  VERSION
 **********************************************************************************************************************/
/* Diagnostic core module version check */
#if ((FBLDIAG_14229_COREINT_VERSION != 0x0206u) || \
     (FBLDIAG_14229_COREINT_RELEASE_VERSION != 0x02u))
# error "Error in fbl_diag_core.c: Source and header file are inconsistent!"
#endif
#if (( FBLDIAG_14229_COREINT_VERSION != FBLDIAG_14229_CORE_VERSION ) || \
     ( FBLDIAG_14229_COREINT_RELEASE_VERSION != FBLDIAG_14229_CORE_RELEASE_VERSION ))
# error "Error in fbl_diag_core.h: Header versions are inconsistent."
#endif
#if ((FBLDIAG_14229_CORE_VERSION != _FBLDIAG_14229_CORE_VERSION) || \
     (FBLDIAG_14229_CORE_RELEASE_VERSION != _FBLDIAG_14229_CORE_RELEASE_VERSION))
# error "Error in fbl_diag_core.c: Source and v_ver.h file are inconsistent!"
#endif

/* Interface version compatibility check */
#if defined( FBL_DIAGCORE_API_REFERENCE_VERSION_MAJOR ) && \
    defined( FBL_DIAGCORE_API_REFERENCE_VERSION_MINOR )
#else
#  error "Error in fbl_diag_core.c: Interface version requirements not defined!"
#endif

#if ( FBL_DIAGCORE_API_REFERENCE_VERSION_MAJOR != FBL_DIAGCORE_API_VERSION_MAJOR) || \
    ( FBL_DIAGCORE_API_REFERENCE_VERSION_MINOR > FBL_DIAGCORE_API_VERSION_MINOR)
# error "Error in fbl_diag_core.c: Interface version compatibility check failed!"
#endif

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

/* Diagnostic states configuration checks */
#if( kDiagStateIdxSessionDefault != 0u )  || \
   ( kDiagStateIdxSessionExtended != 1u ) || \
   ( kDiagStateIdxSessionProgramming != 2u )
# error "Session states must not be changed to different values."
#endif

/** Bit mask to check if response is already configured */
#define kDiagResponseActive               ( kDiagPutNoResponse  | kDiagPutPosResponse | kDiagPutNegResponse )

#if defined( FBL_DIAG_ENABLE_OEM_SESSIONCHECK_NRC )
#else
# define DiagNRCServiceCheckNotSucceeded() DiagNRCServiceNotSupportedInActiveSession() /**< Define configurable session check NRC */
#endif

/* Specific service responses */
/* RoutineControl status for eraseMemory */
#if !defined( kDiagEraseSucceeded )
# define kDiagEraseSucceeded                                0x00u                /**< Erase memory was started and succeeded */
#endif /* kDiagEraseSucceeded */
#if !defined( kDiagEraseFailed )
# define kDiagEraseFailed                                   0x01u                /**< Failure occured after routine was started */
#endif /* kDiagEraseFailed */

/* Signature handling */
#if !defined(kSecSigSize)
# define kSecSigSize 0
#endif

/* Gap fill */
/* Buffer size for gap fill function */
#if ( FBL_MAX_SEGMENT_SIZE > 64u )
# define FILL_BUFFER_SIZE FBL_MAX_SEGMENT_SIZE
#else
# define FILL_BUFFER_SIZE 64u
#endif

#if defined( FBL_DIAG_ENABLE_TASK_LOCKS )
/* Flags indicate, which task is currently running */
# define kFblDiagStateTaskRunning      FBL_BIT0    /**< State task active */
# define kFblDiagTimerTaskRunning      FBL_BIT1    /**< Timer task active */

# define SetDiagStateTaskRunning()     (diagTaskState |= kFblDiagStateTaskRunning)                                /**< Set running state of state task */
# define ClrDiagStateTaskRunning()     (diagTaskState &= FblInvert8Bit(kFblDiagStateTaskRunning))                 /**< Clear running state of state task */
# define GetDiagStateTaskRunning()     ((diagTaskState & kFblDiagStateTaskRunning) == kFblDiagStateTaskRunning)   /**< Return running state of state task */

# define SetDiagTimerTaskRunning()     (diagTaskState |= kFblDiagTimerTaskRunning)                                /**< Set running state of timer task */
# define ClrDiagTimerTaskRunning()     (diagTaskState &= FblInvert8Bit(kFblDiagTimerTaskRunning))                 /**< Clear running state of timer task */
# define GetDiagTimerTaskRunning()     ((diagTaskState & kFblDiagTimerTaskRunning) == kFblDiagTimerTaskRunning)   /**< Return running state of timer task */
#endif /* FBL_DIAG_ENABLE_TASK_LOCKS */

#if !defined( FBL_DEF_ENABLE_NON_KB_MAIN )
# define FblMainGetStartMessageReceived() ((fblMode & FBL_START_WITH_PING) == FBL_START_WITH_PING)
#endif

/***********************************************************************************************************************
 *  TYPEDEFS AND STRUCTURES FOR INTERNAL USE
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/
/** Pointer to the data transfer buffer */
V_MEMRAM0 V_MEMRAM1      vuint8              V_MEMRAM2 V_MEMRAM3 * V_MEMRAM1 V_MEMRAM2 DiagBuffer;
/** Memory segment handle */
V_MEMRAM0 V_MEMRAM1      vsint16             V_MEMRAM2      memSegment;
/** Stores number of received data */
V_MEMRAM0 V_MEMRAM1_NEAR tCwDataLengthType   V_MEMRAM2_NEAR DiagDataLength;
/** Timer value for TesterPresent timeout */
V_MEMRAM0 V_MEMRAM1_NEAR vuint16             V_MEMRAM2_NEAR testerPresentTimeout;
/** Contains the currently processed diag service ID */
V_MEMRAM0 V_MEMRAM1_NEAR vuint8              V_MEMRAM2_NEAR diagServiceCurrent;
/** State bitmap array */
V_MEMRAM0 V_MEMRAM1_NEAR tFblStateBitmap     V_MEMRAM2_NEAR fblDiagStates[FBL_STATE_INDEX(kDiagNumberOfStates - 1u) + 1u];
/** Temporary index for subparameter table of a diag service */
V_MEMRAM0 V_MEMRAM1      vuint8              V_MEMRAM2      subParTabIdx;

#if defined( FBL_ENABLE_DEBUG_STATUS )
/* Variables for error status reporting */
V_MEMRAM0 V_MEMRAM1      vuint16          V_MEMRAM2      errStatErrorCode;          /**< Error status */
V_MEMRAM0 V_MEMRAM1      vuint16          V_MEMRAM2      errStatFblStates;          /**< FBL state flag */
V_MEMRAM0 V_MEMRAM1      vuint8           V_MEMRAM2      errStatLastServiceId;      /**< Last received service identifier */
V_MEMRAM0 V_MEMRAM1      vuint8           V_MEMRAM2      errStatFlashDrvVersion[3]; /**< Flash driver version information */
V_MEMRAM0 V_MEMRAM1      vuint16          V_MEMRAM2      errStatFlashDrvErrorCode;  /**< Flash driver error code */
V_MEMRAM0 V_MEMRAM1      tBlockDescriptor V_MEMRAM2      errStatDescriptor;         /**< Error status block descriptor */
#endif

/***********************************************************************************************************************
 *  LOCAL DATA
 **********************************************************************************************************************/
V_MEMRAM0 static V_MEMRAM1 vuint8               V_MEMRAM2      diagPostParam;
/** Buffer for response pending message */
V_MEMRAM0 static V_MEMRAM1 vuint8               V_MEMRAM2      rcrrpBuffer[3];   /* PRQA S 3218 */ /* MD_FblDiag_3218 */

/** Diagnostic response length */
V_MEMRAM0 static V_MEMRAM1 tCwDataLengthType    V_MEMRAM2      diagResLen;
/** Index of current service's main handler in main handler table */
V_MEMRAM0 static V_MEMRAM1 tFblDiagMainHandler  V_MEMRAM2      serviceMainHandler;
#if defined( FBL_ENABLE_SERVICE_POSTHANDLER )
V_MEMRAM0 static V_MEMRAM1 tFblDiagPostHandler  V_MEMRAM2      servicePostHandler;
#endif

/** The session state flags for the current service - stored for possible use in service handlers */
V_MEMRAM0 static V_MEMRAM1 vuint8               V_MEMRAM2      currentServiceSessionSupport;

#if defined( FBL_DIAG_ENABLE_OEM_SEGMENTNRGET )
#else
/* FblMemSegmentNrGet caching */
V_MEMRAM0 static V_MEMRAM1 vsint16              V_MEMRAM2      cachedSegment;
V_MEMRAM0 static V_MEMRAM1 tFlashBlock          V_MEMRAM2      cachedBlock;
V_MEMRAM0 static V_MEMRAM1 vsint16              V_MEMRAM2      nextValidSegment;
#endif /* FBL_DIAG_ENABLE_OEM_SEGMENTNRGET || FBL_DIAG_ENABLE_OEM_READPROM */

#if defined( FBL_DIAG_ENABLE_TASK_LOCKS )
V_MEMRAM0 static V_MEMRAM1 vuint8               V_MEMRAM2      diagTaskState;
#endif /* FBL_DIAG_ENABLE_TASK_LOCKS */

/***********************************************************************************************************************
 *  LOCAL DATA - visible in core and OEM layer
 **********************************************************************************************************************/

/** Pointer to current service's information table */
V_MEMRAM0 V_MEMROM1 tServiceInfoTable    V_MEMROM2 V_MEMROM3 * V_MEMRAM1 V_MEMRAM2 serviceInfo;
/** Flag indicating the state of pos. response */
V_MEMRAM0 V_MEMRAM1_NEAR vuint8          V_MEMRAM2_NEAR diagResponseFlag;
/** Diagnostic error code */
V_MEMRAM0 V_MEMRAM1 vuint8               V_MEMRAM2      diagErrorCode;

#if defined( FBL_DIAG_ENABLE_CORE_SEGMENTHANDLING )
/* Temporary data used during download in TransferData */
/** Actual number of segments */
V_MEMRAM0 V_MEMRAM1 vuint8               V_MEMRAM2      segmentCount;
#endif /* FBL_DIAG_ENABLE_CORE_SEGMENTHANDLING */

/** Reset timeout observation [ms] */
V_MEMRAM0 V_MEMRAM1 vuint16              V_MEMRAM2      ecuResetTimeout;
/* Sleep mode handling */
#if defined( FBL_ENABLE_SLEEPMODE )
/** Counter, determines when to sleep */
V_MEMRAM0 V_MEMRAM1        vuint32              V_MEMRAM2      sleepCounter;
#endif

/***********************************************************************************************************************
 *  EXTERNAL DATA
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

static void       ChkSuppressPosRspMsgIndication(vuint8 *subparam);
static void       FblDiagDiscardReception(void);
static void       FblDiagResponseProcessor(void);

static tFblResult CheckServicePreconditions(V_MEMROM1 tServiceInfoTable V_MEMROM2 V_MEMROM3 * pServiceInfo);
static V_MEMROM1  tServiceInfoTable V_MEMROM2 V_MEMROM3 * FindServiceInfo(vuint8 serviceId);

static vuint16 CheckSubparameter(V_MEMROM1 vuint8 V_MEMROM2 V_MEMROM3 * subFctParamIdTable, vsintx paramId);
static vuint16 CheckSubServiceTable(V_MEMROM1 tSubFctInfoTable V_MEMROM2 V_MEMROM3 * subFctInfo, vsintx infoTableLen, vsintx paramIdTableLen);
static vuint16 CheckServiceTable(V_MEMROM1 tServiceInfoTable V_MEMROM2 V_MEMROM3 * pServiceInfo);

static void FblDiagDeinit(void);

/* Start codeseg to be executed in RAM */
#define FBLDIAG_RAMCODE_START_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */
static void FblDiagResetServiceFlags(void);
static void FblDiagConfirmation(void);
/* End section to execute code from RAM */
#define FBLDIAG_RAMCODE_STOP_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/***********************************************************************************************************************
 *  ROUTINES TO MANAGE DIAGNOSTIC SERVICES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  Service support functions
 **********************************************************************************************************************/

#if defined( FBL_DIAG_ENABLE_OEM_READPROM )
#else
/***********************************************************************************************************************
 *  FblReadProm
 **********************************************************************************************************************/
/*! \brief       Read from ROM memory.
 *  \details     Reads data from ROM memory using the read function supplied with the flash driver.
 *  \param[in]   address Memory address to read out (logical address).
 *  \param[out]  buffer Target buffer.
 *  \param[in]   length Number of bytes to be read.
 *  \return      Number of actually copied bytes.
 **********************************************************************************************************************/
tFblLength FblReadProm(tFblAddress address, vuint8 *buffer, tFblLength length)
{
   tFblLength actualReadCount;    /* Actual read bytes */
   tFblLength currentReadCount;
   vsint16    nextMemSegment;

   /* Initialize variables */
   actualReadCount = 0u;

   (void)FblRealTimeSupport();

   /* Find start segment */
   memSegment = FblMemSegmentNrGet(address);

   if (memSegment < 0)
   {
      /*
         Start address does not lie within a valid memory segment
         Search first segment which overlaps with requested range
         This allows the segmented reading of memory areas which include gaps
         (e.g. CRC calculation or signature verification)
       */

      /* nextValidSegment contains first segment behind gap */
      if (nextValidSegment < kNrOfFlashBlock)
      {
         /* Gap detection */
         if (
               (address < FlashBlock[nextValidSegment].begin) &&
               (length > (FlashBlock[nextValidSegment].begin - address))
            )
         {
            /* Found first segment which overlaps requested range */
            memSegment = nextValidSegment;

            /* Remove gap from read length */
            length -= (FlashBlock[memSegment].begin - address);
            /* Adapt start address to segment start */
            address = FlashBlock[memSegment].begin;
         }
      }
   }

   /* Initialize next memory segment index */
   nextMemSegment = memSegment;

   /* Start segment found, read segment */
   if (memSegment >= 0)
   {
      while (length > 0u)
      {
         /* Check if read range crosses Flash block boundary */
         if ((address + (length - 1u)) > FlashBlock[memSegment].end) /* PRQA S 3689 */ /* MD_FblDiag_3689 */
         {
            currentReadCount = (FlashBlock[memSegment].end - address) + 1u;   /* PRQA S 3689 */ /* MD_FblDiag_3689 */

            nextMemSegment++;

            if (nextMemSegment < kNrOfFlashBlock)
            {
               length = ((FlashBlock[nextMemSegment].begin - address) < length)
                  ? (length - (FlashBlock[nextMemSegment].begin - address)) : 0u;
            }
            else
            /* Flash block index overflow, exit loop */
            {
               length = 0u;
            }
         }
         else
         {
            currentReadCount = length;
            length = 0u;
         }

         if (MemDriver_RReadSync(buffer, currentReadCount, address) == IO_E_OK)
         {
            memSegment = nextMemSegment;
            buffer = (vuint8 *)&buffer[currentReadCount];
            address = FlashBlock[memSegment].begin;   /* PRQA S 3689 */ /* MD_FblDiag_3689 */
            actualReadCount += currentReadCount;
         }
         else
         {
            /* In case of error quit loop */
            length = 0u;
         }

         (void)FblRealTimeSupport();
      }
   }
   else
   {
      actualReadCount = 0u;
   }

   return actualReadCount;
}
#endif /* FBL_DIAG_ENABLE_OEM_READPROM */

/***********************************************************************************************************************
 *  FblEcuReset
 **********************************************************************************************************************/
/*! \brief       Start ECU reset.
 **********************************************************************************************************************/
void FblDiagEcuReset(void)
{
   /* Disable tester present timeout monitoring */
   StopTesterTimeout();

   /* Do not transmit response message */
   DiagSetNoResponse();
   FblDiagDeinit();
}

/***********************************************************************************************************************
 *  END OF DIAGNOSTIC SERVICE MANAGEMENT HANDLERS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  DIAGNOSTIC SUPPORT FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  Global diagnostic support functions (OEM independent)
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  FblDiagSetError
 **********************************************************************************************************************/
/*! \brief       Sets the corresponding error code for the currently active service
 *  \param[in]   errorCode Negative response code
 **********************************************************************************************************************/
void FblDiagSetError( vuint8 errorCode )
{
   diagErrorCode = errorCode;

   /* No negative response for functional requests with these error codes */
   if (    (GetFunctionalRequest())
        && (!GetRcrRpInProgress())
        && (   (errorCode == kDiagNrcServiceNotSupported )
            || (errorCode == kDiagNrcSubFunctionNotSupported )
            || (errorCode == kDiagNrcRequestOutOfRange )
#if defined( FBL_DIAG_ENABLE_14229_2013 )
            || (errorCode == kDiagNrcSubfunctionNotSupportedInActiveSession)
            || (errorCode == kDiagNrcServiceNotSupportedInActiveSession)
#endif /* FBL_DIAG_ENABLE_14229_2013 */
           )
      )
   {
     DiagSetNoResponse();
   }
   else
   {
      /* Send negative response for all other cases */
      diagResponseFlag |= kDiagPutNegResponse;
   }
}

/***********************************************************************************************************************
 *  FblRealTimeSupport
 **********************************************************************************************************************/
/*! \brief       Transmit busy messages and triggers the watchdog according to system timer.
 *  \pre         Diagnosis and communication stack must be initialized, diagnostic request has to be received.
 **********************************************************************************************************************/
vuint8 FblRealTimeSupport( void )
{
   vuint8 returnCode;

   returnCode = (vuint8)FblLookForWatchdog();
   DiagExRCRResponsePending(kNotForceSendResponsePending);
   return returnCode;
}

/***********************************************************************************************************************
 *  DiagExRCRResponsePending
 **********************************************************************************************************************/
/*! \brief       Transmit a busy message if timer expires
 *  \details     (forceSend == kNotForceSendResponsePending) or if kForceSendResponsePending is passed.
 *  \param[in]   forceSend Determines if a message is sent independently from timer state
 **********************************************************************************************************************/
void DiagExRCRResponsePending( vuint8 forceSend )
{
   /* Only send response pending if service is in progress */
   if (GetRcrRpAllowed())
   {
      /* Conditions to send an RCR-RP if P2-timer expired */
      /* or parameter contains kForceSendResponsePending   */
      if ((forceSend == kForceSendResponsePending)
            || ((forceSend == kForceSendRpIfNotInProgress) && (!GetRcrRpInProgress()))
            || (GetP2Timer() < (GetRcrRpInProgress() ? kFblDiagP2StarMinThreshold : kFblDiagP2MinThreshold)) )
      {
         /* Prepare parameter for the Diag-confirmation function */
         diagPostParam = kDiagPostRcrRp;

         /* Prepare and transmit RCRRP buffer */
         rcrrpBuffer[0] = kDiagRidNegativeResponse;
         rcrrpBuffer[1] = diagServiceCurrent;
         rcrrpBuffer[2] = kDiagNrcRcrResponsePending;
         FblCwTransmitRP(rcrrpBuffer);

         /* Restart P2-Timer to P2* */
         SetP2Timer(kFblDiagTimeP2Star);

         /* If response pending is transmitted and no response
          * is set, a positive response has to be transmitted anyway. */
         diagResponseFlag &= FblInvert8Bit(kDiagPutNoResponse);   /* PRQA S 0277 */ /* MD_FblDef_Invert */

         SetRcrRpInProgress();
      }
   }
}

#if defined( FBL_DIAG_ENABLE_OEM_SEGMENTNRGET )
#else
/***********************************************************************************************************************
 *  FblMemSegmentNrGet
 **********************************************************************************************************************/
/*! \brief       Get the number of the corresponding flash block for the given address
 *  \return      Index of flash block; -1, if not found
 *  \param[in]   address
 **********************************************************************************************************************/
vsint16 FblMemSegmentNrGet( tFblAddress address )
{
   vsint16 segment;

   /* Check address against cached range */
   if (   ( address >= cachedBlock.begin )
       && ( address <= cachedBlock.end )
      )
   {
      /* Range matches, finish processing and return cached segment */
      segment = cachedSegment;
   }
   else
   {
      /* Search matching flash block */
      for (nextValidSegment = 0; nextValidSegment < (vsint16)kNrOfFlashBlock; nextValidSegment++)
      {
         /* Reduced number of watchdog trigger calls */
         if (((vuint8)nextValidSegment & (vuint8)0x0Fu) == (vuint8)0x00u)
         {
            (void)FblRealTimeSupport();
         }

         /* Found first potential match */
         if (FlashBlock[nextValidSegment].end >= address)
         {
            break;
         }
      }

      /* Valid match found */
      if (nextValidSegment < (vsint16)kNrOfFlashBlock)
      {
         if (address >= FlashBlock[nextValidSegment].begin)
         {
            /* Address lies within block => valid segment */
            segment = nextValidSegment;

            /* Cache current match */
            cachedBlock.begin = FlashBlock[segment].begin;
            cachedBlock.end = FlashBlock[segment].end;
         }
         else
         {
            /* Address lies in front of block => gap detected */
            segment = -1;

            /* Cache current gap */
            if (nextValidSegment <= 0)
            {
               /* First block, gap starts at beginning of address space */
               cachedBlock.begin = 0x00u;
            }
            else
            {
               /* Gap starts after end of previous block */
               cachedBlock.begin = FlashBlock[nextValidSegment - 1].end + 1;
            }
            /* Gap ends in front of current segment */
            cachedBlock.end = FlashBlock[nextValidSegment].begin - 1;
         }
      }
      else
      {
         /* Address lies behind last block => gap detected */
         segment = -1;

         /* Cache current gap */
         /* Gap starts after end of previous block */
         cachedBlock.begin = FlashBlock[kNrOfFlashBlock - 1u].end + 1u;
         /* Gap ends at end of address space */
         /* PRQA S 0277 1 */ /* MD_FblDiag_0277 */
         cachedBlock.end = (tFblAddress) - 1;   /* Note: The correct conversion is guaranteed by the C standard */
      }

      /* Cache current segment for next request */
      cachedSegment = segment;
   }

   return segment;
}
#endif /* FBL_DIAG_ENABLE_OEM_SEGMENTNRGET */

/***********************************************************************************************************************
 *  FblDiagMemGetActiveBuffer
 **********************************************************************************************************************/
/*! \brief       Function updates core Diagnostic buffer (DiagBuffer) and returns updated pbDiagData to handler
 *  \return      updated pbDiagData buffer used by diagnostic handler functions *
 **********************************************************************************************************************/
V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * FblDiagMemGetActiveBuffer(void)
{
   DiagBuffer = FblMemGetActiveBuffer();
   return &DiagBuffer[kDiagFmtpbDiagData];
}

#if defined( FBL_DIAG_ENABLE_CORE_RANGECHECKS )
/***********************************************************************************************************************
 *  FblCheckAddressRange
 **********************************************************************************************************************/
/*! \brief       Check whether an address lies within a specified address range
 *  \param[in]   address Address to be checked
 *  \param[in]   rangeStart Start of range
 *  \param[in]   rangeLength Length of range
 *  \return      kFblOk if address lies within range, kFblFailed otherwise
 **********************************************************************************************************************/
tFblResult FblCheckAddressRange( tFblAddress address, tFblAddress rangeStart, tFblLength rangeLength )
{
   tFblResult result;

   result = kFblFailed;

   if ((address >= rangeStart)
        && ((address - rangeStart) < rangeLength))
   {
      result = kFblOk;
   }

   return result;
}

/***********************************************************************************************************************
 *  FblCheckRangeContained
 **********************************************************************************************************************/
/*! \brief       Check whether an address range lies within another specified address range
 *  \details     This function checks if the range specified with address and length lies within the
 *               enclosing range specified by rangeStart and rangeLength.
 *  \param[in]   address Start address of range to be checked
 *  \param[in]   length Length of range to be checked
 *  \param[in]   rangeStart Start of range
 *  \param[in]   rangeLength Length of range
 *  \return      kFblOk if range lies within range, kFblFailed otherwise
 **********************************************************************************************************************/
tFblResult FblCheckRangeContained( tFblAddress address, tFblLength length, tFblAddress rangeStart, tFblLength rangeLength )
{
   tFblResult result;

   result = kFblFailed;

   if ((FblCheckAddressRange(address, rangeStart, rangeLength) == kFblOk)
       && (length <= rangeLength)
       && ((address - rangeStart) <= (rangeLength - length)))
   {
      result = kFblOk;
   }

   return result;
}
#endif /* FBL_DIAG_ENABLE_CORE_RANGECHECKS */

/***********************************************************************************************************************
 *  Local diagnostic support functions (OEM independent)
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  DiagProcessingDone
 **********************************************************************************************************************/
/*! \brief       Prepare positive response.
 *  \details     Prepares the positive reponse for the currently processed service.
 *  \pre         Service must be in progress
 *  \param[in]   dataLength Data length to be transmitted (excluding the service-id itself).
 **********************************************************************************************************************/
void DiagProcessingDone( tCwDataLengthType dataLength )
{
   diagResponseFlag |= kDiagPutPosResponse;
   diagResLen        = dataLength;
}

/***********************************************************************************************************************
 *  FblDiagDiscardReception
 **********************************************************************************************************************/
/*! \brief       Received diagnostic messages are discared
 *  \details     Discards all messages which have been received during a service is processed.
 **********************************************************************************************************************/
static void FblDiagDiscardReception( void )
{
   SetResponseProcessing();   /* Set flag for running response procedure */

   FblCwDiscardReception();
   ClrResponseProcessing();
}

/* Start codeseg to be executed in RAM */
#define FBLDIAG_RAMCODE_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/***********************************************************************************************************************
 *  FblDiagResetServiceFlags
 **********************************************************************************************************************/
/*! \brief       Clear all status flags relevant for service handling.
 *  \pre         Service response shall be finished.
 **********************************************************************************************************************/
static void FblDiagResetServiceFlags( void )
{
   /* Clear neg. response indicator */
   DiagClrError();

   /* Reset internal state in case no response was sent */
   ClrServiceInProgress();
   ClrRcrRpInProgress();
   /* Reset flag for functional request, default is physical request */
   ClrFunctionalRequest();
   ClrDiagHandlerCalled();
   /* Reset flag for suppress positive response bit */
   ClrSuppressPosRspMsg();

   /* diagResponse state back to idle */
   diagResponseFlag = kDiagResponseIdle;
}

/***********************************************************************************************************************
 *  FblDiagConfirmation
 **********************************************************************************************************************/
/*! \brief       Clear all status flags and conclude service transmission.
 *  \pre         Service response has been sent recently.
 **********************************************************************************************************************/
static void FblDiagConfirmation( void )
{
   /* Buffer is now available */
   ClrDiagBufferLocked();

   /* Restart the tester present timer if not in default session */
#if defined( FBL_ENABLE_STAY_IN_BOOT )
   if (GetDiagProgrammingSession() || GetDiagExtendedSession() || FblMainGetStartMessageReceived())
#else
   if (GetDiagProgrammingSession() || GetDiagExtendedSession())
#endif /* FBL_ENABLE_STAY_IN_BOOT */
   {
      ResetTesterTimeout();
   }

   /* Clear all diagnostic states */
   ClrP2Timer();

#if defined( FBL_ENABLE_SERVICE_POSTHANDLER )
   /* Check for a valid service indicator */
   if( GetDiagHandlerCalled() && ( servicePostHandler != V_NULL ))
   {
      /* Call the postHandler of the last received service */
      (servicePostHandler)( diagPostParam );
   }
#endif   /* FBL_ENABLE_SERVICE_POSTHANDLER */

   /* Clear all flags for service handling */
   FblDiagResetServiceFlags();
}

/* End section to execute code from RAM */
#define FBLDIAG_RAMCODE_STOP_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/***********************************************************************************************************************
 *  FblDiagResponseProcessor
 **********************************************************************************************************************/
/*! \brief       Prepares response message and hands it over to ComWrapper.
 *  \details     Prepares either a positive or negative response. Depending on the response type,
 *               the service ID is set to SID + 0x40 or 0x7F.
 *  \pre         Current SID must be in DiagBuffer[0], diagErrorCode must be initialized.
 **********************************************************************************************************************/
static void FblDiagResponseProcessor( void )
{
   /* Service will be concluded under this condition */
   if(( diagResponseFlag & kDiagResponseActive ) != 0u )
   {
      /* Stop transmission of response pending messages */
      ClrRcrRpAllowed();
      ClrP2Timer();
      ClrRcrRpInProgress();

      /* Discard all service requests received during processing this one */
      FblDiagDiscardReception();
      FblErrStatSetSId( diagServiceCurrent );

      /* Negative response has highest priority. */
      /* Must be checked first.                  */
      if(( diagResponseFlag & kDiagPutNegResponse) != 0u )
      {
         /* Set response type */
         diagPostParam = kDiagPostNegResponse;

         /* Prepare response message */
         DiagBuffer[kDiagFmtSubparam]     = diagServiceCurrent;
         DiagBuffer[kDiagFmtServiceId]    = kDiagRidNegativeResponse;
         DiagBuffer[kDiagFmtNegResponse]  = DiagGetError();

         assertFblUser(DiagGetError() != kDiagErrorNone, kFblSysAssertParameterOutOfRange);  /* PRQA S 2214 */ /* MD_FblDiag_2214 */
         FblCwTransmit(DiagBuffer, 3u, kFblCwTxCallNegResponse);

         /* diagResponse state back to idle */
         diagResponseFlag = kDiagResponseIdle;
      }
      else if (( diagResponseFlag & kDiagPutNoResponse ) != 0u )
      {
         /* Set response type */
         diagPostParam = kDiagPostNoResponse;

         /* Service conclusion */
         FblDiagConfirmation();

         /* Check if a reset shall be generated without message transmission */
         if (GetWaitEcuReset())
         {
            /* If so, generate the reset immediately */
            SetResetMsgConfirmed();
         }

         FblCwResetRxBlock();
      }
      else if (( diagResponseFlag & kDiagPutPosResponse ) != 0u )
      {
         /* Set response type */
         diagPostParam = kDiagPostPosResponse;

         /* Transmit positive response if not suppressed */
         DiagBuffer[kDiagFmtServiceId] = (vuint8)(diagServiceCurrent + 0x40u);
         FblCwTransmit(DiagBuffer, (tCwDataLengthType)( diagResLen + 1u ), kFblCwTxCallPosResponse );

         /* diagResponse state back to idle */
         diagResponseFlag = kDiagResponseIdle;
      }
      else
      {
         /* This is an illegal state and should not occur */
         assertFblInternal(0u, kFblSysAssertParameterOutOfRange );  /* PRQA S 2214 */ /* MD_FblDiag_2214 */

         /* Set response type */
         diagPostParam = kDiagResponseIdle;
      }

      /* Should be an assertion */
      if( diagPostParam == kDiagPostRcrRp )
      {
        /* diagResponse state back to idle */
        diagResponseFlag = kDiagResponseIdle;
      }
   }
}  /* PRQA S 6080 */ /* MD_MSR_STMIF */

/***********************************************************************************************************************
 *  ChkSuppressPosRspMsgIndication
 **********************************************************************************************************************/
/*! \brief       This functions checks if a diagnotsic response has to be sent.
 *  \details     If the request is received with Suppress Positive Response Bit (0x80), the response is cancelled.
 *  \param[in]   subparam Diagnostic subparameter
 **********************************************************************************************************************/
static void ChkSuppressPosRspMsgIndication( vuint8 *subparam )
{
   if (((*(subparam)) & (kDiagSuppressPosRspMsgIndicationBit)) != 0)
   {
      /* Set internal flag to suppress positive response */
      DiagSetNoResponse();
      SetSuppressPosRspMsg();

      /* Clear suppress bit in request message */
      (*(subparam)) &= FblInvert8Bit( kDiagSuppressPosRspMsgIndicationBit );  /* PRQA S 0277 */ /* MD_FblDef_Invert */
   }
}
#if defined( FBL_DIAG_ENABLE_CORE_SEGMENTHANDLING )
/***********************************************************************************************************************
 *  FblDiagSegmentNext
 **********************************************************************************************************************/
/*! \brief       Keep track of segment counter
 *  \return      kSwmOutOfRange if maximum segment count exceeded, number of segments otherwise
 **********************************************************************************************************************/
vuint8 FblDiagSegmentNext( void )
{
   vuint8 result;

   /* Compare current used NOAR against max. value */
   if (segmentCount >= SWM_DATA_MAX_NOAR)
   {
      /* ALL PMA processed. Stop reading data */
      result =  kSwmOutOfRange;
   }
   else
   {
      /* Increment segment count */
      segmentCount++;

      result = segmentCount;
   }

   return result;
}
#endif /* FBL_DIAG_ENABLE_CORE_SEGMENTHANDLING */
 
#if defined( FBL_DIAG_ENABLE_CORE_GETBLOCKNR )
# if defined( FBL_DIAG_ENABLE_ADDR_BASED_DOWNLOAD )
/***********************************************************************************************************************
 *  FblGetBlockNrFromAddress
 **********************************************************************************************************************/
/*! \brief       Get logical block number from address and length
 *  \details     Get the number of the corresponding logical block for the given block address.
 *  \param[in]   blockAddress Start address of address range
 *  \param[in]   blockLength Length of address range
 *  \param[out]  pLogicalBlock Logical block number
 *  \return      kFblOk if blockIndex has been found, otherwise kFblFailed
 **********************************************************************************************************************/
tFblResult FblGetBlockNrFromAddress(tFblAddress blockAddress, tFblLength blockLength, vuint8* pLogicalBlock)
{
   tFblResult result;
   vuint8 tempCount;

   /* Initialize variables */
   result = kFblFailed;
   *pLogicalBlock = 0xFFu;

   if( blockLength > 0u)
   {
      (void)FblLookForWatchdog();
      tempCount = 0u;
      /* Search logical block table for blockIndex */
      while ((result == kFblFailed) && (tempCount < FblLogicalBlockTable.noOfBlocks))
      {
         /* Check if requested addresses lie within a logical block */
         result = FblCheckRangeContained(blockAddress,
                                     blockLength,
                                     FblLogicalBlockTable.logicalBlock[tempCount].blockStartAddress,
                                     FblLogicalBlockTable.logicalBlock[tempCount].blockLength
                                    );

         /* Logical block found - save block number */
         if (result == kFblOk)
         {
            *pLogicalBlock = tempCount;
         }

         /* Increment loop counter */
         tempCount++;
      }
   }

   return result;
}
# else
/***********************************************************************************************************************
 *  FblGetBlockNrFromIndex
 **********************************************************************************************************************/
/*! \brief       Get logical block number for a given block index
 *  \details     Get the number of the corresponding logical block for the given block index.
 *  \param[in]   blockIndex Index of logical block
 *  \param[out]  pLogicalBlock Logical block number
 *  \return      kFblOk if blockIndex has been found, otherwise kFblFailed
 **********************************************************************************************************************/
tFblResult FblGetBlockNrFromIndex(vuint8 blockIndex, vuint8* pLogicalBlock)
{
   tFblResult result;
   vuint8 tempCount;

   /* Initialize variables */
   result = kFblFailed;
   *pLogicalBlock = 0xFFu;

   (void)FblLookForWatchdog();
   tempCount = 0u;
   /* Search logical block table for blockIndex */
   while ((result == kFblFailed) && (tempCount < FblLogicalBlockTable.noOfBlocks))
   {
      /* Check if requested index matches a logical block index */
      if (blockIndex == FblLogicalBlockTable.logicalBlock[tempCount].blockIndex)
      {
         result = kFblOk;
         *pLogicalBlock = tempCount;
      }

      /* Increment loop counter */
      tempCount++;
   }

   return result;
}
# endif /* FBL_DIAG_ENABLE_ADDR_BASED_DOWNLOAD */
#endif /* FBL_DIAG_ENABLE_CORE_GETBLOCKNR */

/***********************************************************************************************************************
 *  DIAGNOSTIC MODULE CONTROL ROUTINES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  Initialization handling of diagnostic module
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  FblDiagInitPowerOn
 **********************************************************************************************************************/
/*! \brief       Initialize module variables
 **********************************************************************************************************************/
void FblDiagInitPowerOn(void)
{
   vuint8 i;

   /* Initialize diagnostic states */
   for (i = 0u; i < (FBL_STATE_INDEX(kDiagNumberOfStates - 1u) + 1u); i++)
   {
      fblDiagStates[i] = (tFblStateBitmap)0x00u;
   }

   /* Initialize diagnostic variables */
   memSegment = -1;
   diagResponseFlag = kDiagResponseIdle;

   DiagClrError();

   /* Initialize diagnostic session */
   SetDiagDefaultSession();   /* PRQA S 3109 */ /* MD_FblDiag_3109 */

   /* Stop tester present timer */
   StopTesterTimeout();

   /* Clear timer for response pending transmission */
   ClrP2Timer();

   /* Make sure to turn off the programming voltage */
   ApplFblResetVfp();

   StopEcuResetTimeout();

#if defined( FBL_DEF_ENABLE_NON_KB_MAIN )
# if defined( FBL_ENABLE_SLEEPMODE )
   /* Sleep Timer not running during startup */
   FblSleepCounterClear();
# endif
#endif /* FBL_DEF_ENABLE_NON_KB_MAIN */

#if defined( FBL_DIAG_ENABLE_OEM_SEGMENTNRGET )
#else
   /* FblMemSegmentNrGet caching - initialize cache with a valid entry */
   cachedSegment = 0;
   cachedBlock.begin = FlashBlock[0].begin;
   cachedBlock.end = FlashBlock[0].end;
#endif /* FBL_DIAG_ENABLE_OEM_SEGMENTNRGET */

#if defined( FBL_DIAG_ENABLE_TASK_LOCKS )
   diagTaskState = 0u;
#endif /* FBL_DIAG_ENABLE_TASK_LOCKS */

#if defined( FBL_DIAG_ENABLE_OEM_INITPOWERON )
   /* OEM-specific initializations */
   FblDiagOemInitPowerOn();
#endif /* FBL_DIAG_ENABLE_OEM_INITPOWERON */
}

/***********************************************************************************************************************
 *  FblDiagInit
 **********************************************************************************************************************/
/*! \brief       Initialize diagnostic module
 *  \details     Sets the diagnostic module to default session and initializes depending modules. The diagnostic
 *               module is working from here, but NV-memory dependent features are not available in all configurations.
 **********************************************************************************************************************/
void FblDiagInit(void)
{

   /* Get aligned diag buffer pointer from memory library */
   DiagBuffer = FblMemInitPowerOn();

#if defined( FBL_ENABLE_DEBUG_STATUS )
   /* Initialize error status */
   ApplFblInitErrStatus();
#endif

#if defined ( FBL_DIAG_ENABLE_CORE_SEGMENTHANDLING )
   /* Initialize segment counter */
   FblDiagSegmentInit();
#endif

   /* Initialize security module */
   (void)ApplFblSecurityInit();

   /* Init ComWrapper */
   FblCwSetTxInit();

#if defined( FBL_DIAG_ENABLE_OEM_INIT )
   /* OEM-specific initializations */
   FblDiagOemInit();
#endif /* FBL_DIAG_ENABLE_OEM_INIT */
}

#if defined( FBL_DEF_ENABLE_NON_KB_MAIN )
/***********************************************************************************************************************
 *  FblDiagPostInit
 **********************************************************************************************************************/
/*! \brief       Initialize diagnostic module
 *  \details     Finishes the initialization of the diagnostic module. NV-memory dependent initialization can
 *               be done here.
 **********************************************************************************************************************/
void FblDiagPostInit(void)
{

         /* Send programming session request response */
         if (FblMainGetStartFromAppl())
         {
            /* FBL started from application */
            FblDiagInitStartFromAppl();
         }

# if defined( FBL_ENABLE_SLEEPMODE )
         /* Initialize sleep mode counter */
         FblSleepCounterReload();
# endif

# if defined( FBL_DIAG_ENABLE_OEM_POSTINIT )
   /* OEM-specific initializations */
   FblDiagOemPostInit();
# endif /* FBL_DIAG_ENABLE_OEM_POSTINIT */
}
#endif /* FBL_DEF_ENABLE_NON_KB_MAIN */

/***********************************************************************************************************************
 *  FblDiagDeinit
 **********************************************************************************************************************/
/*! \brief       Function prepares for FBL shutdown on diagnostic service request.
 **********************************************************************************************************************/
static void FblDiagDeinit(void)
{
   /* Discard pending FblLib_Mem tasks */
   FblMemDeinit();

#if defined( FBL_DIAG_ENABLE_OEM_DEINIT )
   /* Shutdown OEM-specfic service handling */
   FblDiagOemDeinit();
#endif /* FBL_DIAG_ENABLE_OEM_DEINIT */

   if (GetDiagProgrammingSession())
   {
      /* ECU programming mode is active */
      ClrDiagProgrammingSession();
      if (GetMemDriverInitialized())
      {
         /* Remove memory driver from RAM-buffer */
         FblCwSetOfflineMode();

         (void)MemDriver_DeinitSync(V_NULL);

         FblCwSetOnlineMode();

         /* Reinitialize memory driver flash */
         ClrMemDriverInitialized();
      }

      /* Turn off programming voltage */
      ApplFblResetVfp();
   }

   ClrWaitEcuReset();   /* PRQA S 3109 */ /* MD_FblDiag_3109 */

   /* Clear all flags for service handling */
   FblDiagResetServiceFlags();

   /* Request shutdown from ComWrapper */
   FblCwShutdownRequest(kFblCwResetEcuRegularCase);
}

/***********************************************************************************************************************
 *  FblDiagStateTask
 **********************************************************************************************************************/
/*! \brief       This function handles the surveillance of the tester present timer.
 *  \details     When a timeout occurs the startup code of the FBL is executed.
 **********************************************************************************************************************/
void FblDiagStateTask( void )
{
   tFblResult result;

#if defined( FBL_DIAG_ENABLE_TASK_LOCKS )
   if (!GetDiagStateTaskRunning())
   {
      SetDiagStateTaskRunning();
#endif /* FBL_DIAG_ENABLE_TASK_LOCKS */

      /* Diagnostic service dispatcher. Will be executed when a service has to be managed */
      if (diagResponseFlag != kDiagResponseIdle)
      {
         /* Prepare return value */
         result = kFblOk;

         /* This is the initial call after reception of a service request */
         if (diagResponseFlag == kDiagResponseRequested)
         {
            /* Initialize security seed */
            (void)ApplFblSecuritySeedInit();

            /* Advance service processing state to repeated service */
            /* This will ensure that the service handler will be    */
            /* called until it has been concluded, e.g. by DiagProcessingDone() */
            diagResponseFlag = kDiagRepeatResponse;

            /* Search for a valid configured service in the service table */
            serviceInfo = FindServiceInfo(diagServiceCurrent);

            /* Check if service was found in service table */
            if (serviceInfo != (V_MEMROM1 tServiceInfoTable  V_MEMROM2 V_MEMROM3 *) V_NULL)
            {
               /* Service available. Check conditions */

               /* Check if service is allowed functionally */
               if (   GetFunctionalRequest()
                   && ((serviceInfo->sessionSupport & kDiagFunctionalRequestAllowed) == 0u)
                  )
               {
                  /* No functional request allowed for this service. */
                  /* Discard service and finish processing.         */
                  DiagSetNoResponse();

                  /* Prevent call of the main handler */
                  result = kFblFailed;
               }
               else
               {
                  /* Check valid parameter */
                  assertFblGen((serviceInfo->mainHandlerIdx < kSizeOfMainHandlerTable), kFblOemAssertMainHandlerIdxInvalid);  /* PRQA S 2214 */ /* MD_FblDiag_2214 */

                  /* Handler can be processed now */
                  serviceMainHandler = kServiceMainHandlerFctTable[serviceInfo->mainHandlerIdx];
#if defined( FBL_ENABLE_SERVICE_POSTHANDLER )
                  servicePostHandler = kServicePostHandlerFctTable[serviceInfo->postHandlerIdx];
#endif
                  result = CheckServicePreconditions(serviceInfo);

                  /* Check succeeded. Continue and invoke user check. */
                  if (result == kFblOk)
                  {
                     /* Service is basically allowed - response pending messages can be sent now */
                     SetRcrRpAllowed();

#if defined( FBL_ENABLE_SERVICE_PREHANDLER )
                  assertFblGen( ((serviceInfo->preHandlerIdx >= 0)
                     && (serviceInfo->preHandlerIdx<kServicePreHandlerFctTableSize)),
                     kFblOemAssertPreHandlerIdxInvalid);

                  /* Calling internal PreHandler */
                  if (kServicePreHandlerFctTable[serviceInfo->preHandlerIdx](&DiagBuffer[kDiagFmtSubparam],
                     (tCwDataLengthType)(DiagDataLength)) == kFblOk)
#endif
                     {
                        result = ApplFblCheckConditions(&DiagBuffer[kDiagFmtSubparam], (tCwDataLengthType)(DiagDataLength));
                     }
                     /* Indicate, that a user callback was called */
                     SetDiagHandlerCalled();
                  }
               }
            }
         }

         /* Don't call the main handler if an error has already been determined or reset is pending. */
         /* Thus, it will be called in case of NoResponse, RepeatedResponse and PutPosResponse.      */
         if ((result == kFblOk) && ((diagResponseFlag & kDiagPutNegResponse) == 0) && (!GetWaitEcuReset()))
         {
            assertFblInternal( (serviceMainHandler != kDiagSubFctNoMainHandler) , kFblSysAssertServiceMainHandlerInvalid); /* PRQA S 2214 */ /* MD_FblDiag_2214 */

            {
               /* Call main handler or repeated service call */
               result = (*serviceMainHandler)(&DiagBuffer[kDiagFmtSubparam], DiagDataLength);   /* PRQA S 3199 */ /* MD_FblDiag_3199 */

               /* Indicate, that a user callback was called */
               SetDiagHandlerCalled();
            }
         }

         /* Finish service processing if service has been concluded before */
         FblDiagResponseProcessor();

      }

      /* Resume stored FblLib_Mem operation */
      FblMemResumeIndication();

      /* Check for reset request */
      /* The following preconditions have to be fulfilled:
         * Reset message confirmed
         * Pre-reset function (e.g. flag written to NV-memory) has finished
      */
      if ((GetWaitEcuReset()) && (GetResetMsgConfirmed()) && (GetEcuResetFctFinished()))
      {
         FblDiagDeinit();
      }

#if defined( FBL_DIAG_ENABLE_OEM_STATETASK )
      FblDiagOemStateTask();
#endif /* FBL_DIAG_ENABLE_OEM_STATETASK */

#if defined( FBL_DIAG_ENABLE_TASK_LOCKS )
      /* Allow further task executions */
      ClrDiagStateTaskRunning();
   }
#endif /* FBL_DIAG_ENABLE_TASK_LOCKS */
}  /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

/***********************************************************************************************************************
 *  Diagnostic tasks
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  FblDiagTimerTask
 **********************************************************************************************************************/
/*! \brief       This function handles the surveillance of the tester present timer.
 *  \details     When a timeout occurs the startup code of the FBL is executed. Has to be executed from RAM
 *               if diagnostic functionality should be maintained while flashing, e.g. if pipelined programming
 *               is enabled.
 **********************************************************************************************************************/
void FblDiagTimerTask(void)
{
#if defined( FBL_DIAG_ENABLE_TASK_LOCKS )
   if (!GetDiagTimerTaskRunning())
   {
      SetDiagTimerTaskRunning();
#endif /* FBL_DIAG_ENABLE_TASK_LOCKS */
      /* Tester present timing */
      if (TimeoutTesterValue() != 0u)
      {
         DecTimeoutTesterValue();
         if (TimeoutTesterValue() == 0u)
         {
#if defined( FBL_DIAG_ENABLE_OEM_SESSION_TIMEOUT )
            FblDiagSessionTimeout();
#else
            /* Tester present timer expired, exit bootloader via reset */
            FblDiagEcuReset();
#endif
         }
      }

      /* ECU reset is pending */
      if (TimeoutEcuResetValue() != 0u)
      {
         DecTimeoutEcuResetValue();
         if (TimeoutEcuResetValue() == 0u)
         {
            /* ECU reset timer expired                       */
            /* Exit bootloader via Reset, but force no response!! */
            FblDiagEcuReset();
         }
      }

#if defined( FBL_DEF_ENABLE_NON_KB_MAIN  )
# if defined( FBL_ENABLE_SLEEPMODE )
            /* Sleep mode timer */
            if (FblGetSleepCounter() != 0u)
            {
               FblDecSleepCounter();
               if (FblGetSleepCounter() == 0u)
               {
                  ApplFblBusSleep();
                  FblSleepCounterReload();
               }
            }
# endif /* FBL_ENABLE_SLEEPMODE */
#endif /* FBL_DEF_ENABLE_NON_KB_MAIN */

#if defined( FBL_DIAG_ENABLE_OEM_TIMERTASK )
      FblDiagOemTimerTask();
#endif /* FBL_DIAG_ENABLE_OEM_TIMERTASK */

#if defined( FBL_DIAG_ENABLE_TASK_LOCKS )
      /* Allow further task executions */
      ClrDiagTimerTaskRunning();
   }
#endif /* FBL_DIAG_ENABLE_TASK_LOCKS */
}

/***********************************************************************************************************************
 *  Diagnostic kernel handling functions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  CheckSubparameter
 **********************************************************************************************************************/
/*! \brief       This function checks if all sub-parameters matches the values in DiagBuffer
 *  \pre         All service table entries are initialized properly.
 *  \param[in]   subFctParamIdTable Pointer to sub-parameter values
 *  \param[in]   paramId Parameter identifier to be checked.
 *  \return      kSubServiceValid: sub-function known and valid or Bit coded failures.
 *               kSubServiceParameterNotFound: No parameter found for this sub-service.
 *               kSubServiceFunctionNotFound:   No function found for this sub-service (first byte).
 **********************************************************************************************************************/
static vuint16 CheckSubparameter(V_MEMROM1 vuint8 V_MEMROM2 V_MEMROM3 * subFctParamIdTable, vsintx paramId)
{
   vuint16 result;

   result = kSubServiceValid;

   while (paramId > 0)
   {
      /* Decrement length indicator to become an index */
      paramId--;

      /* Typecasts are safe because while loop is only executed for positive values of paramId */
      if (DiagBuffer[kDiagFmtSubparam + (vuintx)paramId] != subFctParamIdTable[(vuintx)paramId])
      {
         if ((paramId == 0) && (GetServiceHasSubfunction()))
         {
            /* Assuming the sub-function is always the first byte */
            result |= kSubServiceFunctionNotFound;
         }
         else
         {
            result |= kSubServiceParameterNotFound;
         }
      }
   }
   return result;
}

/***********************************************************************************************************************
 *  CheckSubServiceTable
 **********************************************************************************************************************/
/*! \brief       This routine checks if the subfunction is supported.
 *  \details     Once all subfunction parameters have been checked, it's been checked if this subfunction
 *               is allowed in active session and length is correct.
 *               If a valid service-instance handler was found, it will be used instead of the main handler.
 *  \pre         Service available and valid configuration in the serviceInfoTable.
 *  \param[in]   subFctInfo Pointer to subfunction descriptor
 *  \param[in]   infoTableLen
 *  \param[in]   paramIdTableLen
 *  \return      kSubServiceValid: subfunction known and valid
 **********************************************************************************************************************/
static vuint16 CheckSubServiceTable(V_MEMROM1 tSubFctInfoTable V_MEMROM2 V_MEMROM3 * subFctInfo, vsintx infoTableLen, vsintx paramIdTableLen)
{
   vuint16 result;
   vuint16 resultSubParam;
   vuint16 subParState;

   /* Initialize return value */
   result = (kSubServiceParameterNotFound | kSubServiceFunctionNotFound | kSubServiceTableMismatch);

   subParTabIdx = 0u;
   subParState = 0u;

   /* Walk through the list of all listed sub-functions */
   while (infoTableLen > 0)
   {
      /* Decrement table length to become an index into the table */
      infoTableLen--;

      /* This is tricky: The subParamMask keeps the flag 'ParameterNotFound' or 'SubfunctionNotFound' to 0 if this once was found */
      resultSubParam = CheckSubparameter(subFctInfo[infoTableLen].subFctParamIdTable, paramIdTableLen);
      result = ((resultSubParam & result) | kSubServiceTableMismatch);

      /* Perform sub-service checks */
      if (resultSubParam == kSubServiceValid)
      {
         /* Parameter-ID matches. Sub-function condition checks can be done now. */
         result = kSubServiceValid;
         /* Overwrite Service session info with SubService session info
            Mainly for later security access check on SubService level
         */
         currentServiceSessionSupport = subFctInfo[infoTableLen].sessionSupport;

         /* Sub-function supported in active session ? */
         if ((subFctInfo[infoTableLen].sessionSupport & GetCurrentSession()) == 0u)
         {
            /* Set error bit that subFunction/-parameter not allowed in current session */
            result |= kSubServiceSessionFailure;
         }
         if ((subFctInfo[infoTableLen].sessionSupport & kDiagLengthSpecialCheck) == 0u)
         {
            /* Check for correct data length of this sub function */
            if (subFctInfo[infoTableLen].serviceLen != DiagDataLength)
            {
               /* Set the error bit that length doesn't match */
               result |= kSubServiceLenFailure;
            }
         }
         else
         {
            /* Dynamic length check needed for this sub-service */
            if (FblDiagCheckSpecialLength(&DiagBuffer[kDiagFmtpbDiagData], DiagDataLength, serviceInfo) != kFblOk)
            {
               /* Set the error bit that length doesn't match */
               result |= kSubServiceLenFailure;
            }
         }
         /* Check if sub-service is allowed functionally */
         if (   GetFunctionalRequest()
             && ((subFctInfo[infoTableLen].sessionSupport & kDiagFunctionalRequestAllowed) == 0u)
             && (GetFunctionalRequest())
            )
         {
            /* Set the error bit to indicate security access denied */
            result |= kSubServiceNoFuncSupport;
         }

         /* Check if sub-service is allowed functionally */
         if (   ((subFctInfo[infoTableLen].sessionSupport & kDiagSecuredService) != 0u)
             && (!GetSecurityUnlock())
            )
         {
            /* Set the error bit to indicate security access denied */
            result |= kSubServiceSecAccFailure;
         }

         /* All conditions are met. Overwrite mainHandler if a sub-function
            handler was given. Otherwise, use default main handler.
         */
         if (   (result == kSubServiceValid)
             && (subFctInfo[infoTableLen].mainInstanceHandlerIdx!=kServiceMainHandlerNoHandler)
            )
         {
            /* Get index from sub-function table */
            /* and set-up mainHandler.          */
            serviceMainHandler = kServiceMainHandlerFctTable[subFctInfo[infoTableLen].mainInstanceHandlerIdx];
         }

         /* !!Stop the loop by clearing the loop counter!! */
         infoTableLen = 0;
      }
      else
      {
         if (resultSubParam == kSubServiceFunctionNotFound)
         {
            /* The routine parameter is supported basically*/
            subParState |= kSubServiceParameterFound;

            /* Store index of sub-parameter table with mismatching sub-function.
               This is needed for RoutineControl service to do security level-
               and session checks.
            */
            subParTabIdx = (vuint8)infoTableLen;
         }

         result |= subParState;
      }
   }

   /* Return calculated status */
   return result;
}  /* PRQA S 6030 */ /* MD_MSR_STCYC */

/***********************************************************************************************************************
 *  CheckServiceTable
 **********************************************************************************************************************/
/*! \brief       This routine checks if the subfunction is supported.
 *  \details     Once all subfunction parameters have been checked, it's been checked if this subfunction
 *               is allowed in active session and length is correct.
 *               If a valid service-instance handler was found, it will be used instead of the main handler.
 *  \pre         Service available and valid configuration in the serviceInfoTable.
 *  \param[in]   pServiceInfo Pointer to service info table
 *  \return      kSubServiceValid: subfunction known and valid
 **********************************************************************************************************************/
static vuint16 CheckServiceTable(V_MEMROM1 tServiceInfoTable V_MEMROM2 V_MEMROM3 * pServiceInfo)
{
   vuint16 serviceCheckResult;

   /* Initialize variables */
   serviceCheckResult = kCheckValid;

   /* Initialize sessionSupport variable. Maybe overwritten for subservice */
   /* This may indicate further infos like securedService for subfunctions */
   currentServiceSessionSupport = pServiceInfo->sessionSupport;

   /* Check if service is supported in active session */
   if ((currentServiceSessionSupport & GetCurrentSession()) == 0u)
   {
      serviceCheckResult |= kServiceSessionFailure;
   }

   /* Minimum length check - done on service level */
   if (DiagDataLength < pServiceInfo->serviceLen)
   {
      serviceCheckResult |= kServiceMinLenFailure;
   }

   /* Security Access on service level */
   if (((currentServiceSessionSupport & kDiagSecuredService) != 0u) && (!GetSecurityUnlock()))
   {
      serviceCheckResult |= kServiceSecAccFailure;
   }

   /* Check length on service level */
   if ((pServiceInfo->sessionSupport & kDiagLengthSpecialCheck) != 0u)
   {
      /* Service requires dynamic length check */
      if (FblDiagCheckSpecialLength(&DiagBuffer[kDiagFmtpbDiagData], DiagDataLength, pServiceInfo) != kFblOk)
      {
         serviceCheckResult |= kServiceLenFailure;
      }
   }
   else
   {
      /* Constant length check */
      if (DiagDataLength != pServiceInfo->serviceLen)
      {
         serviceCheckResult |= kServiceLenFailure;
      }
   }

   return serviceCheckResult;
}

/***********************************************************************************************************************
 *  CheckServicePreconditions
 **********************************************************************************************************************/
/*! \brief       The function checks the pre-conditions configured in the service descriptor.
 *  \details     If not fulfilled, a neg. response will be issued and the function returns kFblFailed.
 *               If all conditions are met, the function returns kFblOk.
 *  \pre         Service available and valid configuration in the serviceInfoTable.
 *  \param[in]   pServiceInfo Pointer to service descriptor
 *  \return      kFblOk: Preconditions fulfilled, kFblFailed: Preconditions not fulfilled, neg. response issued.
 *               If all conditions are met, the function returns kFblOk.
 **********************************************************************************************************************/
static tFblResult CheckServicePreconditions(V_MEMROM1 tServiceInfoTable V_MEMROM2 V_MEMROM3* pServiceInfo)
{
   tFblResult result;
   vuint16 serviceCheckResult;

   /* Initialize variables */
   result = kFblOk;

   /* Do service level checks */
   serviceCheckResult = CheckServiceTable(pServiceInfo);

   /* Process suppress positive response bit */
   if ((pServiceInfo->sessionSupport & kDiagServiceIsSubfunction ) == kDiagServiceIsSubfunction)
   {
      SetServiceHasSubfunction();
      /* Sub-function may include suppress positive response bit */
      ChkSuppressPosRspMsgIndication(&DiagBuffer[kDiagFmtSubparam]);
   }
   else
   {
      ClrServiceHasSubfunction();
   }

   /* Process sub-function table */
   if (pServiceInfo->subFctParamIdTableLen != 0)
   {
      /* Remove error bits which are possibly set by service check */
      serviceCheckResult &= FblInvert16Bit(kCheckLenFailure);  /* PRQA S 0277 */ /* MD_FblDef_Invert */
      if (serviceCheckResult == kServiceFailure)
      {
         serviceCheckResult = 0u;
      }
      /* Check sub-service table */
      serviceCheckResult |= CheckSubServiceTable(pServiceInfo->subFctInfo,
                                                 pServiceInfo->subFunctionInfoLen,
                                                 pServiceInfo->subFctParamIdTableLen);
   }

   if (serviceCheckResult != kCheckValid)
   {
      /* Set return value */
      result = kFblFailed;

      /* Evaluate errors and set OEM-specific NRCs */
      if (pServiceInfo->serviceId == kDiagSidRoutineControl)
      {
         /* Special NRC processing for RoutineControl services */
         FblDiagProcessRcNrc(pServiceInfo, serviceCheckResult);
      }
      else if (GetServiceHasSubfunction())
      {
         /* NRC processing for service with sub-function */
         FblDiagProcessSubfunctionNrc(pServiceInfo, serviceCheckResult);
      }
      else if (pServiceInfo->subFctParamIdTableLen != 0)
      {
         /* NRC processing for service with sub-parameter */
         FblDiagProcessSubparamNrc(pServiceInfo, serviceCheckResult);
      }
      else
      {
         /* NRC processing for standard services */
         FblDiagProcessServiceNrc(pServiceInfo, serviceCheckResult);
      }
   }

   return result;
}

/***********************************************************************************************************************
 *  FindServiceInfo
 **********************************************************************************************************************/
/*! \brief       The function searches for the info table that corresponds to the service ID given in the parameter.
 *  \details     Returns pointer to the structure of the service descriptor
 *               for the given service ID. Otherwise returns 0.
 *  \param[in]   serviceId UDS service ID of requested service
 *  \return      Pointer to the service info table
 **********************************************************************************************************************/
static V_MEMROM1 tServiceInfoTable  V_MEMROM2 V_MEMROM3 * FindServiceInfo(vuint8 serviceId)
{
   V_MEMROM1 tServiceInfoTable V_MEMROM2 V_MEMROM3 * serviceInfoTable;
   vsintx i;

   /* Initialize variables */
   serviceInfoTable = (V_MEMROM1 tServiceInfoTable  V_MEMROM2 V_MEMROM3 *) V_NULL;

   /* Search for service information */
   for (i = (vsintx)(kSizeOfServiceTable - 1); i >= 0; i--)
   {
      if (serviceId == kServiceInfoTable[i].serviceId)
      {
         /* Service found */
         serviceInfoTable = &kServiceInfoTable[i];
         break;
      }
   }

   return serviceInfoTable;
}

/***********************************************************************************************************************
 *  Callback functions from fbl_main
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  FblDiagInitStartFromAppl
 **********************************************************************************************************************/
/*! \brief       Starts programming session if the Bootloader is called from the application.
 *  \details     This function prepares a programming session request and starts the diagnostic processing
 *               of this request after the real request has been received by the application software.
 **********************************************************************************************************************/
void FblDiagInitStartFromAppl(void)
{
   FblCwSetRxBlock();

   if (FblDiagRxGetPhysBuffer(kDiagRqlDiagnosticSessionControl + 1) == DiagBuffer)
   {
      /* Diagnostic session control is processed */
      DiagBuffer[kDiagFmtServiceId] = kDiagSidDiagnosticSessionControl;

#if defined( FBL_DIAG_ENABLE_OEM_SPS_REQUEST )
      /* Use OEM function to prepare programming session request */
      FblDiagOemPrepareSpsRequest(DiagBuffer, &DiagDataLength);
#else
      /* Prepare DiagBuffer for DiagnosticSessionControl service */
      DiagBuffer[kDiagFmtSubparam] = kDiagSubProgrammingSession;
      DiagDataLength = kDiagRqlDiagnosticSessionControl;
#endif /* FBL_DIAG_ENABLE_OEM_SPS_REQUEST */

      /* Set diagnostic session (extended session) */
      SetDiagExtendedSession();  /* PRQA S 3109 */ /* MD_FblDiag_3109 */

      /* Activate tester present timer */
      ResetTesterTimeout();

      /* Prepare response message address */
      if (FblCwPrepareResponseAddress() == kFblOk)
      {
         /* Indicate prepared buffer */
         FblDiagRxStartIndication();
         FblDiagRxIndication(DiagBuffer, kDiagRqlDiagnosticSessionControl + 1);

         /* Dispatch prepared buffer */
         FblDiagStateTask();

         /* Preconditions already checked by application */
         SetPreconditionsChecked();
      }
      else
      {  /* PRQA S 3201 */ /* MD_FblDiag_3201 */
         /* No address information found - cancel response */
         FblDiagConfirmation();
         FblCwResetRxBlock();
      }
   }
   else
   {
      FblCwResetRxBlock();
   }
}

/* Start section to execute code from RAM */
#define FBLDIAG_RAMCODE_START_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */
/***********************************************************************************************************************
 *  Callback functions from Communication Wrapper
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  FblDiagRxStartIndication
 **********************************************************************************************************************/
/*! \brief       Data reception call-back function to start diagnostic service processing.
 *  \details     Indicates start of a frame to diagnostic module. Tester present timeout is stopped and
 *               the diagnostic layer is locked until the service is processed.
 *  \pre         TP reception successful
 **********************************************************************************************************************/
void FblDiagRxStartIndication(void)
{
   /* Halts the S3 timer while receiving a diagnostic message */
   StopTesterTimeout();

   /* Indicate an ongoing service processing */
   SetServiceInProgress();

   DiagClrError();

   /* Reset handler */
   serviceMainHandler = FblDiagDefaultMainHandler;
#if defined( FBL_ENABLE_SERVICE_POSTHANDLER )
   servicePostHandler = FblDiagDefaultPostHandler;
#endif
   serviceInfo = (V_MEMROM1 tServiceInfoTable  V_MEMROM2 V_MEMROM3 *) V_NULL;
}

/***********************************************************************************************************************
 *  FblDiagRxIndication
 **********************************************************************************************************************/
/*! \brief       Data reception call-back function to start diagnostic service processing.
 *  \details     Valid TesterPresent requests will simply start the S3 timer and will not be processed.
 *  \pre         TP reception successful
 *  \param[in]   pbDiagBuffer Pointer to diagnostic buffer
 *  \param[in]   rxDataLength number of bytes to be received
 **********************************************************************************************************************/
void FblDiagRxIndication(vuint8* pbDiagBuffer, tCwDataLengthType rxDataLength)
{
   /* Initialize diagBuffer and data length */
   DiagBuffer = pbDiagBuffer;
   DiagDataLength = rxDataLength - 1u;

   /* Reset P2 timer (initial timing) */
#if defined( FBL_DIAG_ENABLE_DYNAMIC_P2_HANDLING )
   SetP2Timer(fblDiagTimeP2Max);
#else
   SetP2Timer(kFblDiagTimeP2);
#endif /* FBL_DIAG_ENABLE_DYNAMIC_P2_HANDLING */

   /* Handle TesterPresent request here */
   if (    (kDiagSidTesterPresent == DiagBuffer[kDiagFmtServiceId])
        && (kDiagSuppressPosRspMsgIndicationBit == DiagBuffer[kDiagFmtSubparam])
        && (kDiagRqlTesterPresent == DiagDataLength)
      )
   {
      /* service processing is not in progress, release the diagBuffer */
      FblDiagConfirmation();
      FblCwResetRxBlock();
   }
   else
   {
      /* Set diagnostic service indication to start service processing */
      diagResponseFlag = kDiagResponseRequested;
      diagServiceCurrent = DiagBuffer[kDiagFmtServiceId];

#if defined( FBL_MEM_ENABLE_PIPELINING )
      if (FblMemTaskIsPending())
      {
         FblMemRxNotification();
      }
#endif
   }

#if defined( FBL_ENABLE_SLEEPMODE )
   /* Request received - reload sleep counter */
   FblSleepCounterReload();
#endif
}

/***********************************************************************************************************************
 *  FblDiagRxGetPhysBuffer
 **********************************************************************************************************************/
/*! \brief       Diagnostic buffer management function for physical requests.
 *  \param[in]   rxDataLength Number of bytes to be received
 *  \return      Pointer to diagnostic buffer
 **********************************************************************************************************************/
vuint8* FblDiagRxGetPhysBuffer(tCwDataLengthType rxDataLength)
{
   vuint8* result;

   /* Initialize variables */
   result = (vuint8*)V_NULL;

   /* Check request length and if buffer is free */
   if ((rxDataLength > 0u )
        && (FBL_DIAG_BUFFER_LENGTH >= rxDataLength)
        && (!GetDiagBufferLocked())
      )
   {
      /* Initialize state variables */
      ClrFunctionalRequest();

      /* Lock buffer  */
      SetDiagBufferLocked();

      /* Return diagnostic buffer */
      result = DiagBuffer;
   }

   return result;
}

/***********************************************************************************************************************
 *  FblDiagRxGetFuncBuffer
 **********************************************************************************************************************/
/*! \brief       Diagnostic buffer management function for functional requests.
 *  \param[in]   rxDataLength Number of bytes to be received
 *  \return      Pointer to diagnostic buffer
 **********************************************************************************************************************/
vuint8* FblDiagRxGetFuncBuffer(tCwDataLengthType rxDataLength)
{
   /* Process functional request like physical request */
   vuint8* buffer = FblDiagRxGetPhysBuffer(rxDataLength);

   /* Set functional request indication */
   if (V_NULL != buffer)
   {
      SetFunctionalRequest();
   }

   return buffer;
}

/***********************************************************************************************************************
 *  FblDiagRxErrorIndication
 **********************************************************************************************************************/
/*! \brief       Indicates that an error has occurred during reception.
 *  \details     According to ISO-14229, now the S3 timer must be restarted.
 *  \pre         TP must been initialized
 **********************************************************************************************************************/
void FblDiagRxErrorIndication(void)
{
   /* Clear all other flags for service management */
   FblDiagConfirmation();
}

/***********************************************************************************************************************
 *  FblDiagTxErrorIndication
 **********************************************************************************************************************/
/*! \brief       Indicates that an error has occurred during transmission.
 *  \details     According to ISO-14229, now the S3 timer must be restarted.
 *  \pre         TP must been initialized
 *  \param[in]   cwMsgType Message type of indication, if it is a RCR-RP or normal message.
 *               The parameter type is defined in the CW-layer.
 **********************************************************************************************************************/
void FblDiagTxErrorIndication(vuint8 cwMsgType)
{
   /* Do not reset states for RCR-RP confirmation */
   if (cwMsgType != kFblCwMsgTypeRcrRp)
   {
      /* Clear all other flags for service management */
      FblDiagConfirmation();
   }
}

/***********************************************************************************************************************
 *  FblDiagTxConfirmation
 **********************************************************************************************************************/
/*! \brief       TP transmission call-back function.
 *  \pre         TP transmission successful
 *  \param[in]   cwMsgType Message type of indication, if it is a RCR-RP or normal message.
 *               The parameter type is defined in the CW-layer.
 **********************************************************************************************************************/
void FblDiagTxConfirmation(vuint8 cwMsgType)
{
   /* Do not reset states for RCR-RP confirmation */
   if (cwMsgType != kFblCwMsgTypeRcrRp)
   {
      /* Clear all other flags for service management */
      FblDiagConfirmation();
   }

   /* Diag wants to reset FBL after sending an RCR-RP */
   if (GetWaitEcuReset())
   {
      SetResetMsgConfirmed();
   }
}
/* End section to execute code from RAM */
# define FBLDIAG_RAMCODE_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/***********************************************************************************************************************
 *  Callback functions from FblLib_Mem
 **********************************************************************************************************************/

#if defined( FBL_DIAG_ENABLE_CORE_PREWRITE )
 /***********************************************************************************************************************
 *  FblDiagMemPreWrite
 **********************************************************************************************************************/
/*! \brief       Funktion performs actions before FblLibMem calls the memdriver write function.
 *  \return      kFblOk if PreWrite operations are successful, kFblFailed if PreWrite operations failed
 **********************************************************************************************************************/
tFblResult FblDiagMemPreWrite(void)
{
   FblCwSetOfflineMode();

   return kFblOk;
}
#endif /* FBL_DIAG_ENABLE_CORE_PREWRITE  */

#if defined( FBL_DIAG_ENABLE_CORE_POSTWRITE )
/***********************************************************************************************************************
 *  FblDiagMemPostWrite
 **********************************************************************************************************************/
/*! \brief       Checks if message is the platform message or not
 *  \return      kFblOk if PostWrite operations are successful, kFblFailed if Postwrite operations failed
 **********************************************************************************************************************/
tFblResult FblDiagMemPostWrite(void)
{
   FblCwSetOnlineMode();

   return kFblOk;
}
#endif /* FBL_DIAG_ENABLE_CORE_POSTWRITE  */

/***********************************************************************************************************************
 *  END OF FILE: FBL_DIAG_CORE.C
 **********************************************************************************************************************/
