/*============================================================================*/
/* Project      = AUTOSAR Renesas X1x MCAL Components                         */
/* Module       = fdl_descriptor.h                                            */
/*============================================================================*/
/*                                  COPYRIGHT                                 */
/*============================================================================*/
/* Copyright(c) 2012-2014 Renesas Electronics Corporation                     */
/*============================================================================*/
/* Purpose:                                                                   */
/* This file contains FDL run-time configuration descriptor variable related  */
/* defines.                                                                   */
/* Functions.                                                                 */
/* AUTOMATICALLY GENERATED FILE - DO NOT EDIT                                 */
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
 * V1.0.0:  05-Dec-2012 : Initial version
 *
 * V1.0.1:  26-Apr-2013 : As per SCR 147, Macro CPU_FREQUENCY_MHZ is renamed
 *                        to FDL_CPU_FREQUENCY_MHZ.
 * V1.0.2:  08-Aug-2013 : As per CR 225, F1x is renamed to X1x.
 * V1.0.3:  10-Jan-2014 : As per CR 066, following changes are made:
 *                        1. FDL descriptor variable is renamed 
 *                        eelApp_fdlConfig_enu to sampleApp_fdlConfig_enu.
 *                        2. COPYRIGHT information is updated.
 * V1.0.4:  23-Aug-2014 : As per CR 594 and mantis #22252, following change 
 *                        is made:
 *                        1. Changes are made at all relevant places
 *                        accordingly to append U or UL after numeric values.
 * V1.0.5:  04-Nov-2014 : As per CR 643 and mantis #24449, following change
 *                        is made:
 *                        1. Macro AUTHENTICATION_ID is removed.
 */
/******************************************************************************/
 
/*******************************************************************************
**                       Generation Tool Version                              **
*******************************************************************************/
/*
 * TOOL VERSION:    1.3.6
 */
/*******************************************************************************
**                         Input File                                         **
*******************************************************************************/

/*
 * INPUT FILE:    D:\usr\usage\Delivery\CBD15x\CBD1500635\D00\external\AUTOSAR\ThirdParty\Mcal_Rh850P1x\Supply\AUTOSAR_RH850_P1x_MCAL_Ver4.00.04\X1X\P1x\modules\Fls\generator\R403_Fls_P1x_BSWMDT.arxml
 *                C:\Users\vadswe\AppData\Local\Temp\Cfg_Gen-1457661614673-8\AutosarFiles_ValGen-1457662302516-0\ExtGen_DrvFls_ECUC_3214324211510032274.arxml
 * GENERATED ON:  10 Mar 2016 - 21:11:49
 */

#ifndef FDL_DESCRIPTOR_H
#define FDL_DESCRIPTOR_H


#define EEL_POOL_START          0U     /**< 1st block of the EEL pool */
#define EEL_POOL_SIZE           0U     /**< no. of blocks for the EEL pool */

/* The Flash programming hardware is provided with a 
  clock, derived from the CPU subsystem frequency. */
#define FDL_CPU_FREQUENCY_MHZ     160U

/* Number of Data Flash blocks, accessible by 
  the FDL. Typically it is the complete no. of available 
  Data Flash blocks */
#define FDL_POOL_SIZE     1024U



/*****************************************************
*  Descriptor variable declaration (FDL descriptor variable)
******************************************************/
extern const r_fdl_descriptor_t sampleApp_fdlConfig_enu;   


#endif  /* #ifndef FDL_DESCRIPTOR_H */

/*******************************************************************************
**                      End of File                                           **
*******************************************************************************/
