/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief        Library containing common functionality for memory programming:
 *                - Erase of memory region
 *                - Data processing (e.g. decryption and decompression)
 *                - Segmentation and alignment
 *                - Signature / checksum verification over RAM contents
 *                - Pipelined programming
 *
 *                Used by OEM dependent diagnostics to program incoming download data.
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
 *  Alexander Starke              ASe           Vector Informatik GmbH
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  01.00.00   2012-03-23  JHg     -                Initial release
 *  01.00.01   2012-03-26  JHg     -                Added description to file header
 *  01.01.00   2012-04-05  JHg     ESCAN00057963    No changes
 *                                                  Changes and optimizations after code review
 *  01.02.00   2012-04-27  JHg     ESCAN00058452    Added support for processed length
 *                                 ESCAN00058453    Removed fbl_mem_cfg.h include
 *                                 ESCAN00058621    No changes
 *  01.03.00   2012-06-22  JHg     ESCAN00059475    No changes
 *                                 ESCAN00059477    No changes
 *  01.04.00   2013-02-01  JHg     ESCAN00064290    Extensions for multi processor systems (pass-through)
 *                                                   Rework of block start indication interface
 *                                                   Pass additional info in structure
 *                         JHg     ESCAN00064292    Added block erase
 *                         JHg     ESCAN00064296    Preamble length switchable at runtime
 *                         JHg     ESCAN00064301    Added signature verification on ROM contents
 *                                                   Rework of block verify interface
 *                                                   Keep track of segment history (structure provided externally)
 *                         JHg     -                Added additional error codes for new functionality
 *                         JHg     ESCAN00061335    No changes
 *                         JHg     ESCAN00064333    No changes
 *                         JHg     ESCAN00061764    No changes
 *                         JHg     ESCAN00061814    No changes
 *                         JHg     ESCAN00064338    Added post handler for SegmentEndIndication
 *                         Ach     ESCAN00062919    Adapted comments to use Doxygen
 *                         JHg     ESCAN00064334    Added interface to remap error codes to OEM dependent value
 *                         JHg     ESCAN00064339    Segmented data processing: data produced during one cycle may be
 *                                                   smaller than buffer size
 *                         JHg     ESCAN00064330    Explicit resume of suspended operation
 *                                                   Added state "suspend pending"
 *                                                   Added active operation check
 *                         JHg     ESCAN00064343    Added support for multiple input sources
 *                         JHg     -                Added additional error codes for new functionality
 *                         JHg     -                Additional integration time checks
 *                         JHg     ESCAN00064543    No changes
 *                         JHg     ESCAN00064720    Replaced __ApplFblMemCheckDfi by __ApplFblMemIsDataProcessingRequired
 *  01.05.00   2013-04-10  JHg     ESCAN00064871    No changes
 *                         JHg     ESCAN00064890    No changes
 *                         JHg     -                No changes
 *                         JHg     ESCAN00065830    No changes
 *                         JHg     ESCAN00066375    No changes
 *                         JHg     ESCAN00066377    Encapsulate erase functionality
 *                         JHg     ESCAN00066379    Added interface version compatibility check
 *                         JHg     ESCAN00066380    Exported FblMemInit
 *  01.06.00   2013-07-22  ASe     ESCAN00066743    No changes
 *                         JHg     ESCAN00067433    Added FblMemDeinit
 *                                                  Updated interface version
 *                         JHg     ESCAN00068321    Force response pending for erase operation
 *                         JHg     ESCAN00069161    Added pipelined verification
 *                                                   Rework of block start and verify interface
 *                                                   Changed verification configuration options and error codes
 *                                                   Updated interface version (incompatible changes)
 *                         JHg     ESCAN00069190    No changes
 *  01.07.00   2013-08-16  JHg     ESCAN00069507    No changes
 *                         JHg     ESCAN00069803    Updated interface version (enable/disable verification at run-time)
 *                                 ESCAN00069781    No changes
 *                                 ESCAN00069797    No changes
 *                         JHg     ESCAN00069843    No changes
 *  02.00.00   2013-12-12  JHg     ESCAN00069945    No changes
 *                         JHg     ESCAN00072568    Error / status for BlockStartVerifyInit replaced by SegmentStartVerifyInit
 *                         JHg     ESCAN00071344    No changes
 *                         JHg     ESCAN00072569    Raised major version to match API version
 *                         JHg     ESCAN00072156    No changes
 *                         JHg     ESCAN00072570    No changes
 *                         JHg     ESCAN00072631    No changes
 *  02.01.00   2014-03-12  JHg     ESCAN00073504    Changed signature of input verification function type
 *                         JHg     ESCAN00074066    No changes
 *  02.02.00   2014-05-09  JHg     ESCAN00075225    No changes
 *                         JHg     -                Removed some integration time checks
 *  03.00.00   2015-03-03  JHg     -                Changed compatibility remapping for extended status (SegmentStartVerifyInit)
 *                         JHg     ESCAN00076591    Updated interface version  (support for external stream output)
 *                         JHg     ESCAN00077689    No changes
 *                         JHg     ESCAN00077891    No changes
 *                         JHg     ESCAN00081491    Added support for resumable programming
 *                         JHg     ESCAN00081494    Added support for verification on processed input data
 *                         JHg     ESCAN00081493    Added selective pipelined programming (forced flush when disabled)
 *  03.01.00   2015-04-23  JHg     ESCAN00082572    No changes
 *                         JHg     ESCAN00082605    Added support for reporting of progress information
 *                         JHg     ESCAN00082606    Added support for gap fill (requires ordered segments)
 *  03.01.01   2015-04-27  JHg     ESCAN00082700    No changes
 *  03.01.02   2015-05-27  JHg     ESCAN00083138    No changes
 *  03.01.03   2015-06-13  JHg     ESCAN00083358    No changes
 *                         JHg     ESCAN00083390    No changes
 *                         JHg     ESCAN00083391    No changes
 *                         JHg     ESCAN00083392    Updated interface version
 *  03.02.00   2015-07-21  JHg     ESCAN00084101    No changes
 *                         JHg     ESCAN00084102    Added parameter type structure for stream processing
 *  03.02.01   2015-08-19  JHg     ESCAN00084279    No changes
 *                         JHg     ESCAN00084559    No changes
 *                         JHg     ESCAN00084560    No changes
 *  03.02.02   2015-09-04  JHg     ESCAN00084994    No changes
 *  04.00.00   2015-09-17  JHg     ESCAN00085249    Support combination of input and processed verification
 *                                                  Updated interface version
 *                         JHg     ESCAN00085250    No changes
 *                         JHg     ESCAN00085251    No changes
 *  04.01.00   2016-04-01  JHg     ESCAN00087997    Updated interface version
 *                         JHg     ESCAN00088935    No changes
 **********************************************************************************************************************/

