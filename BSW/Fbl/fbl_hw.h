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
 *  01.00.00   2013-05-04  CB      ESCAN00067920    Empty response message on channel other than 0
 *                                                  Release
 *  01.01.00   2013-09-24  MVi     ESCAN00070496    Added IAR RL78 RS-CAN Lite
 *  01.02.00   2013-12-20  MVi     ESCAN00070930    No changes
 *                                 ESCAN00070931    Rework ownmemcpy memory qualifier
 *                                 ESCAN00072679    No changes
 *                                 ESCAN00072761    No changes
 *  01.03.00   2014-03-05  TBe     ESCAN00074028    Added support for RL78 Renesas compiler
 *  01.04.00   2014-03-26  CB      ESCAN00074448    No changes
 *  01.05.00   2014-05-22  Dod     ESCAN00074973    Added support for normal fixed addressing
 *                         CB      ESCAN00075823    Added support for RH850 WindRiver/DiabData compiler
 *  01.06.00   2014-07-01  CEl     ESCAN00076640    No changes
 *  01.07.00   2014-08-01  HRf     ESCAN00077536    Add support for GM SLP4
 *  01.08.00   2014-08-27  Ci      ESCAN00078093    Added support for F1H (fbl_sfr.h)
 *                                 ESCAN00078095    Adapted configuration switches
 *                         CEl     ESCAN00078784    Adapted configuration switches
 *                         Rr      ESCAN00078791    Added alternative clock source support
 *                         Rr      ESCAN00078970    No changes
 *                         CEl     ESCAN00079009    Added support for RZ
 *  01.09.00   2015-02-18  CB      ESCAN00079110    Added support for CAN receive ranges
 *                         AND     ESCAN00079110    Fixed macro names for CAN receive ranges
 *                         ASe     ESCAN00080122    No changes
 *                         Ci      ESCAN00080218    Added support for R1x (fbl_sfr.h)
 *                         Rr      ESCAN00080422    No changes
 *  01.10.00   2015-04-29  CB      ESCAN00082512    Minor changes for sleep mode
 *                         QPs     ESCAN00082784    Added support for Renesas compiler
 *  01.11.00   2015-07-09  Shs     ESCAN00083898    No changes
 *  01.11.01   2015-07-30  KJs     ESCAN00084288    No changes
 *  01.11.02   2015-10-26  TnS     ESCAN00084351    Compiler error: Component does not compile for legacy ID handling
 *  01.12.00   2015-10-29  TnS     ESCAN00086139    Added function to restore CPU base pointers
 *  01.13.00   2015-11-23  Shs     ESCAN00086688    No changes
 *  01.13.01   2015-12-23  ASe     ESCAN00087278    No changes
 *  01.13.02   2016-02-25  TnS     ESCAN00078239    No changes
 *                                 ESCAN00088455    MISRA related changes
 *  01.13.03   2016-02-29  Rie     ESCAN00088610    Fixed compiler error for undefined typedef tFblCanIdTable
 *  01.14.00   2016-03-18  CB      ESCAN00088977    No changes
 *  01.14.01   2016-04-05  TnS     ESCAN00089231    No changes
 *  01.15.00   2016-05-09  TnS     ESCAN00089926    No changes
 *                                 ESCAN00089958    Added FblHwRestoreFblContext()
 *  01.16.00   2016-05-23  CB      ESCAN00090124    Support for E1x
 *  01.16.01   2016-05-23  DKl     ESCAN00090520    Added FblHwRestoreFblContext() for RL78
 *  01.17.00   2016-07-13  TnS     ESCAN00090720    No changes
 *                         CB      ESCAN00090982    Added memory qualifiers for CAN-Id-constants
 *  01.18.00   2016-07-18  Shs     ESCAN00091047    No changes
 *             2016-07-21  TBe     ESCAN00090770    Incorrect memory access using wrong gp and tp register
 *                                 ESCAN00091092    FblGetTimerValue might not get linked into RAM
 *  01.19.00   2016-07-28  DlM     ESCAN00091227    Added support for F1K
 *             2016-08-02  JRe     ESCAN00091290    Support of CC-RL inline assembler instructions
 *  01.19.01   2016-08-05  JRe     ESCAN00091339    Compiler error: FblInterruptDisable is missing on RL78/IAR
 *  01.19.02   2016-11-18  DlM     ESCAN00092878    Added second CAN channel support for RL78 derivative F15
 *             2016-11-28  AWh     ESCAN00093053    No changes
 *  01.20.00   2017-01-09  DlM     ESCAN00093473    Added switch FBL_ENABLE_BUSTYPE_CAN for enable or disable CAN relevant code parts
 **********************************************************************************************************************/

