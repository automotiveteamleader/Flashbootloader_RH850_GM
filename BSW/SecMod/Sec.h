/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief        Implementation of the HIS security module
 *
 *  -------------------------------------------------------------------------------------------------------------------
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \par Copyright
 *  \verbatim
 *  Copyright (c) 2015 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 */
/*********************************************************************************************************************/

/***********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  --------------------------------------------------------------------------------------------------------------------
 *  Joern Herwig                  JHg           Vector Informatik GmbH
 *  Markus Schneider              Mss           Vector Informatik GmbH
 *  Christian Baeuerle            CB            Vector Informatik GmbH
 *  Marco Riedl                   Rie           Vector Informatik GmbH
 *  Andreas Wenckebach            AWh           Vector Informatik GmbH
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  01.00.00   2013-06-20  JHg     ESCAN00068213    Initial release based on FblSecMod_Vector 02.02.00
 *  02.00.00   2013-10-01  JHg     ESCAN00070689    Major refactoring
 *                                                   Split into separate sub-packages
 *                                                   Renamed file name (was SecM.h)
 *                                                   Select API sub-set at integration time
 *  02.01.00   2013-11-08  JHg     ESCAN00071173    Added support for CRC, seed/key and verification APIs
 *  02.01.01   2013-11-08  JHg     ESCAN00071733    No changes
 *  02.01.02   2013-11-11  JHg     ESCAN00071785    No changes
 *  02.02.00   2013-12-19  Mss     ESCAN00072743    Adapted switch to support encryption API
 *  02.03.00   2014-02-20  JHg     ESCAN00073562    No changes
 *                         Mss     ESCAN00073600    No changes
 *                         Mss     ESCAN00073811    Fixed typo
 *  02.04.00   2015-02-09  CB      ESCAN00079848    Modified SeedKey timer configuration
 *  02.04.01   2015-03-10  Rie     ESCAN00081703    Fix for SeedKey timer configuration
 *  02.04.02   2015-07-29  AWh     ESCAN00084125    No changes
 *  02.05.00   2015-10-12  JHg     ESCAN00085810    No changes
 *  02.05.01   2015-10-28  JHg     ESCAN00086114    No changes
 *  02.06.00   2015-11-11  JHg     ESCAN00086123    No changes
 *                         JHg     ESCAN00086125    Adaptations for MISRA compliance
 *                         JHg     ESCAN00086239    Moved SecM_GetInteger and SecM_SetInteger from verification
 **********************************************************************************************************************/

#ifndef __SEC_H__
#define __SEC_H__

/**********************************************************************************************************************
 *  VERSION
 *********************************************************************************************************************/

/* ##V_CFG_MANAGEMENT ##CQProject : SysService_SecModHis CQComponent : Implementation */
# define SYSSERVICE_SECMODHIS_VERSION              0x0206u
# define SYSSERVICE_SECMODHIS_RELEASE_VERSION      0x00u

/** API compatible to HIS security module specification 1.1 */
# define SECM_HIS_SECURITY_MODULE_VERSION          0x010100ul

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

/* Security module configuration settings */
#include "Sec_Inc.h"

/* API dependent includes */
# include "Sec_Verification.h"

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

#if defined( SEC_KEY_TIMEOUT ) && ( SEC_KEY_TIMEOUT != 0x00u )
/** Start key timer (seed value valid until timer runs out) */
# define  SecM_StartKeyTimer()   (secTimer = (SEC_KEY_TIMEOUT / SEC_CALL_CYCLE))
/** Stop key timer (invalidate seed value) */
# define  SecM_StopKeyTimer()    (secTimer = 0u)
/** Get value of key timer */
# define  SecM_GetKeyTimer()     (secTimer)
/** Decrement key timer if active (> 0) */
# define  SecM_DecrKeyTimer() \
   if (secTimer > 0)          \
   {                          \
      secTimer--;             \
   }
#else
/* Key timer not used, seed validity never expires */
# define  SecM_StartKeyTimer()
# define  SecM_StopKeyTimer()
# define  SecM_GetKeyTimer()     (1u)
# define  SecM_DecrKeyTimer()
#endif

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/** Parameter type passed to SecM_InitPowerOn */
typedef SecM_ConstVoidPtrType   SecM_InitType;

/** Pointer type for function calls from external application */
typedef void SEC_CALL_TYPE (* pSecTaskFct)( void );

/***********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

#if defined( SEC_KEY_TIMEOUT ) && ( SEC_KEY_TIMEOUT != 0x00u )
V_MEMRAM0 extern V_MEMRAM1 SecM_ShortType V_MEMRAM2 secTimer;
#endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#if defined( __cplusplus )
extern "C" {
#endif

extern SecM_StatusType SecM_InitPowerOn( SecM_InitType initParam );
extern void SecM_Task( void );

extern SecM_WordType SecM_GetInteger( SecM_ByteFastType count, SecM_ConstRamDataType buffer );
extern void SecM_SetInteger( SecM_ByteFastType count, SecM_WordType input, SecM_RamDataType buffer );

#if defined( __cplusplus )
} /* extern "C" */
#endif

#endif /* __SEC_H__ */

/***********************************************************************************************************************
 *  END OF FILE: SEC.H
 **********************************************************************************************************************/
