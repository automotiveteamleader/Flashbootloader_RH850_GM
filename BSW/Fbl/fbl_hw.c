/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief         Hardware-specific module for Renesas RH850/RL78 RS-CAN (Lite)
 *                 Provides the CAN-Interface and timer operations
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
 *  Christian Baeuerle            CB            Vector Informatik GmbH
 *  Marcel Viole                  MVi           Vector Informatik GmbH
 *  Thomas Bezold                 TBe           Vector Informatik GmbH
 *  Dennis O'Donnell              Dod           Vector CANtech, Inc.
 *  Christopher Elkins            CEl           Vector CANtech, Inc.
 *  Ralf Haegenlaeuer             HRf           Vector Informatik GmbH
 *  Andre Caspari                 Ci            Vector Informatik GmbH
 *  Anqi Du                       AND           Vector CANtech, Inc.
 *  Alexander Starke              ASe           Vector Informatik GmbH
 *  Quetty Palacios               QPs           Vector Informatik GmbH
 *  Sebastian Loos                Shs           Vector Informatik GmbH
 *  Johannes Krimmel              KJs           Vector Informatik GmbH
 *  Torben Stoessel               TnS           Vector Informatik GmbH
 *  Marco Riedl                   Rie           Vector Informatik GmbH
 *  Daniel Koebel                 DKl           Vector Informatik GmbH
 *  Daniel Moessner               DlM           Vector Informatik GmbH
 *  Johannes Rebmann              JRe           Vector Informatik GmbH
 *  Andreas Wenckebach            AWh           Vector Informatik GmbH
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  00.90.00   2012-12-18  CB      ESCAN00063751    Initial implementation and beta release
 *  01.00.00   2013-05-04  CB      ESCAN00067920    No changes
 *                                                  Release
 *  01.01.00   2013-09-24  MVi     ESCAN00070496    Added IAR RL78 RS-CAN Lite
 *  01.02.00   2013-12-20  MVi     ESCAN00070930    Removed FblTimerStopp from JSR_APPL
 *                                 ESCAN00070931    Rework ownmemcpy memory qualifier
 *                                 ESCAN00072679    Removed CAN wake up implementation
 *                                 ESCAN00072761    Clear receive rule settings correctly
 *  01.03.00   2014-03-05  TBe     ESCAN00074028    Added support for RL78 Renesas compiler
 *  01.04.00   2014-03-26  CB      ESCAN00074448    Leave FblCanTransmit if tx pending flag is set
 *  01.05.00   2014-05-22  Dod     ESCAN00074973    No changes
 *                         CB      ESCAN00075823    Added support for RH850 WindRiver/DiabData compiler
 *  01.06.00   2014-07-01  CEl     ESCAN00076640    No changes
 *  01.07.00   2014-08-01  HRf     ESCAN00077536    Added support for GM SLP4
 *  01.08.00   2014-10-23  Ci      ESCAN00078093    Added support for F1H (fbl_sfr.h)
 *                                 ESCAN00078095    Adapted configuration switches
 *                         CEl     ESCAN00078784    Adapted configuration switches
 *                         Rr      ESCAN00078791    No changes
 *                         Rr      ESCAN00078970    No changes
 *                         CEl     ESCAN00079009    Added support for RZ
 *  01.09.00   2015-02-18  CB      ESCAN00079110    Added support for CAN receive ranges
 *                         AND     ESCAN00079110    No changes
 *                         ASe     ESCAN00080122    No changes
 *                         Ci      ESCAN00080218    Added support for R1x (fbl_sfr.h)
 *                         Rr      ESCAN00080422    No changes
 *  01.10.00   2015-04-29  CB      ESCAN00082512    Minor changes for sleep mode
 *                         QPs     ESCAN00082784    No changes
 *  01.11.00   2015-07-09  Shs     ESCAN00083898    No changes
 *  01.11.01   2015-07-30  KJs     ESCAN00084288    No changes
 *  01.11.02   2015-10-26  TnS     ESCAN00084351    Compiler error: Component does not compile for legacy ID handling
 *  01.12.00   2015-10-29  TnS     ESCAN00086139    Added function to restore CPU base pointers
 *  01.13.00   2015-11-23  Shs     ESCAN00086688    No changes
 *  01.13.01   2015-12-23  ASe     ESCAN00087278    No changes
 *  01.13.02   2016-02-25  TnS     ESCAN00078239    No changes
 *                                 ESCAN00088455    MISRA related changes
 *  01.13.03   2016-02-29  Rie     ESCAN00088610    No changes
 *  01.14.00   2016-03-18  CB      ESCAN00088977    No changes
 *  01.14.01   2016-04-05  TnS     ESCAN00089231    Compiler warning: Conversion may lose significant digits
 *  01.15.00   2016-05-09  TnS     ESCAN00089926    No changes
 *                                 ESCAN00089958    Added FblHwRestoreFblContext()
 *  01.16.00   2016-05-23  CB      ESCAN00090124    Support for E1x
 *  01.16.01   2016-05-23  DKl     ESCAN00090520    No changes
 *  01.17.00   2016-07-13  TnS     ESCAN00090720    Improve compliance to hardware user manual
 *                         CB      ESCAN00090982    Added memory qualifiers for CAN-Id-constants
 *  01.18.00   2016-07-18  Shs     ESCAN00091047    No changes
 *             2016-07-21  TBe     ESCAN00090770    No changes
 *                                 ESCAN00091092    FblGetTimerValue might not get linked into RAM
 *  01.19.00   2016-07-28  DlM     ESCAN00091227    Added support for F1K
 *             2016-08-02  JRe     ESCAN00091290    Support of CC-RL inline assembler instructions
 *  01.19.01   2016-08-05  JRe     ESCAN00091339    No changes
 *  01.19.02   2016-11-18  DlM     ESCAN00092878    No changes
 *                         AWh     ESCAN00093053    MISRA deviation: Cast of const pointer to non const pointer
 *  01.20.00   2017-01-09  DlM     ESCAN00093473    Added switch FBL_ENABLE_BUSTYPE_CAN for enable or disable CAN relevant code parts
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

