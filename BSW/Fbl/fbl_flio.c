/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief        Flash Driver Wrapper for Renesas RH850
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
 *  Achim Strobelt                Ach           Vector Informatik GmbH
 *  Christopher Elkins            CEl           Vector CANtech, Inc.
 *  Dennis O'Donnell              Dod           Vector CANtech, Inc.
 *  Torben Stoessel               TnS           Vector Informatik GmbH
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id          Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  00.09.00   2012-12-18  Rr      ESCAN00063781      Initial version
 *  01.00.00   2013-06-13  CB                         Release
 *  01.01.00   2014-02-27  Ach     ESCAN00073965      Support P1x derivatives
 *  01.02.00   2014-07-01  CEl     ESCAN00074163      Support ECC safe read function
 *  01.03.00   2014-10-23  Rr      ESCAN00078958      Provide API for changing reset vector via Selflib
 *                                 ESCAN00078773      ECC depends on FLASH_ENABLE_MACHINE_CHECK_ECC_DETECTION
 *                                                    Harmonized ECC handling
 *  01.04.00   2016-02-26  Dod     ESCAN00085770      Added support for P1x-C derivative
 *                         Dod     ESCAN00086372      Setting the reset vector always fails (FlashDriver_SetResetVector)
 *  01.05.00   2016-03-18  QPs     ESCAN00088975      Added support for secondary driver
 *  01.06.00   2016-05-09  TnS     ESCAN00087121      2 bit ECC SYSERR might occur even if
 *                                                      FLASH_ENABLE_MACHINE_CHECK_ECC_DETECTION is used
 *  01.07.00   2016-05-23  CB                         Adapted filter settings for E1x, no code changes
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

#include "fbl_inc.h"
#include "flashrom.h"   /* Support for flash driver in ROM memory */

/***********************************************************************************************************************
 *  VERSION
 **********************************************************************************************************************/

#if ( FBLWRAPPERFLASH_RH850RV40HIS_VERSION != 0x0107u ) || \
    ( FBLWRAPPERFLASH_RH850RV40HIS_RELEASE_VERSION != 0x00u )
# error "Error in fbl_flio.c: Source and header file are inconsistent!"
#endif

#if ( FBLWRAPPERFLASH_RH850RV40HIS_VERSION != _FBLWRAPPERFLASH_HW_VERSION ) || \
    ( FBLWRAPPERFLASH_RH850RV40HIS_RELEASE_VERSION != _FBLWRAPPERFLASH_HW_RELEASE_VERSION )
# error "Error in fbl_flio.c: Source and v_ver.h are inconsistent!"
#endif

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/
# ifndef FLASH_AUTH_ID_0
#  define FLASH_AUTH_ID_0 0xFFFFFFFFul
# endif
# ifndef FLASH_AUTH_ID_1
#  define FLASH_AUTH_ID_1 0xFFFFFFFFul
# endif
# ifndef FLASH_AUTH_ID_2
#  define FLASH_AUTH_ID_2 0xFFFFFFFFul
# endif
# ifndef FLASH_AUTH_ID_3
#  define FLASH_AUTH_ID_3 0xFFFFFFFFul
# endif

/***********************************************************************************************************************
 *  TYPEDEFS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

# define FLASHCODE_START_SEC_VAR
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
V_MEMRAM0 V_MEMRAM1 vuint8 V_MEMRAM2 flashCode[FLASH_SIZE];
# define FLASHCODE_STOP_SEC_VAR
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/***********************************************************************************************************************
 *  LOCAL DATA
 **********************************************************************************************************************/

static tFlashParam flashParam; /**< Data structure used as interface to flash driver. */

/***********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  FlashDriver_InitSync
 **********************************************************************************************************************/
/*! \brief       Initializes the flash driver
 *  \details     This function copies the flash driver to RAM if it is stored in a ROM image and calls the
 *               initialization routine in RAM.
 *  \pre         Flash driver has to be copied to RAM if downloadable flash driver is used.
 *  \param[in]   *address Unused parameter to implement HIS interface.
 *  \return      Reports if initialization was successful or not.
 **********************************************************************************************************************/
