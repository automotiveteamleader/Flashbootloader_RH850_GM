/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief        OEM-specific wrapper for FEE access
 *  -------------------------------------------------------------------------------------------------------------------
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \par Copyright
 *  \verbatim
 *  Copyright (c) 2016 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 */
/*********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Marco Riedl                   Rie           Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  02.00.00   2016-02-05  Rie     -                Initial version
 *  02.01.00   2016-03-11  Rie     -                Fixed macros/defines for EepM use-case
 *                                 -                Update to new interface
 *  02.02.00   2016-07-05  Rie     ESCAN00090830    Added support for no NV driver use-case
 *  02.02.01   2016-09-06  Rie     ESCAN00091757    Fixed MISRA findings
 *  02.02.02   2016-10-17  Rie     ESCAN00092351    Configure used FEE functions
 *********************************************************************************************************************/

#ifndef __WRAPNV_CFG_H__
#define __WRAPNV_CFG_H__

/***********************************************************************************************************************
 *  CONFIGURATION SWITCHES
 **********************************************************************************************************************/

#if defined( WRAPNV_USECASE_FEE )    
#else
#  define WRAPNV_USECASE_FEE
#endif

#if defined( WRAPNV_USECASE_FEE )
/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

# include "Std_Types.h"
# include "Fls.h"
# include "Fee.h"

/**********************************************************************************************************************
 *  CONFIGURATION DEFINES
 *********************************************************************************************************************/

# if defined( WRAPNV_USECASE_FEE )
/** Function called in synchronous API while waiting for the job to be finished */
#  define WRAPNV_POLLING_FCT()             (void)FblRealTimeSupport()
/** Configure whether the Fee/Fls should be initialized during WrapNv initialization */
#  define WRAPNV_ENABLE_FEE_INITIALIZATION
/** Prefix of a NV block */
#  define WRAPNV_BLOCK_PREFIX(value)       FeeConf_FeeBlockConfiguration_ ## value /* PRQA S 0342 */ /* MD_WrapNv_0342 */
# endif /* WRAPNV_USECASE_FEE */
/** Maximum size of all Fee/NvM blocks */
# define WRAPNV_MAX_BLOCK_LENGTH           0x100u

/** FLS function abstraction */
# define WRAPNV_FLS_INIT                   Fls_Init
# define WRAPNV_FLS_GETSTATUS              Fls_GetStatus
# define WRAPNV_FLS_MAINFUNCTION           Fls_MainFunction
/** FLS configuration set used by NV-Wrapper */
# define WRAPNV_FLS_INIT_PARAMETER         FlsConfigSet

/** FEE function abstraction */
# define WRAPNV_FEE_INIT                   Fee_Init
# define WRAPNV_FEE_GETSTATUS              Fee_GetStatus
# define WRAPNV_FEE_GETJOBRESULT           Fee_GetJobResult
# define WRAPNV_FEE_MAINFUNCTION           Fee_MainFunction
# define WRAPNV_FEE_READ                   Fee_Read
# define WRAPNV_FEE_WRITE                  Fee_Write
# define WRAPNV_FEE_INVALIDATEBLOCK        Fee_InvalidateBlock
#endif

/***********************************************************************************************************************
 *  COMPATIBILITY DEFINES
 **********************************************************************************************************************/

#if defined( WRAPNV_USECASE_FEE )

/* TODO: add configuration for FEE/NVM */
/* Below is an example */

/**********************************************************************************************************************
 *  DEFINES
 *********************************************************************************************************************/

/*  Memory Layout
*   |
*   +-- ResetFlags
*       |
*       +-- ProgReqFlag                                           Reprogramming request flag
*       |
*       +-- ResetResponseFlag                                     Reset response flag
*/

/* Reset flags */
#define kEepSizeProgReqFlag                  0x01u
#define kEepSizeResetResponseFlag            0x01u
#define kEepSizeResetFlags                   (  kEepSizeProgReqFlag              \
                                              + kEepSizeResetResponseFlag)

/* Reset flags */
#define WRAPNV_DATASET_RESETFLAGS                    1u
#define WRAPNV_RECORD_ID_PROGREQFLAG                 0u
#define WRAPNV_RECORD_ID_RESETRESPONSEFLAG           1u

/***********************************************************************************************************************
 *  TYPEDEFS
 **********************************************************************************************************************/

/** Specifies how the NV block is configured */
typedef enum
{
   WRAPNV_RECORDACCESS_STRUCTURED = 0u,      /**< NV block consists of several elements */
   WRAPNV_RECORDACCESS_SINGLE                /**< NV block consists of a single element */
} tWrapNvRecordAccess;

/** NV block configuration strucutre */
typedef struct
{
   tWrapNvRecordAccess blockDataAccess;      /**< Access variat to the NV block */
   uint16 blockNumber;                       /**< Number of NV block */
   uint16 blockLength;                       /**< Length of NV block */
   uint16 blockMaxDatasets;                  /**< Maximum datasets alloweed for the NV block */
   P2CONST(uint8, AUTOMATIC, WRAPNV_CONST) blockDefaultValue;           /**< Default value for NV block */
} tWrapNvBlockTbl;

/** NV record configuration structure */
typedef struct
{
   uint16 recordDataOffset;                  /**< Offset in NV block to the data */
   uint16 recordDataLength;                  /**< Length of data inside the NV block */
   P2CONST(tWrapNvBlockTbl, AUTOMATIC, WRAPNV_CONST) recordBlockPtr;    /**< Pointer to the NV block */
} tWrapNvRecordTbl;

/***********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 **********************************************************************************************************************/

extern CONST(tWrapNvRecordTbl, WRAPNV_CONST) kWrapNvRecordTbl[];

/***********************************************************************************************************************
 *  ACCESS MACROS
 **********************************************************************************************************************/

/* Reset flags */
#define ApplFblNvReadProgReqFlag(idx, buf)                        WrapNv_ReadSync(WRAPNV_RECORD_ID_PROGREQFLAG, (idx), (buf))
#define ApplFblNvWriteProgReqFlag(idx, buf)                       WrapNv_WriteSync(WRAPNV_RECORD_ID_PROGREQFLAG, (idx), (buf))
#define ApplFblNvReadAsyncProgReqFlag(idx, buf, op)               WrapNv_ReadAsync(WRAPNV_RECORD_ID_PROGREQFLAG, (idx), (buf), (op))
#define ApplFblNvWriteAsyncProgReqFlag(idx, buf, op)              WrapNv_WriteAsync(WRAPNV_RECORD_ID_PROGREQFLAG, (idx), (buf), (op))

#define ApplFblNvReadResetResponseFlag(idx, buf)                  WrapNv_ReadSync(WRAPNV_RECORD_ID_RESETRESPONSEFLAG, (idx), (buf))
#define ApplFblNvWriteResetResponseFlag(idx, buf)                 WrapNv_WriteSync(WRAPNV_RECORD_ID_RESETRESPONSEFLAG, (idx), (buf))
#define ApplFblNvReadAsyncResetResponseFlag(idx, buf, op)         WrapNv_ReadAsync(WRAPNV_RECORD_ID_RESETRESPONSEFLAG, (idx), (buf), (op))
#define ApplFblNvWriteAsyncResetResponseFlag(idx, buf, op)        WrapNv_WriteAsync(WRAPNV_RECORD_ID_RESETRESPONSEFLAG, (idx), (buf), (op))

#endif /* WRAPNV_USECASE_NONE */

#endif /* __WRAPNV_CFG_H__ */

/***********************************************************************************************************************
 *  END OF FILE: WRAPNV_CFG.H
 **********************************************************************************************************************/
