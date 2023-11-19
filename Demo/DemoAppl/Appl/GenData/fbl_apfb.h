/* -----------------------------------------------------------------------------
  Filename:    fbl_apfb.h
  Description: Toolversion: 06.04.03.01.50.06.35.03.00.00
               
               Serial Number: CBD1500635
               Customer Info: Nexteer Automotive Corporation
                              Package: FBL Gm SLP6
                              Micro: Renesas RH850 P1M R7F701363
                              Compiler: Green Hills 2015.1.7
               
               
               Generator Fwk   : GENy 
               Generator Module: GenTool_GenyFblCanBase
               
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

#if !defined(__FBL_APFB_H__)
#define __FBL_APFB_H__

/* -----------------------------------------------------------------------------
    &&&~ 
 ----------------------------------------------------------------------------- */

/* Defines ******************************************************************* */
#define kMioDeviceFlash                      0

/* Typedefs ****************************************************************** */
typedef struct tFlashBlockTag
{
  tFblAddress begin;
  tFblAddress end;
} tFlashBlock;

/* Global data *************************************************************** */
#define FBL_FBT_START_SEC_CONST
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
V_MEMROM0 extern  V_MEMROM1_FAR tFlashBlock V_MEMROM2_FAR FlashBlock[29];
V_MEMROM0 extern  V_MEMROM1_FAR vuint8 V_MEMROM2_FAR kNrOfFlashBlock;
#define FBL_FBT_STOP_SEC_CONST
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


/* begin Fileversion check */
#ifndef SKIP_MAGIC_NUMBER
#ifdef MAGIC_NUMBER
  #if MAGIC_NUMBER != 260564848
      #error "The magic number of the generated file <D:\usr\usage\Delivery\CBD15x\CBD1500635\Dxx\external\Demo\DemoFbl\Appl\GenData\fbl_apfb.h> is different. Please check time and date of generated files!"
  #endif
#else
  #define MAGIC_NUMBER 260564848
#endif  /* MAGIC_NUMBER */
#endif  /* SKIP_MAGIC_NUMBER */

/* end Fileversion check */

#endif /* __FBL_APFB_H__ */
