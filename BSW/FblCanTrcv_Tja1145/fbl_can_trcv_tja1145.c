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
 *                                 ESCAN00091072    Added support for timeout handling
 *  01.02.01   2016-08-04  AWh     ESCAN00092769    No changes
 *  01.02.02   2016-11-18  Rie     ESCAN00092883    Removed EcuM_SetWakeupEvent and EcuM_CheckWakeup
 **********************************************************************************************************************/

#define FBL_CAN_TRCV_TJA1145_SOURCE

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

#include "fbl_can_trcv_tja1145_inc.h"
#include "fbl_wd.h"

#if defined( FBL_CAN_TRCV_TJA1145_ENABLE_MSR_CAN_TRCV_USAGE )
# include "Std_Types.h"
# include "Spi.h"
# include "EcuM.h"
# include "CanTrcv_30_Tja1145.h"
#endif /* FBL_CAN_TRCV_TJA1145_ENABLE_MSR_CAN_TRCV_USAGE */

/***********************************************************************************************************************
 *  VERSION
 **********************************************************************************************************************/

#if ( FBLDRVTRANS_TJA1145CANSPI_VERSION != 0x0102u ) || \
    ( FBLDRVTRANS_TJA1145CANSPI_RELEASE_VERSION != 0x02u )
# error "Error in fbl_can_trcv_tja1145.c: Source and header file are inconsistent!"
#endif   /* PRQA S 0883 */ /* MD_FblCanTrcv_0883 */

/***********************************************************************************************************************
 *  CONFIGURATION CHECKS
 **********************************************************************************************************************/

#if defined( FBL_CAN_TRCV_TJA1145_ENABLE_MSR_CAN_IF_USAGE )
# if defined( FBL_CAN_TRCV_TJA1145_ENABLE_MSR_CAN_TRCV_USAGE )
# else /* FBL_CAN_TRCV_TJA1145_DISABLE_MSR_CAN_TRCV_USAGE */
#  error "Error in fbl_can_trcv_tja1145.c: Invalid configuration selected"
# endif /* FBL_CAN_TRCV_TJA1145_DISABLE_MSR_CAN_TRCV_USAGE */
#endif /* FBL_CAN_TRCV_TJA1145_ENABLE_MSR_CAN_IF_USAGE */

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

#if defined( FBL_CAN_TRCV_TJA1145_ENABLE_MSR_CAN_TRCV_USAGE )

/** Number of channels / jobs, 4 jobs are required by the CAN transceiver (SeqRWDataLarge has 4 jobs) */
# define FBL_CAN_TRCV_TJA1145_SPI_CHANNEL_COUNT       4u

/** Number of sequences, 3 sequences are required by the CAN transceiver (SeqRWDataLarge, SeqRWDataMedium, SeqRWDataNormal) */
# define FBL_CAN_TRCV_TJA1145_SPI_SEQUENCE_COUNT      3u

#else /* FBL_CAN_TRCV_TJA1145_DISABLE_MSR_CAN_TRCV_USAGE */

# define FBL_CAN_TRCV_TJA1145_MAX_TRANSFER_LENGTH     3u

#endif /* FBL_CAN_TRCV_TJA1145_DISABLE_MSR_CAN_TRCV_USAGE */

#if ( FBL_CAN_TRCV_TJA1145_NUMBER_OF_CHANNELS > 1u)
/* Multi-channel use-case: SPI handle dependent on transceiver channel */
# define FBL_CAN_TRCV_TJA1145_SPI_HANDLE_PARAM_ONLY      spiHandle
# define FBL_CAN_TRCV_TJA1145_SPI_HANDLE_PARAM_FIRST     spiHandle,
# define FBL_CAN_TRCV_TJA1145_SPI_HANDLE                 spiHandle
#else
/* Single-channel use-case */
# if ( FBL_SPI_MAX_NUMBER_OF_HANDLES > 1u )
/* Multi-channel SPI: SPI handle used by transceiver is fixed */
# define FBL_CAN_TRCV_TJA1145_SPI_HANDLE_PARAM_ONLY      FBL_CAN_TRCV_TJA1145_SPI_INIT_HANDLE
# define FBL_CAN_TRCV_TJA1145_SPI_HANDLE_PARAM_FIRST     FBL_CAN_TRCV_TJA1145_SPI_INIT_HANDLE,
# define FBL_CAN_TRCV_TJA1145_SPI_HANDLE                 FBL_CAN_TRCV_TJA1145_SPI_INIT_HANDLE
# else
/* Single-channel SPI: Handle parameter not used by SPI interface */
# define FBL_CAN_TRCV_TJA1145_SPI_HANDLE_PARAM_ONLY      /* Not used */
# define FBL_CAN_TRCV_TJA1145_SPI_HANDLE_PARAM_FIRST     /* Not used */
# define FBL_CAN_TRCV_TJA1145_SPI_HANDLE                 0u
# endif /* FBL_SPI_MAX_NUMBER_OF_HANDLES */
#endif /* FBL_CAN_TRCV_TJA1145_NUMBER_OF_CHANNELS */

#if defined( FBL_CAN_TRCV_TJA1145_ENABLE_MSR_CAN_TRCV_USAGE )
/* Timeout Handling */
# define FblCanTrcvResetStateTransitionTimer()              (fblCanTrcvTimeout = FBL_CAN_TRCV_TJA1145_TIMEOUT + 1u) /* Add one to wait one iteration as minimum */
# define FblCanTrcvDecStateTransitionTimer()                (fblCanTrcvTimeout--)
# define FblCanTrcvGetStateTransitionTimeout()              (0u == fblCanTrcvTimeout)
#endif

