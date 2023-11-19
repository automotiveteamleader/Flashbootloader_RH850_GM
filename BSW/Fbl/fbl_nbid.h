/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file          fbl_nbid.h
 *  \brief         Declaration of functions, variables, and constants. For module details check fbl_nbid.c
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
 *                                 ESCAN00088973   No changes
 *                                 ESCAN00088984   No changes
 *  02.07.00   2016-03-24  Shs     ESCAN00089073   No changes
 *  02.07.01   2016-04-04  ThM     ESCAN00089202   No changes
 *                         AWh     ESCAN00089235   No changes
 *  03.00.00   2016-04-08  AWh     ESCAN00089532   No changes
 *                                 ESCAN00089628   No changes
 *  03.01.00   2016-08-04  HRf     ESCAN00091043   No changes
 *                         AWh     ESCAN00091147   No changes
 *                                 ESCAN00091325   No changes
 *                                 ESCAN00091279   Basic NVM NBID FlashDriver Init too late
 *                                 ESCAN00089426   No changes
 *  03.01.01   2016-10-04  AWh     ESCAN00092167   No changes
 *  03.02.00   2016-10-31  ThM     ESCAN00092596   No changes
 **********************************************************************************************************************/

#ifndef __FBL_NBID_H__
#define __FBL_NBID_H__

/* PRQA S 0828 TAG_FblHdr_828 */ /* MD_MSR_1.1_828 */

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

#if defined( FBL_NBID_USER_DRIVER )
  /* Please use this configuration, if the driver you want to use is/should not be configured to FlashBlock table.
   * You have to define these macros externally to your user drivers properties:
   * - FblNbid_InitSync
   * - FblNbid_DeinitSync
   *   (There is more information on what to define in .c - file of this module)
   *
   * Note that the bootloader is not allowed to access content of this drivers memory during programming.
   */
#else
  /* This is the standard configuration, applicable when used driver is configured to FlashBlock table. */
# define FblNbid_InitSync   MemDriver_InitSync
# define FblNbid_DeinitSync MemDriver_DeinitSync
#endif /* FBL_NBID_USER_DRIVER */

#define FBL_NBID_APPNBR_1ST        0x01u
#define FBL_NBID_APPNBR_2ND        0x02u
#define FBL_NBID_APPNBR_3RD        0x03u
#define FBL_NBID_APPNBR_4TH        0x04u

/* Fbl NBID defines: Key Id is fixed to 0. For valid application numbers (appnumber) check above */
#define FBL_NBID_KEY_ID            0x00u
#define FBL_NBID_APP_ID(appnumber) (appnumber) /* PRQA S 3453 */ /* MD_MSR_19.7 */

#if !defined( FBL_ENABLE_WRAPPER_NV ) && defined( FBL_HDR_ENABLE_BASIC_NVM_HANDLING )
/* Acces macros in Wrapper-Nv style */
# define ApplFblNvReadKey_NBID(idx, buf)            (FblNbidReadNbid(FBL_NBID_KEY_ID, (V_MEMRAM1 vuint16 V_MEMRAM2 V_MEMRAM3 *) (buf))) /* PRQA S 3453 */ /* MD_MSR_19.7 */
# define ApplFblNvWriteKey_NBID(idx, buf)           (FblNbidIncrement(FBL_NBID_KEY_ID, (V_MEMRAM1 vuint16 V_MEMRAM2 V_MEMRAM3 *) (buf))) /* PRQA S 3453 */ /* MD_MSR_19.7 */

# define ApplFblNvReadApp_NBID(idx, buf)            (FblNbidReadNbid(FBL_NBID_APP_ID(FBL_NBID_APPNBR_1ST), (V_MEMRAM1 vuint16 V_MEMRAM2 V_MEMRAM3 *) (buf))) /* PRQA S 3453 */ /* MD_MSR_19.7 */
# define ApplFblNvWriteApp_NBID(idx, buf)           (FblNbidIncrement(FBL_NBID_APP_ID(FBL_NBID_APPNBR_1ST), (V_MEMRAM1 vuint16 V_MEMRAM2 V_MEMRAM3 *) (buf))) /* PRQA S 3453 */ /* MD_MSR_19.7 */

#if ( FBL_MTAB_NO_OF_BLOCKS > 1 )
#  define ApplFblNvReadApp_NBID_2ndApp(idx, buf)    (FblNbidReadNbid(FBL_NBID_APP_ID(FBL_NBID_APPNBR_2ND), (V_MEMRAM1 vuint16 V_MEMRAM2 V_MEMRAM3 *) (buf))) /* PRQA S 3453 */ /* MD_MSR_19.7 */
#  define ApplFblNvWriteApp_NBID_2ndApp(idx, buf)   (FblNbidIncrement(FBL_NBID_APP_ID(FBL_NBID_APPNBR_2ND), (V_MEMRAM1 vuint16 V_MEMRAM2 V_MEMRAM3 *) (buf))) /* PRQA S 3453 */ /* MD_MSR_19.7 */
#  define ApplFblNvReadApp_NBID_3rdApp(idx, buf)    (FblNbidReadNbid(FBL_NBID_APP_ID(FBL_NBID_APPNBR_3RD), (V_MEMRAM1 vuint16 V_MEMRAM2 V_MEMRAM3 *) (buf))) /* PRQA S 3453 */ /* MD_MSR_19.7 */
#  define ApplFblNvWriteApp_NBID_3rdApp(idx, buf)   (FblNbidIncrement(FBL_NBID_APP_ID(FBL_NBID_APPNBR_3RD), (V_MEMRAM1 vuint16 V_MEMRAM2 V_MEMRAM3 *) (buf))) /* PRQA S 3453 */ /* MD_MSR_19.7 */
#  define ApplFblNvReadApp_NBID_4thApp(idx, buf)    (FblNbidReadNbid(FBL_NBID_APP_ID(FBL_NBID_APPNBR_4TH), (V_MEMRAM1 vuint16 V_MEMRAM2 V_MEMRAM3 *) (buf))) /* PRQA S 3453 */ /* MD_MSR_19.7 */
#  define ApplFblNvWriteApp_NBID_4thApp(idx, buf)   (FblNbidIncrement(FBL_NBID_APP_ID(FBL_NBID_APPNBR_4TH), (V_MEMRAM1 vuint16 V_MEMRAM2 V_MEMRAM3 *) (buf))) /* PRQA S 3453 */ /* MD_MSR_19.7 */
# endif /* FBL_MTAB_NO_OF_BLOCKS > 1 */
#endif /* FBL_ENABLE_WRAPPER_NV && FBL_HDR_ENABLE_BASIC_NVM_HANDLING */

/***********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
void FblNbidInit( void );
tFblResult FblNbidReadNbid( vuintx elementId, V_MEMRAM1 vuint16 V_MEMRAM2 V_MEMRAM3 * nbidValue );
tFblResult FblNbidIncrement( vuintx elementId, const V_MEMRAM1 vuint16 V_MEMRAM2 V_MEMRAM3 * nbidValue );

/* PRQA L:TAG_FblHdr_828 */

#endif /* __FBL_NBID_H__ */

/***********************************************************************************************************************
 *  END OF FILE: FBL_NBID.H
 **********************************************************************************************************************/

