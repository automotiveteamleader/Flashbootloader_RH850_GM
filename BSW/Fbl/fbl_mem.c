/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief        Library containing common functionality for memory programming:
 *                - Erase of memory region
 *                - Data processing (e.g. decryption and decompression)
 *                - Segmentation and alignment
 *                - Signature / checksum verification over RAM and ROM contents
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
 *                                 -                Added unsigned qualifier to constants
 *  01.01.00   2012-04-05  JHg     ESCAN00057963    Initialization of return value in segment end indication when
 *                                                   remainder handling is disabled
 *                                                  Changes and optimizations after code review
 *  01.02.00   2012-04-27  JHg     ESCAN00058452    Added support for processed length
 *                                 ESCAN00058621    Added __ApplFblMemCopyBuffer
 *  01.03.00   2012-06-22  JHg     ESCAN00059475    Restore preamble in segment end indication
 *                                 ESCAN00059477    Restore default preamble offset in segment end indication
 *                                                  Disable remainder handling for volatile memory
 *                                                  Check for potential buffer overflow in FblMemGetActiveBuffer
 *  01.04.00   2013-02-01  JHg     ESCAN00064290    Extensions for multi processor systems (pass-through)
 *                                                   Rework of block start indication interface
 *                                                   Pass additional info in structure
 *                         JHg     ESCAN00064292    Added block erase
 *                         JHg     ESCAN00064296    Preamble length switchable at runtime
 *                         JHg     ESCAN00064301    Added signature verification on ROM contents
 *                                                   Rework of block verify interface
 *                                                   Keep track of segment history (structure provided externally)
 *                                                   Extended pass-through interface for BlockVerify to pass
 *                                                   user-specific information
 *                         JHg     ESCAN00061335    Accept buffer offsets differing from constant preamble length
 *                                                   in data indication
 *                         JHg     ESCAN00064333    Differentiate watchdog trigger w/ and w/o status
 *                         JHg     ESCAN00061764    Rework of function return paths
 *                         JHg     ESCAN00061814    Encapsulate FblMemQueuePrepend with FBL_ENABLE_DATA_PROCESSING
 *                         JHg     ESCAN00064338    Added post handler for SegmentEndIndication
 *                         Ach     ESCAN00062919    Adapted comments to use Doxygen
 *                         JHg     ESCAN00064334    Added interface to remap error codes to OEM dependent value
 *                         JHg     ESCAN00064339    Segmented data processing: data produced during one cycle may be
 *                                                   smaller than buffer size
 *                         JHg     ESCAN00064330    Explicit resume of suspended operation
 *                                                   Added state "suspend pending"
 *                         JHg     ESCAN00064343    Added support for multiple input sources
 *                         JHg     ESCAN00064543    Release active buffer in case of failed processing
 *                         JHg     ESCAN00064720    Replaced __ApplFblMemCheckDfi by __ApplFblMemIsDataProcessingRequired
 *  01.05.00   2013-04-10  JHg     ESCAN00064871    Reserve space for remainder data to be programmed
 *                         JHg     ESCAN00064890    Move processing queue handle to global context
 *                         JHg     -                Minor fixes
 *                         JHg     ESCAN00065830    Encapsulate setting of gProgContext
 *                         JHg     ESCAN00066375    Restore original data after programming padded buffer
 *                                                  Relocate buffer offset or search matching input buffer
 *                         JHg     ESCAN00066377    Encapsulate erase functionality
 *                         JHg     ESCAN00066379    Added interface version compatibility check
 *                         JHg     ESCAN00066380    Exported FblMemInit
 *  01.06.00   2013-07-22  ASe     ESCAN00066743    Fixed compiler warning regarding gProgContext
 *                         JHg     ESCAN00067433    Added FblMemDeinit
 *                         JHg     ESCAN00068321    Force response pending for erase operation
 *                         JHg     ESCAN00069161    Added pipelined verification
 *                                                   Rework of block start and verify interface
 *                                                   Changed processing queue from FIFO to priority queue
 *                                                   Added jobs for finalization of data processing and remainder
 *                                                   General refactoring
 *                         JHg     ESCAN00069190    Rework suspend operation
 *  01.07.00   2013-08-16  JHg     ESCAN00069507    Corrected input type of FblMemSetInteger
 *                         JHg     ESCAN00069803    Enable/disable verification at run-time
 *                         JHg     ESCAN00069781    Corrected net size of data processing buffer
 *                                                   Added canary word to detect buffer overflows
 *                         JHg     ESCAN00069797    Set low priority of data processing equal to input priority
 *                         JHg     ESCAN00069843    Limit restoring of data after padding to segmented input use-case
 *                                                   Use dedicated buffer to store temporary data
 *  02.00.00   2013-12-12  JHg     ESCAN00069945    Encapsulate write finalization entry in gLengthLimits
 *                         JHg     ESCAN00072568    Perform input verification initialization on first segment start
 *                         JHg     ESCAN00071344    Processed length: Limit programming to area defined by block
 *                         JHg     -                Additional canary word in front of buffer
 *                         JHg     ESCAN00072569    Raised major version to match API version
 *                         JHg     ESCAN00072156    Encapsulation of FblMemGetSpecificRemainder
 *                         JHg     ESCAN00072570    Removed encapsulation of gProgContext
 *                         JHg     ESCAN00072631    Set programming state to pending in FblMemProgramBuffer
 *  02.01.00   2014-03-12  JHg     ESCAN00073504    No changes
 *                         JHg     ESCAN00074066    Explicitly resume operation for finalization of pipelined verification
 *  02.02.00   2014-05-09  JHg     ESCAN00075225    Restore default input offset for RAM data
 *                         JHg     -                Remove explicit memory qualifiers from __ApplFblMemCopyBuffer
 *  03.00.00   2015-03-03  JHg     ESCAN00076591    Added support for external stream output
 *                         JHg     ESCAN00077689    Finalize data processing when remainder handling is disabled
 *                                 -                Support combination of stream output and pipelined verification
 *                         JHg     ESCAN00077891    Added job for handling of segment address (pipelined verification)
 *                         JHg     ESCAN00081491    Added support for resumable programming
 *                         JHg     ESCAN00081494    Added support for verification on processed input data
 *                         JHg     ESCAN00081493    Added selective pipelined programming (forced flush when disabled)
 *  03.01.00   2015-04-23  JHg     ESCAN00082572    Introduced job class for handling of pipelined queues
 *                         JHg     ESCAN00082605    Added support for reporting of progress information
 *                         JHg     ESCAN00082606    Added support for gap fill (requires ordered segments)
 *  03.01.01   2015-04-27  JHg     ESCAN00082700    Removed assertion for consumed buffer in FblProcessJob
 *  03.01.02   2015-05-27  JHg     ESCAN00083138    Reworked padding byte handling and gap fill calculations
 *  03.01.03   2015-06-13  JHg     ESCAN00083358    Don't update position after hashing of segment address (pipelined verification)
 *                         JHg     ESCAN00083390    Update buffer size of pipelined verification read jobs
 *                         JHg     ESCAN00083391    Unconditionally unblock pipelined verification queue when
 *                                                   FBL_MEM_ENABLE_VERIFY_PIPELINED_ADDRESS_LENGTH is enabled
 *                         JHg     ESCAN00083392    Pass correct block length to output verification
 *  03.02.00   2015-07-21  JHg     ESCAN00084101    Unconditionally limit write length for
 *                                                   FBL_ENABLE_ADAPTIVE_DATA_TRANSFER_RCRRP
 *                         JHg     ESCAN00084102    No changes
 *  03.02.01   2015-08-19  JHg     ESCAN00084279    Re-initialize data processing on snapshot restoration
 *                         JHg     ESCAN00084559    Clear write remainder on snapshot creation
 *                         JHg     ESCAN00084560    Force finalization when programming unprocessed data
 *  03.02.02   2015-09-04  JHg     ESCAN00084994    Postpone remainder handling after checkpoint indication
 *  04.00.00   2015-09-17  JHg     ESCAN00085249    Support combination of input and processed verification
 *                         JHg     ESCAN00085250    Run-time decision whether address and length info is included in verification
 *                         JHg     ESCAN00085251    Pass external segment info to __ApplFblMemIsPipelinedProgrammingDisabled
 *  04.01.00   2016-04-01  JHg     ESCAN00087997    Rx interrupt: Critical section around crucial accesses to programming state
 *                         JHg     ESCAN00088935    Corrected segment info pointer passed to __ApplFblMemIsPipelinedProgrammingDisabled
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

#include "fbl_inc.h"
#include "fbl_mem.h"

/***********************************************************************************************************************
 *  VERSION
 **********************************************************************************************************************/

#if ( FBLLIB_MEM_VERSION != 0x0401u ) || \
    ( FBLLIB_MEM_RELEASE_VERSION != 0x00u )
# error "Error in fbl_mem.c: Source and Header file are inconsistent!"
#endif

#if ( FBLLIB_MEM_VERSION != _FBLLIB_MEM_VERSION ) || \
    ( FBLLIB_MEM_RELEASE_VERSION != _FBLLIB_MEM_RELEASE_VERSION )
# error "Error in fbl_mem.c: Source and v_ver.h are inconsistent!"
#endif

/* Interface version compatibility check */
#if defined( FBL_MEM_API_REFERENCE_VERSION_MAJOR ) && \
    defined( FBL_MEM_API_REFERENCE_VERSION_MINOR )
#else
#  error "Error in fbl_mem.c: Interface version requirements not defined!"
#endif

#if ( FBL_MEM_API_REFERENCE_VERSION_MAJOR != FBL_MEM_API_VERSION_MAJOR) || \
    ( FBL_MEM_API_REFERENCE_VERSION_MINOR > FBL_MEM_API_VERSION_MINOR)
# error "Error in fbl_mem.c: Interface version compatibility check failed!"
#endif

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

/* PRQA S 3458 TAG_FblMem_3458_1 */ /* MD_CBD_19.4 */
/* PRQA S 3453 TAG_FblMem_3453_1 */ /* MD_CBD_19.7 */

/** Internal maximum segment size */
#define FBL_MEM_SEGMENT_SIZE              FBL_MAX_SEGMENT_SIZE

/*-- Remap configuration switches--------------------------------------------*/

#if defined( FBL_ENABLE_UNALIGNED_DATA_TRANSFER )
/** Input buffer changed after data indication: Store and restore preamble */
# define FBL_MEM_ENABLE_PREAMBLE_HANDLING
#endif /* FBL_ENABLE_PIPELINED_PROGRAMMING || FBL_ENABLE_UNALIGNED_DATA_TRANSFER */

#if defined( FBL_ENABLE_DATA_PROCESSING ) || \
    defined( FBL_ENABLE_UNALIGNED_DATA_TRANSFER )
/* No remainder handling necessary for single byte segments */
# if ( FBL_MEM_SEGMENT_SIZE > 1u )
/** Programming may result in remainder which has to be stored for next cycle */
#  define FBL_MEM_ENABLE_REMAINDER_HANDLING
# endif /* FBL_MEM_SEGMENT_SIZE > 1u */
#endif /* FBL_ENABLE_DATA_PROCESSING || FBL_ENABLE_UNALIGNED_DATA_TRANSFER */

#if defined( FBL_ENABLE_ADAPTIVE_DATA_TRANSFER_RCRRP )
/** Split buffer operations (write, data processing, verification and pass-through) into smaller pieces */
# define FBL_MEM_ENABLE_SEGMENTATION
#endif /* FBL_ENABLE_PIPELINED_PROGRAMMING || FBL_ENABLE_ADAPTIVE_DATA_TRANSFER_RCRRP */

#if defined( FBL_ENABLE_PROCESSED_DATA_LENGTH )
/** Keep track of running remaining length of input data */
#  define FBL_MEM_ENABLE_INPUT_LENGTH
#endif /* FBL_ENABLE_PIPELINED_PROGRAMMING || FBL_ENABLE_PROCESSED_DATA_LENGTH */

#if defined( FBL_MEM_WRITE_SEGMENTATION )
#else
/** Set segmentation to default value */
# define FBL_MEM_WRITE_SEGMENTATION    (FBL_MEM_BUFFER_SIZE + FBL_MEM_SEGMENT_SIZE)
#endif /* FBL_MEM_WRITE_SEGMENTATION */

#if defined( FBL_ENABLE_DATA_PROCESSING )
# if defined( FBL_MEM_ENABLE_SEGMENTATION )
#  define FBL_MEM_INTERNAL_PROC_SEGMENTATION       FBL_MEM_PROC_SEGMENTATION
#  if ( FBL_MEM_INTERNAL_PROC_SEGMENTATION == FBL_MEM_PROC_BUFFER_SIZE)
/* Segmented processing enabled, but data processing segmentation matches buffer size */
#  else
/* Split data processing into smaller pieces */
#   define FBL_MEM_ENABLE_PROC_SEGMENTATION
#  endif
# else
/* Fill data processing buffer at once */
#  define FBL_MEM_INTERNAL_PROC_SEGMENTATION       FBL_MEM_PROC_BUFFER_SIZE
# endif /* FBL_MEM_ENABLE_SEGMENTATION */
#endif /* FBL_ENABLE_DATA_PROCESSING */

#if defined( FBL_MEM_VERIFY_SEGMENTATION )
# if defined( FBL_MEM_VERIFY_INPUT_SEGMENTATION )
# else
/** Set input verification length to common segmentation length */
#  define FBL_MEM_VERIFY_INPUT_SEGMENTATION        FBL_MEM_VERIFY_SEGMENTATION
# endif /* FBL_MEM_VERIFY_INPUT_SEGMENTATION */
# if defined( FBL_MEM_VERIFY_PIPELINED_SEGMENTATION )
# else
/** Set pipelined verification length to common segmentation length */
#  define FBL_MEM_VERIFY_PIPELINED_SEGMENTATION    FBL_MEM_VERIFY_SEGMENTATION
# endif /* FBL_MEM_VERIFY_PIPELINED_SEGMENTATION */
#endif /* FBL_MEM_VERIFY_SEGMENTATION */

#if defined( FBL_MEM_VERIFY_INPUT_SEGMENTATION )
#else
/** Set input verification length to default value */
# define FBL_MEM_VERIFY_INPUT_SEGMENTATION         64u
#endif /* FBL_MEM_VERIFY_INPUT_SEGMENTATION */

#if defined( FBL_MEM_VERIFY_PIPELINED_SEGMENTATION )
#else
/** Set pipelined verification length to default value */
# define FBL_MEM_VERIFY_PIPELINED_SEGMENTATION     64u
#endif /* FBL_MEM_VERIFY_PIPELINED_SEGMENTATION */

#if ( defined( FBL_MEM_ENABLE_VERIFY_PIPELINED ) && \
      defined( FBL_MEM_ENABLE_VERIFY_PIPELINED_ADDRESS_LENGTH ) )
/** On-the-fly verification includes address and length information */
# define FBL_MEM_ENABLE_VERIFY_ADDRESS_LENGTH
/** Size of address and length information  */
# define FBL_MEM_VERIFY_ADDRESS_LENGTH_SIZE           4u
/** Size of address and length information  */
# define FBL_MEM_VERIFY_ADDRESS_LENGTH_BUFFER_SIZE    (2u * FBL_MEM_VERIFY_ADDRESS_LENGTH_SIZE)
#endif /* (FBL_MEM_ENABLE_VERIFY_INPUT && FBL_MEM_ENABLE_VERIFY_INPUT_ADDRESS_LENGTH) || (FBL_MEM_ENABLE_VERIFY_PIPELINED && FBL_MEM_ENABLE_VERIFY_PIPELINED_ADDRESS_LENGTH) */

/** Response pending handling */
# define FBL_MEM_ENABLE_RESPONSE_PENDING

/** Flush input data in data indication function */
# define FBL_MEM_ENABLE_INPUT_DATA_FLUSH

#if defined( FBL_MEM_ENABLE_GAP_FILL )
# if defined( FBL_MEM_GAP_FILL_SEGMENTATION )
# else
/* Buffer size for gap fill function */
#  define FBL_MEM_GAP_FILL_SEGMENTATION   FBL_MEM_WRITE_SEGMENTATION
# endif /* FBL_MEM_GAP_FILL_SEGMENTATION */
#endif /* FBL_MEM_ENABLE_GAP_FILL */

#if defined( FBL_MEM_ENABLE_PROGRESS_INFO )
# if defined( FBL_MEM_PROGRESS_ERASE )
# else
/** Progress percentage of erase operation */
#  define FBL_MEM_PROGRESS_ERASE                   10u
# endif /* FBL_MEM_PROGRESS_ERASE */
# if defined( FBL_MEM_PROGRESS_VERIFY )
# else
/** Progress percentage of final verify operation */
#  define FBL_MEM_PROGRESS_VERIFY                  10u
# endif /* FBL_MEM_PROGRESS_VERIFY */
# if defined( FBL_MEM_PROGRESS_INITIAL )
# else
/** Initial progress value of overall progress and partial progress */
#  define FBL_MEM_PROGRESS_INITIAL                 0u
# endif /* FBL_MEM_PROGRESS_INITIAL */
# if defined( FBL_MEM_PROGRESS_COMPLETE )
# else
/** Final progress value of overall progress and partial progress */
#  define FBL_MEM_PROGRESS_COMPLETE                100u
# endif /* FBL_MEM_PROGRESS_COMPLETE */
# if defined( FBL_MEM_PROGRESS_PROGRAM )
# else
/** Progress percentage of program operation */
#  define FBL_MEM_PROGRESS_PROGRAM     (FBL_MEM_PROGRESS_COMPLETE - FBL_MEM_PROGRESS_ERASE - FBL_MEM_PROGRESS_VERIFY)
# endif /* FBL_MEM_PROGRESS_PROGRAM */
# if defined( FBL_MEM_PROGRESS_THRESHOLD_BYTES )
# else
/** Threshold for consecutive calls to reporting callback (in bytes) */
#  define FBL_MEM_PROGRESS_THRESHOLD_BYTES         1024u
# endif /* FBL_MEM_PROGRESS_THRESHOLD_BYTES */
# if defined( FBL_MEM_PROGRESS_THRESHOLD_PERCENTAGE )
# else
/** Threshold for consecutive calls to reporting callback (in percentage points) */
#  define FBL_MEM_PROGRESS_THRESHOLD_PERCENTAGE    5u
# endif /* FBL_MEM_PROGRESS_THRESHOLD_PERCENTAGE */
#endif /* FBL_MEM_ENABLE_PROGRESS_INFO */

/*-- Processing queue -------------------------------------------------------*/
/* Number of input buffers */
/** Single buffer use-case */
# define FBL_MEM_QUEUE_ENTRIES_INPUT               1u
/** Actual buffer count
 *  Input queue shared amongst all input sources */
#define FBL_MEM_BUFFER_COUNT_INPUT                 ((FBL_MEM_QUEUE_ENTRIES_INPUT + FBL_MEM_SOURCE_COUNT) - 1u)

# define FBL_MEM_QUEUE_ENTRIES_VERIFY_INPUT        0u

# define FBL_MEM_QUEUE_ENTRIES_VERIFY_PROCESSED    0u

#if defined( FBL_MEM_ENABLE_VERIFY_PIPELINED )
# if defined( FBL_MEM_VERIFY_PIPELINED_JOB_COUNT )
# else
#  define FBL_MEM_VERIFY_PIPELINED_JOB_COUNT       1u
# endif
# if defined( FBL_MEM_ENABLE_VERIFY_ADDRESS_LENGTH )
/** Reserve an additional entry for address information */
#  define FBL_MEM_QUEUE_ENTRIES_VERIFY_PIPE_READ   (FBL_MEM_VERIFY_PIPELINED_JOB_COUNT + 1u)
# else
/** Reserve one entry in dedicated queue per job */
#  define FBL_MEM_QUEUE_ENTRIES_VERIFY_PIPE_READ   (FBL_MEM_VERIFY_PIPELINED_JOB_COUNT)
# endif /* FBL_MEM_ENABLE_VERIFY_ADDRESS_LENGTH */
/** Reserve configured number of entries for read, update and finalization jobs */
# define FBL_MEM_QUEUE_ENTRIES_VERIFY_PIPE         (FBL_MEM_QUEUE_ENTRIES_VERIFY_PIPE_READ + 2u)
#else
# define FBL_MEM_QUEUE_ENTRIES_VERIFY_PIPE         0u
#endif /* FBL_MEM_ENABLE_VERIFY_PIPELINED */

#if defined( FBL_ENABLE_DATA_PROCESSING )
/** Reserve entries for write and finalization jobs */
# define FBL_MEM_QUEUE_ENTRIES_DATA_PROC           2u
#else
# define FBL_MEM_QUEUE_ENTRIES_DATA_PROC           0u
#endif /* FBL_ENABLE_DATA_PROCESSING */

#if defined( FBL_MEM_ENABLE_STREAM_OUTPUT )
/** Reserve entries for write and finalization jobs */
# define FBL_MEM_QUEUE_ENTRIES_STREAM_OUTPUT       2u
#else
# define FBL_MEM_QUEUE_ENTRIES_STREAM_OUTPUT       0u
#endif /* FBL_MEM_ENABLE_STREAM_OUTPUT */

#if defined( FBL_MEM_ENABLE_REMAINDER_HANDLING )
/** Reserve entry for write remainder finalization
  * Only added in case processing queue already used */
# define FBL_MEM_QUEUE_ENTRIES_REMAINDER           1u
#else
# define FBL_MEM_QUEUE_ENTRIES_REMAINDER           0u
#endif /* FBL_MEM_ENABLE_REMAINDER_HANDLING */

# define FBL_MEM_QUEUE_ENTRIES_CHECKPOINT          0u

#if defined( FBL_MEM_ENABLE_GAP_FILL )
/** Reserve entry for gap fill */
# define FBL_MEM_QUEUE_ENTRIES_GAP_FILL            1u
#else
# define FBL_MEM_QUEUE_ENTRIES_GAP_FILL            0u
#endif /* FBL_MEM_ENABLE_GAP_FILL */

/** Total number of processing queue entries */
#define FBL_MEM_QUEUE_ENTRIES_PROCESSING  \
   ( FBL_MEM_QUEUE_ENTRIES_VERIFY_INPUT + FBL_MEM_QUEUE_ENTRIES_VERIFY_PROCESSED + FBL_MEM_QUEUE_ENTRIES_DATA_PROC   \
     + FBL_MEM_QUEUE_ENTRIES_STREAM_OUTPUT + FBL_MEM_QUEUE_ENTRIES_INPUT + FBL_MEM_QUEUE_ENTRIES_VERIFY_PIPE         \
     + FBL_MEM_QUEUE_ENTRIES_CHECKPOINT + FBL_MEM_QUEUE_ENTRIES_GAP_FILL )

#if ( FBL_MEM_QUEUE_ENTRIES_PROCESSING > 1u )
/** Activate processing queue */
# define FBL_MEM_ENABLE_PROC_QUEUE
/** Add remainder handling to processing queue */
# define FBL_MEM_QUEUE_ENTRIES_TOTAL               (FBL_MEM_QUEUE_ENTRIES_PROCESSING + FBL_MEM_QUEUE_ENTRIES_REMAINDER)
#else
/** Single buffer use-case: no processing queue required */
# define FBL_MEM_DISABLE_PROC_QUEUE
#endif /* FBL_MEM_QUEUE_ENTRIES_PROCESSING > 1u */

#if defined( FBL_MEM_ENABLE_REMAINDER_HANDLING )
# if defined( FBL_MEM_ENABLE_PROC_QUEUE )
# else
/** Update single write job to trigger remainder flush */
#  define FBL_MEM_ENABLE_REMAINDER_HANDLING_SINGLE_JOB
# endif /* FBL_MEM_ENABLE_PROC_QUEUE */
#endif /* FBL_MEM_ENABLE_REMAINDER_HANDLING */

#if defined( FBL_MEM_ENABLE_PROC_QUEUE )
/* Processing queue specific */

/** Reserve two entries in data structure for explicit free and used head */
# define FBL_MEM_RESERVED_QUEUE_ENTRIES         2u
/* Handles for free and used head */
# define FBL_MEM_QUEUE_HANDLE_HEAD_USED         0u
# define FBL_MEM_QUEUE_HANDLE_HEAD_FREE         1u
/* Start handle for actual entries */
# define FBL_MEM_QUEUE_HANDLE_ENTRY_OFFSET      2u

/** Size of data structure for processing queue  */
# define FBL_MEM_QUEUE_SIZE_PROCESSING          (FBL_MEM_RESERVED_QUEUE_ENTRIES + FBL_MEM_QUEUE_ENTRIES_TOTAL)
#if defined( FBL_MEM_ENABLE_VERIFY_PIPELINED )
#  define FBL_MEM_QUEUE_SIZE_PIPE_VERIFY        (FBL_MEM_RESERVED_QUEUE_ENTRIES + FBL_MEM_QUEUE_ENTRIES_VERIFY_PIPE_READ)
# endif /* FBL_MEM_ENABLE_VERIFY_PIPELINED */

/*
   Access macros to check for empty queues
   A queue is empty if the head references itself (cyclic double linked list)
*/
# define FblMemQueueIsEmpty(queue)              (FBL_MEM_QUEUE_HANDLE_HEAD_USED == (queue)[FBL_MEM_QUEUE_HANDLE_HEAD_USED].next)
# define FblMemQueueIsFull(queue)               (FBL_MEM_QUEUE_HANDLE_HEAD_FREE == (queue)[FBL_MEM_QUEUE_HANDLE_HEAD_FREE].next)

/* Access macros to get handle of first entry */
# define FblMemQueueGetFirstUsedHandle(queue)   ((queue)[FBL_MEM_QUEUE_HANDLE_HEAD_USED].next)
# define FblMemQueueGetLastUsedHandle(queue)    ((queue)[FBL_MEM_QUEUE_HANDLE_HEAD_USED].prev)
# define FblMemQueueGetFirstFreeHandle(queue)   ((queue)[FBL_MEM_QUEUE_HANDLE_HEAD_FREE].next)

/** Access macro to entry using handle */
# define FblMemQueueGetEntry(queue, handle)     ((queue)[handle])
/* Access macros to get first or last entry */
# define FblMemQueueGetFirstUsedEntry(queue)    (FblMemQueueGetEntry((queue), FblMemQueueGetFirstUsedHandle(queue)))
# define FblMemQueueGetLastUsedEntry(queue)     (FblMemQueueGetEntry((queue), FblMemQueueGetLastUsedHandle(queue)))
# define FblMemQueueGetFirstFreeEntry(queue)    (FblMemQueueGetEntry((queue), FblMemQueueGetFirstFreeHandle(queue)))
#endif /* FBL_MEM_ENABLE_PROC_QUEUE */

/* Default priorities for queue entries */
# define FBL_MEM_QUEUE_PRIO_LOWEST              0x00u
# define FBL_MEM_QUEUE_PRIO_HIGHEST             0xFFu

/** Null queue entry pointer */
# define FBL_MEM_QUEUE_NULL                     ((V_MEMRAM1 tFblMemQueueEntry V_MEMRAM2 V_MEMRAM3 *)V_NULL)
/** Null job pointer */
# define FBL_MEM_JOB_NULL                       ((V_MEMRAM1 tFblMemJob V_MEMRAM2 V_MEMRAM3 *)V_NULL)

/** Remap fill buffer for single input buffer use-case */
# define FblMemGetPendingInputJob()             (&FBL_MEM_INPUT_JOB[0])

/*-- Verification -----------------------------------------------------------*/
/** Null verification status pointer */
#define FBL_MEM_VERIFY_STATUS_NULL              ((V_MEMRAM1 tFblMemVerifyStatus V_MEMRAM2 V_MEMRAM3 *)V_NULL)
#if defined( FBL_MEM_ENABLE_VERIFY_PIPELINED )
/** Null input verification function pointer (use of 0 avoid cast from pointer to object) */
# define FBL_MEM_VERIFY_FCT_INPUT_NULL          ((tFblMemVerifyFctInput)0)
#endif /* FBL_MEM_ENABLE_VERIFY_INPUT || FBL_MEM_ENABLE_VERIFY_PROCESSED || FBL_MEM_ENABLE_VERIFY_PIPELINED */
#if defined( FBL_MEM_ENABLE_VERIFY_OUTPUT )
/** Null input verification function pointer (use of 0 avoid cast from pointer to object) */
# define FBL_MEM_VERIFY_FCT_OUTPUT_NULL         ((tFblMemVerifyFctOutput)0)
#endif /* FBL_MEM_ENABLE_VERIFY_OUTPUT */

/*-- Segment and platform alignment -----------------------------------------*/

/** Calculate remainder for given address range and segment size */
#define FblMemGetRemainder(address, length, segSize)  ((tFblLength)(((address) + (tFblAddress)(length)) & ((tFblAddress)(segSize) - 1u)))
/** Remainder for global segment size */
#define FblMemGetGlobalRemainder(address, length)     FblMemGetRemainder((address), (length), FBL_MEM_SEGMENT_SIZE)
/* Use global or device specific segment size for remainder determination? */
# if defined( FBL_ENABLE_MULTIPLE_MEM_DEVICES ) && \
     defined( FBL_MEM_ENABLE_REMAINDER_HANDLING )
#  define FblMemGetWriteRemainder(address, length)    FblMemGetSpecificRemainder((address), (length))
# else
/** Only one device present, global remainder equals device specific remainder */
#  define FblMemGetWriteRemainder(address, length)    FblMemGetGlobalRemainder((address), (length))
# endif /* FBL_ENABLE_MULTIPLE_MEM_DEVICES || FBL_MEM_ENABLE_REMAINDER_HANDLING */

/* Buffer padding for potential write remainder */
#if defined( FBL_MEM_ENABLE_REMAINDER_HANDLING )
/* Reserve space for remainder from previous cycle and one for buffer padding for last write */
# define FBL_MEM_REMAINDER_PADDING        (FBL_MEM_SEGMENT_SIZE - 1u)
/* Reserve space for buffer padding of last write */
# define FBL_MEM_WRITE_PADDING            (FBL_MEM_SEGMENT_SIZE - 1u)
#else
# define FBL_MEM_REMAINDER_PADDING        0u
# define FBL_MEM_WRITE_PADDING            0u
#endif /* FBL_MEM_ENABLE_REMAINDER_HANDLING */

#define FBL_MEM_TOTAL_PADDING             (FBL_MEM_REMAINDER_PADDING + FBL_MEM_WRITE_PADDING)

#if defined( FBL_ENABLE_SYSTEM_CHECK )
/** Magic value of "canary" word used to detect buffer overflows (ASCII "Bird") */
# define FBL_MEM_CANARY_VALUE       0x42697264ul
#endif /* FBL_ENABLE_SYSTEM_CHECK */

#if defined( C_CPUTYPE_32BIT ) || \
    defined( C_CPUTYPE_16BIT )
# define FBL_MEM_PLATFORM_ALIGN        4u
# define FBL_MEM_PLATFORM_ALIGN_MASK   0x03u
#else /* C_CPUTYPE_8BIT */
# define FBL_MEM_PLATFORM_ALIGN        1u
# define FBL_MEM_PLATFORM_ALIGN_MASK   0x00u
#endif /* C_CPUTYPE_32BIT || C_CPUTYPE_16BIT */

#if defined( FBL_ENABLE_SYSTEM_CHECK )
/** Macro to generate basic data type including buffer alignment to platform requirements */
# define FBL_MEM_ALIGNED_BUFFER_TYPE(size)                                 \
   struct                                                                  \
   {                                                                       \
      /** Magic value of "canary" word used to detect buffer overflows */  \
      vuint32  canaryFront;                                                \
      /** Reserve buffer for configured size and potential alignment */    \
      vuint8   data[size];                                                 \
      /** Magic value of "canary" word used to detect buffer overflows */  \
      vuint32  canaryBack;                                                 \
   } /* PRQA S 3412 */ /* MD_FblMem_3412 */
#else
/* Force alignment to 32 bit boundary (if required by platform) */
# if defined( C_CPUTYPE_32BIT ) || \
     defined( C_CPUTYPE_16BIT )
/** Macro to generate basic data type including buffer alignment to platform requirements */
#  define FBL_MEM_ALIGNED_BUFFER_TYPE(size)                                \
   struct                                                                  \
   {                                                                       \
      vuint32  alignDummy;                                                 \
      /** Reserve buffer for configured size and potential alignment */    \
      vuint8   data[size];                                                 \
   } /* PRQA S 3412 */ /* MD_FblMem_3412 */
# else /* C_CPUTYPE_8BIT */
/** Macro to generate basic data type including buffer alignment to platform requirements */
#  define FBL_MEM_ALIGNED_BUFFER_TYPE(size)                                \
   struct                                                                  \
   {                                                                       \
      /** Reserve buffer for configured size and potential alignment */    \
      vuint8   data[size];                                                 \
   } /* PRQA S 3412 */ /* MD_FblMem_3412 */
# endif /* C_CPUTYPE_32BIT || C_CPUTYPE_16BIT */
#endif /* FBL_ENABLE_SYSTEM_CHECK */

/** Round length up to next multiple of alignment */
#define FBL_MEM_LENGTH_ALIGN(length, align)  (((((length) - 1u) / (align)) + 1u) * (align))

