/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief        Memory Driver wrapper functions
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
 *  Christian Baeuerle            CB            Vector Informatik GmbH
 *  Armin Happel                  Hp            Vector Informatik GmbH
 *  Konrad Lazarus                Ls            Vector Informatik GmbH
 *  Marco Wierer                  WM            Vector Informatik GmbH
 *  Markus Holzer                 MHz           Vector Informatik GmbH
 *  Andreas Pick                  AP            Vector Informatik GmbH
 *  Ralf Fritz                    Fz            Vector Informatik GmbH
 *  Alexandre C. Plombin          ACP           Vector CANtech, Inc.
 *  Florian Hees                  FHe           Vector Informatik GmbH
 *  Ahmad Nasser                  AMN           Vector CANtech, Inc.
 *  Michael F. Radwick            MFR           Vector CANtech, Inc.
 *  Joern Herwig                  JHg           Vector Informatik GmbH
 *  Andre Caspari                 Ci            Vector Informatik GmbH
 *  Slawomir Wilk                 SWk           Vector Informatik GmbH
 *  Thomas Bezold                 TBe           Vector Informatik GmbH
 *  Quetty Palacios               QPs           Vector Informatik GmbH
 *  Andreas Wenckebach            AWh           Vector Informatik GmbH
 *  Achim Strobelt                Ach           Vector Informatik GmbH
 *  Alexander Starke              ASe           Vector Informatik GmbH
 *  Ralf Haegenlaeuer             HRf           Vector Informatik GmbH
 *  Jens Christ                   Cjs           Vector Informatik GmbH
 *  Marcel Viole                  MVi           Vector Informatik GmbH
 *  Robert Schaeffner             Rr            Vector Informatik GmbH
 *  Torben Stoessel               TnS           Vector Informatik GmbH
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  01.00.00   2003-12-15  WM                       Implementation
 *  01.01.00   2004-02-10  WM      ESCAN00007465    Adaptations for C_COMP_IAR_12
 *  01.02.00   2004-03-05  Fz                       No changes
 *  01.03.00   2004-04-23  WM                       Minor changes
 *  01.04.00   2004-07-06  Mhz                      Minor changes
 *  01.05.00   2004-07-06  Ls                       No changes in this file
 *  01.06.00   2004-07-28  WM                       Minor changes
 *  01.07.00   2004-08-04  WM                       No changes
 *  01.08.00   2004-08-10  Ap                       No changes
 *  01.09.00   2004-12-23  ACP                      Version check reworked
 *  01.10.00   2005-02-16  AMN                      MemDriver_REraseAllSync for FORD_SLP3 added
 *  01.11.00   2005-03-24  FHe                      Added MemDriver_REraseAllSync for GM
 *                         WM                       Minor changes
 *  01.12.00   2005-06-07  Hp                       Added support for V85x/IAR
 *                         AMN                      Moved EraseAllSync to DiagSectorErase for Ford SLP3
 *  01.13.00   2005-08-10  AMN                      Added version check using v_ver.h
 *  01.14.00   2005-09-27  MFR                      Moved EraseAllSync to DiagEraseAll for GM SLP2
 *                         WM      ESCAN00013673    Support for C_COMP_HIWARE_12 added
 *  01.15.00   2005-11-10  MFR                      Minor changes for MISRA
 *  01.16.00   2005-11-30  WM                       No changes
 *  01.17.00   2005-12-29  FHE                      No changes
 *  01.18.00   2006-02-15  WM      ESCAN00015630    Dummy function added
 *  01.19.00   2006-07-17  JHg                      Added V_API_NEAR to functions
 *  01.20.00   2006-08-12  Hp                       Add interface functions for direct device function access.
 *  01.21.00   2006-10-11  Ci                       No changes
 *                         SWk                      No changes
 *  01.22.00   2008-11-10  CB      ESCAN00015630    Added V_API_NEAR for dummy fct.
 *  01.23.00   2009-03-17  JHg     ESCAN00033924    Enabled MemDriverReady for OEM Volvo AB
 *  01.24.00   2009-08-20  Hp                       Status variable to indicate if erase or write is active.
 *                         SWk     ESCAN00036931    CW-layer introduced (optionally).
 *                                                  Add FBL_ENABLE_MEM_DRIVER_DEVICE_COMMENT, used for:
 *                                                  Volvo, GM SLP2/SLP3 only.
 *  01.25.00   2009-08-24  SWk                      Added a dummy statement to avoid the compiler warning
 *  01.26.00   2009-12-09  TBe     ESCAN00039376    Adaptions for +strict option in Cosmic Compiler
 *  01.27.00   2009-12-11  SWk     ESCAN00039695    Rework this file for compatibility with old interface
 *  01.28.00   2010-01-14  QPs     ESCAN00040144    MemDriver_VerifySync with CW-layer and multiple memory
 *                                                  devices reworked
 *  01.29.00   2010-02-16  SWk     ESCAN00040355    Rework of this file
 *                         Ci      ESCAN00039458    Enabled MemDriverReady for OEM Ford (SLP6)
 *  01.30.00   2010-03-22  SWk     ESCAN00041499    No changes
 *  01.31.00   2010-04-21  SWk     ESCAN00042370    Compiler error with multiple memory devices configuration
 *                         SWk     ESCAN00042595    No changes
 *  01.32.00   2010-07-01  SWk     ESCAN00043920    Introduction of FBL_ENABLE_FUNC_VERIFYSYNC for V850 and 78k0(R)
 *  01.33.00   2011-03-02  SWk     ESCAN00048161    Add prototype for dummy() to avoid compiler warning
 *  01.34.00   2011-03-17  SWk     ESCAN00048791    FlashDriver_VerifySync is not always called for the internal flash driver
 *  01.35.00   2011-04-19  SWk     ESCAN00049841    Add prototype of dummy() into NON_BANKED section to avoid compiler warning
 *                         SWk     ESCAN00049851    No changes
 *  01.36.00   2011-08-12  Rr      ESCAN00052726    Enabled MemDriverReady for Renault
 *  01.37.00   2012-04-05  QPs     ESCAN00058147    Added status variable to indicate which function is active
 *  01.38.00   2012-09-06  AWh     ESCAN00061245    Added FBL_CLIENT_GM_SLP4
 *  01.39.00   2012-11-29  Ach     ESCAN00062923    Adapted comments to use Doxygen
 *  01.40.00   2013-12-13  JHg     ESCAN00064823    Moved pragmas for C_COMP_HIWARE_12
 *                         ASe     -                General refactoring
 *                         JHg     ESCAN00072598    Removed explicit dependencies to fbl_diag.h and fbl_wd.h
 *  01.41.00   2014-04-30  HRf     ESCAN00075330    Add support for OEM PATAC
 *  01.42.00   2014-05-28  JHg     ESCAN00075942    Set device active status for all use-cases during initialization
 *  01.42.01   2014-07-29  Cjs     ESCAN00077469    MemDriver_RInitEnd(i); called regardless of result
 *  01.42.02   2015-01-14  AWh     ESCAN00080412    No changes
 *  01.43.00   2015-02-23  MVi     ESCAN00081287    Added support for MemMap
 *  01.44.00   2015-07-29  AWh     ESCAN00084131    Beautification
 *  01.45.00   2015-10-23  Rr      ESCAN00085800    No changes
 *                         Awh     ESCAN00085806    Remove GM SLP1 and GM SLP2
 *  01.46.00   2016-01-22  CB      ESCAN00087673    Adapted switches for MCS12X
 *  01.46.01   2016-02-25  TnS     ESCAN00088473    MISRA related code changes
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