/***********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

#if defined( FBL_CAN_TRCV_TJA1145_ENABLE_MSR_CAN_TRCV_USAGE )
/* MSR SPI channel configuration */
typedef struct
{
   Spi_DataConstPtrType pSrcBuffer;
   Spi_DataPtrType      pDestBuffer;
   Spi_NumberOfDataType length;
} tFblAsrSpiChannelCfg;
#endif /* FBL_CAN_TRCV_TJA1145_ENABLE_MSR_CAN_TRCV_USAGE */

/***********************************************************************************************************************
 *  LOCAL VARIABLES
 **********************************************************************************************************************/

#if defined( FBL_CAN_TRCV_TJA1145_ENABLE_MSR_CAN_TRCV_USAGE )
V_MEMRAM0 static V_MEMRAM1 vuint8 V_MEMRAM2 fblCanTrcvTimeout;
#endif

/***********************************************************************************************************************
 *  LOCAL DATA
 **********************************************************************************************************************/

#if defined( FBL_CAN_TRCV_TJA1145_ENABLE_MSR_CAN_TRCV_USAGE )

V_MEMRAM0 static V_MEMRAM1 tFblAsrSpiChannelCfg V_MEMRAM2 g_FblAsrSpiChannelCfg[FBL_CAN_TRCV_TJA1145_SPI_CHANNEL_COUNT];

V_MEMROM0 static V_MEMROM1 Spi_ChannelType V_MEMROM2 g_FblAsrSpiSequenceCfg[FBL_CAN_TRCV_TJA1145_SPI_SEQUENCE_COUNT] = /* PRQA S 3218 */ /* MD_FblCanTrcv_3218 */
{
   1u, /* SeqRWDataNormal, 1 job */
   3u, /* SeqRWDataMedium, 3 jobs */
   4u  /* SeqRWDataLarge, 4 jobs */
};

# if ( FBL_CAN_TRCV_TJA1145_NUMBER_OF_CHANNELS > 1u)
/** Active transceiver channel */
V_MEMRAM0 static V_MEMRAM1 vuint8 V_MEMRAM2 g_FblTrcvChannel;
# endif /* FBL_CAN_TRCV_TJA1145_NUMBER_OF_CHANNELS > 1u */
#endif /* FBL_CAN_TRCV_TJA1145_ENABLE_MSR_CAN_TRCV_USAGE */

/***********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

#if ( FBL_CAN_TRCV_TJA1145_NUMBER_OF_CHANNELS > 1u )
static tFblResult FblCanTrcvTja1145GetSpiHandle( FBL_CAN_TRCV_TJA1145_HANDLE_TYPE_FIRST V_MEMRAM1 vuint8 V_MEMRAM2 * V_MEMRAM3 spiHandle );
#endif /* FBL_CAN_TRCV_TJA1145_NUMBER_OF_CHANNELS */

/***********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/

#if ( FBL_CAN_TRCV_TJA1145_NUMBER_OF_CHANNELS > 1u)
/***********************************************************************************************************************
 *  FblCanTrcvTja1145GetSpiHandle
 **********************************************************************************************************************/
/*! \brief           Get SPI handle used by given transceiver channel
 *  \param[in]       trcvHandle  Specifies the transceiver channel
 *  \param[out]      spiHandle  SPI handle for given transciver channel
 *  \return          kFblOk if valid channel is given, otherwise kFblFailed
 **********************************************************************************************************************/
static tFblResult FblCanTrcvTja1145GetSpiHandle( FBL_CAN_TRCV_TJA1145_HANDLE_TYPE_FIRST V_MEMRAM1 vuint8 V_MEMRAM2 * V_MEMRAM3 spiHandle )
{
   tFblResult result;

   result = kFblFailed;

   /* Channel within valid range? */
   if (FBL_CAN_TRCV_TJA1145_HANDLE < FBL_CAN_TRCV_TJA1145_NUMBER_OF_CHANNELS)
   {
      /* Map to matching SPI handle */
      *spiHandle  = g_FblCanTrcvTja1145SpiHandleMap[FBL_CAN_TRCV_TJA1145_HANDLE];
      result      = kFblOk;
   }

   return result;
}
#endif /* FBL_CAN_TRCV_TJA1145_NUMBER_OF_CHANNELS */

/***********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

#if defined( FBL_CAN_TRCV_TJA1145_ENABLE_MSR_CAN_IF_USAGE )
#else /* FBL_CAN_TRCV_TJA1145_DISABLE_MSR_CAN_IF_USAGE */

/***********************************************************************************************************************
 *  FblCanTrcvTja1145Init
 **********************************************************************************************************************/
/*! \brief           Initialize communication with TJA1145
 *  \param[in]       trcvHandle (optional)  Specifies the transceiver channel
 *  \return          kFblOk if initialization succeeded, otherwise kFblFailed
 **********************************************************************************************************************/
tFblResult FblCanTrcvTja1145Init( FBL_CAN_TRCV_TJA1145_HANDLE_TYPE_ONLY )
{
   tFblResult result;
# if ( FBL_CAN_TRCV_TJA1145_NUMBER_OF_CHANNELS > 1u)
   vuint8 FBL_CAN_TRCV_TJA1145_SPI_HANDLE;

   /* Get matching SPI handle for transceiver channel */
   result = FblCanTrcvTja1145GetSpiHandle(FBL_CAN_TRCV_TJA1145_HANDLE_PARAM_FIRST &FBL_CAN_TRCV_TJA1145_SPI_HANDLE);

   if (kFblOk == result )
# endif /* FBL_CAN_TRCV_TJA1145_NUMBER_OF_CHANNELS > 1u */
   {
      FBL_CAN_TRCV_TJA1145_INIT_CS(FBL_CAN_TRCV_TJA1145_HANDLE_PARAM_ONLY);

      result = FblCanTrcvTja1145SpiInit(FBL_CAN_TRCV_TJA1145_SPI_HANDLE_PARAM_ONLY);
   }

   return result;
}