IO_ErrorType FlashDriver_InitSync ( void * address ) /* PRQA S 3673 */ /* MD_FblWrapperFlash_3673 */
{
   IO_U16 index;
# if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   (void)address; /* PRQA S 3112 */ /* MD_FblWrapperFlash_3112 */
# endif /* V_ENABLE_USE_DUMMY_STATEMENT */

   /* Copy flashdriver into RAM buffer */
# if defined( FBL_ENABLE_SYSTEM_CHECK )
   /* Check if address and length is correct */
#  if defined( FLASH_DRIVER_RELOCATABLE )
   if (FLASH_SIZE < FLASHDRV_BLOCK0_LENGTH)
#  else /* FLASH_DRIVER_RELOCATABLE */
   if ( (flashCode != (IO_U8 *)FLASHDRV_BLOCK0_ADDRESS) ||
         (FLASH_SIZE < FLASHDRV_BLOCK0_LENGTH))
#  endif /* FLASH_DRIVER_RELOCATABLE */
   {
      return kFlashFailed;
   }
# endif  /* FBL_ENABLE_SYSTEM_CHECK */
   for (index = 0; index < FLASHDRV_BLOCK0_LENGTH; index++)
   {
      if ((index & 0x1F) == 0)
      {
         (void)FblLookForWatchdog();
      }
      flashCode[index] =  FLASHDRV_DECRYPTDATA(flashDrvBlk0[index]);
   }

   /* Verify if flashcode signature is correct */
   if ( (FLASH_DRIVER_MCUTYPE(flashCode)   != FLASH_DRIVER_VERSION_MCUTYPE)  || /* PRQA S 0488 */ /* MD_FblWrapperFlash_17.4 */
        (FLASH_DRIVER_MASKTYPE(flashCode)  != FLASH_DRIVER_VERSION_MASKTYPE) || /* PRQA S 0488 */ /* MD_FblWrapperFlash_17.4 */
        (FLASH_DRIVER_INTERFACE(flashCode) != FLASH_DRIVER_VERSION_INTERFACE)   /* PRQA S 0488 */ /* MD_FblWrapperFlash_17.4 */
      )
   {
      /* Flash driver signature mismatch, wrong flash driver */
      return kFlashInitInvalidVersion;
   }
   /* Initialize flash parameter structure */
   flashParam.wdTriggerFct = (IO_U8(*)(void)) FblLookForWatchdog;
   flashParam.reserved1    = (IO_U8)   0x00u;
   flashParam.reserved2    = (IO_U8)   0x00u;
   flashParam.errorCode    = kFlashOk;
   /* Version defines are preset with version of compile time flash driver. */
   flashParam.majorVersion = (IO_U8)   FLASH_DRIVER_VERSION_MAJOR;
   flashParam.minorVersion = (IO_U8)   FLASH_DRIVER_VERSION_MINOR;
   flashParam.patchLevel   = (IO_U8)   FLASH_DRIVER_VERSION_PATCH;

   flashParam.authID[0]    = FLASH_AUTH_ID_0;
   flashParam.authID[1]    = FLASH_AUTH_ID_1;
   flashParam.authID[2]    = FLASH_AUTH_ID_2;
   flashParam.authID[3]    = FLASH_AUTH_ID_3;

   flashParam.frequencyMHz = FBL_SYSTEM_FREQUENCY;

   /* Call init routine of flash driver */
   FLASH_DRIVER_INIT(flashCode, &flashParam); /* PRQA S 3305, 0305, 0310 */ /* MD_FblWrapperFlash_3305, MD_FblWrapperFlash_0305_FlashHisAPI, MD_FblWrapperFlash_0310_FlashHisAPI */

   return (flashParam.errorCode);
} /* PRQA S 2006 */ /* MD_CBD_14.7 */

/***********************************************************************************************************************
 *  FlashDriver_DeinitSync
 **********************************************************************************************************************/
/*! \brief       Deinitializes the flash driver
 *  \details     This function calls the deinitialization routine in RAM and removes the flash driver
 *               from the RAM buffer.
 *  \pre         Flash driver is initialized.
 *  \param[in]   *address Unused parameter to implement HIS interface.
 *  \return      Reports if deinitialization was successful or not.
 **********************************************************************************************************************/
IO_ErrorType FlashDriver_DeinitSync ( void * address ) /* PRQA S 3673 */ /* MD_FblWrapperFlash_3673 */
{
   vuint32 i;
# if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   (void)address; /* PRQA S 3112 */ /* MD_FblWrapperFlash_3112 */
# endif /* V_ENABLE_USE_DUMMY_STATEMENT */

   /* Call deinit routine of flash driver */
   FLASH_DRIVER_DEINIT(flashCode, &flashParam); /* PRQA S 3305, 0305, 0310 */ /* MD_FblWrapperFlash_3305, MD_FblWrapperFlash_0305_FlashHisAPI, MD_FblWrapperFlash_0310_FlashHisAPI */

   /* Remove flash code from RAM */
   for (i = 0; i < FLASH_SIZE; i++)
   {
      flashCode[i] = 0x00u;

      /* Call FblLookForWatchdog() every 256 Bytes */
      if ((i & 0xFFul) == 0x00ul)
      {
         (void)FblLookForWatchdog();
      }
   }

   return flashParam.errorCode;
}

