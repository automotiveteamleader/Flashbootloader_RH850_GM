/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief        OEM dependent configuration for FblLib_Mem
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
 *  Joern Herwig                  JHg           Vector Informatik GmbH
 *  Achim Strobelt                Ach           Vector Informatik GmbH
 *  Andreas Wenckebach            AWh           Vector Informatik GmbH
 *  Dennis O'Donnell              Dod           Vector CANtech, Inc.
 *  Ralf Haegenlaeuer             HRf           Vector Informatik GmbH
 *  Sebastian Loos                Shs           Vector Informatik GmbH
 *  Thomas Mueller                ThM           Vector Informatik GmbH
 *  --------------------------------------------------------------------------------------------------------------------
 *  TEMPLATE REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  01.00.00   2012-03-23  JHg     -                Initial release
 *  01.00.01   2012-03-26  JHg     -                Additional comments
 *  01.01.00   2012-04-05  JHg     -                Changes after code review
 *  01.02.00   2012-04-27  JHg     -                Updated type casts in macros using ternary operator
 *  01.04.00   2013-02-01  Ach     ESCAN00062919    Adapted comments to use Doxygen
 *                         Ach     -                Correct macro __ApplFblMemDriverReady
 *                         JHg     ESCAN00064292    Added pre and post handlers for erase routine
 *                         JHg     ESCAN00064296    Renamed default preamble length define
 *                         JHg     ESCAN00064301    Split verify functions for RAM and ROM verification
 *                         JHg     ESCAN00064333    Differentiate watchdog trigger w/ and w/o status
 *                         JHg     ESCAN00064334    Added interface to remap error codes to OEM dependent value
 *                         JHg     -                Added additional error codes for new functionality
 *                         JHg     ESCAN00064720    Replaced __ApplFblMemCheckDfi by __ApplFblMemIsDataProcessingRequired
 *  01.05.00   2013-04-10  JHg     ESCAN00066379    Added interface version compatibility check
 *                         JHg     ESCAN00066377    Added additional error codes for new functionality
 *  01.06.00   2013-07-22  JHg     ESCAN00067433    Updated interface version (added FblMemDeinit)
 *                         JHg     ESCAN00069161    Updated interface version (added pipelined verification)
 *                                                   Changed verification error codes
 *  01.07.00   2013-08-16  JHg     ESCAN00069803    Updated interface version (enable/disable verification at run-time)
 *                         JHg     -                Corrected name of parameter type overwrite
 *  02.00.00   2013-12-12  JHg     ESCAN00072568    Updated interface version (initialize verification in segment start)
 *                                                   Error / status for BlockStartVerifyInit replaced by SegmentStartVerifyInit
 *                         JHg     ESCAN00072569    Raised major version to match API version
 *  02.01.00   2014-03-12  JHg     ESCAN00073504    Corrected overwrite macro names for verification functions
 *  03.00.00   2015-03-03  JHg     ESCAN00076591    Added support for external stream output
 *                                                   Added __ApplFblMemIsStreamOutputRequired
 *                                                   Added additional error codes
 *                                                   Updated interface version
 *                         JHg     ESCAN00081493    Added __ApplFblMemIsPipelinedProgrammingDisabled
 *  03.01.00   2015-04-23  JHg     ESCAN00082606    Added __ApplFblMemReportProgress
 *  03.01.03   2015-06-15  JHg     ESCAN00083392    Added FBL_MEM_ENABLE_VERIFY_OUTPUT_FULL_BLOCK_LENGTH
 *                                                  Added SEC_ENABLE_VERIFICATION_ADDRESS_LENGTH check
 *                                                  Updated interface version
 *  03.02.00   2015-07-21  JHg     ESCAN00084102    Updated interface version (parameter type structure for stream processing)
 *  04.00.00   2015-09-17  JHg     ESCAN00085249    Updated interface version (explicit parameters for processed verification)
 *                         JHg     ESCAN00085250    Run-time decision whether address and length info is included in verification
 *                                                  Added __ApplFblMemVerifyInputIsAddressAndLengthIncluded,
 *                                                   __ApplFblMemVerifyProcessedIsAddressAndLengthIncluded and
 *                                                   __ApplFblMemVerifyPipelinedIsAddressAndLengthIncluded
 *                         JHg     ESCAN00085251    Pass external segment info to __ApplFblMemIsPipelinedProgrammingDisabled
 *  04.01.00   2016-04-01  JHg     ESCAN00087997    Added __ApplFblMemEnterCriticalSection and __ApplFblMemLeaveCriticalSection
 *                                                   Updated interface version
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  03.00.00   2013-11-28  AWh     ESCAN00089528   Complete rework to allow usage for extended use cases
 *  03.01.00   2016-08-04  HRf     ESCAN00091043   No changes
 *                         AWh     ESCAN00091147   No changes
 *                                 ESCAN00091325   No changes
 *                                 ESCAN00091279   No changes
 *                                 ESCAN00089426   No changes
 *  03.01.01   2016-10-04  AWh     ESCAN00092167   No changes
 *  03.02.00   2016-10-31  ThM     ESCAN00092596   No changes
 **********************************************************************************************************************/

