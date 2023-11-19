/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief        Flash Driver for Renesas RH850 devices with Rv40 flash
 *
 *  --------------------------------------------------------------------------------------------------------------------
 *  COPYRIGHT
 *  --------------------------------------------------------------------------------------------------------------------
 *  \par Copyright
 *  \verbatim
 *  Copyright (c) 2016 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 */
/**********************************************************************************************************************/

/***********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  --------------------------------------------------------------------------------------------------------------------
 *  Robert Schaeffner             Rr            Vector Informatik GmbH
 *  Christian Baeuerle            CB            Vector Informatik GmbH
 *  Philip Lapczynski             pla           Vector CANtech, Inc.
 *  Quetty Palacios               QPs           Vector Informatik GmbH
 *  Dennis O'Donnell              Dod           Vector CANtech, Inc.
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id          Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  00.90.00   2012-12-18  Rr      ESCAN00063782      Initial version
 *  01.00.00   2013-06-13  CB                         Release
 *  01.01.00   2013-04-01  Rr      ESCAN00072829      Deactivate lock bits handling for flashing
 *  01.02.00   2014-10-23  Rr      ESCAN00076004      Added extended user area support
 *                                                    Added reset vector API to support bootloader updater access
 *                         pla     ESCAN00078538      Added support for IAR compiler
 *  01.02.01   2014-12-08  Rr                         No changes
 *  01.03.00   2015-02-17  Rr      ESCAN00075954      No changes
 *  01.04.00   2015-04-30  QPs     ESCAN00082788      No changes
 *  01.05.00   2016-02-26  Dod     ESCAN00085713      Added support for P1x-C FAL library
 *  01.06.00   2016-03-18  QPs     ESCAN00088975      Added support for secondary driver
 **********************************************************************************************************************/

#ifndef __FBLDRVFLASH_RH850RV40HIS_H__
#define __FBLDRVFLASH_RH850RV40HIS_H__

/***********************************************************************************************************************
 *  VERSION
 **********************************************************************************************************************/

/* ##V_CFG_MANAGEMENT ##CQProject : FblDrvFlash_Rh850Rv40His CQComponent : Implementation */
#define FBLDRVFLASH_RH850RV40HIS_VERSION           0x0106u
#define FBLDRVFLASH_RH850RV40HIS_RELEASE_VERSION   0x00u

/* Flash driver specific version information */
#define FLASH_DRIVER_VERSION_MCUTYPE      0x86u       /**< MCU-Type definition for flash interface */
#define FLASH_DRIVER_VERSION_MASKTYPE     0x00u       /**< Mask type definition                    */
# define FLASH_DRIVER_VERSION_INTERFACE   0x00u       /**< Flash driver interface version          */
#define FLASH_DRIVER_VERSION_MAJOR        (tFlashUint8)(FBLDRVFLASH_RH850RV40HIS_VERSION >> 8u)
#define FLASH_DRIVER_VERSION_MINOR        (tFlashUint8)(FBLDRVFLASH_RH850RV40HIS_VERSION & 0xFFu)
#define FLASH_DRIVER_VERSION_PATCH        (tFlashUint8)(FBLDRVFLASH_RH850RV40HIS_RELEASE_VERSION)

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

/* Flash driver properties */
#define FLASH_SEGMENT_SIZE             0x100u                                 /**< Smallest writeable segment        */
#define FBL_FLASH_DELETED              0xFFu                                  /**< Erase value of flash memory       */
#define FLASH_DRIVER_NOT_RELOCATABLE                                          /**< Flash driver is not relocatable   */

/* Function call table offsets */
#define FLASH_DRIVER_HEADER_OFFSET     0x04u                                  /**< Start of function call table      */
#define FLASH_DRIVER_INIT_OFFSET       (FLASH_DRIVER_HEADER_OFFSET + 0x00u)   /**< Flash driver init function        */
#define FLASH_DRIVER_DEINIT_OFFSET     (FLASH_DRIVER_HEADER_OFFSET + 0x04u)   /**< Flash driver deinit function      */
#define FLASH_DRIVER_ERASE_OFFSET      (FLASH_DRIVER_HEADER_OFFSET + 0x08u)   /**< Flash driver erase function       */
#define FLASH_DRIVER_WRITE_OFFSET      (FLASH_DRIVER_HEADER_OFFSET + 0x0Cu)   /**< Flash driver write function       */
#define FLASH_DRIVER_SETRESVEC_OFFSET  (FLASH_DRIVER_HEADER_OFFSET + 0x10u)   /**< Flash driver set reset vector API */

