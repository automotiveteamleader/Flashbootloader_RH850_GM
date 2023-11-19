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
 *  03.00.01   2016-06-01  Rie     ESCAN00090444    Added interface version check against used SPI IF
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

#define FBL_RENESAS_CSIH_SOURCE

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

#include "fbl_spi_renesas_csih_inc.h"
#include "fbl_spi_renesas_csih_cfg.h"

/***********************************************************************************************************************
 *  VERSION
 **********************************************************************************************************************/

#if ( FBLDRVSPI_RENESASCSIH_VERSION != 0x0201u ) || \
    ( FBLDRVSPI_RENESASCSIH_RELEASE_VERSION != 0x00u )
# error "Error in fbl_spi_<...>.c: Source and header file are inconsistent!"
#endif

/* Interface version compatibility check */
#if ( FBLDRVSPI_VECTORIF_API_VERSION_MAJOR != FBLDRVSPI_VECTORIF_API_REFERENCE_RENESASCSIH_VERSION_MAJOR ) || \
    ( FBLDRVSPI_VECTORIF_API_VERSION_MINOR > FBLDRVSPI_VECTORIF_API_REFERENCE_RENESASCSIH_VERSION_MINOR )
# error "Error in fbl_spi_<...>.c: Interface version compatibility check failed!"
#endif

/***********************************************************************************************************************
 *  CONFIGURATION CHECKS
 **********************************************************************************************************************/

#if ( FBL_SPI_NUMBER_OF_HANDLES > FBL_SPI_MAX_NUMBER_OF_HANDLES )
# error "Error in fbl_spi_<...>.c: Inconsistent configuration of number of handles!"
#endif

#if ( FBL_SPI_NUMBER_OF_HANDLES < 1u )
# error "Error in fbl_spi_<...>.c: Invalid configuration of number of handles!"
#endif /* ( FBL_SPI_NUMBER_OF_HANDLES < 1u ) */

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

/* PRQA S 0303 EOF */ /* MD_FblDrvSpi_0303 */
 
/* PRQA S 3453 TAG_FblSpi_3453_1 */ /* MD_CBD_19.7 */
#if !defined( FBL_IOS )
# define FBL_IOS(type, base, offset) (*((volatile type *)((base) + (offset))))
#endif

/* CSIHnTXE, CSIHnRXE and CSIHnMBS have to be set in combination with CSIHnPWR (see Renesas documentation) */
#define FBL_SPI_CSIH_ENABLE_PWR()   FBL_SPI_CSIH_CTL0 = 0xE0u    /**< Provide operation clock, memory mode */
#define FBL_SPI_CSIH_DISABLE_PWR()  FBL_SPI_CSIH_CTL0 = 0x00u    /**< Stop operation clock */

/* Communication Status Interrupt Flag */
#define FBL_SPI_INTCSIH_IC_IRF      ((FBL_SPI_INTCSIH_IC >> 12u) & 1u)   /**< Communication Status Interrupt request flag in FIFO mode */

/* Receive Status Interrupt Flag */
#define FBL_SPI_INTCSIH_IR_IRF      ((FBL_SPI_INTCSIH_IR >> 12u) & 1u)   /**< Receive Status Interrupt request flag in FIFO mode */

/* CSIHnSTR0 FIFO received data packets */
#define FBL_SPI_CSIH_RX_BYTES       ((FBL_SPI_CSIH_STR0 >> 24u) & 0xFFu)  /**< Indicates the number of received data packets in FIFO mode */

/* CSIHnSTR0 FIFO untransmitted data packets */
#define FBL_SPI_CSIH_TX_BYTES       ((FBL_SPI_CSIH_STR0 >> 16u) & 0xFFu)  /**< Indicates the number of untransmitted data in FIFO mode */

/* Internal Chip Select Value */
#define FBL_SPI_CSIH_CS             (FblInvert8Bit(1 << FBL_SPI_CSIH_CSx))   /**< Create CSIHnCS[7:0] value */

