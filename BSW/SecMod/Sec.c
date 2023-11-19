/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief        Implementation of the HIS security module
 *
 *  --------------------------------------------------------------------------------------------------------------------
 *  COPYRIGHT
 *  --------------------------------------------------------------------------------------------------------------------
 *  \par Copyright
 *  \verbatim
 *  Copyright (c) 2015 by Vector Informatik GmbH.                                                  All rights reserved.
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
 *                                                   Renamed file name (was SecM.c)
 *                                                   Removed SecM_StateTask
 *  02.01.00   2013-11-08  JHg     ESCAN00071173    Added support for CRC, seed/key and verification APIs
 *  02.01.01   2013-11-08  JHg     ESCAN00071733    No changes
 *  02.01.02   2013-11-11  JHg     ESCAN00071785    No changes
 *  02.02.00   2013-12-19  Mss     ESCAN00072743    No changes
 *  02.03.00   2014-02-20  JHg     ESCAN00073562    No changes
 *                         Mss     ESCAN00073600    No changes
 *                         Mss     ESCAN00073811    No changes
 *  02.04.00   2015-02-09  CB      ESCAN00079848    No changes
 *  02.04.01   2015-03-10  Rie     ESCAN00081703    Fix for SeedKey timer configuration
 *  02.04.02   2015-07-29  AWh     ESCAN00084125    No changes
 *  02.05.00   2015-10-12  JHg     ESCAN00085810    No changes
 *  02.05.01   2015-10-28  JHg     ESCAN00086114    No changes
 *  02.06.00   2015-11-11  JHg     ESCAN00086123    Initialize CRC module at start-up
 *                         JHg     ESCAN00086125    Adaptations for MISRA compliance
 *                         JHg     ESCAN00086239    Moved SecM_GetInteger and SecM_SetInteger from verification
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

/* Security module configuration settings */
#include "Sec_Inc.h"

/* Global definitions for security module */
#include "Sec_Types.h"

#include "Sec.h"

/***********************************************************************************************************************
 *   VERSION
 **********************************************************************************************************************/

#if ( SYSSERVICE_SECMODHIS_VERSION != 0x0206u ) || \
    ( SYSSERVICE_SECMODHIS_RELEASE_VERSION != 0x00u )
# error "Error in SecM.c: Source and header file are inconsistent!"
#endif

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

#if defined( SEC_KEY_TIMEOUT ) && ( SEC_KEY_TIMEOUT != 0x00u )
/** Timer for validity of seed value */
V_MEMRAM0 V_MEMRAM1 SecM_ShortType V_MEMRAM2 secTimer;
#endif

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

static void SecM_TimerTask( void );

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  SecM_TimerTask
 *********************************************************************************************************************/
/*! \brief         Handle all timer related task
 *  \details       Called with period of SEC_CALL_CYCLE ms
 *********************************************************************************************************************/
static void SecM_TimerTask( void )
{
   /* Count down key timer */
   SecM_DecrKeyTimer(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
}

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  SecM_InitPowerOn
 *********************************************************************************************************************/
/*! \brief         Initialize security module
 *  \param[in]     initParam Initialization parameters (unused - reserved for future use)
 *  \return        SECM_OK if initialization successful
 *                 SECM_NOT_OK otherwise
 *********************************************************************************************************************/
SecM_StatusType SecM_InitPowerOn( SecM_InitType initParam )
{
   SecM_StatusType result;

#if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   /* Avoid compiler warnings */
   (void)initParam; /* PRQA S 3112 */ /* MD_Sec_3112 */
#endif /* V_ENABLE_USE_DUMMY_STATEMENT */

   result = SECM_OK;

   return result;
}

/**********************************************************************************************************************
 *  SecM_Task
 *********************************************************************************************************************/
/*! \brief         Cyclic task of security module
 *  \details       Called with period of SEC_CALL_CYCLE ms
 *********************************************************************************************************************/
void SecM_Task( void )
{
   SecM_TimerTask();
}

/***********************************************************************************************************************
 *  SecM_GetInteger
 **********************************************************************************************************************/
/*! \brief      Convert given big-endian byte array to integer value
 *  \param[in]  count Number of relevant bytes
 *  \param[in]  buffer Pointer to input buffer
 *  \return     Integer value
 **********************************************************************************************************************/
SecM_WordType SecM_GetInteger( SecM_ByteFastType count, SecM_ConstRamDataType buffer )
{
   SecM_WordType  output;
   SecM_ByteFastType  index;

   output   = 0u;
   for (index = 0u; index < count; index++)
   {
      /* Most significant byte first */
      output <<= 8u;
      /* Add current byte */
      output |= (SecM_WordType)buffer[index];
   }

   return output;
}

/***********************************************************************************************************************
 *  SecM_SetInteger
 **********************************************************************************************************************/
/*! \brief      Convert given integer value to big-endian byte array
 *  \param[in]  count Number of relevant bytes
 *  \param[in]  input Input value
 *  \param[out] buffer Pointer to output buffer
 **********************************************************************************************************************/
void SecM_SetInteger( SecM_ByteFastType count, SecM_WordType input, SecM_RamDataType buffer )
{
   SecM_ByteFastType index;

   /* Loop relevant bytes */
   index = count;
   while (index > 0u)
   {
      index--;
      /* Store most significant byte first */
      buffer[index] = (SecM_ByteType)(input & 0xFFu);
      /* Shift in next byte */
      input >>= 8u;
   }
}

/**********************************************************************************************************************
 *  MISRA
 *********************************************************************************************************************/

/* Module specific MISRA deviations:

   MD_Sec_3112:
     Reason: Dummy statements have been added to avoid compiler warnings because of unused variables.
     Risk: Unused statements add to program complexity just by being there - adding lines to the program and causing
      readers to puzzle over its presence.
     Prevention: Dummy statements can be turned off externally (V_DISABLE_USE_DUMMY_STATEMENT).

*/

/***********************************************************************************************************************
 *  END OF FILE: SEC.C
 **********************************************************************************************************************/