#include "fbl_inc.h"

/***********************************************************************************************************************
 *   VERSION
 **********************************************************************************************************************/

#if (FBLDRVCAN_RH850RSCANCRX_VERSION != 0x0120u) || \
    (FBLDRVCAN_RH850RSCANCRX_RELEASE_VERSION != 0x00u)
# error "Error in fbl_hw.c: Source and Header file are inconsistent!"
#endif
#if (FBLDRVCAN_RH850RSCANCRX_VERSION != _FBLDRVCAN_VERSION) || \
    (FBLDRVCAN_RH850RSCANCRX_RELEASE_VERSION != _FBLDRVCAN_RELEASE_VERSION)
# error "Error in fbl_hw.c: Source and v_ver.h are inconsistent!"
#endif

#if (FBL_SFR_VERSION != FBLDRVCAN_RH850RSCANCRX_VERSION)
# error "Check fbl_sfr.h version"
#endif

/***********************************************************************************************************************
 *  CONFIGURATION CHECKS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

/* PRQA S 3453 TAG_FblHw_3453_1 */ /* MD_MSR_19.7 */

#if defined( FBL_ENABLE_BUSTYPE_CAN )

/* RS-CAN Macros */
/**< Masks one channel depended word in a multiplied shared register */
#define CanLL_MaskWord(pch) \
   ((((pch)&0x01) == 0) ? 0x0000FFFFul : 0xFFFF0000ul)

/**< Returns the register index of a channel depended word in a multiplied shared register */
#define CanLL_WordIndex(pch) \
   (((pch)>>1)&0x03)

/**< Returns the register index of a channel depended byte in a multiplied shared register */
#define CanLL_ByteIndex(pch) \
  (((pch)>>2)&0x01)

/**< Returns a dword containing a byte that is shifted accordingly to be written to a multiplied shared register
 *  (the right register index has to be committed) */
#if defined( V_CPU_RH850 )
#define CanLL_FilterListConfig(pch,val) \
   (kCanRegSize)((((kCanRegSize)(val)) & 0x000000FFul) << (24u - (((pch) & 0x03u) << 3)))

#define kFblCanDlcShift   28u
#endif

/**< Requests a on a physical channel */
#define CanLL_ModeReq_Phys(pch,mode) \
   (Can->ChCtrl[pch].CR = ((Can->ChCtrl[pch].CR & kCanModeMask) | (mode)))

/**< Checks the mode on a physical channel */
#define CanLL_ModeCheck_Phys(pch,mode) \
   ((Can->ChCtrl[pch].SR & kCanModeBits) == ((mode) & kCanModeBits))

/**< Requests a global mode */
#define CanLL_GlobalModeReq(mode) \
   (Can->CGCR = ((Can->CGCR & kCanModeMask) | (mode)))

/**< Checks the global mode (also checks if RAMIST==0) */
#define CanLL_GlobalModeCheck(mode) \
   (Can->CGSR == (mode))

#define CanLL_TxIsHWObjFree( canHwChannel, txObjHandle ) \
   ((Can->ChBS[canHwChannel].TBSR[0] & (kCanSrTxBufMaskAReq|kCanSrTxBufMaskTReq)) == 0u)

#define CanLL_HwIsBusOff(canHwChannel) \
   ((Can->ChCtrl[canHwChannel].SR & kCanSrMaskBusoff) == kCanSrMaskBusoff)

#define CanLL_HwWasBusOff( canHwChannel ) \
  ((Can->ChCtrl[canHwChannel].EF & kCanEfMaskBusoff) == kCanEfMaskBusoff)
/* PRQA L:TAG_FblHw_3453_1 */

/* Compose flag mask depending on the number of used CAN RX identifiers */
#if (FBL_CAN_NUMBER_OF_ID_TABLE_ENTRIES == 6)
# define kFblCanRxFlagMask ((vuint32)0x0000003Ful)
#elif (FBL_CAN_NUMBER_OF_ID_TABLE_ENTRIES == 5)
# define kFblCanRxFlagMask ((vuint32)0x0000001Ful)
#elif (FBL_CAN_NUMBER_OF_ID_TABLE_ENTRIES == 4)
# define kFblCanRxFlagMask ((vuint32)0x0000000Ful)
#elif (FBL_CAN_NUMBER_OF_ID_TABLE_ENTRIES == 3)
# define kFblCanRxFlagMask ((vuint32)0x00000007ul)
#elif (FBL_CAN_NUMBER_OF_ID_TABLE_ENTRIES == 2)
# define kFblCanRxFlagMask ((vuint32)0x00000003ul)
#elif (FBL_CAN_NUMBER_OF_ID_TABLE_ENTRIES == 1)
# define kFblCanRxFlagMask ((vuint32)0x00000001ul)
#else
# error "Unsupported number of CAN RX identifiers."
#endif