#ifndef __FBL_MEM_OEM_H__
#define __FBL_MEM_OEM_H__

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

/* PRQA S 3458 TAG_FblMem_3458_1 */ /* MD_CBD_19.4 */
/* PRQA S 3453 TAG_FblMem_3453_1 */ /* MD_CBD_19.7 */

/*-- Version information ----------------------------------------------------*/

/* Reference interface version */
/** Major interface version identifies incompatible changes */
#define FBL_MEM_API_REFERENCE_VERSION_MAJOR     0x04u
/** Minor interface version identifies backward compatible changes */
#define FBL_MEM_API_REFERENCE_VERSION_MINOR     0x01u
/** Release interface version identifies cosmetic changes */
#define FBL_MEM_API_REFERENCE_VERSION_RELEASE   0x00u

/*-- Buffer configuration ---------------------------------------------------*/

/** Input buffer size */
#define FBL_MEM_BUFFER_SIZE               FBL_DIAG_BUFFER_LENGTH
#if defined( MCMP_MODE_SLAVE )
/** Length of default preamble */
# define FBL_MEM_DEFAULT_PREAMBLE_LENGTH   1u
/** Maximum preamble length */
# define FBL_MEM_MAX_PREAMBLE_LENGTH       1u
#else
/** Length of default preamble */
# define FBL_MEM_DEFAULT_PREAMBLE_LENGTH   FBL_MEM_DEFAULT_PREAMBLE_LENGTH_DIAG
/** Maximum preamble length */
# define FBL_MEM_MAX_PREAMBLE_LENGTH       FBL_MEM_MAX_PREAMBLE_LENGTH_DIAG
#endif
#if defined( FBL_MEM_ENABLE_STREAM_OUTPUT )
/* Allow override in configuration */
# if defined( __ApplFblMemIsDataProcessingRequired )
# else
/** Check whether specific data format identifier implies the need for data processing */
#  define __ApplFblMemIsDataProcessingRequired(dataFormat) \
   ((tFblResult)(((kDiagSubNoDataProcessing == (dataFormat)) || ApplFblGetOemStreamModeSupported(dataFormat)) ? kFblFailed : kFblOk))
# endif /* __ApplFblMemIsDataProcessingRequired */
/* Allow override in configuration */
# if defined( __ApplFblMemIsStreamOutputRequired )
# else
/** Check whether specific data format identifier implies the need for stream output */
#  define __ApplFblMemIsStreamOutputRequired(dataFormat) \
   ((tFblResult)((ApplFblGetOemStreamModeSupported(dataFormat)) ? kFblOk : kFblFailed))
# endif /* __ApplFblMemIsStreamOutputRequired */
#else
/* Allow override in configuration */
# if defined( __ApplFblMemIsDataProcessingRequired )
# else
# define __ApplFblMemIsDataProcessingRequired(dataFormat)    ((tFblResult)(((dataFormat) == MODULE_DF_RAW) ? kFblFailed : kFblOk))
# endif /* __ApplFblMemIsDataProcessingRequired */
#endif /* FBL_MEM_ENABLE_STREAM_OUTPUT */

