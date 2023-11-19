/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief        Serial Peripheral Interface Driver (Renesas Clocked Serial Interface H)
 *
 *  \note         Please note, that this file contains an implementation/configuration example for the SPI driver.
 *                This code may influence the behavior of the SPI driver in principle. Therefore, great care must be
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
 *  03.00.01   2016-06-01  Rie     ESCAN00090444    No changes
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

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

#include "fbl_spi_renesas_csih_inc.h"
#include "fbl_spi_renesas_csih_cfg.h"

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

/* Configure register initialization values */

/* Control register 1
 *   Bit : Value   - Description
 * --------------------------------------------
 * 31-19 : 0       - reserved
 *    18 : 0       - PHE - High-priority comm disabled
 *    17 : 1       - CKR - Default level of SCK is low
 *    16 : 0       - SLIT - INTCSIHTIC generated after transfer(Normal)
 *  15-8 : 0       - CSL[7:0] - chip select is active low
 *     7 : 0       - EDLE - Disable extended data length mode
 *     6 : 0       - JE - Disable job mode
 *     5 : 0       - DCS - Disable data consistency check
 *     4 : 1       - CSRI - Chip select returns to inactive level
 *     3 : 0       - LBM - Deactivate loop back mode
 *     2 : 0       - SIT - No interrupt delay
 *     1 : 0       - HSE - Disable handshake function
 *     0 : 0       - SSE - Input signal ~CSIHTSSI is ignored
 */
#define FBL_SPI_CSIH_CTL1_VAL   0x00020010u

/* Control register 2
 *   Bit : Value   - Description
 * --------------------------------------------
 * 15-13 : 011     - PCLK/8 (Master Mode)
 *  12-0 : 0       - reserved
 */
#define FBL_SPI_CSIH_CTL2_VAL   0xC000u

/* Status clear register 0
 *   Bit : Value   - Description
 * --------------------------------------------
 *    15 : 1       - TMOEC - Clear time out error flag
 *    14 : 1       - OFEC - Clear overflow error flag
 *  13-9 : 0       - reserved
 *     8 : 0       - PCT - FIFO pointer clear command
 *   7-4 : 0       - reserved
 *     3 : 1       - DCEC - Clear data consistency error flags
 *     2 : 0       - reserved
 *     1 : 1       - PEC - Clear parity error flag
 *     0 : 1       - OVEC - Clear overrun error flag
 */
#define FBL_SPI_CSIH_STCR0_VAL  0xC00Bu

/* Configuration register
 *   Bit : Value   - Description
 * --------------------------------------------
 * 31-30 : 0       - BRSSx - Transfer clock set according to CSIHnBRS0
 * 29-28 : 0       - PSx - No parity
 * 27-24 : 8       - DLSx - Data length 8 bits
 * 23-20 : 0       - reserved
 *    19 : 0       - RCBx - Dominant configuration for broadcasting CS
 *    18 : 0       - DIRx - MSB first
 *    17 : 0       - CKPx - Clock phase
 *    16 : 0       - DAPx - Data phase
 *    15 : 0       - IDLx - N/A
 * 14-12 : 0       - IDx - Idle time for CS
 *  11-8 : 0       - HDx - Hold time for CS
 *   7-4 : 0       - INx - Inter-delay time for CS
 *   3-0 : 0       - SPx - Setup time for CS
 */
#define FBL_SPI_CSIH_CFGx_VAL    0x08000000u   /* P3_4 = CSIHTCSS2 */

/* Baud rate setting register
 *   Bit : Value   - Description
 * --------------------------------------------
 * 15-12 : 0       - reserved
 *  11-0 : 1       - BRS - PCLK/(2a x BRS x 2) = 80MHz/(2^5 *1 *2) = 1.25 MHz
 */
#define FBL_SPI_CSIH_BRSy_VAL   0x0001u

/***********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

#define FBL_SPI_RENESAS_CSIH_CONFIG_PARAM_START_SEC_CONST
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */
/** SPI configuration structure
 *  This structure has to provide as many entries as configurations / channels need to be supported
 *  (see also FBL_SPI_NUMBER_OF_HANDLES in fbl_spi_<...>_cfg.h) */
V_MEMROM0 V_MEMROM1 tFblSpiRenesasCsihConfigParam V_MEMROM2 g_SpiRenesasCsihConfigParam[FBL_SPI_NUMBER_OF_HANDLES] =
{
   /* Initialization structure for SPI module */
   {
      0u,
      FBL_SPI_CSIH_CTL1_VAL,
      FBL_SPI_CSIH_CTL2_VAL,
      FBL_SPI_CSIH_STCR0_VAL,
      FBL_SPI_CSIH_CFGx_VAL,
      FBL_SPI_CSIH_BRSy_VAL
   }
};
#define FBL_SPI_RENESAS_CSIH_CONFIG_PARAM_STOP_SEC_CONST
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/***********************************************************************************************************************
 *  END OF FILE: FBL_SPI_RENESAS_CSIH_CFG.C
 **********************************************************************************************************************/
