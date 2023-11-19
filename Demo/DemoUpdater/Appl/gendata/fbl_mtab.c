/* -----------------------------------------------------------------------------
  Filename:    fbl_mtab.c
  Description: Toolversion: 06.04.03.01.50.06.35.02.00.00
               
               Serial Number: CBD1500635
               Customer Info: Nexteer Automotive Corporation
                              Package: FBL Gm SLP6
                              Micro: Renesas RH850 P1M R7F701363
                              Compiler: Green Hills 2015.1.7
               
               
               Generator Fwk   : GENy 
               Generator Module: GenTool_GenyFblCanBase
               
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

/* -----------------------------------------------------------------------------
    &&&~ 
 ----------------------------------------------------------------------------- */

#define FBL_LBT_SOURCE

/* Includes ****************************************************************** */
#include "fbl_inc.h"

/* Global data *************************************************************** */
#define FBL_LBT_START_SEC_CONST
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
V_MEMROM0 V_MEMROM1 tLogicalBlockTable V_MEMROM2 FblLogicalBlockTable = 
{
  FBL_MTAB_MAGIC_FLAG, 
  FBL_MTAB_NO_OF_BLOCKS, 
  
  {
    
    {
      0, 
      0x01u /* Block Index */, 
      0x020000u /* Block Start Address */, 
      0x1E0000u /* Block Length (End Address = 0x1FFFFF) */, 
      0x020000u /* Header Address */, 
      0x037E00u /* Presence Pattern Address */, 
      (tExportFct)FblHdrPipelinedVerifyIntegrity /* Input Verification */, 
      (tExportFct)FblHdrVerifyIntegrity /* Output Verification */
    } /* Name 'Application and Calibration Area1' */
  }
};
#define FBL_LBT_STOP_SEC_CONST
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


/* begin Fileversion check */
#ifndef SKIP_MAGIC_NUMBER
#ifdef MAGIC_NUMBER
  #if MAGIC_NUMBER != 255991074
      #error "The magic number of the generated file <D:\usr\usage\Delivery\CBD15x\CBD1500635\DXX\external\Demo\DemoUpdater\Appl\gendata\fbl_mtab.c> is different. Please check time and date of generated files!"
  #endif
#else
  #error "The magic number is not defined in the generated file <D:\usr\usage\Delivery\CBD15x\CBD1500635\DXX\external\Demo\DemoUpdater\Appl\gendata\fbl_mtab.c> "

#endif  /* MAGIC_NUMBER */
#endif  /* SKIP_MAGIC_NUMBER */

/* end Fileversion check */