/*-- Verification -----------------------------------------------------------*/
#if defined( SEC_ENABLE_VERIFICATION_ADDRESS_LENGTH )
/** Include address and length info in on-the-fly verification on input data */
# define FBL_MEM_ENABLE_VERIFY_INPUT_ADDRESS_LENGTH
/** Include address and length info in on-the-fly verification on processed data */
#  define FBL_MEM_ENABLE_VERIFY_PROCESSED_ADDRESS_LENGTH
/** Include address and length info in pipelined verification */
#  define FBL_MEM_ENABLE_VERIFY_PIPELINED_ADDRESS_LENGTH
#else
# if defined( SEC_VER_DISABLE_LENGTH_AND_ADDRESS_INPUT )
# else
#  error "Not supported by this SLP."
/** Include address and length info in on-the-fly verification on input data */
#  define FBL_MEM_ENABLE_VERIFY_INPUT_ADDRESS_LENGTH
/** Include address and length info in on-the-fly verification on processed data */
#  define FBL_MEM_ENABLE_VERIFY_PROCESSED_ADDRESS_LENGTH
/** Include address and length info in pipelined verification */
#  define FBL_MEM_ENABLE_VERIFY_PIPELINED_ADDRESS_LENGTH
# endif /* SEC_VER_DISABLE_LENGTH_AND_ADDRESS_INPUT */
#endif /* SEC_ENABLE_VERIFICATION_ADDRESS_LENGTH */

#if defined( FBL_MEM_ENABLE_VERIFY_PIPELINED_ADDRESS_LENGTH )
/** Run-time decision whether address and length info is included in pipelined verification
   This is just an example for the macro definition
   Remove macro if not applicable */
# define __ApplFblMemVerifyPipelinedIsAddressAndLengthIncluded(pBlockInfo, pSegmentInfo) \
   ((kDiagSubNoDataProcessing == ((pSegmentInfo)->dataFormat)) ? kFblOk : kFblFailed)
#endif /* FBL_MEM_ENABLE_VERIFY_PIPELINED_ADDRESS_LENGTH */

#if !defined( FBL_MEM_DISABLE_VERIFY_PIPELINED )
  /* The Gm specifications default pipelined verification */
# define FBL_MEM_ENABLE_VERIFY_PIPELINED
#endif

/* Allow for multi segments in Diagbuffer */
#define FBL_MEM_ENABLE_SEGMENTED_INPUT_BUFFER
#if defined( FBL_MEM_ENABLE_VERIFY_OUTPUT )
#else
# define FBL_MEM_ENABLE_VERIFY_OUTPUT
#endif

/* GM SLP6 uses its own Gap fill implementation. */
#define FBL_MEM_DISABLE_GAP_FILL

/** Calculate CRC total over complete block
    Otherwise start of block until last address of last segment is covered */
#define FBL_MEM_ENABLE_VERIFY_OUTPUT_FULL_BLOCK_LENGTH

/** Size of temporary verification result buffer */
#define FBL_MEM_SIG_RESULT_BUFFER_SIZE                   kDiagSigBufLength
/* Defines macros to overwrite default defines / macros required for verification:
#define FBL_MEM_VERIFY_STATUS_TYPE_OVERWRITE             tSecStatus
#define FBL_MEM_VERIFY_PARAM_TYPE_INPUT_OVERWRITE        tSecSignatureParam
#define FBL_MEM_VERIFY_PARAM_TYPE_OUTPUT_OVERWRITE       tSecVerifyParam
#define FBL_MEM_VERIFY_LENGTH_TYPE_OVERWRITE             uint32_t
#define FBL_MEM_VERIFY_SIZE_TYPE_OVERWRITE               FBL_MEMSIZE_TYPE
#define FBL_MEM_VERIFY_ADDRESS_TYPE_OVERWRITE            FBL_ADDR_TYPE
#define FBL_MEM_VERIFY_DATA_PTR_TYPE_OVERWRITE           V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 *
#define FBL_MEM_VERIFY_WD_FCT_TYPE_OVERWRITE             tFblRealtimeFct
#define FBL_MEM_VERIFY_READ_FCT_TYPE_OVERWRITE           tReadMemoryFct
#define FBL_MEM_SEGMENT_LIST_TYPE_OVERWRITE              tSegmentList
#define FBL_MEM_SEGMENT_LIST_ENTRY_TYPE_OVERWRITE        tSegmentInfo
#define FBL_MEM_VERIFY_OK_OVERWRITE                      kSecOk
#define FBL_MEM_VERIFY_STATE_INIT_OVERWRITE              SECM_HASH_INIT
#define FBL_MEM_VERIFY_STATE_COMPUTE_OVERWRITE           SECM_HASH_COMPUTE
#define FBL_MEM_VERIFY_STATE_FINALIZE_OVERWRITE          SECM_HASH_FINALIZE
#define FBL_MEM_VERIFY_STATE_VERIFY_OVERWRITE            SECM_SIG_FINALIZE
#define FBL_MEM_VERIFY_FCT_INPUT_TYPE_OVERWRITE(type)    FBL_CALL_TYPE tSecStatus (*type) (V_MEMRAM1 tSecSignatureParam V_MEMRAM2 V_MEMRAM3 *);
#define FBL_MEM_VERIFY_FCT_OUTPUT_TYPE_OVERWRITE(type)   FBL_CALL_TYPE tSecStatus (*type) (V_MEMRAM1 tSecVerifyParam V_MEMRAM2 V_MEMRAM3 *);
*/