/* Interrupt control registers -------------------------------------------------------------------------------------- */
#define FBL_SPI_INTCSIH_IC          FBL_IOS(vuint16, FBL_SPI_INTCSIH_IC_ADDR,  0x00ul) /**< Communication status interrupt control register */
#define FBL_SPI_INTCSIH_IR          FBL_IOS(vuint16, FBL_SPI_INTCSIH_IR_ADDR,  0x00ul) /**< Receive status interrupt control register */
#define FBL_SPI_INTCSIH_IRE         FBL_IOS(vuint16, FBL_SPI_INTCSIH_IRE_ADDR, 0x00ul) /**< Communication error interrupt control register */

/* CSIH module flags ------------------------------------------------------------------------------------------------ */
#define FBL_SPI_IC_RFxxx            ((vuint16)0x1000u)     /**< Interrupt request flag */
#define FBL_SPI_STR0_ERR_FLAG       ((vuint32)0x0000C00Bu) /**< STR0 Error Flags */
/* PRQA L:TAG_FblSpi_3453_1 */

/** FIFO size (transmit and receive FIFO) */
#define FBL_SPI_FIFO_SIZE              128u

/***********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

/** SPI task status */
typedef enum
{
   FBL_SPI_TASK_STATUS_IDLE,           /**< Task is idle, no transfer pending */
   FBL_SPI_TASK_STATUS_BUSY            /**< Task is busy, transfer is pending */
} tFblSpiTaskStatus;

/***********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

#define FBL_SPI_RENESAS_CSIH_INIT_INTERNAL_START_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */
static void FblSpiInitInternal( FBL_SPI_HANDLE_TYPE_ONLY );
#define FBL_SPI_RENESAS_CSIH_INIT_INTERNAL_STOP_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define FBL_SPI_RENESAS_CSIH_TASK_START_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */
static void FblSpiTask( FBL_SPI_HANDLE_TYPE_ONLY );
#define FBL_SPI_RENESAS_CSIH_TASK_STOP_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define FBL_SPI_RENESAS_CSIH_INIT_START_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */
static tFblResult FblSpiInit( FBL_SPI_HANDLE_TYPE_ONLY );
#define FBL_SPI_RENESAS_CSIH_INIT_STOP_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define FBL_SPI_RENESAS_CSIH_DEINIT_START_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */
static tFblResult FblSpiDeinit( FBL_SPI_HANDLE_TYPE_ONLY );
#define FBL_SPI_RENESAS_CSIH_DEINIT_STOP_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define FBL_SPI_RENESAS_CSIH_TRANSFER_ASYNC_START_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */
static tFblResult FblSpiTransferAsync( FBL_SPI_HANDLE_TYPE_FIRST V_MEMRAM1 tFblSpiTransferParam V_MEMRAM2 V_MEMRAM3 * transferParam );
#define FBL_SPI_RENESAS_CSIH_TRANSFER_ASYNC_STOP_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define FBL_SPI_RENESAS_CSIH_GET_TRANSFER_STATUS_START_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */
static tFblSpiTransferStatus FblSpiGetTransferStatus( FBL_SPI_HANDLE_TYPE_ONLY );
#define FBL_SPI_RENESAS_CSIH_GET_TRANSFER_STATUS_STOP_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define FBL_SPI_RENESAS_CSIH_TRANSFER_SYNC_START_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */
static tFblResult FblSpiTransferSync( FBL_SPI_HANDLE_TYPE_FIRST V_MEMRAM1 tFblSpiTransferParam V_MEMRAM2 V_MEMRAM3 * transferParam );
#define FBL_SPI_RENESAS_CSIH_TRANSFER_SYNC_STOP_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define FBL_SPI_RENESAS_CSIH_CANCEL_START_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */
static tFblResult FblSpiCancel( FBL_SPI_HANDLE_TYPE_ONLY );
#define FBL_SPI_RENESAS_CSIH_CANCEL_STOP_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if ( FBL_SPI_MAX_NUMBER_OF_HANDLES > 1u )
#define FBL_SPI_RENESAS_CSIH_CHANGE_CONFIGURATION_START_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */
static tFblResult FblSpiChangeConfiguration( vuint8 spiHandle, vuint8 newSpiHandle );
#define FBL_SPI_RENESAS_CSIH_CHANGE_CONFIGURATION_STOP_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */
#endif /* ( FBL_SPI_MAX_NUMBER_OF_HANDLES > 1u ) */