#include "fbl_inc.h"

/***********************************************************************************************************************
 *  VERSION
 **********************************************************************************************************************/

#if ( FBLMIO_VERSION != 0x0146u ) || \
    ( FBLMIO_RELEASE_VERSION != 0x01u )
# error "Error in FBL_MIO.C: Source and header file are inconsistent!"
#endif

#if ( FBLMIO_VERSION != _FBLMIO_VERSION ) || \
    ( FBLMIO_RELEASE_VERSION != _FBLMIO_RELEASE_VERSION )
# error "Error in FBL_MIO.C: Source and V_VER.H are inconsistent!"
#endif

/***********************************************************************************************************************
 *  GLOBALS
 **********************************************************************************************************************/

/* Device active status, if currently an    */
/*  erase or programming is active.         */
/*  It contains either the state "Idle" or  */
/*  the device driver number that is active  */
vuint8 memDrvDeviceActive;
vuint8 memDrvFunctionActive;

#define FBLMIO_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/***********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

#if defined( FBL_ENABLE_MULTIPLE_MEM_DEVICES )
/***********************************************************************************************************************
 *  MemDriver_RInitSync
 **********************************************************************************************************************/
/*! \brief      Initialize all memory drivers.
 *  \pre        Memory drivers have to be present (e.g. copied to RAM).
 *  \param[in]  address Parameter to be passed to HIS interface flash driver.
 *  \return     Reports if all memory drivers have been initialized successfully.
 **********************************************************************************************************************/