#endif /* FBL_ENABLE_BUSTYPE_CAN */

/***********************************************************************************************************************
 *  LOCAL DATA
 **********************************************************************************************************************/

#if defined( FBL_ENABLE_BUSTYPE_CAN )
static vuint8 messageIndex;
#endif /* FBL_ENABLE_BUSTYPE_CAN */

/***********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

#if defined( FBL_ENABLE_BUSTYPE_CAN )
#    if defined( FBL_ENABLE_MULTIPLE_NODES )
V_MEMROM0 V_MEMROM1 vuint32 V_MEMROM2 CanTxId[FBL_NUMBER_OF_NODES] = FBL_CAN_TXID;
V_MEMROM0 V_MEMROM1 vuint32 V_MEMROM2 CanRxId[FBL_NUMBER_OF_NODES] = FBL_CAN_RXID;
#    endif  /* FBL_ENABLE_MULTIPLE_NODES */

#  if defined( V_CPU_RH850 )
volatile tCanCellPtr Can;
#  endif

V_MEMRAM0 V_MEMRAM1 vuint32 V_MEMRAM2 CanRxActualId;        /* Current received ID */
V_MEMRAM0 V_MEMRAM1 vuint8 V_MEMRAM2  CanRxActualDLC;       /* Current received DLC    */
V_MEMRAM0 V_MEMRAM1 vuint8 V_MEMRAM2  CanRxActualData[8u];  /* Current received data   */

#  if defined( FBL_ENABLE_CAN_CONFIRMATION )
static void (*confirmationFunction)(CanTransmitHandle txObject);
#  endif

/* Initialization data for registers of CAN controller */
/* The structure is used for CAN initialization data after reset */
V_MEMROM0 V_MEMROM1 tFblCanIdTable V_MEMROM2 kFblCanIdTable =
{
   CAN_BCFG,

#   if (FBL_CAN_NUMBER_OF_ID_TABLE_ENTRIES > 0)
   {
#    if (FBL_CAN_NUMBER_OF_RX_ID > 0)
      /* ID- Codes */
       FBL_CAN_RX_ID0
#     if (FBL_CAN_NUMBER_OF_RX_ID > 1)
      ,FBL_CAN_RX_ID1
#     endif
#     if (FBL_CAN_NUMBER_OF_RX_ID > 2)
      ,FBL_CAN_RX_ID2
#     endif
#     if (FBL_CAN_NUMBER_OF_RX_ID > 3)
      ,FBL_CAN_RX_ID3
#     endif
#     if (FBL_CAN_NUMBER_OF_RX_ID > 4)
      ,FBL_CAN_RX_ID4
#     endif
#     if ( FBL_CAN_NUMBER_OF_RANGES > 0 )
    /* Add "," for following range entries */
      ,
#     endif
#    endif /* FBL_CAN_NUMBER_OF_RX_ID > 0 */

    /* Range filter configuration: ID code */
#   if ( FBL_CAN_NUMBER_OF_RANGES > 0 )
       FBL_CAN_CODE_0
#   endif
#   if ( FBL_CAN_NUMBER_OF_RANGES > 1 )
      ,FBL_CAN_CODE_1
#   endif
   },
     /* ID- Masks */
   {
#    if (FBL_CAN_NUMBER_OF_RX_ID > 0)
       FBL_CAN_RX_ID_MASK
#     if (FBL_CAN_NUMBER_OF_RX_ID > 1)
      ,FBL_CAN_RX_ID_MASK
#     endif
#     if (FBL_CAN_NUMBER_OF_RX_ID > 2)
      ,FBL_CAN_RX_ID_MASK
#     endif
#     if (FBL_CAN_NUMBER_OF_RX_ID > 3)
      ,FBL_CAN_RX_ID_MASK
#     endif
#     if (FBL_CAN_NUMBER_OF_RX_ID > 4)
      ,FBL_CAN_RX_ID_MASK
#     endif
#     if ( FBL_CAN_NUMBER_OF_RANGES > 0 )
    /* Add "," for following range entries */
      ,
#     endif
#    endif /* FBL_CAN_NUMBER_OF_RX_ID > 0 */

    /* Range filter configuration: ID mask */
#    if ( FBL_CAN_NUMBER_OF_RANGES > 0 )
       FBL_CAN_MASK_0
#    endif
#    if ( FBL_CAN_NUMBER_OF_RANGES > 1 )
      ,FBL_CAN_MASK_1
#    endif
   }
#   endif /* FBL_CAN_NUMBER_OF_ID_TABLE_ENTRIES */
};
#endif /* FBL_ENABLE_BUSTYPE_CAN */

/***********************************************************************************************************************
 *  EXTERNAL DATA
 **********************************************************************************************************************/

#if defined( FBL_ENABLE_BUSTYPE_CAN )
extern tFblCanIdTable fblCanIdTable; /* PRQA S 3447, 3451 */ /* MD_FblHw_3447_3451 */
#endif /* FBL_ENABLE_BUSTYPE_CAN */