#ifndef __FBL_MEM_H__
#define __FBL_MEM_H__

/***********************************************************************************************************************
 *  VERSION
 **********************************************************************************************************************/

/* ##V_CFG_MANAGEMENT ##CQProject : FblLib_Mem CQComponent : Implementation */
#define FBLLIB_MEM_VERSION            0x0401u
#define FBLLIB_MEM_RELEASE_VERSION    0x00u

/* Interface version */
/** Major interface version identifies incompatible changes */
#define FBL_MEM_API_VERSION_MAJOR     0x04u
/** Minor interface version identifies backward compatible changes */
#define FBL_MEM_API_VERSION_MINOR     0x01u
/** Release interface version identifies cosmetic changes */
#define FBL_MEM_API_VERSION_RELEASE   0x00u

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

#include "fbl_mem_oem.h"

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

/* PRQA S 3453 FblMem_3453 */ /* MD_CBD_19.7 */
/* PRQA S 3458 FblMem_3458 */ /* MD_CBD_19.4 */

/**
   Check for pending operations
   Enter task on states "pending", "suspend pending" and "suspended"
   Condition equals
      ( (fblMemProgState == kFblMemProgState_Pending) || (fblMemProgState == kFblMemProgState_Checkpoint)
        || (fblMemProgState == kFblMemProgState_SuspendPending) || (fblMemProgState == kFblMemProgState_Suspended) )
*/
#define FblMemTaskIsPending()    (fblMemProgState >= kFblMemProgState_Suspended)
/**
   Check for active operations
   Condition equals
      ( (fblMemProgState == kFblMemProgState_Pending) || (fblMemProgState == kFblMemProgState_Checkpoint)
        || (fblMemProgState == kFblMemProgState_SuspendPending) )
*/
#define FblMemTaskIsActive()     (fblMemProgState >= kFblMemProgState_SuspendPending)

/** Default source handle */
#define FBL_MEM_SOURCE_HANDLE_DEFAULT  0u

#if defined( FBL_MEM_SOURCE_COUNT )
# if (FBL_MEM_SOURCE_COUNT > 1u)
#  if defined( FBL_MEM_ENABLE_MULTI_SOURCE )
#  else
/** Multiple input sources */
#   define FBL_MEM_ENABLE_MULTI_SOURCE
#  endif /* FBL_MEM_ENABLE_MULTI_SOURCE */
# endif /* FBL_MEM_SOURCE_COUNT > 1u */
#endif /* FBL_MEM_SOURCE_COUNT */

/* No additional remapping required, simply cast to expected type */
# define FblMemRemapStatus(status)   ((FBL_MEM_STATUS_TYPE)(status))

#if defined( FBL_MEM_PROC_SEGMENTATION )
#else
/** Set data processing segmentation to default value */
# define FBL_MEM_PROC_SEGMENTATION     FBL_MEM_PROC_BUFFER_SIZE
#endif /* FBL_MEM_PROC_SEGMENTATION */

/*-- Compatibility remapping of configuration switches-----------------------*/

#if defined( FBL_ENABLE_GAP_FILL )
# if defined( FBL_MEM_ENABLE_GAP_FILL) || \
     defined( FBL_MEM_DISABLE_GAP_FILL )
/* Gap fill handling explicitly defined outside */
# else
#  define FBL_MEM_ENABLE_GAP_FILL
# endif /* FBL_MEM_ENABLE_GAP_FILL */
#endif /* FBL_ENABLE_GAP_FILL */

/*-- Remap configuration switches--------------------------------------------*/
#if defined( FBL_MEM_ENABLE_VERIFY_PIPELINED )
/** Pipelined operation */
# define FBL_MEM_ENABLE_PIPELINING
#endif /* FBL_ENABLE_PIPELINED_PROGRAMMING || FBL_MEM_ENABLE_VERIFY_PIPELINED */

#if defined( FBL_MEM_ENABLE_VERIFY_OUTPUT ) || \
    defined( FBL_MEM_ENABLE_GAP_FILL )
# if defined( FBL_MEM_ENABLE_SEGMENT_HANDLING ) || \
     defined( FBL_MEM_DISABLE_SEGMENT_HANDLING )
/* Segment handling explicitly defined outside
   If segments are required by signature/checksum verification on output data
   but segment handlings is disabled the correct segments have to be handled outside! */
# else
/** Segment handling required for signature/checksum verification on output data */
#  define FBL_MEM_ENABLE_SEGMENT_HANDLING
# endif /* FBL_MEM_ENABLE_SEGMENT_HANDLING */
#endif /* FBL_MEM_ENABLE_VERIFY_OUTPUT || FBL_MEM_ENABLE_GAP_FILL */

#if defined( FBL_MEM_ENABLE_VERIFY_OUTPUT )     || \
    defined( FBL_MEM_ENABLE_VERIFY_PIPELINED )  || \
    defined( FBL_MEM_ENABLE_SEGMENT_HANDLING )  || \
    defined( FBL_ENABLE_PROCESSED_DATA_LENGTH ) || \
    defined( FBL_MEM_ENABLE_STREAM_OUTPUT )     || \
    defined( FBL_MEM_ENABLE_GAP_FILL )          || \
    defined( FBL_MEM_ENABLE_PROGRESS_INFO )
/** Persistent storage of block information */
# define FBL_MEM_ENABLE_GLOBAL_BLOCK_INFO
#endif /* FBL_MEM_ENABLE_VERIFY_OUTPUT || FBL_MEM_ENABLE_VERIFY_INPUT || FBL_MEM_ENABLE_VERIFY_PIPELINED || FBL_MEM_ENABLE_SEGMENT_HANDLING || FBL_MEM_ENABLE_PASSTHROUGH || FBL_ENABLE_PROCESSED_DATA_LENGTH || FBL_MEM_ENABLE_STREAM_OUTPUT || FBL_MEM_ENABLE_GAP_FILL || FBL_MEM_ENABLE_PROGRESS_INFO */