#ifndef __FBL_HW_H__
#define __FBL_HW_H__

/***********************************************************************************************************************
 *  VERSION
 **********************************************************************************************************************/

/* ##V_CFG_MANAGEMENT ##CQProject : FblDrvCan_Rh850RscanCrx CQComponent : Implementation */
#define FBLDRVCAN_RH850RSCANCRX_VERSION          0x0120u
#define FBLDRVCAN_RH850RSCANCRX_RELEASE_VERSION  0x00u

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

#if defined( FBL_USE_OWN_MEMCPY )
#else
#include <string.h>
#endif
#include "fbl_sfr.h"  /* Special function register definitions */

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

/* PRQA S 3458 EOF */ /* MD_MSR_19.4 */
/* PRQA S 3453 EOF */ /* MD_MSR_19.7 */

#if defined( FBL_ENABLE_BUSTYPE_CAN )
# define FBLDRVCAN_REFERENCE_IMPLEMENTATION 0x0100u

/* CAN handling -------------------------------------------------------------*/

/* Return values for precopy-routines */
#  define kCanNoCopyData     0x00u
#  define kCanCopyData       0x01u

/* Set up the right CAN driver interface for the higher layers */
/* Full CAN driver.                                            */
#  define MULTIPLE_RECEIVE_BUFFER

#  define FBL_CAN_NUMBER_OF_ID_TABLE_ENTRIES (FBL_CAN_NUMBER_OF_RX_ID + FBL_CAN_NUMBER_OF_RANGES)

/* Handle of receive message */
#  if (FBL_CAN_NUMBER_OF_RX_ID > 0)
#   define FblRxCanMsg0Hdl   1u
#  endif
#  if (FBL_CAN_NUMBER_OF_RX_ID > 1)
#   define FblRxCanMsg1Hdl   2u
#  endif
#  if (FBL_CAN_NUMBER_OF_RX_ID > 2)
#   define FblRxCanMsg2Hdl   3u
#  endif
#  if (FBL_CAN_NUMBER_OF_RX_ID > 3)
#   define FblRxCanMsg3Hdl   4u
#  endif
#  if (FBL_CAN_NUMBER_OF_RX_ID > 4)
#   define FblRxCanMsg4Hdl   5u
#  endif
#  if defined( FBL_ENABLE_CAN_RX_RANGE )
#   if (FBL_CAN_NUMBER_OF_RANGES > 0)
#    define FblRxRange0Hdl  (FBL_CAN_NUMBER_OF_RX_ID+1)
#   endif
#   if (FBL_CAN_NUMBER_OF_RANGES > 1)
#    define FblRxRange1Hdl  (FBL_CAN_NUMBER_OF_RX_ID+2)
#   endif
#  endif

#  define CAN_RX_SLOT_0         1u
#  define CAN_RX_SLOT_1         2u
#  define CAN_RX_SLOT_2         3u
#  define CAN_RX_SLOT_3         4u
#  define CAN_RX_SLOT_4         5u
#  if defined( FBL_ENABLE_CAN_RX_RANGE )
#   if (FBL_CAN_NUMBER_OF_RANGES > 0)
#    define CAN_RX_RANGE_0 (FBL_CAN_NUMBER_OF_RX_ID+1)
#   endif
#   if (FBL_CAN_NUMBER_OF_RANGES > 1)
#    define CAN_RX_RANGE_1 (FBL_CAN_NUMBER_OF_RX_ID+2)
#   endif
#  endif