/***********************************************************************************************************************
 *  FblCanTrcvTja1145Deinit
 **********************************************************************************************************************/
/*! \brief           Deinitialize communication with TJA1145
 *  \param[in]       trcvHandle (optional)  Specifies the transceiver channel
 *  \return          kFblOk if deinitialization succeeded, otherwise kFblFailed
 **********************************************************************************************************************/
tFblResult FblCanTrcvTja1145Deinit( FBL_CAN_TRCV_TJA1145_HANDLE_TYPE_ONLY )
{
   tFblResult result;
# if ( FBL_CAN_TRCV_TJA1145_NUMBER_OF_CHANNELS > 1u)
   vuint8 FBL_CAN_TRCV_TJA1145_SPI_HANDLE;

   /* Get matching SPI handle for transceiver channel */
   result = FblCanTrcvTja1145GetSpiHandle(FBL_CAN_TRCV_TJA1145_HANDLE_PARAM_FIRST &FBL_CAN_TRCV_TJA1145_SPI_HANDLE);

   if (kFblOk == result )
# endif /* FBL_CAN_TRCV_TJA1145_NUMBER_OF_CHANNELS > 1u */
   {
      FBL_CAN_TRCV_TJA1145_DEINIT_CS(FBL_CAN_TRCV_TJA1145_HANDLE_PARAM_ONLY);

      result = FblCanTrcvTja1145SpiDeinit(FBL_CAN_TRCV_TJA1145_SPI_HANDLE_PARAM_ONLY);
   }

   return result;
}

/***********************************************************************************************************************
 *  FblCanTrcvTja1145NormalMode
 **********************************************************************************************************************/
/*! \brief           Set TJA1145 to normal mode
 *  \param[in]       trcvHandle (optional)  Specifies the transceiver channel
 *  \return          kFblOk if succeeded, otherwise kFblFailed
 **********************************************************************************************************************/
