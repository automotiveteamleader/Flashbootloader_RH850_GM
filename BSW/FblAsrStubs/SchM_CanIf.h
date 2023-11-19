/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief SchM_CanIf ASR4 stub for flash bootloader
 *
 *  --------------------------------------------------------------------------------------------------------------------
 *  COPYRIGHT
 *  --------------------------------------------------------------------------------------------------------------------
 *  \par Copyright
 *  \verbatim
 *  Copyright (c) 2016 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 */
/***********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  --------------------------------------------------------------------------------------------------------------------
 *  Marco Riedl                   Rie           Vector Informatik GmbH
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  04.01.00   2016-02-04  Rie     -                Initial
 **********************************************************************************************************************/

#ifndef SCHM_CANIF_H
#define SCHM_CANIF_H

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

#include "fbl_inc.h"

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

#if defined( FBL_ENABLE_COM_RX_INTERRUPT )
# define SchM_Enter_CanIf_CANIF_EXCLUSIVE_AREA_0()   FblInterruptDisable()
# define SchM_Exit_CanIf_CANIF_EXCLUSIVE_AREA_0()    FblInterruptEnable()
# define SchM_Enter_CanIf_CANIF_EXCLUSIVE_AREA_1()   FblInterruptDisable()
# define SchM_Exit_CanIf_CANIF_EXCLUSIVE_AREA_1()    FblInterruptEnable()
# define SchM_Enter_CanIf_CANIF_EXCLUSIVE_AREA_2()   FblInterruptDisable()
# define SchM_Exit_CanIf_CANIF_EXCLUSIVE_AREA_2()    FblInterruptEnable()
# define SchM_Enter_CanIf_CANIF_EXCLUSIVE_AREA_3()   FblInterruptDisable()
# define SchM_Exit_CanIf_CANIF_EXCLUSIVE_AREA_3()    FblInterruptEnable()
# define SchM_Enter_CanIf_CANIF_EXCLUSIVE_AREA_4()   FblInterruptDisable()
# define SchM_Exit_CanIf_CANIF_EXCLUSIVE_AREA_4()    FblInterruptEnable()
#else
# define SchM_Enter_CanIf_CANIF_EXCLUSIVE_AREA_0()
# define SchM_Exit_CanIf_CANIF_EXCLUSIVE_AREA_0()
# define SchM_Enter_CanIf_CANIF_EXCLUSIVE_AREA_1()
# define SchM_Exit_CanIf_CANIF_EXCLUSIVE_AREA_1()
# define SchM_Enter_CanIf_CANIF_EXCLUSIVE_AREA_2()
# define SchM_Exit_CanIf_CANIF_EXCLUSIVE_AREA_2()
# define SchM_Enter_CanIf_CANIF_EXCLUSIVE_AREA_3()
# define SchM_Exit_CanIf_CANIF_EXCLUSIVE_AREA_3()
# define SchM_Enter_CanIf_CANIF_EXCLUSIVE_AREA_4()
# define SchM_Exit_CanIf_CANIF_EXCLUSIVE_AREA_4()
#endif /* FBL_ENABLE_COM_RX_INTERRUPT */

#endif /* SCHM_CANIF_H */

/***********************************************************************************************************************
 *  END OF FILE: SCHM_CANIF.H
 **********************************************************************************************************************/