/*
   Allow overwrite of certain data types or defines by using
   FBL_MEM_VERIFY_<type>_OVERWRITE defines / macros
*/
/*-- On-the-fly verification ------------------------------------------------*/
#if defined( FBL_MEM_VERIFY_OK_OVERWRITE )
#define FBL_MEM_VERIFY_OK                       (FBL_MEM_VERIFY_OK_OVERWRITE)
#else
#define FBL_MEM_VERIFY_OK                       (tFblMemVerifyStatus)(SECM_OK)
#endif
#if defined( FBL_MEM_VERIFY_STATE_INIT_OVERWRITE )
#define FBL_MEM_VERIFY_STATE_INIT               (FBL_MEM_VERIFY_STATE_INIT_OVERWRITE)
#else
#define FBL_MEM_VERIFY_STATE_INIT               kHashInit
#endif
#if defined( FBL_MEM_VERIFY_STATE_COMPUTE_OVERWRITE )
#define FBL_MEM_VERIFY_STATE_COMPUTE            (FBL_MEM_VERIFY_STATE_COMPUTE_OVERWRITE)
#else
#define FBL_MEM_VERIFY_STATE_COMPUTE            kHashCompute
#endif
#if defined( FBL_MEM_VERIFY_STATE_FINALIZE_OVERWRITE )
#define FBL_MEM_VERIFY_STATE_FINALIZE           (FBL_MEM_VERIFY_STATE_FINALIZE_OVERWRITE)
#else
#define FBL_MEM_VERIFY_STATE_FINALIZE           kHashFinalize
#endif
#if defined( FBL_MEM_VERIFY_STATE_VERIFY_OVERWRITE )
#define FBL_MEM_VERIFY_STATE_VERIFY             (FBL_MEM_VERIFY_STATE_VERIFY_OVERWRITE)
#else
#define FBL_MEM_VERIFY_STATE_VERIFY             kSigVerify
#endif

/*-- Error handling ---------------------------------------------------------*/

/* Helper macros for status definitions */
#define FBL_MEM_STATUS_NAME(name)         kFblMemStatus_ ## name /* PRQA S 0342 */ /* MD_FblMem_0342 */
#define FBL_MEM_STATUS_DEFINE(name)       FBL_MEM_STATUS_ ## name /* PRQA S 0342 */ /* MD_FblMem_0342 */

#if defined( FBL_MEM_ENABLE_STATUS_OVERWRITE )
/** Overwrite error codes by OEM dependent value */
# define FBL_MEM_STATUS_DEFINITION(name)  FBL_MEM_STATUS_NAME(name) = FBL_MEM_STATUS_DEFINE(name) /* PRQA S 0342 */ /* MD_FblMem_0342 */
#else
/** Use automatically numbered error codes */
# define FBL_MEM_STATUS_DEFINITION(name)  FBL_MEM_STATUS_NAME(name) /* PRQA S 0342 */ /* MD_FblMem_0342 */
#endif /* FBL_MEM_ENABLE_STATUS_OVERWRITE */

#if defined( FBL_MEM_ENABLE_STATUS_OVERWRITE ) 

#  if defined( FBL_MEM_STATUS_ProgramOverflow )
#  else
/* Status code not required: remap to default */
#   define FBL_MEM_STATUS_ProgramOverflow             FBL_MEM_STATUS_Failed
#  endif

#  if defined( FBL_MEM_STATUS_VerifyCompute )
#  else
#   error Status overwrite value missing: FBL_MEM_STATUS_VerifyCompute
#  endif

#  if defined( FBL_MEM_STATUS_VerifyFinalize )
#  else
#   error Status overwrite value missing: FBL_MEM_STATUS_VerifyFinalize
#  endif

#  if defined( FBL_MEM_STATUS_SegmentStartVerifyInit )
#  else
#   if defined( FBL_MEM_STATUS_BlockStartVerifyInit )
/* Compatibility remapping (API < 2.2.0) */
#    define FBL_MEM_STATUS_SegmentStartVerifyInit     FBL_MEM_STATUS_BlockStartVerifyInit
#   else
#    error Status overwrite value missing: FBL_MEM_STATUS_SegmentStartVerifyInit
#   endif
#  endif
#  if defined( FBL_MEM_EXT_STATUS_BlockStartVerifyInit )
/* Compatibility remapping (API < 2.2.0) */
#   define FBL_MEM_EXT_STATUS_SegmentStartVerifyInit(status)      FBL_MEM_EXT_STATUS_BlockStartVerifyInit(status)
#  endif

#  if defined( FBL_MEM_STATUS_BlockEndVerifyFinalize )
#  else
#   error Status overwrite value missing: FBL_MEM_STATUS_BlockEndVerifyFinalize
#  endif

#  if defined( FBL_MEM_STATUS_BlockVerifyInputVerify )
#  else
/* Status code not required: remap to default */
#   define FBL_MEM_STATUS_BlockVerifyInputVerify      FBL_MEM_STATUS_Failed
#  endif

#  if defined( FBL_MEM_STATUS_BlockVerifyPipeVerify )
#  else
#   error Status overwrite value missing: FBL_MEM_STATUS_BlockVerifyPipeVerify
#  endif

#  if defined( FBL_MEM_STATUS_BlockVerifyOutputVerify )
#  else
#   error Status overwrite value missing: FBL_MEM_STATUS_BlockVerifyOutputVerify
#  endif

#  if defined( FBL_MEM_STATUS_BlockStartParam )
#  else
#   error Status overwrite value missing: FBL_MEM_STATUS_BlockStartParam
#  endif

#  if defined( FBL_MEM_STATUS_SegmentStartVerifyCompute )
#  else
#   error Status overwrite value missing: FBL_MEM_STATUS_SegmentStartVerifyCompute
#  endif

#  if defined( FBL_MEM_STATUS_DataProc )
#  else
#   error Status overwrite value missing: FBL_MEM_STATUS_DataProc
#  endif

#  if defined( FBL_MEM_STATUS_DataProcConsume )
#  else
#   error Status overwrite value missing: FBL_MEM_STATUS_DataProcConsume
#  endif

#  if defined( FBL_MEM_STATUS_DataProcDeinit )
#  else
#   error Status overwrite value missing: FBL_MEM_STATUS_DataProcDeinit
#  endif

#  if defined( FBL_MEM_STATUS_SegmentStartStreamOutInit )
#  else
/* Status code not defined: remap to data processing code */
#   define FBL_MEM_STATUS_SegmentStartStreamOutInit   FBL_MEM_STATUS_SegmentStartDataProcInit
#  endif

#  if defined( FBL_MEM_STATUS_StreamOutput )
#  else
/* Status code not required: remap to default */
#   define FBL_MEM_STATUS_StreamOutput                FBL_MEM_STATUS_Failed
#  endif

