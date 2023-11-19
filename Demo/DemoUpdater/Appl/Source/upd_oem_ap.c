/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief         FBL updater callout routines - GM specific
 *
 *  \note          Please note, that this file contains a collection of callback functions to be used with the
 *                 FBL Updater. These functions may influence the behavior of the updater in principle.
 *                 Therefore, great care must be taken to verify the correctness of the implementation.
 *                 The contents of the originally delivered files are only examples resp. implementation proposals.
 *                 With regard to the fact that these functions are meant for demonstration purposes only, Vector
 *                 Informatik's liability shall be expressly excluded in cases of ordinary negligence, to the extent
 *                 admissible by law or statute.
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
 *  Sebastian Loos                Shs           Vector Informatik GmbH
 *  Andreas Wenckebach            AWh           Vector Informatik GmbH
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  01.00.00   2016-07-27  Shs     -                Initial release
 *  01.00.01   2016-09-26  AWh     ESCAN00092036    Updater will erase parts of itself
 *  01.00.02   2016-12-07  Shs     ESCAN00092943    CFG5: Include Fbl_Lbt.h instead of fbl_mtab.h 
 *                                 ESCAN00093194    No changes
 **********************************************************************************************************************/

#define UPD_AP_OEM

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

#include "upd_oem_ap.h"
#include "upd_main.h"
#include "flashdrv.h"
#include "iotypes.h"
#include "fbl_flio.h"
#include "fbl_mio.h"
#if defined( VGEN_GENY )
# include "fbl_mtab.h"
#endif
#if defined( V_GEN_GENERATOR_MSR )
# include "Fbl_Lbt.h"
#endif
#include "fbl_wd.h"

/***********************************************************************************************************************
 *  VERSION
 **********************************************************************************************************************/

#if ( FBLUPD_AP_GM_VERSION != 0x0100u ) || \
    ( FBLUPD_AP_GM_RELEASE_VERSION != 0x02u )
# error "Error in upd_oem.c: Source and Header file are inconsistent!"
#endif

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/
/* stub away fbl_apnv */
#define kFblPresencePatternSize    2u
#define kFblPresencePattern        {0xA5u, 0x96u}

/* TODO by customer: Please ensure that the correct setting for FBL_PP_SEGMENT_SIZE is chosen.
 * Please compare this value to the value used in your Bootloader project.
 * This is especially important if multiple memory devices with different segemnt sizes are used
 */
#define FBL_PP_SEGMENT_SIZE FBL_MAX_SEGMENT_SIZE

/***********************************************************************************************************************
 *  TYPEDEFS
 **********************************************************************************************************************/

/** Presence pattern buffer with alignment to platform requirements */
typedef struct
{
#if defined( C_CPUTYPE_32BIT ) || \
    defined( C_CPUTYPE_16BIT )
   vuint32  dummy;
#endif /* C_CPUTYPE_32BIT || C_CPUTYPE_16BIT */
   vuint8   data[FBL_PP_SEGMENT_SIZE];
} tFblpresPtnAlignedBuffer;
/***********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL DATA
 **********************************************************************************************************************/
V_MEMROM0 static V_MEMROM1 vuint8 V_MEMROM2 blockPresencePattern[kFblPresencePatternSize] = kFblPresencePattern;

/***********************************************************************************************************************
 *  PROTOTYPES
 **********************************************************************************************************************/
static vsint16 ApplFblGetPresencePatternBaseAddress( vuint8 blockNr, V_MEMRAM1 IO_PositionType V_MEMRAM2 V_MEMRAM3 * pPresPtnAddr,
                                             V_MEMRAM1 IO_SizeType V_MEMRAM2 V_MEMRAM3 * pPresPtnLen );
static tFblResult ApplFblChkModulePresence( tBlockDescriptor * blockDescriptor );
static tFblResult ApplFblClrModulePresence( tBlockDescriptor * blockDescriptor );
/***********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  ApplFblGetPresencePatternBaseAddress
 **********************************************************************************************************************/