#  define FblCanRetransmit()    {Can->ChBS[kFblCanChannel].TBSR[0] &= FblInvert8Bit(kCanSrTxBufMaskPending);Can->ChBC[kFblCanChannel].TBCR[0] |= kCanCrTxBufReq;}

/* Return values of CanRxActualIdType */
#  define kCanIdTypeStd         0x00000000ul
#  define kCanIdTypeExt         0x80000000ul
#  define kCanStdIdMask         0x000007FFul
#  define kCanExtIdMask         0x1FFFFFFFul
#  define kCanActDlcMask        0x0Fu

/* Standard CAN driver macros */
#  define FBL_CAN_RX_ID_MASK    kCanRuleExtMustMatch

/* Convert ID to hardware format */
#   if defined( FBL_ENABLE_STDID_OPTIMIZATION )
#   else
#    define CanRxActualIdType(idRaw)  ((vuint32)(idRaw) & kCanIdTypeExt)
#    define MK_EXTID(id)              ((id) | kCanIdTypeExt)
#    define GET_EXTID_RAW(id)         (vuint32)((id) & 0x1FFFFFFFul)
#    if defined( FBL_ENABLE_CAN_RX_RANGE )
#     define MK_EXTMASK(id)            ((FblInvert32Bit(id)) & ((vuint32)kCanRuleExtMustMatch))
#     define MK_EXTCODE(id)            MK_EXTID(id)
#    endif /* FBL_ENABLE_CAN_RX_RANGE */
#   endif /* FBL_ENABLE_STDID_OPTIMIZATION */
#   if defined( FBL_ENABLE_EXTID_OPTIMIZATION )
#   else
#    define MK_STDID(id) ((id) & kCanStdIdMask)
#    define GET_STDID_RAW(id)         (vuint32)((id) & 0x7FFul)
#    if defined( FBL_ENABLE_CAN_RX_RANGE )
#     define MK_STDMASK(id)            ((FblInvert32Bit(id)) & (vuint32)kCanRuleStdMustMatch)
#     define MK_STDCODE(id)            MK_STDID(id)
#    endif /* FBL_ENABL_CAN_RX_RANGE */
#   endif /* FBL_ENABLE_EXTID_OPTIMIZATION */

/* Macros to access transmit message structure */

/* Set current ID */
#  define FblCanSetTxId(MsgObj,id) ((MsgObj).msgObject.Id=(id))
/* Set current DLC */
#  define FblCanSetTxDLC(MsgObj,dlc)  ((MsgObj).msgObject.DLC=(dlc))
/* Set current confirmation function */
#  define FblCanSetTxConfirmation(MsgObj, ConfFctPtr) ((MsgObj).ConfirmationFct=(ConfFctPtr))

/* RSCAN register masks */
#  define kCanIntMaskGlobalError       0x00000F00ul
#  define kCanIntMaskChError           0x0001FF00ul
#  define kCanIntMaskRxFifo            0x00000002ul
#  define kCanIntMaskTxFifo            0x00000004ul
#  define kCanIntMaskBusoff            0x00000800ul /* BO entry */
#  define kCanIntMaskCancel            0x00010000ul
#  define kCanHaltAtBusoff             0x00200000ul /* Default: Halt mode at bus-off entry (BOM=01) */
#  define kCanSrMaskBusoff             0x00000010ul
#  define kCanSrMaskPassive            0x00000008ul
#  define kCanSrMaskRec                0x00FF0000ul
#  define kCanSrMaskTec                0xFF000000ul
#  define kCanEfMask                   0x00007FFFul
#  define kCanEfMaskBusoff             0x00000008ul /* BO entry */
#  define kCanIntEnableGlobalError     0x00000200ul /* Fifo message lost - overrun */

