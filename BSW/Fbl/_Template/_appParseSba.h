/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief         SBAT Parser
 *
 *  --------------------------------------------------------------------------------------------------------------------
 *  COPYRIGHT
 *  --------------------------------------------------------------------------------------------------------------------
 *  \par Copyright
 *  \verbatim
 *  Copyright (c) 2015 by Vector Informatik GmbH.                                                  All rights reserved.
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
 *  Dennis O'Donnell              Dod           Vector CANtech, Inc.
 *  Sebastian Loos                Shs           Vector Informatik GmbH
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  01.00.00   2014-05-05  AWh                      creation
 *  01.01.00   2014-05-09  AWh                      Update External workspace use case
 *  01.02.00   2015-03-18  Dod                      Include v_def.h
 *                                                  Add comment for required header files from the Fbl
 *  01.03.00   2016-04-21  Shs                      No changes
 *  01.04.00   2016-05-03  Shs                      Increased sec workspace size
 **********************************************************************************************************************/
#ifndef __APP_PARSE_SBA__
#define __APP_PARSE_SBA__

#include "v_def.h"

/* ---  Module Version --- */
#define APP_PARSE_SBA_VERSION               0x0103u
#define APP_PARSE_SBA_RELEASE_VERSION       0x00u

/***********************************************************************************************************************
 *  Required header files from the Fbl project
 **********************************************************************************************************************/
/*
 * These files must be copied to the application project in order to compile the appParseSba.c file with application.
 * Fbl header files:             fbl_def.h, fbl_cfg.h, fbl_assert.h, fbl_assert_oem.h
 * Security module header files: sec_crc.h, sec_inc.h, sec_types.h, sec_verification.h, sec_workspace.h, sec.h,
 *                               secm_cfg.h, secm_inc.h, secm.h, secmpar.h
 * Vector header files:          v_cfg.h, v_def.h
 * Note: v_cfg.h and v_def.h may already be available in the application.  In this case, do not copy them from the Fbl.
 */

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

/* You may enable this for test purpose, if you do not yet have NVM access and want to use dummy data instead of real
 * data still.
 */
#define APP_SBA_TEST_WITH_DUMMY

/* Either of the two options below is possible. Note that you need to reserve
 * workspace memory to the Fbl mapped location if you use WORKSPACE_SHARE_WITH_FBL */
#define WORKSPACE_SHARE_WITH_FBL 0x00u
#define WORKSPACE_USE_OWN 0x01u
#define APP_SBA_CRYPTO_LIB_WORKSPACE WORKSPACE_USE_OWN

#if !defined( APP_PARSE_SBA_READ_SEC_DATA )
/* This function can be implemented in case the controller requires to differentiate between Ram and Rom pointers.
 * The default just passes a Rom pointer to the security module, whereas a Ram pointer is expected, which is
 * no problem for usual machines.
 */
# define APP_PARSE_SBA_READ_SEC_DATA(romLocation) (romLocation)  /* PRQA S 3453 */ /* MD_CBD_19.7 */
#endif

/* This size should be usually sufficient, it might be possible to reduce it in case of ram size problems */
#define APP_SBA_WS_SIZE 2050u

/***********************************************************************************************************************
 *  PROTOTYPES
 **********************************************************************************************************************/

vuint8 AppParseSba_ParseSba( const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * sbaTicket );

/***********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/
#endif /* __APP_PARSE_SBA__ */

/***********************************************************************************************************************
 *  END OF FILE: KBFBL_APFRAMEOEM.H
 **********************************************************************************************************************/