/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/* PRQA S 0303 EOF */ /* MD_FblSfr_MemoryMappedRegister */
/***********************************************************************************************************************
 *  FblTimerInit
 **********************************************************************************************************************/
/*! \brief       Initialize the timer for a 1ms-cycle
 **********************************************************************************************************************/
void FblTimerInit( void )
{
#if defined( V_CPU_RH850 )
   /*
      Timer 0: Master - generates 1ms interrupt signal cyclically ( first one immediately)
      Timer 1: Slave  - counts ms signal from master from 0xffff downwards (0x0 -> 0xffff)
      Timer 2: Free running timer
   */

   FBL_TAUX0TT    = 0x0007u;      /* Stop timer 0,1,2 */

   FBL_TAUX0CMOR0 = 0x0801u;      /* CK0, Master Mode, interval timer, SW trigger, interrupt generation */
   FBL_TAUX0CMOR1 = 0x3006u;      /* CK0, Slave mode, Event counter mode, event = master interrupt */
   FBL_TAUX0CMOR2 = 0x4000u;      /* CK1, interval timer, SW trigger, no interrupt generation */

   FBL_TAUX0CMUR0 = 0x0000u;      /* Disable a TIN input */
   FBL_TAUX0CMUR1 = 0x0000u;      /* Disable a TIN input */
   FBL_TAUX0CMUR2 = 0x0000u;      /* Disable a TIN input */

   FBL_TAUX0RDE   = 0x0003u;      /* Simultaneous rewrite settings */
   FBL_TAUX0RDS   = 0x0000u;
   FBL_TAUX0RDC   = 0x0000u;
   FBL_TAUX0RDM   = 0x0000u;

   FBL_TAUX0TPS   = 0xff00u | (FBL_TIMER_PRESCALER_VALUE); /* CK0 = TimerClock/(2 ^ Prescaler), CK1 = TimerClock */

   FBL_TAUX0CDR0  = FBL_TIMER_RELOAD_VALUE; /* Setting of cycle value in data channel register */
   FBL_TAUX0CDR1  = 0xFFFFu;      /* Reload value */
   FBL_TAUX0CDR2  = 0xFFFFu;      /* Reload value */

   FBL_TAUX0TS    = 0x0007u;      /* Start timer 0,1,2 */
#endif /* V_CPU_RH850 */
}

#define FBLHW_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if defined( V_CPU_RH850 )
/***********************************************************************************************************************
 *  FblGetTimerValue
 **********************************************************************************************************************/
/*! \brief       Returns current timer value
 *  \pre         Timer must be running
 *  \return      Current timer value
 **********************************************************************************************************************/
vuint16 FblGetTimerValue(void)
{
   /* return the free running 16-bit counter value */
   return FBL_TAUX0CNT2;
}
#endif /* V_CPU_RH850 */

#define FBLHW_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if defined( FBL_ENABLE_BUSTYPE_CAN )
/***********************************************************************************************************************
 *  FblCanInit
 **********************************************************************************************************************/
/*! \brief       Initialization of the CAN registers
 **********************************************************************************************************************/
