/* -----------------------------------------------------------------------------
  Filename:    fbl_mtab.h
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

#if !defined(__FBL_MTAB_H__)
#define __FBL_MTAB_H__

/* -----------------------------------------------------------------------------
    &&&~ 
 ----------------------------------------------------------------------------- */

/* Defines ******************************************************************* */

#define FBL_MTAB_APPLICATION_AND_CALIBRATION_AREA1_BLOCK_NUMBER 0
#define FBL_MTAB_APPLICATION_AND_CALIBRATION_AREA1_BLOCK_INDEX 0x01u

/* Macro to determine if the block table is valid */
#define FBL_MTAB_MAGIC_FLAG                  0xB5A4C3D2u
#define IsLogicalBlockTableValid()           (FblLogicalBlockTable.magicFlag == FBL_MTAB_MAGIC_FLAG)

/* Compatibility defines */
#define verifyRAM                            verifyInput
#define verifyROM                            verifyOutput

/* Typedefs ****************************************************************** */

/* Entry type of logical block table */
typedef struct tBlockDescriptorTag
{
  vuint8 blockNr; /* Number of logical block */
  vuint32 blockIndex;
  tFblAddress blockStartAddress; /* Start address of current block */
  tFblLength blockLength; /* Block length in bytes */
  tFblAddress headerAddress;
  tFblAddress presencePatternAddress;
  tExportFct verifyInput;
  tExportFct verifyOutput;
} tBlockDescriptor;

/* The logical block table describes the memory layout of logical blocks */
typedef struct tLogicalBlockTableTag
{
  vuint32 magicFlag; /* Value of FBL_MTAB_MAGIC_FLAG - Indicates the existence of the table */
  vuint8 noOfBlocks; /* Number of configured logical blocks */
  tBlockDescriptor logicalBlock[FBL_MTAB_NO_OF_BLOCKS];
} tLogicalBlockTable;

/* Global data *************************************************************** */
#define FBL_LBT_START_SEC_CONST
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
V_MEMROM0 extern  V_MEMROM1 tLogicalBlockTable V_MEMROM2 FblLogicalBlockTable;
#define FBL_LBT_STOP_SEC_CONST
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */


/* begin Fileversion check */
#ifndef SKIP_MAGIC_NUMBER
#ifdef MAGIC_NUMBER
  #if MAGIC_NUMBER != 255991074
      #error "The magic number of the generated file <D:\usr\usage\Delivery\CBD15x\CBD1500635\DXX\external\Demo\DemoUpdater\Appl\gendata\fbl_mtab.h> is different. Please check time and date of generated files!"
  #endif
#else
  #define MAGIC_NUMBER 255991074
#endif  /* MAGIC_NUMBER */
#endif  /* SKIP_MAGIC_NUMBER */

/* end Fileversion check */

#endif /* __FBL_MTAB_H__ */