#  if defined( FBL_MEM_STATUS_StreamOutputConsume )
#  else
/* Status code not required: remap to default */
#   define FBL_MEM_STATUS_StreamOutputConsume         FBL_MEM_STATUS_Failed
#  endif

#  if defined( FBL_MEM_STATUS_StreamOutputOverflow )
#  else
/* Status code not required: remap to default */
#   define FBL_MEM_STATUS_StreamOutputOverflow        FBL_MEM_STATUS_Failed
#  endif

#  if defined( FBL_MEM_STATUS_StreamOutputDeinit )
#  else
/* Status code not required: remap to default */
#   define FBL_MEM_STATUS_StreamOutputDeinit          FBL_MEM_STATUS_Failed
#  endif

#  if defined( FBL_MEM_STATUS_DriverResumeWrite )
#  else
/* Status code not required: remap to default */
#   define FBL_MEM_STATUS_DriverResumeWrite           FBL_MEM_STATUS_Failed
#  endif

#  if defined( FBL_MEM_STATUS_DriverRemainder )
#  else
/* Status code not required: remap to default */
#   define FBL_MEM_STATUS_DriverRemainder             FBL_MEM_STATUS_Failed
#  endif

#  if defined( FBL_MEM_STATUS_BlockEraseSequence )
#  else
#   error Status overwrite value missing: FBL_MEM_STATUS_BlockEraseSequence
#  endif

#  if defined( FBL_MEM_STATUS_EraseOutsideFbt )
#  else
#   error Status overwrite value missing: FBL_MEM_STATUS_EraseOutsideFbt
#  endif

#  if defined( FBL_MEM_STATUS_EraseDriverNotReady )
#  else
#   error Status overwrite value missing: FBL_MEM_STATUS_EraseDriverNotReady
#  endif

# if defined( __ApplFblMemPreErase )
#  if defined( FBL_MEM_STATUS_ErasePreErase )
#  else
#   error Status overwrite value missing: FBL_MEM_STATUS_ErasePreErase
#  endif
# else
#  if defined( FBL_MEM_STATUS_ErasePreErase )
#  else
/* Status code not required: remap to default */
#   define FBL_MEM_STATUS_ErasePreErase               FBL_MEM_STATUS_Failed
#  endif
# endif /* FBL_MEM_ENABLE_ERASE && __ApplFblMemPreErase */

# if defined( __ApplFblMemPostErase )
#  if defined( FBL_MEM_STATUS_ErasePostErase )
#  else
#   error Status overwrite value missing: FBL_MEM_STATUS_ErasePostErase
#  endif
# else
#  if defined( FBL_MEM_STATUS_ErasePostErase )
#  else
/* Status code not required: remap to default */
#   define FBL_MEM_STATUS_ErasePostErase              FBL_MEM_STATUS_Failed
#  endif
# endif /* FBL_MEM_ENABLE_ERASE && __ApplFblMemPostErase */

# if defined( __ApplFblMemDriverReady )
#  if defined( FBL_MEM_STATUS_EraseDriverNotReady )
#  else
#   error Status overwrite value missing: FBL_MEM_STATUS_EraseDriverNotReady
#  endif
# else
#  if defined( FBL_MEM_STATUS_EraseDriverNotReady )
#  else
/* Status code not required: remap to default */
#   define FBL_MEM_STATUS_EraseDriverNotReady         FBL_MEM_STATUS_Failed
#  endif
# endif /* FBL_MEM_ENABLE_ERASE && __ApplFblMemDriverReady */

#  if defined( FBL_MEM_STATUS_SegmentStartSegmentCount )
#  else
#   error Status overwrite value missing: FBL_MEM_STATUS_SegmentStartSegmentCount
#  endif

# if defined( __ApplFblMemPostSegmentEnd )
#  if defined( FBL_MEM_STATUS_SegmentEndPost )
#  else
#   error Status overwrite value missing: FBL_MEM_STATUS_SegmentEndPost
#  endif
# else
#  if defined( FBL_MEM_STATUS_SegmentEndPost )
#  else
/* Status code not required: remap to default */
#   define FBL_MEM_STATUS_SegmentEndPost              FBL_MEM_STATUS_Failed
#  endif
# endif /* __ApplFblMemPostSegmentEnd */

# if defined( __ApplFblMemDriverReady )
#  if defined( FBL_MEM_STATUS_ProgramDriverNotReady )
#  else
#   error Status overwrite value missing: FBL_MEM_STATUS_ProgramDriverNotReady
#  endif
# else
#  if defined( FBL_MEM_STATUS_ProgramDriverNotReady )
#  else
/* Status code not required: remap to default */
#   define FBL_MEM_STATUS_ProgramDriverNotReady       FBL_MEM_STATUS_Failed
#  endif
# endif /* __ApplFblMemDriverReady */

# if defined( __ApplFblMemPreWrite )
#  if defined( FBL_MEM_STATUS_ProgramPreWrite )
#  else
#   error Status overwrite value missing: FBL_MEM_STATUS_ProgramPreWrite
#  endif
# else
#  if defined( FBL_MEM_STATUS_ProgramPreWrite )
#  else
/* Status code not required: remap to default */
#   define FBL_MEM_STATUS_ProgramPreWrite             FBL_MEM_STATUS_Failed
#  endif
# endif /* __ApplFblMemPreWrite */

# if defined( __ApplFblMemPostWrite )
#  if defined( FBL_MEM_STATUS_ProgramPostWrite )
#  else
#   error Status overwrite value missing: FBL_MEM_STATUS_ProgramPostWrite
#  endif
# else
#  if defined( FBL_MEM_STATUS_ProgramPostWrite )
#  else
/* Status code not required: remap to default */
#   define FBL_MEM_STATUS_ProgramPostWrite            FBL_MEM_STATUS_Failed
#  endif
# endif /* __ApplFblMemPostWrite */

#endif /* FBL_MEM_ENABLE_STATUS_OVERWRITE || FBL_MEM_ENABLE_STATUS_REMAPPING */

/* PRQA L:FblMem_3453 */
/* PRQA L:FblMem_3458 */

/***********************************************************************************************************************
 *  TYPEDEFS
 **********************************************************************************************************************/