/***********************************************************************************************************************
 *  FlashDriver_RWriteSync
 **********************************************************************************************************************/
/*! \brief       Writes data to flash memory.
 *  \details     This function programs flash memory by calling the flash driver routine stored in RAM.
 *  \pre         Flash driver is initialized, memory to be programmed is erased.
 *  \param[in]   writeBuffer Pointer to input data buffer.
 *  \param[in]   writeLength Data length.
 *  \param[in]   writeAddress Write target address ( in flash memory ).
 *  \return      Reports write success or error code.
 **********************************************************************************************************************/
IO_ErrorType FlashDriver_RWriteSync ( IO_MemPtrType writeBuffer, IO_SizeType writeLength, IO_PositionType writeAddress )
{
   IO_ErrorType retVal;

   /* Check alignment of write address and write length. */
   if ((writeAddress & (FLASH_SEGMENT_SIZE - 1)) != 0x00u)
   {
      retVal = kFlashWriteInvalidAddr;
   }
   else if ((writeLength & (FLASH_SEGMENT_SIZE - 1)) != 0x00u)
   {
      retVal = kFlashWriteInvalidSize;
   }
   else
   {
      retVal = kFlashOk;
   }

   if ( (retVal == kFlashOk) && (writeLength > 0) )
   {
      /* Initialize parameters */
      flashParam.address = (tFlashAddress)writeAddress;
      flashParam.length = (tFlashLength)writeLength;
      flashParam.data = (tFlashData*)writeBuffer; /* PRQA S 3305, 0310 */ /* MD_FblWrapperFlash_writeBuffer_3305, MD_FblWrapperFlash_0310_FlashHisAPI */

      /* Enable flash write access */
      FBL_FHVE15 = 0x01ul;
      FBL_FHVE3 = 0x01ul;

      /* Call flash driver in RAM */
      FLASH_DRIVER_WRITE(flashCode, &flashParam); /* PRQA S 3305, 0305, 0310 */ /* MD_FblWrapperFlash_3305, MD_FblWrapperFlash_0305_FlashHisAPI, MD_FblWrapperFlash_0310_FlashHisAPI */
      retVal = flashParam.errorCode;

      /* Disable flash write access */
      FBL_FHVE15 = 0x00ul;
      FBL_FHVE3 = 0x00ul;
   }

   return retVal;
}

/***********************************************************************************************************************
 *  FlashDriver_REraseSync
 **********************************************************************************************************************/
/*! \brief       Erases area in flash memory.
 *  \details     This function erases one or more logical sectors in flash memory.
 *  \pre         Flash driver is initialized.
 *  \param[in]   eraseLength Length of memory to be erased. The length has to be aligned to the length
 *               of one or more flash sectors.
 *  \param[in]   eraseAddress Start address of erase area. The start address has to be the start
 *               address of a logical flash sector.
 *  \return      Reports erase success or error code.
 **********************************************************************************************************************/
IO_ErrorType FlashDriver_REraseSync ( IO_SizeType eraseLength, IO_PositionType eraseAddress )
{
   IO_ErrorType retValue;

   flashParam.address = eraseAddress;
   flashParam.length  = eraseLength;

   /* Enable flash write access */
   FBL_FHVE15 = 0x01ul;
   FBL_FHVE3  = 0x01ul;

   /* Call flash driver in RAM */
   FLASH_DRIVER_ERASE(flashCode, &flashParam); /* PRQA S 3305, 0305, 0310 */ /* MD_FblWrapperFlash_3305, MD_FblWrapperFlash_0305_FlashHisAPI, MD_FblWrapperFlash_0310_FlashHisAPI */
   /* Store return value from flash driver */
   retValue   = flashParam.errorCode;

   /* Disable flash write access */
   FBL_FHVE15 = 0x00ul;
   FBL_FHVE3  = 0x00ul;

   return retValue;
}

#if defined ( FBL_ENABLE_USR_RREAD_SYNC )
#else
/***********************************************************************************************************************
 *  FlashDriver_RReadSync
 **********************************************************************************************************************/
/*! \brief       Reads data from flash memory.
 *  \details     This function reads data from flash memory. It can be configured to report ECC errors to the
 *               calling function.  Error correction is enabled.
 *  \param[out]  readBuffer Pointer to read target buffer. The buffer size must be at least readLength Bytes.
 *  \param[in]   readLength Size of data to be read.
 *  \param[in]   readAddress Start address of data to be read.
 *  \return      Reports read success or error code.
 **********************************************************************************************************************/
