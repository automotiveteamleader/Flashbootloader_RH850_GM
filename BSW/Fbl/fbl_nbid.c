/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file          fbl_nbid.c
 *  \brief         Basic NBID handling module. This module allows restricted amount of updates for NBID elements
 *                 like specified in GB6002 specification (Configurable minimum updates, minimum of specified 16 times
 *                 is guaranteed). Please consider to replace this solution with a non-restricted update NVM solution
 *                 like a Fee. Please check Technical Reference for more information.
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
 *  Andreas Wenckebach            AWh           Vector Informatik GmbH
 *  Sebastian Loos                Shs           Vector Informatik GmbH
 *  Thomas Mueller                ThM           Vector Informatik GmbH
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id       Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  02.06.00   2016-02-19  AWh     ESCAN00088419   Add basic NBID handling
 *  02.06.01   2016-03-18  AWh     ESCAN00088818   No changes
 *                                 ESCAN00088973   Set NBID readability helper markers only if possible
 *                                 ESCAN00088984   No changes
 *  02.07.00   2016-03-24  Shs     ESCAN00089073   No changes
 *  02.07.01   2016-04-04  ThM     ESCAN00089202   No changes
 *                         AWh     ESCAN00089235   NBID is not correctly written and bootloader stops update sequence
 *  03.00.00   2016-04-08  AWh     ESCAN00089532   No changes
 *                                 ESCAN00089628   Buffer access to memDrvLst out of bounds
 *  03.01.00   2016-08-04  HRf     ESCAN00091043   No changes
 *                         AWh     ESCAN00091147   No changes
 *                                 ESCAN00091325   No changes
 *                                 ESCAN00091279   Basic NVM NBID FlashDriver Init too late
 *                                 ESCAN00089426   No changes
 *  03.01.01   2016-10-04  AWh     ESCAN00092167   No changes
 *  03.02.00   2016-10-31  ThM     ESCAN00092596   No changes
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

#include "fbl_inc.h"

#if defined( FBL_HDR_ENABLE_BASIC_NVM_HANDLING )
# if defined( FBL_NBID_USER_DRIVER )
  /* Please use this configuration, if the driver you want to use is/should not be configured to FlashBlock table.
   * You have to define these macros externally to your user drivers properties:
   * - FblNbid_RWriteSync
   * - FblNbid_RReadSync
   * - FBL_NBID_DELETED
   * - FBL_NBID_SEGMENT_SIZE
   *   (There is more information on what to define in .h - file of this module)
   *
   * Note that the bootloader is not allowed to access content of this drivers memory during programming.
   */
# else
  /* This is the standard configuration, applicable when used driver is configured to FlashBlock table. */
#  define FblNbid_RWriteSync MemDriver_RWriteSync
#  define FblNbid_RReadSync  MemDriver_RReadSync

#  if defined( FBL_ENABLE_MULTIPLE_MEM_DEVICES )
/***********************************************************************************************************************
 *  DEFINES: Optional Reconfiguration items in case of FBL_ENABLE_MULTIPLE_MEM_DEVICES config
 **********************************************************************************************************************/

/* It might be necessary to change the deleted value for the memory on which the Basic NVM operates, per default
 * it is the internal flash and nothing has to be configured. */
#   if !defined( FBL_NBID_DELETED )
#    define FBL_NBID_DELETED FBL_FLASH_DELETED
#   endif
/* Usually internal Flashdriver is used. This has to be overwritten, if another driver than internal flash is used */
#   if !defined( FBL_NBID_SEGMENT_SIZE )
#    define FBL_NBID_SEGMENT_SIZE FLASH_SEGMENT_SIZE
#   endif

/* It might be necessary to change the device (index to memDrvLst) value for the memory on which the Basic NVM operates, per default
 * it is the internal flash and nothing has to be configured. */
#   if !defined( kNbidDevice )
#    define kNbidDevice kMioDeviceFlash
#   endif
#  else
#   define FBL_NBID_DELETED FBL_FLASH_DELETED
#   define FBL_NBID_SEGMENT_SIZE FLASH_SEGMENT_SIZE
#   define kNbidDevice kMioDeviceFlash
#  endif /* FBL_ENABLE_MULTIPLE_MEM_DEVICES */
# endif /* FBL_NBID_USER_DRIVER */