/** Align space used by preamble to multiple of platform requirement */
#define FBL_MEM_PREAMBLE_ALIGN(length)       FBL_MEM_LENGTH_ALIGN((length), FBL_MEM_PLATFORM_ALIGN)
/** As preamble always lies within the actual buffer it can be subtracted from the total length */
#define FBL_MEM_PREAMBLE_OFFSET(length)      (FBL_MEM_PREAMBLE_ALIGN(length) - (length))
/** Padded buffer size for input buffers (including maximum platform alignment) */
#define FBL_MEM_PADDED_BUFFER_SIZE           (FBL_MEM_BUFFER_SIZE + (FBL_MEM_PLATFORM_ALIGN - 1u) + FBL_MEM_TOTAL_PADDING)
#if defined( FBL_ENABLE_DATA_PROCESSING )
/** Padded buffer size for data processing buffers */
# define FBL_MEM_PADDED_PROC_BUFFER_SIZE     (FBL_MEM_PROC_BUFFER_SIZE + FBL_MEM_TOTAL_PADDING)
#endif /* FBL_ENABLE_DATA_PROCESSING */

#if defined( FBL_MEM_ENABLE_MULTI_SOURCE )
/** Dynamic input source */
# define FBL_MEM_ACTIVE_SOURCE               gActiveSource
#else
/** Fixed input source */
# define FBL_MEM_ACTIVE_SOURCE               0u
# if defined( FBL_MEM_SOURCE_COUNT )
# else
/** Set input source count if not already defined*/
#  define FBL_MEM_SOURCE_COUNT               1u
# endif
#endif /* FBL_MEM_ENABLE_MULTI_SOURCE */

#if defined( FBL_MEM_ENABLE_DYNAMIC_PREAMBLE_LENGTH )
/** Dynamic preamble length stored per input source */
# define FBL_MEM_PREAMBLE_LENGTH             gPreambleLength[FBL_MEM_ACTIVE_SOURCE]
#else
/** Fixed preamble length */
# define FBL_MEM_PREAMBLE_LENGTH             FBL_MEM_DEFAULT_PREAMBLE_LENGTH
#endif /* FBL_MEM_ENABLE_DYNAMIC_PREAMBLE_LENGTH */

/** Substitution macro to access input buffers depending on active input source */
#define FBL_MEM_INPUT_JOB                    gInputJobs[FBL_MEM_ACTIVE_SOURCE]

/** Helper macro to get number of array entries */
#define FBL_MEM_ARRAY_SIZE(arr)              (sizeof(arr) / sizeof((arr)[0]))

#if defined( FBL_MEM_ENABLE_SEGMENTATION ) || \
    defined( FBL_MEM_ENABLE_VERIFY_PIPELINED )
# if defined( FBL_ENABLE_ADAPTIVE_DATA_TRANSFER_RCRRP )
/** Unconditionally limited for proper RCR-RP handling */
#  define FBL_MEM_FINALIZE_UNLIMITED_MODE    kFblMemOperationMode_Unconditional
# else
/** Write complete remaining data at once during finalization */
#  define FBL_MEM_FINALIZE_UNLIMITED_MODE    kFblMemOperationMode_Finalize
# endif /* FBL_ENABLE_ADAPTIVE_DATA_TRANSFER_RCRRP */
#else
/** Do not limit input length, process complete buffer */
# define FblMemLimitLength(inputLen, type, finalize)     (inputLen)
#endif /* FBL_MEM_ENABLE_SEGMENTATION || FBL_MEM_ENABLE_VERIFY_PIPELINED */

#if defined( __ApplFblMemCopyBuffer )
#else
/** Copy input buffer to destination address */
# define __ApplFblMemCopyBuffer(address, data, length)   (void)MEMCPY((address), (data), (length))
#endif /* __ApplFblMemCopyBuffer */

/** Null buffer pointer */
# define FBL_MEM_BUFFER_NULL                 ((tFblMemRamData)V_NULL)

/*-- Indication function order ----------------------------------------------*/
/*
   Allowed indication order:

       Init(PowerOn)
             |
   +-------->+--------+
   |         V        |
   | +-> BlockErase   |
   | +-------+        |
   |         V        |
   |     BlockStart <-+
   |         |
   |         V
   |    SegmentStart <-+
   |         |         |
   |         V         |
   |   +-> Data        |
   |   +-----+         |
   |         V         |
   |     SegmentEnd    |
   |         +---------+
   |         V
   |      BlockEnd
   |         |
   |         V
   |    BlockVerify <-+
   |         |        |
   +---------+--------+
*/

/* Bitmasks for allowed states */
#define FBL_MEM_ALLOWED_NONE           0x00u
#define FBL_MEM_ALLOWED_BLOCK_START    0x01u
#define FBL_MEM_ALLOWED_SEGMENT_START  0x02u
#define FBL_MEM_ALLOWED_DATA_IND       0x04u
#define FBL_MEM_ALLOWED_SEGMENT_END    0x08u
#define FBL_MEM_ALLOWED_BLOCK_END      0x10u
#define FBL_MEM_ALLOWED_BLOCK_VERIFY   0x20u
#define FBL_MEM_ALLOWED_BLOCK_ERASE    0x40u

/* Access macros for handling of allowed states */
/** Reset all states */
#define FblMemResetAllowed()           (FblMemSetAllowed(FBL_MEM_ALLOWED_NONE))
/** Set states according to bitmask (overwriting previous settings) */
# define FblMemSetAllowed(mask)        (gAllowedInd = (tFblMemAllowedInd)(mask))
/** Add states according to bitmask (previous settings unchanged) */
# define FblMemAddAllowed(mask)        (gAllowedInd |= (tFblMemAllowedInd)(mask))
/** Clear states according to bitmask */
#define FblMemClrAllowed(mask)         (gAllowedInd &= FblInvertBits(mask, tFblMemAllowedInd))
/** Check for required states against bitmask */
#define FblMemIsAllowed(mask)          ((gAllowedInd & (tFblMemAllowedInd)(mask)) == (tFblMemAllowedInd)(mask))

/*-- Error handling ---------------------------------------------------------*/
/** Dummy value for errors without extended status */
#define FBL_MEM_EXT_STATUS_NONE        0x00u
/** Remap errors without extended status to common macro */
#define FBL_MEM_SET_STATUS(status, var)            FBL_MEM_SET_EXT_STATUS(status, FBL_MEM_EXT_STATUS_NONE, var)
/* Extended status configured? */
#if defined( FBL_MEM_ENABLE_EXT_STATUS )
/** Set error code and additionally pass info extended error macro */
# define FBL_MEM_SET_EXT_STATUS(status, ext, var)  {                                      \
                                                      FBL_MEM_EXT_STATUS_ ## status(ext); \
                                                      (var) = kFblMemStatus_ ## status;   \
                                                   } /* PRQA S 0342 */ /* MD_FblMem_0342 */ /* PRQA S 0881 */ /* MD_FblMem_0881 */
/** Pass extended info without setting error code */
# define FBL_MEM_SET_EXT_INFO(type, info)          FBL_MEM_EXT_STATUS_ ## type(info) /* PRQA S 0342 */ /* MD_FblMem_0342 */
#else
/* Set error code, no extended status */
# define FBL_MEM_SET_EXT_STATUS(status, ext, var)  (var) = kFblMemStatus_ ## status /* PRQA S 0342 */ /* MD_FblMem_0342 */
# define FBL_MEM_SET_EXT_INFO(type, info)
#endif /* FBL_MEM_ENABLE_EXT_STATUS */

/** Assertion codes */
#define kFblMemAssertParameterOutOfRange     0x01u
#define kFblMemAssertUserResultOutOfRange    0x02u

/*-- Critical sections ------------------------------------------------------*/
/* Allow override in configuration */
#if defined( __ApplFblMemEnterCriticalSection )
#else
/** Enter critical section: Do nothing in default configuration */
# define __ApplFblMemEnterCriticalSection()
#endif /* __ApplFblMemEnterCriticalSection */

/* Allow override in configuration */
#if defined( __ApplFblMemLeaveCriticalSection )
#else
/** Leave critical section: Do nothing in default configuration */
# define __ApplFblMemLeaveCriticalSection()
#endif /* __ApplFblMemLeaveCriticalSection */

/* PRQA L:TAG_FblMem_3458_1 */
/* PRQA L:TAG_FblMem_3453_1 */

/***********************************************************************************************************************
 *  TYPEDEFS
 **********************************************************************************************************************/

/*-- Indication function order ----------------------------------------------*/
/** States for indication order */
typedef vuintx tFblMemAllowedInd;

/*-- Buffer handling --------------------------------------------------------*/
/** Job types */
typedef enum
{
   kFblMemJobType_InputWrite = 0u      /**< Input buffer, write through without any data processing         */
#if defined( FBL_MEM_ENABLE_REMAINDER_HANDLING )
   , kFblMemJobType_WriteFinalize      /**< Trigger programming of write remainder                          */
#endif /* FBL_MEM_ENABLE_REMAINDER_HANDLING */
#if defined( FBL_ENABLE_DATA_PROCESSING )
   , kFblMemJobType_ProcInput          /**< Input buffer, process data before writing                       */
   , kFblMemJobType_ProcWrite          /**< Write buffer for processed data                                 */
   , kFblMemJobType_ProcFinalize       /**< Trigger for data processing finalization                        */
#endif /* FBL_ENABLE_DATA_PROCESSING */
#if defined( FBL_MEM_ENABLE_VERIFY_PIPELINED )
# if defined( FBL_MEM_ENABLE_VERIFY_ADDRESS_LENGTH )
   , kFblMemJobType_VerifyPipeInfo     /**< Update pipelined verification hash with segment information     */
# endif /* FBL_MEM_ENABLE_VERIFY_ADDRESS_LENGTH */
   , kFblMemJobType_VerifyPipeRead     /**< Read back data already programmed for pipelined verification    */
   , kFblMemJobType_VerifyPipeUpdate   /**< Update pipelined verification hash                              */
   , kFblMemJobType_VerifyPipeFinalize /**< Trigger finalization of pipelined verification                  */
#endif /* FBL_MEM_ENABLE_VERIFY_PIPELINED */
#if defined( FBL_MEM_ENABLE_STREAM_OUTPUT )
   , kFblMemJobType_StreamInput        /**< Input buffer, pass to stream output                             */
   , kFblMemJobType_StreamProc         /**< Pass processed data to stream output                            */
   , kFblMemJobType_StreamFinalize     /**< Trigger for data processing finalization                        */
#endif /* FBL_MEM_ENABLE_STREAM_OUTPUT */
#if defined( FBL_MEM_ENABLE_GAP_FILL )
   , kFblMemJobType_GapFill            /**< Fill gaps between programmed segments                           */
#endif /* FBL_MEM_ENABLE_GAP_FILL */
   , kFblMemJobType_Max
} tFblMemJobType;

/** Job class, initiate special handling on completion */
#if defined( FBL_MEM_ENABLE_PIPELINING )
typedef enum
{
   tFblMemJobClass_Default       /**< Default job, no special handling required */
# if defined( FBL_MEM_ENABLE_VERIFY_PIPELINED )
   , tFblMemJobClass_VerifyPipe  /**< Pipelined verification job */
# endif /* FBL_MEM_ENABLE_VERIFY_PIPELINED */
} tFblMemJobClass;
#endif /* FBL_MEM_ENABLE_PIPELINING */

/** Flag to indicate finalization to subfunction */
typedef enum
{
   kFblMemOperationMode_Normal = 0u,   /**< Normal operation, job complete as soon as "used" member reaches zero */
   kFblMemOperationMode_Finalize,      /**< Finalize operation */
   kFblMemOperationMode_Unconditional  /**< Job unconditionally completed after first run */
} tFblMemOperationMode;

/** Aligned input buffer type */
typedef FBL_MEM_ALIGNED_BUFFER_TYPE(FBL_MEM_PADDED_BUFFER_SIZE) tFblMemBuffer;
#if defined( FBL_ENABLE_DATA_PROCESSING )
/** Aligned buffer type for data processing */
typedef FBL_MEM_ALIGNED_BUFFER_TYPE(FBL_MEM_PADDED_PROC_BUFFER_SIZE) tFblMemProcBuffer;
#endif /* FBL_ENABLE_DATA_PROCESSING */
#if defined( FBL_MEM_ENABLE_GAP_FILL )
/** Aligned buffer type for gap fill */
typedef FBL_MEM_ALIGNED_BUFFER_TYPE(FBL_MEM_GAP_FILL_SEGMENTATION) tFblMemGapFillBuffer;
#endif /* FBL_MEM_ENABLE_GAP_FILL */

/** Job information */
typedef struct
{
   tFblMemRamData       buffer;           /**< Pointer to referenced buffer                        */
   tFblLength           totalSize;        /**< Total size of referenced buffer                     */
   tFblLength           netSize;          /**< Size actually usable for input data                 */
   tFblLength           offset;           /**< Offset of actual buffer content                     */
   tFblLength           position;         /**< Current position in buffer                          */
   tFblLength           used;             /**< Current data length                                 */
#if defined( FBL_MEM_ENABLE_VERIFY_PIPELINED ) || \
    defined( FBL_MEM_ENABLE_GAP_FILL )
   tFblAddress          baseAddress;      /**< Memory base address                                 */
#endif /* FBL_MEM_ENABLE_VERIFY_PIPELINED || FBL_MEM_ENABLE_RESUMABLE_PROGRAMMING || FBL_MEM_ENABLE_GAP_FILL */
   tFblMemJobType       type;             /**< Job type                                            */
   tFblMemOperationMode completion;       /**< Handling of job completion                          */
   vuintx               segmentIndex;     /**< Index of segment associated with job                */
#if defined( FBL_MEM_ENABLE_PIPELINING )
   tFblMemJobClass      jobClass;         /**< Job class, initiate special handling on completion  */
#endif /* FBL_MEM_ENABLE_PIPELINING */
} tFblMemJob;

/** Module internal segment information */
typedef struct
{
   tFblMemSegmentInfo   input;            /**< Input segment information               */
   /* Internal attributes */
   tFblAddress          writeAddress;     /**< Current address                         */
   tFblLength           writeRemainder;   /**< Unwritten remainder                     */
   tFblLength           writeLength;      /**< Remaining length                        */
#if defined( FBL_ENABLE_PROCESSED_DATA_LENGTH )
   tFblLength           writtenLength;    /**< Total written length                    */
#endif /* FBL_ENABLE_PROCESSED_DATA_LENGTH */
#if defined( FBL_ENABLE_UNALIGNED_DATA_TRANSFER )
   tFblLength           inputAddress;     /**< Running address of input data           */
#endif /* FBL_ENABLE_UNALIGNED_DATA_TRANSFER */
#if defined( FBL_MEM_ENABLE_INPUT_LENGTH )
   tFblLength           inputLength;      /**< Running remaining length of input data  */
#endif /* FBL_MEM_ENABLE_INPUT_LENGTH */
#if defined( FBL_ENABLE_DATA_PROCESSING ) || \
    defined( FBL_MEM_ENABLE_STREAM_OUTPUT ) || \
    defined( FBL_MEM_ENABLE_REMAINDER_HANDLING )
   tFblMemJobType       jobType;          /**< Remember input job types                */
#endif /* FBL_ENABLE_DATA_PROCESSING || FBL_MEM_ENABLE_STREAM_OUTPUT || FBL_MEM_ENABLE_PASSTHROUGH || FBL_MEM_ENABLE_REMAINDER_HANDLING */
   vuintx               ownIndex;         /**< Index of segment                        */
   vuintx               nextIndex;        /**< Index of next segment                   */
} tFblMemSegmentInternal;

/*-- Processing queue -------------------------------------------------------*/
/** Priority of queue entry */
typedef vuint8 tFblMemQueuePrio;

/** Job priorities */
typedef enum
{
   kFblMemJobPrio_Lowest               = FBL_MEM_QUEUE_PRIO_LOWEST,  /**< Lowest job priority */
#if defined( FBL_MEM_ENABLE_VERIFY_PIPELINED )
   kFblMemJobPrio_VerifyPipeFinalize,                                      /**< Finalization of pipelined verification */
   kFblMemJobPrio_VerifyPipeInput,                                         /**< Not actually used, but acts as a common priority
                                                                            *   for all pipelined verification jobs which
                                                                            *   generate input data for update job */
# if defined( FBL_MEM_ENABLE_VERIFY_ADDRESS_LENGTH )
   kFblMemJobPrio_VerifyPipeInfo       = kFblMemJobPrio_VerifyPipeInput,   /**< Pipelined verification segment information */
# endif /* FBL_MEM_ENABLE_VERIFY_ADDRESS_LENGTH */
   kFblMemJobPrio_VerifyPipeRead       = kFblMemJobPrio_VerifyPipeInput,   /**< Pipelined verification read */
   kFblMemJobPrio_VerifyPipeUpdate,                                        /**< Pipelined verification update */
#endif /* FBL_MEM_ENABLE_VERIFY_PIPELINED */
   kFblMemJobPrio_Write,                                                   /**< Helper entry for flushing of all pending write jobs */
#if defined( FBL_MEM_ENABLE_GAP_FILL )
   kFblMemJobPrio_GapFill,                                                 /**< Fill gaps between programmed segments */
#endif /* FBL_MEM_ENABLE_GAP_FILL */
#if defined( FBL_MEM_ENABLE_REMAINDER_HANDLING )
   kFblMemJobPrio_WriteFinalize,                                           /**< Finalize remainder write */
#endif /* FBL_MEM_ENABLE_REMAINDER_HANDLING */
#if defined( FBL_ENABLE_DATA_PROCESSING )
   kFblMemJobPrio_ProcFinalize,                                            /**< Finalization of data processing */
#endif /* FBL_ENABLE_DATA_PROCESSING */
#if defined( FBL_MEM_ENABLE_STREAM_OUTPUT )
   kFblMemJobPrio_StreamFinalize,                                          /**< Finalization of stream output */
#endif /* FBL_MEM_ENABLE_STREAM_OUTPUT */
   kFblMemJobPrio_Input,                                                   /**< Not actually used, but acts as a common priority
                                                                            *   for all input jobs */
   kFblMemJobPrio_InputWrite           = kFblMemJobPrio_Input,             /**< Input: Write through */
#if defined( FBL_MEM_ENABLE_STREAM_OUTPUT )
   kFblMemJobPrio_StreamInput          = kFblMemJobPrio_Input,             /**< Input: Stream output */
   kFblMemJobPrio_StreamProcLow        = kFblMemJobPrio_Input,             /**< Stream output of processed data, low priority
                                                                            *   used when processing is split into smaller pieces
                                                                            *   Prevent overtaking by following input jobs */
#endif /* FBL_MEM_ENABLE_STREAM_OUTPUT */
#if defined( FBL_ENABLE_DATA_PROCESSING )
   kFblMemJobPrio_ProcInput            = kFblMemJobPrio_Input,             /**< Input: Data processing */
   kFblMemJobPrio_ProcWriteLow         = kFblMemJobPrio_Input,             /**< Write operation of data processing, low priority
                                                                            *   used when processing is split into smaller pieces
                                                                            *   Prevent overtaking by following input jobs */
   kFblMemJobPrio_ProcWriteHigh,                                           /**< Write operation of data processing, high priority
                                                                            *   used to flush buffer */
#endif /* FBL_ENABLE_DATA_PROCESSING */
#if defined( FBL_MEM_ENABLE_STREAM_OUTPUT )
   kFblMemJobPrio_StreamProcHigh,                                          /**< Stream output of processed data, high priority
                                                                            *   used to flush buffer */
#endif /* FBL_MEM_ENABLE_STREAM_OUTPUT */
   kFblMemJobPrio_Highest              = FBL_MEM_QUEUE_PRIO_HIGHEST        /**< Highest job priority */ /* PRQA S 0723 */ /* MD_FblMem_0723 */
} tFblMemJobPrio; /* PRQA S 3205 */ /* MD_FblMem_3205 */

#if defined( FBL_MEM_ENABLE_PROC_QUEUE )
/** Handle for queue entry */
typedef vuintx tFblMemQueueHandle;

/** Queue entry */
typedef struct
{
   V_MEMRAM1 tFblMemJob V_MEMRAM2 V_MEMRAM3 *   job;     /**< Associated job */
   tFblMemQueuePrio                             prio;    /**< Entry priority */
   tFblMemQueueHandle                           prev;    /**< Handle of previous queue entry */
   tFblMemQueueHandle                           next;    /**< Handle of next queue entry */
} tFblMemQueueEntry;
#endif /* FBL_MEM_ENABLE_PROC_QUEUE */

#if defined( FBL_MEM_ENABLE_SEGMENTATION ) || \
    defined( FBL_MEM_ENABLE_VERIFY_PIPELINED )
/** Limitation of processed length per cycle */
typedef struct
{
   tFblLength           limit;            /**< Length limit, 0 equals unlimited */
   tFblMemOperationMode unlimitedMode;    /**< Operation mode which returns unlimited length */
} tFblMemLengthLimit;
#endif /* FBL_MEM_ENABLE_SEGMENTATION || FBL_MEM_ENABLE_VERIFY_PIPELINED */

/*-- Pipelined programming --------------------------------------------------*/
#if defined( FBL_MEM_ENABLE_PIPELINING )
/*
   Context of active programming operation
   Generation of RCR-RP message only applicable in service context
   Otherwise service dispatching may not be in a state where RCR-RP is allowed
*/
typedef enum
{
   kFblMemContext_Service = 0u,  /**< Programming directly initiated by service dispatcher      */
   kFblMemContext_Background     /**< Background programming while data transfer is in progress */
} tFblMemContext;
#endif /* FBL_MEM_ENABLE_PIPELINING */

/*-- Progress information ---------------------------------------------------*/
#if defined( FBL_MEM_ENABLE_PROGRESS_INFO )
/** Progress reporting state */
typedef enum
{
   kFblMemProgressState_Disabled,   /**< Disable progress reporting */
   kFblMemProgressState_Enabled     /**< Enable progress reporting */
} tFblMemProgressState;

/** Internal progress information */
typedef struct
{
   vuint32 target;            /**< Expected maximum value of partial operation in arbitrary unit (e.g. bytes) */
   vuint8  totalOffset;       /**< Percentage offset of total operation for current operation type */
   vuint8  totalPercentage;   /**< Percentage contribution to total operation of current operation type */
} tFblMemProgressInfoInternal;
#endif /* FBL_MEM_ENABLE_PROGRESS_INFO */

/*-- Resumable programming --------------------------------------------------*/

/*-- Error handling ---------------------------------------------------------*/

/***********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

# define FBLLIB_MEM_START_SEC_VAR
# include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/** State of programming operation */
V_MEMRAM0        V_MEMRAM1 tFblMemProgState        V_MEMRAM2 fblMemProgState; /* PRQA S 1504 */ /* MD_FblMem_1504 */

/***********************************************************************************************************************
 *  LOCAL DATA
 **********************************************************************************************************************/

/* PRQA S 3218 TAG_FblMem_3218 */ /* MD_FblMem_3218 */

/** States for indication order */
V_MEMRAM0 static V_MEMRAM1 tFblMemAllowedInd       V_MEMRAM2 gAllowedInd;
/** Error status for programming operations (potentially ran in background) */
V_MEMRAM0 static V_MEMRAM1 tFblMemStatus           V_MEMRAM2 gErrorStatus;

#if defined( FBL_MEM_ENABLE_PIPELINING )
/** Context of programming operation (service context / background context) */
V_MEMRAM0 static V_MEMRAM1 tFblMemContext          V_MEMRAM2 gProgContext;
#endif /* FBL_MEM_ENABLE_PIPELINING */

#if defined( FBL_MEM_ENABLE_GLOBAL_BLOCK_INFO )
/** Block information */
V_MEMRAM0 static V_MEMRAM1 tFblMemBlockInfo        V_MEMRAM2 gBlockInfo;
#endif /* FBL_MEM_ENABLE_GLOBAL_BLOCK_INFO */

/** Segment information */
V_MEMRAM0 static V_MEMRAM1 tFblMemSegmentInternal  V_MEMRAM2 gSegInfo;

#if defined( FBL_MEM_ENABLE_MULTI_SOURCE )
/** Active input source */
V_MEMRAM0 static V_MEMRAM1 tFblMemInputSource      V_MEMRAM2 gActiveSource;
#endif

#if defined( FBL_MEM_ENABLE_DYNAMIC_PREAMBLE_LENGTH )
/** Preamble length stored per input source */
V_MEMRAM0 static V_MEMRAM1 tFblLength              V_MEMRAM2 gPreambleLength[FBL_MEM_SOURCE_COUNT];
#endif /* FBL_MEM_ENABLE_DYNAMIC_PREAMBLE_LENGTH */

#if defined( FBL_MEM_ENABLE_PREAMBLE_HANDLING )
/** Temporary buffer for storage of preamble */
V_MEMRAM0 static V_MEMRAM1 vuint8                  V_MEMRAM2 gPreambleBuffer[FBL_MEM_MAX_PREAMBLE_LENGTH];
#endif /* FBL_MEM_ENABLE_PREAMBLE_HANDLING */

#if defined( FBL_MEM_ENABLE_REMAINDER_HANDLING )
/** Temporary buffer for write remainder */
V_MEMRAM0 static V_MEMRAM1 vuint8                  V_MEMRAM2 gRemainderBuffer[FBL_MEM_SEGMENT_SIZE];
#endif /* FBL_MEM_ENABLE_REMAINDER_HANDLING */
#if defined( FBL_MEM_ENABLE_SEGMENTED_INPUT_BUFFER )
/** Temporary buffer for data overwritten by buffer padding */
V_MEMRAM0 static V_MEMRAM1 vuint8                  V_MEMRAM2 gPaddingBuffer[FBL_MEM_SEGMENT_SIZE];
#endif /* FBL_MEM_ENABLE_SEGMENTED_INPUT_BUFFER */

/*-- Input buffers ----------------------------------------------------------*/
/** Data buffer(s) */
V_MEMRAM0 static V_MEMRAM1 tFblMemBuffer           V_MEMRAM2 gBasicInputBuffer[FBL_MEM_BUFFER_COUNT_INPUT];
/** Input jobs */
V_MEMRAM0 static V_MEMRAM1 tFblMemJob              V_MEMRAM2 gInputJobs[FBL_MEM_SOURCE_COUNT][FBL_MEM_QUEUE_ENTRIES_INPUT];

#if defined( FBL_MEM_ENABLE_REMAINDER_HANDLING )
/** Write finalization job */
V_MEMRAM0 static V_MEMRAM1 tFblMemJob              V_MEMRAM2 gWriteFinalizeJob;
#endif /* FBL_MEM_ENABLE_REMAINDER_HANDLING */

/*-- On-the-fly verification ------------------------------------------------*/

#if defined( FBL_MEM_ENABLE_VERIFY_PIPELINED )
/** Pipelined verification jobs */
V_MEMRAM0 static V_MEMRAM1 tFblMemJob              V_MEMRAM2 gVerifyPipeReadJob[FBL_MEM_QUEUE_ENTRIES_VERIFY_PIPE_READ];
V_MEMRAM0 static V_MEMRAM1 tFblMemJob              V_MEMRAM2 gVerifyPipeUpdateJob;
V_MEMRAM0 static V_MEMRAM1 tFblMemJob              V_MEMRAM2 gVerifyPipeFinalizeJob;
V_MEMRAM0 static V_MEMRAM1 vuint8                  V_MEMRAM2 gVerifyPipeReadBuffer[FBL_MEM_VERIFY_PIPELINED_SEGMENTATION];
# if defined( FBL_MEM_ENABLE_VERIFY_PIPELINED_ADDRESS_LENGTH )
/** Data buffer(s) for segment information */
V_MEMRAM0 static V_MEMRAM1 vuint8                  V_MEMRAM2 gVerifyPipeInfoBuffer[FBL_MEM_QUEUE_ENTRIES_VERIFY_PIPE_READ][FBL_MEM_VERIFY_ADDRESS_LENGTH_BUFFER_SIZE];
# endif /* FBL_MEM_ENABLE_VERIFY_ADDRESS_LENGTH */
#endif /* FBL_MEM_ENABLE_VERIFY_PIPELINED */

#if defined( FBL_ENABLE_DATA_PROCESSING )
/*-- Data processing --------------------------------------------------------*/
V_MEMRAM0 static V_MEMRAM1 tProcParam              V_MEMRAM2 gProcParam;
/* Data processing jobs */
V_MEMRAM0 static V_MEMRAM1 tFblMemJob              V_MEMRAM2 gProcWriteJob;
V_MEMRAM0 static V_MEMRAM1 tFblMemJob              V_MEMRAM2 gProcFinalizeJob;
/* Data processing buffer */
V_MEMRAM0 static V_MEMRAM1 tFblMemProcBuffer       V_MEMRAM2 gProcBuffer;

# if defined( FBL_MEM_ENABLE_PROC_SEGMENTATION )
/** Handle of data processing write job in processing queue
    Defined globally as information has to be persistent across multiple calls of FblMemProcessJob */
V_MEMRAM0 static V_MEMRAM1 tFblMemQueueHandle      V_MEMRAM2 gProcHandle;
# endif /* FBL_MEM_ENABLE_PROC_SEGMENTATION */
#endif /* FBL_ENABLE_DATA_PROCESSING */

#if defined( FBL_MEM_ENABLE_STREAM_OUTPUT )
/*-- Stream output ----------------------------------------------------------*/
V_MEMRAM0 static V_MEMRAM1 tFblMemStreamProcessing V_MEMRAM2 gStreamParam;
/* Stream output jobs */
# if defined( FBL_ENABLE_DATA_PROCESSING )
V_MEMRAM0 static V_MEMRAM1 tFblMemJob              V_MEMRAM2 gStreamProcJob;
# endif /* FBL_ENABLE_DATA_PROCESSING */
V_MEMRAM0 static V_MEMRAM1 tFblMemJob              V_MEMRAM2 gStreamFinalizeJob;
#endif /* FBL_MEM_ENABLE_STREAM_OUTPUT */

#if defined( FBL_MEM_ENABLE_GAP_FILL )
V_MEMRAM0 static V_MEMRAM1 tFblMemJob              V_MEMRAM2 gGapFillJob;
V_MEMRAM0 static V_MEMRAM1 tFblMemGapFillBuffer    V_MEMRAM2 gGapFillBuffer;
#endif /* FBL_MEM_ENABLE_GAP_FILL */

#if defined( FBL_MEM_ENABLE_PROGRESS_INFO )
/*-- Progress information ---------------------------------------------------*/
/** Current progress information */
V_MEMRAM0 static V_MEMRAM1 tFblMemProgressInfo           V_MEMRAM2 gProgressInfo;
/** Previously reported progress information */
V_MEMRAM0 static V_MEMRAM1 tFblMemProgressInfo           V_MEMRAM2 gPrevProgressInfo;

/** Explicitely enable or disable reporting of erase progress */
V_MEMRAM0 static V_MEMRAM1 tFblMemProgressState          V_MEMRAM2 gProgressState;

/** Internal progress information (current percentage contribution, target value) */
V_MEMRAM0 static V_MEMRAM1 tFblMemProgressInfoInternal   V_MEMRAM2 gProgressInfoInternal;
/** Value of progress remainder during previous update  */
V_MEMRAM0 static V_MEMRAM1 vuint32                       V_MEMRAM2 gProgressPrevRemainder;
#endif /* FBL_MEM_ENABLE_PROGRESS_INFO */

#if defined( FBL_MEM_ENABLE_PROC_QUEUE )
/*-- Processing queue -------------------------------------------------------*/
/** Processing queue */
V_MEMRAM0 static V_MEMRAM1 tFblMemQueueEntry       V_MEMRAM2 gProcessingQueue[FBL_MEM_QUEUE_SIZE_PROCESSING];
# if defined( FBL_MEM_ENABLE_VERIFY_PIPELINED )
V_MEMRAM0 static V_MEMRAM1 tFblMemQueueEntry       V_MEMRAM2 gVerifyPipeQueue[FBL_MEM_QUEUE_SIZE_PIPE_VERIFY];
# endif /* FBL_MEM_ENABLE_VERIFY_PIPELINED */
#endif /* FBL_MEM_ENABLE_PROC_QUEUE */

# define FBLLIB_MEM_STOP_SEC_VAR
# include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

# define FBLLIB_MEM_START_SEC_CONST
# include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if defined( FBL_MEM_ENABLE_PROC_QUEUE )
/** Mapping of priorities to job types
  * Has to be same order as tFblMemJobType */