/*-- On-the-fly verification ------------------------------------------------*/
#if defined( FBL_MEM_VERIFY_STATUS_TYPE_OVERWRITE )
typedef FBL_MEM_VERIFY_STATUS_TYPE_OVERWRITE             tFblMemVerifyStatus;
#else
typedef SecM_StatusType                                  tFblMemVerifyStatus;
#endif /* FBL_MEM_VERIFY_STATUS_TYPE_OVERWRITE */
#if defined( FBL_MEM_VERIFY_PARAM_TYPE_INPUT_OVERWRITE )
typedef FBL_MEM_VERIFY_PARAM_TYPE_INPUT_OVERWRITE        tFblMemVerifyParamInput;
#else
typedef SecM_SignatureParamType                          tFblMemVerifyParamInput;
#endif /* FBL_MEM_VERIFY_PARAM_TYPE_INPUT_OVERWRITE */
#if defined( FBL_MEM_VERIFY_PARAM_TYPE_OUTPUT_OVERWRITE )
typedef FBL_MEM_VERIFY_PARAM_TYPE_OUTPUT_OVERWRITE       tFblMemVerifyParamOutput;
#else
typedef SecM_VerifyParamType                             tFblMemVerifyParamOutput;
#endif /* FBL_MEM_VERIFY_PARAM_TYPE_OUTPUT_OVERWRITE */
#if defined( FBL_MEM_VERIFY_LENGTH_TYPE_OVERWRITE )
typedef FBL_MEM_VERIFY_LENGTH_TYPE_OVERWRITE             tFblMemVerifyLength;
#else
typedef SecM_LengthType                                  tFblMemVerifyLength;
#endif /* FBL_MEM_VERIFY_LENGTH_TYPE_OVERWRITE */
#if defined( FBL_MEM_VERIFY_SIZE_TYPE_OVERWRITE )
typedef FBL_MEM_VERIFY_SIZE_TYPE_OVERWRITE               tFblMemVerifySize;
#else
typedef SecM_SizeType                                    tFblMemVerifySize;
#endif /* FBL_MEM_VERIFY_SIZE_TYPE_OVERWRITE */
#if defined( FBL_MEM_VERIFY_ADDRESS_TYPE_OVERWRITE )
typedef FBL_MEM_VERIFY_ADDRESS_TYPE_OVERWRITE            tFblMemVerifyAddr;
#else
typedef SecM_AddrType                                    tFblMemVerifyAddr;
#endif /* FBL_MEM_VERIFY_ADDRESS_TYPE_OVERWRITE */
#if defined( FBL_MEM_VERIFY_DATA_PTR_TYPE_OVERWRITE )
typedef FBL_MEM_VERIFY_DATA_PTR_TYPE_OVERWRITE           tFblMemVerifyDataPtr;
#else
typedef V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 *           tFblMemVerifyDataPtr;
#endif /* FBL_MEM_VERIFY_DATA_PTR_TYPE_OVERWRITE */
#if defined( FBL_MEM_VERIFY_WD_FCT_TYPE_OVERWRITE )
typedef FBL_MEM_VERIFY_WD_FCT_TYPE_OVERWRITE             tFblMemVerifyWdFct;
#else
typedef FL_WDTriggerFctType                              tFblMemVerifyWdFct;
#endif /* FBL_MEM_VERIFY_WD_FCT_TYPE_OVERWRITE */
#if defined( FBL_MEM_VERIFY_READ_FCT_TYPE_OVERWRITE )
typedef FBL_MEM_VERIFY_READ_FCT_TYPE_OVERWRITE           tFblMemVerifyReadFct;
#else
typedef FL_ReadMemoryFctType                             tFblMemVerifyReadFct;
#endif /* FBL_MEM_VERIFY_READ_FCT_TYPE_OVERWRITE */
#if defined( FBL_MEM_VERIFY_FCT_INPUT_TYPE_OVERWRITE )
typedef FBL_MEM_VERIFY_FCT_INPUT_TYPE_OVERWRITE(tFblMemVerifyFctInput);
#else
typedef FBL_CALL_TYPE tFblMemVerifyStatus (*tFblMemVerifyFctInput) (V_MEMRAM1 tFblMemVerifyParamInput V_MEMRAM2 V_MEMRAM3 *);
#endif /* FBL_MEM_VERIFY_FCT_INPUT_TYPE_OVERWRITE */
#if defined( FBL_MEM_VERIFY_FCT_OUTPUT_TYPE_OVERWRITE )
typedef FBL_MEM_VERIFY_FCT_OUTPUT_TYPE_OVERWRITE(tFblMemVerifyFctOutput);
#else
typedef FBL_CALL_TYPE tFblMemVerifyStatus (*tFblMemVerifyFctOutput) (V_MEMRAM1 tFblMemVerifyParamOutput V_MEMRAM2 V_MEMRAM3 *);
#endif /* FBL_MEM_VERIFY_FCT_OUTPUT_TYPE_OVERWRITE */

#if defined( FBL_MEM_SEGMENT_LIST_TYPE_OVERWRITE )
typedef FBL_MEM_SEGMENT_LIST_TYPE_OVERWRITE              tFblMemSegmentList;
#else
typedef FL_SegmentListType                               tFblMemSegmentList;
#endif /* FBL_MEM_SEGMENT_LIST_TYPE_OVERWRITE */
#if defined( FBL_MEM_SEGMENT_LIST_ENTRY_TYPE_OVERWRITE )
typedef FBL_MEM_SEGMENT_LIST_ENTRY_TYPE_OVERWRITE        tFblMemSegmentListEntry;
#else
typedef FL_SegmentInfoType                               tFblMemSegmentListEntry;
#endif /* FBL_MEM_SEGMENT_LIST_ENTRY_TYPE_OVERWRITE */