/* Reconfigure if you want to enlarge/reduce the minimum possible updates.
 * The GM required minimum is 16, however it is recommended to use the largest value possible.
 * Below checks will guarantee a minimum of 64 (safety margin to 16) as long as you do not overwrite
 * FBL_NBID_MIN_UPDATE_COUNT
 */
# if !defined( FBL_NBID_MIN_UPDATE_COUNT )
#  define FBL_NBID_MIN_UPDATE_COUNT  0x40u
# endif

/* Reconfigure to optimize memory usage in Multi Processor configurations
 * (Only required if Logical Block Table contains non Appl-related entries)
 * The number of applications is <= FBL_MTAB_NO_OF_BLOCKS
 * In order to safe memory you may overwrite FBL_NBID_APP_COUNT if you have
 * less application modules than FBL_MTAB_NO_OF_BLOCKS.
 */
# if !defined( FBL_NBID_APP_COUNT )
#  define FBL_NBID_APP_COUNT FBL_MTAB_NO_OF_BLOCKS
# endif

/***********************************************************************************************************************
 *  DEFINES:
 **********************************************************************************************************************/

# define FBL_NBID_ELEMENT_COUNT                (FBL_NBID_APP_COUNT + 1u)
/* Get length aligned to Segment size. Recommendation is to configure FBL_NBID_TOTAL_MEMORY_LEN
 * value that will not leave unusable bytes.  */
# define FBL_NBID_ELEMENT_MEMORY_LEN           ((((FBL_NBID_TOTAL_MEMORY_LEN / FBL_NBID_ELEMENT_COUNT)\
                                                   - FBL_NBID_SEGMENT_SIZE) | (FBL_NBID_SEGMENT_SIZE - 1u)) + 1u)
# define FBL_NBID_REAL_UPDATE_COUNT            (FBL_NBID_ELEMENT_MEMORY_LEN / FBL_NBID_SEGMENT_SIZE)
# define FblGetSpecificMemoryBase(nbidNumber)  (FBL_NBID_MEMORY_BASE + (FBL_NBID_ELEMENT_MEMORY_LEN * (nbidNumber))) /* PRQA S 3453 */ /* MD_MSR_19.7 */

# define FBL_NBID_FILL_PATTERN                 kFillChar

/***********************************************************************************************************************
 *  LOCAL DATA
 **********************************************************************************************************************/

static struct {
   V_MEMRAM1 vuint32 V_MEMRAM2 dummy; /* ensure 32-bit alignment */
   V_MEMRAM1 vuint8 V_MEMRAM2 buffer[FBL_NBID_SEGMENT_SIZE];
} nbidMarker;

/***********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
static IO_ErrorType FblNbidInternalRead( IO_PositionType readPosition );

/***********************************************************************************************************************
 *  FblNbidInternalRead
 **********************************************************************************************************************/
/*! \brief       Wrap HIS Read function to get required read result values on one flash cell.
 *  \detail      This function will always use IO_E_ERASED return value, when erased state is detected.
 *  \param[in]   readPosition - start address of the flash cell to read.
 *  \return      Reports read success or error code (IO_E_ERASED, IO_E_OK or IO_E_NOT_OK).
 *               IO_E_ERASED will be set in any case the flash cell is found to be erased (see comment in function).
 ***********************************************************************************************************************/
static IO_ErrorType FblNbidInternalRead( IO_PositionType readPosition )
{
   vuint16 i;
   IO_ErrorType readResult;

#if !defined( FBL_NBID_USER_DRIVER )
   /* Set memSegment before accessing FblNbid_RReadSync */
   memSegment = kNbidDevice;
#endif

   readResult = FblNbid_RReadSync(nbidMarker.buffer, FBL_NBID_SEGMENT_SIZE, readPosition);
   if (IO_E_OK == readResult )
   {
      /* For NBID handling, we want to always see IO_E_ERASED return value if a cell is erased.
       * Note that Vector HIS drivers only explicitly set IO_E_ERASED in case the erased value is ECC affected.
       * Therefore we overwrite IO_E_OK in case we find all bytes in the cell have the erased value by IO_E_ERASED. */
      for(i = 0u; i < FBL_NBID_SEGMENT_SIZE; i++)
      {
         if (nbidMarker.buffer[i] != FBL_NBID_DELETED )
         {
            break;
         }
      }

      if (i == FBL_NBID_SEGMENT_SIZE)
      {
         readResult = IO_E_ERASED;
      }
   }
   return readResult;
}