V_MEMROM0 static V_MEMROM1 tFblMemQueuePrio V_MEMROM2 gJobPrio[] =
{
   kFblMemJobPrio_InputWrite                       /**< InputWrite */
#if defined( FBL_MEM_ENABLE_REMAINDER_HANDLING )
   , kFblMemJobPrio_WriteFinalize                  /**< WriteFinalize */
#endif /* FBL_MEM_ENABLE_REMAINDER_HANDLING */
# if defined( FBL_ENABLE_DATA_PROCESSING )
   , kFblMemJobPrio_ProcInput                      /**< ProcInput */
#  if defined( FBL_MEM_ENABLE_PROC_SEGMENTATION )
   , kFblMemJobPrio_ProcWriteLow                   /**< ProcWrite, low priority when processing is split into smaller pieces */
   /* kFblMemJobPrio_ProcWriteHigh not explicitely mapped as it is not used when inserting job,
    * but only when temporarily updating the priority because of a full processing buffer */
#  else
   , kFblMemJobPrio_ProcWriteHigh                  /**< ProcWrite, high priority used to flush buffer */
   /* kFblMemJobPrio_ProcWriteLow not used */
#  endif /* FBL_MEM_ENABLE_PROC_SEGMENTATION */
   , kFblMemJobPrio_ProcFinalize                   /**< ProcFinalize */
# endif /* FBL_ENABLE_DATA_PROCESSING */
# if defined( FBL_MEM_ENABLE_VERIFY_PIPELINED )
#  if defined( FBL_MEM_ENABLE_VERIFY_ADDRESS_LENGTH )
   , kFblMemJobPrio_VerifyPipeInfo                 /**< VerifyPipeInfo */
#  endif /* FBL_MEM_ENABLE_VERIFY_ADDRESS_LENGTH */
   , kFblMemJobPrio_VerifyPipeRead                 /**< VerifyPipeRead */
   , kFblMemJobPrio_VerifyPipeUpdate               /**< VerifyPipeUpdate */
   , kFblMemJobPrio_VerifyPipeFinalize             /**< VerifyPipeFinalize */
# endif /* FBL_MEM_ENABLE_VERIFY_PIPELINED */
#if defined( FBL_MEM_ENABLE_STREAM_OUTPUT )
   , kFblMemJobPrio_StreamInput                    /**< StreamInput */
# if defined( FBL_MEM_ENABLE_PROC_SEGMENTATION )
   , kFblMemJobPrio_StreamProcLow                  /**< StreamProc, low priority when processing is split into smaller pieces */
   /* kFblMemJobPrio_StreamProcHigh not explicitely mapped as it is not used when inserting job,
    * but only when temporarily updating the priority because of a full processing buffer */
# else
   , kFblMemJobPrio_StreamProcHigh                 /**< StreamProc, high priority used to flush buffer */
   /* kFblMemJobPrio_StreamProcLow not used */
# endif /* FBL_MEM_ENABLE_PROC_SEGMENTATION */
   , kFblMemJobPrio_StreamFinalize                 /**< StreamFinalize */
#endif /* FBL_MEM_ENABLE_STREAM_OUTPUT */
#if defined( FBL_MEM_ENABLE_GAP_FILL )
   ,kFblMemJobPrio_GapFill                         /**< GapFill */
#endif /* FBL_MEM_ENABLE_GAP_FILL */
   , kFblMemJobPrio_Lowest                         /**< Concluding entry */
};
#endif /* FBL_MEM_ENABLE_PROC_QUEUE */

#if defined( FBL_MEM_ENABLE_SEGMENTATION ) || \
    defined( FBL_MEM_ENABLE_VERIFY_PIPELINED )
/** Limitation of processed length per cycle
  * Has to be same order as tFblMemJobType */
V_MEMROM0 static V_MEMROM1 tFblMemLengthLimit V_MEMROM2 gLengthLimits[] =
{
   { FBL_MEM_WRITE_SEGMENTATION, FBL_MEM_FINALIZE_UNLIMITED_MODE }                     /**< InputWrite */
# if defined( FBL_MEM_ENABLE_REMAINDER_HANDLING )
   , { 0u, kFblMemOperationMode_Normal }                                               /**< WriteFinalize, no actual input data */
# endif /* FBL_MEM_ENABLE_REMAINDER_HANDLING */
# if defined( FBL_ENABLE_DATA_PROCESSING )
   , { 0xFFFFu, kFblMemOperationMode_Normal }                                          /**< ProcInput, limited to 16 bit */
   , { FBL_MEM_WRITE_SEGMENTATION, FBL_MEM_FINALIZE_UNLIMITED_MODE }                   /**< ProcWrite */
   , { 0u, kFblMemOperationMode_Normal }                                               /**< ProcFinalize, no actual input data */
# endif /* FBL_ENABLE_DATA_PROCESSING */
# if defined( FBL_MEM_ENABLE_VERIFY_PIPELINED )
#  if defined( FBL_MEM_ENABLE_VERIFY_ADDRESS_LENGTH )
   , { FBL_MEM_VERIFY_ADDRESS_LENGTH_BUFFER_SIZE, kFblMemOperationMode_Unconditional } /**< VerifyPipeInfo, unconditionally limited by size of segment information */
#  endif /* FBL_MEM_ENABLE_VERIFY_ADDRESS_LENGTH */
   , { FBL_MEM_VERIFY_PIPELINED_SEGMENTATION, kFblMemOperationMode_Unconditional }     /**< VerifyPipeRead, unconditionally limited by temporary buffer */
   , { FBL_MEM_VERIFY_PIPELINED_SEGMENTATION, kFblMemOperationMode_Unconditional }     /**< VerifyPipeUpdate, limited by VerifyPipeRead */
   , { 0u, kFblMemOperationMode_Normal }                                               /**< VerifyPipeFinalize, no actual input data */
# endif /* FBL_MEM_ENABLE_VERIFY_PIPELINED */
# if defined( FBL_MEM_ENABLE_STREAM_OUTPUT )
   , { FBL_MEM_WRITE_SEGMENTATION, FBL_MEM_FINALIZE_UNLIMITED_MODE }                   /**< StreamInput */
   , { FBL_MEM_WRITE_SEGMENTATION, FBL_MEM_FINALIZE_UNLIMITED_MODE }                   /**< StreamProc */
   , { 0u, kFblMemOperationMode_Normal }                                               /**< StreamFinalize, no actual input data */
# endif /* FBL_MEM_ENABLE_STREAM_OUTPUT */
#if defined( FBL_MEM_ENABLE_GAP_FILL )
   , { 0u, kFblMemOperationMode_Normal }                                               /**< GapFill, limit applied in job processing */
#endif /* FBL_MEM_ENABLE_GAP_FILL */
   , { 0u, kFblMemOperationMode_Normal }                                               /**< Concluding entry */
};
#endif /* FBL_MEM_ENABLE_SEGMENTATION || FBL_MEM_ENABLE_VERIFY_PIPELINED */

/*-- Error handling ---------------------------------------------------------*/

/* PRQA L:TAG_FblMem_3218 */

# define FBLLIB_MEM_STOP_SEC_CONST
# include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/***********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

static tFblMemTriggerStatus FblMemTriggerWatchdogExt( void );
static void FblMemTriggerWatchdog( void );
#if defined( FBL_MEM_ENABLE_RESPONSE_PENDING )
static void FblMemResponsePending( void );
#endif /* FBL_MEM_ENABLE_RESPONSE_PENDING */

static void FblMemInitInputQueue( void );
static void FblMemInitStates( void );
static tFblMemRamData FblMemInitInternal( void );
static tFblMemStatus FblMemQueueBuffer( tFblMemConstRamData buffer, tFblLength offset, tFblLength length );
static void FblMemProcessQueue( tFblMemOperationMode mode );
static void FblMemFlushQueueByPrio( tFblMemQueuePrio prio );
#if defined( FBL_MEM_ENABLE_PIPELINING )
static void FblMemUnblockQueue( const V_MEMRAM1 tFblMemQueueEntry V_MEMRAM2 V_MEMRAM3 * queue );
#endif /* FBL_MEM_ENABLE_PIPELINING */
#if defined( FBL_MEM_ENABLE_VERIFY_PIPELINED )
static tFblMemStatus FblMemVerifyInput( V_MEMRAM1 tFblMemVerifyRoutineInput V_MEMRAM2 V_MEMRAM3 * routine,
   const V_MEMRAM1 tFblMemVerifyData V_MEMRAM2 V_MEMRAM3 * data, vuint8 state,
   V_MEMRAM1 tFblMemVerifyStatus V_MEMRAM2 V_MEMRAM3 * result );
#endif /* FBL_MEM_ENABLE_VERIFY_INPUT || FBL_MEM_ENABLE_VERIFY_PROCESSED || FBL_MEM_ENABLE_VERIFY_PIPELINED */
#if defined( FBL_MEM_ENABLE_VERIFY_PIPELINED )
static V_MEMRAM1 tFblMemJob V_MEMRAM2 V_MEMRAM3 * FblMemPrepareVerifyPipeJob( vuint32 segmentIndex, tFblAddress address );
static void FblMemUpdateVerifyPipeJob( tFblAddress address, tFblLength length );
#endif /* FBL_MEM_ENABLE_VERIFY_PIPELINED */
static tFblMemStatus FblMemProcessJob( V_MEMRAM1 tFblMemJob V_MEMRAM2 V_MEMRAM3 * activeJob, tFblMemOperationMode mode );
static tFblMemStatus FblMemProgramStream( const V_MEMRAM1 tFblMemJob V_MEMRAM2 V_MEMRAM3 * programJob,
   V_MEMRAM1 tFblLength V_MEMRAM2 V_MEMRAM3 * programLength, tFblMemOperationMode mode );
static tFblLength FblMemPadLength( tFblAddress address, tFblLength length );
static tFblLength FblMemPadBuffer( tFblAddress address, tFblLength length, tFblMemRamData data );
#if defined( FBL_MEM_ENABLE_SEGMENTED_INPUT_BUFFER )
static void FblMemUnpadBuffer( tFblMemRamData data, tFblLength padLen );
#endif /* FBL_MEM_ENABLE_SEGMENTED_INPUT_BUFFER */
static tFblMemStatus FblMemCopyBuffer( tFblAddress programAddress,
   const V_MEMRAM1 tFblLength V_MEMRAM2 V_MEMRAM3 * programLength, tFblMemConstRamData programData );
static tFblMemStatus FblMemEraseRegionInternal( tFblAddress eraseAddress, tFblLength eraseLength ); /* PRQA S 1505 */ /* MD_FblMem_1505 */
static tFblMemStatus FblMemProgramBufferInternal( tFblAddress programAddress, /* PRQA S 1505 */ /* MD_FblMem_1505 */
   V_MEMRAM1 tFblLength V_MEMRAM2 V_MEMRAM3 * programLength, tFblMemRamData programData,
   tFblMemProgState checkPointState );
# if defined( FBL_ENABLE_MULTIPLE_MEM_DEVICES ) && \
     defined( FBL_MEM_ENABLE_REMAINDER_HANDLING )
static tFblLength FblMemGetSpecificRemainder( tFblAddress address, tFblLength length );
# endif /* FBL_ENABLE_MULTIPLE_MEM_DEVICES && FBL_MEM_ENABLE_REMAINDER_HANDLING */

#if defined( FBL_MEM_ENABLE_SEGMENTED_INPUT_BUFFER )
static tFblMemStatus FblMemRelocateBufferOffset( V_MEMRAM1 tFblMemJob V_MEMRAM2 V_MEMRAM3 * activeJob,
   tFblMemConstRamData buffer, tFblLength offset, tFblLength length );
#endif /* FBL_MEM_ENABLE_SEGMENTED_INPUT_BUFFER */

#if defined( FBL_MEM_ENABLE_SEGMENTATION ) || \
    defined( FBL_MEM_ENABLE_VERIFY_PIPELINED )
static tFblLength FblMemLimitLength( tFblLength inputLen, tFblMemJobType type, tFblMemOperationMode mode );
#endif /* FBL_MEM_ENABLE_SEGMENTATION || FBL_MEM_ENABLE_VERIFY_PIPELINED */

#if defined( FBL_MEM_ENABLE_PREAMBLE_HANDLING )
static void FblMemStorePreamble( void );
static void FblMemRestorePreamble( void );
#endif /* FBL_MEM_ENABLE_PREAMBLE_HANDLING */

static tFblResult FblMemCheckAllowed( tFblMemAllowedInd check, tFblMemAllowedInd clear );
static tFblMemRamData FblMemGetBuffer( const V_MEMRAM1 tFblMemJob V_MEMRAM2 V_MEMRAM3 * job );
#if defined( FBL_ENABLE_SYSTEM_CHECK )
static void FblMemInitBufferIntegrity( void );
static tFblMemStatus FblMemVerifyBufferIntegrity( void );
#endif /* FBL_ENABLE_SYSTEM_CHECK */

#if defined( FBL_MEM_ENABLE_PROC_QUEUE )
static void FblMemQueueInit( V_MEMRAM1 tFblMemQueueEntry V_MEMRAM2 V_MEMRAM3 * queue, tFblMemQueueHandle length );

static tFblMemQueueHandle FblMemQueueMove( V_MEMRAM1 tFblMemQueueEntry V_MEMRAM2 V_MEMRAM3 * queue,
   tFblMemQueueHandle handle, tFblMemQueueHandle prevNew );

# if defined( FBL_MEM_ENABLE_PIPELINING )
static tFblMemQueueHandle FblMemQueueMoveFirstFreeEntry( V_MEMRAM1 tFblMemQueueEntry V_MEMRAM2 V_MEMRAM3 * queue,
   tFblMemQueueHandle prevNew );
static tFblMemQueueHandle FblMemQueueAppend( V_MEMRAM1 tFblMemQueueEntry V_MEMRAM2 V_MEMRAM3 * queue );
# endif /* FBL_MEM_ENABLE_PIPELINING */
static tFblMemQueueHandle FblMemQueueRemove( V_MEMRAM1 tFblMemQueueEntry V_MEMRAM2 V_MEMRAM3 * queue,
   tFblMemQueueHandle handle );
static tFblMemQueueHandle FblMemQueuePrioUpdate( V_MEMRAM1 tFblMemQueueEntry V_MEMRAM2 V_MEMRAM3 * queue,
   tFblMemQueueHandle handle, tFblMemQueuePrio prio );
static tFblMemQueueHandle FblMemQueuePrioInsert( V_MEMRAM1 tFblMemQueueEntry V_MEMRAM2 V_MEMRAM3 * queue,
   tFblMemQueuePrio prio, V_MEMRAM1 tFblMemJob V_MEMRAM2 V_MEMRAM3 * job );
static tFblMemQueueHandle FblMemQueueDefaultPrioInsert( V_MEMRAM1 tFblMemQueueEntry V_MEMRAM2 V_MEMRAM3 * queue,
   V_MEMRAM1 tFblMemJob V_MEMRAM2 V_MEMRAM3 * job, vuintx segmentIndex );
#endif /* FBL_MEM_ENABLE_PROC_QUEUE */

static void FblMemInitJob( V_MEMRAM1 tFblMemJob V_MEMRAM2 V_MEMRAM3 * job, tFblMemRamData buffer,
   tFblLength size, tFblMemJobType type );

#if defined( FBL_MEM_ENABLE_PROGRESS_INFO )
vuint8 FblMemCalculateProgress( vuint32 current, vuint32 total, vuint8 percentage );
void FblMemInitProgress( void );
void FblMemReportProgress( void );
void FblMemSetupProgress( tFblMemProgressType type, tFblAddress logicalAddress, vuint32 segmentCount,
   vuint8 totalOffset, vuint8 totalPercentage, vuint32 target );
void FblMemOffsetProgress( vuint32 totalDone, vuint32 totalTarget );
void FblMemUpdateProgress( vuint32 remainder );
void FblMemConcludeProgress( void );
# if defined( FBL_MEM_ENABLE_VERIFY_OUTPUT )
#  if defined( FBL_MEM_ENABLE_SWITCH_READMEMORY_PARAM )
/* Parameters order changed in comparison to HIS security module specification */
tFblMemVerifySize FblMemProgressRead( tFblMemVerifyAddr address, tFblMemVerifySize length, tFblMemVerifyDataPtr buffer );
#  else
/* Parameters order as defined by HIS security module specification */
tFblMemVerifySize FblMemProgressRead( tFblMemVerifyAddr address, tFblMemVerifyDataPtr buffer, tFblMemVerifySize length );
#  endif /* FBL_MEM_ENABLE_SWITCH_READMEMORY_PARAM */
# endif /* FBL_MEM_ENABLE_VERIFY_OUTPUT */
#endif /* FBL_MEM_ENABLE_PROGRESS_INFO */

/***********************************************************************************************************************
 *  EXTERNAL DATA
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  FblMemTriggerWatchdogExt
 **********************************************************************************************************************/
/*! \brief    Call configured watchdog trigger routine
 *  \details  If pipelined programming is supported and the trigger is called while a background programming operation
 *            is active, the default watchdog trigger is executed.
 *            Otherwise an erroneous RCR-RP could be generated before service dispatching is finished.
 *            E.g. ISO 14229 enforces that certain message checks (length, ...) are performed before the first RCR-RP
 *            transmission.
 *            For all other cases (non-pipelined programming, service context) the configured routine to support an
 *            adaptive RCR-RP generation is called instead.
 *  \return   Watchdog trigger status
 **********************************************************************************************************************/
static tFblMemTriggerStatus FblMemTriggerWatchdogExt( void )
{
   tFblMemTriggerStatus result;

#if defined( FBL_MEM_ENABLE_PIPELINING ) && \
    defined( __ApplFblMemWdTrigger )
   /* Do not generate RCR-RP while programming in background context */
   if (kFblMemContext_Background == gProgContext)
   {
      /* Default watchdog trigger */
      result = __ApplFblMemWdTrigger();
   }
   else
#endif /* FBL_MEM_ENABLE_PIPELINING && __ApplFblMemWdTrigger */
   {
#if defined( __ApplFblMemAdaptiveRcrRp )
      /* Trigger watchdog and dynamically generate RCR-RP */
      result = __ApplFblMemAdaptiveRcrRp();
#else
      /* Callback not configured, return default value */
      result = FBL_MEM_WD_TRIGGER_DEFAULT;
#endif /* __ApplFblMemAdaptiveRcrRp */
   }

   return result;
}

/***********************************************************************************************************************
 *  FblMemTriggerWatchdog
 **********************************************************************************************************************/
/*! \brief    Call configured watchdog trigger routine
 *  \details  Suppress watchdog trigger status (see FblMemTriggerWatchdogExt)
 **********************************************************************************************************************/
static void FblMemTriggerWatchdog( void )
{
   (void)FblMemTriggerWatchdogExt();
}

#if defined( FBL_MEM_ENABLE_RESPONSE_PENDING )
/***********************************************************************************************************************
 *  FblMemResponsePending
 **********************************************************************************************************************/
/*! \brief    Force RCR-RP by calling configured routine
 *  \details  If pipelined programming is supported only execute function if called in service context. Otherwise an
 *            erroneous RCR-RP could be generated before service dispatching is finished
 **********************************************************************************************************************/
static void FblMemResponsePending( void )
{
# if defined( __ApplFblMemForcedRcrRp )
#  if defined( FBL_MEM_ENABLE_PIPELINING )
   /* Do not force RCR-RP while programming in background context */
   if (kFblMemContext_Service == gProgContext)
#  endif /* FBL_MEM_ENABLE_PIPELINING */
   {
#  if defined( __ApplFblMemIsRcrRpActive )
      /* Do not force RCR-RP if already active */
      if (kFblOk != __ApplFblMemIsRcrRpActive())
#  endif /* __ApplFblMemIsRcrRpActive */
      {
         /* Force RCR-RP */
         __ApplFblMemForcedRcrRp();
      }
   }
# endif /* __ApplFblMemForcedRcrRp */
}
#endif /* FBL_MEM_ENABLE_RESPONSE_PENDING */

/***********************************************************************************************************************
 *  FblMemGetBuffer
 **********************************************************************************************************************/
/*! \brief      Get a pointer to the referenced buffer, taking into account the current position value
 *  \param[in]  job Pointer to job information
 *  \return     Pointer to current buffer position
 *              Null pointer in case referenced buffer is not defined
 **********************************************************************************************************************/
static tFblMemRamData FblMemGetBuffer( const V_MEMRAM1 tFblMemJob V_MEMRAM2 V_MEMRAM3 * job )
{
   tFblMemRamData jobBuffer;

   /* Get referenced buffer */
   jobBuffer = job->buffer;

   /* Check for undefined buffer (null pointer) */
   if (FBL_MEM_BUFFER_NULL != jobBuffer)
   {
      /* Check for potential buffer overflow */
      assertFblInternal((job->offset <= (job->totalSize - job->netSize)), kFblMemAssertParameterOutOfRange); /* PRQA S 2214 */ /* MD_FblDef_Assert */
      assertFblInternal((job->used <= (job->totalSize - job->offset)), kFblMemAssertParameterOutOfRange); /* PRQA S 2214 */ /* MD_FblDef_Assert */

      /* Valid buffer pointer
         Evaluate position */
      jobBuffer = &jobBuffer[job->position];
   }

   /* Return pointer to current buffer position */
   return jobBuffer;
}

#if defined( FBL_ENABLE_SYSTEM_CHECK )
/***********************************************************************************************************************
 *  FblMemInitBufferIntegrity
 **********************************************************************************************************************/
/*! \brief         Initialize "canary" words placed after aligned buffer contents
 *  \details       "Canary" value is placed behind actual buffer contents and contains a magic value.
 *                 In case of a buffer overrun it is likely that the "canary" is modified too. This can be used to
 *                 detect the overrun.
 **********************************************************************************************************************/
static void FblMemInitBufferIntegrity( void )
{
   vuintx idx;

   /* Data buffer(s) */
   for (idx = 0u; idx < FBL_MEM_BUFFER_COUNT_INPUT; idx++)
   {
      gBasicInputBuffer[idx].canaryFront = FBL_MEM_CANARY_VALUE;
      gBasicInputBuffer[idx].canaryBack  = FBL_MEM_CANARY_VALUE;
   }

# if defined( FBL_ENABLE_DATA_PROCESSING )
   /* Data processing buffer */
   gProcBuffer.canaryFront = FBL_MEM_CANARY_VALUE;
   gProcBuffer.canaryBack  = FBL_MEM_CANARY_VALUE;
# endif /* FBL_ENABLE_DATA_PROCESSING */
}

/***********************************************************************************************************************
 *  FblMemVerifyBufferIntegrity
 **********************************************************************************************************************/
/*! \brief         Verify "canary" words placed before and after aligned buffer contents
 *  \details       "Canary" values are placed in front and behind actual buffer contents and contain a magic value.
 *                 In case of a buffer overrun it is likely that the "canary" is modified too. This can be used to
 *                 detect the overrun.
 *  \return        kFblMemStatus_Ok if "canary" words of all aligned buffers are intact,
 *                 kFblMemStatus_Failed otherwise
 **********************************************************************************************************************/
static tFblMemStatus FblMemVerifyBufferIntegrity( void )
{
   tFblMemStatus  retVal;
   vuintx idx;

   retVal = kFblMemStatus_Ok;

   /* Data buffer(s) */
   for (idx = 0u; idx < FBL_MEM_BUFFER_COUNT_INPUT; idx++)
   {
      if ( (FBL_MEM_CANARY_VALUE != gBasicInputBuffer[idx].canaryFront)
        || (FBL_MEM_CANARY_VALUE != gBasicInputBuffer[idx].canaryBack) )
      {
         retVal = kFblMemStatus_Failed;

         break;
      }
   }

# if defined( FBL_ENABLE_DATA_PROCESSING )
   /* Data processing buffer */
   if ( (FBL_MEM_CANARY_VALUE != gProcBuffer.canaryFront)
     || (FBL_MEM_CANARY_VALUE != gProcBuffer.canaryBack) )
   {
      retVal = kFblMemStatus_Failed;
   }
# endif /* FBL_ENABLE_DATA_PROCESSING */

   return retVal;
}
#endif /* FBL_ENABLE_SYSTEM_CHECK */

#if defined( FBL_MEM_ENABLE_PROC_QUEUE )
/* Queue handling ************************************************************/

/***********************************************************************************************************************
 *  FblMemQueueInit
 **********************************************************************************************************************/
/*! \brief         Initialize queue structure
 *  \details       Implements two double linked lists, representing a used and a free queue.
 *  \param[in,out] queue Pointer to queue array
 *  \param[in]     length Total length of queue, including used and free head
 **********************************************************************************************************************/
static void FblMemQueueInit( V_MEMRAM1 tFblMemQueueEntry V_MEMRAM2 V_MEMRAM3 * queue, tFblMemQueueHandle length )
{
   tFblMemQueueHandle handle;
   tFblMemQueueHandle prevHandle;
   tFblMemQueueHandle nextHandle;

   /* Start with empty queue by self-reference */
   queue[FBL_MEM_QUEUE_HANDLE_HEAD_USED].next   = FBL_MEM_QUEUE_HANDLE_HEAD_USED;
   queue[FBL_MEM_QUEUE_HANDLE_HEAD_USED].prev   = FBL_MEM_QUEUE_HANDLE_HEAD_USED;
   /* Set default values */
   queue[FBL_MEM_QUEUE_HANDLE_HEAD_USED].job    = FBL_MEM_JOB_NULL;
   queue[FBL_MEM_QUEUE_HANDLE_HEAD_USED].prio   = FBL_MEM_QUEUE_PRIO_HIGHEST;

   /* Setup double linked list of empty queue entries */
   /* Head references last entry as predecessor */
   prevHandle = length - 1u;
   nextHandle = FBL_MEM_QUEUE_HANDLE_HEAD_FREE;

   /* Append available entries to free queue */
   for (handle = FBL_MEM_QUEUE_HANDLE_HEAD_FREE; handle < length; handle++)
   {
      nextHandle++;

      /* Set predecessor and successor */
      queue[handle].prev   = prevHandle;
      queue[handle].next   = nextHandle;
      /* Set default values */
      queue[handle].job    = FBL_MEM_JOB_NULL;
      queue[handle].prio   = FBL_MEM_QUEUE_PRIO_LOWEST;

      prevHandle = handle;
   }

   /* Last entry references head as successor */
   queue[length - 1u].next = FBL_MEM_QUEUE_HANDLE_HEAD_FREE;
}

/***********************************************************************************************************************
 *  FblMemQueueMove
 **********************************************************************************************************************/
/*! \brief         Cut queue entry from current position and place it at a different location (either in same or other queue)
 *  \pre           FblMemQueueInit executed before, source queue not empty
 *                 Passed handles have to represent actual predecessor and successor relationship
 *  \param[in,out] queue Pointer to queue array
 *  \param[in]     handle Handle of queue entry to be moved
 *  \param[in]     prevNew Handle of new predecessor
 *  \return        Handle of affected entry
 **********************************************************************************************************************/
static tFblMemQueueHandle FblMemQueueMove( V_MEMRAM1 tFblMemQueueEntry V_MEMRAM2 V_MEMRAM3 * queue,
   tFblMemQueueHandle handle, tFblMemQueueHandle prevNew )
{
   tFblMemQueueHandle prevOld;
   tFblMemQueueHandle nextOld;
   tFblMemQueueHandle nextNew;

   /* Check for matching handles */
   if (handle == prevNew)
   {
      /* Entry placed at exact same location
         No need to change anything */
   }
   else
   {
      /* Get current predecessor and successor */
      prevOld              = queue[handle].prev;
      nextOld              = queue[handle].next;

      /* Old queue empty? */
      assertFblInternal((queue[prevOld].next != prevOld), kFblMemAssertParameterOutOfRange); /* PRQA S 2214 */ /* MD_FblDef_Assert */

      /* Remove entry from old queue */
      queue[prevOld].next  = nextOld;
      queue[nextOld].prev  = prevOld;

      /* Get new predecessor */
      nextNew              = queue[prevNew].next;

      /* Insert entry into new queue */
      queue[handle].prev   = prevNew;
      queue[prevNew].next  = handle;
      queue[handle].next   = nextNew;
      queue[nextNew].prev  = handle;
   }

   return handle;
}

# if defined( FBL_MEM_ENABLE_PIPELINING )
/***********************************************************************************************************************
 *  FblMemQueueMoveFirstFreeEntry
 **********************************************************************************************************************/
/*! \brief         Take first free entry, remove it from free queue and insert it into defined location in used queue.
 *  \pre           FblMemQueueInit executed before, queue not full
 *  \param[in,out] queue Pointer to queue array
 *  \param[in]     prevNew Handle of new predecessor
 *  \return        Handle of moved entry
 **********************************************************************************************************************/
static tFblMemQueueHandle FblMemQueueMoveFirstFreeEntry( V_MEMRAM1 tFblMemQueueEntry V_MEMRAM2 V_MEMRAM3 * queue,
   tFblMemQueueHandle prevNew )
{
   /* Free queue empty? */
   assertFblInternal((!FblMemQueueIsFull(queue)), kFblMemAssertParameterOutOfRange); /* PRQA S 2214 */ /* MD_FblDef_Assert */

   /* Move first free entry to new position */
   return FblMemQueueMove(queue, queue[FBL_MEM_QUEUE_HANDLE_HEAD_FREE].next, prevNew);
}

/***********************************************************************************************************************
 *  FblMemQueueAppend
 **********************************************************************************************************************/
/*! \brief         Append entry to end of queue
 *  \details       Take first free entry, remove it from free queue and insert it as last entry into used queue.
 *  \pre           FblMemQueueInit executed before, queue not full
 *  \param[in,out] queue Pointer to queue array
 *  \return        Handle of appended entry
 **********************************************************************************************************************/
static tFblMemQueueHandle FblMemQueueAppend( V_MEMRAM1 tFblMemQueueEntry V_MEMRAM2 V_MEMRAM3 * queue )
{
   /* Insert at end of used queue
      Place between current last entry and tail (== head) */
   return FblMemQueueMoveFirstFreeEntry(queue, queue[FBL_MEM_QUEUE_HANDLE_HEAD_USED].prev);
}

# endif /* FBL_MEM_ENABLE_PIPELINING */

/***********************************************************************************************************************
 *  FblMemQueueRemove
 **********************************************************************************************************************/
/*! \brief         Remove specific entry from queue
 *  \details       Take specific entry, remove it from queue and insert it as last entry into free queue.
 *  \pre           FblMemQueueInit executed before, queue not empty
 *  \param[in,out] queue Pointer to queue array
 *  \param[in]     handle Queue entry handle
 *  \return        Handle of removed entry
 **********************************************************************************************************************/
static tFblMemQueueHandle FblMemQueueRemove( V_MEMRAM1 tFblMemQueueEntry V_MEMRAM2 V_MEMRAM3 * queue,
   tFblMemQueueHandle handle )
{
   /* Insert at end of free queue
      Place between current last entry and tail (== head) */
   return FblMemQueueMove(queue, handle, queue[FBL_MEM_QUEUE_HANDLE_HEAD_FREE].prev);
}

/***********************************************************************************************************************
 *  FblMemQueuePrioUpdate
 **********************************************************************************************************************/
/*! \brief         Update priority of a queue entry
 *  \details       Take specific entry, remove it from queue and re-insert it according to the updated priority.
 *  \pre           FblMemQueueInit executed before, queue not empty
 *  \param[in,out] queue Pointer to queue array
 *  \param[in]     handle Queue entry handle
 *  \param[in]     prio New priority value
 *  \return        Handle of updated entry
 **********************************************************************************************************************/
static tFblMemQueueHandle FblMemQueuePrioUpdate( V_MEMRAM1 tFblMemQueueEntry V_MEMRAM2 V_MEMRAM3 * queue,
   tFblMemQueueHandle handle, tFblMemQueuePrio prio )
{
   tFblMemQueueHandle prevHandle;

   /* Start search at last entry */
   prevHandle = queue[FBL_MEM_QUEUE_HANDLE_HEAD_USED].prev;

   /* Skip all entries with lower priority
      Remark: Search is assured to stop at head because it has the highest possible priority */
   while (queue[prevHandle].prio < prio)
   {
      prevHandle = queue[prevHandle].prev;
   }

   /* Append after first entry with higher or equal priority */
   (void)FblMemQueueMove(queue, handle, prevHandle);
   /* Update priority of inserted entry */
   queue[handle].prio = prio;

   return handle;
}

/***********************************************************************************************************************
 *  FblMemQueuePrioInsert
 **********************************************************************************************************************/
/*! \brief         Insert entry into queue using given priority
 *  \details       Take first free entry, remove it from free queue and insert it according to the given priority.
 *                 Additionally set job associated with queue entry
 *  \pre           FblMemQueueInit executed before, queue not empty
 *  \param[in,out] queue Pointer to queue array
 *  \param[in]     prio Priority value
 *  \param[in]     job Job associated with queue entry
 *  \return        Handle of inserted entry
 **********************************************************************************************************************/
static tFblMemQueueHandle FblMemQueuePrioInsert( V_MEMRAM1 tFblMemQueueEntry V_MEMRAM2 V_MEMRAM3 * queue,
   tFblMemQueuePrio prio, V_MEMRAM1 tFblMemJob V_MEMRAM2 V_MEMRAM3 * job )
{
   tFblMemQueueHandle handle;

   /* Free queue empty? */
   assertFblInternal((!FblMemQueueIsFull(queue)), kFblMemAssertParameterOutOfRange); /* PRQA S 2214 */ /* MD_FblDef_Assert */

   /* Relocate first free entry according to given priority */
   handle = FblMemQueuePrioUpdate(queue, queue[FBL_MEM_QUEUE_HANDLE_HEAD_FREE].next, prio);
   /* Set job of entry */
   queue[handle].job = job;

   return handle;
}