IO_ErrorType V_API_NEAR MemDriver_RInitSync( void * address )
{
   IO_U8 i;
   IO_ErrorType result = IO_E_OK;
   IO_ErrorType rc;

   for (i = 0; i < kNrOfMemDrv; i++)
   {
      MemDriver_RInitBegin(i);
      rc = (*memDrvLst[i].init)(address);
      MemDriver_RInitEnd(i);
      if (rc != IO_E_OK)
      {
         result = rc;
         break;
      }
   }
   return result;
}

/***********************************************************************************************************************
 *  MemDriver_RDeinitSync
 **********************************************************************************************************************/
/*! \brief      Deinitialize all memory drivers.
 *  \pre        Memory drivers have to be present (e.g. copied to RAM).
 *  \param[in]  address Parameter to be passed to HIS interface flash driver.
 *  \return     The return code shows the success of the deinitialization.
 **********************************************************************************************************************/
IO_ErrorType V_API_NEAR MemDriver_RDeinitSync( void * address )
{
   IO_U8 i;
   IO_ErrorType result = IO_E_OK;

   for (i = 0; i < kNrOfMemDrv; i++)
   {
      result = (*memDrvLst[i].deinit)(address);
      if (result != IO_E_OK)
      {
         return result;
      }
   }
   return result;
}
#else
#endif

# if defined( FBL_ENABLE_MULTIPLE_MEM_DEVICES )
/***********************************************************************************************************************
 *  MemDriver_RWriteSync
 **********************************************************************************************************************/
/*! \brief      Write data into memory
 *  \details    This function selects the memory driver used to program a memory device and
 *              calls the write function of this driver.
 *  \pre        All drivers are initialized, memory to be written is erased (flash memory).
 *  \param[in]  buf Pointer to input data buffer.
 *  \param[in]  len Data length. Has to be aligned to a multiple of the smallest writable data segment.
 *  \param[in]  addr Write target address. Has to be aligned to a write segment start address.
 *  \return     Reports write success or error code.
 **********************************************************************************************************************/
IO_ErrorType V_API_NEAR MemDriver_RWriteSync(IO_MemPtrType buf, IO_SizeType len, IO_PositionType addr)
{
   IO_ErrorType ret_status;
   MemDriver_RWriteBegin(FlashBlock[memSegment].device);
   ret_status = (*memDrvLst[FlashBlock[memSegment].device].write)((buf), (len), (addr));
   MemDriver_RWriteEnd(FlashBlock[memSegment].device);
   return ret_status;
}

/***********************************************************************************************************************
 *  MemDriver_REraseSync
 **********************************************************************************************************************/
/*! \brief      Erase area in memory
 *  \details    This function erases an area in memory.
 *  \pre        All drivers are initialized.
 *  \param[in]  len Length of memory to be erased. The length has to be aligned to the length of one or
 *              more erasable sectors of the used memories.
 *  \param[in]  addr Start address of erase area. The start address has to be aligned to the start address
 *              of an erasable area.
 *  \return     Reports erase success or error code.
 **********************************************************************************************************************/
IO_ErrorType V_API_NEAR MemDriver_REraseSync(IO_SizeType len, IO_PositionType addr)
{
   IO_ErrorType ret_status;
   MemDriver_REraseBegin(FlashBlock[memSegment].device);
   ret_status = (*memDrvLst[FlashBlock[memSegment].device].erase) ((len), (addr));
   MemDriver_REraseEnd(FlashBlock[memSegment].device);
   return ret_status;
}

# else /* FBL_ENABLE_MULTIPLE_MEM_DEVICES */
/***********************************************************************************************************************
 *  MemDriver_RWriteSync
 **********************************************************************************************************************/
