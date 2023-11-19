/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief        Serial Peripheral Interface Driver Public Interface
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
 *  01.00.01   2016-06-01  Rie     ESCAN00090259    Added interface version defines
 **********************************************************************************************************************/

#ifndef __FBL_SPI_IF_H__
#define __FBL_SPI_IF_H__

/***********************************************************************************************************************
 *  VERSION
 **********************************************************************************************************************/

/* ##V_CFG_MANAGEMENT ##CQProject : FblDrvSpi_VectorIf CQComponent : Implementation */
#define FBLDRVSPI_VECTORIF_VERSION           0x0100u
#define FBLDRVSPI_VECTORIF_RELEASE_VERSION   0x01u

/* Interface version */
/** Major interface version identifies incompatible changes */
#define FBLDRVSPI_VECTORIF_API_VERSION_MAJOR       0x03u
/** Minor interface version identifies backward compatible changes */
#define FBLDRVSPI_VECTORIF_API_VERSION_MINOR       0x00u
/** Release interface version identifies cosmetic changes */
#define FBLDRVSPI_VECTORIF_API_VERSION_RELEASE     0x00u

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

#if ( FBL_SPI_MAX_NUMBER_OF_HANDLES > 1u )

/* Multi-channel / -configuration use-case:
 * The first / single argument of all API functions is the spiHandle. The handle is an index into the
 * tFblSpi<...>ConfigParam structure which encodes the channel / configuration to be used. */
# define FBL_SPI_HANDLE_TYPE_ONLY            vuint8 spiHandle  /* PRQA S 3460 */ /* MD_CBD_19.4 */
# define FBL_SPI_HANDLE_TYPE_FIRST           vuint8 spiHandle, /* PRQA S 3460 */ /* MD_CBD_19.4 */
# define FBL_SPI_HANDLE_PARAM_ONLY           spiHandle
# define FBL_SPI_HANDLE_PARAM_FIRST          spiHandle,
# define FBL_SPI_HANDLE                      spiHandle

#elif ( FBL_SPI_MAX_NUMBER_OF_HANDLES == 1u )

/* Single-channel / -configuration use-case:
 * The API functions do not use the SPI handle argument. In this case the SPI driver supports only a single
 * channel / configuration. */
# define FBL_SPI_HANDLE_TYPE_ONLY            void  /* PRQA S 3460 */ /* MD_CBD_19.4 */
# define FBL_SPI_HANDLE_TYPE_FIRST           /* Not used */
# define FBL_SPI_HANDLE_PARAM_ONLY           /* Not used */
# define FBL_SPI_HANDLE_PARAM_FIRST          /* Not used */
# define FBL_SPI_HANDLE                      0u

#else /* ( FBL_SPI_MAX_NUMBER_OF_HANDLES < 1u ) */
# error "FBL_SPI_IF.H: Invalid configuration of FBL_SPI_MAX_NUMBER_OF_HANDLES (must be >= 1)"
#endif /* ( FBL_SPI_MAX_NUMBER_OF_HANDLES < 1u ) */

/***********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

/** SPI transfer status (see also FblSpi<...>GetTransferStatus()) */
typedef enum
{
   FBL_SPI_TRANSFER_STATUS_IDLE,          /**< Module is idle, no transfer pending */
   FBL_SPI_TRANSFER_STATUS_BUSY,          /**< Module is busy, transfer is pending */
   FBL_SPI_TRANSFER_STATUS_COMPLETED,     /**< Module is idle, the last transfer was successfully finished */
   FBL_SPI_TRANSFER_STATUS_FAILED,        /**< Module is idle, the last transfer failed */
   FBL_SPI_TRANSFER_STATUS_CANCELLED      /**< Module is idle, the last transfer has been canceled */
} tFblSpiTransferStatus;

/** SPI transfer mode (see also FblSpi<...>SetTransferMode()) */
typedef enum
{
   FBL_SPI_TRANSFER_MODE_SINGLE,          /**< Module uses single mode for data transfers */
   FBL_SPI_TRANSFER_MODE_DUAL_TX,         /**< Module uses dual mode for data transfers (transmission mode) */
   FBL_SPI_TRANSFER_MODE_DUAL_RX,         /**< Module uses dual mode for data transfers (reception mode) */
   FBL_SPI_TRANSFER_MODE_QUAD_TX,         /**< Module uses quad mode for data transfers (transmission mode) */
   FBL_SPI_TRANSFER_MODE_QUAD_RX          /**< Module uses quad mode for data transfers (reception mode) */
} tFblSpiTransferMode;

