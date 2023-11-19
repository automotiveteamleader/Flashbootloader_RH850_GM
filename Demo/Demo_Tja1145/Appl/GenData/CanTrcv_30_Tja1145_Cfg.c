/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *
 *                 This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                 Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                 All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  LICENSE
 *  -------------------------------------------------------------------------------------------------------------------
 *            Module: CanTrcv
 *           Program: GM Global B (MSR_Gm_SLP1)
 *          Customer: Nexteer Automotive Corporation
 *       Expiry Date: Not restricted
 *  Ordered Derivat.: Renesas RH850 P1M R7F701363
 *    License Scope : The usage is restricted to CBD1500636_D02
 *
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *              File: CanTrcv_30_Tja1145_Cfg.c
 *   Generation Time: 2017-07-11 15:18:02
 *           Project: Tja1145 - Version 1.0
 *          Delivery: CBD1500636_D02
 *      Tool Version: DaVinci Configurator (beta) 5.14.11 SP1
 *
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 ! BETA VERSION                                                                                                       !
 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 ! These programs are fully operative programs. With regard to the fact that the programs are a beta-version only,    !
 ! Vector Informatik's liability shall be expressly excluded in cases of ordinary negligence, to the extent           !
 ! admissible by law or statute.                                                                                      !
 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
**********************************************************************************************************************/


/* PRQA S 0777 EOF */ /* MD_CanTrcv_30_Tja1145_0777 */
/* PRQA S 0779 EOF */ /* MD_CanTrcv_30_Tja1145_0779 */

/**********************************************************************************************************************
  INCLUDES
**********************************************************************************************************************/
#include "CanTrcv_30_Tja1145.h"

/**********************************************************************************************************************
  VERSION DEFINES (Adapted with: ESCAN00086365)
**********************************************************************************************************************/
#if (DRVTRANS_CAN_30_TJA1145_GENTOOL_CFG5_MAJOR_VERSION != 0x03U)
# error "CanTrcv_30_30_Tja1145_Cfg.c : Incompatible DRVTRANS_30_TJA1145_GENTOOL_CFG5_MAJOR_VERSION in generated File!"
#endif

#if (DRVTRANS_CAN_30_TJA1145_GENTOOL_CFG5_MINOR_VERSION != 0x02U)
# error "CanTrcv_30_30_Tja1145_Cfg.c : Incompatible DRVTRANS_30_TJA1145_GENTOOL_CFG5_MINOR_VERSION in generated File!"
#endif

#if (DRVTRANS_CAN_30_TJA1145_GENTOOL_CFG5_PATCH_VERSION != 0x01U)
# error "CanTrcv_30_30_Tja1145_Cfg.c : Incompatible DRVTRANS_30_TJA1145_GENTOOL_CFG5_PATCH_VERSION in generated File!"
#endif


/**********************************************************************************************************************
    ADDITIONAL (HW SPECIFIC)
**********************************************************************************************************************/




/**********************************************************************************************************************
  ComStackLib
**********************************************************************************************************************/
/**********************************************************************************************************************
  LOCAL DATA PROTOTYPES
**********************************************************************************************************************/
/**********************************************************************************************************************
  CONFIGURATION CLASS: PRE_COMPILE
  SECTION: LOCAL DATA TYPES AND STRUCTURES
**********************************************************************************************************************/

/**********************************************************************************************************************
  CONFIGURATION CLASS: LINK
  SECTION: LOCAL DATA TYPES AND STRUCTURES
**********************************************************************************************************************/


/**********************************************************************************************************************
  LOCAL DATA
**********************************************************************************************************************/
/**********************************************************************************************************************
  CONFIGURATION CLASS: PRE_COMPILE
  SECTION: LOCAL DATA
**********************************************************************************************************************/

/**********************************************************************************************************************
  CONFIGURATION CLASS: LINK
  SECTION: LOCAL DATA
**********************************************************************************************************************/


