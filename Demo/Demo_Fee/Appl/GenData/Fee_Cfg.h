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
 *              File: Fee_Cfg.h
 *   Generation Time: 2016-05-21 12:46:53
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
 *  PUBLIC SECTION
 *********************************************************************************************************************/
#if !defined (FEE_CFG_H_PUBLIC)
# define FEE_CFG_H_PUBLIC

  /********************************************************************************************************************
   *  GLOBAL CONSTANT MACROS
   *******************************************************************************************************************/
  /****************************************************************************
   * VERSION IDENTIFICATION
   ***************************************************************************/
# define FEE_CFG_MAJOR_VERSION                    (8u)
# define FEE_CFG_MINOR_VERSION                    (0u)
# define FEE_CFG_PATCH_VERSION                    (0u)

  /****************************************************************************
   * API CONFIGURATION
   ***************************************************************************/
# define FEE_VERSION_INFO_API                     (STD_OFF)
# define FEE_GET_ERASE_CYCLE_API                  (STD_OFF)
# define FEE_GET_WRITE_CYCLE_API                  (STD_OFF)
# define FEE_FORCE_SECTOR_SWITCH_API              (STD_OFF)
# define FEE_FSS_CONTROL_API              		  (STD_OFF)

# define FEE_DATA_CONVERSION_API                  (STD_OFF)

  /****************************************************************************
   * DEVELOPMENT CONFIGURATION
   ***************************************************************************/
# define FEE_DEV_ERROR_DETECT                     (STD_ON)
# define FEE_CFG_CHK_PARAM_BLOCK_NUMBER           (STD_ON)

# if (STD_ON == FEE_DEV_ERROR_DETECT)

#  define FEE_CFG_CHK_STATUS_UNINIT               (STD_ON)
#  define FEE_CFG_CHK_BUSY                        (STD_ON)
#  define FEE_CFG_CHK_IMMEDIATE_DATA              (STD_ON)
#  define FEE_CFG_DO_PARAM_CHK                    (STD_ON)

#  if (STD_ON == FEE_CFG_DO_PARAM_CHK)
#   define FEE_CFG_CHK_PARAM_SECTOR_NUMBER        (STD_ON)
#   define FEE_CFG_CHK_PARAM_NULL_POINTER         (STD_ON)
#   define FEE_CFG_CHK_PARAM_LENGTH_OFFSET        (STD_ON)
#   define FEE_CFG_CHK_PARAM_MODE                 (STD_ON)
#  else
#   define FEE_CFG_CHK_PARAM_SECTOR_NUMBER        (STD_OFF)
#   define FEE_CFG_CHK_PARAMR_NULL_POINTER        (STD_OFF)
#   define FEE_CFG_CHK_PARAM_LENGTH_OFFSET        (STD_OFF)
#   define FEE_CFG_CHK_PARAM_MODE                 (STD_OFF)
#  endif

#  define FEE_DEBUG_REPORTING                     (STD_ON)

# else

#  define FEE_CFG_CHK_STATUS_UNINIT                                   (STD_OFF)
#  define FEE_CFG_CHK_BUSY                                            (STD_OFF)
#  define FEE_CFG_CHK_IMMEDIATE_DATA                                  (STD_OFF)
#  define FEE_CFG_DO_PARAM_CHK                                        (STD_OFF)
#  define FEE_CFG_CHK_PARAM_SECTOR_NUMBER                             (STD_OFF)
#  define FEE_CFG_CHK_PARAM_NULL_POINTER                              (STD_OFF)
#  define FEE_CFG_CHK_PARAM_LENGTH_OFFSET                             (STD_OFF)
#  define FEE_CFG_CHK_PARAM_MODE                                      (STD_OFF)
#  define FEE_DEBUG_REPORTING                                         (STD_OFF)

# endif

  /****************************************************************************
   * GENERAL CONFIGURATION PARAMETER
   ***************************************************************************/

#define FeeConf_FeeBlockConfiguration_FeeConfigBlock (16UL) 
#define FeeConf_FeeBlockConfiguration_FeeEPS_DIAG_MON_NvmKeyNbid (14UL) 
#define FeeConf_FeeBlockConfiguration_FeeRte_NvmBlock_AppNbId (12UL) 
#define FeeConf_FeeBlockConfiguration_FeeEPS_DIAG_MON_NvmEcuId (10UL) 
#define FeeConf_FeeBlockConfiguration_FeeEPS_DIAG_MON_NvmSbat (8UL) 
#define FeeConf_FeeBlockConfiguration_FeeRte_NvmBlock_CmnMfgSrv_NxtrMfgEnaCntr (2UL) 
#define FeeConf_FeeBlockConfiguration_FeeRte_NvmBlock_PartNr_PartNrBasMdlPartNr (4UL) 
#define FeeConf_FeeBlockConfiguration_FeeRte_NvmBlock_PartNr_PartNrEndMdlPartNr (6UL) 


#define FEE0 (0U) 
#define FEE1 (1U) 


#define FEE_NUMBER_OF_PARTITIONS (2)

#endif /* FEE_CFG_H_PUBLIC */

/**********************************************************************************************************************
 *  END OF FILE: Fee_Cfg.h
 *********************************************************************************************************************/
 