#define FBL_SPI_RENESAS_CSIH_SET_TRANSFER_MODE_START_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */
static tFblResult FblSpiSetTransferMode( FBL_SPI_HANDLE_TYPE_FIRST tFblSpiTransferMode transferMode );
#define FBL_SPI_RENESAS_CSIH_SET_TRANSFER_MODE_STOP_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/***********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

#define FBL_SPI_RENESAS_CSIH_START_SEC_CONST
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */
/* SPI driver interface */
V_MEMROM0 V_MEMROM1 tFblSpiIf V_MEMROM2 g_FblSpiRenesasCsih =
{
   FblSpiInit,
   FblSpiDeinit,
   FblSpiTransferAsync,
   FblSpiGetTransferStatus,
   FblSpiTransferSync,
   FblSpiCancel,
#if ( FBL_SPI_MAX_NUMBER_OF_HANDLES > 1u )
   FblSpiChangeConfiguration,
#endif /* ( FBL_SPI_MAX_NUMBER_OF_HANDLES > 1u ) */
   FblSpiSetTransferMode
};
#define FBL_SPI_RENESAS_CSIH_STOP_SEC_CONST
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/***********************************************************************************************************************
 *  LOCAL DATA
 **********************************************************************************************************************/

/** The temporarily buffered transfer parameter provided with FblSpiTransferAsync() / FblSpiTransferSync() */
V_MEMRAM0 static V_MEMRAM1 tFblSpiTransferParam V_MEMRAM2 V_MEMRAM3 * g_TransferParam[FBL_SPI_NUMBER_OF_HANDLES];

/** The current transfer status (transfer status for all data provided with g_TransferParam[]) */
V_MEMRAM0 static V_MEMRAM1 tFblSpiTransferStatus V_MEMRAM2 g_TransferStatus[FBL_SPI_NUMBER_OF_HANDLES];

/** The current task status (transfer status for data fragments, e.g. in case a FIFO is used) */
V_MEMRAM0 static V_MEMRAM1 tFblSpiTaskStatus V_MEMRAM2 g_TaskStatus[FBL_SPI_NUMBER_OF_HANDLES];

/** The already transferred length */
V_MEMRAM0 static V_MEMRAM1 vuint16 V_MEMRAM2 g_TransferredLength[FBL_SPI_NUMBER_OF_HANDLES];

/** The current pending length */
V_MEMRAM0 static V_MEMRAM1 vuint16 V_MEMRAM2 g_PendingLength[FBL_SPI_NUMBER_OF_HANDLES];

/***********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/

#define FBL_SPI_RENESAS_CSIH_INIT_INTERNAL_START_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */
/***********************************************************************************************************************
 *  FblSpiInitInternal
 **********************************************************************************************************************/
/*! \brief           Initialize the modules global states
 *  \param[in]       spiHandle Optional: Specifies the SPI configuration entry in g_Spi<...>ConfigParam
 **********************************************************************************************************************/
static void FblSpiInitInternal( FBL_SPI_HANDLE_TYPE_ONLY )
{
   g_TransferParam[FBL_SPI_HANDLE] = V_NULL;
   g_TransferStatus[FBL_SPI_HANDLE] = FBL_SPI_TRANSFER_STATUS_IDLE;
   g_TaskStatus[FBL_SPI_HANDLE] = FBL_SPI_TASK_STATUS_IDLE;
   g_TransferredLength[FBL_SPI_HANDLE] = 0u;
   g_PendingLength[FBL_SPI_HANDLE] = 0u;
}
#define FBL_SPI_RENESAS_CSIH_INIT_INTERNAL_STOP_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define FBL_SPI_RENESAS_CSIH_TASK_START_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */
/***********************************************************************************************************************
 *  FblSpiTask
 **********************************************************************************************************************/
/*! \brief           Handles the actual data transfer
 *  \param[in]       spiHandle Optional: Specifies the SPI configuration entry in g_Spi<...>ConfigParam
 **********************************************************************************************************************/
