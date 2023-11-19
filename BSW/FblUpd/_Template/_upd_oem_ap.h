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
 *  01.00.01   2016-09-26  AWh     ESCAN00092036    No changes
 *  01.00.02   2016-12-07  Shs     ESCAN00092943    No changes
 *                                 ESCAN00093194    Reworked Stub defines
 **********************************************************************************************************************/

#ifndef __UPD_OEM_AP_H__
#define __UPD_OEM_AP_H__

/***********************************************************************************************************************
 *  VERSION
 **********************************************************************************************************************/

/* ##V_CFG_MANAGEMENT ##CQProject : FblUpd_Ap_Gm CQComponent : Implementation */
#define FBLUPD_AP_GM_VERSION            0x0100u
#define FBLUPD_AP_GM_RELEASE_VERSION    0x02u

/* Interface version */
/** Major interface version identifies incompatible changes */
#define FBL_UPD_AP_OEM_API_VERSION_MAJOR     0x01u
/** Minor interface version identifies backward compatible changes */
#define FBL_UPD_AP_OEM_API_VERSION_MINOR     0x00u
/** Release interface version identifies cosmetic changes */
#define FBL_UPD_AP_OEM_API_VERSION_RELEASE   0x00u

/***********************************************************************************************************************
*  INCLUDES
**********************************************************************************************************************/

#include "upd_types.h"
#include "upd_cfg.h"

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

/*-- Stubs -----------------------------------------------------------------------------------------------------------*/

/* stub fbl_apnv */
#define ApplFblReadPattern(buffer, address)           (MemDriver_RReadSync((IO_MemPtrType)(buffer), (IO_SizeType)kFblPresencePatternSize, (IO_PositionType)(address))) /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define ApplFblWritePattern(buffer, length, address)  (MemDriver_RWriteSync((IO_MemPtrType)(buffer), (IO_SizeType)(length), (IO_PositionType)(address)) == IO_E_OK)    /* PRQA S 3453 */ /* MD_MSR_19.7 */

/* stub fbl_diag */
#define FblRealTimeSupport FblLookForWatchdog

/* stub fbl_hdr */
#define FblHdrPipelinedVerifyIntegrity V_NULL
#define FblHdrVerifyIntegrity V_NULL


/***********************************************************************************************************************
 *  TYPEDEFS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  FUNCTION PROTOTYPES
 **********************************************************************************************************************/
extern tFblResult ApplFblUpdOemInvalidateUpdater( void );
/***********************************************************************************************************************
 *  CONFIGURATION CHECKS
 **********************************************************************************************************************/

#endif /* __UDP_OEM_AP_H__ */

/***********************************************************************************************************************
 *  END OF FILE: UDP_OEM_AP.H
 **********************************************************************************************************************/