tFblResult FblCanTrcvTja1145NormalMode( FBL_CAN_TRCV_TJA1145_HANDLE_TYPE_ONLY )
{
   tFblResult result;

# if defined( FBL_CAN_TRCV_TJA1145_ENABLE_MSR_CAN_TRCV_USAGE )
   CanTrcv_TrcvModeType opMode;

   result = kFblFailed;

   CanTrcv_30_Tja1145_InitMemory();
   CanTrcv_30_Tja1145_Init(CanTrcv_30_Tja1145_Config_Ptr);
   CanTrcv_30_Tja1145_Init(CanTrcv_30_Tja1145_Config_Ptr);

   /* Initialize Timeout Handling */
   FblCanTrcvResetStateTransitionTimer();

   /* Given transceiver channel has to match AUTOSAR configuration */
   if (E_OK == CanTrcv_30_Tja1145_SetOpMode(FBL_CAN_TRCV_TJA1145_HANDLE_PARAM, CANTRCV_TRCVMODE_NORMAL))
   {
      do
      {
         /* Watchdog will be handled by SPI driver which is implicitly called (FblLookForWatchdogVoid()) */
         CanTrcv_30_Tja1145_MainFunction();

         /* Request current operation mode of Transceiver and check for a Timeout */
         if ((E_NOT_OK == CanTrcv_30_Tja1145_GetOpMode(FBL_CAN_TRCV_TJA1145_HANDLE_PARAM, &opMode)) || (FblCanTrcvGetStateTransitionTimeout()))
         {
            result = kFblFailed;
         }
         else
         {
            result = kFblOk;
         }

         /* Check if 1ms hardware timer expired */
         if (FBL_TM_TRIGGERED == (FblLookForWatchdog() & FBL_TM_TRIGGERED))
         {
            FblCanTrcvDecStateTransitionTimer();
         }
      }
      while ((kFblOk == result) && (CANTRCV_TRCVMODE_NORMAL != opMode));
   }

# else /* FBL_CAN_TRCV_TJA1145_DISABLE_MSR_CAN_TRCV_USAGE */

   tFblSpiTransferParam transferParam;
   vuint8 transmitBuffer[FBL_CAN_TRCV_TJA1145_MAX_TRANSFER_LENGTH];
   vuint8 receiveBuffer[FBL_CAN_TRCV_TJA1145_MAX_TRANSFER_LENGTH];
#  if ( FBL_CAN_TRCV_TJA1145_NUMBER_OF_CHANNELS > 1u)
   vuint8 FBL_CAN_TRCV_TJA1145_SPI_HANDLE;

   /* Get matching SPI handle for transceiver channel */
   result = FblCanTrcvTja1145GetSpiHandle(FBL_CAN_TRCV_TJA1145_HANDLE_PARAM_FIRST &FBL_CAN_TRCV_TJA1145_SPI_HANDLE);

   if (kFblOk == result )
#  endif /* FBL_CAN_TRCV_TJA1145_NUMBER_OF_CHANNELS > 1u */
   {
      transferParam.pTransmitBuffer = transmitBuffer;
      transferParam.pReceiveBuffer = receiveBuffer;
      transferParam.pollingFct = FblLookForWatchdogVoid;
      transferParam.chipSelectMode = FBL_SPI_CHIP_SELECT_MODE_BOTH;

      /* 1st: Read and check transceiver identification ******************************************************************/

      transmitBuffer[0u] = (0x7Eu << 1u) | 0x01u;                 /* Address 0x7e, read-only */
      transmitBuffer[1u] = 0x00u;                                 /* Dummy */
      transmitBuffer[2u] = 0x00u;                                 /* Dummy */
      receiveBuffer[0u]  = 0x00u;                                 /* Dummy */
      receiveBuffer[1u]  = 0x00u;                                 /* Dummy */
      receiveBuffer[2u]  = 0x00u;                                 /* Dummy */

      transferParam.transferLength = 2u;

      FBL_CAN_TRCV_TJA1145_SET_CS(FBL_CAN_TRCV_TJA1145_HANDLE_PARAM_ONLY);
      result = FblCanTrcvTja1145SpiTransferSync(FBL_CAN_TRCV_TJA1145_SPI_HANDLE_PARAM_FIRST &transferParam);
      FBL_CAN_TRCV_TJA1145_CLR_CS(FBL_CAN_TRCV_TJA1145_HANDLE_PARAM_ONLY);

      if (   (kFblOk != result)
          || (((0x7Eu << 1u) | 0x01u) != receiveBuffer[0u])       /* Address 0x7e, read-only */
          || (   (0x70u != receiveBuffer[1u])                     /* TJA1145T, TJA1145TK */
              && (0x74u != receiveBuffer[1u])))                   /* TJA1145T/FD, TJA1145TK/FD */
      {
         /* Either reading the transceiver identification failed or the identification code is unknown / not supported */
         result = kFblFailed;
      }

      /* 2nd: Read and check transceiver status **************************************************************************/

      if (kFblOk == result)
      {
         transmitBuffer[0u] = (0x22u << 1u) | 0x01u;              /* Address 0x22, read-only */
         transmitBuffer[1u] = 0x00u;                              /* Dummy */
         transmitBuffer[2u] = 0x00u;                              /* Dummy */
         receiveBuffer[0u]  = 0x00u;                              /* Dummy */
         receiveBuffer[1u]  = 0x00u;                              /* Dummy */
         receiveBuffer[2u]  = 0x00u;                              /* Dummy */

         transferParam.transferLength = 2u;

         FBL_CAN_TRCV_TJA1145_SET_CS(FBL_CAN_TRCV_TJA1145_HANDLE_PARAM_ONLY);
         result = FblCanTrcvTja1145SpiTransferSync(FBL_CAN_TRCV_TJA1145_SPI_HANDLE_PARAM_FIRST &transferParam);
         FBL_CAN_TRCV_TJA1145_CLR_CS(FBL_CAN_TRCV_TJA1145_HANDLE_PARAM_ONLY);

         if (   (kFblOk != result)
             || (((0x22u << 1u) | 0x01u) != receiveBuffer[0u]))   /* Address 0x22, read-only */
         {
            /* Reading the transceiver status failed, the status itself is intentionally not checked here as it might be
             * arbitrary when entering the FBL */
            result = kFblFailed;
         }
      }

      /* 3rd: Clear all pending events ***********************************************************************************/

      if (kFblOk == result)
      {
         transmitBuffer[0u] = (0x61u << 1u);                      /* Address 0x61 */
         transmitBuffer[1u] = 0x06u;                              /* W1C on address 0x61: OTW, SPIF (PO bit is intentionally not handled) */
         transmitBuffer[2u] = 0x00u;                              /* Dummy */
         receiveBuffer[0u]  = 0x00u;                              /* Dummy */
         receiveBuffer[1u]  = 0x00u;                              /* Dummy */
         receiveBuffer[2u]  = 0x00u;                              /* Dummy */

         transferParam.transferLength = 2u;

         FBL_CAN_TRCV_TJA1145_SET_CS(FBL_CAN_TRCV_TJA1145_HANDLE_PARAM_ONLY);
         result = FblCanTrcvTja1145SpiTransferSync(FBL_CAN_TRCV_TJA1145_SPI_HANDLE_PARAM_FIRST &transferParam);
         FBL_CAN_TRCV_TJA1145_CLR_CS(FBL_CAN_TRCV_TJA1145_HANDLE_PARAM_ONLY);

         if (   (kFblOk != result)
             || ((0x61u << 1u) != receiveBuffer[0u])              /* Address 0x61 */
             || (0x00u != (receiveBuffer[1u] & 0x06u)))           /* OTW = 0, SPIF = 0 (PO bit is intentionally not handled) */
         {
            /* Clearing all pending events failed */
            result = kFblFailed;
         }
      }

      if (kFblOk == result)
      {
         transmitBuffer[0u] = (0x60u << 1u);                      /* Address 0x60 */
         transmitBuffer[1u] = 0x0Cu;                              /* W1C on address 0x60: WPE, TRXE (SYSE is intentionally not handled, might be set by PO bit) */
         transmitBuffer[2u] = 0x00u;                              /* Dummy */
         receiveBuffer[0u]  = 0x00u;                              /* Dummy */
         receiveBuffer[1u]  = 0x00u;                              /* Dummy */
         receiveBuffer[2u]  = 0x00u;                              /* Dummy */

         transferParam.transferLength = 2u;

         FBL_CAN_TRCV_TJA1145_SET_CS(FBL_CAN_TRCV_TJA1145_HANDLE_PARAM_ONLY);
         result = FblCanTrcvTja1145SpiTransferSync(FBL_CAN_TRCV_TJA1145_SPI_HANDLE_PARAM_FIRST &transferParam);
         FBL_CAN_TRCV_TJA1145_CLR_CS(FBL_CAN_TRCV_TJA1145_HANDLE_PARAM_ONLY);

         if (   (kFblOk != result)
             || ((0x60u << 1u) != receiveBuffer[0u])              /* Address 0x60 */
             || (0x00u != (receiveBuffer[1u] & 0x0Cu)))           /* WPE = 0, TRXE = 0 (SYSE is intentionally not handled, might be set by PO bit) */
         {
            /* Clearing all pending events failed */
            result = kFblFailed;
         }
      }

      if (kFblOk == result)
      {
         transmitBuffer[0u] = (0x63u << 1u);                      /* Address 0x63 and implicitly 0x64 */
         transmitBuffer[1u] = 0x33u;                              /* W1C on address 0x63: PNFDE, CBS, CF, CW */
         transmitBuffer[2u] = 0x03u;                              /* W1C on address 0x64: WPR, WPF */
         receiveBuffer[0u]  = 0x00u;                              /* Dummy */
         receiveBuffer[1u]  = 0x00u;                              /* Dummy */
         receiveBuffer[2u]  = 0x00u;                              /* Dummy */

         transferParam.transferLength = 3u;

         FBL_CAN_TRCV_TJA1145_SET_CS(FBL_CAN_TRCV_TJA1145_HANDLE_PARAM_ONLY);
         result = FblCanTrcvTja1145SpiTransferSync(FBL_CAN_TRCV_TJA1145_SPI_HANDLE_PARAM_FIRST &transferParam);
         FBL_CAN_TRCV_TJA1145_CLR_CS(FBL_CAN_TRCV_TJA1145_HANDLE_PARAM_ONLY);

         if (   (kFblOk != result)
             || ((0x63u << 1u) != receiveBuffer[0u])              /* Address 0x63 and implicitly 0x64 */
             || (0x00u != receiveBuffer[1u])                      /* PNFDE = 0, CBS = 0, CF = 0, CW = 0 */
             || (0x00u != receiveBuffer[2u]))                     /* WPR = 0, WPF = 0 */
         {
            /* Clearing all pending events failed */
            result = kFblFailed;
         }
      }

      /* 4th: Select operating mode **************************************************************************************/

      if (kFblOk == result)
      {
         transmitBuffer[0u] = (0x20u << 1u);                      /* Address 0x20 */
         transmitBuffer[1u] = 0x01u;                              /* CMC = 01, active mode, under-voltage detection active */
         transmitBuffer[2u] = 0x00u;                              /* Dummy */
         receiveBuffer[0u]  = 0x00u;                              /* Dummy */
         receiveBuffer[1u]  = 0x00u;                              /* Dummy */
         receiveBuffer[2u]  = 0x00u;                              /* Dummy */

         transferParam.transferLength = 2u;

         FBL_CAN_TRCV_TJA1145_SET_CS(FBL_CAN_TRCV_TJA1145_HANDLE_PARAM_ONLY);
         result = FblCanTrcvTja1145SpiTransferSync(FBL_CAN_TRCV_TJA1145_SPI_HANDLE_PARAM_FIRST &transferParam);
         FBL_CAN_TRCV_TJA1145_CLR_CS(FBL_CAN_TRCV_TJA1145_HANDLE_PARAM_ONLY);

         if (   (kFblOk != result)
             || ((0x20u << 1u) != receiveBuffer[0u])              /* Address 0x20 */
             || (0x01u != receiveBuffer[1u]))                     /* CMC = 01, active mode, under-voltage detection active */
         {
            /* Setting operating mode failed */
            result = kFblFailed;
         }
      }

      /* 5th: Set active mode ********************************************************************************************/

      if (kFblOk == result)
      {
         transmitBuffer[0u] = (0x01u << 1u);                      /* Address 0x01 */
         transmitBuffer[1u] = 0x07u;                              /* MC = 111, normal mode */
         transmitBuffer[2u] = 0x00u;                              /* Dummy */
         receiveBuffer[0u]  = 0x00u;                              /* Dummy */
         receiveBuffer[1u]  = 0x00u;                              /* Dummy */
         receiveBuffer[2u]  = 0x00u;                              /* Dummy */

         transferParam.transferLength = 2u;

         FBL_CAN_TRCV_TJA1145_SET_CS(FBL_CAN_TRCV_TJA1145_HANDLE_PARAM_ONLY);
         result = FblCanTrcvTja1145SpiTransferSync(FBL_CAN_TRCV_TJA1145_SPI_HANDLE_PARAM_FIRST &transferParam);
         FBL_CAN_TRCV_TJA1145_CLR_CS(FBL_CAN_TRCV_TJA1145_HANDLE_PARAM_ONLY);

         if (   (kFblOk != result)
             || ((0x01u << 1u) != receiveBuffer[0u])              /* Address 0x01 */
             || (   (0x04u != receiveBuffer[1u])                  /* MC = 100, standby mode (normal mode might not be instantly applied) */
                 && (0x07u != receiveBuffer[1u])))                /* MC = 111, normal mode */
         {
            /* Setting active mode failed */
            result = kFblFailed;
         }
      }

      /* 6th: Check operating mode selection *****************************************************************************/

      if (kFblOk == result)
      {
         transmitBuffer[0u] = (0x20u << 1u) | 0x01u;              /* Address 0x20, read-only */
         transmitBuffer[1u] = 0x00u;                              /* Dummy */
         transmitBuffer[2u] = 0x00u;                              /* Dummy */
         receiveBuffer[0u]  = 0x00u;                              /* Dummy */
         receiveBuffer[1u]  = 0x00u;                              /* Dummy */
         receiveBuffer[2u]  = 0x00u;                              /* Dummy */

         transferParam.transferLength = 2u;

         FBL_CAN_TRCV_TJA1145_SET_CS(FBL_CAN_TRCV_TJA1145_HANDLE_PARAM_ONLY);
         result = FblCanTrcvTja1145SpiTransferSync(FBL_CAN_TRCV_TJA1145_SPI_HANDLE_PARAM_FIRST &transferParam);
         FBL_CAN_TRCV_TJA1145_CLR_CS(FBL_CAN_TRCV_TJA1145_HANDLE_PARAM_ONLY);

         if (   (kFblOk != result)
             || (((0x20u << 1u) | 0x01u) != receiveBuffer[0u])    /* Address 0x20, read-only */
             || (0x01u != receiveBuffer[1u]))                     /* CMC = 01, active mode, under-voltage detection active */
         {
            /* Checking the operating mode selection failed */
            result = kFblFailed;
         }
      }

      /* 7th: Check active mode setting **********************************************************************************/

      if (kFblOk == result)
      {
         transmitBuffer[0u] = (0x01u << 1u) | 0x01u;              /* Address 0x01, read-only */
         transmitBuffer[1u] = 0x00u;                              /* Dummy */
         transmitBuffer[2u] = 0x00u;                              /* Dummy */
         receiveBuffer[0u]  = 0x00u;                              /* Dummy */
         receiveBuffer[1u]  = 0x00u;                              /* Dummy */
         receiveBuffer[2u]  = 0x00u;                              /* Dummy */

         transferParam.transferLength = 2u;

         FBL_CAN_TRCV_TJA1145_SET_CS(FBL_CAN_TRCV_TJA1145_HANDLE_PARAM_ONLY);
         result = FblCanTrcvTja1145SpiTransferSync(FBL_CAN_TRCV_TJA1145_SPI_HANDLE_PARAM_FIRST &transferParam);
         FBL_CAN_TRCV_TJA1145_CLR_CS(FBL_CAN_TRCV_TJA1145_HANDLE_PARAM_ONLY);

         if (   (kFblOk != result)
             || (((0x01u << 1u) | 0x01u) != receiveBuffer[0u])    /* Address 0x01, read-only */
             || (0x07u != receiveBuffer[1u]))                     /* MC = 111, normal mode */
         {
            /* Checking the active mode setting failed */
            result = kFblFailed;
         }
      }
   }
# endif /* FBL_CAN_TRCV_TJA1145_DISABLE_MSR_CAN_TRCV_USAGE */

   return result;
} /* PRQA S 6010, 6030 */ /* MD_FblCanTrcv_6010, MD_FblCanTrcv_6030 */