/***********************************************************************************************************************
 *  FblMemQueueDefaultPrioInsert
 **********************************************************************************************************************/
/*! \brief         Insert entry into queue using default priority
 *  \details       Take first free entry, remove it from free queue and insert it according to the default priority of
                   the given job's type. Additionally set job associated with queue entry and update segment index of job.
 *  \pre           FblMemQueueInit executed before, queue not empty
 *  \param[in,out] queue Pointer to queue array
 *  \param[in]     job Job associated with queue entry
 *  \param[in]     segmentIndex Segment index assigned to job
 *  \return        Handle of inserted entry
 **********************************************************************************************************************/
static tFblMemQueueHandle FblMemQueueDefaultPrioInsert( V_MEMRAM1 tFblMemQueueEntry V_MEMRAM2 V_MEMRAM3 * queue,
   V_MEMRAM1 tFblMemJob V_MEMRAM2 V_MEMRAM3 * job, vuintx segmentIndex )
{
   /* Valid job type? */
   assertFblInternal((job->type < kFblMemJobType_Max), kFblMemAssertParameterOutOfRange); /* PRQA S 2214 */ /* MD_FblDef_Assert */

   /* Remember segment index associated with job */
   job->segmentIndex = segmentIndex;

   return FblMemQueuePrioInsert(queue, gJobPrio[job->type], job);
}
#endif /* FBL_MEM_ENABLE_PROC_QUEUE */

/***********************************************************************************************************************
 *  FblMemInitJob
 **********************************************************************************************************************/
/*! \brief     Initialize job information structure
 *  \details   Assign given buffer to job and set type
 *  \param[in,out] job Job information to be initialized
 *  \param[in]     buffer Buffer associated with job
 *  \param[in]     size Size of given buffer
 *  \param[in]     type Type of job
 **********************************************************************************************************************/
static void FblMemInitJob( V_MEMRAM1 tFblMemJob V_MEMRAM2 V_MEMRAM3 * job, tFblMemRamData buffer, tFblLength size,
   tFblMemJobType type )
{
   /* Assign given buffer */
   job->buffer          = buffer;
   job->totalSize       = size;
   job->netSize         = size;
   /* Default values */
   job->offset          = 0u;
   job->position        = 0u;
   job->used            = 0u;
#if defined( FBL_MEM_ENABLE_PIPELINING )
   /* Default job */
   job->jobClass        = tFblMemJobClass_Default;
#endif /* FBL_MEM_ENABLE_PIPELINING */
   /* Set job type */
   job->type            = type;
   /* Job complete as soon as "used" member reaches zero */
   job->completion      = kFblMemOperationMode_Normal;
}

/***********************************************************************************************************************
 *  FblMemInitInputQueue
 **********************************************************************************************************************/
/*! \brief     Initialize input queue
 *  \details   Additionally job information of input buffers is prepared.
 *             Will also be carried out for single buffer use-case
 **********************************************************************************************************************/
static void FblMemInitInputQueue( void )
{
   vuintx idx;
   vuintx actualBasicIdx;

   /* First input buffer assigned to specified input source */
   actualBasicIdx = FBL_MEM_ACTIVE_SOURCE;

   idx = 0u;

   {
      /* Reset job information and assign actual buffers */
      FblMemInitJob( &FBL_MEM_INPUT_JOB[idx], gBasicInputBuffer[actualBasicIdx].data,
                     FBL_MEM_ARRAY_SIZE(gBasicInputBuffer[actualBasicIdx].data), kFblMemJobType_InputWrite );
      /* Overwrite net size, to exclude overhead for remainder and padding */
      FBL_MEM_INPUT_JOB[idx].netSize   = FBL_MEM_BUFFER_SIZE;
      /* Default offset to align actual data to platform requirements */
      FBL_MEM_INPUT_JOB[idx].offset    = FBL_MEM_PREAMBLE_OFFSET(FBL_MEM_PREAMBLE_LENGTH);

   }
}

/***********************************************************************************************************************
 *  FblMemInitStates
 **********************************************************************************************************************/
/*! \brief    Setup all data structures
 *  \pre      FblMemInitPowerOn executed before
 **********************************************************************************************************************/
static void FblMemInitStates( void )
{
   /* Setup idle state */
   fblMemProgState   = kFblMemProgState_Idle;
   gErrorStatus      = kFblMemStatus_Ok;
#if defined( FBL_MEM_ENABLE_PIPELINING )
   gProgContext      = kFblMemContext_Service;
#endif /* FBL_MEM_ENABLE_PIPELINING */

   /* No operations allowed */
   FblMemResetAllowed();
}

/***********************************************************************************************************************
 *  FblMemInitInternal
 **********************************************************************************************************************/
/*! \brief    Setup all data structures
 *  \pre      FblMemInitPowerOn executed before
 *  \return   Pointer to active input buffer
 **********************************************************************************************************************/
static tFblMemRamData FblMemInitInternal( void )
{
#if defined( FBL_MEM_ENABLE_VERIFY_PIPELINED )
   vuintx idx;
   tFblMemQueueHandle queueHandle;
#endif /* FBL_MEM_ENABLE_VERIFY_PIPELINED */

   /* Setup idle state */
   FblMemInitStates();

#if defined( FBL_MEM_ENABLE_PROC_QUEUE )
   /* Setup processing queue */
   FblMemQueueInit(gProcessingQueue, FBL_MEM_ARRAY_SIZE(gProcessingQueue));
#endif /* FBL_MEM_ENABLE_PROC_QUEUE */
   FblMemInitInputQueue();

#if defined( FBL_MEM_ENABLE_REMAINDER_HANDLING )
   FblMemInitJob(&gWriteFinalizeJob, gRemainderBuffer, FBL_MEM_SEGMENT_SIZE, kFblMemJobType_WriteFinalize);
#endif /* FBL_MEM_ENABLE_REMAINDER_HANDLING */

#if defined( FBL_ENABLE_DATA_PROCESSING )
   /* Reset current data length, no full re-initialization necessary */
   gProcWriteJob.used = 0u;
#endif /* FBL_ENABLE_DATA_PROCESSING */

#if defined( FBL_MEM_ENABLE_STREAM_OUTPUT ) && \
    defined( FBL_ENABLE_DATA_PROCESSING )
   /* Reset current data length, no full re-initialization necessary */
   gStreamProcJob.used = 0u;
#endif /* FBL_MEM_ENABLE_STREAM_OUTPUT && FBL_ENABLE_DATA_PROCESSING*/

#if defined( FBL_MEM_ENABLE_VERIFY_PIPELINED )
   /* Setup pipelined verification queue */
   FblMemQueueInit(gVerifyPipeQueue, FBL_MEM_ARRAY_SIZE(gVerifyPipeQueue));

   queueHandle = (tFblMemQueueHandle)FBL_MEM_QUEUE_HANDLE_ENTRY_OFFSET;

# if ( FBL_MEM_QUEUE_ENTRIES_VERIFY_PIPE_READ == 1u )
   idx = 0u;
# else
   for (idx = 0u; idx < FBL_MEM_QUEUE_ENTRIES_VERIFY_PIPE_READ; idx++)
# endif /* FBL_MEM_QUEUE_ENTRIES_VERIFY_PIPE_READ */
   {
      /* Initialize job */
# if defined( FBL_MEM_ENABLE_VERIFY_PIPELINED_ADDRESS_LENGTH )
      /* Assign dedicated buffer for segment information */
      FblMemInitJob(&gVerifyPipeReadJob[idx], gVerifyPipeInfoBuffer[idx], FBL_MEM_VERIFY_ADDRESS_LENGTH_BUFFER_SIZE, kFblMemJobType_VerifyPipeRead);
# else
      FblMemInitJob(&gVerifyPipeReadJob[idx], FBL_MEM_BUFFER_NULL, 0, kFblMemJobType_VerifyPipeRead);
# endif
      /* Set dedicated job class */
      gVerifyPipeReadJob[idx].jobClass = tFblMemJobClass_VerifyPipe;

      /* Assign job to verify queue entry */
      FblMemQueueGetEntry(gVerifyPipeQueue, queueHandle).job = &gVerifyPipeReadJob[idx];
# if ( FBL_MEM_QUEUE_ENTRIES_VERIFY_PIPE_READ > 1u )
      queueHandle++;
# endif /* FBL_MEM_QUEUE_ENTRIES_VERIFY_PIPE_READ */
   }

   /* Initialize jobs for update and finalize */
   FblMemInitJob(&gVerifyPipeUpdateJob, gVerifyPipeReadBuffer, FBL_MEM_VERIFY_PIPELINED_SEGMENTATION, kFblMemJobType_VerifyPipeUpdate);
   FblMemInitJob(&gVerifyPipeFinalizeJob, FBL_MEM_BUFFER_NULL, 0, kFblMemJobType_VerifyPipeFinalize);
#endif /* FBL_MEM_ENABLE_VERIFY_PIPELINED */

#if defined( FBL_ENABLE_SYSTEM_CHECK )
   FblMemInitBufferIntegrity();
#endif /* FBL_ENABLE_SYSTEM_CHECK */

#if defined( FBL_MEM_ENABLE_PROGRESS_INFO )
   FblMemInitProgress();
#endif /* FBL_MEM_ENABLE_PROGRESS_INFO */

   /* Allow block start / erase indication */
   FblMemSetAllowed(FBL_MEM_ALLOWED_BLOCK_START | FBL_MEM_ALLOWED_BLOCK_ERASE);

   return FblMemGetActiveBuffer();
}

#if defined( FBL_MEM_ENABLE_PREAMBLE_HANDLING )
/***********************************************************************************************************************
 *  FblMemStorePreamble
 **********************************************************************************************************************/
/*! \brief      Store the current buffer preamble (protocol information before actual data)
 *  \details    Active fill buffer may be exchanged by subsequent operations, so preamble has to be restored for
 *              calling instance
 *  \pre        FblMemInitPowerOn executed before, fill buffer available
 **********************************************************************************************************************/
static void FblMemStorePreamble( void )
{
#if defined( FBL_MEM_ENABLE_DYNAMIC_PREAMBLE_LENGTH )
   /* Verify preamble fits into reserved buffer */
   assertFblGen(FBL_MEM_PREAMBLE_LENGTH <= FBL_MEM_MAX_PREAMBLE_LENGTH, kFblMemAssertParameterOutOfRange); /* PRQA S 2214 */ /* MD_FblDef_Assert */
#endif /* FBL_MEM_ENABLE_DYNAMIC_PREAMBLE_LENGTH */

   /* Copy preamble from active fill to temporary buffer */
   (void)MEMCPY(gPreambleBuffer, FblMemGetActiveBuffer(), FBL_MEM_PREAMBLE_LENGTH);
}

/***********************************************************************************************************************
 *  FblMemRestorePreamble
 **********************************************************************************************************************/
/*! \brief      Restore the previously stored preamble (protocol information before actual data) into the (new) active
 *              fill buffer
 *  \details    Active buffer could be exchanged by preceding operations, so preamble has to be restored for calling
 *              instance
 *  \pre        FblMemInitPowerOn executed before, fill buffer available
 **********************************************************************************************************************/
static void FblMemRestorePreamble( void )
{
   /* Copy previously stored preamble from temporary to active fill buffer */
   (void)MEMCPY(FblMemGetActiveBuffer(), gPreambleBuffer, FBL_MEM_PREAMBLE_LENGTH);
}
#endif /* FBL_MEM_ENABLE_PREAMBLE_HANDLING */

#if defined( FBL_MEM_ENABLE_SEGMENTATION ) || \
    defined( FBL_MEM_ENABLE_VERIFY_PIPELINED )
/***********************************************************************************************************************
 *  FblMemLimitLength
 **********************************************************************************************************************/
/*! \brief        Length will be truncated if limit is exceeded
 *  \details      Original value will be returned if finalization operation mode is set
 *  \param[in]    inputLen Original length
 *  \param[in]    type Type of active job
 *  \param[in]    mode Current operation mode (used for finalization)
 *  \return       Input length limited to given range
 **********************************************************************************************************************/
static tFblLength FblMemLimitLength( tFblLength inputLen, tFblMemJobType type, tFblMemOperationMode mode )
{
   tFblLength lengthLimit;

# if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   /* Parameters not used: avoid compiler warning */
#  if defined( FBL_MEM_ENABLE_SEGMENTATION )
#  else
   (void)mode; /* PRQA S 3112 */ /* MD_FblMem_3112 */
#  endif /* FBL_MEM_ENABLE_SEGMENTATION */
# endif /* V_ENABLE_USE_DUMMY_STATEMENT */

   /* Valid job type? */
   assertFblInternal((type < kFblMemJobType_Max), kFblMemAssertParameterOutOfRange); /* PRQA S 2214 */ /* MD_FblDef_Assert */

   lengthLimit = gLengthLimits[type].limit;

   if (lengthLimit > 0)
   {
# if defined( FBL_MEM_ENABLE_SEGMENTATION )
      /* Segmentation explicitly enabled
         Truncate to given limit, unless unlimited mode for job type (typically finalize) is set */
      if ((gLengthLimits[type].unlimitedMode != mode) && (inputLen > lengthLimit))
# else
      /* Truncate to given limit, if unconditional mode is configured for job type */
      if ((gLengthLimits[type].unlimitedMode == kFblMemOperationMode_Unconditional) && (inputLen > lengthLimit))
# endif /* FBL_MEM_ENABLE_SEGMENTATION */
      {
         inputLen = lengthLimit;
      }
   }

   return inputLen;
}
#endif /* FBL_MEM_ENABLE_SEGMENTATION || FBL_MEM_ENABLE_VERIFY_PIPELINED */

/***********************************************************************************************************************
 *  FblMemPadLength
 **********************************************************************************************************************/
/*! \brief      Calculate number of bytes required to pad provided address range to memory segment size
 *  \param[in]  address Start address of memory range
 *  \param[in]  length Length of memory range
 *  \return     Number of required padding bytes
 **********************************************************************************************************************/
static tFblLength FblMemPadLength( tFblAddress address, tFblLength length )
{
   tFblLength  localAddress;
   tFblLength  localLength;
   tFblLength  padLen;
   tFblAddress alignMask;
   vsint16     oldSegment;

   /* Local copy of length */
   localLength = length;
   /* Special handling required for zero length
      Address used directly */
   if (length > 0u)
   {
      localLength--;
   }

   /* Calculate end address */
   localAddress  = address + localLength;

   /* Initialize padding length */
   padLen      = 0u;

   /* Remember current memory segment */
   oldSegment  = memSegment;

   /* Evaluate memory segment */
   memSegment  = FblMemSegmentNrGet(localAddress);

   /* Check if segment was found */
   if (memSegment >= 0)
   {
      /* Bit mask for memory segment alignment */
      alignMask = (tFblAddress)(MemDriver_SegmentSize - 1u);

      /* Padding length calculated for end address of data to be padded */
      /* Invert all masked bits */
      padLen = localAddress ^ alignMask;

      /* Modify padding length for special case (zero length) */
      if (0u == length)
      {
         padLen++;
      }

      /* Apply mask to cut of unnecessary bits (e.g. additional address info, overflow of addition) */
      padLen &= alignMask;
   }

   /* Restore memory segment */
   memSegment = oldSegment;

   return padLen;
}

/***********************************************************************************************************************
 *  FblMemPadBuffer
 **********************************************************************************************************************/
/*! \brief      Fill up the provided buffer with kFillChar if not aligned to the memory segment size
 *  \pre        Buffer provided has to be large enough to hold added padding bytes
 *  \param[in]  address Start address of memory range
 *  \param[in]  length Length of memory range
 *  \param[in,out]  data Pointer to last byte of actual data
 *  \return     Number of padded bytes
 **********************************************************************************************************************/
static tFblLength FblMemPadBuffer( tFblAddress address, tFblLength length, tFblMemRamData data )
{
   tFblMemRamData padBuffer;
   tFblLength padLen;
   tFblLength idx;

   /* Calculate number of required padding bytes */
   padLen = FblMemPadLength(address, length);

   /* In case data was already aligned the last buffer byte may be located at the very last memory address
      Prevent address wrap around by relocating buffer not until padding is necessary */
   if (padLen > 0u)
   {
      /* Input buffer points to last byte of actual data */
      padBuffer = &data[1];

      /* Append data to align buffer to segment size */
      for (idx = 0u; idx < padLen; idx++)
      {
#if defined( FBL_MEM_ENABLE_SEGMENTED_INPUT_BUFFER )
         /* Save original data, restored after programming operation */
         gPaddingBuffer[idx] = padBuffer[idx];
#endif /* FBL_MEM_ENABLE_SEGMENTED_INPUT_BUFFER */

         padBuffer[idx] = kFillChar;
      }
   }

   return padLen;
}

#if defined( FBL_MEM_ENABLE_SEGMENTED_INPUT_BUFFER )
/***********************************************************************************************************************
 *  FblMemUnpadBuffer
 **********************************************************************************************************************/
/*! \brief      Restore previous state of buffer after padding with fill pattern
 *  \pre        memSegment correctly initialized (only relevant for multiple memory devices configuration)
 *              FblMemPadBuffer called before with exact same parameters
 *  \param[in]  data Pointer to last byte of actual data
 *  \param[in]  padLen Number of previously padded bytes
 **********************************************************************************************************************/
static void FblMemUnpadBuffer( tFblMemRamData data, tFblLength padLen )
{
   /* In case data was already aligned the last buffer byte may be located at the very last memory address
      Prevent address wrap around by relocating buffer not until padding is necessary */
   if (padLen > 0u)
   {
      /* Restore original data, overwritten by padding */
      (void)MEMCPY(&data[1], gPaddingBuffer, padLen);
   }
}
#endif /* FBL_MEM_ENABLE_SEGMENTED_INPUT_BUFFER */

/***********************************************************************************************************************
 *  FblMemCopyBuffer
 **********************************************************************************************************************/
/*! \brief      Performs program operation to volatile memory
 *  \param[in]  programAddress Program address
 *  \param[in]  programLength Length of data (output: length actually programmed)
 *  \param[in]  programData Pointer to program data
 *  \return     Result of operation (potentially remapped to OEM specific NRC)
 **********************************************************************************************************************/
static tFblMemStatus FblMemCopyBuffer( tFblAddress programAddress,
   const V_MEMRAM1 tFblLength V_MEMRAM2 V_MEMRAM3 * programLength, tFblMemConstRamData programData )
{
   /* Copy input buffer to destination address */
   __ApplFblMemCopyBuffer(programAddress, programData, *programLength); /* PRQA S 0306 */ /* MD_FblMem_0306 */ /* PRQA S 0432 */ /* MD_FblHw_Memcpy */

   return kFblMemStatus_Ok;
}

/***********************************************************************************************************************
 *  FblMemEraseRegionInternal
 **********************************************************************************************************************/
/*! \brief      Performs erase operation in non-volatile memory
 *  \details    All memory segments fully or partially covered by given region are affected. Gaps in the memory segment
 *              definition are skipped.
 *  \pre        Memory driver initialized
 *  \param[in]  eraseAddress Start address of erase region
 *  \param[in]  eraseLength Length of erase region
 *  \return     Result of operation (potentially remapped to OEM specific NRC)
 **********************************************************************************************************************/
static tFblMemStatus FblMemEraseRegionInternal( tFblAddress eraseAddress, tFblLength eraseLength ) /* PRQA S 1505 */ /* MD_FblMem_1505 */
{
   tFblMemStatus  retVal;
   IO_ErrorType   flashErrorCode;
   tFblLength     eraseRemainder;
   tFblLength     currentLength;
   tFblLength     distance;
   vsint16        nextMemSegment;

   retVal         = kFblMemStatus_Ok;

   eraseRemainder = eraseLength;
   memSegment     = FblMemSegmentNrGet(eraseAddress);

   /* Check if there is a valid block for start address */
   if (memSegment < 0)
   {
      FBL_MEM_SET_EXT_INFO(EraseAddress, eraseAddress);
      FBL_MEM_SET_STATUS(EraseOutsideFbt, retVal);  /* PRQA S 3109 */ /* MD_CBD_14.3 */
   }
   else
   {
      nextMemSegment = memSegment;

      while (eraseRemainder > 0u)
      {
         /* Watchdog and response pending handling */
         FblMemTriggerWatchdog();
         /* Force response pending on first loop cycle */
         FblMemResponsePending();

         /* Initialize error address */
         FBL_MEM_SET_EXT_INFO(EraseAddress, eraseAddress);

#if defined( __ApplFblMemDriverReady )
         /* Check if required flash driver is initialized */
         if (kFblOk != __ApplFblMemDriverReady(memSegment))
         {
            /* Flash driver initialization failure */
            FBL_MEM_SET_STATUS(EraseDriverNotReady, retVal);  /* PRQA S 3109 */ /* MD_CBD_14.3 */
            return retVal;
         }
#endif

         /* Check if erase range crosses Flash block boundary */
         currentLength = (FlashBlock[memSegment].end - eraseAddress) + 1u;
         if (eraseRemainder > currentLength)
         {
            nextMemSegment = memSegment + 1;

            if (nextMemSegment >= kNrOfFlashBlock)
            {
               /* End of erase region lies behind defined memory */
               FBL_MEM_SET_STATUS(EraseOutsideFbt, retVal);  /* PRQA S 3109 */ /* MD_CBD_14.3 */
               return retVal;
            }

            /* Update remainder */
            distance = FlashBlock[nextMemSegment].begin - eraseAddress;
            if (distance <= eraseRemainder)
            {
               eraseRemainder -= distance;
            }
            else
            {
               /* End of erase region lies in gap */
               FBL_MEM_SET_STATUS(EraseOutsideFbt, retVal);  /* PRQA S 3109 */ /* MD_CBD_14.3 */
               return retVal;
            }
         }
         else
         {
            currentLength = eraseRemainder;
            eraseRemainder = 0u;
         }

#if defined( __ApplFblMemPreErase )
         /* Perform actions directly before memory driver write */
         if (kFblOk != __ApplFblMemPreErase())
         {
            FBL_MEM_SET_STATUS(ErasePreErase, retVal); /* PRQA S 3109 */ /* MD_CBD_14.3 */
            return retVal;
         }
#endif /* __ApplFblMemPreErase */
         flashErrorCode = MemDriver_REraseSync(currentLength, eraseAddress);
#if defined( __ApplFblMemPostErase )
         /* Perform actions directly after memory driver write */
         if (kFblOk != __ApplFblMemPostErase())
         {
            FBL_MEM_SET_STATUS(ErasePostErase, retVal);   /* PRQA S 3109 */ /* MD_CBD_14.3 */
            return retVal;
         }
#endif /* __ApplFblMemPostErase */

         if (IO_E_OK != flashErrorCode)
         {
            /* Error while erasing */
            FBL_MEM_SET_EXT_STATUS(DriverErase, flashErrorCode, retVal);   /* PRQA S 3109 */ /* MD_CBD_14.3 */
            return retVal;
         }

#if defined( FBL_MEM_ENABLE_PROGRESS_INFO )
         /* Only report progress when explicitly enabled */
         if (kFblMemProgressState_Enabled == gProgressState)
         {
            FblMemUpdateProgress(eraseRemainder);
         }
#endif /* FBL_MEM_ENABLE_PROGRESS_INFO */

         /* Continue with next segment */
         memSegment = nextMemSegment;
         eraseAddress = FlashBlock[memSegment].begin;
      }
   }

   return retVal;
}  /* PRQA S 2006 */ /* MD_CBD_14.7 */

/***********************************************************************************************************************
 *  FblMemProgramBufferInternal
 **********************************************************************************************************************/
/*! \brief         Performs program operation to non-volatile memory
 *  \details       If the length is not aligned to the segment size the odd bytes are padded with the configured fill
 *                 character.
 *                 Programming may be suspended by an external event. In this case parameter programLength will be
 *                 updated to reflect the length actually programmed
 *                 In case resumable programming is active, crossing a flash block boundary leads to the setting
 *                 of the programming state to the passed value.
 *  \pre           Memory driver initialized, address aligned to memory segment size
 *  \param[in]     programAddress Program address
 *  \param[in,out] programLength Length of data (output: length actually programmed)
 *  \param[in,out] programData Pointer to program data (contents are padded in case length is not aligned to memory
 *                  segment size!)
 *  \param[in]     checkPointState Programming state value to be set in case a checkpoint is reached
 *                  Typical values are either kFblMemProgState_Checkpoint (suspend programming and indicate checkpoint)
 *                  or kFblMemProgState_Pending (continue programming)
 *
 *  \return        Result of operation (potentially remapped to OEM specific NRC)
 **********************************************************************************************************************/
static tFblMemStatus FblMemProgramBufferInternal( tFblAddress programAddress, /* PRQA S 1505 */ /* MD_FblMem_1505 */
   V_MEMRAM1 tFblLength V_MEMRAM2 V_MEMRAM3 * programLength, tFblMemRamData programData, tFblMemProgState checkPointState )
{
   tFblMemStatus     retVal;
   tFblLength        currentLength;
   tFblLength        localLength;
   tFblLength        segLength;
   tFblLength        padLength;
   tFblLength        bufferIndex;
   tFblLength        padOffset;
   IO_ErrorType      flashErrorCode;

#if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   /* Parameters not used: avoid compiler warning */
   (void)checkPointState; /* PRQA S 3112 */ /* MD_FblMem_3112 */
#endif /* V_ENABLE_USE_DUMMY_STATEMENT */

   retVal      = kFblMemStatus_Ok;
   padLength   = 0u;                                     /* PRQA S 3198 */ /* MD_FblMem_3198 */
   padOffset   = 0u;                                     /* PRQA S 3198 */ /* MD_FblMem_3198 */

   /* Copy requested length to local variable */
   localLength    = *programLength;
   /* Start at beginning of buffer */
   bufferIndex    = 0u;

   /* Loop while data left and operation not suspended by external event */
   while ((localLength > 0u) && (kFblMemProgState_Pending == fblMemProgState))
   {
      /* Watchdog and response pending handling */
      FblMemTriggerWatchdog();
#if defined( FBL_ENABLE_ADAPTIVE_DATA_TRANSFER_RCRRP )
      /* Disable forced RCR-RP for adaptive mode */
#else
      /* Force response pending on first loop cycle */
      FblMemResponsePending();
#endif /* FBL_ENABLE_ADAPTIVE_DATA_TRANSFER_RCRRP */

      /* Initialize error address */
      FBL_MEM_SET_EXT_INFO(ProgramAddress, programAddress);

      /* Length to be programmed in current loop cycle */
      currentLength = localLength;
      /* Evaluate memory segment */
      memSegment    = FblMemSegmentNrGet(programAddress);

      /* Check if segment was found */
      if (memSegment < 0)
      {
         FBL_MEM_SET_STATUS(ProgramOutsideFbt, retVal);  /* PRQA S 3109 */ /* MD_CBD_14.3 */
         return retVal;
      }

#if defined( __ApplFblMemDriverReady )
      /* Check if required flash driver is initialized */
      if (kFblOk != __ApplFblMemDriverReady(memSegment))
      {
         /* Flash driver initialization failure */
         FBL_MEM_SET_STATUS(ProgramDriverNotReady, retVal);  /* PRQA S 3109 */ /* MD_CBD_14.3 */
         return retVal;
      }
#endif

      /* Check if address is aligned to memory segment size */
      if (0u != (programAddress & ((tFblAddress)MemDriver_SegmentSize - 1u)))
      {
         FBL_MEM_SET_STATUS(ProgramUnalignedAddress, retVal);  /* PRQA S 3109 */ /* MD_CBD_14.3 */
         return retVal;
      }

      /* Remaining length until end of current segment */
      segLength = (FlashBlock[memSegment].end - programAddress) + 1u;

      /* Download memory overlaps segment: adjust to current segment */
      if (segLength < currentLength)
      {
         /* Adapt currentLength to segment programming size */
         currentLength = segLength;
      }
      else
      {
         /* Offset to last byte of actual data */
         padOffset   = bufferIndex + (currentLength - 1u);

         /* Pad buffer to memory segment */
         padLength = FblMemPadBuffer(programAddress, currentLength, &programData[padOffset]);
         currentLength += padLength;
      }

#if defined( __ApplFblMemPreWrite )
      /* Perform actions directly before memory driver write */
      if (kFblOk != __ApplFblMemPreWrite())
      {
         FBL_MEM_SET_STATUS(ProgramPreWrite, retVal); /* PRQA S 3109 */ /* MD_CBD_14.3 */
         return retVal;
      }
#endif /* __ApplFblMemPreWrite */

      /* Pass programming request to memory driver */
      flashErrorCode = MemDriver_RWriteSync(&programData[bufferIndex], currentLength, programAddress);

#if defined( FBL_MEM_ENABLE_SEGMENTED_INPUT_BUFFER )
      /* Restore original data, overwritten by padding */
      FblMemUnpadBuffer(&programData[padOffset], padLength);
#endif /* FBL_MEM_ENABLE_SEGMENTED_INPUT_BUFFER */

#if defined( __ApplFblMemPostWrite )
      /* Perform actions directly after memory driver write */
      if (kFblOk != __ApplFblMemPostWrite())
      {
         FBL_MEM_SET_STATUS(ProgramPostWrite, retVal);   /* PRQA S 3109 */ /* MD_CBD_14.3 */
         return retVal;
      }
#endif /* __ApplFblMemPostWrite */

      /* Check result of programming operation */
      if (IO_E_OK != flashErrorCode)
      {
         FBL_MEM_SET_EXT_STATUS(DriverWrite, flashErrorCode, retVal);   /* PRQA S 3109 */ /* MD_CBD_14.3 */
         return retVal;
      }

      /* Update address for next cycle */
      programAddress += (tFblAddress)currentLength;

      if (localLength > currentLength)
      {
         /* Prepare buffer index for next loop */
         bufferIndex += currentLength;
         localLength -= currentLength;
      }
      else
      {
         /* Lengths should be exactly the same here */
         assertFblInternal(((currentLength - padLength) == localLength), kFblMemAssertParameterOutOfRange); /* PRQA S 2214 */ /* MD_FblDef_Assert */

         /* Buffer completely processed */
         localLength = 0u;
      }

   }

   /* Return length actually written (without padding) */
   *programLength -= localLength;

   return retVal;
} /* PRQA S 6050 */ /* MD_FblMem_60xx */ /* PRQA S 2006 */ /* MD_CBD_14.7 */

# if defined( FBL_ENABLE_MULTIPLE_MEM_DEVICES ) && \
     defined( FBL_MEM_ENABLE_REMAINDER_HANDLING )
/***********************************************************************************************************************
 *  FblMemGetSpecificRemainder
 **********************************************************************************************************************/
/*! \brief      Calculate remainder of given range specific for affected memory device
 *  \param[in]  address Start address of memory range
 *  \param[in]  length Length of memory range
 *  \return     Remainder to full memory segment in byte
 **********************************************************************************************************************/
static tFblLength FblMemGetSpecificRemainder( tFblAddress address, tFblLength length )
{
   tFblLength  remainder;
   vsint16     tempSegment;

   /* Initialize remainder */
   remainder = 0u;

   /* Re-evaluate memory segment */
   tempSegment = memSegment;
   memSegment = FblMemSegmentNrGet(address);

   if (memSegment >= 0)
   {
      /* Calculate remainder */
      remainder = FblMemGetRemainder(address, length, MemDriver_SegmentSize);
   }

   /* Restore previous memory segment value */
   memSegment = tempSegment;

   return remainder;
}
# endif /* FBL_ENABLE_MULTIPLE_MEM_DEVICES && FBL_MEM_ENABLE_REMAINDER_HANDLING */

#if defined( FBL_MEM_ENABLE_SEGMENTED_INPUT_BUFFER )
/***********************************************************************************************************************
 *  FblMemRelocateBufferOffset
 **********************************************************************************************************************/
/*! \brief         Verify provided buffer lies within range of existing buffer and relocate offset accordingly
 *  \details       Provided buffer has to reside completely in existing buffer
 *  \param[in,out] activeJob Information of buffer to be updated
 *  \param[in]     buffer Pointer to provided buffer
 *  \param[in]     offset Offset index into provided buffer
 *  \param[in]     length Length of data in provided buffer
 *  \return        kFblMemStatus_Ok if requested buffer lies inside provided information and offset could be relocated,
 *                 kFblMemStatus_Failed otherwise
 **********************************************************************************************************************/
