/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief         FBL updater callout routines - Platform specific for RH850
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
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  01.00.00   2016-07-27  Shs     -                Initial release
 *  01.00.01   2016-09-01  Shs     ESCAN00091707    Reworked function names, added ApplFblUpdHwInit()
 **********************************************************************************************************************/

#ifndef __UPD_HW_CFG_H__
#define __UPD_HW_CFG_H__

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/
/*-- Define Hook Functions -------------------------------------------------------------------------------------------*/
#if defined( FLASH_ENABLE_SET_RESETVECTOR_API )
# if !defined( FBL_UPD_HOOK_PREPARE_ERASE )
#  define FBL_UPD_HOOK_PREPARE_ERASE             ApplFblUpdHwPrepareErase
# endif /* FBL_UPD_HOOK_PREPARE_ERASE */

# if !defined( FBL_UPD_HOOK_FINALIZE_VERIFY )
#  define FBL_UPD_HOOK_FINALIZE_VERIFY           ApplFblUpdHwFinalizeVerify
# endif /* FBL_UPD_HOOK_FINALIZE_VERIFY */
#endif /* defined(FLASH_ENABLE_SET_RESETVECTOR_API)*/

/*-- Overwrite Callout Functions -------------------------------------------------------------------------------------*/
#define FBL_UPD_CALLOUT_SET_VFP                  ApplFblUpdHwSetVfp
#define FBL_UPD_CALLOUT_RESET_VFP                ApplFblUpdHwResetVfp
#define FBL_UPD_CALLOUT_RESET                    ApplFblUpdHwReset
#define FBL_UPD_CALLOUT_INIT_POWER_ON_HW         ApplFblUpdHwInit

/*-- Define Reset Vectors --------------------------------------------------------------------------------------------*/
/** FBL reset vector address for normal FBL usage (32 bit, has to be aligned to multiples of 0x200) */
#define FBL_UPD_FBL_RESET_VECTOR_ADDR            0x00000000ul

/** Updater reset vector address to restart the update in case of a reset during the update procedure
 *  (has to be aligned to multiples of 0x200) 
 *  Example: extern void _usr_init( void );
 *     #define FBL_UPD_UPDATER_RESET_VECTOR_ADDR (vuint32)_usr_init
 */
#define FBL_UPD_UPDATER_RESET_VECTOR_ADDR        ApplIntJmpTable[1]

#endif /* __UPD_HW_CFG_H__ */

/***********************************************************************************************************************
 *  END OF FILE: UPD_HW_CFG.H
 **********************************************************************************************************************/