/***********************************************************************************************************************
 *  FblCanTrcvTja1145SleepMode
 **********************************************************************************************************************/
/*! \brief           Set TJA1145 to Sleep mode
 *  \param[in]       trcvHandle (optional)  Specifies the transceiver channel
 *  \return          kFblOk if succeeded, otherwise kFblFailed
 **********************************************************************************************************************/
tFblResult FblCanTrcvTja1145SleepMode( FBL_CAN_TRCV_TJA1145_HANDLE_TYPE_ONLY )
{
   tFblResult result = kFblFailed;

# if defined( FBL_CAN_TRCV_TJA1145_ENABLE_MSR_CAN_TRCV_USAGE )
   CanTrcv_TrcvModeType opMode;

   /* Initialize Timeout Handling */
   FblCanTrcvResetStateTransitionTimer();

   /* State transition to standBy mode must first occur before sleep */
   /* Given transceiver channel has to match AUTOSAR configuration */
   if (E_OK == CanTrcv_30_Tja1145_SetOpMode(FBL_CAN_TRCV_TJA1145_HANDLE_PARAM, CANTRCV_TRCVMODE_STANDBY))
   {
      do
      {
         /* Watchdog will be handled by SPI driver which is implicitly called (FblLookForWatchdogVoid()) */
         CanTrcv_30_Tja1145_MainFunction();

         /* Request current operation mode of Transceiver and check for a Timeout */
         if ((E_NOT_OK == CanTrcv_30_Tja1145_GetOpMode(FBL_CAN_TRCV_TJA1145_HANDLE_PARAM, &opMode)) || (FblCanTrcvGetStateTransitionTimeout()))
         {
            result = kFblFailed;
         }
         else
         {
            result = kFblOk;
         }

         /* Check if 1ms hardware timer expired */
         if (FBL_TM_TRIGGERED == (FblLookForWatchdog() & FBL_TM_TRIGGERED))
         {
            FblCanTrcvDecStateTransitionTimer();
         }
      }
      while ((kFblOk == result) && (CANTRCV_TRCVMODE_STANDBY != opMode));
   }

   /* Evaluate Standby Mode state transition */
   if (result == kFblOk)
   {
      /* Reset Timeout */
      FblCanTrcvResetStateTransitionTimer();

      /* Given transceiver channel has to match AUTOSAR configuration */
      if ((E_OK == CanTrcv_30_Tja1145_SetOpMode(FBL_CAN_TRCV_TJA1145_HANDLE_PARAM, CANTRCV_TRCVMODE_SLEEP)))
      {
         do
         {
            /* Watchdog will be handled by SPI driver which is implicitly called (FblLookForWatchdogVoid()) */
            CanTrcv_30_Tja1145_MainFunction();

            /* Request current operation mode of Transceiver and check for a Timeout */
            if ((E_NOT_OK == CanTrcv_30_Tja1145_GetOpMode(FBL_CAN_TRCV_TJA1145_HANDLE_PARAM, &opMode)) || (FblCanTrcvGetStateTransitionTimeout()))
            {
               result = kFblFailed;
            }
            else
            {
               result = kFblOk;
            }

            /* Check if 1ms hardware timer expired */
            if (FBL_TM_TRIGGERED == (FblLookForWatchdog() & FBL_TM_TRIGGERED))
            {
               FblCanTrcvDecStateTransitionTimer();
            }
         }
         while ((kFblOk == result) && (CANTRCV_TRCVMODE_SLEEP != opMode));
      }
      else
      {
         /* SetOpMode failed */
         result = kFblFailed;
      }
   }

# else /* FBL_CAN_TRCV_TJA1145_DISABLE_MSR_CAN_TRCV_USAGE */

   /* Sleep Mode transition not supported */

# endif /* FBL_CAN_TRCV_TJA1145_DISABLE_MSR_CAN_TRCV_USAGE */

   return result;
} /* PRQA S 6010, 6030 */ /* MD_FblCanTrcv_6010, MD_FblCanTrcv_6030 */
#endif /* FBL_CAN_TRCV_TJA1145_DISABLE_MSR_CAN_IF_USAGE */

