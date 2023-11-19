/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief        Transceiver Driver (TJA1145)
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
 *  Alexander Starke              ASe           Vector Informatik GmbH
 *  Joern Herwig                  JHg           Vector Informatik GmbH
 *  Steve Werner                  Swe           Vector CANtech Inc.
 *  Andreas Wenckebach            AWh           Vector Informatik GmbH
 *  Marco Riedl                   Rie           Vector Informatik GmbH
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  01.00.00   2015-07-23  ASe     -                Initial version
 *  01.01.00   2015-09-11  JHg     ESCAN00085179    Added support for multiple transceiver channels
 *  01.02.00   2016-08-04  Swe     ESCAN00089172    Added support for sleep mode
 *                                 ESCAN00091072    No changes
 *  01.02.01   2016-08-04  AWh     ESCAN00092769    Compiler error: Prototypes missing for EcuM_SetWakeupEvent,
 *                                                   EcuM_CheckWakeup
 *  01.02.02   2016-11-18  Rie     ESCAN00092883    Removed EcuM_SetWakeupEvent and EcuM_CheckWakeup
 **********************************************************************************************************************/

#ifndef __FBL_CAN_TRCV_TJA1145_H__
#define __FBL_CAN_TRCV_TJA1145_H__

/***********************************************************************************************************************
 *  VERSION
 **********************************************************************************************************************/

/* ##V_CFG_MANAGEMENT ##CQProject : FblDrvTrans_Tja1145CanSpi CQComponent : Implementation */
#define FBLDRVTRANS_TJA1145CANSPI_VERSION             0x0102u
#define FBLDRVTRANS_TJA1145CANSPI_RELEASE_VERSION     0x02u

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

#include "fbl_def.h"

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

#if ( FBL_CAN_TRCV_TJA1145_NUMBER_OF_CHANNELS > 1u )

/* Multi-channel use-case:
 * The first / single argument of all API functions is the trcvHandle. The handle is an index into the
 * g_FblCanTrcvTja1145SpiHandleMap array which maps the transceiver channel to the SPI handle to be used. */
# define FBL_CAN_TRCV_TJA1145_HANDLE_TYPE_ONLY        tFblCanTrcvTja1145Handle trcvHandle  /* PRQA S 3460 */ /* MD_CBD_19.4 */
# define FBL_CAN_TRCV_TJA1145_HANDLE_TYPE_FIRST       tFblCanTrcvTja1145Handle trcvHandle, /* PRQA S 3460 */ /* MD_CBD_19.4 */
# define FBL_CAN_TRCV_TJA1145_HANDLE_PARAM_ONLY       trcvHandle
# define FBL_CAN_TRCV_TJA1145_HANDLE_PARAM_FIRST      trcvHandle,
# define FBL_CAN_TRCV_TJA1145_HANDLE                  trcvHandle

#elif ( FBL_CAN_TRCV_TJA1145_NUMBER_OF_CHANNELS == 1u )

/* Single-channel use-case:
 * The API functions do not use the transceiver handle argument. In this case the transceiver driver supports only
 * a single channel. */
# define FBL_CAN_TRCV_TJA1145_HANDLE_TYPE_ONLY        void  /* PRQA S 3460 */ /* MD_CBD_19.4 */
# define FBL_CAN_TRCV_TJA1145_HANDLE_TYPE_FIRST       /* Not used */
# define FBL_CAN_TRCV_TJA1145_HANDLE_PARAM_ONLY       /* Not used */
# define FBL_CAN_TRCV_TJA1145_HANDLE_PARAM_FIRST      /* Not used */
# define FBL_CAN_TRCV_TJA1145_HANDLE                  0u

#else /* ( FBL_CAN_TRCV_TJA1145_NUMBER_OF_CHANNELS < 1u ) */
# error "FBL_CAN_TRCV_TJA1145.H: Invalid configuration of FBL_CAN_TRCV_TJA1145_NUMBER_OF_CHANNELS (must be >= 1)"
#endif /* ( FBL_CAN_TRCV_TJA1145_NUMBER_OF_CHANNELS < 1u ) */

/***********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

/** Handle for transceiver channel */
typedef vuint8 tFblCanTrcvTja1145Handle;

/***********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

#if defined( FBL_CAN_TRCV_TJA1145_ENABLE_MSR_CAN_IF_USAGE )
#else /* FBL_CAN_TRCV_TJA1145_DISABLE_MSR_CAN_IF_USAGE */
tFblResult FblCanTrcvTja1145Init( FBL_CAN_TRCV_TJA1145_HANDLE_TYPE_ONLY );
tFblResult FblCanTrcvTja1145Deinit( FBL_CAN_TRCV_TJA1145_HANDLE_TYPE_ONLY );
tFblResult FblCanTrcvTja1145SleepMode( FBL_CAN_TRCV_TJA1145_HANDLE_TYPE_ONLY );
tFblResult FblCanTrcvTja1145NormalMode( FBL_CAN_TRCV_TJA1145_HANDLE_TYPE_ONLY );
#endif /* FBL_CAN_TRCV_TJA1145_DISABLE_MSR_CAN_IF_USAGE */

#endif /* __FBL_CAN_TRCV_TJA1145_H__ */

/***********************************************************************************************************************
 *  END OF FILE: FBL_CAN_TRCV_TJA1145.H
 **********************************************************************************************************************/
