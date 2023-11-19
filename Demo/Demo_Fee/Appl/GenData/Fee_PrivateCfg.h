/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2012 - 2016 by Vector Informatik GmbH.                                           All rights reserved.
 *
 *                 This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                 Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                 All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  LICENSE
 *  -------------------------------------------------------------------------------------------------------------------
 *            Module: Fee
 *           Program: GM Global B / CAN/LIN/IP (MSR_Gm_SLP1)
 *          Customer: Nexteer Automotive Corporation
 *       Expiry Date: Not restricted
 *  Ordered Derivat.: Renesas RH850 P1M
 *    License Scope : The usage is restricted to CBD1500636_D00
 *
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *              File: Fee_PrivateCfg.h
 *   Generation Time: 2016-02-29 17:25:03
 *           Project: EIP_GlobalB - Version 1.0
 *          Delivery: CBD1500636_D00
 *      Tool Version: DaVinci Configurator (beta) 5.10.26 SP3
 *
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 ! BETA VERSION                                                                                                       !
 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 ! These programs are fully operative programs. With regard to the fact that the programs are a beta-version only,    !
 ! Vector Informatik's liability shall be expressly excluded in cases of ordinary negligence, to the extent           !
 ! admissible by law or statute.                                                                                      !
 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
**********************************************************************************************************************/


/**********************************************************************************************************************
 *  PRIVATE SECTION
 *********************************************************************************************************************/
#if (!defined FEE_PRIVATE_CFG_H)
# define FEE_PRIVATE_CFG_H

  /****************************************************************************
   * VERSION IDENTIFICATION
   ***************************************************************************/
# define FEE_PRIVATE_CFG_MAJOR_VERSION                                (8u)
# define FEE_PRIVATE_CFG_MINOR_VERSION                                (0u)
# define FEE_PRIVATE_CFG_PATCH_VERSION                                (0u)

  /****************************************************************************
   * Data consistency 
   ***************************************************************************/
#include "SchM_Fee.h"

#define Fee_GlobalSuspend() SchM_Enter_Fee_FEE_EXCLUSIVE_AREA_0() 
#define Fee_GlobalRestore() SchM_Exit_Fee_FEE_EXCLUSIVE_AREA_0() 


  /****************************************************************************
   * Configurations for Development mode
   ***************************************************************************/
# if (STD_ON == FEE_DEBUG_REPORTING)
#  define Fee_Errorhook(Fee_FunctionId, Fee_ErrorCode) \
      Det_ReportError((FEE_MODULE_ID), (FEE_INSTANCE_ID), (Fee_FunctionId), (Fee_ErrorCode))   /* PRQA S 3453 *//* MD_MSR_19.7 */

     #include "Det.h"
#else 
#  define Fee_Errorhook(Fee_FunctionId, Fee_ErrorCode)
# endif


  /*******************************************************************************************************************
   *  GLOBAL CONSTANT MACROS
   ******************************************************************************************************************/
   
# define FEE_INTERNAL_BUFFER_SIZE                        			  (128U)
# define FEE_USE_JOB_CACHE                               			  (STD_OFF)
   
  /****************************************************************************
   * LOWER LAYER CONFIGURATION PARAMETERS
   ***************************************************************************/
# define FEE_POLLING_MODE                              (STD_ON)

# define Fee_FlsRead                                          Fls_Read
# define Fee_FlsWrite                                         Fls_Write
# define Fee_FlsErase                                         Fls_Erase
# define Fee_FlsCompare                                       Fls_Compare
# define Fee_FlsCancel                                        Fls_Cancel
# define Fee_FlsGetStatus                                     Fls_GetStatus
# define Fee_FlsGetJobResult                                  Fls_GetJobResult


  /****************************************************************************
   * UPPER LAYER CONFIGURATION PARAMETERS
   ***************************************************************************/
# define FEE_NVM_POLLING_MODE                          (STD_ON)

# if (STD_OFF == FEE_NVM_POLLING_MODE)
#  define Fee_NvMJobEndNotification()         			NULL_PTR()
#  define Fee_NvMJobErrorNotification()     			NULL_PTR()
# endif

  /****************************************************************************
   * Error Callback Notification
   ***************************************************************************/
# define FEE_USE_APPL_ERROR_CALLBACK            		(STD_ON)

# if (STD_ON == FEE_USE_APPL_ERROR_CALLBACK)
#  include "Fee_Callouts.h"
# endif

  /********************************************************************************************************************
   *  INCLUDES
   *******************************************************************************************************************/


#include "Fls.h" 
#include "Fls.h" 


# if (STD_OFF == FEE_NVM_POLLING_MODE)

# endif

#endif /* (!defined FEE_PRIVATE_CFG_H) */

/**********************************************************************************************************************
 *  END OF FILE: Fee_PrivateCfg.h
 *********************************************************************************************************************/
 