/* Defines to access version and type information */
#define FLASH_DRIVER_MCUTYPE(flashCode)      (*(tFlashUint8*)((flashCode) + 3u)) /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define FLASH_DRIVER_MASKTYPE(flashCode)     (*(tFlashUint8*)((flashCode) + 2u)) /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define FLASH_DRIVER_INTERFACE(flashCode)    (*(tFlashUint8*)((flashCode) + 0u)) /* PRQA S 3453 */ /* MD_MSR_19.7 */

/* Routine specific error codes */
#define kFlashFctInit                  ((tFlashErrorCode)0x00u)               /**< Error during flash initialization */
#define kFlashFctDeinit                ((tFlashErrorCode)0x20u)               /**< Error during flash de-init        */
#define kFlashFctErase                 ((tFlashErrorCode)0x40u)               /**< Error during flash erase          */
#define kFlashFctWrite                 ((tFlashErrorCode)0x60u)               /**< Error during flash write          */

/* Common error codes */
#define kFlashOk                       ((tFlashErrorCode)0x00u)               /**< Function call successful          */
#define kFlashFailed                   ((tFlashErrorCode)0x01u)               /**< Function call failed              */
#define kFlashVerify                   ((tFlashErrorCode)0x02u)               /**< Verify error                      */
#define kFlashInvalidParam             ((tFlashErrorCode)0x03u)               /**< Invalid parameter                 */
#define kFlashInvalidAddress           ((tFlashErrorCode)0x04u)               /**< Invalid flash address             */
#define kFlashInvalidSize              ((tFlashErrorCode)0x05u)               /**< Invalid flash size                */
#define kFlashCmdBufFull               ((tFlashErrorCode)0x06u)               /**< Buffer filling problem            */
#define kFlashProtect                  ((tFlashErrorCode)0x07u)               /**< Protection error                  */
#define kFlashAcc                      ((tFlashErrorCode)0x08u)               /**< Flash access error                */
#define kFlashInvalidVersion           ((tFlashErrorCode)0x09u)               /**< Invalid version                   */
#define kFlashSequence                 ((tFlashErrorCode)0x0Au)               /**< Sequence error                    */

/* Flash init error codes */
#define kFlashInitFailed               (kFlashFctInit|kFlashFailed)
#define kFlashInitInvalidParam         (kFlashFctInit|kFlashInvalidParam)
#define kFlashInitInvalidAddr          (kFlashFctInit|kFlashInvalidAddress)
#define kFlashInitInvalidSize          (kFlashFctInit|kFlashInvalidSize)
#define kFlashInitInvalidVersion       (kFlashFctInit|kFlashInvalidVersion)

/* Flash deinit error codes */
#define kFlashDeinitFailed             (kFlashFctDeinit|kFlashFailed)
#define kFlashDeinitInvalidParam       (kFlashFctDeinit|kFlashInvalidParam)

/* Flash erase error codes */
#define kFlashEraseFailed              (kFlashFctErase|kFlashFailed)
#define kFlashEraseVerify              (kFlashFctErase|kFlashVerify)
#define kFlashEraseInvalidParam        (kFlashFctErase|kFlashInvalidParam)
#define kFlashEraseInvalidAddr         (kFlashFctErase|kFlashInvalidAddress)
#define kFlashEraseInvalidSize         (kFlashFctErase|kFlashInvalidSize)
#define kFlashEraseSequence            (kFlashFctErase|kFlashSequence)
#define kFlashEraseAcc                 (kFlashFctErase|kFlashAcc)
#define kFlashEraseCmdBufFull          (kFlashFctErase|kFlashCmdBufFull)

/* Flash write error codes */
#define kFlashWriteFailed              (kFlashFctWrite|kFlashFailed)
#define kFlashWriteVerify              (kFlashFctWrite|kFlashVerify)
#define kFlashWriteInvalidParam        (kFlashFctWrite|kFlashInvalidParam)
#define kFlashWriteInvalidAddr         (kFlashFctWrite|kFlashInvalidAddress)
#define kFlashWriteInvalidSize         (kFlashFctWrite|kFlashInvalidSize)
#define kFlashWriteSequence            (kFlashFctWrite|kFlashSequence)
#define kFlashWriteAcc                 (kFlashFctWrite|kFlashAcc)
#define kFlashWriteCmdBufFull          (kFlashFctWrite|kFlashCmdBufFull)

