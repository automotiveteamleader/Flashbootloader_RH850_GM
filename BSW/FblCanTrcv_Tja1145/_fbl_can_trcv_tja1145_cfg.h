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
 *  01.02.00   2016-08-04  Swe     ESCAN00089172    No changes
 *                                 ESCAN00091072    No changes
 *  01.02.01   2016-08-04  AWh     ESCAN00092769    No changes
 *  01.02.02   2016-11-18  Rie     ESCAN00092883    No changes
 **********************************************************************************************************************/

#ifndef __FBL_CAN_TRCV_TJA1145_CFG_H__
#define __FBL_CAN_TRCV_TJA1145_CFG_H__

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

#include "fbl_spi_manufacturer_interface_inc.h"

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

/* Basically 3 use-cases are supported:
 *
 * 1st: No MSR modules are being used, the module directly writes "magic values" to the TJA1145 registers using the FBL
 *      SPI driver. Only the transition to normal mode is supported.
 *      - FBL_CAN_TRCV_TJA1145_DISABLE_MSR_CAN_TRCV_USAGE
 *      - FBL_CAN_TRCV_TJA1145_DISABLE_MSR_CAN_IF_USAGE
 * 2nd: The MSR CAN transceiver module (but no CAN interface - CanIf) is being used.
 *      - FBL_CAN_TRCV_TJA1145_ENABLE_MSR_CAN_TRCV_USAGE
 *      - FBL_CAN_TRCV_TJA1145_DISABLE_MSR_CAN_IF_USAGE
 * 3rd: The MSR CAN transceiver and CAN interface module are being used.
 *      - FBL_CAN_TRCV_TJA1145_ENABLE_MSR_CAN_TRCV_USAGE
 *      - FBL_CAN_TRCV_TJA1145_ENABLE_MSR_CAN_IF_USAGE
 */

/* Configure the usage of FBL-only modules (simplified/reduced TJA1145 feature support) or MSR modules */
/*#define FBL_CAN_TRCV_TJA1145_ENABLE_MSR_CAN_TRCV_USAGE*/
#define FBL_CAN_TRCV_TJA1145_DISABLE_MSR_CAN_TRCV_USAGE

#if defined( FBL_CAN_TRCV_TJA1145_ENABLE_MSR_CAN_TRCV_USAGE )
/* Configure the usage of MSR CanIf */
# define FBL_CAN_TRCV_TJA1145_ENABLE_MSR_CAN_IF_USAGE
/* # define FBL_CAN_TRCV_TJA1145_DISABLE_MSR_CAN_IF_USAGE */
#else /* FBL_CAN_TRCV_TJA1145_DISABLE_MSR_CAN_TRCV_USAGE */
# define FBL_CAN_TRCV_TJA1145_DISABLE_MSR_CAN_IF_USAGE
#endif /* FBL_CAN_TRCV_TJA1145_DISABLE_MSR_CAN_TRCV_USAGE */

/** Number of transceiver channels */
#define FBL_CAN_TRCV_TJA1145_NUMBER_OF_CHANNELS          1u

/* There are several loops, which must poll the TJA1145  */
/* If the condition does not happen they need to timeout */
/* This value must be set to a value large enough to     */
/* allow the TJA1145 to respond  (ms)                    */
#if defined( FBL_CAN_TRCV_TJA1145_ENABLE_MSR_CAN_TRCV_USAGE )
# define FBL_CAN_TRCV_TJA1145_TIMEOUT   10u
#endif

/* Macros to control chip select, if SPI driver handles the chip-select signal the macros can be left empty */
#define FBL_CAN_TRCV_TJA1145_INIT_CS(channel)
#define FBL_CAN_TRCV_TJA1145_DEINIT_CS(channel)
#define FBL_CAN_TRCV_TJA1145_SET_CS(channel)
#define FBL_CAN_TRCV_TJA1145_CLR_CS(channel)

#if ( FBL_CAN_TRCV_TJA1145_NUMBER_OF_CHANNELS == 1u)
# if ( FBL_SPI_MAX_NUMBER_OF_HANDLES > 1u )
/* Specify the configuration index of the g_Spi<...>ConfigParam for this transceiver device (see also fbl_spi_<..>cfg.h) */
# define FBL_CAN_TRCV_TJA1145_SPI_INIT_HANDLE            0u
# endif /* FBL_SPI_MAX_NUMBER_OF_HANDLES */
#endif /* FBL_CAN_TRCV_TJA1145_NUMBER_OF_CHANNELS */

/* Mapping of the SPI-hardware-specific functions */
#define FblCanTrcvTja1145SpiInit                         FblSpiManufacturerInterfaceInit
#define FblCanTrcvTja1145SpiTransferSync                 FblSpiManufacturerInterfaceTransferSync
#define FblCanTrcvTja1145SpiDeinit                       FblSpiManufacturerInterfaceDeinit

/***********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

#if ( FBL_CAN_TRCV_TJA1145_NUMBER_OF_CHANNELS > 1u)
V_MEMROM0 extern V_MEMROM1 vuint8 V_MEMROM2 g_FblCanTrcvTja1145SpiHandleMap[FBL_CAN_TRCV_TJA1145_NUMBER_OF_CHANNELS];
#endif /* FBL_CAN_TRCV_TJA1145_NUMBER_OF_CHANNELS */

#endif /* __FBL_CAN_TRCV_TJA1145_CFG_H__ */

/***********************************************************************************************************************
 *  END OF FILE: FBL_CAN_TRCV_TJA1145_CFG.H
 **********************************************************************************************************************/
