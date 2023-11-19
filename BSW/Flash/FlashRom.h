/****************************************************************
*
*  Filename:      D:\usr\usage\Delivery\CBD15x\CBD1500635\DXX\external\BSW\Flash\Build\..\FlashRom.h
*  Project:       Exported definition of C-Array Flash-Driver
*  File created:  Thu Jun 01 15:34:55 2017

*
****************************************************************/

#ifndef __FLASHDRV__
#define __FLASHDRV__
#define FLASHDRV_GEN_RAND 5755

#define FLASHDRV_NUMBLOCKS 1

#define FLASHDRV_DECRYPTVALUE	0xCA
#define FLASHDRV_DECRYPTDATA(a)   (vuint8)((vuint8)(a) ^ (vuint8)FLASHDRV_DECRYPTVALUE)
#define FLASHDRV_BLOCK0_ADDRESS	0xFEDE0000
#define FLASHDRV_BLOCK0_LENGTH	0x1BA8
#define FLASHDRV_BLOCK0_CHECKSUM	0x9717u

V_MEMROM0 extern V_MEMROM1 vuint8 V_MEMROM2 flashDrvBlk0[FLASHDRV_BLOCK0_LENGTH];


#endif    /* #ifdef __FLASHDRV__ */

