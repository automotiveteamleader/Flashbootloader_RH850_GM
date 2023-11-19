/*============================================================================*/
/* Project      = AUTOSAR Renesas X1x MCAL Components                         */
/* Module       = SchM_Fls.h                                                  */
/*============================================================================*/
/*                                  COPYRIGHT                                 */
/*============================================================================*/
/* Copyright(c) 2013-2015 Renesas Electronics Corporation                     */
/*============================================================================*/
/* Purpose:                                                                   */
/* Header file information for application.                                   */
/*                                                                            */
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
 * V1.0.0:  21-Mar-2013  : Initial version
 *
 * V1.0.1:  07-Aug-2013  : As per CR 225, Copyright and Device name are updated
 *                         as part of merge activity.
 *
 * V1.0.2:  01-Apr-2014  : As per CR 425, following changes are made:
 *                         1. Copyright information is updated.
 *                         2. Unused defines are removed.
 *
 * V1.0.3:  15-May-2015  : As part of P1x V4.00.04 release, following changes
 *                         are made:
 *                         1. As per mantis #28186, section
 *                            DRIVERSTATE_DATA_PROTECTION is renamed to
 *                            FLS_DRIVERSTATE_DATA_PROTECTION.
 *                         2. Updated file version and copyright information.
 */
/******************************************************************************/
#ifndef SCHM_FLS_H
#define SCHM_FLS_H

/*******************************************************************************
**                      Include Section                                       **
*******************************************************************************/
#include "Std_Types.h"
/*******************************************************************************
**                      Version Information                                  **
*******************************************************************************/

/*******************************************************************************
**                      Global Symbols                                        **
*******************************************************************************/

/*******************************************************************************
**                      Global Data Types                                     **
*******************************************************************************/

/*******************************************************************************
**                      Function Prototypes                                   **
*******************************************************************************/

/*
 * This type define the exclusive areas along with scheduler services are used
 * to provide data integrity for shared resources
 */
extern void SchM_Enter_Fls_FLS_DRIVERSTATE_DATA_PROTECTION(void);
extern void SchM_Exit_Fls_FLS_DRIVERSTATE_DATA_PROTECTION(void);

#endif /* SCHM_FLS_H */

/*******************************************************************************
**                      End of File                                           **
*******************************************************************************/
