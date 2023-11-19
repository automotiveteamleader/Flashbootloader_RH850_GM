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
 *  01.00.02   2016-12-07  Shs     ESCAN00092943    No changes
 *                                 ESCAN00093194    Reworked Stub defines
 **********************************************************************************************************************/

#ifndef __UPD_OEM_CFG_H__
#define __UPD_OEM_CFG_H__

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/
/*-- Define Hook Functions -------------------------------------------------------------------------------------------*/
#if !defined( FBL_UPD_HOOK_INVALIDATE_UPDATER )
# define FBL_UPD_HOOK_INVALIDATE_UPDATER        ApplFblUpdOemInvalidateUpdater
#endif /* FBL_UPD_HOOK_INVALIDATE_UPDATER */

/*-- Overwrite Callout Functions -------------------------------------------------------------------------------------*/

/*-- Configuration Defines -------------------------------------------------------------------------------------------*/

/* Stub fbl_cw */
#define FBL_UPD_STUB_CW_API
#define FBL_UPD_DISABLE_RESPONSE_PENDING

#if defined( FBL_UPD_STUB_CW_API )
/* Stub missing defines for fbl_cw */
# define FBL_CAN_NUMBER_OF_RX_ID    1u
# define FBL_CAN_NUMBER_OF_RANGES   1u
# define kFblCanTxOk                0u
# define kFblCanTxFailed            1u
# define kFblCanTxInProgress        2u
# define kFblCanSleepOk             0u
# define FBL_CAN_RX_ID0             0u
# define FBL_CAN_CODE_0             0u
# define FBL_CAN_MASK_0             0u
#endif /* FBL_UPD_STUB_CW_API */

/* TODO by customer: Adapt to match your configuration
 * typically the Updater resides in the first Logical Block (see fbl_mtab.c)
 * This information is used to determine the location of the Updaters Presence Pattern.
 */
#define FBL_UPD_LBT_NR_OF_UPDATER   0u

/* TODO by customer: Adapt to match your configuration
 * Configure segment Idx to Flash Block table to element in same memory like the PSI
 * Typically the same single driver is shared for all bootloader sectors and PSI, so driver used by sector 0
 * can be used.
 */
#define UPD_OEM_PSI_SEGMENT         0u

#endif /* __UPD_HW_CFG_H__ */

/***********************************************************************************************************************
 *  END OF FILE: UPD_OEM_CFG.H
 **********************************************************************************************************************/
