/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief        Transceiver Driver (TJA1145)
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

#ifndef __SPI_H__
#define __SPI_H__

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

/* MSR SPI channel handles */
/* Handles for transceiver channel 0 */
#define SpiConf_SpiChannel_SpiChannel        0u
#define SpiConf_SpiChannel_SpiChannel_001    1u
#define SpiConf_SpiChannel_SpiChannel_002    2u
#define SpiConf_SpiChannel_SpiChannel_003    3u

/* Handles for transceiver channel 1 */
#define SpiConf_SpiChannel_SpiChannel_004    4u
#define SpiConf_SpiChannel_SpiChannel_005    5u
#define SpiConf_SpiChannel_SpiChannel_006    6u
#define SpiConf_SpiChannel_SpiChannel_007    7u

/* Handles for transceiver channel 2 */
#define SpiConf_SpiChannel_SpiChannel_008    8u
#define SpiConf_SpiChannel_SpiChannel_009    9u
#define SpiConf_SpiChannel_SpiChannel_010    10u
#define SpiConf_SpiChannel_SpiChannel_011    11u

/* Handles for transceiver channel 3 */
#define SpiConf_SpiChannel_SpiChannel_012    12u
#define SpiConf_SpiChannel_SpiChannel_013    13u
#define SpiConf_SpiChannel_SpiChannel_014    14u
#define SpiConf_SpiChannel_SpiChannel_015    15u

/* MSR SPI sequence handles */
/* Handles for transceiver channel 0 */
#define SpiConf_SpiSequence_SpiSequence      0u
#define SpiConf_SpiSequence_SpiSequence_001  1u
#define SpiConf_SpiSequence_SpiSequence_002  2u

/* Handles for transceiver channel 1 */
#define SpiConf_SpiSequence_SpiSequence_003  0u
#define SpiConf_SpiSequence_SpiSequence_004  1u
#define SpiConf_SpiSequence_SpiSequence_005  2u

/* Handles for transceiver channel 2 */
#define SpiConf_SpiSequence_SpiSequence_006  0u
#define SpiConf_SpiSequence_SpiSequence_007  1u
#define SpiConf_SpiSequence_SpiSequence_008  2u

/* Handles for transceiver channel 4 */
#define SpiConf_SpiSequence_SpiSequence_009  0u
#define SpiConf_SpiSequence_SpiSequence_010  1u
#define SpiConf_SpiSequence_SpiSequence_011  2u

/***********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

typedef uint8 Spi_ChannelType;            /**< MSR SPI channel handle */
typedef uint8 Spi_SequenceType;           /**< MSR SPI sequence handle */
typedef uint8 Spi_DataType;               /**< MSR SPI data type */
typedef uint16 Spi_NumberOfDataType;      /**< MSR SPI data length type */

typedef P2VAR(Spi_DataType, AUTOMATIC, SPI_APPL_DATA) Spi_DataPtrType;           /**< Pointer to MSR SPI data type */
typedef P2CONST(Spi_DataType, AUTOMATIC, SPI_APPL_DATA) Spi_DataConstPtrType;    /**< Pointer to MSR SPI data length type */

/***********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

FUNC(Std_ReturnType, SPI_CODE) Spi_SetupEB( Spi_ChannelType Channel, Spi_DataConstPtrType SrcDataBufferPtr,
   Spi_DataPtrType      DesDataBufferPtr, Spi_NumberOfDataType Length );

FUNC(Std_ReturnType, SPI_CODE) Spi_SyncTransmit( Spi_SequenceType Sequence );

#endif /* __SPI_H__ */