/*! \brief       Returns the base address of the presence pattern and mask and the length of both fields.
 *  \param[in]   blockNr module ID of a downloaded module.
 *  \param[out]   pPresPtnAddr Pointer to RAM location to place the address to the begin of presence pattern region
 *                (pattern, not mask)
 *  \param[out]   pPresPtnLen Pointer to the RAM location where the length of the presence pattern shall be stored to.
 *                The length is the length of the pattern only (mask is excluded).
 *  \return      memSegment of the presence pattern location or kFblDiagMemSegmNotFound in case of an error.
 **********************************************************************************************************************/
static vsint16 ApplFblGetPresencePatternBaseAddress( vuint8 blockNr, V_MEMRAM1 IO_PositionType V_MEMRAM2 V_MEMRAM3 * pPresPtnAddr,
                                             V_MEMRAM1 IO_SizeType V_MEMRAM2 V_MEMRAM3 * pPresPtnLen )
{
   (* pPresPtnAddr) = FblLogicalBlockTable.logicalBlock[blockNr].presencePatternAddress;
   (* pPresPtnLen) = FBL_PP_SEGMENT_SIZE;

   return UPD_OEM_PSI_SEGMENT;
}

/***********************************************************************************************************************
 *  ApplFblChkModulePresence
 **********************************************************************************************************************/
/*! \brief       Checks if mask and value of the presence pattern are set for a valid module.
 *  \details     The location of the presence pattern and mask is taken from the logical block table in FBL_MTAB.C.
 *  \param[in]   blockDescriptor Pointer to the logical block descriptor
 *  \return      kFblOk:     Presence pattern are set and Mask value are OK,
 *               kFblFailed: Presence pattern not set or mask flag not correct.
 **********************************************************************************************************************/
/* PRQA S 6080 TAG_FblUpd_Ap_Gm_6080_1 */ /* MD_FblUpd_Ap_Gm_6080 */
static tFblResult ApplFblChkModulePresence( tBlockDescriptor * blockDescriptor ) /* PRQA S 3673 */ /* MD_FblUpd_Ap_Gm_3673_blockDescriptor */
{
   vuint8           flashPresPtn[kFblPresencePatternSize];
   vuint8           flashPresMsk[kFblPresencePatternSize];
   IO_PositionType  presPtnAddress;
   IO_SizeType      presPtnLen;
   IO_ErrorType     readResult;
   vuint32          i;
   tFblResult       result;

   /* Calculate location of presence pattern.           */
   /* Note that the end of the block descriptor already */
   /* points to the presence pattern itself.            */
   memSegment = ApplFblGetPresencePatternBaseAddress(blockDescriptor->blockNr, &presPtnAddress, &presPtnLen);

   if (memSegment >= 0)
   {
      /* Read presence pattern */
      readResult = ApplFblReadPattern(flashPresPtn, presPtnAddress);
      if ((readResult != IO_E_OK) && (readResult != IO_E_ERASED))
      {
         /* Read has failed */
         result = kFblFailed;
      }
      else
      {
         /* Read presence pattern */
         readResult = ApplFblReadPattern(flashPresMsk, (presPtnAddress + presPtnLen));
         if ((readResult != IO_E_OK) && (readResult != IO_E_ERASED))
         {
            /* Read has failed */
            result = kFblFailed;
         }
         else
         {
            result = kFblOk;
            for (i = 0u; ((i < kFblPresencePatternSize) && (kFblOk == result)); i++)
            {
               /* Compare the PP-value against the expected one */
               if ((flashPresMsk[i] ^ flashPresPtn[i]) != (blockPresencePattern[i] ^ FBL_FLASH_DELETED))
               {
                  /* PP value is different or MASK does not have its expected value */
                  result = kFblFailed;
               }
            }
         }
      }
   }
   else
   {
      result = kFblFailed;
   }
   return result;
}
/* PRQA L:TAG_FblUpd_Ap_Gm_6080_1 */