void FblCanInit( void )
{
   vuint8 canPhysChannel;

   vuint8 canFilterRulesIndex;
#  if defined( V_CPU_RH850 )
   vuint8 canFilterRulesPage;
#  endif

   vuint16 hwObjHandle;
   vuint8 canCount;

#  if defined( V_CPU_RH850 )
   Can = (tCanCellPtr)kFblCanBaseAdr;

#  endif
   /* Wait until CAN RAM initialization is complete */
   while (((Can->CGSR & kCanRamIst) != 0u))
   {
      ;
   }

   /* Transit to global reset mode */
   CanLL_GlobalModeReq(kCanResetMode);
   while ( (!CanLL_GlobalModeCheck(kCanResetMode)))
   {
      ;
   }

   /* Iterate physical channels to make sure all of them are in stop mode (currently in reset or stop) */
   for (canPhysChannel = 0u; canPhysChannel < kFblCanMaxPhysChannels; canPhysChannel++)
   {
     CanLL_ModeReq_Phys(canPhysChannel,kCanStopMode);
   }

   Can->CGCFG = kCanGlobalConfigSet;

   /* Clear receive rule settings */
   Can->CRNCFG[0] = kCanZero;
#  if (kFblCanMaxPhysChannels > 4)
   Can->CRNCFG[1] = kCanZero;
#  endif

   canPhysChannel = kFblCanChannel;

   /* Set channel reset mode (currently in stop or reset mode) */
   CanLL_ModeReq_Phys(canPhysChannel,kCanResetMode);
   while ((!CanLL_ModeCheck_Phys(canPhysChannel,kCanResetMode)))
   {
      ;
   }

   /* Init filter rules */
   Can->CRNCFG[CanLL_ByteIndex(canPhysChannel)] |= CanLL_FilterListConfig(canPhysChannel,FBL_CAN_NUMBER_OF_ID_TABLE_ENTRIES); /* rule layout: number of receive rules  */

   canFilterRulesIndex = 0u;
#  if defined( V_CPU_RH850 )
   canFilterRulesPage = 0u;
   Can->CRECR = kCanRuleWrite | canFilterRulesPage;    /* Set enable write to receive rules page */
#  endif

   for (hwObjHandle = 0u ; hwObjHandle < FBL_CAN_NUMBER_OF_ID_TABLE_ENTRIES; hwObjHandle++)
   {
      mRuleCode(canFilterRulesIndex) = fblCanIdTable.CanRxId[hwObjHandle]; /* IDE, RTR, RMS are set accordingly by macro */
      mRuleMask(canFilterRulesIndex) = fblCanIdTable.CanRxMask[hwObjHandle];

      /* Store messages in global rx buffers */
      mRuleBuf(canFilterRulesIndex) = kCanRuleUseBuf | ((hwObjHandle) << 8);
#  if defined( V_CPU_RH850 )
      mRuleFifo(canFilterRulesIndex) = kCanZero;
#  endif

      canFilterRulesIndex++;
   }

   /* Rx buffer layout */
   Can->CRBNUM = FBL_CAN_NUMBER_OF_ID_TABLE_ENTRIES;    /* Number of rx buffers */

#  if defined( V_CPU_RH850 )
   /* Receiving (disable rule write access) */
   Can->CRECR = kCanRuleReceive;
#  endif

   /* Global operating mode and global error interrupt enable/disable */

   CanLL_GlobalModeReq(kCanOperationMode);
   while ((!CanLL_GlobalModeCheck(kCanOperationMode)))
   {
      ;
   }

   /* Set bus-timing */
   Can->ChCtrl[canPhysChannel].BCFG = fblCanIdTable.BCFG;

   /* Set bus-off behavior */
   Can->ChCtrl[canPhysChannel].CR |= kCanHaltAtBusoff;

   /* Clear TB interrupt enable bits  */
   /* PRQA S 3355 1 */ /* MD_FblHw_Configuration */
   Can->CTBIER[CanLL_WordIndex(canPhysChannel)] &= ((kCanRegSize) FblInvert32Bit(CanLL_MaskWord(canPhysChannel)));

   /* Clear message status and send & cancel requests for all TB objects on channel */
   for(canCount = 0;canCount < kCanMaxTxBufCell;canCount++)
   {
      Can->ChBS[canPhysChannel].TBSR[canCount] &= (FblInvert8Bit(kCanSrTxBufMaskPending));
      Can->ChBC[canPhysChannel].TBCR[canCount] &= (FblInvert8Bit(kCanCrTxBufReq | kCanCrTxBufCancel));
   }

   /* Clear error flags */
   Can->ChCtrl[canPhysChannel].EF &= FblInvert32Bit(kCanEfMask);

   /* Clear rx full pending buffers 0 - 31*/
   Can->CRBRCF[0] = 0u;

   /* Switch to operation mode (and wait till it is reached) */
   CanLL_ModeReq_Phys(canPhysChannel,kCanOperationMode);

   while (!CanLL_ModeCheck_Phys(canPhysChannel,kCanOperationModeCheck))
   {
      ;
   }
}  /* PRQA S 6010 */ /* MD_MSR_STPTH */

/***********************************************************************************************************************
 *  FblCanInitIdentifier
 **********************************************************************************************************************/
/*! \brief       Initialize CAN initialization parameters
 **********************************************************************************************************************/
void FblCanParamInit( void )
{
#    if defined( FBL_ENABLE_MULTIPLE_NODES )
   /* The Tx Id is set in FblDiagInit() */
   fblCanIdTable.CanRxId[0] = CanRxId[fblEcuHandle];
#    endif  /* FBL_ENABLE_MULTIPLE_NODES  */
}

#   define FBLHW_START_SEC_CODE
#   include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/***********************************************************************************************************************
 *  FblCanTransmit
 **********************************************************************************************************************/
/*! \brief       This function transmits a CAN message
 *  \pre         CAN interface must be initialized before call
 *  \param[in]   tmtObject Pointer to TransmitObject
 *  \return      The function returns kFblCanTxOk if CAN cell accepted transmit or kFblCanTxFailed if not
 **********************************************************************************************************************/
vuint8 FblCanTransmit( tMsgObject * tmtObject ) /* PRQA S 3673 */ /* MD_FblHw_3673 */
{
   vuint8 result = kFblCanTxOk;

   /* Check for transmit message object free */
   if ( !CanLL_TxIsHWObjFree( kFblCanChannel, kCanTxMsgBuffer ))
   {
      /* Tx object n used, quit with error */
      result = kFblCanTxFailed;
   }
   else
   {
      if ((Can->ChBS[kFblCanChannel].TBSR[0] & kCanSrTxBufMaskPending) != 0)
      {
#if defined( FBL_ENABLE_CAN_CONFIRMATION )
         result = kFblCanTxFailed;
#else
         /* Clear complete or cancel flag and release the object */
         Can->ChBS[kFblCanChannel].TBSR[0] &= FblInvert8Bit(kCanSrTxBufMaskPending);
#endif
      }
   }

   if (kFblCanTxOk == result)
   {
      /* Set transmit ID */
      mBufId(kCanTxMsgBuffer)= tmtObject->msgObject.Id;

      /* Set DLC */
      mBufDlc(kCanTxMsgBuffer) = (((kCanRegSize)(tmtObject->msgObject.DLC)) << kFblCanDlcShift);

      /* Copy message data */
      mBufbData(kCanTxMsgBuffer,0) = tmtObject->msgObject.DataFld[0];
      mBufbData(kCanTxMsgBuffer,1) = tmtObject->msgObject.DataFld[1];
      mBufbData(kCanTxMsgBuffer,2) = tmtObject->msgObject.DataFld[2];
      mBufbData(kCanTxMsgBuffer,3) = tmtObject->msgObject.DataFld[3];
      mBufbData(kCanTxMsgBuffer,4) = tmtObject->msgObject.DataFld[4];
      mBufbData(kCanTxMsgBuffer,5) = tmtObject->msgObject.DataFld[5];
      mBufbData(kCanTxMsgBuffer,6) = tmtObject->msgObject.DataFld[6];
      mBufbData(kCanTxMsgBuffer,7) = tmtObject->msgObject.DataFld[7];

      /* Tx request */
      Can->ChBC[kFblCanChannel].TBCR[0] |= kCanCrTxBufReq;

#  if defined( FBL_ENABLE_CAN_CONFIRMATION )
      confirmationFunction = tmtObject->ConfirmationFct;
#  endif
   }

   return result;
}

