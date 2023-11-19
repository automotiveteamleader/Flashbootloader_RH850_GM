/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief        Implementation of the verification component of the HIS security module - Default workspaces
 *
 *  \description  Defines the default workspaces used by the library parts for verification, seed/key and decryption
 *  -------------------------------------------------------------------------------------------------------------------
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \par Copyright
 *  \verbatim
 *  Copyright (c) 2014 by Vector Informatik GmbH.                                                  All rights reserved.
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
 *  Markus Schneider              Mss           Vector Informatik GmbH
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  01.00.00   2013-06-20  JHg     ESCAN00068213    Initial release based on FblSecMod_Vector 02.02.00
 *  02.00.00   2013-10-01  JHg     ESCAN00070691    Major refactoring
 *                                                   Moved to separate sub-package
 *                                                   Select available workspaces at integration-time
 *                                                   Export pointer and size of workspaces instead workspaces directly
 *  02.01.00   2013-11-08  JHg     ESCAN00071174    Added support for seed/key and verification (filter settings)
 *  02.02.00   2013-12-19  Mss     ESCAN00072741    Added support for AES encryption
 *  02.03.00   2014-02-12  Mss     ESCAN00073596    Added support for AES192 and AES256
 *  02.03.01   2014-05-26  JHg     ESCAN00075254    Fixed case of MemMap.h
 *  02.04.00   2015-10-12  JHg     ESCAN00085809    Added support for RSASSA-PSS signatures
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

/* File has to be delivered as object code */
#define SEC_OBJECT_CODE_ONLY

/* Security module configuration settings */
#include "Sec_Inc.h"

/* Global definitions for security module */
#include "Sec_Types.h"

/* Workspace interface */
#include "Sec_Workspace.h"

/* Required for definitions of workspace types */
# include "Sec_VerificationLib.h"

/**********************************************************************************************************************
 *  VERSION
 *********************************************************************************************************************/

#if ( SYSSERVICE_SECMODHIS_WORKSPACE_VERSION != 0x0204u ) || \
    ( SYSSERVICE_SECMODHIS_WORKSPACE_RELEASE_VERSION != 0x00u )
# error "Error in SEC_WORKSPACE.C: Source and header file are inconsistent!"
#endif

/**********************************************************************************************************************
 *  LOCAL DATA
 *********************************************************************************************************************/

#define SECMWS_START_SEC_DATA
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/** Default workspace for SHA-256 hash primitive */
V_MEMRAM0 static V_MEMRAM1 SecM_WorkspaceHashSha256Type  V_MEMRAM2 workSpaceHashSha256;

/** Default workspace for SHA-256 RSA V1.5 signature primitive */
V_MEMRAM0 static V_MEMRAM1 SecM_WorkspaceSigRsaV15Sha256Type   V_MEMRAM2 workSpaceSigRsaV15Sha256;

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/** Pointer to default workspace for SHA-256 hash primitive */
V_MEMROM0 V_MEMROM1 SecM_WorkspacePtrType V_MEMROM2 secWorkSpacePtrHashSha256    = (SecM_WorkspacePtrType)&workSpaceHashSha256;     /* PRQA S 0310 */ /* MD_SecWorkspace_0310 */
/** Size of default workspace for SHA-256 hash primitive */
V_MEMROM0 V_MEMROM1 SecM_LengthType       V_MEMROM2 secWorkSpaceSizeHashSha256   = sizeof(workSpaceHashSha256);

/** Pointer to default workspace for SHA-256 RSA V1.5 signature primitive */
V_MEMROM0 V_MEMROM1 SecM_WorkspacePtrType V_MEMROM2 secWorkSpacePtrSigRsaV15Sha256  = (SecM_WorkspacePtrType)&workSpaceSigRsaV15Sha256;   /* PRQA S 0310 */ /* MD_SecWorkspace_0310 */
/** Size of default workspace for SHA-256 RSA V1.5 signature primitive */
V_MEMROM0 V_MEMROM1 SecM_LengthType       V_MEMROM2 secWorkSpaceSizeSigRsaV15Sha256 = sizeof(workSpaceSigRsaV15Sha256);

#define SECMWS_STOP_SEC_DATA
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  MISRA
 *********************************************************************************************************************/

/* Module specific MISRA deviations:

   MD_SecWorkspace_0310:
      Reason: References to workspaces are passed as generic pointer which is casted back to the actual type on use.
      Risk: Passing of pointer with less strict alignment than required by workspace type.
      Prevention: Generic pointer type references basic structure to enforce alignment required by most workspace types.

*/

/***********************************************************************************************************************
 *  END OF FILE: SEC_WORKSPACELIB.C
 **********************************************************************************************************************/
