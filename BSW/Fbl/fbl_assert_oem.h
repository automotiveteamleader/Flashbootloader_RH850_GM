/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief        OEM specific assertion defines (GM)
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
 *  Andreas Wenckebach            AWh           Vector Informatik GmbH
 *  Dennis O'Donnell              Dod           Vector CANtech, Inc.
 *  Ralf Haegenlaeuer             HRf           Vector Informatik GmbH
 *  Sebastian Loos                Shs           Vector Informatik GmbH
 *  Wei Yang                      Wyg           Vector Informatik GmbH
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  01.00.00   2014-06-03  AWh     -                Creation
 *  01.00.01   2014-07-10  AWh     -                Removed duplicates to fbl_assert.h
 *  03.01.00   2015-06-12  AWh     -                Changed version to match component version
 *  03.02.00   2015-07-23  AWh     -                No changes
 *  03.03.00   2015-12-04  AWh     ESCAN00085579    No changes
 *                                 ESCAN00085914    No changes
 *  03.04.00   2016-02-29  HRf     ESCAN00088540    No changes
 *                                 ESCAN00088611    No changes
 *  03.05.00   2016-03-16  Shs     ESCAN00088916    No changes
 *                                 ESCAN00089094    No changes
 *  04.00.00   2016-05-11  AWh     ESCAN00089529    No changes
 *  04.01.00   2016-11-10  AWh     ESCAN00090256    No changes
 *                                 ESCAN00090901    No changes
 *                                 ESCAN00091143    No changes
 *                                 ESCAN00091409    No changes
 *                                 ESCAN00092312    No changes
 *                         Dod     ESCAN00092740    No changes
 *  04.01.01   2016-11-12  AWh     ESCAN00092772    No changes
 *                                 ESCAN00092793    No changes
 *  04.01.02   2016-11-22  AWh     ESCAN00092937    No changes
 *  04.01.03   2016-12-14  Shs     ESCAN00093225    No changes
 *                                 ESCAN00093243    No changes
 *                         AWh     ESCAN00093284    No changes
 *  04.01.04   2017-01-26  AWh     ESCAN00093739    No changes
 *                                 ESCAN00093934    No changes
 *                                 ESCAN00093935    No changes
 *  04.02.00   2017-03-07  Wyg     ESCAN00094278    No changes
 *  04.02.01   2017-03-20  AWh     ESCAN00094415    No changes
 *                         AWh     ESCAN00093957    No changes
 **********************************************************************************************************************/

#ifndef __FBL_OEM_ASSERT_H__
#define __FBL_OEM_ASSERT_H__

/***********************************************************************************************************************
 *  DEFINITION OF ASSERTION CODES (OEM)
 **********************************************************************************************************************/

/* Note: OEM assertions are only allowed in the range from 0x80-0xFF. Others are reserved. */

/* 0x80-0x9F: General FBL assertions */
#define kFblAssertVariableInValidState             0x80u
#define kFblAssertParameterValid                   0x81u
#define kFblAssertConditionCorrect                 0x82u
#define kFblAssertWrongServiceHandler              0x83u
#define kFblOemAssertPreHandlerIdxInvalid          0x84u
#define kFblOemAssertPostHandlerIdxInvalid         0x85u
#define kFblOemAssertMainHandlerIdxInvalid         0x86u

/* 0xA0-0xBF: Internal assertions */
#define kFblAssertInternalIlegalState              0xA0u
#define kFblAssertInternalApplFblCorrectErrSetting 0xA1u

/* 0xC0-0xDF: Generated data assertions */
#define kFblAssertAppl1NotFirstBlock               0xC0u
#define kFblAssertCorrectGenyConfig                0xC1u

/* 0xE0-0xFF: User parameter assertions */
#define kFblAssertApplFblGetMdlHdrAddrCfgrd        0xE0u

#endif /* __FBL_OEM_ASSERT_H__ */
/***********************************************************************************************************************
 *  END OF FILE: FBL_ASSERT_OEM.H
 **********************************************************************************************************************/
