/*============================================================================*/
/* Project      = AUTOSAR Renesas X1x MCAL Components                         */
/* Module       = Os.h                                                        */
/*============================================================================*/
/*                                  COPYRIGHT                                 */
/*============================================================================*/
/* Copyright(c) 2013 Renesas Electronics Corporation                          */
/*============================================================================*/
/* Purpose:                                                                   */
/* This file is a stub for OS component                                       */
/*============================================================================*/
/*                                                                            */
/* Unless otherwise agreed upon in writing between your company and           */
/* Renesas Electronics Corporation the following shall apply!                 */
/*                                                                            */
/* Warranty Disclaimer                                                        */
/*                                                                            */
/* There is no warranty of any kind whatsoever granted by Renesas. Any        */
/* warranty is expressly disclaimed and excluded by Renesas, either expressed */
/* or implied, including but not limited to those for non-infringement of     */
/* intellectual property, merchantability and/or fitness for the particular   */
/* purpose.                                                                   */
/*                                                                            */
/* Renesas shall not have any obligation to maintain, service or provide bug  */
/* fixes for the supplied Product(s) and/or the Application.                  */
/*                                                                            */
/* Each User is solely responsible for determining the appropriateness of     */
/* using the Product(s) and assumes all risks associated with its exercise    */
/* of rights under this Agreement, including, but not limited to the risks    */
/* and costs of program errors, compliance with applicable laws, damage to    */
/* or loss of data, programs or equipment, and unavailability or              */
/* interruption of operations.                                                */
/*                                                                            */
/* Limitation of Liability                                                    */
/*                                                                            */
/* In no event shall Renesas be liable to the User for any incidental,        */
/* consequential, indirect, or punitive damage (including but not limited     */
/* to lost profits) regardless of whether such liability is based on breach   */
/* of contract, tort, strict liability, breach of warranties, failure of      */
/* essential purpose or otherwise and even if advised of the possibility of   */
/* such damages. Renesas shall not be liable for any services or products     */
/* provided by third party vendors, developers or consultants identified or   */
/* referred to the User by Renesas in connection with the Product(s) and/or   */
/* the Application.                                                           */
/*                                                                            */
/*============================================================================*/
/* Environment:                                                               */
/*              Devices:        X1x                                           */
/*============================================================================*/

/*******************************************************************************
**                      Revision Control History                              **
*******************************************************************************/
/*
 * V1.0.0:  17-Jun-2013  : Initial Version
 * V1.0.1:  09-Dec-2013  : As per CR 225, Copyright and Device name are updated  
 *                         as part of merge activity and u is appended to 
 *                         definition of OsCounter0 to specify unsigned type.
 */
/*******************************************************************************
**                         Input File                                         **
*******************************************************************************/
#ifndef OS_H
#define OS_H

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "Std_Types.h"
/*******************************************************************************
**                      Version Information                                   **
*******************************************************************************/
/* AUTOSAR release version information */
#define OS_AR_RELEASE_MAJOR_VERSION    4
#define OS_AR_RELEASE_MINOR_VERSION    0
#define OS_AR_RELEASE_REVISION_VERSION 3

/* Module Software version information */
#define OS_SW_MAJOR_VERSION    4
#define OS_SW_MINOR_VERSION    0
#define OS_SW_PATCH_VERSION    0
/*******************************************************************************
**                      Macro                                                 **
*******************************************************************************/
#define ISR(X) void OS_ISR_##X(void)
/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/
#define OsCounter0    (uint8)0u
#define ETH_OS_COUNTER_ID 1
#define ETH_OS_COUNTER_MAX 1000
/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/
/* OS ID Type */
typedef uint8 CounterType;
/* OS Tick reference type */
typedef uint32 TickType;
typedef P2VAR(TickType, AUTOMATIC, OS_APPL_DATA) TickRefType;

/*******************************************************************************
**                      Function Prototypes                                   **
*******************************************************************************/

extern StatusType GetCounterValue( CounterType CounterID, TickRefType Value );
#endif /* OS_H */
/*******************************************************************************
**                      End of File                                           **
*******************************************************************************/
