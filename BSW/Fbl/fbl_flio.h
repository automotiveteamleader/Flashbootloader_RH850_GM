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
 *  01.01.00   2014-02-27  Ach     ESCAN00073965      No changes
 *  01.02.00   2014-07-01  CEl     ESCAN00074163      No changes
 *  01.03.00   2014-10-23  Rr      ESCAN00078958      Provide API for changing reset vector via Selflib
 *                                 ESCAN00078773      ECC depends on FLASH_ENABLE_MACHINE_CHECK_ECC_DETECTION
 *                                                    Harmonized ECC handling
 *  01.04.00   2016-02-26  Dod     ESCAN00085770      No changes
 *                         Dod     ESCAN00086372      No changes
 *  01.05.00   2016-03-18  QPs     ESCAN00088975      Added support for secondary driver
 *  01.06.00   2016-05-09  TnS     ESCAN00087121      No changes
 *  01.07.00   2016-05-23  CB                         Adapted filter settings for E1x, no code changes
 **********************************************************************************************************************/
#ifndef __FBLWRAPPERFLASH_RH850RV40HIS_H__
#define __FBLWRAPPERFLASH_RH850RV40HIS_H__

/***********************************************************************************************************************
 *  VERSION
 **********************************************************************************************************************/

/* ##V_CFG_MANAGEMENT ##CQProject : FblWrapperFlash_Rh850Rv40His CQComponent : Implementation */
#define FBLWRAPPERFLASH_RH850RV40HIS_VERSION           0x0107u
#define FBLWRAPPERFLASH_RH850RV40HIS_RELEASE_VERSION   0x00u

/***********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

IO_ErrorType FlashDriver_InitSync ( void * );
IO_ErrorType FlashDriver_DeinitSync ( void * );
IO_ErrorType FlashDriver_RWriteSync ( IO_MemPtrType, IO_SizeType, IO_PositionType );
IO_ErrorType FlashDriver_REraseSync ( IO_SizeType, IO_PositionType );
IO_ErrorType FlashDriver_RReadSync ( IO_MemPtrType, IO_SizeType, IO_PositionType );
IO_U32 FlashDriver_GetVersionOfDriver( void );
#if defined( FLASH_ENABLE_SET_RESETVECTOR_API )
IO_ErrorType FlashDriver_SetResetVector ( IO_PositionType );
#endif /* FLASH_ENABLE_SET_RESETVECTOR_API */

/***********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

# define FLASHCODE_START_SEC_VAR_EXPORT
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
V_MEMRAM0 extern V_MEMRAM1 vuint8 V_MEMRAM2 flashCode[FLASH_SIZE];
# define FLASHCODE_STOP_SEC_VAR_EXPORT
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* __FBLWRAPPERFLASH_RH850RV40HIS_H__ */

/***********************************************************************************************************************
 *  END OF FILE: FBL_FLIO.H
 **********************************************************************************************************************/