#if defined( FBL_CAN_TRCV_TJA1145_ENABLE_MSR_CAN_TRCV_USAGE )

/***********************************************************************************************************************
 *  Spi_SetupEB
 **********************************************************************************************************************/
/*! \brief           This function substitutes the "real" MSR Spi_SetupEB() function in order to allow to use the
 *                   FBL SPI driver instead of the MSR SPI / DIO driver(s)
 *  \pre             MSR transceiver and SPI configuration has to match the following scheme:
 *
 *                   CanTrcvChannels           | CanTrcvSpiSequenceAccess | Spi Sequence Name
 *                   --------------------------+--------------------------+--------------------------
 *                   CanTrcvChannel            | SeqRWDataNormal          | SpiSequence
 *                                             | SeqRWDataMedium          | SpiSequence_001
 *                                             | SeqRWDataLarge           | SpiSequence_002
 *                   --------------------------+--------------------------+--------------------------
 *                   CanTrcvChannel_001        | SeqRWDataNormal          | SpiSequence_003
 *                                             | SeqRWDataMedium          | SpiSequence_004
 *                                             | SeqRWDataLarge           | SpiSequence_005
 *                   --------------------------+--------------------------+--------------------------
 *                   CanTrcvChannel_002        | SeqRWDataNormal          | SpiSequence_006
 *                                             | ...                      | ...
 *
 *                   SpiSequences              | Job Assignment
 *                   --------------------------+--------------------------
 *                   SpiSequence               | SpiJob
 *                   --------------------------+--------------------------
 *                   SpiSequence_001           | SpiJob
 *                                             | SpiJob_001
 *                                             | SpiJob_002
 *                   --------------------------+--------------------------
 *                   SpiSequence_002           | SpiJob
 *                                             | SpiJob_001
 *                                             | SpiJob_002
 *                                             | SpiJob_003
 *                   --------------------------+--------------------------
 *                   SpiSequence_003           | SpiJob_004
 *                   --------------------------+--------------------------
 *                   SpiSequence_004           | SpiJob_004
 *                                             | SpiJob_005
 *                                             | SpiJob_006
 *                   --------------------------+--------------------------
 *                   SpiSequence_005           | SpiJob_004
 *                                             | SpiJob_005
 *                                             | SpiJob_006
 *                                             | SpiJob_007
 *                   --------------------------+--------------------------
 *                   SpiSequence_006           | SpiJob_008
 *                   ...                       | ...
 *
 *                   Device Assignment         | SpiJobs                  | Channel Assignment
 *                   --------------------------+--------------------------+--------------------------
 *                   SpiExternalDevice         | SpiJob                   | SpiChannel
 *                                             | SpiJob_001               | SpiChannel_001
 *                                             | SpiJob_002               | SpiChannel_002
 *                                             | SpiJob_003               | SpiChannel_003
 *                   --------------------------+--------------------------+--------------------------
 *                   SpiExternalDevice_001     | SpiJob_004               | SpiChannel_004
 *                                             | SpiJob_005               | SpiChannel_005
 *                                             | SpiJob_006               | SpiChannel_006
 *                                             | SpiJob_007               | SpiChannel_007
 *                   --------------------------+--------------------------+--------------------------
 *                   SpiExternalDevice_002     | SpiJob_008               | SpiChannel_008
 *                                             | ...                      | ...
 *
 **********************************************************************************************************************/