static tFblMemStatus FblMemRelocateBufferOffset( V_MEMRAM1 tFblMemJob V_MEMRAM2 V_MEMRAM3 * activeJob,
   tFblMemConstRamData buffer, tFblLength offset, tFblLength length )
{
   tFblMemStatus  retVal;
   tFblMemRamData activeBuffer;
   tFblLength activeOffset;

   retVal         = kFblMemStatus_Failed;
   activeBuffer   = activeJob->buffer;

   /* Provided buffer has to reside in raw input buffer */
   if (buffer >= activeBuffer)
   {
      activeOffset = (tFblLength)(buffer - activeBuffer); /* PRQA S 0488 */ /* MD_FblMem_0488 */

      /* Provided data has to lie within raw input buffer (excluding potential padding) */
      if ( (length <= activeJob->netSize)
        && (offset <= (activeJob->netSize - length))
        && (activeOffset < activeJob->totalSize)
        && ((offset + length + FBL_MEM_REMAINDER_PADDING) <= (activeJob->totalSize - activeOffset)) )
      {
#if defined( FBL_MEM_ENABLE_REMAINDER_HANDLING )
         /* Remainder requires reserved area in front of actual data (defined by current offset) */
         if ((activeOffset + offset) >= activeJob->offset)
#endif /* FBL_MEM_ENABLE_REMAINDER_HANDLING */
         {
            /* Relocate offset to provided buffer */
            activeJob->offset = activeOffset;
            retVal = kFblMemStatus_Ok;
         }
      }
   }

   return retVal;
}
#endif /* FBL_MEM_ENABLE_SEGMENTED_INPUT_BUFFER */

/***********************************************************************************************************************
 *  FblMemProgramStream
 **********************************************************************************************************************/
/*! \brief         Take a byte stream to be programmed and align it to the memory requirements
 *  \details       If the length is not aligned to the segment size the odd bytes are stored in a remainder buffer,
 *                 which will be programmed on the next call.
 *                 Programming may be suspended by an external event. In this case parameter programLength will be
 *                 updated to reflect the length actually programmed.
 *  \pre           FblMemSegmentStartIndication executed before
 *  \param[in]     programJob Information of buffer to be programmed
 *  \param[in,out] programLength Requested program length (output: length actually programmed)
 *  \param[in]     mode Operation mode (include remainder in case of finalization)
 *  \return        Result of operation (potentially remapped to OEM specific NRC)
 **********************************************************************************************************************/
static tFblMemStatus FblMemProgramStream( const V_MEMRAM1 tFblMemJob V_MEMRAM2 V_MEMRAM3 * programJob,
   V_MEMRAM1 tFblLength V_MEMRAM2 V_MEMRAM3 * programLength, tFblMemOperationMode mode )
{
   tFblMemStatus  retVal;
   tFblMemRamData programData;
   tFblLength     localLength;
#if defined( FBL_MEM_ENABLE_REMAINDER_HANDLING )
   tFblLength     requestLength;
   tFblLength     remainder;
#endif /* FBL_MEM_ENABLE_REMAINDER_HANDLING */

#if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   /* Parameters not used: avoid compiler warning */
# if defined( FBL_MEM_ENABLE_REMAINDER_HANDLING )
# else
   (void)mode; /* PRQA S 3112 */ /* MD_FblMem_3112 */
# endif /* FBL_MEM_ENABLE_REMAINDER_HANDLING */
#endif /* V_ENABLE_USE_DUMMY_STATEMENT */

   FblMemTriggerWatchdog();

   /* Store requested length to local variable */
   localLength = *programLength;
   /* Pointer to current buffer position */
   programData = FblMemGetBuffer(programJob);

#if defined( FBL_MEM_ENABLE_REMAINDER_HANDLING )
   remainder   = 0u;

   /* No remainder handling required for volatile memory */
   if (kFblMemType_RAM != gSegInfo.input.type)
   {
      /*
         Handle remainder from previous run
         Special case where remainder is empty is not explicitly handled
         as performing the following operations with a value of zero
         doesn't have any side-effects
      */

      /* Check available space in front of current data */
      assertFblInternal(gSegInfo.writeRemainder <= programJob->position, kFblMemAssertParameterOutOfRange); /* PRQA S 2214 */ /* MD_FblDef_Assert */

      /* Redirect programming pointer to make room for remainder */
      programData = &programJob->buffer[programJob->position - gSegInfo.writeRemainder];

      /* Copy remainder in front of actual data */
      (void)MEMCPY(programData, gRemainderBuffer, gSegInfo.writeRemainder);
      /* Update length to be programmed */
      localLength += gSegInfo.writeRemainder;

      /* Write complete data if explicit finalization is requested */
      if ( (kFblMemOperationMode_Finalize == mode)
# if defined( FBL_ENABLE_PROCESSED_DATA_LENGTH )
# else
      /* or end of requested segment is reached */
        || (localLength == gSegInfo.writeLength)
# endif /* FBL_ENABLE_PROCESSED_DATA_LENGTH */
         )
      {
         /* No remainder left */
      }
      else
      {
         /* Calculate new remainder, not aligned to segment size */
         remainder = FblMemGetWriteRemainder(gSegInfo.writeAddress, localLength);
         /* Update length to be programmed */
         localLength -= remainder;

      }
   }

   /* Remember actual programming length, may be altered by programming routine */
   requestLength = localLength;
#endif /* FBL_MEM_ENABLE_REMAINDER_HANDLING */

   /* Verify complete programming length does not exceed previously requested memory range */
#if defined( FBL_MEM_ENABLE_REMAINDER_HANDLING )
   if ((localLength + remainder) > gSegInfo.writeLength)
#else
   if (localLength > gSegInfo.writeLength)
#endif /* FBL_MEM_ENABLE_REMAINDER_HANDLING */
   {
      FBL_MEM_SET_STATUS(ProgramOverflow, retVal); /* PRQA S 3109 */ /* MD_CBD_14.3 */
   }
   else
   {
      /*
         Call programming function even if actual length is zero
         In this case the input length will be completely moved to the remainder
      */
      if (kFblMemType_RAM == gSegInfo.input.type)
      {
         /* Copy to volatile memory */
         retVal = FblMemCopyBuffer(gSegInfo.writeAddress, &localLength, programData);
      }
      else
      {
         /* Program non-volatile memory */
         retVal = FblMemProgramBufferInternal(gSegInfo.writeAddress, &localLength, programData, kFblMemProgState_Checkpoint);
      }
   }

   /* Check result */
   if (kFblMemStatus_Ok == retVal)
   {
      /* Returned length shall not exceed requested length */
      assertFblInternal(localLength <= gSegInfo.writeLength, kFblMemAssertParameterOutOfRange); /* PRQA S 2214 */ /* MD_FblDef_Assert */

      /* Update segment information */
      gSegInfo.writeAddress    += (tFblAddress)localLength;
      /* Reduce expected remainder */
      gSegInfo.writeLength     -= localLength;
#if defined( FBL_ENABLE_PROCESSED_DATA_LENGTH )
      /* Length actually written */
      gSegInfo.writtenLength   += localLength;
#endif /* FBL_ENABLE_PROCESSED_DATA_LENGTH */

#if defined( FBL_MEM_ENABLE_REMAINDER_HANDLING )
      /* Everything consumed, program length already matches */
      if (requestLength == localLength)
      {
         /* Everything written, store new remaining data */
         gSegInfo.writeRemainder = remainder;
         /* Call copy function even if remainder length is zero */
         (void)MEMCPY(gRemainderBuffer, &programData[localLength], remainder);
      }
      /* Data partially programmed (most likely suspended by external event) */
      else if (localLength > 0u)
      {
         /* Store consumed length */
         *programLength = localLength - gSegInfo.writeRemainder;
         /* Some data written, no remainder */
         gSegInfo.writeRemainder = 0u;
      }
      else
      {
         /* Nothing consumed at all */
         *programLength = 0u;
         /* Nothing written at all, remainder from previous run unchanged */
      }
#else
      /* Store consumed length */
      *programLength = localLength;
#endif /* FBL_MEM_ENABLE_REMAINDER_HANDLING */
   }

   return retVal;
}/* PRQA S 6010 */ /* MD_FblMem_60xx */

#if defined( FBL_MEM_ENABLE_VERIFY_PIPELINED )
/***********************************************************************************************************************
 *  FblMemVerifyInput
 **********************************************************************************************************************/
/*! \brief      Execute input verification operation
 *  \details    Pass input data to given input verification routine
 *              No operation will be carried out if null pointer is passed as function
 *  \param[in]  routine Input verification operation to be executed
 *  \param[in]  data Verification data
 *                - FBL_MEM_VERIFY_STATE_INIT / FBL_MEM_VERIFY_STATE_FINALIZE: Usually irrelevant
 *                - FBL_MEM_VERIFY_STATE_COMPUTE: Data to be hashed
 *                - FBL_MEM_VERIFY_STATE_VERIFY: Reference checksum / signature
 *              state Sub-operation to be executed
 *  \param[in,out]   result Pointer to extended verification result
 *                      FBL_MEM_VERIFY_STATUS_NULL if neither relevant nor used
 *  \return     kFblMemStatus_Ok if operation successfully executed,
 *                 kFblMemStatus_Failed otherwise
 **********************************************************************************************************************/
static tFblMemStatus FblMemVerifyInput( V_MEMRAM1 tFblMemVerifyRoutineInput V_MEMRAM2 V_MEMRAM3 * routine,
   const V_MEMRAM1 tFblMemVerifyData V_MEMRAM2 V_MEMRAM3 * data, vuint8 state,
   V_MEMRAM1 tFblMemVerifyStatus V_MEMRAM2 V_MEMRAM3 * result )
{
   tFblMemStatus retVal;
   tFblMemVerifyStatus localResult;

   retVal      = kFblMemStatus_Ok;
   localResult = FBL_MEM_VERIFY_OK;

   /* Null pointer passed as verification function disables functionality */
   if (FBL_MEM_VERIFY_FCT_INPUT_NULL != routine->function)
   {
      routine->param->sigState          = state;
      routine->param->sigSourceBuffer   = data->data;
      routine->param->sigByteCount      = (tFblMemVerifyLength)data->length;
# if defined( FBL_MEM_ENABLE_EXT_TRIGGER_INPUT_VERIFY )
      routine->param->wdTriggerFct      = (tFblMemVerifyWdFct)FblMemTriggerWatchdogExt;
# else
      routine->param->wdTriggerFct      = (tFblMemVerifyWdFct)FblMemTriggerWatchdog;
# endif /* FBL_MEM_ENABLE_EXT_TRIGGER_INPUT_VERIFY */

      /* Call verification function and set extended status */
      localResult = routine->function(routine->param);
      if (FBL_MEM_VERIFY_OK != localResult)
      {
         retVal = kFblMemStatus_Failed;
      }
   }

   if (FBL_MEM_VERIFY_STATUS_NULL != result)
   {
      /* Pass extended result */
      *result = localResult;
   }

   return retVal;
}
#endif /* FBL_MEM_ENABLE_VERIFY_INPUT || FBL_MEM_ENABLE_VERIFY_PROCESSED || FBL_MEM_ENABLE_VERIFY_PIPELINED */

#if defined( FBL_MEM_ENABLE_VERIFY_PIPELINED )
/***********************************************************************************************************************
 *  FblMemPrepareVerifyPipeJob
 **********************************************************************************************************************/
/*! \brief      Get active pipelined verification job
 *  \details    If no previous job is pending for the requested segment, a new job is queued, using the provided address
 *              as starting point.
 *  \param[in]  segmentIndex Index of requested segment
 *  \param[in]  address Address of newly programmed data
 **********************************************************************************************************************/
static V_MEMRAM1 tFblMemJob V_MEMRAM2 V_MEMRAM3 * FblMemPrepareVerifyPipeJob( vuint32 segmentIndex, tFblAddress address )
{
   V_MEMRAM1 tFblMemJob V_MEMRAM2 V_MEMRAM3 * verifyJob;

   verifyJob = FBL_MEM_JOB_NULL;

   /* Null pointer passed as verification function disables functionality */
   if (FBL_MEM_VERIFY_FCT_INPUT_NULL != gBlockInfo.verifyRoutinePipe.function)
   {
      /* Check for error condition */
      if (kFblMemStatus_Ok == gErrorStatus)
      {
         /* Verification job for current segment not yet included in processing queue */
         if ( (FblMemQueueIsEmpty(gVerifyPipeQueue))
            || (FblMemQueueGetLastUsedEntry(gVerifyPipeQueue).job->segmentIndex != segmentIndex) )
         {
            /* Assert queue not full */
            assertFblInternal((!FblMemQueueIsFull(gVerifyPipeQueue)), kFblMemAssertParameterOutOfRange); /* PRQA S 2214 */ /* MD_FblDef_Assert */

            /* Get first free job */
            verifyJob = FblMemQueueGetFirstFreeEntry(gVerifyPipeQueue).job;

            /* Start verification at current address */
            verifyJob->baseAddress  = address;
            verifyJob->position     = 0u;
            verifyJob->netSize      = 0u;
            verifyJob->totalSize    = 0u;

            /* Add job to processing queue */
            (void)FblMemQueueDefaultPrioInsert(gProcessingQueue, verifyJob, segmentIndex);
            /* Append input info to separate queue, managing verification jobs exclusively */
            (void)FblMemQueueAppend(gVerifyPipeQueue);
         }

         /* Get active verification job */
         verifyJob = FblMemQueueGetLastUsedEntry(gVerifyPipeQueue).job;
      }
   }

   return verifyJob;
}

/***********************************************************************************************************************
 *  FblMemUpdateVerifyPipeJob
 **********************************************************************************************************************/
/*! \brief      Update job of pipelined verification
 *  \details    Add programmed data contents to queue, so it can be verified in parallel to data transmission.
 *              If no previous data is pending for the current segment, a new job is queued, using the provided address
 *              as starting point.
 *              The length is added to the last job in the queue.
 *  \param[in]  address Address of newly programmed data
 *  \param[in]  length Length of newly programmed data
 **********************************************************************************************************************/
static void FblMemUpdateVerifyPipeJob( tFblAddress address, tFblLength length )
{
   V_MEMRAM1 tFblMemJob V_MEMRAM2 V_MEMRAM3 * verifyJob;

   verifyJob = FblMemPrepareVerifyPipeJob(gSegInfo.ownIndex, address);

   /* Check for null pointer */
   if (FBL_MEM_JOB_NULL != verifyJob)
   {
      /* Update pipelined verification information */
      verifyJob->used      += length;
      verifyJob->netSize   += length;
      verifyJob->totalSize += length;
   }
}
#endif /* FBL_MEM_ENABLE_VERIFY_PIPELINED */

/***********************************************************************************************************************
 *  FblMemProcessJob
 **********************************************************************************************************************/
/*! \brief      Perform processing operations on provided job
 *  \details    Depending on the type and configuration one of the following operations will be carried out:
 *              - Update verification with buffer contents
 *              - Read already programmed data in temporary buffer for verification
 *              - Process buffer contents (e.g. decryption or decompression)
 *                Result placed in intermediate buffer, which is prepended to the processing queue
 *              - Program buffer contents
 *              - Flush program remainder
 *              - Pass data to other instance
 *              Buffer may be processed at once or split into smaller segments, requiring multiple call cycles to
 *              finish the buffer
 *              This depends on the configuration and finalization flag
 *  \pre        FblMemSegmentStartIndication executed before
 *  \param[in]  activeJob Information of processed buffer
 *  \param[in]  mode Operation mode used to trigger finalization
 *  \return     Result of operation (potentially remapped to OEM specific NRC)
 **********************************************************************************************************************/
static tFblMemStatus FblMemProcessJob( V_MEMRAM1 tFblMemJob V_MEMRAM2 V_MEMRAM3 * activeJob, tFblMemOperationMode mode )
{
   tFblLength     actualLength;
#if defined( FBL_MEM_ENABLE_VERIFY_PIPELINED )  || \
    defined( FBL_ENABLE_DATA_PROCESSING )       || \
    defined( FBL_MEM_ENABLE_STREAM_OUTPUT )     
   tFblMemRamData activeBuffer;
#endif /* FBL_MEM_ENABLE_VERIFY_INPUT || FBL_MEM_ENABLE_VERIFY_PIPELINED || FBL_ENABLE_DATA_PROCESSING || FBL_MEM_ENABLE_STREAM_OUTPUT || FBL_MEM_ENABLE_PASSTHROUGH */
#if defined( FBL_ENABLE_DATA_PROCESSING )
   V_MEMRAM1 tFblMemJob V_MEMRAM2 V_MEMRAM3 * procOutJob;
#endif /* FBL_ENABLE_DATA_PROCESSING */
#if defined( FBL_MEM_ENABLE_VERIFY_PIPELINED )
   tFblMemVerifyData verifyData;
#endif /* FBL_MEM_ENABLE_VERIFY_INPUT || FBL_MEM_ENABLE_VERIFY_PIPELINED */
#if defined( FBL_MEM_ENABLE_VERIFY_PIPELINED )
   tFblAddress   prevAddress;
   tFblLength    prevRemainder;
#endif /* FBL_MEM_ENABLE_VERIFY_PIPELINED */
#if defined( FBL_MEM_ENABLE_STREAM_OUTPUT )
   tFblResult streamResult;
#endif /* FBL_MEM_ENABLE_STREAM_OUTPUT */
#if defined( FBL_MEM_ENABLE_GAP_FILL )
   tFblAddress fillAddress;
   tFblLength  fillLength;
   tFblLength  tempLength;
#endif /* FBL_MEM_ENABLE_GAP_FILL */
   tFblMemOperationMode finalizeStream;

#if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   /* Parameters not used: avoid compiler warning */
# if defined( FBL_MEM_ENABLE_SEGMENTATION )
# else
   (void)mode; /* PRQA S 3112 */ /* MD_FblMem_3112 */
# endif /* FBL_MEM_ENABLE_SEGMENTATION */
#endif /* V_ENABLE_USE_DUMMY_STATEMENT */

   /* Handle watchdog trigger */
   FblMemTriggerWatchdog();

#if defined( FBL_MEM_ENABLE_VERIFY_PIPELINED ) || \
    defined( FBL_ENABLE_DATA_PROCESSING )      || \
    defined( FBL_MEM_ENABLE_STREAM_OUTPUT )    
   /* Get pointer to current buffer position */
   activeBuffer   = FblMemGetBuffer(activeJob);
#endif /* FBL_MEM_ENABLE_VERIFY_INPUT || FBL_MEM_ENABLE_VERIFY_PIPELINED || FBL_ENABLE_DATA_PROCESSING || FBL_MEM_ENABLE_STREAM_OUTPUT || FBL_MEM_ENABLE_PASSTHROUGH */
   /* Limit processed length to configured value */
   actualLength = FblMemLimitLength(activeJob->used, activeJob->type, mode);

   /* Handle buffer types */
   switch (activeJob->type)
   {
#if defined( FBL_MEM_ENABLE_VERIFY_PIPELINED )
      /* Pipelined verification */
# if defined( FBL_MEM_ENABLE_VERIFY_ADDRESS_LENGTH )
      case kFblMemJobType_VerifyPipeInfo:
      {
         /* Pass buffer to signature calculation */
         verifyData.data   = activeBuffer;
         verifyData.length = actualLength;

         /* Add current data to the signature */
         if (kFblMemStatus_Ok != FblMemVerifyInput( &gBlockInfo.verifyRoutinePipe, &verifyData,
                                                    FBL_MEM_VERIFY_STATE_COMPUTE, FBL_MEM_VERIFY_STATUS_NULL ))
         {
            /* Compute operation failed */
            FBL_MEM_SET_STATUS(VerifyCompute, gErrorStatus);   /* PRQA S 3109 */ /* MD_CBD_14.3 */
         }

         /* Switch job type to read back programmed data */
         activeJob->type   = kFblMemJobType_VerifyPipeRead;
         /* Prevent modification of position value used by read back operation */
         activeJob->used   -= actualLength;
         actualLength      = 0u;

         break;
      }
# endif /* FBL_MEM_ENABLE_VERIFY_ADDRESS_LENGTH */
      case kFblMemJobType_VerifyPipeRead:
      {
# if defined( FBL_MEM_ENABLE_VERIFY_ADDRESS_LENGTH )
         /* Reset to default completion mode, as address information operates in explicit finalization mode */
         activeJob->completion = kFblMemOperationMode_Normal;
# endif /* FBL_MEM_ENABLE_VERIFY_ADDRESS_LENGTH */

         /* Read back a chunk of the already programmed data for verification */
# if defined( FBL_MEM_ENABLE_SWITCH_READMEMORY_PARAM )
         /* Parameters order changed in comparison to HIS security module specification */
         if (gBlockInfo.readFct(activeJob->baseAddress + activeJob->position, actualLength, gVerifyPipeUpdateJob.buffer) != actualLength)
# else
         /* Parameters order as defined by HIS security module specification */
         if (gBlockInfo.readFct(activeJob->baseAddress + activeJob->position, gVerifyPipeUpdateJob.buffer, actualLength) != actualLength)
# endif /* FBL_MEM_ENABLE_SWITCH_READMEMORY_PARAM */
         {
            /* Read operation failed */
            FBL_MEM_SET_STATUS(VerifyCompute, gErrorStatus);   /* PRQA S 3109 */ /* MD_CBD_14.3 */
         }
         else
         {
            /* Data successfully read, pass to update routine */
            gVerifyPipeUpdateJob.position = 0u;
            gVerifyPipeUpdateJob.used     = actualLength;

            /* Queue updated job */
            (void)FblMemQueueDefaultPrioInsert(gProcessingQueue, &gVerifyPipeUpdateJob, activeJob->segmentIndex);
         }

         break;
      }
      case kFblMemJobType_VerifyPipeUpdate:
      {
         /* Pass buffer to signature calculation */
         verifyData.data   = activeBuffer;
         verifyData.length = actualLength;

         /* Add current data to the signature */
         if (kFblMemStatus_Ok != FblMemVerifyInput( &gBlockInfo.verifyRoutinePipe, &verifyData,
                                                    FBL_MEM_VERIFY_STATE_COMPUTE, FBL_MEM_VERIFY_STATUS_NULL ))
         {
            /* Compute operation failed */
            FBL_MEM_SET_STATUS(VerifyCompute, gErrorStatus);   /* PRQA S 3109 */ /* MD_CBD_14.3 */
         }

         break;
      }
      case kFblMemJobType_VerifyPipeFinalize:
      {
         /* Not used by finalization */
         verifyData.data   = activeBuffer;
         verifyData.length = actualLength;

         /* Finish the calculation */
         if (kFblMemStatus_Ok != FblMemVerifyInput( &gBlockInfo.verifyRoutinePipe, &verifyData,
                                                    FBL_MEM_VERIFY_STATE_FINALIZE, FBL_MEM_VERIFY_STATUS_NULL ))
         {
            /* Verify finalization failed */
            FBL_MEM_SET_STATUS(VerifyFinalize, gErrorStatus);   /* PRQA S 3109 */ /* MD_CBD_14.3 */
         }

         break;
      }
#endif /* FBL_MEM_ENABLE_VERIFY_PIPELINED */
#if defined( FBL_ENABLE_DATA_PROCESSING )
      /* Data processing */
      case kFblMemJobType_ProcInput:
      case kFblMemJobType_ProcFinalize:
      {
# if defined( FBL_MEM_ENABLE_STREAM_OUTPUT )
         if (kFblOk == __ApplFblMemIsStreamOutputRequired(gSegInfo.input.dataFormat))
         {
            procOutJob  = &gStreamProcJob;
         }
         else
# endif /* FBL_MEM_ENABLE_STREAM_OUTPUT */
         {
            procOutJob  = &gProcWriteJob;
         }

         /* Update position of output buffer to accommodate for programming remainder */
         procOutJob->position = gSegInfo.writeRemainder;

         /*
            Initialize parameters for data processing.
            Note: Other members have been set during initialization
         */
         gProcParam.dataBuffer      = activeBuffer;
         gProcParam.dataLength      = (vuint16)actualLength;
         /* Align output buffer according to current fill level */
         gProcParam.dataOutBuffer   = &((FblMemGetBuffer(procOutJob))[procOutJob->used]);
         gProcParam.dataOutLength   = 0u;

         /* Call API function for user specific data processing */
         if (kFblOk != ApplFblDataProcessing(&gProcParam))
         {
            FBL_MEM_SET_STATUS(DataProc, gErrorStatus);   /* PRQA S 3109 */ /* MD_CBD_14.3 */
         }
         else
         {
            if ((gProcParam.dataOutLength | gProcParam.dataLength) == 0u)
            {
               if (kFblMemJobType_ProcFinalize == activeJob->type)
               {
                  /* Finalize data processing
                     gProcWriteJob already has correct values */
                  if (kFblOk != ApplFblDeinitDataProcessing(&gProcParam))
                  {
                     FBL_MEM_SET_STATUS(DataProcDeinit, gErrorStatus); /* PRQA S 3109 */ /* MD_CBD_14.3 */
                  }

                  /* Finish job processing */
                  activeJob->completion = kFblMemOperationMode_Unconditional;
               }
               else
               {
                  /* Nothing consumed or produced at all, avoid endless loop */
                  FBL_MEM_SET_STATUS(DataProcConsume, gErrorStatus);  /* PRQA S 3109 */ /* MD_CBD_14.3 */
               }
            }
         }

         if (kFblMemStatus_Ok == gErrorStatus)
         {
            /* Update consumed length */
            actualLength = gProcParam.dataLength;

            /* Any data produced? */
            if (gProcParam.dataOutLength > 0u)
            {
               /* Output buffer changed? */
               assertFblUser((gProcParam.dataOutBuffer == &((FblMemGetBuffer(procOutJob))[procOutJob->used])), kFblMemAssertUserResultOutOfRange); /* PRQA S 2214 */ /* MD_FblDef_Assert */
               /* Output length exceeds configured limit? */
               assertFblUser((gProcParam.dataOutLength <= FBL_MEM_INTERNAL_PROC_SEGMENTATION), kFblMemAssertUserResultOutOfRange); /* PRQA S 2214 */ /* MD_FblDef_Assert */

               /* Store processed data information */
               procOutJob->used += gProcParam.dataOutLength;

               /* Total used length exceeds buffer limits? */
               assertFblInternal((procOutJob->used <= (procOutJob->totalSize - procOutJob->offset)), kFblMemAssertParameterOutOfRange); /* PRQA S 2214 */ /* MD_FblDef_Assert */

# if defined( FBL_MEM_ENABLE_PROC_SEGMENTATION )
               /* Accumulate multiple data processing calls before passing output data to write routine */

               /* Buffer was previously empty
                  Add behind current input data using low prio so that data will definitely be programmed after processing */
               if (procOutJob->used == gProcParam.dataOutLength)
               {
                  /* Insert processed data into the queue and remember handle */
                  gProcHandle = FblMemQueueDefaultPrioInsert(gProcessingQueue, procOutJob, activeJob->segmentIndex);
               }

               /* Buffer is (almost) filled and has to be programmed before continuing processing */
               if ((procOutJob->netSize - procOutJob->used) < FBL_MEM_INTERNAL_PROC_SEGMENTATION)
               {
                  /* Move processed data buffer in front of input data job by updating to higher priority
                     Handle set during current or previous cycle */
                  (void)FblMemQueuePrioUpdate(gProcessingQueue, gProcHandle, kFblMemJobPrio_ProcWriteHigh);
               }
# else
               /* Insert processed data into the queue */
               (void)FblMemQueueDefaultPrioInsert(gProcessingQueue, procOutJob, activeJob->segmentIndex);
# endif /* FBL_MEM_ENABLE_PROC_SEGMENTATION */
            }

         }

         break;
      }
      case kFblMemJobType_ProcWrite:
#endif /* FBL_ENABLE_DATA_PROCESSING */
#if defined( FBL_MEM_ENABLE_REMAINDER_HANDLING )
      case kFblMemJobType_WriteFinalize:
#endif /* FBL_MEM_ENABLE_REMAINDER_HANDLING */
      case kFblMemJobType_InputWrite:
      {
#if defined( FBL_MEM_ENABLE_VERIFY_PIPELINED )
         /* Remember current address and remainder length */
         prevAddress   = gSegInfo.writeAddress;
         prevRemainder = gSegInfo.writeRemainder;
#endif /* FBL_MEM_ENABLE_VERIFY_PIPELINED */

#if defined( FBL_MEM_ENABLE_REMAINDER_HANDLING )
         if (kFblMemJobType_WriteFinalize == activeJob->type)
         {
            finalizeStream       = kFblMemOperationMode_Finalize;
            /* Update position of output buffer to accommodate for programming remainder */
            activeJob->position  = gSegInfo.writeRemainder;
         }
# if defined( FBL_ENABLE_UNALIGNED_DATA_TRANSFER )
# else
         else if (kFblMemJobType_InputWrite == activeJob->type)
         {
            /* Force programming of any potential remainder */
            finalizeStream = kFblMemOperationMode_Finalize;
         }
# endif /* FBL_ENABLE_UNALIGNED_DATA_TRANSFER */
         else
#endif /* FBL_MEM_ENABLE_REMAINDER_HANDLING */
         {
            finalizeStream = kFblMemOperationMode_Normal;
         }

         /* Program data, error code reflects result of operation */
         gErrorStatus = FblMemProgramStream(activeJob, &actualLength, finalizeStream);

#if defined( FBL_MEM_ENABLE_PROGRESS_INFO )
         /* Update progress information */
         if (kFblMemStatus_Ok == gErrorStatus)
         {
#if defined( FBL_MEM_ENABLE_GAP_FILL )
            FblMemUpdateProgress(gSegInfo.writeLength + gGapFillJob.used);
#else
            FblMemUpdateProgress(gSegInfo.writeLength);
#endif
         }
#endif /* FBL_MEM_ENABLE_PROGRESS_INFO*/

#if defined( FBL_MEM_ENABLE_VERIFY_PIPELINED )
         /* Update pipelined verification information
            Start verification at current write address
            Include currently written data without possible remainder */
         FblMemUpdateVerifyPipeJob(prevAddress, (actualLength + prevRemainder) - gSegInfo.writeRemainder);
#endif /* FBL_MEM_ENABLE_VERIFY_PIPELINED */

         break;
      }
#if defined( FBL_MEM_ENABLE_STREAM_OUTPUT )
      case kFblMemJobType_StreamInput:
      case kFblMemJobType_StreamProc:
      case kFblMemJobType_StreamFinalize:
      {
         /*
            Initialize parameters for stream output.
            Note: Other members have been set during initialization
         */
         gStreamParam.inputData        = activeBuffer;
         gStreamParam.inputLength      = actualLength;
         gStreamParam.consumedLength   = 0u;
         gStreamParam.producedLength   = 0u;
         gStreamParam.address          = gSegInfo.writeAddress;
         gStreamParam.length           = gSegInfo.writeLength;

         if (kFblMemJobType_StreamFinalize == activeJob->type)
         {
            /* Call API function for user specific stream output */
            streamResult = ApplFblFinalizeStreamOutput(&gStreamParam);
         }
         else
         {
            /* Call API function for user specific stream output */
            streamResult = ApplFblStreamOutput(&gStreamParam);
         }

         if (kFblOk != streamResult)
         {
            FBL_MEM_SET_STATUS(StreamOutput, gErrorStatus);   /* PRQA S 3109 */ /* MD_CBD_14.3 */
         }
         else
         {
            if ((gStreamParam.consumedLength | gStreamParam.producedLength) == 0u)
            {
               if (kFblMemJobType_StreamFinalize == activeJob->type)
               {
                  /* Finalize stream output
                     Parameter already has correct values */
                  if (kFblOk != ApplFblDeinitStreamOutput(&gStreamParam))
                  {
                     FBL_MEM_SET_STATUS(StreamOutputDeinit, gErrorStatus); /* PRQA S 3109 */ /* MD_CBD_14.3 */
                  }

                  /* Finish job processing */
                  activeJob->completion = kFblMemOperationMode_Unconditional;
               }
               else
               {
                  /* Nothing consumed or produced at all, avoid endless loop */
                  FBL_MEM_SET_STATUS(StreamOutputConsume, gErrorStatus);  /* PRQA S 3109 */ /* MD_CBD_14.3 */
               }
            }
         }

         if (kFblMemStatus_Ok == gErrorStatus)
         {
            /* Update consumed length */
            actualLength = gStreamParam.consumedLength;

            /* Any data produced? */
            if (gStreamParam.producedLength > 0u)
            {
               /* Verify complete programming length does not exceed previously requested memory range */
               if (gStreamParam.producedLength > gSegInfo.writeLength)
               {
                  FBL_MEM_SET_STATUS(StreamOutputConsume, gErrorStatus); /* PRQA S 3109 */ /* MD_CBD_14.3 */
               }
               else
               {
#if defined( FBL_MEM_ENABLE_VERIFY_PIPELINED )
                  /* Update pipelined verification information */
                  FblMemUpdateVerifyPipeJob(gSegInfo.writeAddress, gStreamParam.producedLength);
#endif /* FBL_MEM_ENABLE_VERIFY_PIPELINED */

                  /* Update segment information */
                  gSegInfo.writeAddress   += gStreamParam.producedLength;
                  /* Reduce expected remainder */
                  gSegInfo.writeLength    -= gStreamParam.producedLength;
#if defined( FBL_ENABLE_PROCESSED_DATA_LENGTH )
                  /* Length actually written */
                  gSegInfo.writtenLength  += gStreamParam.producedLength;
#endif /* FBL_ENABLE_PROCESSED_DATA_LENGTH */
               }
            }

         }

         break;
      }
#endif /* FBL_MEM_ENABLE_STREAM_OUTPUT */
#if defined( FBL_MEM_ENABLE_GAP_FILL )
      case kFblMemJobType_GapFill:
      {
         /* Start address of current fill operation */
         fillAddress    = activeJob->baseAddress + activeJob->position;
         /* Remember total length */
         fillLength     = actualLength;
         /* Reset length actually filled */
         actualLength   = 0u;

         /* Find start segment for current address */
         memSegment = FblMemSegmentNrGet(fillAddress);

         /* Valid segment addressed? */
         if (memSegment < 0)
         {
            /* Address lies in gap between flash blocks
               Range till next block can be skipped */

            /* Use last filled address to get follow-up segment */
            memSegment = FblMemSegmentNrGet(fillAddress - 1u) + 1;

            /* Check segment range */
            if ((memSegment >= kNrOfFlashBlock) || (memSegment <= 0))
            {
               FBL_MEM_SET_STATUS(ProgramOutsideFbt, gErrorStatus);  /* PRQA S 3109 */ /* MD_CBD_14.3 */
            }
            else
            {
               /* Skip gap */
               if (FlashBlock[memSegment].begin > fillAddress)
               {
                  /* Distance to start of next flash block can be skipped */
                  tempLength = FlashBlock[memSegment].begin - fillAddress;

                  if (tempLength < fillLength)
                  {
                     /* Range partially covers next flash block
                        Adapt fill range accordingly */
                     actualLength = tempLength;
                     fillLength  -= actualLength;
                     fillAddress  = FlashBlock[memSegment].begin;
                  }
                  else
                  {
                     /* Full range lies within gap and can be skipped completely */
                     actualLength   = fillLength;
                     fillLength     = 0u;
                  }
               }
               else
               {
                  /* Full range lies within gap and can be skipped completely */
                  actualLength   = fillLength;
                  fillLength     = 0u;
               }
            }
         }

         if (kFblMemStatus_Ok == gErrorStatus)
         {
            /* Limit fill operation to configured segmentation (equals size of prepared buffer) */
            if (fillLength > FBL_MEM_GAP_FILL_SEGMENTATION)
            {
               fillLength = FBL_MEM_GAP_FILL_SEGMENTATION;
            }

            /* Distance to end of current flash block */
            tempLength =(FlashBlock[memSegment].end - fillAddress) + 1u;
            /* Limit fill range to current flash block */
            if (fillLength > tempLength)
            {
               fillLength = tempLength;
            }

            /* Perform actual fill operation */
            if (fillLength > 0u)
            {
               /* Program non-volatile memory */
               gErrorStatus = FblMemProgramBufferInternal(fillAddress, &fillLength, gGapFillBuffer.data, kFblMemProgState_Pending);

               /* Update length actually processed */
               actualLength += fillLength;
            }
         }

#if defined( FBL_MEM_ENABLE_PROGRESS_INFO )
         if (kFblMemStatus_Ok == gErrorStatus)
         {
            /* Update programming progress */
            FblMemUpdateProgress(gSegInfo.writeLength + (gGapFillJob.used - actualLength));
         }
#endif /* FBL_MEM_ENABLE_PROGRESS_INFO*/

         break;
      }
#endif /* FBL_MEM_ENABLE_GAP_FILL */
      case kFblMemJobType_Max:
      default:
      {
         /* Invalid buffer type */
         FBL_MEM_SET_STATUS(Failed, gErrorStatus);  /* PRQA S 3109 */ /* MD_CBD_14.3 */

         break;
      }
   }

   /* Update buffer usage */
   activeJob->position  += actualLength;
   activeJob->used      -= actualLength;

#if defined( FBL_ENABLE_SYSTEM_CHECK )
   if (kFblMemStatus_Ok == gErrorStatus)
   {
      if (kFblMemStatus_Ok != FblMemVerifyBufferIntegrity())
      {
         /* Detected buffer corruption */
         FBL_MEM_SET_STATUS(Failed, gErrorStatus);  /* PRQA S 3109 */ /* MD_CBD_14.3 */
      }
   }
#endif /* FBL_ENABLE_SYSTEM_CHECK */

   /* Return global error state as function may be called in both service and background context */
   return gErrorStatus;
} /* PRQA S 6030 */ /* PRQA S 6050 */ /* MD_FblMem_60xx */ /* PRQA S 4700 */ /* MD_FblMem_4700 */

