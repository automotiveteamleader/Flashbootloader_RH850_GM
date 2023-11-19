/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief         Implementation of common data structure.
 *                 This file is to be linked and compiled to the Fbl project only.
 *                 Only comdat.h should be used in your application.
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
 *
 *  \par Note
 *  \verbatim
 *  Please note, that this file contains a collection of callback functions to be used with the Flash Bootloader.
 *  These functions may influence the behaviour of the bootloader in principle.
 *  Therefore, great care must be taken to verify the correctness of the implementation.
 *
 *  The contents of the originally delivered files are only examples resp. implementation proposals.
 *  With regard to the fact that these functions are meant for demonstration purposes only, Vector Informatik´s
 *  liability shall be expressly excluded in cases of ordinary negligence, to the extent admissible by law or statute.
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
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  02.00.00   2016-07-26  AWh     -                Creation
 *  02.00.01   0216-11-17  Shs     -                No changes
 *  02.01.00   2016-11-18  AWh     -                Add fblAndApplSharedRam, remove configurable comdat elements (csum)
**********************************************************************************************************************/

#include "fbl_inc.h"

/***********************************************************************************************************************
 *  VERSION
 **********************************************************************************************************************/

#if ( FBL_COMDAT_VERSION != 0x0201u ) || \
    ( FBL_COMDAT_RELEASE_VERSION != 0x00u )
# error "Error in comdat.c: Source and header file are inconsistent!"
#endif

#if defined( VGEN_ENABLE_CANFBL )
#else
# error "This file is shall only be linked to the bootloader project and not used in aplication context."
#endif

# if !defined( FBL_GMHEADER_BDL_TIMEOUT )
#  define FBL_GMHEADER_BDL_TIMEOUT 0u
# endif

#define FBLCOMDAT_SHARED_RAM_MEMORY_START_SEC_DATA
#include "MemMap.h"
V_MEMRAM0 volatile V_MEMRAM1 tFblSharedRam V_MEMRAM2 fblAndApplSharedRam;
#define FBLCOMDAT_SHARED_RAM_MEMORY_STOP_SEC_DATA
#include "MemMap.h"

# if defined( FBL_ENABLE_COMMON_DATA )
#define FBLCOMDAT_COMMON_DATA_START_SEC_DATA
#include "MemMap.h"
/* Common data structure definition                                        *
 * This information can be linked to a fix location or accessed indirectly *
 * via fixed linked FblHeader. This allows to share data with the          *
 * application.                                                            */
V_MEMROM0 V_MEMROM1 tFblCommonData V_MEMROM2 fblCommonData =
{
   /* Start Rom Check Data Block */
  {
      FBL_GMHEADER_CSUM
     ,FBL_GMHEADER_ADDR_REG                /**< Product Memory Address + Number Of Bytes  */
   },
   FBL_COMDAT_VERIFICATION_PATTERN         /**< Verification Pattern                      */
  ,FBL_COMDAT_VERSION                      /**< Comdat version                            */
  ,FBL_GMHEADER_MODID_FBL                  /**< Module ID                                 */
  ,FBL_GMHEADER_ECUID                      /**< Ecu Id                                    */
   /* Start Boot Info Block */
  ,{
      /* Public RSA key for signature verification */
      {
         { SecM_RsaMod, sizeof(SecM_RsaMod) }
        ,{ SecM_RsaExp, sizeof(SecM_RsaExp) }
      }
     ,FBL_GMHEADER_SUBJNAME                 /**< Subject name                              */
     ,FBL_GMHEADER_ECUNAME                  /**< Ecu Name                                  */
     ,FBL_GMHEADER_DCID_FBL                 /**< Data Compatibility Identifier             */
     ,FBL_GMHEADER_DLS_FBL                  /**< Design Level Suffix                       */
     ,FBL_GMHEADER_SWMI_FBL                 /**< SoftWare Module Identifier                */
   /* AscII Partnumber can be derived from Part Number. Add an element, e.g. ASCIIPN[16], if you prefere redundancy. */
     ,FBL_GMHEADER_BDL_TIMEOUT              /**<  Bit Difference Timer Timeout. (0.1s unit) 0->disabled. */
    }
   ,(tExportFct)SecM_VerifySignature
#if defined( FBL_ENABLE_FBL_START )
# if defined( FBL_DEF_ENABLE_NON_KB_MAIN )
   ,(V_MEMRAM1 tFblSharedRam V_MEMRAM2 V_MEMRAM3 *) &fblAndApplSharedRam   /**< Pointer to common Ram Data  */
#else
   ,(V_MEMRAM1 tCanInitTable V_MEMRAM2 V_MEMRAM3 *) &CanInitTable   /**< Pointer to common Ram Data  */
# endif /* FBL_DEF_ENABLE_NON_KB_MAIN */
#else /* FBL_ENABLE_FBL_START*/
   /* You may decide to use tFblSharedRam or a similar structure. fblStartParameter variable is not provided by Vector core modules */
#endif /* FBL_ENABLE_FBL_START */
};
#define FBLCOMDAT_COMMON_DATA_STOP_SEC_DATA
#include "MemMap.h"
#endif /* FBL_ENABLE_COMMON_DATA */

/***********************************************************************************************************************
 *  END OF FILE: _COMDAT.c
 **********************************************************************************************************************/

