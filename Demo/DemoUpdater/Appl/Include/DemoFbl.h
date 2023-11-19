/****************************************************************
*
*  Filename:      D:\usr\usage\Delivery\CBD15x\CBD1500635\DXX\external\Demo\DemoUpdater\Appl\\DemoFbl.h
*  Project:       Exported definition of C-Array Flash-Driver
*  File created:  Wed Jun 07 16:49:26 2017

*
****************************************************************/

#ifndef __FBLUPDATE__
#define __FBLUPDATE__
#define FBLUPDATE_GEN_RAND 9296

#define FBLUPDATE_NUMBLOCKS 1

#define FBLUPDATE_DECRYPTDATA(a)   (vuint8)(a)

/* Definition of the structure that stores address and length info */
typedef struct _tFblUpdateBlkInfo
{
   FBL_ADDR_TYPE     blockAddress;
   FBL_MEMSIZE_TYPE  blockLength;
   V_MEMROM0 V_MEMROM1  vuint8 V_MEMROM2 * V_MEMROM1 V_MEMROM2 blockSource;
} tFblUpdateBlkInfo;

V_MEMROM0 extern V_MEMROM1 tFblUpdateBlkInfo V_MEMROM2 FblUpdateBlkInfo[FBLUPDATE_NUMBLOCKS];

#define FBLUPDATE_START_SEC_CONST_EXPORT
#include "MemMap.h"

#define FBLUPDATE_BLOCK0_ADDRESS	0x0
#define FBLUPDATE_BLOCK0_LENGTH	0xB200
#define FBLUPDATE_BLOCK0_CHECKSUM	0x4B1Cu

V_MEMROM0 extern V_MEMROM1 vuint8 V_MEMROM2 FblUpdateBlk0[FBLUPDATE_BLOCK0_LENGTH];

#define FBLUPDATE_STOP_SEC_CONST_EXPORT
#include "MemMap.h"


#endif    /* #ifdef __FBLUPDATE__ */