#  if defined( FBL_HW_ENABLE_ALTERNATIVE_CLOCK_SOURCE )
#   define kCanGlobalConfigSet         0x00000010ul /* Clock source select = clk_xincan */
#  else
#   define kCanGlobalConfigSet         0x00000000ul /* Default: Clock source select = clkc */
#  endif

#  define kCanRuleWrite                0x00000100ul
#  define kCanRuleReceive              0x00000000ul
#  define kCanRuleUseBuf               0x00008000ul
#  define kCanRuleStdMustMatch         0xC00007FFul
#  define kCanRuleExtMustMatch         0xDFFFFFFFul
#  define kCanRuleStdHeader            0xC0000000ul
#  define kCanRulePageMask             0xFFF0u
#  define kCanRuleIndexMask            0x000Fu

#  define kCanOne16                    (vuint16)0xFFFFu
#  define kCanOne8                     (vuint8)0xFFu
#  define kCanOne                      1u
#  define kCanZero                     0u
#  if defined( V_CPU_RH850 )
#   define kCanRxFifoMask              0x000000FFul
#   define kCanDlcMask                 0xF0000000ul
#  endif

#  define kCanModeMask                 0xFFFFFFF8ul /* Incl. STP */
#  define kCanModeBits                 0x00000007ul /* Default: No check for COMST */
#  define kCanOperationMode            0x00000000ul
#  define kCanOperationModeCheck       0x00000080ul /* Incl. COMST */
#  define kCanResetMode                0x00000001ul /* Global and channel */
#  define kCanTestMode                 0x00000002ul /* Global only */
#  define kCanHaltMode                 0x00000002ul /* Channel only */
#  define kCanStopMode                 0x00000005ul /* Global and channel */
#  define kCanRamIst                   0x00000008ul

#  define kCanCrTxBufReq               0x01u
#  define kCanCrTxBufCancel            0x02u
#  define kCanSrTxBufMaskPending       0x06u
#  define kCanSrTxBufMaskComplete      0x04u
#  define kCanSrTxBufMaskCancel        0x02u
#  define kCanSrTxBufMaskFree          0x1Fu
#  define kCanSrTxBufMaskAReq          0x10u
#  define kCanSrTxBufMaskTReq          0x08u

#  if defined( V_CPU_RH850 )
#   define kCanHwTxStartIndex          160u
#  endif

#  if defined( V_CPU_RH850 )
#  define kCanTxMsgBuffer              (kCanHwTxStartIndex + (16u * kFblCanChannel))
#  endif

#endif /* FBL_ENABLE_BUSTYPE_CAN */

#if defined( V_CPU_RH850 ) 
/* Compatibility to older tool versions */
# if defined( FBL_USE_OWN_MEMCPY )
#  define MEMCPY(dst, src, len) \
          ownmemcpy((void *)(dst), (V_MEMRAM1_FAR void V_MEMRAM2_FAR V_MEMRAM3 const *)(src), (tFblLength)(len))
# else
#  define MEMCPY(dst, src, len) memcpy((void *)(dst), (V_MEMROM2 void *)(src), (size_t)(len))
# endif
#endif

#if defined( V_CPU_RH850 )
/* Address definition for jump macros */
#define RESETVECT                         0x0u                                            /**< Bootloader startup code */
#define APPLSTART                         (*((volatile vuint32 *)&ApplIntJmpTable[1u]))   /**< Startup code of application */
#define FblCheckBootVectTableIsValid()    (((volatile vuint32*)ApplIntJmpTable)[0u] == APPLVECT_FROM_BOOT)

/* Macros for jumps */
#define JSR(x) (*((void(*)(void))x))()   /* Jump to SubRoutine */