/* Function call macros */
/** Flash write function */
#define FLASH_DRIVER_WRITE(flashCode, flashParam)\
   ((tFlashFct)(*(tFlashUint32 *)&(flashCode)[FLASH_DRIVER_WRITE_OFFSET]))(flashParam) /* PRQA S 3453 */ /* MD_MSR_19.7 */

/** Flash erase function */
#define FLASH_DRIVER_ERASE(flashCode, flashParam)\
   ((tFlashFct)(*(tFlashUint32 *)&(flashCode)[FLASH_DRIVER_ERASE_OFFSET]))(flashParam) /* PRQA S 3453 */ /* MD_MSR_19.7 */

/** Flash init function */
#define FLASH_DRIVER_INIT(flashCode, flashParam)\
   ((tFlashFct)(*(tFlashUint32 *)&(flashCode)[FLASH_DRIVER_INIT_OFFSET]))(flashParam) /* PRQA S 3453 */ /* MD_MSR_19.7 */

/** Flash deinitialization function */
#define FLASH_DRIVER_DEINIT(flashCode,flashParam)\
   ((tFlashFct)(*(tFlashUint32 *)&(flashCode)[FLASH_DRIVER_DEINIT_OFFSET]))(flashParam) /* PRQA S 3453 */ /* MD_MSR_19.7 */

#if defined( FLASH_ENABLE_SET_RESETVECTOR_API )
/** Flash set reset vector function */
#define FLASH_DRIVER_SET_RESET_VECTOR(flashCode,flashParam)\
   ((tFlashFct)(*(tFlashUint32 *)&(flashCode)[FLASH_DRIVER_SETRESVEC_OFFSET]))(flashParam) /* PRQA S 3453 */ /* MD_MSR_19.7 */
#endif /* FLASH_ENABLE_SET_RESETVECTOR_API */

/***********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

typedef unsigned long  tFlashData;                     /**< Flash data object                       */
typedef unsigned long  tFlashAddress;                  /**< Flash target address type               */
typedef unsigned long  tFlashLength;                   /**< Flash length type                       */
typedef unsigned short tFlashErrorCode;                /**< Flash Error Code                        */

/** Typedefs for basic types */
typedef unsigned char  tFlashUint8;                    /**< equivalent to vuint8                    */
typedef unsigned short tFlashUint16;                   /**< equivalent to vuint16                   */
typedef unsigned long  tFlashUint32;                   /**< equivalent to vuint32                   */

typedef struct tagFlashParam
{
   /* Version information */
   tFlashUint8       patchLevel;                       /**< Flash driver patch level                */
   tFlashUint8       minorVersion;                     /**< Flash driver minor version              */
   tFlashUint8       majorVersion;                     /**< Flash driver major version              */
   tFlashUint8       reserved1;                        /**< Reserved for future use                 */
   /* Return value/error code */
   tFlashErrorCode   errorCode;                        /**< Flash driver return value/error code    */
   tFlashUint16      reserved2;                        /**< Reserved for future use                 */
   /* Erase/write input parameters */
   tFlashAddress     address;                          /**< Logical target address                  */
   tFlashLength      length;                           /**< Length in logical sectors or bytes      */
   tFlashData*       data;                             /**< Pointer to data buffer (read only)      */
   /* Pointer to watchdog trigger function */
   tFlashUint8       (* wdTriggerFct)(void);           /**< Pointer to watchdog handling function   */
   /* Erase/write output parameters */
   tFlashData        intendedData;                     /**< Intended data at error address          */
   tFlashData        actualData;                       /**< Actual data at error address            */
   tFlashAddress     errorAddress;                     /**< Error address                           */
   /* flash type specific */
   tFlashUint32      authID[4];                        /**< Authentication ID array                 */
   tFlashUint16      frequencyMHz;                     /**< Flash hardware frequency in MHz         */
} tFlashParam;

/** Flash function pointer */
typedef void (*tFlashFct)(tFlashParam *flashParam);

#endif /* __FBLDRVFLASH_RH850RV40HIS_H___ */

/***********************************************************************************************************************
 *  END OF FILE: FLASHDRV.H
 **********************************************************************************************************************/