FUNC(Std_ReturnType, SPI_CODE) Spi_SetupEB( Spi_ChannelType Channel, Spi_DataConstPtrType SrcDataBufferPtr,
   Spi_DataPtrType DesDataBufferPtr, Spi_NumberOfDataType Length )
{
   Std_ReturnType result;
# if ( FBL_CAN_TRCV_TJA1145_NUMBER_OF_CHANNELS > 1u)
   tFblCanTrcvTja1145Handle FBL_CAN_TRCV_TJA1145_HANDLE;
# endif /* FBL_CAN_TRCV_TJA1145_NUMBER_OF_CHANNELS > 1u */

   result = E_NOT_OK;

   /* Channel in valid range? */
   if (Channel < (FBL_CAN_TRCV_TJA1145_SPI_CHANNEL_COUNT * FBL_CAN_TRCV_TJA1145_NUMBER_OF_CHANNELS))
   {
# if ( FBL_CAN_TRCV_TJA1145_NUMBER_OF_CHANNELS > 1u)
      FBL_CAN_TRCV_TJA1145_HANDLE = 0u;

      /* Map MSR SPI channel to transceiver channel and entry in SPI channel config */
      while (Channel >= FBL_CAN_TRCV_TJA1145_SPI_CHANNEL_COUNT)
      {
         /* Decrement channel until in range */
         Channel -= FBL_CAN_TRCV_TJA1145_SPI_CHANNEL_COUNT;
         FBL_CAN_TRCV_TJA1145_HANDLE++;
      }

      /* Remember transceiver channel for use in Spi_SyncTransmit */
      g_FblTrcvChannel = FBL_CAN_TRCV_TJA1145_HANDLE;
# endif /* FBL_CAN_TRCV_TJA1145_NUMBER_OF_CHANNELS > 1u */

      g_FblAsrSpiChannelCfg[Channel].pSrcBuffer  = SrcDataBufferPtr;
      g_FblAsrSpiChannelCfg[Channel].pDestBuffer = DesDataBufferPtr;
      g_FblAsrSpiChannelCfg[Channel].length      = Length;

      result = E_OK;
   }

   return result;
}

/***********************************************************************************************************************
 *  Spi_SyncTransmit
 **********************************************************************************************************************/
/*! \brief           This function substitutes the "real" MSR Spi_SyncTransmit() function in order to allow to use the
 *                   FBL SPI driver instead of the MSR SPI / DIO driver(s)
 *  \pre             See Spi_SetupEB
 **********************************************************************************************************************/