#define JSR_APPL()         JSR(APPLSTART)
#define JSR_RESET()        JSR(RESETVECT)

/* Address de-serialization */
#define FblFlashAddressGet3Bytes(a)    (FBL_ADDR_TYPE)(((a)[0] << 16) + ((a)[1] << 8) + (a)[2])
#define FblDownloadLengthGet3Bytes(a)  (FBL_MEMSIZE_TYPE)(((a)[0] << 16) + ((a)[1] << 8) + (a)[2])

/* Read 4 bytes from diagnostic buffer */
#define FblFlashAddressGet4Bytes(a)    (FBL_ADDR_TYPE)(((a)[0] << 24) + ((a)[1] << 16) + ((a)[2] << 8) + (a)[3])
#define FblDownloadLengthGet4Bytes(a)  (FBL_MEMSIZE_TYPE)(((a)[0] << 24) + ((a)[1] << 16) + ((a)[2] << 8) + (a)[3])

/* Restore base pointer register of FBL context */
#define FblHwRestoreFblContext()       { \
                                          /* Restore base pointers of FBL */ \
                                          __asm(" .extern __tp "); \
                                          __asm(" .extern __gp "); \
                                          /* Initialisation of the global pointer) */ \
                                          __asm(" movhi hi(__gp),zero,gp"); \
                                          __asm(" movea lo(__gp),gp,gp"); \
                                          /* Initialisation of the text pointer*/ \
                                          __asm(" movhi hi(__tp),zero,tp"); \
                                          __asm(" movea lo(__tp),tp,tp"); \
                                       }

/* Timer handling ----------------------------------------------------------*/

#define FblTimerStopp()              FBL_TAUX0TT |= 0x0007u;
#define FblTimerGet()                ((FBL_ICTAUX0I0 & 0x1000) == 0x1000u)
#define FblTimerReset()              FBL_ICTAUX0I0 &= (vuint16)~0x1000u

# if defined( V_COMP_RENESAS )
#define FblInterruptDisable()        __DI()
#define FblInterruptEnable()         __EI()
# else
#define FblInterruptDisable()        __asm("  di")
#define FblInterruptEnable()         __asm("  ei")
# endif /* V_COMP_RENESAS */

#define FblBootProtect()
#endif /* V_CPU_RH850 */

/***********************************************************************************************************************
 *  GLOBAL DATA TYPES
 **********************************************************************************************************************/

#if defined( FBL_ENABLE_BUSTYPE_CAN )
/* Data types for driver function parameters */
typedef vuint8 CanTransmitHandle;
typedef vuint8 CanReceiveHandle;
typedef vuint8 CanInitHandle;
typedef volatile unsigned char * pChipDataPtr;

/* Status/Control-Register */

/** Initialization structure for CAN driver */
typedef struct tagFblCanIdTable
{
   vuint32  BCFG;                           /**< Channel configuration register  */
#   if ( FBL_CAN_NUMBER_OF_ID_TABLE_ENTRIES > 0 )
   vuint32 CanRxId[FBL_CAN_NUMBER_OF_ID_TABLE_ENTRIES];/**< Receive IDs for full CAN        */
   vuint32 CanRxMask[FBL_CAN_NUMBER_OF_ID_TABLE_ENTRIES];/**< Receive mask for full CAN        */
#   endif  /* FBL_CAN_NUMBER_OF_RX_ID */
} tFblCanIdTable;

/** CAN register map */
typedef volatile struct
{
   vuint32 Id;               /**< Complete ID                                    */
   vuint8  DLC;              /**< Data length reg.:  X X X X DLC3 DLC2 DLC1 DLC0 */
   vuint8  DataFld[8];       /**< Data 0 .. 7                                    */
} tInternalMsgObject;

typedef volatile struct
{
   tInternalMsgObject msgObject;
   void (*ConfirmationFct)(CanTransmitHandle txObject);
} tMsgObject;

#endif /* FBL_ENABLE_BUSTYPE_CAN */

