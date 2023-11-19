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
 *  02.01.00   2017-01-23  DKl     ESCAN00093432    Added support for P1x-C derivative
 **********************************************************************************************************************/

#ifndef __FBL_SPI_RENESAS_CSIH_CFG_H__
#define __FBL_SPI_RENESAS_CSIH_CFG_H__

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/
/* PRQA S 3453 TAG_FblSpi_3453_1 */ /* MD_CBD_19.7 */

/* CSIH base addresses */
#define FBL_SPI_CSIH0_BASE      0xFFD80000ul
#define FBL_SPI_CSIH1_BASE      0xFFD82000ul
#define FBL_SPI_CSIH2_BASE      0xFFD84000ul
#define FBL_SPI_CSIH3_BASE      0xFFD86000ul

/* Select the desired CSIH module */
#define FBL_SPI_CSIH_BASE       FBL_SPI_CSIH2_BASE

/* Select CSIH2BRS0 register used by this driver */
#define FBL_SPI_CSIH_BRSy_REG   0u /**< Baud rate setting register */

/* Select CSIH2CFG0 register used by this driver */
#define FBL_SPI_CSIH_CFGx_REG   2u /**< Configuration setting register */

/* CSIH module registers */
#define FBL_SPI_CSIH_CTL0           FBL_IOS(vuint8,  FBL_SPI_CSIH_BASE, 0x0000ul )                 /**< Control register 0 */
#define FBL_SPI_CSIH_CTL1           FBL_IOS(vuint32, FBL_SPI_CSIH_BASE, 0x0010ul )                 /**< Control register 1 */
#define FBL_SPI_CSIH_CTL2           FBL_IOS(vuint16, FBL_SPI_CSIH_BASE, 0x0014ul )                 /**< Control register 2 */
#define FBL_SPI_CSIH_STR0           FBL_IOS(vuint32, FBL_SPI_CSIH_BASE, 0x0004ul )                 /**< Status register 0 */
#define FBL_SPI_CSIH_STCR0          FBL_IOS(vuint16, FBL_SPI_CSIH_BASE, 0x0008ul )                 /**< Status clear register 0 */
#define FBL_SPI_CSIH_CFG(x)         FBL_IOS(vuint32, FBL_SPI_CSIH_BASE, 0x1044ul + ((x) * 0x04ul)) /**< Configuration register 0-7 */
#define FBL_SPI_CSIH_TX0W           FBL_IOS(vuint32, FBL_SPI_CSIH_BASE, 0x1008ul )                 /**< Transmit data register 0 - word access */
#define FBL_SPI_CSIH_TX0H           FBL_IOS(vuint16, FBL_SPI_CSIH_BASE, 0x100Cul )                 /**< Transmit data register 0 - half word access */
#define FBL_SPI_CSIH_RX0W           FBL_IOS(vuint32, FBL_SPI_CSIH_BASE, 0x1010ul )                 /**< Receive data register 0 - word access */
#define FBL_SPI_CSIH_RX0H           FBL_IOS(vuint16, FBL_SPI_CSIH_BASE, 0x1014ul )                 /**< Receive data register 0 - half word access */
#define FBL_SPI_CSIH_MCTL0          FBL_IOS(vuint16, FBL_SPI_CSIH_BASE, 0x1040ul )                 /**< Memory control register 0 */
#define FBL_SPI_CSIH_MCTL1          FBL_IOS(vuint32, FBL_SPI_CSIH_BASE, 0x1000ul )                 /**< Memory control register 1 */
#define FBL_SPI_CSIH_MCTL2          FBL_IOS(vuint32, FBL_SPI_CSIH_BASE, 0x1004ul )                 /**< Memory control register 2 */

#define FBL_SPI_CSIH_BRS(x)         FBL_IOS(vuint16, FBL_SPI_CSIH_BASE, 0x1068ul + ((x) * 0x04ul)) /**< Baud rate setting register 0-3 */

/* EI Level Interrupt Control Registers
 *   Bit : Value   - Description
 * --------------------------------------------
 *    15 : 1       - EICTn - Interrupt Channel Type  = High-level detection
 * 14-13 : 0       - reserved
 *    12 : 0       - EIRFn - Interrupt Request Flag =  No interrupt request (Value after reset)
 *  11-8 : 0       - reserved
 *     7 : 0       - EIMKn - Interrupt Mask = Interrupt processing enabled
 *     6 : 0       - EITBn - Interrupt Vector Method Select = Direct branching method based on priority
 *   5-4 : 0       - reserved
 *   3-0 : 0       - EIP3n-0n - 16 interrupt priority levels (0:highest priority,15:lowest priority) = Highest Priority
 */