/**********************************************************************************************************************
  GLOBAL DATA
**********************************************************************************************************************/
/**********************************************************************************************************************
  CONFIGURATION CLASS: PRE_COMPILE
  SECTION: GLOBAL DATA
**********************************************************************************************************************/
/**********************************************************************************************************************
  CanTrcv_30_Tja1145_ChannelUsed
**********************************************************************************************************************/
#define CANTRCV_30_TJA1145_START_SEC_CONST_8BIT
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
CONST(CanTrcv_30_Tja1145_ChannelUsedType, CANTRCV_30_TJA1145_CONST) CanTrcv_30_Tja1145_ChannelUsed[1] = {
  /* Index    ChannelUsed  */
  /*     0 */        TRUE
};
#define CANTRCV_30_TJA1145_STOP_SEC_CONST_8BIT
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  CanTrcv_30_Tja1145_PnCfg
**********************************************************************************************************************/
/** 
  \var    CanTrcv_30_Tja1145_PnCfg
  \details
  Element         Description
  Baudrate        Baudrate register
  CanId0          CAN identifier register 0 [Std: don't care, Ext: ID07..ID00]
  CanId1          CAN identifier register 1 [Std: don't care, Ext: ID15..ID08]
  CanId2          CAN identifier register 2 [Std: ID05..ID00: Ext: ID23..ID16]
  CanId3          CAN identifier register 3 [Std: ID10..ID06, Ext: ID28..ID24]
  CanIdMask0      CAN identifier mask register 0 [Std: don't care, Ext: ID07..ID00]
  CanIdMask1      CAN identifier mask register 1 [Std: don't care, Ext: ID15..ID08]
  CanIdMask2      CAN identifier mask register 2 [Std: ID05..ID00: Ext: ID23..ID16]
  CanIdMask3      CAN identifier mask register 3 [Std: ID10..ID06, Ext: ID28..ID24] 
  FrameControl    Frame control register (IDE, PNMD, DLC)
*/ 
#define CANTRCV_30_TJA1145_START_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
CONST(CanTrcv_30_Tja1145_PnCfgType, CANTRCV_30_TJA1145_CONST) CanTrcv_30_Tja1145_PnCfg[1] = {
    /* Index    Baudrate                       CanId0                                CanId1  CanId2  CanId3  CanIdMask0                             CanIdMask1  CanIdMask2  CanIdMask3  FrameControl */
  { /*     0 */       5U  /* (=500 KBit/s) */,  0x00U  /* (CanId = 0x10DBFE00UL) */,  0xFEU,  0xDBU,  0x10U,      0xFFU  /* (MASK=0x000000FFUL) */,      0x00U,      0x00U,      0x00U,        0xC8U }
};
#define CANTRCV_30_TJA1145_STOP_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  CanTrcv_30_Tja1145_PnEnabled
**********************************************************************************************************************/
#define CANTRCV_30_TJA1145_START_SEC_CONST_8BIT
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
CONST(CanTrcv_30_Tja1145_PnEnabledType, CANTRCV_30_TJA1145_CONST) CanTrcv_30_Tja1145_PnEnabled[1] = {
  /* Index    PnEnabled  */
  /*     0 */      TRUE
};
#define CANTRCV_30_TJA1145_STOP_SEC_CONST_8BIT
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  CanTrcv_30_Tja1145_PnPayloadCfg
**********************************************************************************************************************/
/** 
  \var    CanTrcv_30_Tja1145_PnPayloadCfg
  \details
  Element      Description
  DataMask0    Data mask register 0
  DataMask1    Data mask register 1
  DataMask2    Data mask register 2
  DataMask3    Data mask register 3
  DataMask4    Data mask register 4
  DataMask5    Data mask register 5
  DataMask6    Data mask register 6
  DataMask7    Data mask register 7
*/ 
#define CANTRCV_30_TJA1145_START_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
CONST(CanTrcv_30_Tja1145_PnPayloadCfgType, CANTRCV_30_TJA1145_CONST) CanTrcv_30_Tja1145_PnPayloadCfg[1] = {
    /* Index    DataMask0  DataMask1  DataMask2  DataMask3  DataMask4  DataMask5  DataMask6  DataMask7 */
  { /*     0 */     0xFFU,     0xFFU,     0xFFU,     0xFFU,     0xFFU,     0xFFU,     0xFFU,     0xFFU }
};
#define CANTRCV_30_TJA1145_STOP_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  CanTrcv_30_Tja1145_SpiChannelCfg
**********************************************************************************************************************/
/** 
  \var    CanTrcv_30_Tja1145_SpiChannelCfg
  \details
  Element                 Description
  CanTrcvSpiChRWData_0
  CanTrcvSpiChRWData_1
  CanTrcvSpiChRWData_2
  CanTrcvSpiChRWData_3
*/ 
#define CANTRCV_30_TJA1145_START_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
CONST(CanTrcv_30_Tja1145_SpiChannelCfgType, CANTRCV_30_TJA1145_CONST) CanTrcv_30_Tja1145_SpiChannelCfg[1] = {
    /* Index    CanTrcvSpiChRWData_0           CanTrcvSpiChRWData_1               CanTrcvSpiChRWData_2               CanTrcvSpiChRWData_3              */
  { /*     0 */ SpiConf_SpiChannel_SpiChannel, SpiConf_SpiChannel_SpiChannel_001, SpiConf_SpiChannel_SpiChannel_002, SpiConf_SpiChannel_SpiChannel_003 }
};
#define CANTRCV_30_TJA1145_STOP_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  CanTrcv_30_Tja1145_SpiSequenceCfg
**********************************************************************************************************************/
/** 
  \var    CanTrcv_30_Tja1145_SpiSequenceCfg
  \details
  Element                   Description
  CanTrcvSeqRWDataLarge 
  CanTrcvSeqRWDataMedium
  CanTrcvSeqRWDataNormal
*/ 
#define CANTRCV_30_TJA1145_START_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
CONST(CanTrcv_30_Tja1145_SpiSequenceCfgType, CANTRCV_30_TJA1145_CONST) CanTrcv_30_Tja1145_SpiSequenceCfg[1] = {
    /* Index    CanTrcvSeqRWDataLarge                CanTrcvSeqRWDataMedium               CanTrcvSeqRWDataNormal          */
  { /*     0 */ SpiConf_SpiSequence_SpiSequence_002, SpiConf_SpiSequence_SpiSequence_001, SpiConf_SpiSequence_SpiSequence }
};
#define CANTRCV_30_TJA1145_STOP_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  CanTrcv_30_Tja1145_WakeupByBusUsed
**********************************************************************************************************************/
#define CANTRCV_30_TJA1145_START_SEC_CONST_8BIT
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
CONST(CanTrcv_30_Tja1145_WakeupByBusUsedType, CANTRCV_30_TJA1145_CONST) CanTrcv_30_Tja1145_WakeupByBusUsed[1] = {
  /* Index    WakeupByBusUsed  */
  /*     0 */            TRUE
};
#define CANTRCV_30_TJA1145_STOP_SEC_CONST_8BIT
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  CanTrcv_30_Tja1145_WakeupSource
**********************************************************************************************************************/
#define CANTRCV_30_TJA1145_START_SEC_CONST_32BIT
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
CONST(EcuM_WakeupSourceType, CANTRCV_30_TJA1145_CONST) CanTrcv_30_Tja1145_WakeupSource[1] = {
  /* Index    WakeupSource  */
  /*     0 */ 0x00000020UL
};
#define CANTRCV_30_TJA1145_STOP_SEC_CONST_32BIT
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  CanTrcv_30_Tja1145_WuSrcPor
**********************************************************************************************************************/
#define CANTRCV_30_TJA1145_START_SEC_CONST_32BIT
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
CONST(EcuM_WakeupSourceType, CANTRCV_30_TJA1145_CONST) CanTrcv_30_Tja1145_WuSrcPor[1] = {
  /* Index    WuSrcPor      */
  /*     0 */ 0x00000020UL
};
#define CANTRCV_30_TJA1145_STOP_SEC_CONST_32BIT
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  CanTrcv_30_Tja1145_WuSrcSyserr
**********************************************************************************************************************/
#define CANTRCV_30_TJA1145_START_SEC_CONST_32BIT
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
CONST(EcuM_WakeupSourceType, CANTRCV_30_TJA1145_CONST) CanTrcv_30_Tja1145_WuSrcSyserr[1] = {
  /* Index    WuSrcSyserr   */
  /*     0 */ 0x00000020UL
};
#define CANTRCV_30_TJA1145_STOP_SEC_CONST_32BIT
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */


/**********************************************************************************************************************
  CONFIGURATION CLASS: LINK
  SECTION: GLOBAL DATA
**********************************************************************************************************************/