static void FblSpiTask( FBL_SPI_HANDLE_TYPE_ONLY )
{
   vuint16 transferIndex;
   vuint16 remainingLength;
   vuint16 i;

   if (FBL_SPI_TASK_STATUS_IDLE == g_TaskStatus[FBL_SPI_HANDLE])
   {
      /* Clear tx and rx indication */
      FBL_SPI_INTCSIH_IC &= FblInvert16Bit(FBL_SPI_IC_RFxxx); /* PRQA S 0277 */ /* MD_FblSpiRenesas_0277 */
      FBL_SPI_INTCSIH_IR &= FblInvert16Bit(FBL_SPI_IC_RFxxx); /* PRQA S 0277 */ /* MD_FblSpiRenesas_0277 */

      /* Do the actual (partial) transfer without blocking */
      remainingLength = g_TransferParam[FBL_SPI_HANDLE]->transferLength - g_TransferredLength[FBL_SPI_HANDLE];

      /* Get the total pending length */
      g_PendingLength[FBL_SPI_HANDLE] = ((remainingLength > FBL_SPI_FIFO_SIZE) ? FBL_SPI_FIFO_SIZE : remainingLength);

      /* Set trigger for INTCSIHTIR interrupt (receive data full) */
      FBL_SPI_CSIH_MCTL1 = (FBL_SPI_FIFO_SIZE - g_PendingLength[FBL_SPI_HANDLE]) & 0x0000007Fu;

      /* Do the data transmission
       * The for-loop is intentionally duplicated to avoid the if-condition check in each cycle */
      if (V_NULL == g_TransferParam[FBL_SPI_HANDLE]->pTransmitBuffer)
      {
         for (i = 0u; i < g_PendingLength[FBL_SPI_HANDLE]; i++)
         {
            /* Transmit FBL_SPI_DUMMY_DATA */
            FBL_SPI_CSIH_TX0W = ((FBL_SPI_CSIH_CS << 16u) | FBL_SPI_DUMMY_DATA); /* PRQA S 0277 */ /* MD_FblSpiRenesas_0277 */
         }
       }
      else /* V_NULL != pTransmitBuffer */
      {
         transferIndex = g_TransferredLength[FBL_SPI_HANDLE];

         for (i = 0u; i < g_PendingLength[FBL_SPI_HANDLE]; i++)
         {
            /* Transmit N-1 data bytes */
            FBL_SPI_CSIH_TX0W = ((FBL_SPI_CSIH_CS << 16u) | g_TransferParam[FBL_SPI_HANDLE]->pTransmitBuffer[transferIndex + i]); /* PRQA S 0277 */ /* MD_FblSpiRenesas_0277 */
         }
      }

      g_TaskStatus[FBL_SPI_HANDLE] = FBL_SPI_TASK_STATUS_BUSY;
   }
   else /* FBL_SPI_TASK_STATUS_BUSY */
   {
      if ((FBL_SPI_CSIH_TX_BYTES == 0)   &&   /* TX FIFO counter zero */
          (FBL_SPI_INTCSIH_IC_IRF == 1)  &&   /* Tx Interrupt flag */
          (FBL_SPI_CSIH_RX_BYTES > 0))        /* RX FIFO counter is not zero */
      {
         transferIndex = g_TransferredLength[FBL_SPI_HANDLE];
         g_TransferredLength[FBL_SPI_HANDLE] += g_PendingLength[FBL_SPI_HANDLE];

         /* Do the data reception
          * The for-loop is intentionally duplicated to avoid the if-condition check in each cycle */
         if (V_NULL == g_TransferParam[FBL_SPI_HANDLE]->pReceiveBuffer)
         {
            for (i = 0u; i < g_PendingLength[FBL_SPI_HANDLE]; i++)
            {
               /* Clean receive FIFO / registers */
               (void)FBL_SPI_CSIH_RX0H;
            }
         }
         else /* V_NULL != pReceiveBuffer */
         {
            for (i = 0u; i < g_PendingLength[FBL_SPI_HANDLE]; i++)
            {
               /* Read receive buffer */
               g_TransferParam[FBL_SPI_HANDLE]->pReceiveBuffer[transferIndex + i] = (vuint8)(FBL_SPI_CSIH_RX0H & 0xFFu);
            }
         }

         g_PendingLength[FBL_SPI_HANDLE] = 0u;
         g_TaskStatus[FBL_SPI_HANDLE] = FBL_SPI_TASK_STATUS_IDLE;

         if (g_TransferredLength[FBL_SPI_HANDLE] == g_TransferParam[FBL_SPI_HANDLE]->transferLength)
         {
#if defined( FBL_SPI_ENABLE_INTERNAL_CS_HANDLING )
            /* Handle chip-select if requested */
            if (   (FBL_SPI_CHIP_SELECT_MODE_CLR == g_TransferParam[FBL_SPI_HANDLE]->chipSelectMode)
                || (FBL_SPI_CHIP_SELECT_MODE_BOTH == g_TransferParam[FBL_SPI_HANDLE]->chipSelectMode))
            {
               FBL_SPI_CLR_CS(FBL_SPI_HANDLE_PARAM_ONLY);
            }
#endif /* FBL_SPI_ENABLE_INTERNAL_CS_HANDLING */

            g_TransferStatus[FBL_SPI_HANDLE] = FBL_SPI_TRANSFER_STATUS_COMPLETED;
            g_TransferredLength[FBL_SPI_HANDLE] = 0u;
            g_TransferParam[FBL_SPI_HANDLE] = V_NULL;

            /* Clear FIFO of unused receive bytes */
            while ((FBL_SPI_CSIH_STR0 & 0xFF000000ul) != 0)
            {
               (void)FBL_SPI_CSIH_RX0H;
            }

            /* Clear FIFO Tx and Rx indication */
            FBL_SPI_INTCSIH_IC &= FblInvert16Bit(FBL_SPI_IC_RFxxx); /* PRQA S 0277 */ /* MD_FblSpiRenesas_0277 */
            FBL_SPI_INTCSIH_IR &= FblInvert16Bit(FBL_SPI_IC_RFxxx); /* PRQA S 0277 */ /* MD_FblSpiRenesas_0277 */
         }
      }
   }
} /* PRQA S 6030 */ /* MD_MSR_STCYC */
#define FBL_SPI_RENESAS_CSIH_TASK_STOP_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/***********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

