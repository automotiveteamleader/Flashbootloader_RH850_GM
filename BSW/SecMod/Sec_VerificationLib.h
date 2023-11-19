/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief        Implementation of the verification component of the HIS security module - Signature verification
 *
 *  \description  Offers signature/checksum verification primitives based on hash, HMAC and RSA
 *  -------------------------------------------------------------------------------------------------------------------
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \par Copyright
 *  \verbatim
 *  Copyright (c) 2016 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 */
/*********************************************************************************************************************/

/***********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  --------------------------------------------------------------------------------------------------------------------
 *  Joern Herwig                  JHg           Vector Informatik GmbH
 *  Alexander Starke              ASe           Vector Informatik GmbH
 *  Christian Baeuerle            CB            Vector Informatik GmbH
 *  Andreas Wenckebach            AWh           Vector Informatik GmbH
 *  Thomas Mueller                ThM           Vector Informatik GmbH
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  01.00.00   2013-06-20  JHg     ESCAN00068213    Initial release based on FblSecMod_Vector 02.02.00
 *  02.00.00   2013-11-08  JHg     ESCAN00071179    Major refactoring
 *                                                   Moved to separate sub-package
 *                                                   Split into source and library parts
 *                                                   Select available verification primitives at integration-time
 *  02.00.01   2013-12-19  JHg     ESCAN00072260    No changes
 *                         JHg     ESCAN00072561    No changes
 *  02.00.02   2014-01-20  JHg     ESCAN00073088    No changes
 *  02.01.00   2014-01-28  ASe     ESCAN00073047    No changes
 *  02.02.00   2014-02-10  JHg     ESCAN00073560    No changes
 *                         JHg     ESCAN00073355    No changes
 *  02.02.01   2014-05-28  JHg     ESCAN00074792    No changes
 *                         JHg     ESCAN00073918    No changes
 *  02.02.02   2015-02-09  CB      ESCAN00078641    No changes
 *  02.02.03   2015-07-29  AWh     ESCAN00084130    No changes
 *  02.03.00   2015-10-12  JHg     ESCAN00085807    Added support for RSASSA-PSS signatures
 *                         JHg     ESCAN00085808    No changes
 *  02.04.00   2015-11-03  JHg     ESCAN00086240    No changes
 *                         JHg     ESCAN00086241    No changes
 *  02.04.01   2015-12-18  JHg     ESCAN00087162    No changes
 *  02.04.02   2016-04-25  ThM     ESCAN00083969    No changes
 **********************************************************************************************************************/

#ifndef __SEC_VERIFICATIONLIB_H_
#define __SEC_VERIFICATIONLIB_H_

/**********************************************************************************************************************
 *  VERSION
 *********************************************************************************************************************/

#define SYSSERVICE_SECMODHIS_VERIFICATIONLIB_VERSION            0x0204u
#define SYSSERVICE_SECMODHIS_VERIFICATIONLIB_RELEASE_VERSION    0x02u

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

#include "Sec_Inc.h"

#include "Sec_Verification.h"

/* ES library access */
#include "ESLib.h"

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/** Workspace for SHA-256 hash primitive */
typedef struct
{
   SecM_BasicWorkspaceHashSha256Type   basic;   /**< Basic information (message digest, keys) */
   eslt_WorkSpaceSHA256                esLib;   /**< ESLib workspace */
} SecM_WorkspaceHashSha256Type;

/** Workspace for SHA-256 RSA V1.5 signature primitive */
typedef struct
{
   SecM_BasicWorkspaceSigRsaV15Sha256Type basic;   /**< Basic information (message digest, keys) */
   eslt_WorkSpaceRSAver                   esLib;   /**< ESLib workspace */
} SecM_WorkspaceSigRsaV15Sha256Type;

/***********************************************************************************************************************
 *  CONFIGURATION CHECKS
 **********************************************************************************************************************/

/* Inclusion only allowed in modules which are delivered as object code, e.g. Sec_VerificationLib.c and Sec_Workspace.c! */
#if defined( SEC_OBJECT_CODE_ONLY )
#else
# error "Error in SEC_VERIFICATIONLIB.H: Included in file which isn't marked as object code delivery!"
#endif

#endif /* __SEC_VERIFICATIONLIB_H_ */

/***********************************************************************************************************************
 *  END OF FILE: SEC_VERIFICATIONLIB.H
 **********************************************************************************************************************/