FUNC(Std_ReturnType, SPI_CODE) Spi_SyncTransmit( Spi_SequenceType Sequence )
{
   Std_ReturnType       result;
   Spi_ChannelType      maxChannel;
   Spi_ChannelType      channel;
   tFblSpiTransferParam transferParam;
# if ( FBL_CAN_TRCV_TJA1145_NUMBER_OF_CHANNELS > 1u)
   tFblCanTrcvTja1145Handle FBL_CAN_TRCV_TJA1145_HANDLE;
   vuint8 FBL_CAN_TRCV_TJA1145_SPI_HANDLE;
# endif /* FBL_CAN_TRCV_TJA1145_NUMBER_OF_CHANNELS > 1u */

   result = E_NOT_OK;

   /* Check if sequence is valid */
   if (Sequence < FBL_CAN_TRCV_TJA1145_SPI_SEQUENCE_COUNT)
   {
# if ( FBL_CAN_TRCV_TJA1145_NUMBER_OF_CHANNELS > 1u)
      /* Get current transceiver channel */
      FBL_CAN_TRCV_TJA1145_HANDLE = g_FblTrcvChannel;

      /* Get matching SPI handle for transceiver channel */
      if (kFblOk == FblCanTrcvTja1145GetSpiHandle(FBL_CAN_TRCV_TJA1145_HANDLE_PARAM_FIRST &FBL_CAN_TRCV_TJA1145_SPI_HANDLE))
# endif /* FBL_CAN_TRCV_TJA1145_NUMBER_OF_CHANNELS > 1u */
      {
#  if defined( FBL_CAN_TRCV_TJA1145_ENABLE_MSR_CAN_IF_USAGE )
         FBL_CAN_TRCV_TJA1145_INIT_CS(FBL_CAN_TRCV_TJA1145_HANDLE_PARAM_ONLY);

         /* It is OK to initialize the SPI interface each time as it simplifies the modules usage and this function is
          * only called (maximum a few times) during initialization */
         if (kFblOk == FblCanTrcvTja1145SpiInit(FBL_CAN_TRCV_TJA1145_SPI_HANDLE_PARAM_ONLY))
#  endif /* FBL_CAN_TRCV_TJA1145_ENABLE_MSR_CAN_IF_USAGE */
         {
            result = E_OK;
            maxChannel = g_FblAsrSpiSequenceCfg[Sequence];
            transferParam.pollingFct = FblLookForWatchdogVoid;
            transferParam.chipSelectMode  = FBL_SPI_CHIP_SELECT_MODE_BOTH;

            /* Iterate over all jobs */
            for (channel = 0u; channel < maxChannel; channel++)
            {
               transferParam.pTransmitBuffer = g_FblAsrSpiChannelCfg[channel].pSrcBuffer;
               transferParam.pReceiveBuffer = g_FblAsrSpiChannelCfg[channel].pDestBuffer;
               transferParam.transferLength = g_FblAsrSpiChannelCfg[channel].length;

               FBL_CAN_TRCV_TJA1145_SET_CS(FBL_CAN_TRCV_TJA1145_HANDLE_PARAM_ONLY);
               result = FblCanTrcvTja1145SpiTransferSync(FBL_CAN_TRCV_TJA1145_SPI_HANDLE_PARAM_FIRST &transferParam);
               FBL_CAN_TRCV_TJA1145_CLR_CS(FBL_CAN_TRCV_TJA1145_HANDLE_PARAM_ONLY);

               if (kFblOk != result)
               {
                  result = E_NOT_OK;
                  break;
               }
            }

# if defined( FBL_CAN_TRCV_TJA1145_ENABLE_MSR_CAN_IF_USAGE )
            /* It is OK to deinitialize the SPI interface each time as it simplifies the modules usage and this function is
             * only called (maximum a few times) during initialization */
            if (E_OK == result)
            {
               if (kFblOk != FblCanTrcvTja1145SpiDeinit(FBL_CAN_TRCV_TJA1145_SPI_HANDLE_PARAM_ONLY))
               {
                  result = E_NOT_OK;
               }
            }

            FBL_CAN_TRCV_TJA1145_DEINIT_CS(FBL_CAN_TRCV_TJA1145_HANDLE_PARAM_ONLY);
# endif /* FBL_CAN_TRCV_TJA1145_ENABLE_MSR_CAN_IF_USAGE */
         }
      }
   }

   return result;
}

#endif /* FBL_CAN_TRCV_TJA1145_ENABLE_MSR_CAN_TRCV_USAGE */

/* Module specific MISRA deviations:

   MD_FblCanTrcv_3218:
      Reason: File scope static global variable is only access by one functions. Global data is kept in a single
              place to ease maintenance.
      Risk: No identifiable risk.
      Prevention: No prevention required.

   MD_FblCanTrcv_6010:
      Reason: Error handling is quite complex resulting in complicated flow.
      Risk: Resulting code is difficult to understand and maintain.
      Prevention: All changes to the affected code parts have to be reviewed by the module responsible.

   MD_FblCanTrcv_6030:
      Reason: Error handling is quite complex resulting in complicated flow.
      Risk: Resulting code is difficult to understand and maintain.
      Prevention: All changes to the affected code parts have to be reviewed by the module responsible.

   MD_FblCanTrcv_0883:
      Prevention: Correctness of repeated inclusion protection has to be verified by code inspection.
      Reason:     QAC does not recognize #elif statements and concludes that the header file is not protected against
                  repeated inclusion.
      Risk:       The repeated inclusion protection is missing.

*/

/***********************************************************************************************************************
 *  END OF FILE: FBL_CAN_TRCV_TJA1145.C
 **********************************************************************************************************************/