/** Error codes reported by FblMem routines */
typedef enum
{
   /* Define status codes: kFblMemStatus_<name> */
   /* Default */
   FBL_MEM_STATUS_DEFINITION(Ok)                            /* 0x00 */
   ,FBL_MEM_STATUS_DEFINITION(Failed)                       /* 0x01 */
   /* BlockErase indication */
   ,FBL_MEM_STATUS_DEFINITION(BlockEraseSequence)           /* 0x02 */
   /* BlockStart indication */
   ,FBL_MEM_STATUS_DEFINITION(BlockStartSequence)           /* 0x03 */
   ,FBL_MEM_STATUS_DEFINITION(BlockStartParam)              /* 0x04 */
   /* BlockEnd indication */
   ,FBL_MEM_STATUS_DEFINITION(BlockEndSequence)             /* 0x05 */
   ,FBL_MEM_STATUS_DEFINITION(BlockEndVerifyFinalize)       /* 0x06 */
   /* BlockVerify */
   ,FBL_MEM_STATUS_DEFINITION(BlockVerifySequence)          /* 0x07 */
   ,FBL_MEM_STATUS_DEFINITION(BlockVerifyInputVerify)       /* 0x08 */
   ,FBL_MEM_STATUS_DEFINITION(BlockVerifyProcessedVerify)   /* 0x09 */
   ,FBL_MEM_STATUS_DEFINITION(BlockVerifyPipeVerify)        /* 0x0A */
   ,FBL_MEM_STATUS_DEFINITION(BlockVerifyOutputVerify)      /* 0x0B */
   /* SegmentStart indication */
   ,FBL_MEM_STATUS_DEFINITION(SegmentStartSequence)         /* 0x0C */
   ,FBL_MEM_STATUS_DEFINITION(SegmentStartDataProcInit)     /* 0x0D */
   ,FBL_MEM_STATUS_DEFINITION(SegmentStartStreamOutInit)    /* 0x0E */
   ,FBL_MEM_STATUS_DEFINITION(SegmentStartVerifyInit)       /* 0x0F */ /* PRQA S 0779 */ /* MD_FblMem_0779 */
   ,FBL_MEM_STATUS_DEFINITION(SegmentStartVerifyCompute)    /* 0x10 */ /* PRQA S 0779 */ /* MD_FblMem_0779 */
   ,FBL_MEM_STATUS_DEFINITION(SegmentStartSegmentCount)     /* 0x11 */
   /* SegmentEnd indication */
   ,FBL_MEM_STATUS_DEFINITION(SegmentEndSequence)           /* 0x12 */
   ,FBL_MEM_STATUS_DEFINITION(SegmentEndInsufficientData)   /* 0x13 */
   ,FBL_MEM_STATUS_DEFINITION(SegmentEndPost)               /* 0x14 */
   /* Data indication */
   ,FBL_MEM_STATUS_DEFINITION(DataIndSequence)              /* 0x15 */
   ,FBL_MEM_STATUS_DEFINITION(DataIndParam)                 /* 0x16 */
   ,FBL_MEM_STATUS_DEFINITION(DataIndOverflow)              /* 0x17 */
   /* Data processing */
   ,FBL_MEM_STATUS_DEFINITION(DataProc)                     /* 0x18 */
   ,FBL_MEM_STATUS_DEFINITION(DataProcConsume)              /* 0x19 */
   ,FBL_MEM_STATUS_DEFINITION(DataProcDeinit)               /* 0x1A */
   /* Stream output */
   ,FBL_MEM_STATUS_DEFINITION(StreamOutput)                 /* 0x1B */
   ,FBL_MEM_STATUS_DEFINITION(StreamOutputConsume)          /* 0x1C */
   ,FBL_MEM_STATUS_DEFINITION(StreamOutputOverflow)         /* 0x1D */
   ,FBL_MEM_STATUS_DEFINITION(StreamOutputDeinit)           /* 0x1E */
   /* Memory driver */
   ,FBL_MEM_STATUS_DEFINITION(DriverResumeWrite)            /* 0x1F */
   ,FBL_MEM_STATUS_DEFINITION(DriverWrite)                  /* 0x20 */
   ,FBL_MEM_STATUS_DEFINITION(DriverErase)                  /* 0x21 */
   ,FBL_MEM_STATUS_DEFINITION(DriverRemainder)              /* 0x22 */
   ,FBL_MEM_STATUS_DEFINITION(DriverSuspendWrite)           /* 0x23 */
   /* Programming operation */
   ,FBL_MEM_STATUS_DEFINITION(ProgramOverflow)              /* 0x24 */
   ,FBL_MEM_STATUS_DEFINITION(ProgramOutsideFbt)            /* 0x25 */
   ,FBL_MEM_STATUS_DEFINITION(ProgramUnalignedAddress)      /* 0x26 */
   ,FBL_MEM_STATUS_DEFINITION(ProgramDriverNotReady)        /* 0x27 */
   ,FBL_MEM_STATUS_DEFINITION(ProgramPreWrite)              /* 0x28 */
   ,FBL_MEM_STATUS_DEFINITION(ProgramPostWrite)             /* 0x29 */
   /* Erase operation */
   ,FBL_MEM_STATUS_DEFINITION(EraseOutsideFbt)              /* 0x2A */
   ,FBL_MEM_STATUS_DEFINITION(EraseDriverNotReady)          /* 0x2B */
   ,FBL_MEM_STATUS_DEFINITION(ErasePreErase)                /* 0x2C */
   ,FBL_MEM_STATUS_DEFINITION(ErasePostErase)               /* 0x2D */
   /* On-the-fly verification */
   ,FBL_MEM_STATUS_DEFINITION(VerifyCompute)                /* 0x2E */
   ,FBL_MEM_STATUS_DEFINITION(VerifyFinalize)               /* 0x2F */
   /* Pass-through */
   ,FBL_MEM_STATUS_DEFINITION(PassThroughLocal)             /* 0x30 */
   ,FBL_MEM_STATUS_DEFINITION(PassThroughRemote)            /* 0x31 */
} tFblMemStatus;

/** Memory type */
typedef enum
{
   kFblMemType_RAM = 0u,   /* Volatile memory      */
   kFblMemType_ROM         /* Non-volatile memory  */
} tFblMemType;

/** State of programming operation */
typedef enum
{
   kFblMemProgState_Idle = 0u,
   kFblMemProgState_Error,
   kFblMemProgState_Suspended,
   kFblMemProgState_SuspendPending,
   kFblMemProgState_Checkpoint,
   kFblMemProgState_Pending
} tFblMemProgState;

/** Pointer type to byte data in RAM */
typedef V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 *        tFblMemRamData;
/** Pointer type to constant byte data in RAM */
typedef const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 *  tFblMemConstRamData;

/** Data format identifier */
typedef vuint8 tFblMemDfi;

/** Input verification routine */
typedef struct
{
   tFblMemVerifyFctInput      function;
   V_MEMRAM1 tFblMemVerifyParamInput   V_MEMRAM2 V_MEMRAM3 * param;
} tFblMemVerifyRoutineInput;

/** Output verification routine */
typedef struct
{
   tFblMemVerifyFctOutput     function;
   V_MEMRAM1 tFblMemVerifyParamOutput  V_MEMRAM2 V_MEMRAM3 * param;
} tFblMemVerifyRoutineOutput;