/*! \brief      Write data into memory
 *  \details    This function calls the write function of the internal flash memory.
 *  \pre        Internal flash driver is initialized, memory to be written is erased.
 *  \param[in]  buf Pointer to input data buffer.
 *  \param[in]  len Data length. Has to be aligned to a multiple of the smallest writable data segment.
 *  \param[in]  addr Write target address. Has to be aligned to a write segment start address.
 *  \return     Reports write success or error code.
 **********************************************************************************************************************/
IO_ErrorType V_API_NEAR MemDriver_RWriteSync( IO_MemPtrType buf, IO_SizeType len, IO_PositionType addr )
{
   IO_ErrorType ret_status;
   MemDriver_RWriteBegin(kMioDeviceInternalFlash); /* PRQA S 3109 */ /* MD_CBD_14.3 */
   ret_status = FlashDriver_RWriteSync(buf, len, addr);
   MemDriver_RWriteEnd(kMioDeviceInternalFlash); /* PRQA S 3109 */ /* MD_CBD_14.3 */
   return ret_status;
}

/***********************************************************************************************************************
 *  MemDriver_REraseSync
 **********************************************************************************************************************/
/*! \brief      Erase area in memory
 *  \details    This function erases an area in memory.
 *  \pre        Internal flash driver is initialized.
 *  \param[in]  len Length of memory to be erased. The length has to be aligned to the length of one or
 *              more erasable sectors of the internal flash.
 *  \param[in]  addr Start address of erase area. The start address has to be aligned to the start address
 *              of an erasable area.
 *  \return     Reports erase success or error code.
 **********************************************************************************************************************/
IO_ErrorType V_API_NEAR MemDriver_REraseSync( IO_SizeType len, IO_PositionType addr )
{
   IO_ErrorType ret_status;
   MemDriver_REraseBegin((kMioDeviceInternalFlash)); /* PRQA S 3109 */ /* MD_CBD_14.3 */
   ret_status = FlashDriver_REraseSync(len, addr);
   MemDriver_REraseEnd((kMioDeviceInternalFlash)); /* PRQA S 3109 */ /* MD_CBD_14.3 */
   return ret_status;
}

/***********************************************************************************************************************
 *  MemDriver_RInitSync
 **********************************************************************************************************************/
/*! \brief      Initialize all memory drivers
 *  \pre        All memory drivers need to be copied to RAM (if necessary)
 *  \param      address Not used; conformance to HIS interface
 *  \return     The return code shows the success of the initialization
 **********************************************************************************************************************/
IO_ErrorType V_API_NEAR MemDriver_RInitSync( void * address ) /* PRQA S 3673 */ /* MD_FblMio_3673 */
{
   IO_ErrorType ret_status;
# if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   (void)address; /* PRQA S 3112 */ /* MD_FblMio_3112 */
# endif
   MemDriver_RInitBegin(kMioDeviceInternalFlash); /* PRQA S 3109 */ /* MD_CBD_14.3 */
   ret_status = FlashDriver_InitSync(V_NULL);
   MemDriver_RInitEnd(kMioDeviceInternalFlash); /* PRQA S 3109 */ /* MD_CBD_14.3 */
   return ret_status;
}

# endif /* FBL_ENABLE_MULTIPLE_MEM_DEVICES */

#define FBLMIO_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*****************************************************************************/

/* module specific MISRA deviations:

   MD_FblMio_3112:
     Reason: Dummy statements have been added to avoid compiler warnings because of unused variables.
     Risk: Unused statements add to program complexity just by being there - adding lines to the program and causing
      readers to puzzle over its presence.
     Prevention: Dummy statements can be turned off externally (V_DISABLE_USE_DUMMY_STATEMENT).

   MD_FblMio_3673:
     Reason: This is a standard API that should not be changed for compatibility reasons.
     Risk: No identifiable risk.
     Prevention: No prevention required.

   MD_FblMio_Dummy:
     Reason: This Dummy function is only existent because some compilers require that every source module implements functions/variables and not
             only macros. However, depending on the configuration, only macros will be present in the file. The function is only required to be
             present, in configurations where no other variables/functions are present.
     Risk: No identifiable risk.
     Prevention: No prevention required.

*/

/***********************************************************************************************************************
 *  END OF FILE: FBL_MIO.C
 **********************************************************************************************************************/