IO_ErrorType FlashDriver_RReadSync ( IO_MemPtrType readBuffer, IO_SizeType readLength, IO_PositionType readAddress )
{
   /* PRQA S 0303 TAG_0303_HWaccess */ /* MD_FblWrapperFlash_0303_HWaccess */
   /* Initialize return value */
   IO_ErrorType retValue      = IO_E_OK;

# if defined( FLASH_ENABLE_MACHINE_CHECK_ECC_DETECTION )
   const vuint8* tempPtr      = readBuffer;

   /* Save the current SEG_CONT register */
   vuint16 tempSEGCONT        = FBL_SEG_CONT;
   /* Save the current ECC control register configurations */
   tFblEccCtl tempCFECCCTL    = FBL_CODE_FLASH_ECC_CTL;
   tFblEccErrInt tempCFERRINT = FBL_CODE_FLASH_ECC_ERRINT;

   /* Disable SYSERR generation for ECC C-Flash errors */
   FBL_SEG_CONT               = tempSEGCONT & FblInvert16Bit(kSegContRome);
   /* Enable ECC detection and ECC correction */
   FBL_CODE_FLASH_ECC_CTL     = kEccProtWrite;

   /* Disable ECC notification signal generation */
   FBL_CODE_FLASH_ECC_ERRINT  = kEccNotificationDisable;

   /* Clear Error status before read */
   FBL_CODE_FLASH_ECC_ERROR_CLEAR();

   /* Read from code flash */
   (void)MEMCPY(readBuffer, readAddress, readLength); /* PRQA S 0306 */ /* MD_FblWrapperFlash_0306_Memcpy */

   /* Check for uncorrectable two bit error occurred -> Read again to check for erased memory */
   if ( (FBL_CODE_FLASH_ECC_FSTERSTR & kEccUncorrectableError) == kEccUncorrectableError )
   {
      /* For second read, disable ECC correction */
      FBL_CODE_FLASH_ECC_CTL  = ( kEccProtWrite | kEccCorrectionDisable );

      /* Read from code flash again */
      (void)MEMCPY(readBuffer, readAddress, readLength); /* PRQA S 0306 */ /* MD_FblWrapperFlash_0306_Memcpy */

      /* Check for erased value, if all erased value driver assumes data to be erased */
      while ((retValue == IO_E_OK) && (readLength > 0u))
      {
         if (*tempPtr != FBL_FLASH_DELETED)
         {
            retValue = IO_E_NOT_OK;
         }
         tempPtr++; /* PRQA S 0489 */ /* MD_FblWrapperFlash_17.4 */
         readLength--;
      }

      /* Check if all value were flash erased value */
      if (retValue == IO_E_OK)
      {
         retValue = IO_E_ERASED;
      }

      /* Clear ECC Error status registers */
      FBL_CODE_FLASH_ECC_ERROR_CLEAR();
   }

   /* Restore previous ECC setting */
   FBL_SEG_FLAG &= FblInvert16Bit(kSegFlagRomf);   /* Clear pending Code flash error flag */
   FBL_SEG_CONT = tempSEGCONT;
   FBL_CODE_FLASH_ECC_CTL = (kEccProtWrite | tempCFECCCTL);
   FBL_CODE_FLASH_ECC_ERRINT = tempCFERRINT;
# else
   /* Read flash with plain memcpy */
   (void)MEMCPY(readBuffer, readAddress, readLength); /* PRQA S 0306 */ /* MD_FblWrapperFlash_0306_Memcpy */
# endif /* FLASH_ENABLE_MACHINE_CHECK_ECC_DETECTION */
   return retValue;
   /* PRQA L: TAG_0303_HWaccess */
}
#endif /* FBL_ENABLE_USR_RREAD_SYNC */

/***********************************************************************************************************************
 *  FlashDriver_GetVersionOfDriver
 **********************************************************************************************************************/
/*! \brief       Reports the flash driver version.
 *  \details     This function reads the flash driver version and reports it to the application. Depending on the
 *               initialization state of the driver, the following data is reported:
 *               - Flash driver version which was used to build the Bootloader if no driver has been downloaded yet.
 *               - Actual flash driver version if the flash driver has been downloaded.
 *  \return      BCD coded version of flash driver.
 **********************************************************************************************************************/