/*-- Memory driver ----------------------------------------------------------*/
#if defined( FBL_ENABLE_MULTIPLE_MEM_DEVICES )
/*
   Check whether specific memory driver is ready
   Remove macro if not applicable
*/
/* not used for Gm SLP6, just globally track in Diag Oem layer "MemDriverInitialized" */
/* # define __ApplFblMemDriverReady(segment)          ((tFblResult)((GetMemDriverReady(FlashBlock[(segment)].device)) ? kFblOk : kFblFailed)) */
#endif
/*
   Perform actions directly before and after memory driver write
   Remove macro if not applicable
*/
/*#define __ApplFblMemPreWrite()                     FblDiagMemPreWrite()
#define __ApplFblMemPostWrite()                    FblDiagMemPostWrite()*/
/*
   Perform actions directly before and after memory driver erase
   Remove macro if not applicable
*/
/*#define __ApplFblMemPreErase()                     FblDiagMemPreWrite()
#define __ApplFblMemPostErase()                    FblDiagMemPostWrite()*/
/*
   Perform actions directly after segment end indication
   Remove macro if not applicable
*/
/* #define __ApplFblMemPostSegmentEnd() */

/*-- Watchdog ---------------------------------------------------------------*/
/* Default return code */
#define FBL_MEM_WD_TRIGGER_DEFAULT                 (FBL_NO_TRIGGER)
/*
   Overwrite status type returned by watchdog trigger function
   Remove macro if not applicable
*/
/* #define FBL_MEM_TRIGGER_STATUS_OVERWRITE           vuint8 */

/*
   Default watchdog trigger
   Remove macro if not applicable
*/
#if !defined( __ApplFblMemWdTrigger )
# define __ApplFblMemWdTrigger()                    FblLookForWatchdog()
#endif

#if defined( MCMP_MODE_SLAVE )
#else
/* Watchdog trigger including adaptive generation of RCR-RP
   Remove macro if not applicable */
# if !defined( __ApplFblMemAdaptiveRcrRp )
#  define __ApplFblMemAdaptiveRcrRp()                FblRealTimeSupport()
# endif
/* Check whether RCR-RP is already active
   Remove macro if not applicable or required (e.g. if kForceSendRpIfNotInProgress is available) */
# if !defined( __ApplFblMemIsRcrRpActive )
#  define __ApplFblMemIsRcrRpActive()                ((tFblResult)(GetRcrRpInProgress() ? kFblOk : kFblFailed))
# endif
/* Forced generation of RCR-RP (hint: use kForceSendRpIfNotInProgress if available)
   Remove macro if not applicable */
# if !defined( __ApplFblMemForcedRcrRp )
#  define __ApplFblMemForcedRcrRp()                  DiagExRCRResponsePending(kForceSendResponsePending)
# endif
#endif /* MCMP_MODE_SLAVE */

