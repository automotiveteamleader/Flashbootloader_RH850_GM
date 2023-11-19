/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief        Serial Peripheral Interface Driver (Renesas Clocked Serial Interface H)
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
 *  Alexander Starke              ASe           Vector Informatik GmbH
 *  Marco Riedl                   Rie           Vector Informatik GmbH
 *  Dennis O'Donnell              Dod           Vector CANtech, Inc.
 *  Andre Caspari                 Ci            Vector Informatik GmbH
 *  Steve Werner                  Swe           Vector CANtech, Inc.
 *  Daniel Koebel                 DKl           Vector Informatik GmbH
 *  --------------------------------------------------------------------------------------------------------------------
 *  TEMPLATE REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  03.00.00   2015-11-18  ASe     -                Initial version
 *  03.00.01   2016-06-01  Rie     ESCAN00090444    Added SPI IF reference interface version defines
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  01.00.00   2014-10-28  Dod/Ci                   Initial version
 *  02.00.00   2016-03-17  Swe     ESCAN00088970    Updated to template version 03.00.00
 *  02.00.01   2016-09-13  Rie     ESCAN00091847    Fixed task handling
 *  02.01.00   2017-01-23  DKl     ESCAN00093432    No changes
 **********************************************************************************************************************/

#ifndef __FBL_SPI_RENESAS_CSIH_H__
#define __FBL_SPI_RENESAS_CSIH_H__

/***********************************************************************************************************************
 *  VERSION
 **********************************************************************************************************************/

/* ##V_CFG_MANAGEMENT ##CQProject : FblDrvSpi_RenesasCsih CQComponent : Implementation */
#define FBLDRVSPI_RENESASCSIH_VERSION            0x0201u
#define FBLDRVSPI_RENESASCSIH_RELEASE_VERSION    0x00u

/* Reference interface version */
/** Major interface version identifies incompatible changes */
#define FBLDRVSPI_VECTORIF_API_REFERENCE_RENESASCSIH_VERSION_MAJOR     0x03u
/** Minor interface version identifies backward compatible changes */
#define FBLDRVSPI_VECTORIF_API_REFERENCE_RENESASCSIH_VERSION_MINOR     0x00u
/** Release interface version identifies cosmetic changes */
#define FBLDRVSPI_VECTORIF_API_REFERENCE_RENESASCSIH_VERSION_RELEASE   0x00u

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

/* Defines for function-like SPI interface access */
#define FblSpiRenesasCsihInit                  g_FblSpiRenesasCsih.initFct
#define FblSpiRenesasCsihDeinit                g_FblSpiRenesasCsih.deinitFct
#define FblSpiRenesasCsihTransferAsync         g_FblSpiRenesasCsih.transferAsyncFct
#define FblSpiRenesasCsihGetTransferStatus     g_FblSpiRenesasCsih.getTransferStatusFct
#define FblSpiRenesasCsihTransferSync          g_FblSpiRenesasCsih.transferSyncFct
#define FblSpiRenesasCsihCancel                g_FblSpiRenesasCsih.cancelFct
#if ( FBL_SPI_MAX_NUMBER_OF_HANDLES > 1u )
# define FblSpiRenesasCsihChangeConfiguration  g_FblSpiRenesasCsih.changeConfigurationFct
#endif /* ( FBL_SPI_MAX_NUMBER_OF_HANDLES > 1u ) */
#define FblSpiRenesasCsihSetTransferMode       g_FblSpiRenesasCsih.setTransferModeFct

/***********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 **********************************************************************************************************************/

#define FBL_SPI_RENESAS_CSIH_START_SEC_CONST_EXPORT
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */
/* SPI driver interface */
V_MEMROM0 extern V_MEMROM1 tFblSpiIf V_MEMROM2 g_FblSpiRenesasCsih;
#define FBL_SPI_RENESAS_CSIH_STOP_SEC_CONST_EXPORT
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* __FBL_SPI_RENESAS_CSIH_H__ */

/***********************************************************************************************************************
 *  END OF FILE: FBL_SPI_RENESAS_CSIH.H
 **********************************************************************************************************************/
