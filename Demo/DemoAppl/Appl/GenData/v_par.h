/* -----------------------------------------------------------------------------
  Filename:    v_par.h
  Description: Toolversion: 06.04.03.01.50.06.35.03.00.00
               
               Serial Number: CBD1500635
               Customer Info: Nexteer Automotive Corporation
                              Package: FBL Gm SLP6
                              Micro: Renesas RH850 P1M R7F701363
                              Compiler: Green Hills 2015.1.7
               
               
               Generator Fwk   : GENy 
               Generator Module: GenTool_GenyVcfgNameDecorator
               
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

#if !defined(__V_PAR_H__)
#define __V_PAR_H__

/* -----------------------------------------------------------------------------
    &&&~ BaseEnv_PHF_Includes
 ----------------------------------------------------------------------------- */

#include "v_cfg.h"
#include "v_def.h"


/* -----------------------------------------------------------------------------
    &&&~ GENy Version Information
 ----------------------------------------------------------------------------- */

#define VGEN_DELIVERY_VERSION_BYTE_0         0x06
#define VGEN_DELIVERY_VERSION_BYTE_1         0x04
#define VGEN_DELIVERY_VERSION_BYTE_2         0x03
#define VGEN_DELIVERY_VERSION_BYTE_3         0x01
#define VGEN_DELIVERY_VERSION_BYTE_4         0x50
#define VGEN_DELIVERY_VERSION_BYTE_5         0x06
#define VGEN_DELIVERY_VERSION_BYTE_6         0x35
#define VGEN_DELIVERY_VERSION_BYTE_7         0x03
#define VGEN_DELIVERY_VERSION_BYTE_8         0x00
#define VGEN_DELIVERY_VERSION_BYTE_9         0x00
#define kGENyVersionNumberOfBytes            10
/* ROM CATEGORY 4 START */
V_MEMROM0 extern  V_MEMROM1 vuint8 V_MEMROM2 kGENyVersion[kGENyVersionNumberOfBytes];
/* ROM CATEGORY 4 END */




/* begin Fileversion check */
#ifndef SKIP_MAGIC_NUMBER
#ifdef MAGIC_NUMBER
  #if MAGIC_NUMBER != 260564848
      #error "The magic number of the generated file <D:\usr\usage\Delivery\CBD15x\CBD1500635\Dxx\external\Demo\DemoFbl\Appl\GenData\v_par.h> is different. Please check time and date of generated files!"
  #endif
#else
  #define MAGIC_NUMBER 260564848
#endif  /* MAGIC_NUMBER */
#endif  /* SKIP_MAGIC_NUMBER */

/* end Fileversion check */

#endif /* __V_PAR_H__ */