/***********************************************************************************************************************
 *  FblMemProcessQueue
 **********************************************************************************************************************/
/*! \brief      Process first job in processing queue (directly use single input job if un-queued)
 *  \details    Remove finished job from processing queue (and pipelined queue in case of input job)
 *              Switch to idle mode if all pending jobs are finished
 *  \pre        FblMemQueueBuffer executed before
 *  \param[in]  mode Operation mode used to trigger finalization
 **********************************************************************************************************************/
static void FblMemProcessQueue( tFblMemOperationMode mode )
{
#if defined( FBL_MEM_ENABLE_PROC_QUEUE )
   tFblMemQueueHandle activeHandle;
#endif /* FBL_MEM_ENABLE_PROC_QUEUE */
#if defined( FBL_MEM_ENABLE_PIPELINING )
   V_MEMRAM1 tFblMemQueueEntry V_MEMRAM2 V_MEMRAM3 * removeQueue;
#endif /* FBL_MEM_ENABLE_PIPELINING */
   V_MEMRAM1 tFblMemJob V_MEMRAM2 V_MEMRAM3 * activeJob;

   FblMemTriggerWatchdog();

   /*
      Critical section secures access to programming state
      May also be altered in interrupt context
   */
   __ApplFblMemEnterCriticalSection();

   /*
      Module in idle or error state?
      Condition equals ((kFblMemProgState_Idle == fblMemProgState) || (kFblMemProgState_Error == fblMemProgState))
   */
   if (fblMemProgState < kFblMemProgState_Suspended)
   {
      /* Nothing to do */
   }
#if defined( FBL_MEM_ENABLE_PIPELINING )
   /* Only explicit pending state allowed while programming in background context */
   else if ( (kFblMemContext_Background == gProgContext)
          && (kFblMemProgState_Pending != fblMemProgState) )
   {
      /* Module suspended */
   }
#endif /* FBL_MEM_ENABLE_PIPELINING */
   else
   {
      /* Operation potentially paused during previous execution cycle */
      fblMemProgState = kFblMemProgState_Pending;
   }

   /*
      No further critical access to programming state
      Critical section can be left
   */
   __ApplFblMemLeaveCriticalSection();

   if (kFblMemProgState_Pending == fblMemProgState)
   {
#if defined( FBL_MEM_ENABLE_PIPELINING )
      removeQueue       = FBL_MEM_QUEUE_NULL;
#endif /* FBL_MEM_ENABLE_PIPELINING */
#if defined( FBL_MEM_ENABLE_PROC_QUEUE )
      /* Set active job, stored in first entry of processing queue */
      activeHandle      = FblMemQueueGetFirstUsedHandle(gProcessingQueue);
      activeJob         = gProcessingQueue[activeHandle].job;
#else
      /* Set active job to single input job */
      activeJob         = &FBL_MEM_INPUT_JOB[0];
#endif /* FBL_MEM_ENABLE_PROC_QUEUE */

      /* Perform processing cycle on active job */
      if (kFblMemStatus_Ok == FblMemProcessJob(activeJob, mode))
      {
         /* Buffer completely processed and completion mode allows job to be cleared? */
         if (  (kFblMemOperationMode_Unconditional == activeJob->completion)
            || ((kFblMemOperationMode_Normal == activeJob->completion) && (0u == activeJob->used)) )
         {
#if defined( FBL_MEM_ENABLE_PROC_QUEUE )
            /* Remove buffer from processing queue */
            (void)FblMemQueueRemove(gProcessingQueue, activeHandle);

            if (FblMemQueueIsEmpty(gProcessingQueue))
#endif /* FBL_MEM_ENABLE_PROC_QUEUE */
            {
               /* Processing queue empty: switch to idle state */
               fblMemProgState = kFblMemProgState_Idle;
            }

#if defined( FBL_MEM_ENABLE_PIPELINING )
            /* Update additional queues */
            switch (activeJob->jobClass)
            {
# if defined( FBL_MEM_ENABLE_VERIFY_PIPELINED )
               case tFblMemJobClass_VerifyPipe:
               {
                  /* Remove from verification job queue */
                  removeQueue = gVerifyPipeQueue;

                  break;
               }
# endif
               /* Default class: no special action required */
               case tFblMemJobClass_Default:
               /* Ignore unknown job class */
               default:
               {
                  break;
               }
            }
#endif /* FBL_MEM_ENABLE_PIPELINING */
         }
      }
      else
      {
         /* Error while processing buffer: switch to error state */
         fblMemProgState = kFblMemProgState_Error;

      }

#if defined( FBL_MEM_ENABLE_PIPELINING )
      /*
         Remove from input buffer queue
         Buffer available for new data again
      */
      if ( (FBL_MEM_QUEUE_NULL != removeQueue)
        && (!FblMemQueueIsEmpty(removeQueue)) )
      {
         (void)FblMemQueueRemove(removeQueue, FblMemQueueGetFirstUsedHandle(removeQueue));
      }
#endif /* FBL_MEM_ENABLE_PIPELINING */
   }
} /* PRQA S 6080 */ /* MD_FblMem_60xx */ /* PRQA S 2006 */ /* MD_CBD_14.7 */

/***********************************************************************************************************************
 *  FblMemFlushQueueByPrio
 **********************************************************************************************************************/
/*! \brief      Process all jobs in processing queue with priority greater or equal to given value
 *  \details    Loop until processing queue is empty or a job with a priority lower to the given one is reached
 *  \pre        FblMemQueueBuffer executed before
 *  \param[in]  prio Lowest job priority to be processed
 **********************************************************************************************************************/
static void FblMemFlushQueueByPrio( tFblMemQueuePrio prio )
{
#if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   /* Parameters not used: avoid compiler warning */
# if defined( FBL_MEM_ENABLE_PROC_QUEUE )
# else
   (void)prio; /* PRQA S 3112 */ /* MD_FblMem_3112 */
# endif /* FBL_MEM_ENABLE_PROC_QUEUE */
#endif /* V_ENABLE_USE_DUMMY_STATEMENT */

   /* Loop while jobs are pending */
   while ( (FblMemTaskIsPending())
#if defined( FBL_MEM_ENABLE_PROC_QUEUE )
        /* Only jobs with priority greater or equal to given one are processed */
        && (FblMemQueueGetFirstUsedEntry(gProcessingQueue).prio >= prio)
#endif
         )
   {
      /* Execute processing cycle */
      FblMemProcessQueue(kFblMemOperationMode_Finalize);
   }
}

#if defined( FBL_MEM_ENABLE_PIPELINING )
/***********************************************************************************************************************
 *  FblMemUnblockQueue
 **********************************************************************************************************************/
/*! \brief      Unblock queue by freeing at least one queue entry
 *  \details    Loop until given queue isn't completely filled any more
 *  \pre        FblMemQueueBuffer executed before
 *  \param[in]  queue Pointer to queue which should be unblocked
 **********************************************************************************************************************/
static void FblMemUnblockQueue( const V_MEMRAM1 tFblMemQueueEntry V_MEMRAM2 V_MEMRAM3 * queue )
{
   /* Loop while queue is completely filled and jobs are pending */
   while ( (FblMemQueueIsFull(queue)) && (FblMemTaskIsPending()) )
   {
      /* Execute processing cycle */
      FblMemProcessQueue(kFblMemOperationMode_Normal);
   }
}
#endif /* FBL_MEM_ENABLE_PIPELINING */

/***********************************************************************************************************************
 *  FblMemQueueBuffer
 **********************************************************************************************************************/
/*! \brief      Handle new chunk of input data
 *  \details    If any combination of verification, data processing and pipelined programming is configured add jobs to
 *              processing queue
 *              Otherwise processing will directly work on single input buffer
 *              If pipelined programming is configured queue buffer and finish processing of one pending input buffer
 *              in case no free input buffer is available
 *  \pre        FblMemInitPowerOn executed before, provided buffer and data offset equal the parameters of active
 *              input buffer
 *  \param[in]  buffer Pointer to input buffer (including preamble)
 *  \param[in]  offset Offset of actual data (after preamble)
 *  \param[in]  length Length of data (without preamble)
 *  \return     Result of operation (potentially remapped to OEM specific NRC)
 **********************************************************************************************************************/
static tFblMemStatus FblMemQueueBuffer( tFblMemConstRamData buffer, tFblLength offset, tFblLength length )
{
   tFblMemStatus  retVal;
   V_MEMRAM1 tFblMemJob V_MEMRAM2 V_MEMRAM3 * activeJob;

#if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   /* Parameters not used: avoid compiler warning */
# if defined( FBL_MEM_ENABLE_SEGMENTED_INPUT_BUFFER ) || \
     defined( FBL_ENABLE_SYSTEM_CHECK )               || \
     defined( FBL_ENABLE_ASSERTION )
# else
   (void)buffer; /* PRQA S 3112 */ /* MD_FblMem_3112 */
# endif /* FBL_MEM_ENABLE_SEGMENTED_INPUT_BUFFER || FBL_ENABLE_SYSTEM_CHECK || FBL_ENABLE_ASSERTION */
#endif /* V_ENABLE_USE_DUMMY_STATEMENT */

   /* Received data resides in active fill buffer */
   activeJob = FblMemGetPendingInputJob();

#if defined( FBL_MEM_ENABLE_SEGMENTED_INPUT_BUFFER ) || \
    defined( FBL_ENABLE_SYSTEM_CHECK )
# if defined( FBL_MEM_ENABLE_SEGMENTED_INPUT_BUFFER )
   if (kFblMemStatus_Ok != FblMemRelocateBufferOffset(activeJob, buffer, offset, length))
# else /* FBL_ENABLE_SYSTEM_CHECK */
   /* Limit buffer handling to previously provided buffer */
   if ( (buffer != FblMemGetBuffer(activeJob))
     || (offset >= activeJob->netSize)
     || (length > (activeJob->netSize - offset)) )
# endif /* FBL_MEM_ENABLE_SEGMENTED_INPUT_BUFFER */
   {
      FBL_MEM_SET_STATUS(DataIndParam, retVal); /* PRQA S 3109 */ /* MD_CBD_14.3 */
   }
   else
#else
   assertFblUser(buffer == FblMemGetBuffer(activeJob),      kFblMemAssertParameterOutOfRange); /* PRQA S 2214 */ /* MD_FblDef_Assert */
   assertFblUser(offset <  activeJob->netSize,              kFblMemAssertParameterOutOfRange); /* PRQA S 2214 */ /* MD_FblDef_Assert */
   assertFblUser(length <= (activeJob->netSize - offset),   kFblMemAssertParameterOutOfRange); /* PRQA S 2214 */ /* MD_FblDef_Assert */
#endif /* FBL_MEM_ENABLE_SEGMENTED_INPUT_BUFFER || FBL_ENABLE_SYSTEM_CHECK */
   {

      /* Evaluate current error state and directly return if not okay (error in background task) */
      if (kFblMemProgState_Error != fblMemProgState)
      {
         /* Update buffer state with provided information */
         activeJob->position  = activeJob->offset + offset;
         activeJob->used      = length;
#if defined( FBL_ENABLE_DATA_PROCESSING )    || \
    defined( FBL_MEM_ENABLE_STREAM_OUTPUT )  || \
    defined( FBL_MEM_ENABLE_REMAINDER_HANDLING )
         /*
            Switch type of input buffer to value set in segment indication:
            write through, pass through, data processing or stream output
         */
         activeJob->type      = gSegInfo.jobType;
#endif /* FBL_ENABLE_DATA_PROCESSING || FBL_MEM_ENABLE_STREAM_OUTPUT || FBL_MEM_ENABLE_PASSTHROUGH || FBL_MEM_ENABLE_REMAINDER_HANDLING */

         {
#if defined( FBL_MEM_ENABLE_VERIFY_PIPELINED )
            /* Processing queue full */
            if (FblMemQueueIsFull(gVerifyPipeQueue))
            {
# if defined( FBL_MEM_ENABLE_VERIFY_PIPELINED_ADDRESS_LENGTH )
               /* Last entry is exclusively used for storage of address information
                  This reserved job has to be free at all times so no blocking operation is necessary during segment start */
# else
               /* Verification job for current segment not yet included */
               if (FblMemQueueGetLastUsedEntry(gVerifyPipeQueue).job->segmentIndex != gSegInfo.ownIndex)
# endif /* FBL_MEM_ENABLE_VERIFY_PIPELINED_ADDRESS_LENGTH */
               {
                  /* Free at least one queue entry */
                  FblMemUnblockQueue(gVerifyPipeQueue);
               }
            }
#endif /* FBL_MEM_ENABLE_VERIFY_PIPELINED */
         }

#if defined( FBL_MEM_ENABLE_PROC_QUEUE )
# if defined( FBL_MEM_ENABLE_VERIFY_PIPELINED )
         /* Check for error condition */
         if (kFblMemProgState_Error != fblMemProgState)
# endif /* FBL_MEM_ENABLE_VERIFY_PIPELINED */
         {
            /* Append buffer to processing queue */
            (void)FblMemQueueDefaultPrioInsert(gProcessingQueue, activeJob, gSegInfo.ownIndex);

         }
#endif /* FBL_MEM_ENABLE_PROC_QUEUE */

#if defined( FBL_MEM_ENABLE_PIPELINING )
         /* Check for error condition */
         if (kFblMemProgState_Error != fblMemProgState)
#endif /* FBL_MEM_ENABLE_PIPELINING */
         {
            /* Set pending programming request flag */
            fblMemProgState = kFblMemProgState_Pending;
         }
      }

      /* Result depending on queue processing by FblMemUnblockQueue */
      retVal = gErrorStatus;
   }

   return retVal;
}

/***********************************************************************************************************************
 *  FblMemCheckAllowed
 **********************************************************************************************************************/
/*! \brief      Check if all required states are set
 *  \details    Clear certain states if successful, completely reset allowed states otherwise
 *  \pre        Allowed state initialized before
 *  \param[in]  check Bitmask of states which have to be active
 *  \param[in]  clear Bitmask of states to be cleared in case check successful
 *  \return     All required states set (kFblOk) or not (kFblFailed)
 **********************************************************************************************************************/
static tFblResult FblMemCheckAllowed( tFblMemAllowedInd check, tFblMemAllowedInd clear )
{
   tFblResult retVal;

   /*
      Check executed at beginning of (almost) every exported function
      Trigger watchdog here
    */
   FblMemTriggerWatchdog();

   /* Check for required states */
   if (FblMemIsAllowed(check))
   {
      /* Success: clear requested states */
      FblMemClrAllowed(clear);
      retVal = kFblOk;
   }
   else
   {
      /* Failure: completely reset states */
      FblMemResetAllowed();
      retVal = kFblFailed;
   }

   return retVal;
}

#if defined( FBL_MEM_ENABLE_PROGRESS_INFO )
/***********************************************************************************************************************
 *  FblMemCalculateProgress
 **********************************************************************************************************************/
/*! \brief      Calculate progress percentage
 *  \details    Percentage is calculated according to the current progress, the expected maximum value and the resulting
 *              maximum percentage
 *  \param[in]  current Current progress in arbitrary unit (e.g. bytes)
 *  \param[in]  total Expected maximum value in arbitrary unit (e.g. bytes)
 *  \param[in]  percentage Maximum reported percentage
 *  \return     Current progress percentage
 **********************************************************************************************************************/
vuint8 FblMemCalculateProgress( vuint32 current, vuint32 total, vuint8 percentage )
{
   vuint8 progress;

   /* Init to maximum percentage */
   progress = percentage;

   /* Prevent division by zero */
   if ((0u != total) && (0u != percentage))
   {
      /* Calculate progress value
         Use different calculation paths depending on total value to prevent integer overflow or division by zero
         Remark: Unsigned cast of -1 is guaranteed to result in maximum representable value */
      if (total < (((vuint32)-1) / (vuint32)percentage))
      {
         /* Applying multiplication to current value prevents loss of precision,
            which otherwise could result in invalid percentages (e.g. 101%) */
         progress = (vuint8)(((current * percentage) / total) & 0xFFu);
      }
      else
      {
         /* Total value is large enough to provide sufficient precision */
         progress = (vuint8)((current / (total / percentage)) & 0xFFu);
      }
   }

   /* Limit reported progress to maximum percentage */
   if (progress > percentage)
   {
      progress = percentage;
   }

   return progress;
}

/***********************************************************************************************************************
 *  FblMemInitProgress
 **********************************************************************************************************************/
/*! \brief      Initialize progress states
 *  \details    Called during startup
 **********************************************************************************************************************/
void FblMemInitProgress( void )
{
   /* Initialize progress information */
   gProgressInfo.type                  = kFblMemProgressType_Undefined;

   /* Ensure reporting is triggered after change of any value */
   gPrevProgressInfo.type              = kFblMemProgressType_Undefined;
   gPrevProgressInfo.totalProgress     = FBL_MEM_PROGRESS_INITIAL;
   gPrevProgressInfo.partialProgress   = FBL_MEM_PROGRESS_INITIAL;
}

/***********************************************************************************************************************
 *  FblMemReportProgress
 **********************************************************************************************************************/
/*! \brief      Report current progress
 *  \details    Report current progress to user callback in case any of the following applies:
 *                - Type or segment count has changed since previous report
 *                - Partial progress completed, but not reported yet
 *                - Total or partial progress increased by at least the configured threshold since previous report
 **********************************************************************************************************************/
void FblMemReportProgress( void )
{
   /* Check for relevant changes since previous report */
   if (  (FBL_MEM_PROGRESS_COMPLETE       == gProgressInfo.partialProgress)
      || (gPrevProgressInfo.type          != gProgressInfo.type)
      || (gPrevProgressInfo.segmentCount  != gProgressInfo.segmentCount)
      || ((gProgressInfo.totalProgress - gPrevProgressInfo.totalProgress) >= (vuint8)FBL_MEM_PROGRESS_THRESHOLD_PERCENTAGE)
      || ((gProgressInfo.partialProgress - gPrevProgressInfo.partialProgress) >= (vuint8)FBL_MEM_PROGRESS_THRESHOLD_PERCENTAGE) )
   {
      /* Prevent re-reporting of completed partial progress */
      if (FBL_MEM_PROGRESS_COMPLETE != gPrevProgressInfo.partialProgress)
      {
         /* Inform user callback */
         __ApplFblMemReportProgress(&gProgressInfo);

         /* Remember reported progress */
         gPrevProgressInfo = gProgressInfo;
      }
   }
}

/***********************************************************************************************************************
 *  FblMemSetupProgress
 **********************************************************************************************************************/
/*! \brief      Setup progress for new partial operation
 *  \details    Stores the meta information (type, logical block address and segment count), sets up the percentage
 *              offset and current contribution of the total operation and stores the target value of the partial operation
 *  \param[in]  type Type of partial operation
 *  \param[in]  logicalAddress Logical start address of processed block
 *  \param[in]  segmentCount Count of segment processed by partial operation, typically zero for erase and verification,
 *                           segment index for programming and index of last segment incremented by one for concluding
 *                           gap fill
 *  \param[in]  totalOffset Percentage offset of total operation for current operation type
 *  \param[in]  totalPercentage Percentage contribution to total operation of current operation type
 *  \param[in]  target Expected maximum value of partial operation in arbitrary unit (e.g. bytes)
 **********************************************************************************************************************/
void FblMemSetupProgress( tFblMemProgressType type, tFblAddress logicalAddress, vuint32 segmentCount,
   vuint8 totalOffset, vuint8 totalPercentage, vuint32 target )
{
   /* Store meta information */
   gProgressInfo.type                     = type;
   gProgressInfo.logicalAddress           = logicalAddress;
   gProgressInfo.segmentCount             = segmentCount;

   /* Setup percentage offset and contribution of total operation */
   gProgressInfoInternal.totalOffset      = totalOffset;
   gProgressInfoInternal.totalPercentage  = totalPercentage;
   /* Store target value of partial operation */
   gProgressInfoInternal.target           = target;

   /* Ensure first update is reported by setting default values for previously reported info */
   gProgressPrevRemainder                 = 0u;
   gPrevProgressInfo.totalProgress        = FBL_MEM_PROGRESS_INITIAL;
   gPrevProgressInfo.partialProgress      = FBL_MEM_PROGRESS_INITIAL;
}

/***********************************************************************************************************************
 *  FblMemOffsetProgress
 **********************************************************************************************************************/
/*! \brief      Modify total percentages to reflect current progress
 *  \details    During progress setup the total percentages reflect the complete contribution of all operations of the
 *              current type. This function offsets those values according to the current progress.
 *  \pre        FblMemSetupProgress called before
 *  \param[in]  totalDone Current total progress in arbitrary unit (e.g. bytes)
 *  \param[in]  totalTarget Expected maximum value of total operation in arbitrary unit (e.g. bytes)
 **********************************************************************************************************************/
void FblMemOffsetProgress( vuint32 totalDone, vuint32 totalTarget )
{
   /* Update offset according to current progress */
   gProgressInfoInternal.totalOffset      +=
      FblMemCalculateProgress(totalDone, totalTarget, gProgressInfoInternal.totalPercentage);
   /* Cut down percentage contribution according to current progress */
   gProgressInfoInternal.totalPercentage   =
      FblMemCalculateProgress(gProgressInfoInternal.target, totalTarget, gProgressInfoInternal.totalPercentage);
}

/***********************************************************************************************************************
 *  FblMemUpdateProgress
 **********************************************************************************************************************/
/*! \brief      Update the progress information
 *  \details    Progress information is updated according to the progress of the current partial operation
 *              Update is performed in case any of the following applies:
 *                - First update
 *                - Progress increased by at least the configured threshold since previous update
 *  \pre        FblMemSetupProgress and optionally FblMemOffsetProgress called before
 *  \param[in]  remainder Remainder of current partial operation in arbitrary unit (e.g. bytes)
 **********************************************************************************************************************/
void FblMemUpdateProgress( vuint32 remainder )
{
   vuint32 done;

   /* Check for relevant changes since previous update */
   if (  (gProgressPrevRemainder < remainder)
      || ((gProgressPrevRemainder - remainder) >= FBL_MEM_PROGRESS_THRESHOLD_BYTES) )
   {
      /* Calculate current partial progress in arbitray unit */
      done = gProgressInfoInternal.target - remainder;

      /* Calculate progress percentages */
      gProgressInfo.totalProgress   =
         gProgressInfoInternal.totalOffset
         + FblMemCalculateProgress(done, gProgressInfoInternal.target, gProgressInfoInternal.totalPercentage);
      gProgressInfo.partialProgress =
         FBL_MEM_PROGRESS_INITIAL + FblMemCalculateProgress(done, gProgressInfoInternal.target, FBL_MEM_PROGRESS_COMPLETE);

      /* Report updated progress */
      FblMemReportProgress();

      /* Remember currently updated remainder value */
      gProgressPrevRemainder = remainder;
   }
}

/***********************************************************************************************************************
 *  FblMemConcludeProgress
 **********************************************************************************************************************/
/*! \brief      Conclude the current partial operation
 *  \details    Explicitely set partial progress to maximum percentage and total progress according to the set up values
 *  \pre        FblMemSetupProgress and optionally FblMemOffsetProgress called before
 **********************************************************************************************************************/
void FblMemConcludeProgress( void )
{
   /* Set progress percentages to maximum values */
   gProgressInfo.totalProgress   = gProgressInfoInternal.totalOffset + gProgressInfoInternal.totalPercentage;
   gProgressInfo.partialProgress = FBL_MEM_PROGRESS_COMPLETE;

   /* Report updated progress */
   FblMemReportProgress();
}

/***********************************************************************************************************************
 *  FblMemProgressRead
 **********************************************************************************************************************/
/*! \brief      Update verification progress
 *  \details    Read operations of output verification are re-routed through this function to update the progress
 *              information according the current read address
 *  \param[in]  address Memory address to read out
 *  \param[out] buffer Target buffer
 *  \param[in]  length Number of bytes to read
 *  \return     Number of actually copied bytes
 **********************************************************************************************************************/
# if defined( FBL_MEM_ENABLE_VERIFY_OUTPUT )
#  if defined( FBL_MEM_ENABLE_SWITCH_READMEMORY_PARAM )
/* Parameters order changed in comparison to HIS security module specification */
tFblMemVerifySize FblMemProgressRead( tFblMemVerifyAddr address, tFblMemVerifySize length, tFblMemVerifyDataPtr buffer )
#  else
/* Parameters order as defined by HIS security module specification */
tFblMemVerifySize FblMemProgressRead( tFblMemVerifyAddr address, tFblMemVerifyDataPtr buffer, tFblMemVerifySize length )
#  endif /* FBL_MEM_ENABLE_SWITCH_READMEMORY_PARAM */
{
   vuint32 position;

   /* Calculate position relative to block start address */
   position = address - gBlockInfo.targetAddress;
   /* Update progress with remainder */
   FblMemUpdateProgress(gBlockInfo.targetLength - position);

   /* Perform actual read operation */
   return gBlockInfo.readFct(address, buffer, length);
}
# endif /* FBL_MEM_ENABLE_VERIFY_OUTPUT */
#endif /* FBL_MEM_ENABLE_PROGRESS_INFO */

/***********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  FblMemInitPowerOnExt
 **********************************************************************************************************************/
/*! \brief      Initialize module
 *  \param[in]  preambleLen  Length of preamble stored during buffer switch
 *  \param[in]  sourceHandle Handle of input source
 *  \return     Pointer to initial input buffer
 **********************************************************************************************************************/
tFblMemRamData FblMemInitPowerOnExt( tFblLength preambleLen, tFblMemInputSource sourceHandle ) /* PRQA S 1505 */ /* MD_FblMem_1505 */
{
#if defined( FBL_MEM_ENABLE_GAP_FILL )
   vuintx idx;
#endif /* FBL_MEM_ENABLE_GAP_FILL */

#if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   /* Parameters not used: avoid compiler warning */
# if defined( FBL_MEM_ENABLE_DYNAMIC_PREAMBLE_LENGTH )
# else
   (void)preambleLen; /* PRQA S 3112 */ /* MD_FblMem_3112 */
# endif /* FBL_MEM_ENABLE_DYNAMIC_PREAMBLE_LENGTH */
# if defined( FBL_MEM_ENABLE_MULTI_SOURCE )
# else
   (void)sourceHandle; /* PRQA S 3112 */ /* MD_FblMem_3112 */
# endif /* FBL_MEM_ENABLE_MULTI_SOURCE */
#endif /* V_ENABLE_USE_DUMMY_STATEMENT */

#if defined( FBL_MEM_ENABLE_MULTI_SOURCE )
   /* Verify source handle lies within range */
   assertFblUser(sourceHandle < FBL_MEM_SOURCE_COUNT, kFblMemAssertParameterOutOfRange);
#endif /* FBL_MEM_ENABLE_MULTI_SOURCE */

#if defined( FBL_MEM_ENABLE_PROC_QUEUE )
   /* Verify job prio mapping array */
   assertFblInternal(kFblMemJobType_Max == (FBL_MEM_ARRAY_SIZE(gJobPrio) - 1), kFblMemAssertParameterOutOfRange); /* PRQA S 3201 */ /* PRQA S 3325 */ /* MD_FblMem_AssertJobMax */ /* PRQA S 2214 */ /* MD_FblDef_Assert */
#endif /* FBL_MEM_ENABLE_PROC_QUEUE */
#if defined( FBL_MEM_ENABLE_SEGMENTATION ) || \
    defined( FBL_MEM_ENABLE_VERIFY_PIPELINED )
   /* Verify length limit mapping array */
   assertFblInternal(kFblMemJobType_Max == (FBL_MEM_ARRAY_SIZE(gLengthLimits) - 1), kFblMemAssertParameterOutOfRange);  /* PRQA S 3201 */ /* PRQA S 3325 */ /* MD_FblMem_AssertJobMax */ /* PRQA S 2214 */ /* MD_FblDef_Assert */
#endif /* FBL_MEM_ENABLE_SEGMENTATION || FBL_MEM_ENABLE_VERIFY_PIPELINED */

#if defined( FBL_ENABLE_DATA_PROCESSING )
   /* Initialize data processing info */
   FblMemInitJob(&gProcWriteJob, gProcBuffer.data, FBL_MEM_ARRAY_SIZE(gProcBuffer.data), kFblMemJobType_ProcWrite);
   /* Overwrite net size, to exclude overhead for remainder and padding */
   gProcWriteJob.netSize = FBL_MEM_PROC_BUFFER_SIZE;
   /* Initialize info to trigger data processing finalization */
   FblMemInitJob(&gProcFinalizeJob, FBL_MEM_BUFFER_NULL, 0, kFblMemJobType_ProcFinalize);

   /*
      Always provide full configured segmentation size to data processing,
      regardless of any remainder
   */
   gProcParam.dataOutMaxLength = (vuint16)FBL_MEM_INTERNAL_PROC_SEGMENTATION;
# if defined( FBL_MEM_ENABLE_EXT_TRIGGER_DATA_PROC )
   gProcParam.wdTriggerFct     = FblMemTriggerWatchdogExt;
# else
   gProcParam.wdTriggerFct     = FblMemTriggerWatchdog;
# endif /* FBL_MEM_ENABLE_EXT_TRIGGER_DATA_PROC */
#endif /* FBL_ENABLE_DATA_PROCESSING */

#if defined( FBL_MEM_ENABLE_STREAM_OUTPUT )
   /* Initialize stream output info */
# if defined( FBL_ENABLE_DATA_PROCESSING )
   FblMemInitJob(&gStreamProcJob, gProcBuffer.data, FBL_MEM_ARRAY_SIZE(gProcBuffer.data), kFblMemJobType_StreamProc);
   /* Overwrite net size, to exclude overhead for remainder and padding */
   gStreamProcJob.netSize = FBL_MEM_PROC_BUFFER_SIZE;
# endif /* FBL_ENABLE_DATA_PROCESSING */
   /* Initialize info to trigger stream output finalization */
   FblMemInitJob(&gStreamFinalizeJob, FBL_MEM_BUFFER_NULL, 0, kFblMemJobType_StreamFinalize);

   gStreamParam.outputData = FBL_MEM_BUFFER_NULL;
   gStreamParam.outputSize = 0u;
   gStreamParam.watchdog   = FblMemTriggerWatchdog;
#endif /* FBL_MEM_ENABLE_STREAM_OUTPUT */

#if defined( FBL_MEM_ENABLE_GAP_FILL )
   /* Initialize gap fill job */
   FblMemInitJob(&gGapFillJob, FBL_MEM_BUFFER_NULL, FBL_MEM_ARRAY_SIZE(gGapFillBuffer.data), kFblMemJobType_GapFill);

   /* Fill gap fill buffer with fill character */
   for (idx = 0u; idx < FBL_MEM_ARRAY_SIZE(gGapFillBuffer.data); idx++)
   {
      gGapFillBuffer.data[idx] = kFillChar;
   }
#endif /* FBL_MEM_ENABLE_GAP_FILL */

#if defined( FBL_MEM_ENABLE_MULTI_SOURCE )
   gActiveSource = sourceHandle;
#endif /* FBL_MEM_ENABLE_MULTI_SOURCE */

#if defined( FBL_MEM_ENABLE_DYNAMIC_PREAMBLE_LENGTH )
   gPreambleLength[FBL_MEM_ACTIVE_SOURCE] = preambleLen;
#endif /* FBL_MEM_ENABLE_DYNAMIC_PREAMBLE_LENGTH */

   /* Perform additional initialization and return initial buffer */
   return FblMemInitInternal();
}