/* Pass watchdog trigger with extended status result to data processing */
#define FBL_MEM_ENABLE_EXT_TRIGGER_DATA_PROC
/* Pass watchdog trigger with extended status result to signature verification over input data */
#define FBL_MEM_DISABLE_EXT_TRIGGER_INPUT_VERIFY
/* Pass watchdog trigger with extended status result to signature verification over output data */
#define FBL_MEM_DISABLE_EXT_TRIGGER_OUTPUT_VERIFY

/*-- Progress information ---------------------------------------------------*/
#if defined( FBL_MEM_ENABLE_PROGRESS_INFO )
/* Allow override in configuration */
# if defined( __ApplFblMemReportProgress )
# else
/** Progress information reporting */
/*#  define __ApplFblMemReportProgress(progressInfo)*/
# endif /* __ApplFblMemReportProgress */
#endif /* FBL_MEM_ENABLE_PROGRESS_INFO */

/*-- Critical sections ------------------------------------------------------*/
/* Allow override in configuration */
#if defined( __ApplFblMemEnterCriticalSection )
#else
/** Enter critical section */
#  define __ApplFblMemEnterCriticalSection()
#endif /* __ApplFblMemEnterCriticalSection */

/* Allow override in configuration */
#if defined( __ApplFblMemLeaveCriticalSection )
#else
/** Leave critical section */
#  define __ApplFblMemLeaveCriticalSection()
#endif /* __ApplFblMemLeaveCriticalSection */

/*-- Error handling ---------------------------------------------------------*/
/** Overwrite error codes by OEM dependent value */
#define FBL_MEM_ENABLE_STATUS_OVERWRITE
/* Enable interface to remap error codes to OEM dependent value */
#define FBL_MEM_DISABLE_STATUS_REMAPPING
/* OEM dependent status type */
#define FBL_MEM_STATUS_TYPE                           vuint8

