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

#ifndef __UPD_HW_AP_H__
#define __UPD_HW_AP_H__

/***********************************************************************************************************************
 *  VERSION
 **********************************************************************************************************************/

/* ##V_CFG_MANAGEMENT ##CQProject : FblUpd_Ap_RH850 CQComponent : Implementation */
#define FBLUPD_AP_RH850_VERSION              0x0100u
#define FBLUPD_AP_RH850_RELEASE_VERSION      0x01u

/* Interface version */
/** Major interface version identifies incompatible changes */
#define FBL_UPD_AP_HW_API_VERSION_MAJOR      0x01u
/** Minor interface version identifies backward compatible changes */
#define FBL_UPD_AP_HW_API_VERSION_MINOR      0x00u
/** Release interface version identifies cosmetic changes */
#define FBL_UPD_AP_HW_API_VERSION_RELEASE    0x00u

/***********************************************************************************************************************
*  INCLUDES
**********************************************************************************************************************/

#include "upd_types.h"
#include "upd_cfg.h"

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  TYPEDEFS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  FUNCTION PROTOTYPES
 **********************************************************************************************************************/
void ApplFblUpdHwInit(void);
tFblResult ApplFblUpdHwPrepareErase( void );
tFblResult ApplFblUpdHwFinalizeVerify( void );
void ApplFblUpdHwSetVfp(void);
void ApplFblUpdHwResetVfp(void);
void ApplFblUpdHwReset(void);
/***********************************************************************************************************************
 *  CONFIGURATION CHECKS
 **********************************************************************************************************************/

#endif /* __UPD_HW_AP_H__ */

/***********************************************************************************************************************
 *  END OF FILE: UDP_HW_AP.H
 **********************************************************************************************************************/