#define FBL_SPI_RENESAS_CSIH_INIT_START_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */
/***********************************************************************************************************************
 *  FblSpiInit
 **********************************************************************************************************************/
/*! \brief           Initialize serial interface
 *  \param[in]       spiHandle Optional: Specifies the SPI configuration entry in g_Spi<...>ConfigParam
 *  \return          kFblOk if initialization succeeded, otherwise kFblFailed
 **********************************************************************************************************************/
static tFblResult FblSpiInit( FBL_SPI_HANDLE_TYPE_ONLY )
{
   FBL_SPI_INIT_CLOCKS(FBL_SPI_HANDLE_PARAM_ONLY);
   FBL_SPI_INIT_PINS(FBL_SPI_HANDLE_PARAM_ONLY);

   /* Disable operation clock */
   FBL_SPI_CSIH_DISABLE_PWR();

   /* Init baud rate setting register */
   FBL_SPI_CSIH_BRS(FBL_SPI_CSIH_BRSy_REG) = g_SpiRenesasCsihConfigParam[FBL_SPI_HANDLE].brsy;

   /* Init configuration register */
   FBL_SPI_CSIH_CFG(FBL_SPI_CSIH_CFGx_REG) = g_SpiRenesasCsihConfigParam[FBL_SPI_HANDLE].cfgx;

   /* Init control register 1 */
   FBL_SPI_CSIH_CTL1 = g_SpiRenesasCsihConfigParam[FBL_SPI_HANDLE].ctl1;

   /* Init control register 2 */
   FBL_SPI_CSIH_CTL2 = g_SpiRenesasCsihConfigParam[FBL_SPI_HANDLE].ctl2;

   /* Set FIFO Mode and timeout to 0 in master mode */
   FBL_SPI_CSIH_MCTL0 = 0u;

   /* Clear FIFO pointers and status flags */
   FBL_SPI_CSIH_STCR0 |= g_SpiRenesasCsihConfigParam[FBL_SPI_HANDLE].stcr0;

   /* Clear tx and rx indication */
   FBL_SPI_INTCSIH_IC &= FblInvert16Bit(FBL_SPI_IC_RFxxx); /* PRQA S 0277 */ /* MD_FblSpiRenesas_0277 */
   FBL_SPI_INTCSIH_IR &= FblInvert16Bit(FBL_SPI_IC_RFxxx); /* PRQA S 0277 */ /* MD_FblSpiRenesas_0277 */

#if defined( FBL_SPI_ENABLE_INTERNAL_CS_HANDLING )
   FBL_SPI_INIT_CS(FBL_SPI_HANDLE_PARAM_ONLY);
   FBL_SPI_CLR_CS(FBL_SPI_HANDLE_PARAM_ONLY);
#endif /* FBL_SPI_ENABLE_INTERNAL_CS_HANDLING */

   /* Enable SPI */
   FBL_SPI_CSIH_ENABLE_PWR();

   /* Initialize SPI module global states */
   FblSpiInitInternal(FBL_SPI_HANDLE_PARAM_ONLY);

   return kFblOk;
}
#define FBL_SPI_RENESAS_CSIH_INIT_STOP_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define FBL_SPI_RENESAS_CSIH_DEINIT_START_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */
/***********************************************************************************************************************
 *  FblSpiDeinit
 **********************************************************************************************************************/