/***********************************************************************************************************************
 *  FblNbidInit
 **********************************************************************************************************************/
/*! \brief       Initialization of this module
 **********************************************************************************************************************/
void FblNbidInit( void )
{
# if defined( FBL_ENABLE_ASSERTION )
   IO_PositionType i;
   IO_ErrorType readResult;

   if (FblCheckBootVectTableIsValid())
   {
      /* Before any application download, all memory assigned to basic NVM need to be erased. */
      for(i = FBL_NBID_MEMORY_BASE; i< (FBL_NBID_MEMORY_BASE + FBL_NBID_TOTAL_MEMORY_LEN); i += FBL_NBID_SEGMENT_SIZE)
      {
         readResult = FblNbidInternalRead( i );
         /* Check for invalid condition: End marker not found */
         if (IO_E_ERASED != readResult )
         {
            /* Set any */
            ApplFblFatalError(kFblAssertConditionCorrect);
         }
      }
   }
# endif

   /* Memory should be configured to "protected" area, not generated to FlashBlock table. */
   assertFblUser(0 >= FblMemSegmentNrGet(FBL_NBID_MEMORY_BASE), kFblSysAssertInitializationFailed);  /* PRQA S 2214 */ /* MD_FblAssert_Assertion */
}

/***********************************************************************************************************************
 *  FblNbidIncrement
 **********************************************************************************************************************/
/*! \brief       Increment stored NBID value of elementId up to passed in value.
 *  \detail      This is only possible up to FBL_NBID_REAL_UPDATE_COUNT.
 *  \param[in]   elemtId - Identifier for element to increment
 *  \param[in]   *nbidValue - New value that shall be set
 *  \return      Success of incrementing NBID to desired value (kFblOk/kFblFailed)
 *                 kFblOk: Either increment to desired value successful or FBL_NBID_REAL_UPDATE_COUNT reached
 *                 kFblFailed: FBL_NBID_REAL_UPDATE_COUNT not yet reached and increment not successful
 *                             (e.g. nbidValue smaller than current stored value, write operation not successfully
 *                              removing erased state of flash cell.)
 ************************************************************************************************************************/
tFblResult FblNbidIncrement( vuintx elementId, const V_MEMRAM1 vuint16 V_MEMRAM2 V_MEMRAM3 * nbidValue )
{
   vuint16 i;
   vuint16 currNbid;
   vuint16 inNbidVal = *nbidValue;
   tFblResult retVal = FblNbidReadNbid(elementId, &currNbid);

   if (kFblOk == retVal)
   {
      if (inNbidVal > FBL_NBID_REAL_UPDATE_COUNT)
      {
         /* NBID value cannot be updated beyond FBL_NBID_REAL_UPDATE_COUNT.
          * Increment to maximum possible value. Not that this is no error state. */
         inNbidVal = FBL_NBID_REAL_UPDATE_COUNT;
      }

      /* Fill buffer with fill value, overwrite with pattern at end to increase manual readability during debugging. */
      for(i = 0u; i < FBL_NBID_SEGMENT_SIZE; i++)
      {
         nbidMarker.buffer[i] = FBL_NBID_FILL_PATTERN;
      }
#if ( FBL_NBID_SEGMENT_SIZE > 2 )
      nbidMarker.buffer[FBL_NBID_SEGMENT_SIZE - 2u] = FblInvert8Bit(elementId);
#endif
#if ( FBL_NBID_SEGMENT_SIZE > 1 )
      nbidMarker.buffer[FBL_NBID_SEGMENT_SIZE - 1u] = (vuint8) elementId;
#endif

      while (currNbid < inNbidVal)
      {
#if !defined( FBL_NBID_USER_DRIVER )
         /* Set memSegment before accessing FblNbid_RWriteSync */
          memSegment = kNbidDevice;
#endif

         /* Ignore the result of the Write Operation explicitly. */
         (void) FblNbid_RWriteSync((IO_MemPtrType) nbidMarker.buffer, (IO_SizeType) FBL_NBID_SEGMENT_SIZE,
                                      FblGetSpecificMemoryBase(elementId) + (currNbid * FBL_NBID_SEGMENT_SIZE) );
         currNbid++;
      }

      (void) FblNbidReadNbid(elementId, &currNbid);
      if (currNbid != inNbidVal)
      {
         retVal = kFblFailed;
      }
      else
      {
         retVal = kFblOk;
      }
   }

   return retVal;
}

