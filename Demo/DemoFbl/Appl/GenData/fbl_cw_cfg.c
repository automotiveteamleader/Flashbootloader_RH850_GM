/* -----------------------------------------------------------------------------
  Filename:    fbl_cw_cfg.c
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

#define FBL_CW_CFG_SOURCE

#include "fbl_inc.h"

#define FBL_CW_CFG_START_SEC_CONST
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

V_MEMROM0 V_MEMROM1 vuint32 V_MEMROM2 diagPhysRxIdList[1] = { MK_EXTID(0x14da4500u) };
V_MEMROM0 V_MEMROM1 vuint32 V_MEMROM2 diagPhysRxIdList_1[1] = { MK_STDID(0x645u) };
V_MEMROM0 V_MEMROM1 vuint32 V_MEMROM2 diagFuncRxIdList[1] = { MK_EXTID(0x10dbfe00u) };
/* RX ID configuration */
V_MEMROM0 V_MEMROM1 tFblCanRxIdList V_MEMROM2 fblCanIdRxConfiguration[FBL_NUMBER_OF_RX_ID] = 
{
  
  {
    1u /* Nodes / nrOfNodes */, 
    1u /* Channels / nrOfChannels */, 
    1u /* MessageObject / fblCanIdTableIndex */, 
    FblRxRange0Hdl /* ReceiveHandle / rxObject */, 
    diagPhysRxIdList /* GenName / idList */, 
    FblCwProcessPhysicalRangeRequest /* IndicationConfirmationFunction / fblCwIndicationHandler */
  }, 
  
  {
    1u /* Nodes / nrOfNodes */, 
    1u /* Channels / nrOfChannels */, 
    0u /* MessageObject / fblCanIdTableIndex */, 
    FblRxCanMsg0Hdl /* ReceiveHandle / rxObject */, 
    diagPhysRxIdList_1 /* GenName / idList */, 
    FblCwProcessPhysicalRequest /* IndicationConfirmationFunction / fblCwIndicationHandler */
  }, 
  
  {
    1u /* Nodes / nrOfNodes */, 
    1u /* Channels / nrOfChannels */, 
    2u /* MessageObject / fblCanIdTableIndex */, 
    FblRxRange1Hdl /* ReceiveHandle / rxObject */, 
    diagFuncRxIdList /* GenName / idList */, 
    FblCwProcessFunctionalRangeRequest /* IndicationConfirmationFunction / fblCwIndicationHandler */
  }
};
V_MEMROM0 V_MEMROM1 vuint32 V_MEMROM2 diagPhysTxIdList[1] = { MK_EXTID(0x14da0045u) };
/* TX ID configuration */
V_MEMROM0 V_MEMROM1 tFblCanTxIdList V_MEMROM2 fblCanIdTxConfiguration[FBL_NUMBER_OF_TX_ID] = 
{
  
  {
    1u /* Nodes / nrOfNodes */, 
    1u /* Channels / nrOfChannels */, 
    0u /* MessageObject / txObject */, 
    diagPhysTxIdList /* GenName / idList */, 
    FblTpConfirmation /* IndicationConfirmationFunction / fblCwConfirmationHandler */
  }
};

#define FBL_CW_CFG_STOP_SEC_CONST
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* begin Fileversion check */
#ifndef SKIP_MAGIC_NUMBER
#ifdef MAGIC_NUMBER
  #if MAGIC_NUMBER != 260732447
      #error "The magic number of the generated file <D:\usr\usage\Delivery\CBD15x\CBD1500635\Dxx\external\Demo\DemoFbl\Appl\GenData\fbl_cw_cfg.c> is different. Please check time and date of generated files!"
  #endif
#else
  #error "The magic number is not defined in the generated file <D:\usr\usage\Delivery\CBD15x\CBD1500635\Dxx\external\Demo\DemoFbl\Appl\GenData\fbl_cw_cfg.c> "

#endif  /* MAGIC_NUMBER */
#endif  /* SKIP_MAGIC_NUMBER */

/* end Fileversion check */

