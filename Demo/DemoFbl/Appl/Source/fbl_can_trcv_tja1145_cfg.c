/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief        Transceiver Driver Configuration (TJA1145)
 *
 *  \note         Please note, that this file contains an implementation/configuration example for the TJA1145 driver.
 *                This code may influence the behavior of the TJA1145 driver in principle. Therefore, great care must be
 *                taken to verify the correctness of the implementation.
 *
 *                The contents of the originally delivered files are only examples resp. implementation proposals.
 *                With regard to the fact that these functions are meant for demonstration purposes only, Vector
 *                Informatik's liability shall be expressly excluded in cases of ordinary negligence, to the extent
 *                admissible by law or statute.
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
 *  Joern Herwig                  JHg           Vector Informatik GmbH
 *  Steve Werner                  Swe           Vector CANtech Inc.
 *  Andreas Wenckebach            AWh           Vector Informatik GmbH
 *  Marco Riedl                   Rie           Vector Informatik GmbH
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  01.01.00   2015-09-11  JHg     ESCAN00085179    Added support for multiple transceiver channels
 *  01.02.00   2016-08-04  Swe     ESCAN00089172    No changes
 *                                 ESCAN00091072    No changes
 *  01.02.01   2016-08-04  AWh     ESCAN00092769    No changes
 *  01.02.02   2016-11-18  Rie     ESCAN00092883    No changes
 **********************************************************************************************************************/

#define FBL_CAN_TRCV_TJA1145_CFG_SOURCE

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

#include "fbl_can_trcv_tja1145_inc.h"

/***********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

#if ( FBL_CAN_TRCV_TJA1145_NUMBER_OF_CHANNELS > 1u)
/** Mapping from transceiver channel to SPI handle */
V_MEMROM0 V_MEMROM1 vuint8 V_MEMROM2 g_FblCanTrcvTja1145SpiHandleMap[FBL_CAN_TRCV_TJA1145_NUMBER_OF_CHANNELS] =
{
   0u,
   1u
};   /* PRQA S 0883 */ /* MD_FblCanTrcv_0883 */
#endif /* FBL_CAN_TRCV_TJA1145_NUMBER_OF_CHANNELS */

/***********************************************************************************************************************
 *  END OF FILE: FBL_CAN_TRCV_TJA1145_CFG.C
 **********************************************************************************************************************/