/*! \brief           Deinitialize serial interface
 *  \param[in]       spiHandle Optional: Specifies the SPI configuration entry in g_Spi<...>ConfigParam
 *  \return          kFblOk if deinitialization succeeded, otherwise kFblFailed
 **********************************************************************************************************************/
static tFblResult FblSpiDeinit( FBL_SPI_HANDLE_TYPE_ONLY )
{
   /* Disable SPI module */
   FBL_SPI_CSIH_DISABLE_PWR();

#if defined( FBL_SPI_ENABLE_INTERNAL_CS_HANDLING )
   FBL_SPI_CLR_CS(FBL_SPI_HANDLE_PARAM_ONLY);
   FBL_SPI_DEINIT_CS(FBL_SPI_HANDLE_PARAM_ONLY);
#endif /* FBL_SPI_ENABLE_INTERNAL_CS_HANDLING */

   FBL_SPI_DEINIT_PINS(FBL_SPI_HANDLE_PARAM_ONLY);
   FBL_SPI_DEINIT_CLOCKS(FBL_SPI_HANDLE_PARAM_ONLY);

   FblSpiInitInternal(FBL_SPI_HANDLE_PARAM_ONLY);

   return kFblOk;
}
#define FBL_SPI_RENESAS_CSIH_DEINIT_STOP_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define FBL_SPI_RENESAS_CSIH_TRANSFER_ASYNC_START_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */
/***********************************************************************************************************************
 *  FblSpiTransferAsync
 **********************************************************************************************************************/
/*! \brief           Transfers the given block of data asynchronously (function does not block)
 *  \param[in]       spiHandle Optional: Specifies the SPI configuration entry in g_Spi<...>ConfigParam
 *  \param[in,out]   transferParam The transfer parameter structure
 *  \return          kFblOk if transfer succeeded, otherwise kFblFailed
 **********************************************************************************************************************/
static tFblResult FblSpiTransferAsync( FBL_SPI_HANDLE_TYPE_FIRST V_MEMRAM1 tFblSpiTransferParam V_MEMRAM2 V_MEMRAM3 * transferParam )
{
   tFblResult result;

   /* The function argument checks are intentionally omitted, we assume the caller to use the module correctly */

   if (FBL_SPI_TRANSFER_STATUS_BUSY == g_TransferStatus[FBL_SPI_HANDLE])
   {
      result = kFblFailed;
   }
   else
   {
#if defined( FBL_SPI_ENABLE_INTERNAL_CS_HANDLING )
      /* Handle chip-select if requested */
      if (   (FBL_SPI_CHIP_SELECT_MODE_SET == transferParam->chipSelectMode)
          || (FBL_SPI_CHIP_SELECT_MODE_BOTH == transferParam->chipSelectMode))
      {
         FBL_SPI_SET_CS(FBL_SPI_HANDLE_PARAM_ONLY);
      }
#endif /* FBL_SPI_ENABLE_INTERNAL_CS_HANDLING */

      g_TransferParam[FBL_SPI_HANDLE] = transferParam;
      g_TransferStatus[FBL_SPI_HANDLE] = FBL_SPI_TRANSFER_STATUS_BUSY;

      /* Immediately trigger first transfer:
       * - Master mode: Reduces latencies, i.e. improves performance
       * - Slave mode: Callee typically assumes that data has been transferred to SPI registers once function returns */
      FblSpiTask(FBL_SPI_HANDLE_PARAM_ONLY);

      result = kFblOk;
   }

   return result;
}
#define FBL_SPI_RENESAS_CSIH_TRANSFER_ASYNC_STOP_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define FBL_SPI_RENESAS_CSIH_GET_TRANSFER_STATUS_START_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */
/***********************************************************************************************************************
 *  FblSpiGetTransferStatus
 **********************************************************************************************************************/
