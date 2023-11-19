/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief        Serial Peripheral Interface Driver Configuration
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
 *  Marco Riedl                   Rie           Vector Informatik GmbH
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  01.00.00   2015-04-09  ASe     -                Initial version
 *  01.00.01   2016-06-01  Rie     ESCAN00090259    No changes
 **********************************************************************************************************************/

#ifndef __FBL_SPI_IF_CFG_H__
#define __FBL_SPI_IF_CFG_H__

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

/** Maximum number of SPI configuration handles (see also g_Spi<...>ConfigParam[])
 *  For each SPI channel / configuration that shall be supported a dedicated handle is required. If multiple drivers
 *  are used, check for the one using the most handles. Configure the found (maximum) number of handles here. */
#define FBL_SPI_MAX_NUMBER_OF_HANDLES        1u

#endif /* __FBL_SPI_IF_CFG_H__ */

/***********************************************************************************************************************
 *  END OF FILE: FBL_SPI_IF_CFG.H
 **********************************************************************************************************************/
