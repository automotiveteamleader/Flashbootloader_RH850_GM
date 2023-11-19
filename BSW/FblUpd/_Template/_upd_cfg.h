/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief        Main module of FBL updater
 *
 *  --------------------------------------------------------------------------------------------------------------------
 *  COPYRIGHT
 *  --------------------------------------------------------------------------------------------------------------------
 *  \par Copyright
 *  \verbatim
 *  Copyright (c) 2017 by Vector Informatik GmbH.                                                  All rights reserved.
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
 *  Markus Feninger               Fr            Vector Informatik GmbH
 *  Joern Herwig                  JHg           Vector Informatik GmbH
 *  Daniel Moessner               DlM           Vector Informatik GmbH
 *  Marco Riedl                   Rie           Vector Informatik GmbH
 *  Sebastian Loos                Shs           Vector Informatik GmbH
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  02.00.00   2015-08-04  Fr      -                Initial version
 *  03.00.00   2016-07-22  JHg     ESCAN00091120    Major rework
 *  03.00.01   2016-08-05  DlM     ESCAN00091311    No changes
 *  03.01.00   2016-08-18  Rie     ESCAN00091520    Changed include order
 *  03.01.01   2016-09-09  JHg     ESCAN00091830    Additional comments for configuration switches
 *  03.02.00   2016-10-14  DlM     ESCAN00092338    No changes
 *  03.02.01   2016-12-07  Shs     ESCAN00093203    Enable FBL_UPD_ENABLE_RESPONSE_PENDING only if it is not disabled
 *  03.02.02   2016-12-13  JHg     ESCAN00093274    No changes
 *  03.02.03   2017-02-07  JHg     ESCAN00093818    No changes
 *  03.02.04   2017-04-17  Shs     ESCAN00094760    No changes
 **********************************************************************************************************************/

#ifndef __UPD_CFG_H__
#define __UPD_CFG_H__

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

#include "upd_hw_cfg.h"
#include "upd_oem_cfg.h"
#include "upd_ap_cfg.h"

/* Updated bootloader */
/* TODO by customer: Adapt file name to match updated bootloader */
#include "DemoFbl.h"

#if defined( FBL_ENABLE_FLASHDRV_ROM )
#else
/* Header file for flashdriver ROM array */
# include "FlashRom.h"
#endif

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

/** Send RCR-RP messages during operation */
#if !defined( FBL_UPD_ENABLE_RESPONSE_PENDING ) && !defined( FBL_UPD_DISABLE_RESPONSE_PENDING )
# define FBL_UPD_ENABLE_RESPONSE_PENDING
#endif
#if defined( FBL_UPD_RESPONSE_PENDING_SIZE )
#else
/** Size of RCR-RP message */
# define FBL_UPD_RESPONSE_PENDING_SIZE       3u
#endif

/** Enabled padding of write buffers to match flash segment size */
#define FBL_UPD_ENABLE_PADDING

/** Limit the number of update attempts in case of failure */
#if defined( FBL_UPD_MAX_RETRIES )
#else
# define FBL_UPD_MAX_RETRIES                 3u
#endif

/** Buffer length used during write */
#if defined( FBL_UPD_BUFFER_SIZE )
#else
# define FBL_UPD_BUFFER_SIZE                 4096u
#endif
/** Buffer length used during verify */
#if defined( FBL_UPD_COMPARE_SIZE )
#else
# define FBL_UPD_COMPARE_SIZE                (FBL_UPD_BUFFER_SIZE/2)
#endif

/** Maximum number of sub-segments per update segment */
#if defined( FBL_UPD_SEGMENT_COUNT )
#else
# define FBL_UPD_SEGMENT_COUNT               2u
#endif /* FBL_UPD_SEGMENT_COUNT */

/** Watchdog triggered every n-th cycle (has to power of two) */
#if defined( FBL_UPD_WATCHDOG_CYCLE_COUNT )
#else
# define FBL_UPD_WATCHDOG_CYCLE_COUNT        0x20u
#endif

#endif /* __UPD_CFG_H__ */

/***********************************************************************************************************************
 *  END OF FILE: UPD_CFG.H
 **********************************************************************************************************************/