/***********************************************************************************************************************
 *  ApplFblModuleClrPresence
 **********************************************************************************************************************/
/*! \brief       Sets the mask presence pattern in flash memory to invalidate the block.
 *  \details     The location of the presence pattern will be taken from the logical block descriptor.
 *  \pre         Module shall be marked as invalid now.
 *  \param[in]   blockDescriptor Pointer to the logical block descriptor
 *  \return      kFblOk: Mask for invalidation successfully written, kFblFailed: Error writing invalidation mask
 **********************************************************************************************************************/
static tFblResult ApplFblClrModulePresence( tBlockDescriptor * blockDescriptor )
{
   tFblResult                 result = kFblOk;
   vuint32                    i;
   IO_PositionType            presPtnAddress;
   IO_SizeType                presPtnLen;
   tFblpresPtnAlignedBuffer   flashHeader;
   vuint8 *                   pFlashHeader;

   /* Calculate location of presence pattern.           */
   /* Note that the end of the block descriptor already */
   /* points to the presence pattern itself.            */
   memSegment = ApplFblGetPresencePatternBaseAddress(blockDescriptor->blockNr, &presPtnAddress, &presPtnLen);

   /* valid segment found */
   if (memSegment >= 0)
   {
     /* FlashWrite requires an aligned address to RAM */
     pFlashHeader = flashHeader.data;

      /* Check if presence pattern is set for this module */
      /* Only clear and remove the marker if not already disabled */
      if (ApplFblChkModulePresence(blockDescriptor) == kFblOk)
      {
         /* Copy presence pattern to RAM buffer */
         for (i = 0u; i < kFblPresencePatternSize; i++)
         {
            /* Set the inverse of the presence pattern */
            pFlashHeader[i] = FblInvert8Bit(blockPresencePattern[i]);
         }
         /* Clear remaining buffer if any */
         for (i = kFblPresencePatternSize; i < FBL_MAX_SEGMENT_SIZE; i++)
         {
            pFlashHeader[i] = 0;
         }
         /* Now write them */
         if (!ApplFblWritePattern(pFlashHeader, presPtnLen, (presPtnAddress + presPtnLen)))
         {
            /* Writing the mask has failed. */
            result = kFblFailed;
         }
         (void)FblRealTimeSupport();
      }
   }
   else
   {
      /* Check if presence pattern is set for this module */
      /* Only clear and remove the marker if not already disabled */
      if (ApplFblChkModulePresence(blockDescriptor)==kFblOk)
      {
         result = kFblFailed;
      }
   }

   return result;
}

/***********************************************************************************************************************
 *  ApplFblUpdOemInvalidateUpdater
 **********************************************************************************************************************/
/*! \brief    Callback function to invalidate the updater/application
 *  \details  Update validity information of e.g. Eep-data
 *  \return   invalidation state
 **********************************************************************************************************************/
tFblResult ApplFblUpdOemInvalidateUpdater( void )
{
   tFblResult       result;
   tBlockDescriptor downloadBlockDescriptor;

   /* Invalidate update bootloader to prevent that */
   /* the new bootloader starts the updater again  */
   downloadBlockDescriptor.blockNr           = FBL_UPD_LBT_NR_OF_UPDATER;

   result = ApplFblClrModulePresence(&downloadBlockDescriptor);

   return result;
}

/***********************************************************************************************************************
 *  MISRA DEVIATIONS
 **********************************************************************************************************************/
/* module specific MISRA deviations:
   MD_FblUpd_Ap_Gm_3673_blockDescriptor:
     Reason: API definition don't use const keyword.
     Risk: -
     Prevention: not necessary.

   MD_FblUpd_Ap_Gm_6080:
     Reason: Re-use of reliable, well-proven code
     Risk: Complexity decreases readability and maintainability
     Prevention: Catch complexity through (regression) testing
*/
/***********************************************************************************************************************
 *  END OF FILE: UPD_OEM_AP.C
 **********************************************************************************************************************/