#if defined( FBL_MEM_ENABLE_STATUS_OVERWRITE ) 
/* Default */
# define FBL_MEM_STATUS_Ok                            HdrErr_NoError
# define FBL_MEM_STATUS_Failed                        HdrErr_ExtNrcConditionsNotCorrect
/* BlockErase indication */
# define FBL_MEM_STATUS_BlockEraseSequence            HdrErr_ExtNrcRequestSequenceError       /* cast on 0x31, if LibMem check FblMemCheckAllowed fails */
/* BlockStart indication */
# define FBL_MEM_STATUS_BlockStartSequence            HdrErr_ExtNrcRequestSequenceError       /* cast on 0x36, if LibMem check FblMemCheckAllowed fails */
# define FBL_MEM_STATUS_BlockStartParam               HdrErr_ExtNrcConditionsNotCorrect       /* not used in current configuration. */
/* BlockEnd indication */
# define FBL_MEM_STATUS_BlockEndSequence              HdrErr_ExtNrcRequestSequenceError       /* cast on 0x37, if LibMem check FblMemCheckAllowed fails */
# define FBL_MEM_STATUS_BlockEndVerifyFinalize        HdrErr_ExtNrcGeneralProgrammingFailure  /* not used in current configuration. */
/* BlockVerify */
# define FBL_MEM_STATUS_BlockVerifySequence           HdrErr_ExtNrcRequestSequenceError       /* cast on 0x37, if LibMem check FblMemCheckAllowed fails */
# define FBL_MEM_STATUS_BlockVerifyInputVerify        HdrErr_ExtNrcGeneralProgrammingFailure  /* not used in current configuration. */
# define FBL_MEM_STATUS_BlockVerifyProcessedVerify    HdrErr_ExtNrcGeneralProgrammingFailure  /* not used in current configuration. */
# define FBL_MEM_STATUS_BlockVerifyPipeVerify         HdrErr_ExtNrcGeneralProgrammingFailure  /* cast on 0x37, if signature is wrong */
# define FBL_MEM_STATUS_BlockVerifyOutputVerify       HdrErr_ExtNrcGeneralProgrammingFailure  /* cast on 0x37, if digest is wrong */
/* SegmentStart indication */
# define FBL_MEM_STATUS_SegmentStartSequence          HdrErr_ExtNrcRequestSequenceError       /* cast on 0x36, if LibMem check FblMemCheckAllowed fails */
# define FBL_MEM_STATUS_SegmentStartDataProcInit      HdrErr_ExtNrcRequestOutOfRange          /* cast on 0x36, if Data Processing Init fails */
# define FBL_MEM_STATUS_SegmentStartStreamOutInit     HdrErr_ExtNrcRequestOutOfRange          /* not used in current configuration. */
# define FBL_MEM_STATUS_SegmentStartVerifyInit        HdrErr_ExtNrcRequestOutOfRange          /* cast on 0x36, if verification fails */
# define FBL_MEM_STATUS_SegmentStartVerifyCompute     HdrErr_ExtNrcGeneralProgrammingFailure  /* not used in current configuration. */
# define FBL_MEM_STATUS_SegmentStartSegmentCount      HdrErr_ExtNrcRequestOutOfRange          /* cast on 0x36, if number of max libmem segments fails */
/* SegmentEnd indication */
# define FBL_MEM_STATUS_SegmentEndSequence            HdrErr_ExtNrcRequestSequenceError       /* cast on 0x36, if LibMem check FblMemCheckAllowed fails */
# define FBL_MEM_STATUS_SegmentEndInsufficientData    HdrErr_ExtNrcRequestSequenceError       /* cast on 0x36, if Segment is not fully programmed but should be */
# define FBL_MEM_STATUS_SegmentEndPost                HdrErr_ExtNrcConditionsNotCorrect       /* not used in current configuration. */
/* Data indication */
# define FBL_MEM_STATUS_DataIndSequence               HdrErr_ExtNrcRequestSequenceError       /* cast on 0x36, if LibMem check FblMemCheckAllowed fails */
# define FBL_MEM_STATUS_DataIndParam                  HdrErr_ExtNrcGeneralProgrammingFailure  /* cast on 0x36, if DiagBuffer is corrupted */
# define FBL_MEM_STATUS_DataIndOverflow               HdrErr_LengthExceeded                   /* cast on 0x36, if more data requested than declared previously */
/* Data processing */
# define FBL_MEM_STATUS_DataProc                      HdrErr_ExtNrcGeneralProgrammingFailure  /* cast on 0x36, if Data Processing fails */
# define FBL_MEM_STATUS_DataProcConsume               HdrErr_ExtNrcGeneralProgrammingFailure  /* cast on 0x36, if Data Processing fails */
# define FBL_MEM_STATUS_DataProcDeinit                HdrErr_ExtNrcConditionsNotCorrect       /* cast on 0x36, if Data Processing deinit fails */
/* Stream output */
# define FBL_MEM_STATUS_StreamOutput                  HdrErr_ExtNrcGeneralProgrammingFailure  /* not used in current configuration. */
# define FBL_MEM_STATUS_StreamOutputConsume           HdrErr_ExtNrcGeneralProgrammingFailure  /* not used in current configuration. */
# define FBL_MEM_STATUS_StreamOutputOverflow          HdrErr_LengthExceeded                   /* not used in current configuration. */
# define FBL_MEM_STATUS_StreamOutputDeinit            HdrErr_ExtNrcConditionsNotCorrect       /* not used in current configuration. */
/* Memory driver */
# define FBL_MEM_STATUS_DriverResumeWrite             HdrErr_FlashWrite                       /* not used in current configuration. */
# define FBL_MEM_STATUS_DriverWrite                   HdrErr_FlashWrite                       /* cast on 0x36, if Writing fails */
# define FBL_MEM_STATUS_DriverErase                   HdrErr_ExtNrcGeneralProgrammingFailure  /* cast on 0x31, if Erasure fails, Pec overwritten in FblHdrEraseRoutine */
# define FBL_MEM_STATUS_DriverRemainder               HdrErr_FlashWrite                       /* not used in current configuration. */
# define FBL_MEM_STATUS_DriverSuspendWrite            HdrErr_FlashWrite                       /* not used in current configuration. */
/* Programming operation */
# define FBL_MEM_STATUS_ProgramOverflow               HdrErr_LengthExceeded                   /* cast on 0x36/0x37, if Writing fails */
# define FBL_MEM_STATUS_ProgramOutsideFbt             HdrErr_FlashWrite                       /* cast on 0x36/0x37, if FBT check fails */
# define FBL_MEM_STATUS_ProgramUnalignedAddress       HdrErr_FlashWrite                       /* cast on 0x36/0x37, if alignment check fails */
# define FBL_MEM_STATUS_ProgramDriverNotReady         HdrErr_ExtFlashWriteDriverNotReady      /* not used in current configuration. */
# define FBL_MEM_STATUS_ProgramPreWrite               HdrErr_FlashWrite                       /* not used in current configuration. */
# define FBL_MEM_STATUS_ProgramPostWrite              HdrErr_FlashWrite                       /* not used in current configuration. */
/* Erase operation */
# define FBL_MEM_STATUS_EraseOutsideFbt               HdrErr_ExtNrcGeneralProgrammingFailure  /* cast on 0x31 if FBT check fails, Pec overwritten in FblHdrEraseRoutine */
# define FBL_MEM_STATUS_EraseDriverNotReady           HdrErr_ExtEraseDriverNotReady           /* not used in current configuration. */
# define FBL_MEM_STATUS_ErasePreErase                 HdrErr_ExtNrcGeneralProgrammingFailure  /* not used in current configuration. */
# define FBL_MEM_STATUS_ErasePostErase                HdrErr_ExtNrcGeneralProgrammingFailure  /* not used in current configuration. */
/* On-the-fly verification */
# define FBL_MEM_STATUS_VerifyCompute                 HdrErr_ExtNrcGeneralProgrammingFailure  /* cast on 0x36/0x37, if verification fails */
# define FBL_MEM_STATUS_VerifyFinalize                HdrErr_ExtNrcGeneralProgrammingFailure  /* cast on 0x36/0x37, if verification finilaze fails */
/* Pass-through */
# define FBL_MEM_STATUS_PassThroughLocal              HdrErr_ExtNrcConditionsNotCorrect       /* not used in current configuration. */
# define FBL_MEM_STATUS_PassThroughRemote             HdrErr_ExtNrcConditionsNotCorrect       /* not used in current configuration. */
#endif /* FBL_MEM_ENABLE_STATUS_OVERWRITE || FBL_MEM_ENABLE_STATUS_REMAPPING */

