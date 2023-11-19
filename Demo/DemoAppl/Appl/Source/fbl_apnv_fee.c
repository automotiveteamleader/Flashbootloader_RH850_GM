/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief        Wrapper for FEE access
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

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Andreas Wenckebach            AWh           Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id          Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2015-05-22  AWh     -                  Initial version
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "fbl_inc.h"

#include "Fee.h"
#include "Fls.h"
#include "SchM_Fls.h"

/**********************************************************************************************************************
 *  DEFINES
 *********************************************************************************************************************/

/* Erased value (if data is not available) */
#define FBL_NV_ERASED_VALUE                                0xFFu

/* Macro checking for FBL_TM_TRIGGERED can be used, if hard polling is not possible. Watchdog need to be initialized. */
#define CHECK_POLLING_ALLOWED(triggerResult) (1u == 1u)
/* #define CHECK_POLLING_ALLOWED(triggerResult) (FBL_TM_TRIGGERED == (triggerResult & FBL_TM_TRIGGERED)) */

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/***********************************************************************************************************************
 *  ApplFblNvRead
 **********************************************************************************************************************/
/*! \brief       Reads data from EA
 *  \param[in]   blockNumber Number of block
 *  \param[in]   blockOffset Offset in block
 *  \param[out]  pBuffer The returned read data
 *  \param[in]   length Number of bytes to read
 *  \return      kFblOk if succeeded, kFblFailed otherwise
 **********************************************************************************************************************/
tFblResult ApplFblNvRead( vuint16 blockNumber, vuint16 blockOffset,
                                 V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * pBuffer, vuint16 length )
{
   tFblResult result;
   vuintx i;

   if (E_OK == Fee_Read(blockNumber, blockOffset, pBuffer, length))
   {
      while ( MEMIF_IDLE != Fee_GetStatus() )
      {
         result = FblRealTimeSupport();

         if (CHECK_POLLING_ALLOWED(result))
         {
            Fls_MainFunction();
         }

         Fee_MainFunction();
      }

      if (MEMIF_JOB_OK == Fee_GetJobResult())
      {
         result = kFblOk;
      }
      else if (   (MEMIF_BLOCK_INCONSISTENT == Fee_GetJobResult())
               || (MEMIF_BLOCK_INVALID == Fee_GetJobResult()))
      {
         for (i = 0u; i < length; i++)
         {
            pBuffer[i] = FBL_FLASH_DELETED;
         }

         result = kFblOk;
      }
      else
      {
         result = kFblFailed;
      }
   }
   else
   {
      result = kFblFailed;
   }

   return result;
}

/***********************************************************************************************************************
 *  ApplFblNvWrite
 **********************************************************************************************************************/
/*! \brief       Writes data to EA
 *  \param[in]   blockNumber Number of block
 *  \param[out]  pBuffer The data to be written
 *  \return      kFblOk if succeeded, kFblFailed otherwise
 **********************************************************************************************************************/
tFblResult ApplFblNvWrite( vuint16 blockNumber, V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * pBuffer )
{
   tFblResult result;

   if (E_OK == Fee_Write(blockNumber, pBuffer))
   {
      while ( MEMIF_IDLE != Fee_GetStatus() )
      {
         result = FblRealTimeSupport();

         if (CHECK_POLLING_ALLOWED(result))
         {
            Fls_MainFunction();
         }

         Fee_MainFunction();
      }

      if (MEMIF_JOB_OK == Fee_GetJobResult())
      {
         result = kFblOk;
      }
      else
      {
         result = kFblFailed;
      }
   }
   else
   {
      result = kFblFailed;
   }

   return result;
}

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/***********************************************************************************************************************
 *  ApplFblNvInit
 **********************************************************************************************************************/
/*! \brief       Initializes NV-data handling
 *  \return      kFblOk if succeeded, kFblFailed otherwise
 **********************************************************************************************************************/
tFblResult ApplFblNvInit( void )
{
   tFblResult result;

   result = kFblOk;

   Fls_Init(FlsConfigSet);

   while (MEMIF_IDLE != Fls_GetStatus())
   {
      result = FblRealTimeSupport();

      if (CHECK_POLLING_ALLOWED(result))
      {
         Fls_MainFunction();
      }
   }

   Fee_Init();

   while ( MEMIF_IDLE != Fee_GetStatus() )
   {
      result = FblRealTimeSupport();

      if (CHECK_POLLING_ALLOWED(result))
      {
         Fls_MainFunction();
      }

      Fee_MainFunction();
   }

#if (FEE_FSS_CONTROL_API == STD_ON)
   Fee_EnableFss();
#endif

   return result;
}

/***********************************************************************************************************************
 *  END OF FILE: FBL_APNV_EA.C
 **********************************************************************************************************************/