/** Information passed in block erase / start indication */
typedef struct
{
   tFblAddress                targetAddress;          /**< Target address (erase operation) */
   tFblLength                 targetLength;           /**< Target length (erase operation) */
   /* Used for signature verification */
   tFblAddress                logicalAddress;         /**< Logical address (verification operation) */
   tFblLength                 logicalLength;          /**< Logical length (verification operation) */
   tFblMemVerifyRoutineInput  verifyRoutineInput;     /**< On-the-fly verification on input data */
   tFblMemVerifyRoutineInput  verifyRoutineProcessed; /**< On-the-fly verification on processed data */
   tFblMemVerifyRoutineInput  verifyRoutinePipe;      /**< Pipelined verification on output data */
   tFblMemVerifyRoutineOutput verifyRoutineOutput;    /**< Verification on output data */
   tFblMemVerifyReadFct       readFct;                /**< Memory read function, used by pipelined and output verification */
   V_MEMRAM1 tFblMemSegmentList V_MEMRAM2 V_MEMRAM3 * segmentList; /**< List of programmed segments, used by output verification */
   vuint8                     maxSegments;            /**< Maximum number of entries in segment list */
} tFblMemBlockInfo;

/** Verification data */
typedef struct
{
   tFblMemRamData data;    /**< Pointer to verification data (signature/checksum) */
   tFblLength     length;  /**< Length of verification data */
} tFblMemVerifyData;

/** Information passed in block verify */
typedef struct
{
   tFblMemVerifyData    verifyDataInput;     /**< On-the-fly verification on input data      */
   tFblMemVerifyData    verifyDataProcessed; /**< On-the-fly verification on processed data  */
   tFblMemVerifyData    verifyDataPipe;      /**< Pipelined verification on output data      */
   tFblMemVerifyData    verifyDataOutput;    /**< Verification on output data                */
} tFblMemBlockVerifyData;

/** Information passed in segment start indication */
typedef struct
{
   tFblAddress    targetAddress;    /* Target address (program operation)        */
   tFblLength     targetLength;     /* Target length (program operation)         */
   /* Used for signature verification */
   tFblAddress    logicalAddress;   /* Logical address (verification operation)  */
   tFblLength     logicalLength;    /* Logical length (verification operation)   */
   /* Handling info */
   tFblMemType    type;             /* Type (RAM / ROM)                          */
   tFblMemDfi     dataFormat;       /* Data format identifier (data processing)  */
} tFblMemSegmentInfo;

/** Return type for watchdog trigger */
#if defined( FBL_MEM_TRIGGER_STATUS_OVERWRITE )
typedef FBL_MEM_TRIGGER_STATUS_OVERWRITE  tFblMemTriggerStatus;
#else
typedef vuint8                            tFblMemTriggerStatus;
#endif

/** Input source handle */
typedef vuintx tFblMemInputSource;

#if defined( FBL_MEM_ENABLE_PROGRESS_INFO )
/** Operation types reported in progress information */
typedef enum
{
   kFblMemProgressType_Erase,       /**< Erase operation */
   kFblMemProgressType_Program,     /**< Programming operation (may include gap fill) */
   kFblMemProgressType_GapFill,     /**< Concluding gap fill operation */
   kFblMemProgressType_Verify,      /**< Verification operation */
   kFblMemProgressType_Undefined    /**< Undefined, used internally */
} tFblMemProgressType;

/** Progress information */
typedef struct
{
   tFblMemProgressType  type;             /**< Operation type */
   tFblAddress          logicalAddress;   /**< Logical block address */
   vuint32              segmentCount;     /**< Segment count
                                               Typically zero for erase and verification,
                                               segment index for programming and
                                               index of last segment incremented by one for concluding gap fill */
   vuint8               totalProgress;    /**< Total operation progress (current block) */
   vuint8               partialProgress;  /**< Partial operation progress */
} tFblMemProgressInfo;
#endif /* FBL_MEM_ENABLE_PROGRESS_INFO */

#if defined( FBL_MEM_ENABLE_STREAM_OUTPUT )
typedef struct
{
   tFblAddress                baseAddress;      /**< Address of base memory region allowed to be modified */
   tFblLength                 baseLength;       /**< Length of base memory region allowed to be modified  */
   tFblAddress                address;          /**< Current programming address (based on previously produced length) */
   tFblLength                 length;           /**< Current programming remainder (based on previously produced length) */
   tFblMemConstRamData        inputData;        /**< Pointer to input data buffer */
   tFblMemRamData             outputData;       /**< Pointer to output data buffer (not used for stream output) */
   void (* watchdog)(void);                     /**< Watchdog trigger function */
   tFblLength                 inputLength;      /**< Length of input data */
   tFblLength                 outputSize;       /**< Size of output data buffer (not used for stream output) */
   tFblLength                 consumedLength;   /**< Number of consumed input data bytes */
   tFblLength                 producedLength;   /**< Number of produced output data bytes (stream output: directly programmed to memory) */
   vuint8                     mode;             /**< Applied data processing mode*/
} tFblMemStreamProcessing;
#endif /* FBL_MEM_ENABLE_STREAM_OUTPUT */

/***********************************************************************************************************************
 *  FUNCTION PROTOTYPES
 **********************************************************************************************************************/

#if defined( __cplusplus )
extern "C" {
#endif

tFblMemRamData FblMemInitPowerOn( void );
tFblMemRamData FblMemInitPowerOnExt( tFblLength preambleLen, tFblMemInputSource sourceHandle );
tFblMemRamData FblMemInit( void );
void FblMemDeinit( void );
tFblMemRamData FblMemGetActiveBuffer( void );
tFblMemStatus FblMemBlockEraseIndication( const V_MEMRAM1 tFblMemBlockInfo V_MEMRAM2 V_MEMRAM3 * block );
tFblMemStatus FblMemBlockStartIndication( V_MEMRAM1 tFblMemBlockInfo V_MEMRAM2 V_MEMRAM3 * block );
tFblMemStatus FblMemBlockEndIndication( void );
tFblMemStatus FblMemBlockVerify( const V_MEMRAM1 tFblMemBlockVerifyData V_MEMRAM2 V_MEMRAM3 * verifyData,
   V_MEMRAM1 tFblMemVerifyStatus V_MEMRAM2 V_MEMRAM3 * verifyResult );
tFblMemStatus FblMemSegmentStartIndication( const V_MEMRAM1 tFblMemSegmentInfo V_MEMRAM2 V_MEMRAM3 * segment );
tFblMemStatus FblMemSegmentEndIndication( V_MEMRAM1 tFblLength V_MEMRAM2 V_MEMRAM3 * writeLength );
tFblMemStatus FblMemDataIndication( tFblMemConstRamData buffer, tFblLength offset, tFblLength length );
void FblMemResumeIndication( void );
void FblMemTask( void );

/* Low level memory operations */
tFblMemStatus FblMemEraseRegion( tFblAddress eraseAddress, tFblLength eraseLength );
tFblMemStatus FblMemProgramBuffer( tFblAddress programAddress,V_MEMRAM1 tFblLength V_MEMRAM2 V_MEMRAM3 * programLength,
   tFblMemRamData programData );

/* Helper functions */
void FblMemSetInteger( vuintx count, vuint32 input, tFblMemRamData buffer );
vuint32 FblMemGetInteger( vuintx count, tFblMemConstRamData buffer );

#if defined( FBL_MEM_ENABLE_MULTI_SOURCE )
void FblMemLockInputSource( tFblMemInputSource sourceHandle );
#endif

#if defined( FBL_MEM_ENABLE_PIPELINING )
#  define FBLLIB_MEM_RAMCODE_START_SEC_CODE_EXPORT
#  include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

void FblMemRxNotification( void );

#  define FBLLIB_MEM_RAMCODE_STOP_SEC_CODE_EXPORT
#  include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
#endif /* FBL_MEM_ENABLE_PIPELINING */

#if defined( __cplusplus )
} /* extern "C" */
#endif