/** SPI chip-select mode (see also tFblSpiTransferParam) */
typedef enum
{
   FBL_SPI_CHIP_SELECT_MODE_SET,          /**< Assert/set chip select before transfer */
   FBL_SPI_CHIP_SELECT_MODE_CLR,          /**< Deassert/clear chip select after transfer */
   FBL_SPI_CHIP_SELECT_MODE_BOTH,         /**< Both assert/set and deassert/clear chip select during transfer */
   FBL_SPI_CHIP_SELECT_MODE_NONE          /**< Do not handle chip select during transfer */
} tFblSpiChipSelectMode;

/** SPI communication mode */
typedef enum
{
   FBL_SPI_COMMUNICATION_MODE_MASTER,     /**< Module uses master mode */
   FBL_SPI_COMMUNICATION_MODE_SLAVE       /**< Module uses slave mode */
} tFblSpiCommunicationMode;

/** Polling function (can be used for watchdog handling ...) */
typedef void (*tFblSpiPollingFct)( void );

/** SPI transfer parameter structure (see also FblSpi<...>TransferSync() and FblSpi<...>TransferAsync()) */
typedef struct
{
   /** Pointer to input transfer buffer, use V_NULL if data to be transmitted is non-relevant and default value shall
    *  be used. See also FBL_SPI_DUMMY_DATA in fbl_spi_<...>_cfg.h.
    *  In case a valid pointer (not V_NULL) is provided the size of transmitBuffer must be at least transferLength.*/
   const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * pTransmitBuffer;

   /** Pointer to output transfer buffer, use V_NULL if data to be received is non-relevant and shall be ignored /
    *  discarded. In case a valid pointer (not V_NULL) is provided the size of receiveBuffer must be at least
    *  transferLength. The caller might use identical buffers for both transmitBuffer and receiveBuffer which
    *  causes the input data to be overwritten by output data. */
   V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * pReceiveBuffer;

   /** Number of bytes that shall be transferred */
   vuint16 transferLength;

   /** The chip-select mode decides the way the SPI driver manipulates the chip-select pin during transfer */
   tFblSpiChipSelectMode chipSelectMode;

   /** The polling function (called by FblSpi<...>TransferSync() once the driver blocks waiting for transfer completion) */
   tFblSpiPollingFct pollingFct;

} tFblSpiTransferParam;

/* (De-)Initialization functions */

typedef tFblResult (*tFblSpiInitFct)( FBL_SPI_HANDLE_TYPE_ONLY );
typedef tFblResult (*tFblSpiDeinitFct)( FBL_SPI_HANDLE_TYPE_ONLY );

/* Transfer functions */

typedef tFblResult (*tFblSpiTransferAsyncFct)( FBL_SPI_HANDLE_TYPE_FIRST V_MEMRAM1 tFblSpiTransferParam V_MEMRAM2 V_MEMRAM3 * transferParam );
typedef tFblSpiTransferStatus (*tFblSpiGetTransferStatusFct)( FBL_SPI_HANDLE_TYPE_ONLY );
typedef tFblResult (*tFblSpiTransferSyncFct)( FBL_SPI_HANDLE_TYPE_FIRST V_MEMRAM1 tFblSpiTransferParam V_MEMRAM2 V_MEMRAM3 * transferParam );
typedef tFblResult (*tFblSpiCancelFct)( FBL_SPI_HANDLE_TYPE_ONLY );

/* Configuration functions */

#if ( FBL_SPI_MAX_NUMBER_OF_HANDLES > 1u )
typedef tFblResult (*tFblSpiChangeConfigurationFct)( FBL_SPI_HANDLE_TYPE_FIRST vuint8 newSpiHandle );
#endif /* ( FBL_SPI_MAX_NUMBER_OF_HANDLES > 1u ) */

typedef tFblResult (*tFblSpiSetTransferModeFct)( FBL_SPI_HANDLE_TYPE_FIRST tFblSpiTransferMode transferMode );

/** SPI driver interface */
typedef struct
{
   tFblSpiInitFct                   initFct;
   tFblSpiDeinitFct                 deinitFct;
   tFblSpiTransferAsyncFct          transferAsyncFct;
   tFblSpiGetTransferStatusFct      getTransferStatusFct;
   tFblSpiTransferSyncFct           transferSyncFct;
   tFblSpiCancelFct                 cancelFct;
#if ( FBL_SPI_MAX_NUMBER_OF_HANDLES > 1u )
   tFblSpiChangeConfigurationFct    changeConfigurationFct;
#endif /* ( FBL_SPI_MAX_NUMBER_OF_HANDLES > 1u ) */
   tFblSpiSetTransferModeFct        setTransferModeFct;
} tFblSpiIf;

#endif /* __FBL_SPI_IF_H__ */

/***********************************************************************************************************************
 *  END OF FILE: FBL_SPI_IF.H
 **********************************************************************************************************************/
