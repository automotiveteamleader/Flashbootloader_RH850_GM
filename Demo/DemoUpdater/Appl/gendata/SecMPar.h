/* -----------------------------------------------------------------------------
  Filename:    SecMPar.h
  Description: Toolversion: 06.04.03.01.50.06.35.02.00.00
               
               Serial Number: CBD1500635
               Customer Info: Nexteer Automotive Corporation
                              Package: FBL Gm SLP6
                              Micro: Renesas RH850 P1M R7F701363
                              Compiler: Green Hills 2015.1.7
               
               
               Generator Fwk   : GENy 
               Generator Module: SysService_SecModHis
               
               Configuration   : D:\usr\usage\Delivery\CBD15x\CBD1500635\DXX\external\Demo\DemoUpdater\Config\DemoUpdater.gny
               
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

#if !defined(__SECMPAR_H__)
#define __SECMPAR_H__

#include "SecM_inc.h"
#define SECMPAR_START_SEC_DATA
#include "MemMap.h"
V_MEMROM0 extern  V_MEMROM1 vuint8 V_MEMROM2 SecM_RsaMod[256];
V_MEMROM0 extern  V_MEMROM1 vuint8 V_MEMROM2 SecM_RsaExp[4];
#define SECMPAR_STOP_SEC_DATA
#include "MemMap.h"

/* begin Fileversion check */
#ifndef SKIP_MAGIC_NUMBER
#ifdef MAGIC_NUMBER
  #if MAGIC_NUMBER != 255991074
      #error "The magic number of the generated file <D:\usr\usage\Delivery\CBD15x\CBD1500635\DXX\external\Demo\DemoUpdater\Appl\gendata\SecMPar.h> is different. Please check time and date of generated files!"
  #endif
#else
  #define MAGIC_NUMBER 255991074
#endif  /* MAGIC_NUMBER */
#endif  /* SKIP_MAGIC_NUMBER */

/* end Fileversion check */

#endif /* __SECMPAR_H__ */