/*! \brief           Handles the actual transfer of the data provided to FblSpiTransferAsync() /
 *                   FblSpiTransferSync() and provides the transfer status
 *  \param[in]       spiHandle Optional: Specifies the SPI configuration entry in g_Spi<...>ConfigParam
 *  \return          The transfer status
 **********************************************************************************************************************/
static tFblSpiTransferStatus FblSpiGetTransferStatus( FBL_SPI_HANDLE_TYPE_ONLY )
{
   if (FBL_SPI_TRANSFER_STATUS_BUSY == g_TransferStatus[FBL_SPI_HANDLE])
   {
      FblSpiTask(FBL_SPI_HANDLE_PARAM_ONLY);
   }

   return g_TransferStatus[FBL_SPI_HANDLE];
}
#define FBL_SPI_RENESAS_CSIH_GET_TRANSFER_STATUS_STOP_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define FBL_SPI_RENESAS_CSIH_TRANSFER_SYNC_START_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */
/***********************************************************************************************************************
 *  FblSpiTransferSync
 **********************************************************************************************************************/
/*! \brief           Transfers the given block of data synchronously (function blocks)
 *  \param[in]       spiHandle Optional: Specifies the SPI configuration entry in g_Spi<...>ConfigParam
 *  \param[in,out]   transferParam The transfer parameter structure
 *  \return          kFblOk if transfer succeeded, otherwise kFblFailed
 **********************************************************************************************************************/
static tFblResult FblSpiTransferSync( FBL_SPI_HANDLE_TYPE_FIRST V_MEMRAM1 tFblSpiTransferParam V_MEMRAM2 V_MEMRAM3 * transferParam )
{
   tFblResult result;
   tFblSpiTransferStatus transferStatus;

   result = FblSpiTransferAsync(FBL_SPI_HANDLE_PARAM_FIRST transferParam);

   if (kFblOk == result)
   {
      do
      {
         transferStatus = FblSpiGetTransferStatus(FBL_SPI_HANDLE_PARAM_ONLY);

         if (V_NULL != transferParam->pollingFct)
         {
            transferParam->pollingFct();
         }
      }
      while (FBL_SPI_TRANSFER_STATUS_BUSY == transferStatus);

      if (FBL_SPI_TRANSFER_STATUS_COMPLETED != transferStatus)
      {
         result = kFblFailed;
      }
   }

   return result;
}
#define FBL_SPI_RENESAS_CSIH_TRANSFER_SYNC_STOP_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define FBL_SPI_RENESAS_CSIH_CANCEL_START_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */
/***********************************************************************************************************************
 *  FblSpiCancel
 **********************************************************************************************************************/
/*! \brief           Cancels any pending transfers
 *  \param[in]       spiHandle Optional: Specifies the SPI configuration entry in g_Spi<...>ConfigParam
 *  \return          kFblOk if cancellation succeeded, otherwise kFblFailed
 **********************************************************************************************************************/
