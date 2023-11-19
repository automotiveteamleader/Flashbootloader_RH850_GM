/* -----------------------------------------------------------------------------
  Filename:    fbl_cw_cfg.h
  Description: Toolversion: 06.04.03.01.50.06.35.03.00.00
               
               Serial Number: CBD1500635
               Customer Info: Nexteer Automotive Corporation
                              Package: FBL Gm SLP6
                              Micro: Renesas RH850 P1M R7F701363
                              Compiler: Green Hills 2015.1.7
               
               
               Generator Fwk   : GENy 
               Generator Module: FblWrapperCom_Can
               
               Configuration   : D:\usr\usage\Delivery\CBD15x\CBD1500635\Dxx\external\Demo\DemoFbl\Config\DemoFbl.gny
               
               ECU: 
                       TargetSystem: Hw_Rh850Cpu
                       Compiler:     GreenHills
                       Derivates:    P1M
               
               Channel "Channel0":
                       Databasefile: D:\usr\usage\Delivery\CBD15x\CBD1500635\DXX\external\Demo\DemoFbl\Config\demo_sw.dbc
                       Bussystem:    CAN
                       Manufacturer: General Motors
                       Node:         DUT

 ----------------------------------------------------------------------------- */
/* -----------------------------------------------------------------------------
  C O P Y R I G H T
 -------------------------------------------------------------------------------
  Copyright (c) 2001-2015 by Vector Informatik GmbH. All rights reserved.
 
  This software is copyright protected and proprietary to Vector Informatik 
  GmbH.
  
  Vector Informatik GmbH grants to you only those rights as set out in the 
  license conditions.
  
  All other rights remain with Vector Informatik GmbH.
 -------------------------------------------------------------------------------
 ----------------------------------------------------------------------------- */

#if !defined(__FBL_CW_CFG_H__)
#define __FBL_CW_CFG_H__

/* Configuration types */
typedef void (*tFblCwIndicationHandler)( vuint8, volatile vuint8* );
typedef void (*tFblCwConfirmationHandler)( vuint8);
/* Receive identifier structure */
typedef struct tFblCanRxIdListTag
{
  vuintx nrOfNodes; /* Number of nodes, e.g. used by multiple nodes setups */
  vuintx nrOfChannels; /* Number of channels, e.g. used by multiple platforms setups */
  vuintx fblCanIdTableIndex; /* Index into FblCanIdTable */
  vuintx rxObject; /* Message object parameter in FblHandleRxMsg */
  V_MEMROM1 vuint32 V_MEMROM2 V_MEMROM3 * idList; /* Flattened ID list */
  tFblCwIndicationHandler fblCwIndicationHandler; /* Message handler called by FblHandleRxMsg */
} tFblCanRxIdList;
/* Transmit identifier structure */
typedef struct tFblCanTxIdListTag
{
  vuintx nrOfNodes; /* Number of nodes, e.g. used by multiple nodes setups */
  vuintx nrOfChannels; /* Number of channels, e.g. used by multiple platforms setups */
  vuintx txObject; /* Message object parameter in FblHandleRxMsg */
  V_MEMROM1 vuint32 V_MEMROM2 V_MEMROM3 * idList; /* Flattened ID list */
  tFblCwConfirmationHandler fblCwConfirmationHandler; /* Confirmation function called after message has been transmitted */
} tFblCanTxIdList;
/* Basic CAN defines */
/* Receive identifiers */
#define FBL_CAN_NUMBER_OF_RX_ID              1u
#define FBL_CAN_NUMBER_OF_RANGES             2u
#define FBL_NUMBER_OF_RX_ID                  (FBL_CAN_NUMBER_OF_RX_ID + FBL_CAN_NUMBER_OF_RANGES)
/* Special configuration for NormalFixed Addressing */
#define FBL_ENABLE_NORMAL_FIXED_ADDRESSING
#define FBL_ENABLE_CAN_RX_RANGE
#define FBL_CAN_MASK_0                       MK_EXTMASK(0x000000FFu)
#define FBL_CAN_MASK_1                       MK_EXTMASK(0x000000FFu)
/* Response identifiers */
#define FBL_NUMBER_OF_TX_ID                  1u
#define kFblCwDiagTxObject                   0u

#define FBL_CW_CFG_START_SEC_CONST
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* Configuration */
V_MEMROM0 extern  V_MEMROM1 vuint32 V_MEMROM2 diagPhysRxIdList[1];
V_MEMROM0 extern  V_MEMROM1 vuint32 V_MEMROM2 diagPhysRxIdList_1[1];
V_MEMROM0 extern  V_MEMROM1 vuint32 V_MEMROM2 diagFuncRxIdList[1];
V_MEMROM0 extern  V_MEMROM1 tFblCanRxIdList V_MEMROM2 fblCanIdRxConfiguration[FBL_NUMBER_OF_RX_ID];
V_MEMROM0 extern  V_MEMROM1 vuint32 V_MEMROM2 diagPhysTxIdList[1];
V_MEMROM0 extern  V_MEMROM1 tFblCanTxIdList V_MEMROM2 fblCanIdTxConfiguration[FBL_NUMBER_OF_TX_ID];
#define FBL_CW_NUMBER_OF_CHANNELS            1
#define FBL_CW_DISABLE_MULTIPLE_CHANNELS
#define FBL_CW_NUMBER_OF_NODES               1
#define FBL_CW_DISABLE_MULTIPLE_NODES

#define FBL_CW_CFG_STOP_SEC_CONST
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* User Config File ********************************************************** */
/* User Section ************************************************************** */
/* FblWrapperCom_Can feature selection */
#define FBL_CW_ENABLE_PHYSICAL_RANGE_HANDLER
#define FBL_CW_ENABLE_FUNCTIONAL_RANGE_HANDLER
#define FBL_CW_ENABLE_PHYSICAL_REQUEST_HANDLER
#if defined( FBL_ENABLE_XCP )
# define FBL_CW_ENABLE_XCP_REQUEST_HANDLER
#endif
#define FBL_ENABLE_BUSTYPE_CAN
/* *************************************************************************** */

/* begin Fileversion check */
#ifndef SKIP_MAGIC_NUMBER
#ifdef MAGIC_NUMBER
  #if MAGIC_NUMBER != 260732447
      #error "The magic number of the generated file <D:\usr\usage\Delivery\CBD15x\CBD1500635\Dxx\external\Demo\DemoFbl\Appl\GenData\fbl_cw_cfg.h> is different. Please check time and date of generated files!"
  #endif
#else
  #define MAGIC_NUMBER 260732447
#endif  /* MAGIC_NUMBER */
#endif  /* SKIP_MAGIC_NUMBER */

/* end Fileversion check */

#endif /* __FBL_CW_CFG_H__ */