/***********************************************************************************************************************
 *  FblCanMsgTransmitted
 **********************************************************************************************************************/
/*! \brief       This function returns kFblCanTxOk, if the message was transmitted.
 *  \pre         CAN interface must be initialized before call
 *  \return      kFblCanTxInProgress - ongoing transmission\n
 *               kFblCanTxOk         - message transmitted\n
 *               kFblCanTxFailed     - no TX in progress or finished
 **********************************************************************************************************************/
vuint8 FblCanMsgTransmitted( void )
{
   vuint8 result = kFblCanTxFailed;

   /* Check if message transmission is pending  */
   if (!CanLL_TxIsHWObjFree( kFblCanChannel, kCanTxMsgBuffer ))
   {
      /* Message transmission currently in progress */
      result = kFblCanTxInProgress;
   }
   else
   {
      if ((Can->ChBS[kFblCanChannel].TBSR[0] & kCanSrTxBufMaskPending) != 0u)
      {
#  if defined( FBL_ENABLE_CAN_CONFIRMATION )
         /* Call confirmation function if available */
         if (confirmationFunction != 0u)
         {
            confirmationFunction(0);
         }
#  endif
         /* Clear complete or cancel flag and release the object */
         Can->ChBS[kFblCanChannel].TBSR[0] &= FblInvert8Bit(kCanSrTxBufMaskPending);
         result = kFblCanTxOk;
      }
   }

   return result;
}
/***********************************************************************************************************************
 *  FblCanReceive
 **********************************************************************************************************************/
/*! \brief       Function searches for new received can messages
 *  \pre         CAN hardware has to be initialized before call
 *  \return      New message received ( value !=0 )
 **********************************************************************************************************************/
vuint32 FblCanReceive( void )
{
   /* Polling full-CAN rx objects */
   vuint32 rxIndicationFlag;

   /* Check for global full-CAN rx indication pending */
   if((Can->CRBRCF[0] & (kCanRegSize)kFblCanRxFlagMask) != 0u)
   {
      rxIndicationFlag = 0x01u;
      messageIndex = 0u;
      do
      {
         if ((Can->CRBRCF[0] & rxIndicationFlag) == rxIndicationFlag )
         {
            return rxIndicationFlag;
         }
         rxIndicationFlag <<= 1;
         messageIndex++;
      }
      while(messageIndex < FBL_CAN_NUMBER_OF_ID_TABLE_ENTRIES);
   }

   return 0;
}  /* PRQA S 2006 */ /* MD_MSR_14.7 */

/***********************************************************************************************************************
 *  FblCanProcessRxMessage
 **********************************************************************************************************************/
/*! \brief       Processing of the Id of the received CAN messages.
 *  \details     After the type of the message is determined, the right RxCanMsg-Handle is passed to FblHandleRxMsg()
 *               Finally the receive buffer is released.
 *  \pre         CAN hardware has to be initialized before call
 *               The variable messageIndex has been initialized in FblCanReceive
 **********************************************************************************************************************/
void FblCanProcessRxMsg( void )
{
   do
   {
      /* First reset complete flag */
# if defined( V_CPU_RH850 )
      Can->CRBRCF[0] = FblInvert32Bit(0x01u << messageIndex);
#endif

      CanRxActualData[0] =  mBufbData(messageIndex,0);
      CanRxActualData[1] =  mBufbData(messageIndex,1);
      CanRxActualData[2] =  mBufbData(messageIndex,2);
      CanRxActualData[3] =  mBufbData(messageIndex,3);
      CanRxActualData[4] =  mBufbData(messageIndex,4);
      CanRxActualData[5] =  mBufbData(messageIndex,5);
      CanRxActualData[6] =  mBufbData(messageIndex,6);
      CanRxActualData[7] =  mBufbData(messageIndex,7);

      CanRxActualDLC = (vuint8)((mBufDlc(messageIndex) >> kFblCanDlcShift) & ((vuint32)kCanActDlcMask));
      CanRxActualId  = mBufId(messageIndex);

      /* Two conditions lead to a situation where the complete flag is set here again:
       1) a new message was received, while copying data from the hw -> overrun
       2) the flag could not be cleared as hw copying to the buffer was still in progress

       The process of copying data is repeated until this flag is 0
       -> copied data is consistent (as flag cannot be cleared when hw copying is still in
          progress and is set to 1 when the copying of a new message starts (see hw manual)
       -> always the newest data is copied

       Since most overruns will not be detected (only chance is here while accessing the
       buffer), there is no report of an overrun in this case either.
      */
   } while ((Can->CRBRCF[0] & (vuint32)(0x01 << messageIndex)) != 0u);

#if defined( FBL_ENABLE_STDID_OPTIMIZATION )
   CanRxActualId = CanRxActualId & kCanStdIdMask;
#elif defined( FBL_ENABLE_EXTID_OPTIMIZATION )
   CanRxActualId = CanRxActualId & kCanExtIdMask;
#else
   if (CanRxActualIdType(CanRxActualId) == kCanIdTypeExt)
   {
      CanRxActualId = CanRxActualId & kCanExtIdMask;
   }
   else
   {
      CanRxActualId = CanRxActualId & kCanStdIdMask;
   }
#endif

   /* Message index starts at 0, message handles start with "1" */
   FblHandleRxMsg((vuint8)(messageIndex + 1u),CanRxActualData);
}