static tFblResult FblSpiCancel( FBL_SPI_HANDLE_TYPE_ONLY )
{
   tFblResult result;

   result = FblSpiDeinit(FBL_SPI_HANDLE_PARAM_ONLY);

   if (kFblOk == result)
   {
      result = FblSpiInit(FBL_SPI_HANDLE_PARAM_ONLY);
   }

   /* Override FBL_SPI_TRANSFER_STATUS_IDLE which has been set in initialization function */
   g_TransferStatus[FBL_SPI_HANDLE] = FBL_SPI_TRANSFER_STATUS_CANCELLED;

   return result;
}
#define FBL_SPI_RENESAS_CSIH_CANCEL_STOP_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if ( FBL_SPI_MAX_NUMBER_OF_HANDLES > 1u )
# define FBL_SPI_RENESAS_CSIH_CHANGE_CONFIGURATION_START_SEC_CODE
# include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */
/***********************************************************************************************************************
 *  FblSpiChangeConfiguration
 **********************************************************************************************************************/
/*! \brief           Changes the configuration of a SPI module
 *  \details         This function is intended to be used in case there are multiple users with different SPI
 *                   configuration requirements sharing a single channel.
 *  \param[in]       spiHandle Specifies the SPI configuration entry in g_Spi<...>ConfigParam
 *  \param[in]       newSpiHandle Specifies the SPI configuration entry to be used for new configuration
 *  \return          kFblOk is operation succeeded, otherwise kFblFailed
 **********************************************************************************************************************/
static tFblResult FblSpiChangeConfiguration( vuint8 spiHandle, vuint8 newSpiHandle )
{
   tFblResult result;

   result = FblSpiDeinit(spiHandle);

   if (kFblOk == result)
   {
      result = FblSpiInit(newSpiHandle);
   }

   return result;
}
# define FBL_SPI_RENESAS_CSIH_CHANGE_CONFIGURATION_STOP_SEC_CODE
# include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */
#endif /* ( FBL_SPI_MAX_NUMBER_OF_HANDLES > 1u ) */

#define FBL_SPI_RENESAS_CSIH_SET_TRANSFER_MODE_START_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */
/***********************************************************************************************************************
 *  FblSpiSetTransferMode
 **********************************************************************************************************************/
/*! \brief           Changes the transfer mode of a SPI module
 *  \details         The transfer mode configures the usage of the SPI pins during transfer. Typical SPI slave devices
 *                   use single mode (full-duplex), some memory devices use dual- or quad-mode to improve performance
 *                   (half-duplex, multiple pins are used for transfer in the respective direction).
 *  \param[in]       spiHandle Optional: Specifies the SPI configuration entry in g_Spi<...>ConfigParam
 *  \param[in]       transferMode The transfer mode to be used
 *  \return          kFblOk is operation succeeded, otherwise kFblFailed
 **********************************************************************************************************************/
static tFblResult FblSpiSetTransferMode( FBL_SPI_HANDLE_TYPE_FIRST tFblSpiTransferMode transferMode )
{
   tFblResult result;

#if defined( V_ENABLE_USE_DUMMY_STATEMENT )
# if ( FBL_SPI_MAX_NUMBER_OF_HANDLES > 1u )
   (void)FBL_SPI_HANDLE_PARAM_ONLY;
# endif /* ( FBL_SPI_MAX_NUMBER_OF_HANDLES > 1u ) */
#endif /* V_ENABLE_USE_DUMMY_STATEMENT */

   if (FBL_SPI_TRANSFER_MODE_SINGLE == transferMode)
   {
      /* Mode supported, nothing to be done */
      result = kFblOk;
   }
   else
   {
      /* Mode not supported */
      result = kFblFailed;
   }

   return result;
}
#define FBL_SPI_RENESAS_CSIH_SET_TRANSFER_MODE_STOP_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/***********************************************************************************************************************
 *  MISRA DEVIATIONS
 **********************************************************************************************************************/

/* Module specific MISRA deviations:

 MD_FblSpiRenesas_0277:
      Reason:Bit-width of a non-basic type may not be trivial as it could be configuration dependent.
       Cast of negative one to unsigned type is used to get maximum representable value (all bits set).
      Risk: Although this conversion is guaranteed by the C standard (6.3.1.3/2) it may not be obvious.
      Prevention: Document intend and explanation in code.

   MD_FblDrvSpi_0303:
     Reason: Casting of integer to pointer type is required through memory mapped register.
     Risk: No risk.
     Prevention: No prevention.
*/

/***********************************************************************************************************************
 *  END OF FILE: FBL_SPI_RENESAS_CSIH.C
 **********************************************************************************************************************/
