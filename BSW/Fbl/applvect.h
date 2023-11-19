/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief         This file contains the start address of the application software. It is usually linked and
 *                 compiled with the Bootloader and is located OUTSIDE the protected area of the FBL. The start
 *                 address has to be overwritten by the downloaded application software.
 *
 *                 Please note that there is no interrupt indirection on the Renesas RH850 platform. Please use the
 *                 appropriate setting (e.g. INTBP, interrupt base pointer) to redirect interrupts to
 *                 your application software.
 *                 This file must be compiled and linked in the Bootloader and the application software to the
 *                 same memory location.
 *
 *  \note          Please note, that this file contains example source code used by the Flash Bootloader. These
 *                 functions may influence the behavior of the bootloader in principle. Therefore, great care must
 *                 be taken to verify the correctness of the implementation. The contents of the originally delivered
 *                 files are only examples resp. implementation proposals. With regard to the fact that these
 *                 functions are meant for demonstration purposes only, Vector Informatik's liability shall be
 *                 expressly excluded in cases of ordinary negligence, to the extent admissible by law or statute.
 *
 *  --------------------------------------------------------------------------------------------------------------------
 *  COPYRIGHT
 *  --------------------------------------------------------------------------------------------------------------------
 *  \par Copyright
 *  \verbatim
 *  Copyright (c) 2017 by Vector Informatik GmbH.                                                  All rights reserved.
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
 *  Christian Baeuerle            CB            Vector Informatik GmbH
 *  Quetty Palacios               QPs           Vector Informatik GmbH
 *  Fadie Ghraib                  FG            Vector CANtech, Inc.
 *  Daniel Koebel                 DKl           Vector Informatik GmbH
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  01.00.00   2013-06-04  CB                       Creation
 *  01.01.00   2014-05-22  CB      ESCAN00075842    Added support for WindRiver / DiabData compiler
 *  01.02.00   2015-05-06  FG      ESCAN00081479    No changes
 *                         QPs     ESCAN00082781    Added support for Renesas compiler
 *  01.03.00   2017-01-11  DKl     ESCAN00093522    Use V_COMP switches
 **********************************************************************************************************************/

#ifndef __APPL_VECT_H__
#define __APPL_VECT_H__

/***********************************************************************************************************************
 *  VERSION
 **********************************************************************************************************************/

/* ##V_CFG_MANAGEMENT ##CQProject : FblVtabAppl_Rh850 CQComponent : Implementation */
#define FBLVTABAPPL_RH850_VERSION          0x0103
#define FBLVTABAPPL_RH850_RELEASE_VERSION  0x00

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

#define APPLVECT_FROM_BOOT       0xA5000000UL                             /* Reserved for Bootloader internal value   */
                                                                          /* Set this magic code just for the table   */
                                                                          /* linked with the bootloade, but not       */
                                                                          /* for the application vector table,  r     */

#define APPLVECT_FROM_APPL       0xAA000000UL                             /* Set this label for the user              */
                                                                          /* Application vector table only            */

/***********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

#define FBL_APPLVECT_START_SEC_CONST
# include "MemMap.h"

V_MEMROM0 extern V_MEMROM1 vuint32 V_MEMROM2 ApplIntJmpTable[2];

#define FBL_APPLVECT_STOP_SEC_CONST
#include "MemMap.h"

#endif   /* __APPL_VECT_H__ */

/***********************************************************************************************************************
 *  END OF FILE: APPLVECT.H
 **********************************************************************************************************************/