/***********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

# define FBLLIB_MEM_START_SEC_VAR_EXPORT
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

V_MEMRAM0 extern V_MEMRAM1 tFblMemProgState V_MEMRAM2 fblMemProgState;

# define FBLLIB_MEM_STOP_SEC_VAR_EXPORT
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/***********************************************************************************************************************
 *  CONFIGURATION CHECKS
 **********************************************************************************************************************/

/* Plausibility checks of configuration settings *****************************/
/*-- Integration time -------------------------------------------------------*/

/*-- Compatibility remapping of configuration switches-----------------------*/

/*-- Compile time -----------------------------------------------------------*/
#if defined( FBL_ENABLE_ADAPTIVE_DATA_TRANSFER_RCRRP )
# if defined( FBL_MEM_WRITE_SEGMENTATION )
# else
#  error Write segmentation not defined
# endif
# if defined( FBL_ENABLE_UNALIGNED_DATA_TRANSFER )
# else
#  if ( (FBL_MEM_WRITE_SEGMENTATION & (FBL_MAX_SEGMENT_SIZE - 1u)) != 0u )
#   error Write segmentation has to be multiple of segment size
#  endif
# endif /* FBL_ENABLE_UNALIGNED_DATA_TRANSFER */
#endif /* FBL_ENABLE_PIPELINED_PROGRAMMING || FBL_ENABLE_ADAPTIVE_DATA_TRANSFER_RCRRP */

#if defined( FBL_ENABLE_DATA_PROCESSING )
# if defined( FBL_MEM_PROC_BUFFER_SIZE )
# else
#  error Data processing buffer size not defined
# endif
# if ( FBL_MEM_PROC_SEGMENTATION > 0xFFFFu )
#  error Data processing segmentation exceeds interface constraints
# endif
# if ( FBL_MEM_PROC_SEGMENTATION > FBL_MEM_PROC_BUFFER_SIZE )
#  error Data processing segmentation exceeds buffer size
# endif
# if ( FBL_MEM_PROC_SEGMENTATION == FBL_MEM_PROC_BUFFER_SIZE )
# else
# endif
#endif /* FBL_ENABLE_DATA_PROCESSING */

#if defined( FBL_MEM_BUFFER_SIZE )
#else
# error Input buffer size not defined
#endif /* FBL_MEM_BUFFER_SIZE */

#if defined( FBL_MEM_DEFAULT_PREAMBLE_LENGTH )
#else
# error Default preamble length not defined
#endif /* FBL_MEM_DEFAULT_PREAMBLE_LENGTH */

#if defined( FBL_MEM_MAX_PREAMBLE_LENGTH )
#else
# error Maximum preamble length not defined
#endif /* FBL_MEM_MAX_PREAMBLE_LENGTH */

#if defined( FBL_MEM_WD_TRIGGER_DEFAULT )
#else
# error Default watchdog trigger return code not defined
#endif /* FBL_MEM_WD_TRIGGER_DEFAULT */

#if defined( FBL_MEM_ENABLE_STATUS_OVERWRITE ) 
# if defined( FBL_MEM_STATUS_Ok ) && \
     defined( FBL_MEM_STATUS_Failed )
# else
#  error Status overwrite or remapping enabled, but actual values missing
# endif
#endif /* FBL_MEM_ENABLE_STATUS_OVERWRITE || FBL_MEM_ENABLE_STATUS_REMAPPING */

#if defined( FBL_MEM_STATUS_Ok ) || \
    defined( FBL_MEM_STATUS_Failed )
# if defined( FBL_MEM_ENABLE_STATUS_OVERWRITE ) 
# else
#  error Status values defined, but neither status overwrite nor remapping enabled
# endif
#endif /* FBL_MEM_STATUS_Ok || FBL_MEM_STATUS_Failed */

#if defined( FBL_MEM_ENABLE_EXT_STATUS )
# if defined( FBL_MEM_EXT_STATUS_Ok ) && \
     defined( FBL_MEM_EXT_STATUS_Failed )
# else
#  error Extended status enabled, but actual macros missing
# endif
#endif /* FBL_MEM_ENABLE_EXT_STATUS */

#if defined( FBL_MEM_EXT_STATUS_Ok ) || \
    defined( FBL_MEM_EXT_STATUS_Failed )
# if defined( FBL_MEM_ENABLE_EXT_STATUS )
# else
#  error Extended status macros defined, but feature not enabled
# endif
#endif /* FBL_MEM_EXT_STATUS_Ok || FBL_MEM_EXT_STATUS_Failed */

#if defined( FBL_MEM_ENABLE_STREAM_OUTPUT )
# if defined( __ApplFblMemIsStreamOutputRequired )
# else
#  error Stream output enabled, but data format check not specified
# endif
#endif /* FBL_MEM_ENABLE_STREAM_OUTPUT */

#if defined( FBL_MEM_ENABLE_PROGRESS_INFO )
# if defined( FBL_ENABLE_PROCESSED_DATA_LENGTH)
#  error Combination of progress information with processed data length not supported
# endif /* FBL_ENABLE_PROCESSED_DATA_LENGTH */
# if defined( __ApplFblMemReportProgress )
# else
#  error Progress information enabled, but report callout not specified
# endif /* __ApplFblMemReportProgress */
#endif /* FBL_MEM_ENABLE_PROGRESS_INFO */

#endif /* __FBL_MEM_H__ */

/***********************************************************************************************************************
 *  END OF FILE: FBL_MEM.H
 **********************************************************************************************************************/