#if defined( FBL_ENABLE_DEBUG_STATUS )
/** Enable extended error status handling */
# define FBL_MEM_ENABLE_EXT_STATUS

/* Default */
# define FBL_MEM_EXT_STATUS_Ok(status)
# define FBL_MEM_EXT_STATUS_Failed(status)
/* BlockErase indication */
# define FBL_MEM_EXT_STATUS_BlockEraseSequence(status)
/* BlockStart indication */
# define FBL_MEM_EXT_STATUS_BlockStartSequence(status)
# define FBL_MEM_EXT_STATUS_BlockStartParam(status)
/* BlockEnd indication */
# define FBL_MEM_EXT_STATUS_BlockEndSequence(status)
# define FBL_MEM_EXT_STATUS_BlockEndVerifyFinalize(status)
/* BlockVerify */
# define FBL_MEM_EXT_STATUS_BlockVerifySequence(status)
# define FBL_MEM_EXT_STATUS_BlockVerifySigVerify(status)
# define FBL_MEM_EXT_STATUS_BlockVerifyInputVerify(status)
# define FBL_MEM_EXT_STATUS_BlockVerifyProcessedVerify(status)
# define FBL_MEM_EXT_STATUS_BlockVerifyPipeVerify(status)
# define FBL_MEM_EXT_STATUS_BlockVerifyOutputVerify(status)
/* SegmentStart indication */
# define FBL_MEM_EXT_STATUS_SegmentStartSequence(status)
# define FBL_MEM_EXT_STATUS_SegmentStartDataProcInit(status)
# define FBL_MEM_EXT_STATUS_SegmentStartStreamOutInit(status)
# define FBL_MEM_EXT_STATUS_SegmentStartVerifyInit(status)
# define FBL_MEM_EXT_STATUS_SegmentStartVerifyCompute(status)
# define FBL_MEM_EXT_STATUS_SegmentStartSegmentCount(status)
/* SegmentEnd indication */
# define FBL_MEM_EXT_STATUS_SegmentEndSequence(status)
# define FBL_MEM_EXT_STATUS_SegmentEndInsufficientData(status)
# define FBL_MEM_EXT_STATUS_SegmentEndPost(status)
/* Data indication */
# define FBL_MEM_EXT_STATUS_DataIndSequence(status)
# define FBL_MEM_EXT_STATUS_DataIndParam(status)
# define FBL_MEM_EXT_STATUS_DataIndOverflow(status)
/* Data processing */
# define FBL_MEM_EXT_STATUS_DataProc(status)
# define FBL_MEM_EXT_STATUS_DataProcConsume(status)
# define FBL_MEM_EXT_STATUS_DataProcDeinit(status)
/* Stream output */
# define FBL_MEM_EXT_STATUS_StreamOutput(status)
# define FBL_MEM_EXT_STATUS_StreamOutputConsume(status)
# define FBL_MEM_EXT_STATUS_StreamOutputOverflow(status)
# define FBL_MEM_EXT_STATUS_StreamOutputDeinit(status)
/* Memory driver */
# define FBL_MEM_EXT_STATUS_DriverResumeWrite(status)
/* Status contains flash driver error code */
# define FBL_MEM_EXT_STATUS_DriverWrite(status)                {FblHdrDebugStatus(0x8000u|(status)); /* PRQA S 3109 */ /* MD_FblHdr_Debug_3109 */}
# define FBL_MEM_EXT_STATUS_DriverErase(status)                {FblHdrDebugStatus(0x8000u|(status)); /* PRQA S 3109 */ /* MD_FblHdr_Debug_3109 */}
# define FBL_MEM_EXT_STATUS_DriverRemainder(status)
# define FBL_MEM_EXT_STATUS_DriverSuspendWrite(status)
/* Programming operation */
# define FBL_MEM_EXT_STATUS_ProgramOverflow(status)
# define FBL_MEM_EXT_STATUS_ProgramOutsideFbt(status)          {FblHdrDebugStatus(kDiagErrNoMemoryRegionFound|(status)); /* PRQA S 3109 */ /* MD_FblHdr_Debug_3109 */}
# define FBL_MEM_EXT_STATUS_ProgramUnalignedAddress(status)    {FblHdrDebugStatus(kDiagErrCodeNotAligned|(status)); /* PRQA S 3109 */ /* MD_FblHdr_Debug_3109 */}
# define FBL_MEM_EXT_STATUS_ProgramDriverNotReady(status)      {FblHdrDebugStatus(kDiagErrFlashcodeInitFailed|(status)); /* PRQA S 3109 */ /* MD_FblHdr_Debug_3109 */}
# define FBL_MEM_EXT_STATUS_ProgramPreWrite(status)
# define FBL_MEM_EXT_STATUS_ProgramPostWrite(status)
/* Erase operation */
# define FBL_MEM_EXT_STATUS_EraseOutsideFbt(status)            {FblHdrDebugStatus(kDiagErrNoMemoryRegionFound|(status)); /* PRQA S 3109 */ /* MD_FblHdr_Debug_3109 */}
# define FBL_MEM_EXT_STATUS_EraseDriverNotReady(status)        {FblHdrDebugStatus(kDiagErrFlashcodeInitFailed|(status)); /* PRQA S 3109 */ /* MD_FblHdr_Debug_3109 */}
# define FBL_MEM_EXT_STATUS_ErasePreErase(status)
# define FBL_MEM_EXT_STATUS_ErasePostErase(status)
/* On-the-fly verification */
# define FBL_MEM_EXT_STATUS_VerifyCompute(status)
# define FBL_MEM_EXT_STATUS_VerifyFinalize(status)
/* Pass-through */
# define FBL_MEM_EXT_STATUS_PassThroughLocal(status)
# define FBL_MEM_EXT_STATUS_PassThroughRemote(status)

/* Extended info: status contains current address */
# define FBL_MEM_EXT_STATUS_ProgramAddress(status)
# define FBL_MEM_EXT_STATUS_EraseAddress(status)
#endif /* FBL_ENABLE_DEBUG_STATUS */

/* PRQA L:TAG_FblMem_3458_1 */
/* PRQA L:TAG_FblMem_3453_1 */

#endif /* __FBL_MEM_OEM_H__ */

/***********************************************************************************************************************
 *  END OF FILE: FBL_MEM_OEM.H
 **********************************************************************************************************************/