/***********************************************************************************************************************
 *  FblMemInitPowerOn
 **********************************************************************************************************************/
/*! \brief    Initialize module
 *  \return   Pointer to initial input buffer
 **********************************************************************************************************************/
tFblMemRamData FblMemInitPowerOn( void )
{
   /* Perform initialization and return initial buffer */
   return FblMemInitPowerOnExt(FBL_MEM_DEFAULT_PREAMBLE_LENGTH, FBL_MEM_SOURCE_HANDLE_DEFAULT);
}

/***********************************************************************************************************************
 *  FblMemInit
 **********************************************************************************************************************/
/*! \brief    (Re)-initialize input buffers
 *  \details  Should be called before performing additional tasks in case a previous operation failed
 *  \pre      FblMemInitPowerOn executed before
 *  \return   Pointer to active input buffer
 **********************************************************************************************************************/
tFblMemRamData FblMemInit( void )
{
   tFblMemRamData activeBuffer;

#if defined( FBL_MEM_ENABLE_PREAMBLE_HANDLING )
   /*
      Active fill buffer may change
      Store preamble so operation is transparent for caller
   */
   FblMemStorePreamble();
#endif /* FBL_MEM_ENABLE_PREAMBLE_HANDLING */

   /* Perform re-initialization and get active buffer */
   activeBuffer = FblMemInitInternal();

#if defined( FBL_MEM_ENABLE_PREAMBLE_HANDLING )
   /*
      Active fill buffer may have changed
      Restore previously stored preamble so operation is transparent for caller
   */
   FblMemRestorePreamble();
#endif /* FBL_MEM_ENABLE_PREAMBLE_HANDLING */

   return activeBuffer;
}

/***********************************************************************************************************************
 *  FblMemDeinit
 **********************************************************************************************************************/
/*! \brief    De-initialize module
 *  \details  Should be called when memory driver is de-initialized
 **********************************************************************************************************************/
void FblMemDeinit( void )
{
   /* No further operations allowed, reset states */
   FblMemInitStates();
}

#if defined( FBL_MEM_ENABLE_MULTI_SOURCE )
/***********************************************************************************************************************
 *  FblMemLockInputSource
 **********************************************************************************************************************/
/*! \brief      Lock input to specific source
 *  \param[in]  sourceHandle Handle of input source
 **********************************************************************************************************************/
void FblMemLockInputSource( tFblMemInputSource sourceHandle )
{
   /* Verify source handle lies within range */
   assertFblInternal(sourceHandle < FBL_MEM_SOURCE_COUNT, kFblMemAssertParameterOutOfRange);

   gActiveSource = sourceHandle;
}
#endif /* FBL_MEM_ENABLE_MULTI_SOURCE */

/***********************************************************************************************************************
 *  FblMemGetActiveBuffer
 **********************************************************************************************************************/
/*! \brief    Return active input buffer, which can be used for current data reception
 *  \details  Buffer pointer includes previously set offset to accommodate for alignment requirements
 *  \pre      FblMemInitPowerOn executed before
 *  \return   Pointer to active input buffer
 **********************************************************************************************************************/
tFblMemRamData FblMemGetActiveBuffer( void )
{
   V_MEMRAM1 tFblMemJob V_MEMRAM2 V_MEMRAM3 * inputJob;

   /* Get pending job info */
   inputJob = FblMemGetPendingInputJob();
   /* Reset position to current offset */
   inputJob->position = inputJob->offset;

   /* Return pointer to active fill buffer */
   return FblMemGetBuffer(inputJob);
}

/***********************************************************************************************************************
 *  FblMemBlockEraseIndication
 **********************************************************************************************************************/
/*! \brief      Indicate start of a new block (just before first segment)
 *  \pre        FblMemInitPowerOn executed before
 *  \param[in]  block  Pointer to block information structure
 *                     Only address and length members have to be initialized
 *  \return     Result of operation (potentially remapped to OEM specific NRC)
 **********************************************************************************************************************/
tFblMemStatus FblMemBlockEraseIndication( const V_MEMRAM1 tFblMemBlockInfo V_MEMRAM2 V_MEMRAM3 * block )
{
   tFblMemStatus  retVal;

   /* Check allowed states */
   if (kFblOk != FblMemCheckAllowed(FBL_MEM_ALLOWED_BLOCK_ERASE, FBL_MEM_ALLOWED_NONE))
   {
      FBL_MEM_SET_STATUS(BlockEraseSequence, retVal);   /* PRQA S 3109 */ /* MD_CBD_14.3 */
   }
   else
   {
#if defined( FBL_MEM_ENABLE_PROGRESS_INFO )
      /* Setup erase progress */
      FblMemSetupProgress(kFblMemProgressType_Erase, block->logicalAddress, 0u,
         FBL_MEM_PROGRESS_INITIAL, FBL_MEM_PROGRESS_ERASE, block->targetLength);
      /* Report initial progress */
      FblMemUpdateProgress(block->targetLength);

      /* Explicitely enable progress information for erase operation */
      gProgressState = kFblMemProgressState_Enabled;
#endif /* FBL_MEM_ENABLE_PROGRESS_INFO */

      /* Perform erase */
      retVal = FblMemEraseRegionInternal(block->targetAddress, block->targetLength);

#if defined( FBL_MEM_ENABLE_PROGRESS_INFO )
      /* Conclude erase progress */
      FblMemConcludeProgress();
#endif /* FBL_MEM_ENABLE_PROGRESS_INFO */
   }

   if (kFblMemStatus_Ok == retVal)
   {
      /* Allow block start / erase indication */
      FblMemSetAllowed(FBL_MEM_ALLOWED_BLOCK_START | FBL_MEM_ALLOWED_BLOCK_ERASE);
   }

   return retVal;
}

/***********************************************************************************************************************
 *  FblMemBlockStartIndication
 **********************************************************************************************************************/
/*! \brief       Indicate start of a new block (just before first segment)
 *  \details     Block may consist of one or more segments
 *  \pre         FblMemInitPowerOn executed before
 *               FblMemInit executed before in case previous cycle failed
 *  \param[in]   block  Pointer to block information structure
 *                      Required members depending on configuration
 *  \return      Result of operation (potentially remapped to OEM specific NRC)
 **********************************************************************************************************************/
tFblMemStatus FblMemBlockStartIndication( V_MEMRAM1 tFblMemBlockInfo V_MEMRAM2 V_MEMRAM3 * block ) /* PRQA S 3673 */ /* MD_FblMem_3673 */
{
   tFblMemStatus     retVal;

#if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   /* Parameters not used: avoid compiler warning */
# if defined( FBL_MEM_ENABLE_GLOBAL_BLOCK_INFO )
# else
   (void)block; /* PRQA S 3112 */ /* MD_FblMem_3112 */
# endif /* FBL_MEM_ENABLE_GLOBAL_BLOCK_INFO */
#endif /* V_ENABLE_USE_DUMMY_STATEMENT */

   retVal = kFblMemStatus_Ok;

   /* Check allowed states */
   if (kFblOk != FblMemCheckAllowed(FBL_MEM_ALLOWED_BLOCK_START, FBL_MEM_ALLOWED_BLOCK_START))
   {
      FBL_MEM_SET_STATUS(BlockStartSequence, retVal);   /* PRQA S 3109 */ /* MD_CBD_14.3 */
   }
   else
   {
      /* Satisfy MISRA */
   }

   if (kFblMemStatus_Ok == retVal)
   {
      /* Store block information */
#if defined( FBL_MEM_ENABLE_GLOBAL_BLOCK_INFO )
      gBlockInfo = *block;
#endif /* FBL_MEM_ENABLE_GLOBAL_BLOCK_INFO */
#if defined( FBL_MEM_ENABLE_SEGMENT_HANDLING )
      /* Reset segment list */
      gBlockInfo.segmentList->nrOfSegments = 0u;
#endif /* FBL_MEM_ENABLE_SEGMENT_HANDLING */

      /* Setup index of first segment */
      gSegInfo.nextIndex = 0u;

      /* Allow segment start indication */
      FblMemSetAllowed(FBL_MEM_ALLOWED_SEGMENT_START);
   }

   return retVal;
}

/***********************************************************************************************************************
 *  FblMemBlockEndIndication
 **********************************************************************************************************************/
/*! \brief    Indicate end of current block (after last segment)
 *  \details  Finalize verification if configured
 *  \pre      FblMemSegmentEndIndication executed before
 *  \return   Result of operation (potentially remapped to OEM specific NRC)
 **********************************************************************************************************************/
tFblMemStatus FblMemBlockEndIndication( void )
{
   tFblMemStatus retVal;
#if defined( FBL_MEM_ENABLE_GAP_FILL )
   tFblAddress baseAddress;
   tFblLength  baseLength;
#endif /* FBL_MEM_ENABLE_GAP_FILL */

   retVal = kFblMemStatus_Ok;

   /* Check allowed states */
   if (kFblOk != FblMemCheckAllowed(FBL_MEM_ALLOWED_BLOCK_END, FBL_MEM_ALLOWED_BLOCK_END))
   {
      FBL_MEM_SET_STATUS(BlockEndSequence, retVal);   /* PRQA S 3109 */ /* MD_CBD_14.3 */
   }
   else
   {
#if defined( FBL_MEM_ENABLE_GAP_FILL )
      /* No gap fill required for volatile memory */
      if (kFblMemType_RAM != gSegInfo.input.type)
      {
         /* Address range information of last segment */
         baseAddress = gBlockInfo.segmentList->segmentInfo[gSegInfo.ownIndex].targetAddress;
         baseLength  = gBlockInfo.segmentList->segmentInfo[gSegInfo.ownIndex].length;

         /* Include padding previously applied to end of last segment */
         baseLength += FblMemPadLength(baseAddress, baseLength);

         /* Length till end of block
            Subtract offset of segment start and length of segment (including padding) from block length
            to prevent range overflow */
         gGapFillJob.used  = (gBlockInfo.targetLength - (baseAddress - gBlockInfo.targetAddress)) - baseLength;

         /* Gap fill not necessary when segment ends at last block address */
         if (gGapFillJob.used > 0u)
         {
            /* Setup concluding gap fill from end of last segment to end of block */
            gGapFillJob.position    = 0u;
            /* Gap fill starts after end of last segment */
            gGapFillJob.baseAddress = baseAddress + baseLength;

#if defined( FBL_MEM_ENABLE_PROGRESS_INFO )
            /* Setup gap fill progress */
            FblMemSetupProgress(kFblMemProgressType_GapFill, gBlockInfo.logicalAddress, gSegInfo.nextIndex,
               FBL_MEM_PROGRESS_INITIAL + FBL_MEM_PROGRESS_ERASE, FBL_MEM_PROGRESS_PROGRAM, gGapFillJob.used);
            /* Adjust total percentages */
            FblMemOffsetProgress((gGapFillJob.baseAddress - gBlockInfo.targetAddress), gBlockInfo.targetLength);
            /* Report initial progress */
            FblMemUpdateProgress(gGapFillJob.used);
#endif /* FBL_MEM_ENABLE_PROGRESS_INFO*/

            /* Trigger concluding gap fill */
            (void)FblMemQueueDefaultPrioInsert(gProcessingQueue, &gGapFillJob, gSegInfo.ownIndex);

            /* Continue operation */
            fblMemProgState = kFblMemProgState_Pending;

            /* Finish processing of all pending buffers */
            FblMemFlushQueueByPrio(kFblMemJobPrio_Write);
         }
      }
#endif /* FBL_MEM_ENABLE_GAP_FILL */

#if defined( FBL_MEM_ENABLE_PROGRESS_INFO )
      /* Conclude any pending progress (programming or gap fill) */
      FblMemConcludeProgress();
#endif /* FBL_MEM_ENABLE_PROGRESS_INFO*/

#if defined( FBL_MEM_ENABLE_VERIFY_PIPELINED )
      /* Null pointer passed as verification function disables functionality */
      if (FBL_MEM_VERIFY_FCT_INPUT_NULL != gBlockInfo.verifyRoutinePipe.function)
      {
         /* No additional segments for this block, trigger finalization of pipelined verification
            Add to processing queue as other jobs may still be pending */
         (void)FblMemQueueDefaultPrioInsert(gProcessingQueue, &gVerifyPipeFinalizeJob, gSegInfo.ownIndex);

         /* Continue operation */
         fblMemProgState = kFblMemProgState_Pending;
      }
      else
#endif /* FBL_MEM_ENABLE_VERIFY_PIPELINED */
      {
         /* Concluding else */
      }
   }

   if (kFblMemStatus_Ok == retVal)
   {
      /* Allow block verify */
      FblMemSetAllowed(FBL_MEM_ALLOWED_BLOCK_VERIFY);
   }

   return retVal;
}

/***********************************************************************************************************************
 *  FblMemBlockVerify
 **********************************************************************************************************************/
/*! \brief      Finalize block verification
 *  \details    Hint: also call function if verification not configured
 *  \pre        FblMemBlockEndIndication executed before
 *  \param[in]  verifyData Pointer to verification structure
 *                         Required members dependent on configuration
 *  \param[out] verifyResult Pointer to extended verification result
 *                           Null pointer if no extended result required
 *  \return     Result of operation (potentially remapped to OEM specific NRC)
 **********************************************************************************************************************/
tFblMemStatus FblMemBlockVerify( const V_MEMRAM1 tFblMemBlockVerifyData V_MEMRAM2 V_MEMRAM3 * verifyData,
   V_MEMRAM1 tFblMemVerifyStatus V_MEMRAM2 V_MEMRAM3 * verifyResult )
{
   tFblMemStatus retVal;
   tFblMemVerifyStatus localResult;
#if defined( FBL_MEM_ENABLE_VERIFY_OUTPUT )
# if defined( FBL_MEM_ENABLE_VERIFY_OUTPUT_FULL_BLOCK_LENGTH )
# else
   V_MEMRAM1 tFblMemSegmentListEntry V_MEMRAM2 V_MEMRAM3 * lastSegment;
# endif /* FBL_MEM_ENABLE_VERIFY_OUTPUT_FULL_BLOCK_LENGTH */
#endif /* FBL_MEM_ENABLE_VERIFY_OUTPUT */

#if defined( FBL_MEM_ENABLE_VERIFY_OUTPUT )     || \
    defined( FBL_MEM_ENABLE_VERIFY_PIPELINED )  
#else
# if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   /* Parameters not used: avoid compiler warning */
   (void)verifyData; /* PRQA S 3112 */ /* MD_FblMem_3112 */
# endif /* V_ENABLE_USE_DUMMY_STATEMENT */
#endif /* FBL_MEM_ENABLE_VERIFY_INPUT || FBL_MEM_ENABLE_VERIFY_OUTPUT || FBL_MEM_ENABLE_VERIFY_PIPELINED || FBL_MEM_ENABLE_PASSTHROUGH */

   retVal      = kFblMemStatus_Ok;
   localResult = FBL_MEM_VERIFY_OK;

   /* Check allowed states */
   if (kFblOk != FblMemCheckAllowed(FBL_MEM_ALLOWED_BLOCK_VERIFY, FBL_MEM_ALLOWED_NONE))
   {
      FBL_MEM_SET_STATUS(BlockVerifySequence, retVal);   /* PRQA S 3109 */ /* MD_CBD_14.3 */
   }
   else
   {
#if defined( FBL_MEM_ENABLE_VERIFY_PIPELINED )
      /* Finish processing of all pending verification jobs */
      FblMemFlushQueueByPrio(kFblMemJobPrio_Lowest);

      /* Check for error condition */
      if (kFblMemProgState_Error == fblMemProgState)
      {
         /* Report error set by programming routines (previous or current) */
         retVal = gErrorStatus;
      }
      else
#endif /* FBL_MEM_ENABLE_VERIFY_PIPELINED */
      {
#if defined( FBL_MEM_ENABLE_VERIFY_PIPELINED )
         /* Pipelined verification is used
            Finalize by comparing with the transferred signature */
         if (kFblMemStatus_Ok != FblMemVerifyInput( &gBlockInfo.verifyRoutinePipe, &verifyData->verifyDataPipe,
                                                    FBL_MEM_VERIFY_STATE_VERIFY, &localResult ))
         {
            FBL_MEM_SET_STATUS(BlockVerifyPipeVerify, retVal);  /* PRQA S 3109 */ /* MD_CBD_14.3 */
         }
         else
#endif /* FBL_MEM_ENABLE_VERIFY_PIPELINED */
#if defined( FBL_MEM_ENABLE_VERIFY_OUTPUT )
         {
            /* Null pointer passed as verification function disables functionality */
            if (FBL_MEM_VERIFY_FCT_OUTPUT_NULL != gBlockInfo.verifyRoutineOutput.function)
            {
               /* Initialize verification parameter structure */
# if defined( FBL_MEM_ENABLE_EXT_TRIGGER_OUTPUT_VERIFY )
               gBlockInfo.verifyRoutineOutput.param->wdTriggerFct       = (tFblMemVerifyWdFct)FblMemTriggerWatchdogExt;
# else
               gBlockInfo.verifyRoutineOutput.param->wdTriggerFct       = (tFblMemVerifyWdFct)FblMemTriggerWatchdog;
# endif /* FBL_MEM_ENABLE_EXT_TRIGGER_OUTPUT_VERIFY */
#if defined( FBL_MEM_ENABLE_PROGRESS_INFO )
               /* Setup erase progress */
               FblMemSetupProgress(kFblMemProgressType_Verify, gBlockInfo.logicalAddress, 0u,
                  FBL_MEM_PROGRESS_INITIAL + FBL_MEM_PROGRESS_ERASE + FBL_MEM_PROGRESS_PROGRAM, FBL_MEM_PROGRESS_VERIFY,
                  gBlockInfo.targetLength);
               /* Report initial progress */
               FblMemUpdateProgress(gBlockInfo.targetLength);

               /* Overwrite read function to keep track of progress */
               gBlockInfo.verifyRoutineOutput.param->readMemory         = FblMemProgressRead;
#else
               gBlockInfo.verifyRoutineOutput.param->readMemory         = gBlockInfo.readFct;
#endif /* FBL_MEM_ENABLE_PROGRESS_INFO*/
               gBlockInfo.verifyRoutineOutput.param->verificationData   = verifyData->verifyDataOutput.data;

               gBlockInfo.verifyRoutineOutput.param->blockStartAddress  = gBlockInfo.targetAddress;
# if defined( FBL_MEM_ENABLE_VERIFY_OUTPUT_FULL_BLOCK_LENGTH )
               gBlockInfo.verifyRoutineOutput.param->blockLength        = gBlockInfo.targetLength;
# else
               /* Total length covered by all programmed segments (from block start)*/
               lastSegment = &(gBlockInfo.segmentList->segmentInfo[gBlockInfo.segmentList->nrOfSegments - 1u]);

               /* Segments have to be stored in increasing address order */
               gBlockInfo.verifyRoutineOutput.param->blockLength = (lastSegment->targetAddress - gBlockInfo.targetAddress)
                                                                   + lastSegment->length;
# endif /* FBL_MEM_ENABLE_VERIFY_OUTPUT_FULL_BLOCK_LENGTH */

               /* Call verification function */
               localResult = gBlockInfo.verifyRoutineOutput.function(gBlockInfo.verifyRoutineOutput.param);
               if (FBL_MEM_VERIFY_OK != localResult)
               {
                  FBL_MEM_SET_STATUS(BlockVerifyOutputVerify, retVal);  /* PRQA S 3109 */ /* MD_CBD_14.3 */
               }
            }
         }
#else
         {
            /* Concluding else */
         }
#endif /* FBL_MEM_ENABLE_VERIFY_OUTPUT */

#if defined( FBL_MEM_ENABLE_PROGRESS_INFO )
         /* Conclude pending verification progress */
         FblMemConcludeProgress();
#endif /* FBL_MEM_ENABLE_PROGRESS_INFO*/
      }
   }

   if (kFblMemStatus_Ok == retVal)
   {
      /* Allow new block start / erase indication */
      FblMemAddAllowed(FBL_MEM_ALLOWED_BLOCK_START | FBL_MEM_ALLOWED_BLOCK_ERASE);
   }

   if (FBL_MEM_VERIFY_STATUS_NULL != verifyResult)
   {
      /* Pass extended result */
      *verifyResult = localResult;
   }

   return retVal;
} /* PRQA S 6080 */ /* MD_FblMem_60xx */

/***********************************************************************************************************************
 *  FblMemSegmentStartIndication
 **********************************************************************************************************************/
/*! \brief      Indicate start of a new segment
 *  \details    Segment may consist of one or more chunks of input data programmed into a contiguous memory range
 *              Update verification with logical address and length if configured
 *              Initialize data processing if configured
 *  \pre        FblMemBlockStartIndication executed before
 *  \param[in]  segment Pointer to segment information
 *                         Target address (program operation)
 *                         Target length (program operation)
 *                          either unprocessed data length actually written to memory or processed data length of input
 *                          data
 *                         Logical address (verification operation)
 *                         Logical length (verification operation)
 *                         Type (RAM / ROM)
 *                         Data format identifier (data processing)
 *  \return     Result of operation (potentially remapped to OEM specific NRC)
 **********************************************************************************************************************/
tFblMemStatus FblMemSegmentStartIndication( const V_MEMRAM1 tFblMemSegmentInfo V_MEMRAM2 V_MEMRAM3 * segment )
{
   tFblMemStatus     retVal;
#if defined( FBL_MEM_ENABLE_VERIFY_PIPELINED )
   tFblMemVerifyData verifyData;
#endif /* FBL_MEM_ENABLE_VERIFY_INPUT || FBL_MEM_ENABLE_VERIFY_PIPELINED */
#if defined( FBL_MEM_ENABLE_VERIFY_PIPELINED ) && \
    defined( FBL_MEM_ENABLE_VERIFY_PIPELINED_ADDRESS_LENGTH )
   V_MEMRAM1 tFblMemJob V_MEMRAM2 V_MEMRAM3 * verifyJob;
#endif /* FBL_MEM_ENABLE_VERIFY_PIPELINED && FBL_MEM_ENABLE_VERIFY_PIPELINED_ADDRESS_LENGTH */
#if defined( FBL_MEM_ENABLE_GAP_FILL )
   tFblAddress baseAddress;
   tFblLength  baseLength;
#endif /* FBL_MEM_ENABLE_GAP_FILL */
#if defined( FBL_MEM_ENABLE_PROGRESS_INFO )
   tFblAddress progressAddress;
   tFblLength  progressRemainder;
#endif /* FBL_MEM_ENABLE_PROGRESS_INFO */

   retVal = kFblMemStatus_Ok;

#if defined( FBL_ENABLE_DATA_PROCESSING )    || \
    defined( FBL_MEM_ENABLE_STREAM_OUTPUT )  || \
    defined( FBL_MEM_ENABLE_REMAINDER_HANDLING )
   /* Remember input buffer type: write through */
   gSegInfo.jobType = kFblMemJobType_InputWrite;
#endif /* FBL_ENABLE_DATA_PROCESSING || FBL_MEM_ENABLE_STREAM_OUTPUT || FBL_MEM_ENABLE_PASSTHROUGH || FBL_MEM_ENABLE_REMAINDER_HANDLING */

   /* Check allowed states */
   if (kFblOk != FblMemCheckAllowed(FBL_MEM_ALLOWED_SEGMENT_START, FBL_MEM_ALLOWED_SEGMENT_START))
   {
      FBL_MEM_SET_STATUS(SegmentStartSequence, retVal);  /* PRQA S 3109 */ /* MD_CBD_14.3 */
   }
#if defined( FBL_MEM_ENABLE_SEGMENT_HANDLING )
   else if (gBlockInfo.segmentList->nrOfSegments >= gBlockInfo.maxSegments)
   {
      /* Maximum number of segment list entries exceeded */
      FBL_MEM_SET_STATUS(SegmentStartSegmentCount, retVal);  /* PRQA S 3109 */ /* MD_CBD_14.3 */
   }
#endif /* FBL_MEM_ENABLE_SEGMENT_HANDLING */
   else
   {
#if defined( FBL_MEM_ENABLE_VERIFY_PIPELINED )
      /* Initialize verification on first segment */
      if (0u == gSegInfo.nextIndex)
      {
         /* Not used by initialization */
         verifyData.data   = FBL_MEM_BUFFER_NULL;
         verifyData.length = 0u;

         /* Inverted order of verification routines
            Error code may be overwritten (simplifies implementation) */
# if defined( FBL_MEM_ENABLE_VERIFY_PIPELINED )
         /* Initialize the calculation */
         if (kFblMemStatus_Ok != FblMemVerifyInput( &gBlockInfo.verifyRoutinePipe, &verifyData,
                                                      FBL_MEM_VERIFY_STATE_INIT, FBL_MEM_VERIFY_STATUS_NULL ))
         {
            /* Overwrites previous error code */
            FBL_MEM_SET_STATUS(SegmentStartVerifyInit, retVal); /* PRQA S 3109 */ /* MD_CBD_14.3 */
         }
# endif /* FBL_MEM_ENABLE_VERIFY_PIPELINED */

      }

      if (kFblMemStatus_Ok == retVal)
#endif /* FBL_MEM_ENABLE_VERIFY_INPUT || FBL_MEM_ENABLE_VERIFY_PIPELINED */
#if defined( FBL_MEM_ENABLE_VERIFY_ADDRESS_LENGTH )
      {
         {
# if defined( FBL_MEM_ENABLE_VERIFY_PIPELINED ) && \
     defined( FBL_MEM_ENABLE_VERIFY_PIPELINED_ADDRESS_LENGTH )
#  if defined( __ApplFblMemVerifyPipelinedIsAddressAndLengthIncluded )
            /* Skip update of signature information? */
            if (kFblOk == __ApplFblMemVerifyPipelinedIsAddressAndLengthIncluded(&gBlockInfo, segment))
#  endif
            {
               /* Get first free job
                  At least one (additional) job should be free at this point as the previous data indication(s)
                  unblocked the pipelined verification queue */
               verifyJob = FblMemPrepareVerifyPipeJob(gSegInfo.nextIndex, segment->targetAddress);

               /* Check for null pointer */
               if (FBL_MEM_JOB_NULL != verifyJob)
               {
                  /* Switch job type to include segment information */
                  verifyJob->type         = kFblMemJobType_VerifyPipeInfo;
                  verifyJob->used         = FBL_MEM_VERIFY_ADDRESS_LENGTH_BUFFER_SIZE;
                  verifyJob->netSize      = FBL_MEM_VERIFY_ADDRESS_LENGTH_BUFFER_SIZE;
                  verifyJob->totalSize    = FBL_MEM_VERIFY_ADDRESS_LENGTH_BUFFER_SIZE;
                  /* Do not remove verification job as it is reused for the read-back */
                  verifyJob->completion   = kFblMemOperationMode_Finalize;

                  FblMemSetInteger(FBL_MEM_VERIFY_ADDRESS_LENGTH_SIZE, segment->logicalAddress, &verifyJob->buffer[0u]);
                  FblMemSetInteger(FBL_MEM_VERIFY_ADDRESS_LENGTH_SIZE, segment->logicalLength,  &verifyJob->buffer[FBL_MEM_VERIFY_ADDRESS_LENGTH_SIZE]);
               }
            }
# endif /* FBL_MEM_ENABLE_VERIFY_PIPELINED && FBL_MEM_ENABLE_VERIFY_PIPELINED_ADDRESS_LENGTH */
         }
      }

      if (kFblMemStatus_Ok == retVal)
#endif /* FBL_MEM_ENABLE_VERIFY_ADDRESS_LENGTH */
      {
         /* Check whether data processing is requested */
         if (kFblOk == __ApplFblMemIsDataProcessingRequired(segment->dataFormat))
         {
#if defined( FBL_ENABLE_DATA_PROCESSING )
            /* Remember input buffer type: data processing */
            gSegInfo.jobType = kFblMemJobType_ProcInput;

            /* Initialize user specific processing of received data */
            gProcParam.mode = segment->dataFormat;

            /* Check result */
            if (kFblOk != ApplFblInitDataProcessing(&gProcParam))
            {
               FBL_MEM_SET_STATUS(SegmentStartDataProcInit, retVal); /* PRQA S 3109 */ /* MD_CBD_14.3 */
            }
#else
            /* Data processing not supported, refuse indication */
            FBL_MEM_SET_STATUS(SegmentStartDataProcInit, retVal); /* PRQA S 3109 */ /* MD_CBD_14.3 */
#endif /* FBL_ENABLE_DATA_PROCESSING */
         }
      }

#if defined( __ApplFblMemIsStreamOutputRequired )
      if (kFblMemStatus_Ok == retVal)
      {
         /* Check whether stream output is requested */
         if (kFblOk == __ApplFblMemIsStreamOutputRequired(segment->dataFormat))
         {
# if defined( FBL_MEM_ENABLE_STREAM_OUTPUT )
#  if defined( FBL_ENABLE_DATA_PROCESSING )
            if (kFblOk == __ApplFblMemIsDataProcessingRequired(segment->dataFormat))
            {
               /* Input buffer type (data processing) already set */
            }
            else
#  endif /* FBL_ENABLE_DATA_PROCESSING */
            {
               /* Remember input buffer type: stream output */
               gSegInfo.jobType = kFblMemJobType_StreamInput;
            }

            /* Initialize user specific stream output of received data */
            gStreamParam.mode          = segment->dataFormat;
            gStreamParam.baseAddress   = segment->targetAddress;
            gStreamParam.baseLength    = gBlockInfo.targetLength - (segment->targetAddress - gBlockInfo.targetAddress);

            /* Check result */
            if (kFblOk != ApplFblInitStreamOutput(&gStreamParam))
            {
               FBL_MEM_SET_STATUS(SegmentStartStreamOutInit, retVal); /* PRQA S 3109 */ /* MD_CBD_14.3 */
            }
# else
            /* Stream output not supported, refuse indication */
            FBL_MEM_SET_STATUS(SegmentStartStreamOutInit, retVal); /* PRQA S 3109 */ /* MD_CBD_14.3 */
# endif /* FBL_MEM_ENABLE_STREAM_OUTPUT */
         }
      }
#endif /* __ApplFblMemIsStreamOutputRequired */

#if defined( FBL_MEM_ENABLE_PROGRESS_INFO )
      /* Progress information for segment (without gap fill) */
      progressAddress   = segment->targetAddress;
      progressRemainder = segment->targetLength;
#endif /* FBL_MEM_ENABLE_PROGRESS_INFO */

#if defined( FBL_MEM_ENABLE_GAP_FILL )
      if (kFblMemStatus_Ok == retVal)
      {
         /* No gap fill required for volatile memory */
         if (kFblMemType_RAM != segment->type)
         {
            /* First segment of block? */
            if (0u == gSegInfo.nextIndex)
            {
               /* Fill from beginning of block */
               baseAddress = gBlockInfo.targetAddress;
               baseLength  = 0u;
            }
            else
            {
               /* Fill from end of previous segment */
               baseAddress = gBlockInfo.segmentList->segmentInfo[gSegInfo.ownIndex].targetAddress;
               baseLength  = gBlockInfo.segmentList->segmentInfo[gSegInfo.ownIndex].length;
            }

            /* Include padding applied to end of previous segment */
            baseLength += FblMemPadLength(baseAddress, baseLength);

            /* Segments in ascending order? */
            assertFblInternal((segment->targetAddress >= baseAddress), kFblMemAssertParameterOutOfRange); /* PRQA S 2214 */ /* MD_FblDef_Assert */
            assertFblInternal(((segment->targetAddress - baseAddress) >= baseLength), kFblMemAssertParameterOutOfRange); /* PRQA S 2214 */ /* MD_FblDef_Assert */

            /* Length till start of current segment
               Subtract length of previous segment (including padding) from offset between segments
               to prevent range overflow */
            gGapFillJob.used = (segment->targetAddress - baseAddress) - baseLength;

            /* Gap fill not necessary when segment starts directly after previous segment */
            if (gGapFillJob.used > 0u)
            {
               /* Setup gap fill */
               gGapFillJob.position    = 0u;
               /* Gap fill starts after end of previous segment */
               gGapFillJob.baseAddress = baseAddress + baseLength;

#if defined( FBL_MEM_ENABLE_PROGRESS_INFO )
               /* Include gap fill range in progress information */
               progressAddress    = gGapFillJob.baseAddress;
               progressRemainder += gGapFillJob.used;
#endif /* FBL_MEM_ENABLE_PROGRESS_INFO */

               /* Trigger gap fill */
               (void)FblMemQueueDefaultPrioInsert(gProcessingQueue, &gGapFillJob, gSegInfo.nextIndex);

            }
         }
      }
#endif /* FBL_MEM_ENABLE_GAP_FILL */

#if defined( FBL_MEM_ENABLE_PROGRESS_INFO )
      /* Setup programming progress (may include gap fill) */
      FblMemSetupProgress(kFblMemProgressType_Program, gBlockInfo.logicalAddress, gSegInfo.nextIndex,
         FBL_MEM_PROGRESS_INITIAL + FBL_MEM_PROGRESS_ERASE, FBL_MEM_PROGRESS_PROGRAM, progressRemainder);
      /* Adjust total percentages */
      FblMemOffsetProgress((progressAddress - gBlockInfo.targetAddress), gBlockInfo.targetLength);
      /* Report initial progress */
      FblMemUpdateProgress(progressRemainder);
#endif /* FBL_MEM_ENABLE_PROGRESS_INFO */
   }

   if (kFblMemStatus_Ok == retVal)
   {
      /* Copy input parameters to local variable */
      gSegInfo.input          = *segment;

      /* Initialize internal attributes */
      gSegInfo.writeAddress   = segment->targetAddress;
      gSegInfo.writeRemainder = 0u;
#if defined( FBL_ENABLE_PROCESSED_DATA_LENGTH )
      /* Available length from start of segment to end of block */
      gSegInfo.writeLength    = gBlockInfo.targetLength - (segment->targetAddress - gBlockInfo.targetAddress);
      gSegInfo.writtenLength  = 0u;
#else
      gSegInfo.writeLength    = segment->targetLength;
#endif /* FBL_ENABLE_PROCESSED_DATA_LENGTH */
#if defined( FBL_ENABLE_UNALIGNED_DATA_TRANSFER )
      /* Sliding input address required for remainder calculation */
      gSegInfo.inputAddress   = segment->targetAddress;
#endif /* FBL_ENABLE_UNALIGNED_DATA_TRANSFER */
#if defined( FBL_MEM_ENABLE_INPUT_LENGTH )
      /* Running input length required for early overflow detection */
      gSegInfo.inputLength    = segment->targetLength;
#endif /* FBL_MEM_ENABLE_INPUT_LENGTH */

      gSegInfo.ownIndex       = gSegInfo.nextIndex;
      gSegInfo.nextIndex++;

      /* Allow data indication */
      FblMemSetAllowed(FBL_MEM_ALLOWED_DATA_IND);
   }

   return retVal;
}