/***********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

/* Initialize ms timer */
void FblTimerInit( void );

#define FBLHW_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
#if defined( V_CPU_RH850 )
vuint16 FblGetTimerValue( void );
#endif
#define FBLHW_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if defined( FBL_ENABLE_BUSTYPE_CAN )
void FblCanInit( void );
void FblCanParamInit( void );

#  if defined( FBL_ENABLE_STAY_IN_BOOT ) 
/* For startup delay phase only */
vuint8 FblCanStartMsg( void );
#  endif

#   define FBLHW_START_SEC_CODE
#   include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* Transmit CAN message */
vuint8 FblCanTransmit(tMsgObject *);

/* Check if new message was received */
vuint32 FblCanReceive( void );

/* Function to handle CAN receive messages */
void FblCanProcessRxMsg( void );

/* Polling function for transmitted CAN messages */
vuint8 FblCanMsgTransmitted( void );

#   define FBLHW_STOP_SEC_CODE
#   include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#  if defined( FBL_ENABLE_CAN_CONFIRMATION )
/* Function to handle tx confirmation*/
void FblCanConfirmation(void);
#  endif

void FblCanErrorTask( void );

#  if defined( FBL_ENABLE_SLEEPMODE )
/** Function to enter sleep mode */
vuint8 FblCanSleep( void );
/** Function to check sleep mode */
vuint8 FblCanIsAsleep( void );

/** Function to quit sleep mode */
void FblCanWakeUp( void );
#   if defined( FBL_ENABLE_WAKEUP_INT )
void FblCanWakeUpInterrupt( void );
#   endif /* FBL_ENABLE_WAKEUP_INT */
#  endif /* FBL_ENABLE_SLEEPMODE */
#endif /* FBL_ENABLE_BUSTYPE_CAN */

/* Memory access functions */
vuint8 FblReadByte( FBL_ADDR_TYPE address );
FBL_MEMSIZE_TYPE FblReadBlock( FBL_ADDR_TYPE address, vuint8 * buffer, FBL_MEMSIZE_TYPE length );
#if defined( FBL_USE_OWN_MEMCPY )
#  define MEMCPY_START_SEC_CODE
#  include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
void * ownmemcpy(void * dest, V_MEMRAM1_FAR void V_MEMRAM2_FAR V_MEMRAM3 const * source, tFblLength size);
#  define MEMCPY_STOP_SEC_CODE
#  include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
#endif   /* FBL_USE_OWN_MEMCPY */

/***********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

#if defined( FBL_ENABLE_BUSTYPE_CAN )
#  if defined( V_CPU_RH850 )
extern volatile tCanCellPtr Can;
#  endif

extern vuint8 CanEcuNumber;

#    if defined( FBL_ENABLE_MULTIPLE_NODES )
V_MEMROM0 extern V_MEMROM1 vuint32 V_MEMROM2 CanTxId[FBL_NUMBER_OF_NODES];
V_MEMROM0 extern V_MEMROM1 vuint32 V_MEMROM2 CanRxId[FBL_NUMBER_OF_NODES];
#    endif  /* FBL_ENABLE_MULTIPLE_NODES */

/* CAN driver interface */
V_MEMRAM0 extern V_MEMRAM1 vuint32 V_MEMRAM2 CanRxActualId;
/** Current received DLC */
V_MEMRAM0 extern V_MEMRAM1 vuint8 V_MEMRAM2 CanRxActualDLC;
V_MEMRAM0 extern V_MEMRAM1 vuint8 V_MEMRAM2 CanRxActualData[8u];

V_MEMROM0 extern V_MEMROM1 tFblCanIdTable V_MEMROM2 kFblCanIdTable;
#endif /* FBL_ENABLE_BUSTYPE_CAN */

#endif /* __FBL_HW_H__ */

/***********************************************************************************************************************
 *  END OF FILE: FBL_HW.H
 **********************************************************************************************************************/
