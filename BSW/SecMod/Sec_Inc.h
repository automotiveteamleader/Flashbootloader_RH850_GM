/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief        Implementation of the HIS security module - Common includes
 *
 *  \description  Contains all includes used by all security module sources
 *                Not to be included in other sources directly, include Sec.h instead
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
 *                                                   Renamed file name (was SecM_inc.h)
 *                                                   Conditional inclusion of FBL configuration
 *                                                   Removed API includes
 *  02.01.00   2013-11-08  JHg     ESCAN00071173    Only include configuration and parameters for CRC or verification API
 *  02.01.01   2013-11-08  JHg     ESCAN00071733    No changes
 *  02.01.02   2013-11-11  JHg     ESCAN00071785    Unconditionally include configuration and parameter settings
 *  02.02.00   2013-12-19  Mss     ESCAN00072743    No changes
 *  02.03.00   2014-02-20  JHg     ESCAN00073562    No changes
 *                         Mss     ESCAN00073600    No changes
 *                         Mss     ESCAN00073811    No changes
 *  02.04.00   2015-02-09  CB      ESCAN00079848    No changes
 *  02.04.01   2015-03-10  Rie     ESCAN00081703    No changes
 *  02.04.02   2015-07-29  AWh     ESCAN00084125    Beautification
 *  02.05.00   2015-10-12  JHg     ESCAN00085810    No changes
 *  02.05.01   2015-10-28  JHg     ESCAN00086114    No changes
 *  02.06.00   2015-11-11  JHg     ESCAN00086123    No changes
 *                         JHg     ESCAN00086125    No changes
 *                         JHg     ESCAN00086239    No changes
 **********************************************************************************************************************/

#ifndef __SEC_INC_H__  /* PRQA S 0883 */ /* MD_SecInc_0883 */
#define __SEC_INC_H__

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

/* Global configuration */
#include "v_cfg.h"

/* Global definitions */
#include "v_def.h"

#if defined( VGEN_ENABLE_CANFBL )
/* Bootloader types and definitions (e.g. SWM_DATA_MAX_NOAR) */
# include "fbl_cfg.h"
#endif /* VGEN_ENABLE_CANFBL */

/* Security module configuration */
# include "SecM_cfg.h"

/* Additional header files */
#include "Sec_Types.h"

/* Security module parameters */
# include "SecMPar.h"

#endif /* __SEC_INC_H__ */

/*
   MD_SecInc_0883:
      Reason: Sec_Types.h and SecmPar.h include fbl_inc.h. This should not be a problem usually, because fbl_inc.h is protected against multiple inclusion.
              However, QAC still produces a warning that no multiple inclusion protection is in place. This is a false warning.
      Risk: No risk
      Prevention: No prevention.
*/

/***********************************************************************************************************************
 *  END OF FILE: SEC_INC.H
 **********************************************************************************************************************/
