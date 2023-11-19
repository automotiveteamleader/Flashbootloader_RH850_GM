/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *
 *                 This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                 Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                 All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  LICENSE
 *  -------------------------------------------------------------------------------------------------------------------
 *            Module: CanTrcv
 *           Program: GM Global B (MSR_Gm_SLP1)
 *          Customer: Nexteer Automotive Corporation
 *       Expiry Date: Not restricted
 *  Ordered Derivat.: Renesas RH850 P1M R7F701363
 *    License Scope : The usage is restricted to CBD1500636_D02
 *
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *              File: CanTrcv_GeneralTypes.h
 *   Generation Time: 2017-07-11 15:18:02
 *           Project: Tja1145 - Version 1.0
 *          Delivery: CBD1500636_D02
 *      Tool Version: DaVinci Configurator (beta) 5.14.11 SP1
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

#ifndef __CANTRCV_GENERALTYPES_H
#define __CANTRCV_GENERALTYPES_H

typedef enum CanTrcv_TrcvModeTypeTag 
{
  CANTRCV_TRCVMODE_NORMAL = 0u,
  CANTRCV_TRCVMODE_SLEEP,
  CANTRCV_TRCVMODE_STANDBY
} CanTrcv_TrcvModeType;


typedef enum CanTrcv_TrcvWakeupModeTypeTag
{
  CANTRCV_WUMODE_ENABLE = 0u,
  CANTRCV_WUMODE_CLEAR,
  CANTRCV_WUMODE_DISABLE
} CanTrcv_TrcvWakeupModeType;

typedef enum CanTrcv_TrcvWakeupReasonTypeTag
{
  CANTRCV_WU_ERROR = 0u,
  CANTRCV_WU_BY_BUS,
  CANTRCV_WU_BY_PIN,
  CANTRCV_WU_INTERNALLY,
  CANTRCV_WU_NOT_SUPPORTED,
  CANTRCV_WU_POWER_ON,
  CANTRCV_WU_RESET,
  CANTRCV_WU_BY_SYSERR
} CanTrcv_TrcvWakeupReasonType;

#endif


