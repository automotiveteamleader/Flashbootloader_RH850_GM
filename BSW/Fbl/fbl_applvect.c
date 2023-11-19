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
 *  01.02.00   2015-05-06  FG      ESCAN00081479    Added support for IAR compiler
 *                         QPs     ESCAN00082781    Added support for Renesas compiler
 *  01.02.00   2017-01-11  DKl     ESCAN00093522    Use V_COMP switches
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

#include "v_cfg.h"
#include "v_def.h"
#include "applvect.h"

/***********************************************************************************************************************
 *   VERSION
 **********************************************************************************************************************/

#if ( FBLVTABAPPL_RH850_VERSION != 0x0103 ) || \
    ( FBLVTABAPPL_RH850_RELEASE_VERSION != 0x00 )
# error "Error in applvect.c: Source and header file are inconsistent!"
#endif

/***********************************************************************************************************************
 *  PROTOTYPES
 **********************************************************************************************************************/

#if defined( V_COMP_GHS ) || \
    defined( V_COMP_DIABDATA )
extern void _usr_init( void ); /* PRQA S 3447 */ /* MD_FblVtabApplRh850_3447 */
#define RESET_ENTRY_POINT (vuint32)_usr_init
#endif

#if defined( V_COMP_IAR )
extern void _iar_program_start( void ); /* PRQA S 3447 */ /* MD_FblVtabApplRh850_3447 */
#define RESET_ENTRY_POINT (vuint32)_iar_program_start
#endif

#if defined( V_COMP_RENESAS )
extern void _start( void ); /* PRQA S 3447 */ /* MD_FblVtabApplRh850_3447 */
#define RESET_ENTRY_POINT (vuint32)_start
#endif

/***********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

#define FBL_APPLVECT_START_SEC_CONST
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

V_MEMROM0 V_MEMROM1 vuint32 V_MEMROM2 ApplIntJmpTable[2] = {APPLVECT_FROM_BOOT, RESET_ENTRY_POINT}; /* PRQA S 0305, 0685, 0428 */ /* MD_FblVtabApplRh850_JmpTable */

#define FBL_APPLVECT_STOP_SEC_CONST
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* Module specific MISRA deviations:

MD_FblVtabApplRh850_3447:
   Reason: The function is declared in the startup code.
   Risk: Prototypes could be unmatched.
   Prevention: no prevention required.

 MD_FblVtabApplRh850_JmpTable:
   Reason: Casting to integral type in order to allow storage in of entry address in ApplIntJmpTable.
   Risk: Creates non portable software.
   Prevention: This is only used in hw-dependent file here.

*/

/***********************************************************************************************************************
 *  END OF FILE: APPLVECT.C
 **********************************************************************************************************************/
