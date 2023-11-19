/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief        Main module of FBL updater - Type definitions
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
 *  Joern Herwig                  JHg           Vector Informatik GmbH
 *  Daniel Moessner               DlM           Vector Informatik GmbH
 *  Marco Riedl                   Rie           Vector Informatik GmbH
 *  Sebastian Loos                Shs           Vector Informatik GmbH
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  03.00.00   2016-07-22  JHg     ESCAN00091120    Major rework
 *                                                   Renamed component to FblUpd_Main
 *                                                   Moved callouts to separate components
 *  03.00.01   2016-08-05  DlM     ESCAN00091311    No changes
 *  03.01.00   2016-08-18  Rie     ESCAN00091520    No changes
 *  03.01.01   2016-09-09  JHg     ESCAN00091830    No changes
 *  03.02.00   2016-10-14  DlM     ESCAN00092338    No changes
 *  03.02.01   2016-12-07  Shs     ESCAN00093203    No changes
 *  03.02.03   2017-02-07  JHg     ESCAN00093818    Removed include of fbl_cfg.h (already done in fbl_def.h)
 *  03.02.04   2017-04-17  Shs     ESCAN00094760    No changes
 **********************************************************************************************************************/

#ifndef __UPD_TYPES_H__
#define __UPD_TYPES_H__

/***********************************************************************************************************************
*  INCLUDES
**********************************************************************************************************************/

/* Required for FBL base types */
#include "v_cfg.h"
#include "v_def.h"
#include "fbl_def.h"

/***********************************************************************************************************************
 *  TYPEDEFS
 **********************************************************************************************************************/

/** Segment information */
typedef struct
{
   tFblAddress    source;     /**< Address of source data (e.g. constant buffer) */
   tFblAddress    target;     /**< Target address */
   tFblLength     length;     /**< Length of segment */
} tFblUpdSegmentInfo;

/** Update hook function pointer */
typedef tFblResult (*tFblUpdFunc)( void );
/** Segment adjustment hook function pointer */
typedef tFblResult (* tFblUpAdjustSegment)( V_MEMRAM1 tFblUpdSegmentInfo V_MEMRAM2 V_MEMRAM3 * pSegmentList,
   V_MEMRAM1 vuintx V_MEMRAM2 V_MEMRAM3 * pSegmentCount );

#endif /* __UPD_TYPES_H__ */

/***********************************************************************************************************************
 *  END OF FILE: UPD_TYPES.H
 **********************************************************************************************************************/