/***********************************************************************************************************************
 *  FblNbidReadNbid
 **********************************************************************************************************************/
/*! \brief       Read current NBID value for given NBID elementId
 *  \param[in]   elementId - Id of element to read NBID value
 *  \param[out]   *nbidValue - Read NBID value
 *  \return      kFblFailed, if invalid elementId
 *               kFblOk else
  **********************************************************************************************************************/
tFblResult FblNbidReadNbid( vuintx elementId, V_MEMRAM1 vuint16 V_MEMRAM2 V_MEMRAM3 * nbidValue )
{
   tFblResult retVal;

#if !defined( FBL_NBID_USER_DRIVER )
   /* Set memSegment before accessing MemDriver_SegmentSize */
   memSegment = kNbidDevice;

   /* Assertion may help to find early, that FBL_NBID_SEGMENT_SIZE is not adequately chosen.
    * However, also on disabled assertion, this shall lead to an error. */
   assertFblUser((MemDriver_SegmentSize == FBL_NBID_SEGMENT_SIZE), kFblAssertInternalIlegalState);  /* PRQA S 2214 */ /* MD_FblAssert_Assertion */ /* PRQA S 3201, 3325 */ /* MD_FblNbid_3201_3325 */
#endif

   if (   (FBL_NBID_ELEMENT_COUNT <= elementId)
#if !defined( FBL_NBID_USER_DRIVER )
         || (MemDriver_SegmentSize != FBL_NBID_SEGMENT_SIZE)
#endif
      )
   {
      retVal = kFblFailed;
   }
   else
   {
      IO_ErrorType readResult = IO_E_OK;
      tFblAddress elementBase = FblGetSpecificMemoryBase(elementId);
      *nbidValue = 0u;

      /* Stop incrementing *nbidValue at first erased Flash cell, or at last element */
      while ( (IO_E_ERASED != readResult) && (*nbidValue < FBL_NBID_REAL_UPDATE_COUNT) )
      {
         readResult = FblNbidInternalRead((IO_PositionType) elementBase + (*nbidValue * FBL_NBID_SEGMENT_SIZE));

         if (IO_E_ERASED != readResult )
         {
            (*nbidValue)++;
         }
      }
      retVal = kFblOk;
   }

   return retVal;
}

/***********************************************************************************************************************
 *  Configuration Checks
 **********************************************************************************************************************/
# if ( FBL_NBID_MIN_UPDATE_COUNT < 16 )
#  error "GM specifies that the minimum allowed possible updates shall be 16."
# endif
# if ( FBL_NBID_REAL_UPDATE_COUNT < FBL_NBID_MIN_UPDATE_COUNT )
#  error "Reserved FBL_NBID_TOTAL_MEMORY_LEN is not large enough to allow for FBL_NBID_MIN_UPDATE_COUNT!"
# endif
# if !defined( FBL_NBID_MEMORY_BASE ) || !defined( FBL_NBID_TOTAL_MEMORY_LEN )
#  error "Configure FBL_NBID_MEMORY_BASE and FBL_NBID_TOTAL_MEMORY_LEN to allow usage of fbl_nbid.c/.h module"
#  error "Configure them e.g. in GENy user configuration."
# endif
#endif /* defined( FBL_HDR_ENABLE_BASIC_NVM_HANDLING ) */

/* module specific MISRA deviations:

   MD_FblNbid_3201_3325:
     Reason: Configuration dependent, this check can trigger a condition that can never be false.
     Risk: No risk
     Prevention: No prevention

*/

/***********************************************************************************************************************
 *  END OF FILE: FBL_NBID.C
 **********************************************************************************************************************/

