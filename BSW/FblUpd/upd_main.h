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
 *                                                   Renamed component to FblUpd_Main
 *                                                   Moved callouts to separate components
 *  03.00.01   2016-08-05  DlM     ESCAN00091311    Added missing call of FblCwSetTxInit()
 *  03.01.00   2016-08-18  Rie     ESCAN00091520    No changes
 *  03.01.01   2016-09-09  JHg     ESCAN00091830    No changes
 *  03.02.00   2016-10-14  DlM     ESCAN00092338    No changes
 *  03.02.01   2016-12-07  Shs     ESCAN00093203    No changes
 *  03.02.02   2016-12-13  JHg     ESCAN00093274    No changes
 *  03.02.03   2017-02-07  JHg     ESCAN00093818    No changes
 *  03.02.04   2017-04-17  Shs     ESCAN00094760    No changes
 **********************************************************************************************************************/

#ifndef __UPD_MAIN_H__
#define __UPD_MAIN_H__

/***********************************************************************************************************************
 *  VERSION
 **********************************************************************************************************************/

/* ##V_CFG_MANAGEMENT ##CQProject : FblUpd_Main CQComponent : Implementation */
#define FBLUPD_MAIN_VERSION            0x0302u
#define FBLUPD_MAIN_RELEASE_VERSION    0x04u

/* Interface version */
/** Major interface version identifies incompatible changes */
#define FBL_UPD_API_VERSION_MAJOR      0x01u
/** Minor interface version identifies backward compatible changes */
#define FBL_UPD_API_VERSION_MINOR      0x01u
/** Release interface version identifies cosmetic changes */
#define FBL_UPD_API_VERSION_RELEASE    0x00u

/***********************************************************************************************************************
*  INCLUDES
**********************************************************************************************************************/

#include "upd_types.h"
#include "upd_cfg.h"

#if defined( FBL_UPD_ENABLE_RESPONSE_PENDING )
# include "fbl_cw.h"
#endif /* FBL_UPD_ENABLE_RESPONSE_PENDING */

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

/* Default values for configuration parameters */

/** Limit the number of update attempts in case of failure */
#if defined( FBL_UPD_MAX_RETRIES )
#else
# define FBL_UPD_MAX_RETRIES              3u
#endif /* FBL_UPD_MAX_RETRIES */

/** Buffer size used during write */
#if defined( FBL_UPD_BUFFER_SIZE )
#else
# define FBL_UPD_BUFFER_SIZE              (4096u)
#endif /* FBL_UPD_BUFFER_SIZE */
/** Buffer size used during verify */
#if defined( FBL_UPD_COMPARE_SIZE )
#else
# define FBL_UPD_COMPARE_SIZE             (FBL_UPD_BUFFER_SIZE / 2u)
#endif /* FBL_UPD_COMPARE_SIZE */

/** Maximum number of sub-segments per update segment */
#if defined( FBL_UPD_SEGMENT_COUNT )
#else
# define FBL_UPD_SEGMENT_COUNT               2u
#endif /* FBL_UPD_SEGMENT_COUNT */

/** Watchdog triggered every n-th cycle (has to power of two) */
#if defined( FBL_UPD_WATCHDOG_CYCLE_COUNT )
#else
# define FBL_UPD_WATCHDOG_CYCLE_COUNT        (0x20u)
#endif

/* Communication wrapper stubs */

#if defined( FBL_UPD_ENABLE_RESPONSE_PENDING )
/** Response pending state (always active) */
# define GetRcrRpInProgress()             ((vuint8)0x01u)
#else
/** Response pending state (never active) */
# define GetRcrRpInProgress()             ((vuint8)0x00u)
#endif /* FBL_UPD_ENABLE_RESPONSE_PENDING */
/** Diagnostic buffer state (always locked) */
#define GetDiagBufferLocked()             ((vuint8)0x01u)

#if defined( FBL_CW_LOCKS_TASKS ) || \
    defined( FBLCW_LOCKS_TASKS )
/* Communication wrapper present */
#else
# define FBL_UPD_STUB_CW_API

# define FBL_CW_LOCKS_TASKS
# define FBLCW_LOCKS_TASKS
# define FblCwIsTimerTaskAllowed()        ((vuint8)0x00u)
# define FblCwIsStateTaskAllowed()        ((vuint8)0x00u)
# define FblCwRetransmitRP()
# define FblCwClrInit()
# define FblCwTimerTask()
# define FblCwStateTask()
# endif /* FBL_CW_LOCKS_TASKS || FBLCW_LOCKS_TASKS */

/***********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

#define FBL_UPD_MAIN_START_SEC_VAR
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

extern V_MEMRAM0 V_MEMRAM1 vsint16  V_MEMRAM2 memSegment;

#if defined( FBL_UPD_ENABLE_RESPONSE_PENDING )
extern V_MEMRAM0 V_MEMRAM1 vuint8   V_MEMRAM2 DiagBuffer[FBL_UPD_RESPONSE_PENDING_SIZE];
#endif /* FBL_UPD_ENABLE_RESPONSE_PENDING */

#define FBL_UPD_MAIN_STOP_SEC_VAR
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/***********************************************************************************************************************
 *  FUNCTION PROTOTYPES
 **********************************************************************************************************************/

#if defined( FBL_UPD_ENABLE_RESPONSE_PENDING)
# define FBL_UPD_RAMCODE_START_SEC_CODE
# include "MemMap.h"

void FblDiagTxErrorIndication( vuint8 cwMsgType );
void FblDiagRxIndication( V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * pbDiagBuffer, tCwDataLengthType rxDataLength );
void FblDiagTxConfirmation( vuint8 cwMsgType );
V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * FblDiagRxGetPhysBuffer( tCwDataLengthType rxDataLength );
V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * FblDiagRxGetFuncBuffer( tCwDataLengthType rxDataLength );
void FblDiagRxStartIndication( void );
void FblDiagRxErrorIndication( void );

# define FBL_UPD_RAMCODE_STOP_SEC_CODE
# include "MemMap.h"
#endif /* FBL_UPD_ENABLE_RESPONSE_PENDING */

vsint16 FblMemSegmentNrGet( tFblAddress address );

#endif /* __UPD_MAIN_H__ */

/***********************************************************************************************************************
 *  END OF FILE: UPD_MAIN.H
 **********************************************************************************************************************/