/***********************************************************************************************************************
 *  FblMemSegmentEndIndication
 **********************************************************************************************************************/
/*! \brief      Indicate end of current segment
 *  \details    Finish processing of all pending buffers, finalize data
 *              processing and program any remainder
 *  \pre        FblMemDataIndication executed before
 *  \param[out] writeLength Pointer to length actually written
 *  \return     Result of operation (potentially remapped to OEM specific NRC)
 *              May return error generated by background task operation
 **********************************************************************************************************************/
tFblMemStatus FblMemSegmentEndIndication( V_MEMRAM1 tFblLength V_MEMRAM2 V_MEMRAM3 * writeLength )
{
   tFblMemStatus retVal;
#if defined( FBL_MEM_ENABLE_REMAINDER_HANDLING_SINGLE_JOB ) || \
    defined( FBL_ENABLE_UNALIGNED_DATA_TRANSFER ) || \
    defined( FBL_MEM_ENABLE_SEGMENTED_INPUT_BUFFER )
   V_MEMRAM1 tFblMemJob V_MEMRAM2 V_MEMRAM3 * activeJob;
#endif /* FBL_MEM_ENABLE_REMAINDER_HANDLING_SINGLE_JOB || FBL_ENABLE_UNALIGNED_DATA_TRANSFER || FBL_MEM_ENABLE_SEGMENTED_INPUT_BUFFER */
#if defined( FBL_MEM_ENABLE_SEGMENT_HANDLING )
   V_MEMRAM1 tFblMemSegmentListEntry V_MEMRAM2 V_MEMRAM3 * activeSegment;
#endif /* FBL_MEM_ENABLE_SEGMENT_HANDLING */

   retVal = kFblMemStatus_Ok;

   /* Check allowed states */
   if (kFblOk != FblMemCheckAllowed(FBL_MEM_ALLOWED_SEGMENT_END, FBL_MEM_ALLOWED_DATA_IND | FBL_MEM_ALLOWED_SEGMENT_END))
   {
      FBL_MEM_SET_STATUS(SegmentEndSequence, retVal); /* PRQA S 3109 */ /* MD_CBD_14.3 */
   }
   else
   {
#if defined( FBL_MEM_ENABLE_PREAMBLE_HANDLING )
      /* Active fill buffer may change
         Store preamble so operation is transparent for caller */
      FblMemStorePreamble();
#endif /* FBL_MEM_ENABLE_PREAMBLE_HANDLING */

#if defined( FBL_MEM_ENABLE_REMAINDER_HANDLING_SINGLE_JOB )
      /* Active input job may be used as finalization trigger (flush remainder) */
      activeJob = FblMemGetPendingInputJob();
#endif /* FBL_MEM_ENABLE_REMAINDER_HANDLING_SINGLE_JOB */

      {
#if defined( FBL_ENABLE_DATA_PROCESSING )    || \
    defined( FBL_MEM_ENABLE_STREAM_OUTPUT )  || \
    defined( FBL_MEM_ENABLE_REMAINDER_HANDLING )
         if (kFblMemProgState_Error != fblMemProgState)
         {
# if defined( FBL_ENABLE_DATA_PROCESSING )
            /* Data processing active? */
            if (kFblOk == __ApplFblMemIsDataProcessingRequired(gSegInfo.input.dataFormat))
            {
               /* Do not remove finalize job until no data is produced anymore */
               gProcFinalizeJob.completion = kFblMemOperationMode_Finalize;

               /* No additional data in this segment, trigger finalization of data processing
                  Add to queue as some jobs may still be pending */
               (void)FblMemQueueDefaultPrioInsert(gProcessingQueue, &gProcFinalizeJob, gSegInfo.ownIndex);
            }
# endif /* FBL_ENABLE_DATA_PROCESSING */

# if defined( FBL_MEM_ENABLE_STREAM_OUTPUT )
            /* Stream output active? */
            if (kFblOk == __ApplFblMemIsStreamOutputRequired(gSegInfo.input.dataFormat))
            {
               /* Do not remove finalize job until no data is produced anymore */
               gStreamFinalizeJob.completion = kFblMemOperationMode_Finalize;

               /* No additional data in this segment, trigger finalization of stream output
                  Add to queue as some jobs may still be pending */
               (void)FblMemQueueDefaultPrioInsert(gProcessingQueue, &gStreamFinalizeJob, gSegInfo.ownIndex);
            }
            /* Skip write finalization */
            else
# endif /* FBL_MEM_ENABLE_STREAM_OUTPUT */
            {
# if defined( FBL_MEM_ENABLE_REMAINDER_HANDLING )
#  if defined( FBL_MEM_ENABLE_PROC_QUEUE )
               /* Insert dummy info into queue to trigger finalization of write */
               (void)FblMemQueueDefaultPrioInsert(gProcessingQueue, &gWriteFinalizeJob, gSegInfo.ownIndex);
#  else
               /* Enable switch combination equals FBL_MEM_ENABLE_REMAINDER_HANDLING_SINGLE_JOB */

               /* Re-use active input job as finalization trigger (flush remainder)
                  Change job type, all other members already have a consistent value */
               activeJob->type = kFblMemJobType_WriteFinalize;
#  endif /* FBL_MEM_ENABLE_PROC_QUEUE */
# endif /* FBL_MEM_ENABLE_REMAINDER_HANDLING */
            }

# if defined( FBL_MEM_ENABLE_PROC_QUEUE )
            if (!FblMemQueueIsEmpty(gProcessingQueue))
# endif /* FBL_MEM_ENABLE_PROC_QUEUE */
            {
               /* Continue operation */
               fblMemProgState = kFblMemProgState_Pending;
            }
         }
#endif /* FBL_ENABLE_DATA_PROCESSING || FBL_MEM_ENABLE_STREAM_OUTPUT || FBL_MEM_ENABLE_REMAINDER_HANDLING */
      }

#if defined( FBL_MEM_ENABLE_PROC_QUEUE ) || \
    defined( FBL_MEM_ENABLE_REMAINDER_HANDLING )
      /* Finish processing of all pending buffers */
      FblMemFlushQueueByPrio(kFblMemJobPrio_Write);

      /* Check for error condition */
      if (kFblMemProgState_Error == fblMemProgState)
      {
         /* Report error set by programming routines (previous or current) */
         retVal = gErrorStatus;
      }
      else
#endif /* FBL_MEM_ENABLE_PROC_QUEUE || FBL_MEM_ENABLE_REMAINDER_HANDLING */
      {
#if defined( FBL_MEM_ENABLE_REMAINDER_HANDLING_SINGLE_JOB )
         /* Remainder handling finished, restore original job type */
         activeJob->type = kFblMemJobType_InputWrite;
#endif /* FBL_MEM_ENABLE_REMAINDER_HANDLING_SINGLE_JOB */

#if defined( FBL_ENABLE_PROCESSED_DATA_LENGTH )
         /* Requested data should be completely provided at this point */
         if (0u != gSegInfo.inputLength)
#else
         /* Requested data should be completely programmed at this point */
         if (0u != gSegInfo.writeLength)
#endif /* FBL_ENABLE_PROCESSED_DATA_LENGTH */
         {
            FBL_MEM_SET_STATUS(SegmentEndInsufficientData, retVal);  /* PRQA S 3109 */ /* MD_CBD_14.3 */
         }
#if defined( __ApplFblMemPostSegmentEnd )
         else
         {
            /* Perform actions directly after segment end */
            if (kFblOk != __ApplFblMemPostSegmentEnd())
            {
               FBL_MEM_SET_STATUS(SegmentEndPost, retVal);   /* PRQA S 3109 */ /* MD_CBD_14.3 */
            }
         }
#endif /* __ApplFblMemPostSegmentEnd */

         /* Return written length */
#if defined( FBL_ENABLE_PROCESSED_DATA_LENGTH )
         *writeLength = gSegInfo.writtenLength;
#else
         *writeLength = gSegInfo.input.targetLength - gSegInfo.writeLength;
#endif /* FBL_ENABLE_PROCESSED_DATA_LENGTH */

#if defined( FBL_MEM_ENABLE_SEGMENT_HANDLING )
         /* Add segment to list */
         activeSegment = &(gBlockInfo.segmentList->segmentInfo[gBlockInfo.segmentList->nrOfSegments]);
         activeSegment->targetAddress        = gSegInfo.input.targetAddress;
         activeSegment->transferredAddress   = gSegInfo.input.logicalAddress;
         activeSegment->length               = *writeLength;
         gBlockInfo.segmentList->nrOfSegments++;
#endif /* FBL_MEM_ENABLE_SEGMENT_HANDLING */

#if defined( FBL_MEM_ENABLE_PROGRESS_INFO )
         FblMemConcludeProgress();
#endif /* FBL_MEM_ENABLE_PROGRESS_INFO*/
      }

#if defined( FBL_ENABLE_UNALIGNED_DATA_TRANSFER ) || \
    defined( FBL_MEM_ENABLE_SEGMENTED_INPUT_BUFFER )
      /* Restore default offset to align actual data to platform requirements */
      activeJob         = FblMemGetPendingInputJob();
      activeJob->offset = FBL_MEM_PREAMBLE_OFFSET(FBL_MEM_PREAMBLE_LENGTH);
#endif /* FBL_ENABLE_UNALIGNED_DATA_TRANSFER || FBL_MEM_ENABLE_SEGMENTED_INPUT_BUFFER */

#if defined( FBL_MEM_ENABLE_PREAMBLE_HANDLING )
      /*
         Active fill buffer may have changed
         Restore previously stored preamble so operation is transparent for caller
      */
      FblMemRestorePreamble();
#endif /* FBL_MEM_ENABLE_PREAMBLE_HANDLING */
   }

   /* Check result of final programming */
   if (kFblMemStatus_Ok == retVal)
   {
      /* Allow additional segment start or block end indication */
      FblMemSetAllowed(FBL_MEM_ALLOWED_SEGMENT_START | FBL_MEM_ALLOWED_BLOCK_END);
   }

   return retVal;
} /* PRQA S 4700 */ /* MD_FblMem_4700 */

/***********************************************************************************************************************
 *  FblMemDataIndication
 **********************************************************************************************************************/
/*! \brief      Indicate new chunk of input data
 *  \details    If pipelined programming is configured queue buffer and immediately return in case another input buffer
 *              is available
 *              Otherwise finish processing of one pending input buffer
 *              In non-pipelined mode indicated input buffer is directly processed
 *  \pre        FblMemSegmentStartIndication executed before, provided buffer
 *              and data offset equal the parameters of active input buffer
 *  \param[in]  buffer Pointer to input buffer (including preamble)
 *  \param[in]  offset Offset of actual data (after preamble)
 *  \param[in]  length Length of data (without preamble)
 *  \return     Result of operation (potentially remapped to OEM specific NRC)
 *              May return error generated by background task operation
 **********************************************************************************************************************/
tFblMemStatus FblMemDataIndication( tFblMemConstRamData buffer, tFblLength offset, tFblLength length )
{
   tFblMemStatus retVal;
#if defined( FBL_ENABLE_UNALIGNED_DATA_TRANSFER ) || \
    defined( FBL_MEM_ENABLE_SEGMENTED_INPUT_BUFFER )
   V_MEMRAM1 tFblMemJob V_MEMRAM2 V_MEMRAM3 * activeJob;
#endif /* FBL_ENABLE_UNALIGNED_DATA_TRANSFER || FBL_MEM_ENABLE_SEGMENTED_INPUT_BUFFER */

   retVal = kFblMemStatus_Ok;

   /* Check allowed states */
   if (kFblOk != FblMemCheckAllowed(FBL_MEM_ALLOWED_DATA_IND, FBL_MEM_ALLOWED_DATA_IND | FBL_MEM_ALLOWED_SEGMENT_END))
   {
      FBL_MEM_SET_STATUS(DataIndSequence, retVal); /* PRQA S 3109 */ /* MD_CBD_14.3 */
   }
   else
   {
#if defined( FBL_ENABLE_SYSTEM_CHECK ) || \
    defined( FBL_ENABLE_PROCESSED_DATA_LENGTH )
# if defined( FBL_ENABLE_PROCESSED_DATA_LENGTH )
      /* Unconditionally check input length */
      {
# else
#  if defined( FBL_ENABLE_DATA_PROCESSING )
      {
         /*
            Data processing active?
            Unprocessed data directly written through
         */
         if (kFblOk != __ApplFblMemIsDataProcessingRequired(gSegInfo.input.dataFormat))
#  else
      {
#  endif /* FBL_ENABLE_DATA_PROCESSING */
# endif /* FBL_ENABLE_PROCESSED_DATA_LENGTH */
         {
            /*
               Verify newly provided data does not exceed length requested
               in segment info
            */
# if defined( FBL_MEM_ENABLE_INPUT_LENGTH )
            if (gSegInfo.inputLength < length)
# else
            if (gSegInfo.writeLength < length)
# endif /* FBL_MEM_ENABLE_INPUT_LENGTH */
            {
               FBL_MEM_SET_STATUS(DataIndOverflow, retVal); /* PRQA S 3109 */ /* MD_CBD_14.3 */
            }
         }
      }
#endif /* FBL_ENABLE_SYSTEM_CHECK || FBL_ENABLE_PROCESSED_DATA_LENGTH */
   }

   if (kFblMemStatus_Ok == retVal)
   {
#if defined( FBL_MEM_ENABLE_PREAMBLE_HANDLING )
      /*
         Active fill buffer may change
         Store preamble so operation is transparent for caller
      */
      FblMemStorePreamble();
#endif /* FBL_MEM_ENABLE_PREAMBLE_HANDLING */

      /* Queue the active buffer for further processing */
      retVal = FblMemQueueBuffer(buffer, offset, length);
      if (kFblMemStatus_Ok == retVal)
      {
#if defined( FBL_MEM_ENABLE_INPUT_LENGTH )
         /* Keep track of expected input data length */
         gSegInfo.inputLength -= length;
#endif /* FBL_MEM_ENABLE_INPUT_LENGTH */

#if defined( FBL_MEM_ENABLE_INPUT_DATA_FLUSH )
         {
            /* Directly process all input data */
            FblMemFlushQueueByPrio(kFblMemJobPrio_Write);
         }

         /* Check for programming error */
         if (kFblMemProgState_Error == fblMemProgState)
         {
            retVal = gErrorStatus;
         }
         else
#endif /* FBL_MEM_ENABLE_INPUT_DATA_FLUSH */
         {
#if defined( FBL_ENABLE_UNALIGNED_DATA_TRANSFER ) || \
    defined( FBL_MEM_ENABLE_SEGMENTED_INPUT_BUFFER )
            /* Restore default offset to align actual data to platform requirements */
            activeJob         = FblMemGetPendingInputJob();
            activeJob->offset = FBL_MEM_PREAMBLE_OFFSET(FBL_MEM_PREAMBLE_LENGTH);

# if defined( FBL_ENABLE_UNALIGNED_DATA_TRANSFER )
            /* No remainder handling required for volatile memory */
            if (kFblMemType_RAM != gSegInfo.input.type)
            {
               /* Align active input buffer to current remainder */
               activeJob->offset += FblMemGetWriteRemainder(gSegInfo.inputAddress, length);

               /* Update sliding input address */
               gSegInfo.inputAddress += length;
            }
# endif /* FBL_ENABLE_UNALIGNED_DATA_TRANSFER */
#endif /* FBL_ENABLE_UNALIGNED_DATA_TRANSFER || FBL_MEM_ENABLE_SEGMENTED_INPUT_BUFFER */
         }
      }

#if defined( FBL_MEM_ENABLE_PREAMBLE_HANDLING )
      /*
         Active fill buffer may have changed
         Restore previously stored preamble so operation is transparent for caller
      */
      FblMemRestorePreamble();
#endif /* FBL_MEM_ENABLE_PREAMBLE_HANDLING */
   }

   if (kFblMemStatus_Ok == retVal)
   {
      /* Allow additional data or segment end indication */
      FblMemSetAllowed(FBL_MEM_ALLOWED_DATA_IND | FBL_MEM_ALLOWED_SEGMENT_END);
   }

   return retVal;
}

/***********************************************************************************************************************
 *  FblMemTask
 **********************************************************************************************************************/
/*! \brief    Perform background tasks
 *  \details  If pipelined programming is configured pending buffers are processed until the operation is suspended by
 *            a Rx notification
 *  \pre      FblMemInitPowerOn executed before
 **********************************************************************************************************************/
void FblMemTask( void )
{
#if defined( FBL_MEM_ENABLE_PIPELINING )
   switch (fblMemProgState)
   {
      /* Operation pending */
      case kFblMemProgState_Pending:
      {
         /* Switch to background context
          * Affects watchdog and suspend handling*/
         gProgContext = kFblMemContext_Background;

         /*
            Loop while processing isn't paused by external event (e.g. data reception)
            and buffers are pending
         */
         while (kFblMemProgState_Pending == fblMemProgState)
         {
            /* Execute processing cycle */
            FblMemProcessQueue(kFblMemOperationMode_Normal);
         }

         /* Suspend request received during processing */
         if (kFblMemProgState_SuspendPending == fblMemProgState)
         {
            fblMemProgState = kFblMemProgState_Suspended;
         }

         /* Switch back (foreground) service context
          * Affects watchdog and suspend handling*/
         gProgContext = kFblMemContext_Service;

         break;
      }
      /* No operation pending */
      case kFblMemProgState_Idle:
      case kFblMemProgState_Error:
      /* Operation suspended by external event */
      case kFblMemProgState_Suspended:
      case kFblMemProgState_SuspendPending:
      /* Operation suspended by internal checkpoint event */
      case kFblMemProgState_Checkpoint:
      /* Unknown state value */
      default:
      {
         /* Nothing to do */
         break;
      }
   }
#endif /* FBL_MEM_ENABLE_PIPELINING */
}

/***********************************************************************************************************************
 *  FblMemResumeIndication
 **********************************************************************************************************************/
/*! \brief    Resume suspended operation
 *  \details  If pipelined programming is configured a pending operation which was suspended earlier through a RX
 *            notification will be resumed
 *  \pre      FblMemInitPowerOn executed before
 **********************************************************************************************************************/
void FblMemResumeIndication( void )
{
#if defined( FBL_MEM_ENABLE_PIPELINING )
   /* Resume any pending operations */
   if (FblMemTaskIsPending())
   {
      /* Continue operation on next call cycle of task
         Note: fblMemProgState may already be pending and isn't necessarily suspended */
      fblMemProgState = kFblMemProgState_Pending;
   }
#endif /* FBL_MEM_ENABLE_PIPELINING */
}

#if defined( FBL_MEM_ENABLE_PIPELINING )

#  define FBLLIB_MEM_RAMCODE_START_SEC_CODE
#  include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/***********************************************************************************************************************
 *  FblMemRxNotification
 **********************************************************************************************************************/
/*! \brief    Suspend any pending programming operation
 *  \details  If memory driver supports pausing of active operation call respective user callback too
 *  \pre      FblMemInitPowerOn executed before
 **********************************************************************************************************************/
void FblMemRxNotification( void )
{
   /*
      Notification may be executed in interrupt context
      Critical section secures access to programming state
   */
   __ApplFblMemEnterCriticalSection();

   /*
      Actions only necessary if programming is pending
      Condition equals ((fblMemProgState == kFblMemProgState_Pending) || (fblMemProgState == kFblMemProgState_Checkpoint))
   */
   if (fblMemProgState >= kFblMemProgState_Checkpoint)
   {
      /* Suspend programming to handle received request */
      fblMemProgState = kFblMemProgState_SuspendPending;

   }

   /* Leave critical section */
   __ApplFblMemLeaveCriticalSection();
}

#  define FBLLIB_MEM_RAMCODE_STOP_SEC_CODE
#  include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* FBL_MEM_ENABLE_PIPELINING */

/***********************************************************************************************************************
 *  FblMemEraseRegion
 **********************************************************************************************************************/
/*! \brief      Performs erase operation in non-volatile memory
 *  \details    All memory segments fully or partially covered by given region are affected. Gaps in the memory segment
 *              definition are skipped.
 *  \pre        Memory driver initialized
 *  \param[in]  eraseAddress Start address of erase region
 *  \param[in]  eraseLength Length of erase region
 *  \return     Result of operation (potentially remapped to OEM specific NRC)
 **********************************************************************************************************************/
tFblMemStatus FblMemEraseRegion( tFblAddress eraseAddress, tFblLength eraseLength ) /* PRQA S 1505 */ /* MD_FblMem_1505 */
{
   tFblMemStatus  retVal;

#if defined( FBL_MEM_ENABLE_PROGRESS_INFO )
   /* Disable progress information */
   gProgressState = kFblMemProgressState_Disabled;
#endif /* FBL_MEM_ENABLE_PROGRESS_INFO */

   retVal = FblMemEraseRegionInternal(eraseAddress, eraseLength);

   return retVal;
}  /* PRQA S 2006 */ /* MD_CBD_14.7 */

/***********************************************************************************************************************
 *  FblMemProgramBuffer
 **********************************************************************************************************************/
/*! \brief         Performs program operation to non-volatile memory
 *  \details       If the length is not aligned to the segment size the odd bytes are padded with the configured fill
 *                 character.
 *                 Programming may be suspended by an external event. In this case parameter programLength will be
 *                 updated to reflect the length actually programmed
 *  \pre           Memory driver initialized, address aligned to memory segment size
 *  \param[in]     programAddress Program address
 *  \param[in,out] programLength Length of data (output: length actually programmed)
 *  \param[in,out] programData Pointer to program data (contents are padded in case length is not aligned to memory
 *                  segment size!)
 *  \return        Result of operation (potentially remapped to OEM specific NRC)
 **********************************************************************************************************************/
tFblMemStatus FblMemProgramBuffer( tFblAddress programAddress, /* PRQA S 1505 */ /* MD_FblMem_1505 */
   V_MEMRAM1 tFblLength V_MEMRAM2 V_MEMRAM3 * programLength, tFblMemRamData programData )
{
   tFblMemProgState activeProgState;
   tFblMemStatus retVal;

   {
      /* Remember active programming state */
      activeProgState = fblMemProgState;

      /* Operation potentially paused during previous execution cycle */
      fblMemProgState   = kFblMemProgState_Pending;

      /* Perform actual programming */
      retVal = FblMemProgramBufferInternal(programAddress, programLength, programData, kFblMemProgState_Pending);

      /* Restore previous programming state */
      fblMemProgState = activeProgState;
   }

   return retVal;
}

/***********************************************************************************************************************
 *  FblMemSetInteger
 **********************************************************************************************************************/
/*! \brief      Convert given integer value to big-endian byte array
 *  \param[in]  count Number of relevant bytes
 *  \param[in]  input Input value
 *  \param[out] buffer Pointer to output buffer
 **********************************************************************************************************************/
void FblMemSetInteger( vuintx count, vuint32 input, tFblMemRamData buffer ) /* PRQA S 1505 */ /* MD_FblMem_1505 */
{
   /* Loop relevant bytes */
   while (count > 0u)
   {
      count--;
      /* Store most significant byte first */
      buffer[count] = (vuint8)(input & 0xFFu);
      /* Shift in next byte */
      input >>= 8u;
   }
}

/***********************************************************************************************************************
 *  FblMemGetInteger
 **********************************************************************************************************************/
/*! \brief      Convert given big-endian byte array to integer value
 *  \param[in]  count Number of relevant bytes
 *  \param[in]  buffer Pointer to input buffer
 *  \return     Integer value
 **********************************************************************************************************************/
vuint32 FblMemGetInteger( vuintx count, tFblMemConstRamData buffer )
{
   vuint32 output;
   vuintx idx;

   output = 0u;
   idx    = 0u;

   /* Loop relevant bytes */
   while (count > 0u)
   {
      /* Most significant byte first */
      output <<= 8u;
      /* Add current byte */
      output |= (vuint32)buffer[idx];

      idx++;
      count--;
   }

   return output;
}

/*****************************************************************************/

/* module specific MISRA deviations:

   MD_FblMem_0306:
     Reason: Address conversion between integer values and pointers is required to allow for hardware independent
      configuration and address range checks.
     Risk: The size of integer required to hold the result of a pointer cast is implementation defined.
     Prevention: The size of the respective integer data type which holds the address value is adapted on a hardware
      specific basis.

   MD_FblMem_0310:
     Reason: Snapshot stored in byte buffer. This way caller doesn't need to know specific structure type.
     Risk: Byte buffer size or alignment not suitable for direct access through structure pointer.
     Prevention: Offset calculated to align pointer to platform requirements.
      Effective size after alignment checked against actual structure requirements.

   MD_FblMem_0342:
     Reason: Usage of glue operator unifies naming scheme, without relying on massive use of copy&paste.
      Additionally support for different use-cases (user defined error codes, enumerated error codes) can be easily
      achieved.
     Risk: Not supported by outdated compilers.
     Prevention: No prevention required.

   MD_FblMem_0488:
     Reason: Calling instance passes an arbitrary buffer pointer. Relative offset to internal buffer has to be
      calculated.
     Risk: 1) Range overflow caused by subtraction.
      2) Accessing memory outside of allocated buffer.
     Prevention: Input and internal pointer have exact same type
      1) Input pointer compared against internal buffer before subtraction.
      2) Pointer arithmetic not used to access data directly. Instead calculated offset and input length are verified
         against constraints given by internal buffer. Actual data accesses use array indexing.

   MD_FblMem_0723:
     Reason: Enumeration property of incrementing entries used to generate job priorities. Certain job types should
      have identical priorities though.
     Risk: Code is difficult to understand and maintain.
     Prevention: All changes to the affected code parts have to be reviewed by the module responsible.

   MD_FblMem_1504:
     Reason: Some exported global variables may not be used in the some configurations, but may be of interest for
      other use-cases.
     Risk: No identifiable risk.
     Prevention: No prevention required.

   MD_FblMem_1505:
     Reason: Some exported functionality isn't used in the default configuration, but may be of interest for special
      use-cases.
     Risk: No identifiable risk.
     Prevention: No prevention required.

   MD_FblMem_3112:
     Reason: Dummy statements have been added to avoid compiler warnings because of unused variables.
     Risk: Unused statements add to program complexity just by being there - adding lines to the program and causing
      readers to puzzle over its presence.
     Prevention: Dummy statements can be turned off externally (V_DISABLE_USE_DUMMY_STATEMENT).

   MD_FblMem_3198:
      Reason: Depending on configuration different paths may be executed. Initialization makes sure no invalid variable
       contents are used accidentally.
     Risk: No identifiable risk.
     Prevention: No prevention required.

   MD_FblMem_3205:
     Reason: Only enumeration values used in integer context, but enumeration type never referenced directly.
     Risk: No identifiable risk.
     Prevention: No prevention required.

   MD_FblMem_3218:
     Reason: The local data buffers of this module are kept at a central location for a better overview and maintenance.
     Risk: Scope is larger than required (whole file instead of one function). Some other function could access
      the variable.
     Prevention: Restrict the functionality in this module to the intended purpose. Don't add functions which shall not
      be able to access the local data buffers.

   MD_FblMem_3305:
     Reason: Snapshot stored in byte buffer. This way caller doesn't need to know specific structure type.
     Risk: Byte buffer alignment not suitable for direct access through structure pointer.
     Prevention: Offset calculated to align pointer to platform requirements.

   MD_FblMem_3415:
     Reason: If condition relies on lazy evaluation, second argument only executed if ROM download is active.
     Risk: No identifiable risk.
     Prevention: No prevention required.

   MD_FblMem_3673:
     Reason: Depending on configuration the parameter is actually modified inside the calling hierarchy.
     Risk: No identifiable risk.
     Prevention: No prevention required.

   MD_FblMem_4700:
     Reason: Complexity of function depending on enabled feature set. Splitting functionality into multiple functions
      would result in duplication of code.
     Risk: Code is difficult to understand and maintain.
     Prevention: All changes to the affected code parts have to be reviewed by the module responsible.

   MD_FblMem_3412:
     Reason: Several similar structs with varying members depending on configuration are required.
     Risk: Code is difficult to understand, but easier to maintain.
     Prevention: No prevention required.

   MD_FblMem_0881:
     Reason: Order is irrelevant, because resulting tokens are independent.
     Risk: No risk
     Prevention: No prevention required.

   MD_FblMem_60xx:
     Reason: Monolithic functions chosen in order to save code size
     Risk: Complexity decreases readability and maintainability
     Prevention: Catch complexity through (regression) testing

   MD_FblMem_0779:
     Reason: This is following the general style of the status definitions. Only one member deviates to 0779.
             This is accepted as deviation
     Risk: SegmentStartVerifyInit and SegmentStartVerifyCompute could be confused
     Prevention: No prevention.

   MD_FblMem_AssertJobMax:
     Reason: For certain configurations this check is always false. However, this check can only be done at runtime.
     Risk: No risk.
     Prevention: No prevention required. (Note: assertions are disabled in production software).
*/

/***********************************************************************************************************************
 *  END OF FILE: FBL_MEM.C
 **********************************************************************************************************************/