#   define FBLHW_STOP_SEC_CODE
#   include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#  if defined( FBL_ENABLE_STAY_IN_BOOT ) 
/***********************************************************************************************************************
 *  FblCanStartMsg
 **********************************************************************************************************************/
/*! \brief       Returns kFblOk after a start message request has been received.
 *  \details     Note: Rx-Buffer is not released if message was the start message.
 *               Required, since data must be safely processed after reception.
 *  \pre         CAN hardware has to be initialized before call; May only be called after a CAN message was received
 *               The variable messageIndex has been initialized in FblCanReceive
 *  \return      Return value says whether a start message request was received
 **********************************************************************************************************************/
vuint8 FblCanStartMsg( void )
{
   /* Message index starts at 0, message handles start with "1" */
   if ((CAN_RX_SLOT_STARTMSG  == (messageIndex + 1u))
#   if defined( CAN_RX_SLOT_STARTMSG2 )
      || (CAN_RX_SLOT_STARTMSG2 == (messageIndex + 1u))
#   endif
      )
   {
      do
      {
         /* First reset complete flag */
# if defined( V_CPU_RH850 )
         Can->CRBRCF[0] = FblInvert32Bit(0x01u << messageIndex);
#endif

         CanRxActualData[0] =  mBufbData(messageIndex,0);
         CanRxActualData[1] =  mBufbData(messageIndex,1);
         CanRxActualData[2] =  mBufbData(messageIndex,2);
         CanRxActualData[3] =  mBufbData(messageIndex,3);
         CanRxActualData[4] =  mBufbData(messageIndex,4);
         CanRxActualData[5] =  mBufbData(messageIndex,5);
         CanRxActualData[6] =  mBufbData(messageIndex,6);
         CanRxActualData[7] =  mBufbData(messageIndex,7);

         CanRxActualDLC = (vuint8)((mBufDlc(messageIndex) >> kFblCanDlcShift) & ((vuint32)kCanActDlcMask));

         CanRxActualId  = mBufId(messageIndex);

      } while((Can->CRBRCF[0] & (vuint32)(0x01 << messageIndex)) != 0u);

#if defined( FBL_ENABLE_STDID_OPTIMIZATION )
      CanRxActualId = CanRxActualId & kCanStdIdMask;
#elif defined( FBL_ENABLE_EXTID_OPTIMIZATION )
      CanRxActualId = CanRxActualId & kCanExtIdMask;
#else
      if (CanRxActualIdType(CanRxActualId) == kCanIdTypeExt)
      {
         CanRxActualId = CanRxActualId & kCanExtIdMask;
      }
      else
      {
         CanRxActualId = CanRxActualId & kCanStdIdMask;
      }
#endif
      return FblCheckOwnStartMsg(CanRxActualData);
   }
   else
   {
      /* Reset complete flag */
# if defined( V_CPU_RH850 )
      Can->CRBRCF[0] = FblInvert32Bit(0x01u << messageIndex);
#endif

      return kFblFailed;
   }
}
#  endif

/***********************************************************************************************************************
 *  FblCanErrorTask
 **********************************************************************************************************************/
/*! \brief       This function checks if the CAN hardware is in a bus-off state.
 *  \details     If this state is detected, the recovery will be initiated.
 *  \pre         CAN hardware has to be initialized before call
 **********************************************************************************************************************/
void FblCanErrorTask(void)
{
   if(CanLL_HwWasBusOff(kFblCanChannel))
   {
      /* Inform application */
      ApplFblCanBusOff();
   }
}

#  if defined( FBL_ENABLE_SLEEPMODE )
/***********************************************************************************************************************
 *  FblCanSleep
 **********************************************************************************************************************/
/*! \brief       Enter sleep mode
 *  \pre         CAN interface must be initialized before call
 **********************************************************************************************************************/
vuint8 FblCanSleep( void )
{
   /* enter channel reset mode */
   CanLL_ModeReq_Phys(kFblCanChannel,kCanResetMode);
   while ((!CanLL_ModeCheck_Phys(kFblCanChannel,kCanResetMode)))
   {
      ;
   }

   /* Clear rx full reception flags 0 - 31*/
   Can->CRBRCF[0] = 0u;

   /* Deactivate clock */
   CanLL_ModeReq_Phys(kFblCanChannel,kCanStopMode);

   return kFblCanSleepOk;
}