/* Configure interrupt register addresses */
#if defined( V_PROCESSOR_F1L )
/* Set the communication status interrupt Address */
# define FBL_SPI_INTCSIH_IC_ADDR   0xFFFFA0D8ul
/* Set the receive status interrupt Address */
# define FBL_SPI_INTCSIH_IR_ADDR   0xFFFFA0DAul
/* Set the communication error interrupt Address */
# define FBL_SPI_INTCSIH_IRE_ADDR  0xFFFFA0DCul
#elif defined( V_PROCESSOR_F1H )
/* Set the communication status interrupt Address */
# define FBL_SPI_INTCSIH_IC_ADDR   0xFFFFB0E8ul
/* Set the receive status interrupt Address */
# define FBL_SPI_INTCSIH_IR_ADDR   0xFFFFB0EAul
/* Set the communication error interrupt Address */
# define FBL_SPI_INTCSIH_IRE_ADDR  0xFFFFB0ECul
#elif defined ( V_PROCESSOR_P1M )
/* Set the communication status interrupt Address- EIINT Channel 100 */
# define FBL_SPI_INTCSIH_IC_ADDR   0xFFFFB0C8ul
/* Set the receive status interrupt Address - EIINT Channel 99*/
# define FBL_SPI_INTCSIH_IR_ADDR   0xFFFFB0C6ul
/* Set the communication error interrupt Address - EIINT Channel 98 */
# define FBL_SPI_INTCSIH_IRE_ADDR  0xFFFFB0C4ul
#else
# error "Processor variant not supported"
#endif

/** Number of SPI configuration handles (see also g_Spi<...>ConfigParam[])
 *  For each SPI channel / configuration that shall be supported a dedicated handle is required */
#define FBL_SPI_NUMBER_OF_HANDLES         1u

/* Chip select handling:
 *
 * Use case 1: Chip-select handling is realized by caller, e.g. external flash/EEPROM or SBC driver
 *             The macros below should be left empty

 * Use case 2: SPI hardware handles chip-select pin automatically
 *             The macros below should be left empty (especially FBL_SPI_SET_CS() and
 *             FBL_SPI_CLR_CS()
 *
 * Use case 3: GPIO chip-select pin is handled manually by the macros below
 *             The macros shall be filled out with an appropriate pin handling
 *
 * Use case 4: Chip-select handling is realized by master, in case the module works in slave mode
 *             The macros below should be left empty */

/* #define FBL_SPI_ENABLE_INTERNAL_CS_HANDLING */
#define FBL_SPI_ENABLE_EXTERNAL_CS_HANDLING

/* Select the desired chip select port (internal) */
#define FBL_SPI_CSIH_CSx        2u

#if defined( FBL_SPI_ENABLE_EXTERNAL_CS_HANDLING )
#else /* FBL_SPI_ENABLE_INTERNAL_CS_HANDLING */
# define FBL_SPI_INIT_CS(spiHandle)
# define FBL_SPI_DEINIT_CS(spiHandle)
# define FBL_SPI_SET_CS(spiHandle)
# define FBL_SPI_CLR_CS(spiHandle)
#endif /* FBL_SPI_ENABLE_INTERNAL_CS_HANDLING */

/* Pin configuration:
 * Many hardware devices require the configuration of internal multiplexers, drivers ... in order to map the SPI
 * modules signals to specific output pins */
#define FBL_SPI_INIT_PINS(spiHandle)
#define FBL_SPI_DEINIT_PINS(spiHandle)

/* Clock configuration:
 * Many hardware devices require the configuration of internal clock units in order to use the SPI */
#define FBL_SPI_INIT_CLOCKS(spiHandle)
#define FBL_SPI_DEINIT_CLOCKS(spiHandle)

/* The data value being transferred in case no transmit buffer is provided */
#define FBL_SPI_DUMMY_DATA                0xFFu

/* PRQA L:TAG_FblSpi_3453_1 */
/***********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

/** SPI configuration structure */
typedef struct
{
   vuint8                     channel;    /**< Physical channel (0, 1, ...) */
   vuint32                    ctl1;       /**< Control register 1 */
   vuint16                    ctl2;       /**< Control register 2 */
   vuint16                    stcr0;      /**< Status clear register 0 */
   vuint32                    cfgx;       /**< Configuration register 0 */
   vuint16                    brsy;       /**< Baud rate setting register 0 */
} tFblSpiRenesasCsihConfigParam;

/***********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

#define FBL_SPI_RENESAS_CSIH_CONFIG_PARAM_START_SEC_CONST_EXPORT
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */
V_MEMROM0 extern V_MEMROM1 tFblSpiRenesasCsihConfigParam V_MEMROM2 g_SpiRenesasCsihConfigParam[FBL_SPI_NUMBER_OF_HANDLES];
#define FBL_SPI_RENESAS_CSIH_CONFIG_PARAM_STOP_SEC_CONST_EXPORT
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* __FBL_SPI_RENESAS_CSIH_CFG_H__ */

/***********************************************************************************************************************
 *  END OF FILE: FBL_SPI_RENESAS_CSIH_CFG.H
 **********************************************************************************************************************/