IO_U32 FlashDriver_GetVersionOfDriver( void )
{
   /* Check if flash code is present */
   if ( (FLASH_DRIVER_MCUTYPE(flashCode)   != FLASH_DRIVER_VERSION_MCUTYPE)  || /* PRQA S 0488 */ /* MD_FblWrapperFlash_17.4 */
        (FLASH_DRIVER_MASKTYPE(flashCode)  != FLASH_DRIVER_VERSION_MASKTYPE) || /* PRQA S 0488 */ /* MD_FblWrapperFlash_17.4 */
        (FLASH_DRIVER_INTERFACE(flashCode) != FLASH_DRIVER_VERSION_INTERFACE)   /* PRQA S 0488 */ /* MD_FblWrapperFlash_17.4 */
      )
   {
      /* Flash driver not present or signature mismatch: return built-in version */
      return (IO_U32)(((IO_U32)FLASH_DRIVER_VERSION_MAJOR << 16) | \
                      ((IO_U32)FLASH_DRIVER_VERSION_MINOR <<  8) | \
                      ((IO_U32)FLASH_DRIVER_VERSION_PATCH));
   }
   else
   {
      /* Flash driver is present, return actual version */
      return (IO_U32)(((IO_U32)flashParam.majorVersion << 16) | \
                      ((IO_U32)flashParam.minorVersion <<  8) | \
                      ((IO_U32)flashParam.patchLevel));
   }
} /* PRQA S 2006 */ /* MD_CBD_14.7 */

#if defined( FLASH_ENABLE_SET_RESETVECTOR_API )
/***********************************************************************************************************************
 *  FlashDriver_SetResetVector
 **********************************************************************************************************************/
/*! \brief       Set new reset vector.
 *  \pre         Flash driver is initialized.
 *  \param[in]   New reset vector address.
 *  \return      Reports success or error code.
 **********************************************************************************************************************/
IO_ErrorType FlashDriver_SetResetVector ( IO_PositionType vectorAddress )
{
   flashParam.address    = vectorAddress;

   /* Enable flash write access */
   FBL_FHVE15 = 0x01ul;
   FBL_FHVE3 = 0x01ul;

   FLASH_DRIVER_SET_RESET_VECTOR(flashCode, &flashParam); /* PRQA S 3305, 0305, 0310 */ /* MD_FblWrapperFlash_3305, MD_FblWrapperFlash_0305_FlashHisAPI, MD_FblWrapperFlash_0310_FlashHisAPI */

   /* Disable flash write access */
   FBL_FHVE15 = 0x00ul;
   FBL_FHVE3 = 0x00ul;

   return (flashParam.errorCode);
}
#endif /* FLASH_ENABLE_SET_RESETVECTOR_API */

/***********************************************************************************************************************
 *  MISRA DEVIATIONS
 **********************************************************************************************************************/

/* Module specific MISRA deviations:

   MD_FblWrapperFlash_3305:
     Reason: flashCode needs to be accessed in 2 different ways 8 Bit and 32 Bit wide.
     Risk: Misaligned access will cause non functional driver.
     Prevention: flashCode shall be mapped by linker command file 4 Byte aligned.

   MD_FblWrapperFlash_writeBuffer_3305:
     Reason: writeBuffer needs to be accessed in 2 different ways 8 Bit and 32 Bit wide.
     Risk: Misaligned access will cause bad values in flash.
     Prevention: writeBuffer must be 4 Byte aligned (calling restriction). Caller is responsible.

   MD_FblWrapperFlash_3673:
     Reason: API definition don't use const keyword.
     Risk: -
     Prevention: not necessary - The API is fix for several drivers.

   MD_FblWrapperFlash_3112
     Reason: Avoid compiler warnings for unused parameters.
     Risk: -
     Prevention: -

   MD_FblWrapperFlash_17.4
     Reason: pointer arithmetic warning.
     Risk: unintended pointer behaviour because of different pointer sizes
     Prevention: code review and functional tests

   MD_FblWrapperFlash_0303_HWaccess:
   Misra Rule 11.3
     Reason: Hardware access need pointer as volatile.
     Risk: Access of unknown memory.
     Prevention: Runtime tests.

   MD_FblWrapperFlash_0305_FlashHisAPI:
   Misra Rule 11.3
     Reason: Defined HIS Flash driver API.
     Risk: Access of unknown memory.
     Prevention: Runtime tests.

   MD_FblWrapperFlash_0306_Memcpy:
   Misra Rule 11.3
     Reason: memcpy API vs HIS API.
     Risk: Access of unknown memory.
     Prevention: Runtime tests.

   MD_FblWrapperFlash_0310_FlashHisAPI:
   Misra Rule 11.4
     Reason: Defined HIS Flash driver API.
     Risk: Access of unknown memory.
     Prevention: Runtime tests.
*/

/***********************************************************************************************************************
 *  END OF FILE: FBL_FLIO.C
 **********************************************************************************************************************/