/***********************************************************************************************************************
 *  FblCanIsAsleep
 **********************************************************************************************************************/
/*! \brief       Returns the state of CAN cell regarding sleep mode
 *  \return      1 if can is asleep, 0 if can is awake
 **********************************************************************************************************************/
vuint8 FblCanIsAsleep(void)
{
   /* Not supported, return 0 */
   return 0u;
}

/***********************************************************************************************************************
 *  FblCanWakeUp
 **********************************************************************************************************************/
/*! \brief       Wake up the can controller by clearing the sleep bit in the CCR (Condition Control Register)
 **********************************************************************************************************************/
void FblCanWakeUp( void )
{
   /* No generic wake up implementation available. Please *
    * implement wake up in user callback.                 */
   return;
}

#   if defined( FBL_ENABLE_WAKEUP_INT )
/***********************************************************************************************************************
 *  CanWakeUpInterrupt
 **********************************************************************************************************************/
/*! \brief       Wake-up interrupt service function
 *  \pre         Wake-up interrupt has to be enabled before entering sleepmode
 **********************************************************************************************************************/
void FblCanWakeUpInterrupt( void )
{
}
#   endif /* FBL_ENABLE_WAKEUP_INT */
#  endif /* FBL_ENABLE_SLEEPMODE */
/* End of interface for CAN **************************************************/
#endif /* FBL_ENABLE_BUSTYPE_CAN */

/***********************************************************************************************************************
 *  FblReadByte
 **********************************************************************************************************************/
/*! \brief          This function can be used to read one byte from the given address.
 *  \param[in]      Read address.
 *  \return         Byte at memory location 'address'
 **********************************************************************************************************************/
vuint8 FblReadByte( FBL_ADDR_TYPE address )
{
   /* PRQA S 0306 1 */ /* MD_FblHw_MemoryMappedFeature */
   return *(V_MEMROM1_FAR vuint8 V_MEMROM2_FAR V_MEMROM3 *)address;
}

/***********************************************************************************************************************
 *  FblReadBlock
 **********************************************************************************************************************/
/*! \brief        This function copies a given amount of bytes.
 *  \param[in]    address Memory address to read out
 *  \param[out]   buffer Target buffer
 *  \param[in]    length Number of bytes to read
 *  \return       Number of actually copied bytes
 **********************************************************************************************************************/
FBL_MEMSIZE_TYPE FblReadBlock( FBL_ADDR_TYPE address, vuint8 * buffer, FBL_MEMSIZE_TYPE length )
{
   /* PRQA S 0306 1 */ /* MD_FblHw_MemoryMappedFeature */
   (void)MEMCPY (buffer, address, length);
   return length;
}

#if defined( FBL_USE_OWN_MEMCPY )
#  define MEMCPY_START_SEC_CODE
#  include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
/***********************************************************************************************************************
 *  ownmemcpy
 **********************************************************************************************************************/
/*! \brief       Bytewise copy of size bytes from source to destination
 *  \param[out]  Dest Pointer to destination buffer
 *  \param[in]   Source Pointer to source buffer
 *  \param[in]   size Number of bytes to be copied
 *  \return      Pointer to destination
 **********************************************************************************************************************/
void * ownmemcpy(void * dest, V_MEMRAM1_FAR void V_MEMRAM2_FAR V_MEMRAM3 const * source, tFblLength size)
{
   tFblLength i;
   vuint8 * sd = (vuint8 *)dest;
# if defined( V_CPU_RH850 )
   vuint8 const * ss = (vuint8 const *)source;
# endif

   (void)FblLookForWatchdog();

   for(i = 0u; i < size; i++)
   {
      if ((i & (tFblLength)0x0Fu) == 0x00u)
      {
         (void)FblLookForWatchdog();
      }

# if defined( V_CPU_RH850 )
      sd[i] = ss[i];
# endif
   }

   return dest;
}
#  define MEMCPY_STOP_SEC_CODE
#  include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
#endif /* FBL_USE_OWN_MEMCPY */

/* Module specific MISRA deviations:

   MD_FblHw_Configuration:
      Reason:     Due to configuration specific code which might be included in other configurations.
      Risk:       Introducing further complexity due to potential unnecessary code.
      Prevention: Reviews.

   MD_FblHw_MemoryMappedFeature:
      Reason:     Casting of integer to pointer type and vice versa is required through memory mapped variables and
                  functions.
      Risk:       The size of integer required to hold the result of a pointer cast is implementation-defined.
      Prevention: Test of delivery on real hardware.

   MD_FblHw_0306:
      Reason:     Addition/Subtraction on far pointer is not possible. Therefore the indirection with integral type is
                  is used.
      Risk:       The size of integer required to hold the result of a pointer cast is implementation-defined.
      Prevention: Test of delivery on real hardware.

   MD_FblHw_3447_3451:
      Reason:     The current design of the HW-layer does not offer fblCanIdTable declared extern in header file.
                  Sometimes, double declarations may occur.
      Risk:       Reduced Maintainability.
      Prevention: No prevention. This will have to be improved in future design.

   MD_FblHw_3673:
      Reason:     Preservation of generic interfaces.
      Risk:       No risk.
      Prevention: No prevention.
*/
/***********************************************************************************************************************
 *  END OF FILE: FBL_HW.C
 **********************************************************************************************************************/
