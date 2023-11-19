/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/** \file         WrapNv.c
 *  \brief        Wrapper for NV-memory access
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
 *  02.00.00   2016-02-22  Rie     ESCAN00088726    Initial version to support Fee/NvM
 *  02.01.00   2016-03-09  Rie     ESCAN00088817    Removed function call NvM_Errorhook
 *                                 ESCAN00088819    Abstraction of Fls interface
 *  02.02.00   2016-07-05  Rie     ESCAN00090830    Added support for no NV driver use-case
 *  02.02.01   2016-09-06  Rie     ESCAN00091757    Fixed MISRA findings
 *  02.02.02   2016-10-17  Rie     ESCAN00092351    Configure used FEE functions
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "WrapNv.h"

/***********************************************************************************************************************
 *   VERSION
 **********************************************************************************************************************/

#if ( SYSSERVICE_WRAPPERNV_VERSION != 0x0202u ) || \
    ( SYSSERVICE_WRAPPERNV_RELEASE_VERSION != 0x02u )
# error "Error in WrapNv.c: Source and header file are inconsistent!"
#endif

/**********************************************************************************************************************
 *  DEFINES
 *********************************************************************************************************************/

/** Default value, in case block/data is not available and explicit default value not given */
#if !defined( WRAPNV_DEFAULT_VALUE )
# define WRAPNV_DEFAULT_VALUE     0xFFu
#endif /* WRAPNV_DEFAULT_VALUE */

#if !defined( STATIC )
# define STATIC                  static
#endif

/***********************************************************************************************************************
 *  TYPEDEFS
 **********************************************************************************************************************/

typedef enum
{
   WRAPNV_STATE_IDLE = 0u,
   WRAPNV_STATE_READ_INIT,
   WRAPNV_STATE_READ_PENDING,
   WRAPNV_STATE_READ_MODIFY_INIT,
   WRAPNV_STATE_READ_MODIFY_PENDING,
   WRAPNV_STATE_MODIFY_DATA,
   WRAPNV_STATE_WRITE_INIT,
   WRAPNV_STATE_WRITE_PENDING,
   WRAPNV_STATE_DELETE_INIT,
   WRAPNV_STATE_DELETE_PENDING
} tWrapNvState;

/***********************************************************************************************************************
 *  LOCAL DATA
 **********************************************************************************************************************/

# define WRAPNV_START_SEC_VAR_NOINIT_UNSPECIFIED
# include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* NV block specific */
STATIC VAR(uint16, WRAPNV_VAR_NOINIT) gWrapNvBlockNumber;               /**< Number of block */
STATIC VAR(uint16, WRAPNV_VAR_NOINIT) gWrapNvBlockDataset;              /**< Dataset of block */
STATIC VAR(uint8, WRAPNV_VAR_NOINIT)  gWrapNvBlockBuffer[WRAPNV_MAX_BLOCK_LENGTH];  /**< Data buffer for whole NV block */ /* PRQA S 3218 */ /* MD_WrapNv_3218 */
STATIC P2CONST(uint8, AUTOMATIC, WRAPNV_CONST) gWrapNvBlockDefaultValue;   /**< Default value of a NV block, if invalid/not present */

/* Data specific */
STATIC VAR(uint16, WRAPNV_VAR_NOINIT) gWrapNvReadDataOffset;            /**< Offset in NV block to the data */
STATIC VAR(uint16, WRAPNV_VAR_NOINIT) gWrapNvReadDataLength;            /**< Length of data inside the NV block */
STATIC VAR(tWrapNvRamData, WRAPNV_VAR_NOINIT) gWrapNvReadDataBuffer;    /**< Input buffer from upper layer */
STATIC VAR(uint16, WRAPNV_VAR_NOINIT) gWrapNvWriteDataOffset;           /**< Offset in NV block to the data */
STATIC VAR(uint16, WRAPNV_VAR_NOINIT) gWrapNvWriteDataLength;           /**< Length of data inside the NV block */
STATIC VAR(tWrapNvConstData, WRAPNV_VAR_NOINIT) gWrapNvWriteDataBuffer; /**< Write buffer from upper layer */
STATIC VAR(tWrapNvRamData, WRAPNV_VAR_NOINIT) gWrapNvBlockAccessBuffer; /**< Buffer used to read and/or write data from/to the NV block */

STATIC VAR(tWrapNvState, WRAPNV_VAR_NOINIT) gWrapNvCurrentState;        /**< Current state of FEE/NVM access (read/modify/write) */

STATIC VAR(tWrapNvOpStatus, WRAPNV_VAR_NOINIT) gWrapNvInternalOpStatus; /**< Used to keep track if a request was canceled */

# define WRAPNV_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/***********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

# define WRAPNV_START_SEC_CODE
# include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

STATIC FUNC(MemIf_StatusType, WRAPNV_CODE) WrapNv_GetStatus( void );
STATIC FUNC(MemIf_JobResultType, WRAPNV_CODE) WrapNv_GetJobResult( void );
STATIC FUNC(WrapNv_ReturnType, WRAPNV_CODE) WrapNv_Read( uint16 id, uint16 idx, tWrapNvRamData buffer );
STATIC FUNC(WrapNv_ReturnType, WRAPNV_CODE) WrapNv_ReadPartial( uint16 id, uint16 idx, tWrapNvRamData buffer, uint16 offset, uint16 length );
STATIC FUNC(WrapNv_ReturnType, WRAPNV_CODE) WrapNv_Delete( uint16 id, uint16 idx );
STATIC FUNC(WrapNv_ReturnType, WRAPNV_CODE) WrapNv_Write( uint16 id, uint16 idx, tWrapNvConstData buffer );
STATIC FUNC(WrapNv_ReturnType, WRAPNV_CODE) WrapNv_MainFunction( void );

/***********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  WrapNv_GetStatus
 **********************************************************************************************************************/
/*! \brief       Checks the process state of the NV
 *  \return      MEMIF_IDLE in case job is finished (failed or OK state), otherwise MEMIF_BUSY
 **********************************************************************************************************************/
STATIC FUNC(MemIf_StatusType, WRAPNV_CODE) WrapNv_GetStatus( void )
{
   MemIf_StatusType result;

# if defined( WRAPNV_USECASE_FEE )
   result = WRAPNV_FEE_GETSTATUS();
# endif /* WRAPNV_USECASE_FEE */

   return result;
}

/***********************************************************************************************************************
 *  WrapNv_GetJobResult
 **********************************************************************************************************************/
/*! \brief       Checks the result of the last request
 *  \pre         WrapNv_GetStatus has been called and returned MEMIF_IDLE
 *  \return      return MEMIF_JOB_OK in case data could be read successfully
 *                      MEMIF_BLOCK_INCONSISTENT in case data could not be read (e.g. data are corrupt)
 *                      MEMIF_INVALID in case the block has been invalidated previously
 *                      MEMIF_JOB_FAILED otherwise
 **********************************************************************************************************************/
STATIC FUNC(MemIf_JobResultType, WRAPNV_CODE) WrapNv_GetJobResult( void )
{
   MemIf_JobResultType result;

# if defined( WRAPNV_USECASE_FEE )
   result = WRAPNV_FEE_GETJOBRESULT();
# endif /* WRAPNV_USECASE_FEE */

   return result;
}

/***********************************************************************************************************************
 *  WrapNv_Read
 **********************************************************************************************************************/
/*! \brief       Initialize the read operation
 *  \param[in]   id  Id of record
 *  \param[in]   idx  Dataset of block
 *  \param[out]  buffer  Pointer to data buffer
 *  \return      result     WRAPNV_E_OK on job completion, WRAPNV_E_NOT_OK if job execution failed,
                            WRAPNV_E_PENDING if job needs more time
 **********************************************************************************************************************/
STATIC FUNC(WrapNv_ReturnType, WRAPNV_CODE) WrapNv_Read( uint16 id, uint16 idx, tWrapNvRamData buffer )
{
   WrapNv_ReturnType result;
   uint8 errorId;
   P2CONST(tWrapNvRecordTbl, AUTOMATIC, WRAPNV_CONST) localRecordPtr;
   P2CONST(tWrapNvBlockTbl, AUTOMATIC, WRAPNV_CONST) localBlockPtr;

   /* Initialize variable */
   result = WRAPNV_E_NOT_OK;
   errorId = WRAPNV_E_NO_ERROR;
   localRecordPtr = &kWrapNvRecordTbl[id];
   localBlockPtr = localRecordPtr->recordBlockPtr;

   if (WRAPNV_STATE_IDLE != gWrapNvCurrentState)
   {
      errorId = WRAPNV_E_NOT_IDLE;
   }
   /* Check if dataset is valid */
   else if (   (idx >= localBlockPtr->blockMaxDatasets)
            || (idx > 0xFFu))
   {
      errorId = WRAPNV_E_PARAM_DATASET;
   }
   else
   {
      gWrapNvBlockNumber =localBlockPtr->blockNumber;
      gWrapNvBlockDataset = idx;
      gWrapNvBlockDefaultValue = localBlockPtr->blockDefaultValue;

      gWrapNvReadDataOffset = localRecordPtr->recordDataOffset;
      gWrapNvReadDataLength = localRecordPtr->recordDataLength;
      gWrapNvReadDataBuffer = buffer;

      gWrapNvWriteDataOffset = 0u;
      gWrapNvWriteDataLength = 0u;
      gWrapNvWriteDataBuffer = NULL_PTR;

# if defined( WRAPNV_USECASE_FEE )
      gWrapNvBlockAccessBuffer = NULL_PTR;
# endif /* WRAPNV_USECASE_FEE */

      gWrapNvCurrentState = WRAPNV_STATE_READ_INIT;

      result = WrapNv_MainFunction();
   }

# if defined( DET_ENABLED )
   if(errorId != WRAPNV_E_NO_ERROR)
   {
      (void)Det_ReportError(WRAPNV_MODULE_ID, WRAPNV_INSTANCE_ID, WRAPNV_READ, errorId );
   }
# else
   (void)errorId; /* PRQA S 3112 */ /* MD_MSR_14.2 */
# endif /* DET_ENABLED */

   return result;
}

/***********************************************************************************************************************
 *  WrapNv_ReadPartial
 **********************************************************************************************************************/
/*! \brief       Initialize operation for reading only partial of the data
 *  \param[in]   id  Id of record
 *  \param[in]   idx  Dataset of block
 *  \param[out]  buffer  Pointer to data buffer
 *  \param[in]   offset Offset in the entry to read only partial
 *  \param[in]   length Length of the partial read
 *  \return      result     WRAPNV_E_OK on job completion, WRAPNV_E_NOT_OK if job execution failed,
                            WRAPNV_E_PENDING if job needs more time
 **********************************************************************************************************************/
STATIC FUNC(WrapNv_ReturnType, WRAPNV_CODE) WrapNv_ReadPartial( uint16 id, uint16 idx, tWrapNvRamData buffer,
   uint16 offset, uint16 length )
{
   WrapNv_ReturnType result;
   uint8 errorId;
   P2CONST(tWrapNvRecordTbl, AUTOMATIC, WRAPNV_CONST) localRecordPtr;
   P2CONST(tWrapNvBlockTbl, AUTOMATIC, WRAPNV_CONST) localBlockPtr;

   /* Initialize variable */
   result = WRAPNV_E_NOT_OK;
   errorId = WRAPNV_E_NO_ERROR;
   localRecordPtr = &kWrapNvRecordTbl[id];
   localBlockPtr = localRecordPtr->recordBlockPtr;

   if (WRAPNV_STATE_IDLE != gWrapNvCurrentState)
   {
      errorId = WRAPNV_E_NOT_IDLE;
   }
   /* Check if dataset is valid */
   else if (   (idx >= localBlockPtr->blockMaxDatasets)
            || (idx > 0xFFu))
   {
      errorId = WRAPNV_E_PARAM_DATASET;
   }
   else if ((offset + length) > localRecordPtr->recordDataLength)
   {
      errorId = WRAPNV_E_PARAM_READ;
   }
   else
   {
      gWrapNvBlockNumber = localBlockPtr->blockNumber;
      gWrapNvBlockDataset = idx;
      gWrapNvBlockDefaultValue = localBlockPtr->blockDefaultValue;

      gWrapNvReadDataOffset = localRecordPtr->recordDataOffset + offset;
      gWrapNvReadDataLength = length;
      gWrapNvReadDataBuffer = buffer;

      gWrapNvWriteDataOffset = 0u;
      gWrapNvWriteDataLength = 0u;
      gWrapNvWriteDataBuffer = NULL_PTR;

# if defined( WRAPNV_USECASE_FEE )
      gWrapNvBlockAccessBuffer = NULL_PTR;
# endif /* WRAPNV_USECASE_FEE */

      gWrapNvCurrentState = WRAPNV_STATE_READ_INIT;

      result = WrapNv_MainFunction();
   }

# if defined( DET_ENABLED )
   if(errorId != WRAPNV_E_NO_ERROR)
   {
      (void)Det_ReportError(WRAPNV_MODULE_ID, WRAPNV_INSTANCE_ID, WRAPNV_READ, errorId );
   }
# else
   (void)errorId; /* PRQA S 3112 */ /* MD_MSR_14.2 */
# endif /* DET_ENABLED */

   return result;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/***********************************************************************************************************************
 *  WrapNv_Delete
 **********************************************************************************************************************/
/*! \brief       Initialize the delete operation
 *  \param[in]   id  Id of record
 *  \param[in]   idx  Dataset of block
 *  \return      result     WRAPNV_E_OK on job completion, WRAPNV_E_NOT_OK if job execution failed,
                            WRAPNV_E_PENDING if job needs more time
 **********************************************************************************************************************/
STATIC FUNC(WrapNv_ReturnType, WRAPNV_CODE) WrapNv_Delete( uint16 id, uint16 idx )
{
   WrapNv_ReturnType result;
   uint8 errorId;
   P2CONST(tWrapNvRecordTbl, AUTOMATIC, WRAPNV_CONST) localRecordPtr;
   P2CONST(tWrapNvBlockTbl, AUTOMATIC, WRAPNV_CONST) localBlockPtr;

   /* Initialize variable */
   result = WRAPNV_E_NOT_OK;
   errorId = WRAPNV_E_NO_ERROR;
   localRecordPtr = &kWrapNvRecordTbl[id];
   localBlockPtr = localRecordPtr->recordBlockPtr;

   if (WRAPNV_STATE_IDLE != gWrapNvCurrentState)
   {
      errorId = WRAPNV_E_NOT_IDLE;
   }
   /* Check if dataset is valid */
   else if (   (idx >= localBlockPtr->blockMaxDatasets)
            || (idx > 0xFFu))
   {
      errorId = WRAPNV_E_PARAM_DATASET;
   }
   else
   {
      gWrapNvBlockNumber = localBlockPtr->blockNumber;
      gWrapNvBlockDataset = idx;
      gWrapNvBlockDefaultValue = localBlockPtr->blockDefaultValue;

      gWrapNvReadDataOffset = 0u;
      gWrapNvReadDataLength = 0u;
      gWrapNvReadDataBuffer = NULL_PTR;

      gWrapNvWriteDataOffset = 0u;
      gWrapNvWriteDataLength = 0u;
      gWrapNvWriteDataBuffer = NULL_PTR;

      gWrapNvBlockAccessBuffer = NULL_PTR;

      gWrapNvCurrentState = WRAPNV_STATE_DELETE_INIT;

      result = WrapNv_MainFunction();
   }

# if defined( DET_ENABLED )
   if(errorId != WRAPNV_E_NO_ERROR)
   {
      (void)Det_ReportError(WRAPNV_MODULE_ID, WRAPNV_INSTANCE_ID, WRAPNV_READ, errorId );
   }
# else
   (void)errorId; /* PRQA S 3112 */ /* MD_MSR_14.2 */
# endif /* DET_ENABLED */

   return result;
}

/***********************************************************************************************************************
 *  WrapNv_Write
 **********************************************************************************************************************/
/*! \brief       Initialize the write operation
 *  \param[in]   id  Id of record
 *  \param[in]   idx  Dataset of block
 *  \param[in]   buffer  Pointer to data buffer
 *  \return      result     WRAPNV_E_OK on job completion, WRAPNV_E_NOT_OK if job execution failed,
                            WRAPNV_E_PENDING if job needs more time
 **********************************************************************************************************************/
STATIC FUNC(WrapNv_ReturnType, WRAPNV_CODE) WrapNv_Write( uint16 id, uint16 idx, tWrapNvConstData buffer )
{
   WrapNv_ReturnType result;
   uint8 errorId;
   P2CONST(tWrapNvRecordTbl, AUTOMATIC, WRAPNV_CONST) localRecordPtr;
   P2CONST(tWrapNvBlockTbl, AUTOMATIC, WRAPNV_CONST) localBlockPtr;

   /* Initialize variable */
   result = WRAPNV_E_NOT_OK;
   errorId = WRAPNV_E_NO_ERROR;
   localRecordPtr = &kWrapNvRecordTbl[id];
   localBlockPtr = localRecordPtr->recordBlockPtr;

   if (WRAPNV_STATE_IDLE != gWrapNvCurrentState)
   {
      errorId = WRAPNV_E_NOT_IDLE;
   }
   /* Check if dataset is valid */
   else if (   (idx >= localBlockPtr->blockMaxDatasets)
            || (idx > 0xFFu))
   {
      errorId = WRAPNV_E_PARAM_DATASET;
   }
   else
   {
      gWrapNvBlockNumber = localBlockPtr->blockNumber;
      gWrapNvBlockDataset = idx;
      gWrapNvBlockDefaultValue = localBlockPtr->blockDefaultValue;

      if (localBlockPtr->blockDataAccess == WRAPNV_RECORDACCESS_STRUCTURED)
      {
         gWrapNvReadDataOffset = 0u;
         gWrapNvReadDataLength = localBlockPtr->blockLength;
         gWrapNvReadDataBuffer = gWrapNvBlockBuffer;   /* Map the read buffer to the same buffer as the access buffer */

         gWrapNvWriteDataOffset = localRecordPtr->recordDataOffset;
         gWrapNvWriteDataLength = localRecordPtr->recordDataLength;
         gWrapNvWriteDataBuffer = buffer;

         gWrapNvBlockAccessBuffer = gWrapNvBlockBuffer;

         gWrapNvCurrentState = WRAPNV_STATE_READ_MODIFY_INIT;
      }
      else
      {
         gWrapNvReadDataOffset = 0u;
         gWrapNvReadDataLength = 0u;
         gWrapNvReadDataBuffer = NULL_PTR;

         gWrapNvWriteDataOffset = 0u;
         gWrapNvWriteDataLength = 0u;
         gWrapNvWriteDataBuffer = NULL_PTR;

         gWrapNvBlockAccessBuffer = (tWrapNvRamData)buffer;  /* PRQA S 0311 */ /* MD_WrapNv_0311 */

         gWrapNvCurrentState = WRAPNV_STATE_WRITE_INIT;
      }

      result = WrapNv_MainFunction();
   }

# if defined( DET_ENABLED )
   if(errorId != WRAPNV_E_NO_ERROR)
   {
      (void)Det_ReportError(WRAPNV_MODULE_ID, WRAPNV_INSTANCE_ID, WRAPNV_READ, errorId );
   }
# else
   (void)errorId; /* PRQA S 3112 */ /* MD_MSR_14.2 */
# endif /* DET_ENABLED */

   return result;
}

/***********************************************************************************************************************
 *  WrapNv_MainFunction
 **********************************************************************************************************************/
/*! \brief       Main task of the NV-Wrapper to get the current status of the read/write operation
 *  \return      result     WRAPNV_E_OK on job completion, WRAPNV_E_NOT_OK if job execution failed,
                            WRAPNV_E_PENDING if job needs more time
 **********************************************************************************************************************/
STATIC FUNC(WrapNv_ReturnType, WRAPNV_CODE) WrapNv_MainFunction( void )
{
   WrapNv_ReturnType result;
   uint8_least index;

   result = WRAPNV_E_PENDING;

   switch (gWrapNvCurrentState)
   {
      case WRAPNV_STATE_READ_INIT:
      case WRAPNV_STATE_READ_MODIFY_INIT:
      {
         if (MEMIF_IDLE == WrapNv_GetStatus())
         {
# if defined( WRAPNV_USECASE_FEE )
            result = WRAPNV_FEE_READ(gWrapNvBlockNumber | gWrapNvBlockDataset, gWrapNvReadDataOffset, gWrapNvReadDataBuffer, gWrapNvReadDataLength);
# endif /* WRAPNV_USECASE_FEE */

            /* If read request was successful, remap result to WRAPNV_E_PENDING */
            if (WRAPNV_E_OK == result)
            {
               result = WRAPNV_E_PENDING;

               if (WRAPNV_STATE_READ_INIT == gWrapNvCurrentState)
               {
                  gWrapNvCurrentState = WRAPNV_STATE_READ_PENDING;
               }
               else
               {
                  gWrapNvCurrentState = WRAPNV_STATE_READ_MODIFY_PENDING;
               }
            }
            else
            {
               gWrapNvCurrentState = WRAPNV_STATE_IDLE;
            }
         }
         break;
      }
      case WRAPNV_STATE_READ_PENDING:
      case WRAPNV_STATE_READ_MODIFY_PENDING:
      {
         if (MEMIF_IDLE == WrapNv_GetStatus())
         {
            switch( WrapNv_GetJobResult() )
            {
               case MEMIF_JOB_OK:
               {

                  result = WRAPNV_E_OK;
                  break;
               }
               case MEMIF_BLOCK_INCONSISTENT:
               case MEMIF_BLOCK_INVALID:
               {
                  if (gWrapNvBlockDefaultValue != NULL_PTR)
                  {
                     for (index = 0u; index < gWrapNvReadDataLength; index++)
                     {
                        gWrapNvReadDataBuffer[index] = gWrapNvBlockDefaultValue[gWrapNvReadDataOffset + index];
                     }
                  }
                  else
                  {
                     /* No explicit default value configured - fill read buffer with default value */
                     for (index = 0u; index < gWrapNvReadDataLength; index++)
                     {
                        gWrapNvReadDataBuffer[index] = WRAPNV_DEFAULT_VALUE;
                     }
                  }
                  result = WRAPNV_E_OK;
                  break;
               }
               default:
               {
                  result = WRAPNV_E_NOT_OK;
                  break;
               }
            }

            if (WRAPNV_STATE_READ_PENDING == gWrapNvCurrentState)
            {
               /* No more action required - reset state */
               gWrapNvCurrentState = WRAPNV_STATE_IDLE;
            }
            else
            {
               if (WRAPNV_E_OK == result)
               {
                  /* This is a read-modify-write operation - remap WRAPNV_E_OK state to WRAPNV_E_PENDING */
                  result = WRAPNV_E_PENDING;
                  gWrapNvCurrentState = WRAPNV_STATE_MODIFY_DATA;
               }
            }
         }
         break;
      }
      case WRAPNV_STATE_MODIFY_DATA:
      {
         /* Modify data with new information */
         for (index = 0u; index < gWrapNvWriteDataLength; index++)
         {
            gWrapNvBlockAccessBuffer[gWrapNvWriteDataOffset + index] = gWrapNvWriteDataBuffer[index];
         }

         gWrapNvCurrentState = WRAPNV_STATE_WRITE_INIT;
         /* Intentional fall through */
      }
      case WRAPNV_STATE_WRITE_INIT: /* PRQA S 2003 */ /* MD_WrapNv_2003 */
      {
         if (MEMIF_IDLE == WrapNv_GetStatus())
         {
# if defined( WRAPNV_USECASE_FEE )
            result = WRAPNV_FEE_WRITE(gWrapNvBlockNumber | gWrapNvBlockDataset, gWrapNvBlockAccessBuffer);
# endif /* WRAPNV_USECASE_FEE */

            /* If write request was successful, remap result to WRAPNV_E_PENDING */
            if (WRAPNV_E_OK == result)
            {
               result = WRAPNV_E_PENDING;

               gWrapNvCurrentState = WRAPNV_STATE_WRITE_PENDING;
            }
            else
            {
               gWrapNvCurrentState = WRAPNV_STATE_IDLE;
            }
         }
         break;
      }
      case WRAPNV_STATE_DELETE_INIT:
      {
         if (MEMIF_IDLE == WrapNv_GetStatus())
         {
# if defined( WRAPNV_USECASE_FEE )
            result = WRAPNV_FEE_INVALIDATEBLOCK(gWrapNvBlockNumber | gWrapNvBlockDataset);
# endif /* WRAPNV_USECASE_FEE */

            /* If invalidate request was successful, remap result to WRAPNV_E_PENDING */
            if (WRAPNV_E_OK == result)
            {
               result = WRAPNV_E_PENDING;

               gWrapNvCurrentState = WRAPNV_STATE_DELETE_PENDING;
            }
            else
            {
               gWrapNvCurrentState = WRAPNV_STATE_IDLE;
            }
         }
         break;
      }
      case WRAPNV_STATE_WRITE_PENDING:
      case WRAPNV_STATE_DELETE_PENDING:
      {
         if (MEMIF_IDLE == WrapNv_GetStatus())
         {
            if (MEMIF_JOB_OK == WrapNv_GetJobResult())
            {
               result = WRAPNV_E_OK;
            }
            else
            {
               result = WRAPNV_E_NOT_OK;
            }

            /* Reset current state */
            gWrapNvCurrentState = WRAPNV_STATE_IDLE;
         }
         break;
      }
      default:
      {
         result = WRAPNV_E_NOT_OK;
         break;
      }
   }

   return result;
} /* PRQA S 6010, 6030, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STMIF */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/***********************************************************************************************************************
 *  WrapNv_Init
 **********************************************************************************************************************/
/*! \brief       Initializes NV-Wrapper to access FEE/NVM
 **********************************************************************************************************************/
FUNC(void, WRAPNV_CODE) WrapNv_Init( void )
{
# if defined( WRAPNV_ENABLE_FEE_INITIALIZATION )
   WRAPNV_FLS_INIT(WRAPNV_FLS_INIT_PARAMETER);

   while (MEMIF_IDLE != WRAPNV_FLS_GETSTATUS())
   {
      WRAPNV_POLLING_FCT();

      WRAPNV_FLS_MAINFUNCTION();
   }

   WRAPNV_FEE_INIT();

   while (MEMIF_IDLE != WRAPNV_FEE_GETSTATUS())
   {
      WRAPNV_POLLING_FCT();

      WRAPNV_FLS_MAINFUNCTION();
      WRAPNV_FEE_MAINFUNCTION();
   }

#  if (FEE_FSS_CONTROL_API == STD_ON)
   Fee_EnableFss();
#  endif
# endif /* WRAPNV_ENABLE_FEE_INITIALIZATION */

   /* Initialize state */
   gWrapNvCurrentState = WRAPNV_STATE_IDLE;
   gWrapNvInternalOpStatus = WRAPNV_OPSTATUS_IDLE;
}

/***********************************************************************************************************************
 *  WrapNv_ReadSync
 **********************************************************************************************************************/
/*! \brief       Reads specific data synchronously from a NV block
 *  \pre         Function must only be called from where MainFunction polling is allowed.
 *  \param[in]   id  Id of record
 *  \param[in]   idx  Dataset of block
 *  \param[in]   buffer  Pointer to data buffer
 *  \return      kFblOk if succeeded, kFblFailed otherwise
 **********************************************************************************************************************/
FUNC(WrapNv_ReturnType, WRAPNV_CODE) WrapNv_ReadSync( uint16 id, uint16 idx, tWrapNvRamData buffer )
{
   WrapNv_ReturnType result;

   result = WrapNv_Read(id, idx, buffer);

   while (WRAPNV_E_PENDING == result)
   {
      result = WrapNv_MainFunction();

      WRAPNV_POLLING_FCT();

      WRAPNV_FLS_MAINFUNCTION();
      WRAPNV_FEE_MAINFUNCTION();
   }

   return result;
}

/***********************************************************************************************************************
 *  WrapNv_ReadPartialSync
 **********************************************************************************************************************/
/*! \brief       Reads specific data synchronously from a NV block
 *  \pre         Function must only be called from where MainFunction polling is allowed.
 *  \param[in]   id  Id of record
 *  \param[in]   idx  Dataset of block
 *  \param[in]   buffer  Pointer to data buffer
 *  \param[in]   offset Offset in the entry to read only partial
 *  \param[in]   length Length of the partial read
 *  \return      kFblOk if succeeded, kFblFailed otherwise
 **********************************************************************************************************************/
FUNC(WrapNv_ReturnType, WRAPNV_CODE) WrapNv_ReadPartialSync( uint16 id, uint16 idx, tWrapNvRamData buffer,
   uint16 offset, uint16 length )
{
   WrapNv_ReturnType result;

   result = WrapNv_ReadPartial(id, idx, buffer, offset, length);

   while (WRAPNV_E_PENDING == result)
   {
      result = WrapNv_MainFunction();

      WRAPNV_POLLING_FCT();

      WRAPNV_FLS_MAINFUNCTION();
      WRAPNV_FEE_MAINFUNCTION();
   }

   return result;
}

/***********************************************************************************************************************
 *  WrapNv_WriteSync
 **********************************************************************************************************************/
/*! \brief       Writes specific data synchronously to a NV block
 *  \pre         Function must only be called from where MainFunction polling is allowed.
 *  \param[in]   id  Id of record
 *  \param[in]   idx  Dataset of block
 *  \param[in]   buffer  Pointer to data buffer
 *  \return      kFblOk if succeeded, kFblFailed otherwise
 **********************************************************************************************************************/
FUNC(WrapNv_ReturnType, WRAPNV_CODE) WrapNv_WriteSync( uint16 id, uint16 idx, tWrapNvConstData buffer )
{
   WrapNv_ReturnType result;

   result = WrapNv_Write(id, idx, buffer);

   while (WRAPNV_E_PENDING == result)
   {
      result = WrapNv_MainFunction();

      WRAPNV_POLLING_FCT();

      WRAPNV_FLS_MAINFUNCTION();
      WRAPNV_FEE_MAINFUNCTION();
   }

   return result;
}

/***********************************************************************************************************************
 *  WrapNv_DeleteSync
 **********************************************************************************************************************/
/*! \brief       Delete specific NV block synchronously
 *  \pre         Function must only be called from where MainFunction polling is allowed.
 *  \param[in]   id  Id of record
 *  \param[in]   idx  Dataset of block
 *  \return      kFblOk if succeeded, kFblFailed otherwise
 **********************************************************************************************************************/
FUNC(WrapNv_ReturnType, WRAPNV_CODE) WrapNv_DeleteSync( uint16 id, uint16 idx )
{
   WrapNv_ReturnType result;

   result = WrapNv_Delete(id, idx);

   while (WRAPNV_E_PENDING == result)
   {
      result = WrapNv_MainFunction();

      WRAPNV_POLLING_FCT();

      WRAPNV_FLS_MAINFUNCTION();
      WRAPNV_FEE_MAINFUNCTION();
   }

   return result;
}

/***********************************************************************************************************************
 *  WrapNv_ReadAsync
 **********************************************************************************************************************/
/*! \brief       Reads specific data asynchronously from a NV block
 *  \param[in]   id  Id of record
 *  \param[in]   idx  Dataset of block
 *  \param[out]  buffer  Pointer to data buffer
 *  \param[in]   opStatus  Operation status of DCM
 *  \return      result     WRAPNV_E_OK on job completion, WRAPNV_E_NOT_OK if job execution failed,
                            WRAPNV_E_PENDING if job needs more time
 **********************************************************************************************************************/
FUNC(WrapNv_ReturnType, WRAPNV_CODE) WrapNv_ReadAsync( uint16 id, uint16 idx, tWrapNvRamData buffer,
   tWrapNvOpStatus opStatus)
{
   WrapNv_ReturnType result;
   tWrapNvOpStatus currentOpStatus;

   /* Initialize variable */
   currentOpStatus = opStatus;

   /* Check if this is a new read request , but th previous request is still pending (previous job has been "canceled") */
   if (   (WRAPNV_STATE_IDLE != gWrapNvCurrentState)
       && (WRAPNV_OPSTATUS_INIT == opStatus))
   {
      /* Remember that the init has been postponed until previous job finished and ... */
      gWrapNvInternalOpStatus = WRAPNV_OPSTATUS_PENDING;
      /* get status of previous job instead of initializing current read request */
      currentOpStatus = WRAPNV_OPSTATUS_PENDING;
   }

   /* Remap op-status to initialize the read request */
   if (WRAPNV_OPSTATUS_INIT == gWrapNvInternalOpStatus)
   {
      gWrapNvInternalOpStatus = WRAPNV_OPSTATUS_IDLE;
      currentOpStatus = WRAPNV_OPSTATUS_INIT;
   }

   /* Only init and pending states are allowed at this point */
   switch (currentOpStatus)
   {
      case WRAPNV_OPSTATUS_INIT:
      {
         result = WrapNv_Read(id, idx, buffer);
         break;
      }
      case WRAPNV_OPSTATUS_PENDING:
      {
         result = WrapNv_MainFunction();
         break;
      }
      case WRAPNV_OPSTATUS_CANCEL:
      default:
      {
         result = WRAPNV_E_NOT_OK;
         break;
      }
   }

   /* Check if previous job is pending */
   if (WRAPNV_OPSTATUS_PENDING == gWrapNvInternalOpStatus)
   {
      /* Set internal state to initialize current request */
      /* A failure of previous job should not affect current request, therefore set the result to WRAPNV_E_PENDING */
      if (   (WRAPNV_E_OK == result)
          || (WRAPNV_E_NOT_OK == result))
      {
         gWrapNvInternalOpStatus = WRAPNV_OPSTATUS_INIT;
         result = WRAPNV_E_PENDING;
      }
   }

   return result;
}

/***********************************************************************************************************************
 *  WrapNv_ReadPartialAsync
 **********************************************************************************************************************/
/*! \brief       Reads specific data asynchronously from a NV block
 *  \param[in]   id  Id of record
 *  \param[in]   idx  Dataset of block
 *  \param[out]  buffer  Pointer to data buffer
 *  \param[in]   offset Offset in the entry to read only partial
 *  \param[in]   length Length of the partial read
 *  \param[in]   opStatus  Operation status of DCM
 *  \return      result     WRAPNV_E_OK on job completion, WRAPNV_E_NOT_OK if job execution failed,
                            WRAPNV_E_PENDING if job needs more time
 **********************************************************************************************************************/
FUNC(WrapNv_ReturnType, WRAPNV_CODE) WrapNv_ReadPartialAsync( uint16 id, uint16 idx, tWrapNvRamData buffer,
   uint16 offset, uint16 length, tWrapNvOpStatus opStatus)
{
   WrapNv_ReturnType result;
   tWrapNvOpStatus currentOpStatus;

   /* Initialize variable */
   currentOpStatus = opStatus;

   /* Check if this is a new read request , but th previous request is still pending (previous job has been "canceled") */
   if (   (WRAPNV_STATE_IDLE != gWrapNvCurrentState)
       && (WRAPNV_OPSTATUS_INIT == opStatus))
   {
      /* Remember that the init has been postponed until previous job finished and ... */
      gWrapNvInternalOpStatus = WRAPNV_OPSTATUS_PENDING;
      /* get status of previous job instead of initializing current read request */
      currentOpStatus = WRAPNV_OPSTATUS_PENDING;
   }

   /* Remap op-status to initialize the read request */
   if (WRAPNV_OPSTATUS_INIT == gWrapNvInternalOpStatus)
   {
      gWrapNvInternalOpStatus = WRAPNV_OPSTATUS_IDLE;
      currentOpStatus = WRAPNV_OPSTATUS_INIT;
   }

   /* Only init and pending states are allowed at this point */
   switch (currentOpStatus)
   {
      case WRAPNV_OPSTATUS_INIT:
      {
         result = WrapNv_ReadPartial(id, idx, buffer, offset, length);
         break;
      }
      case WRAPNV_OPSTATUS_PENDING:
      {
         result = WrapNv_MainFunction();
         break;
      }
      case WRAPNV_OPSTATUS_CANCEL:
      default:
      {
         result = WRAPNV_E_NOT_OK;
         break;
      }
   }

   /* Check if previous job is pending */
   if (WRAPNV_OPSTATUS_PENDING == gWrapNvInternalOpStatus)
   {
      /* Set internal state to initialize current request */
      /* A failure of previous job should not affect current request, therefore set the result to WRAPNV_E_PENDING */
      if (   (WRAPNV_E_OK == result)
          || (WRAPNV_E_NOT_OK == result))
      {
         gWrapNvInternalOpStatus = WRAPNV_OPSTATUS_INIT;
         result = WRAPNV_E_PENDING;
      }
   }

   return result;
} /* PRQA S 6060 */ /* MD_MSR_STPAR */

/***********************************************************************************************************************
 *  WrapNv_WriteAsync
 **********************************************************************************************************************/
/*! \brief       Writes specific data asynchronously to a NV block
 *  \param[in]   id  Id of record
 *  \param[in]   idx  Dataset of block
 *  \param[in]   buffer  Pointer to data buffer
 *  \param[in]   opStatus  Operation status of DCM
 *  \return      result     WRAPNV_E_OK on job completion, WRAPNV_E_NOT_OK if job execution failed,
                            WRAPNV_E_PENDING if job needs more time
 **********************************************************************************************************************/
FUNC(WrapNv_ReturnType, WRAPNV_CODE) WrapNv_WriteAsync( uint16 id, uint16 idx, tWrapNvConstData buffer,
   tWrapNvOpStatus opStatus)
{
   WrapNv_ReturnType result;
   tWrapNvOpStatus currentOpStatus;

   /* Initialize variable */
   currentOpStatus = opStatus;

   /* Check if this is a new write request , but th previous request is still pending (previous job has been "canceled") */
   if (   (WRAPNV_STATE_IDLE != gWrapNvCurrentState)
       && (WRAPNV_OPSTATUS_INIT == opStatus))
   {
      /* Remember that the init has been postponed until previous job finished and ... */
      gWrapNvInternalOpStatus = WRAPNV_OPSTATUS_PENDING;
      /* get status of previous job instead of initializing current write request */
      currentOpStatus = WRAPNV_OPSTATUS_PENDING;
   }

   /* Remap op-status to initialize the write request */
   if (WRAPNV_OPSTATUS_INIT == gWrapNvInternalOpStatus)
   {
      gWrapNvInternalOpStatus = WRAPNV_OPSTATUS_IDLE;
      currentOpStatus = WRAPNV_OPSTATUS_INIT;
   }

   /* Only init and pending states are allowed at this point */
   switch (currentOpStatus)
   {
      case WRAPNV_OPSTATUS_INIT:
      {
         result = WrapNv_Write(id, idx, buffer);
         break;
      }
      case WRAPNV_OPSTATUS_PENDING:
      {
         result = WrapNv_MainFunction();
         break;
      }
      default:
      {
         result = WRAPNV_E_NOT_OK;
         break;
      }
   }

   /* Check if previous job is pending */
   if (WRAPNV_OPSTATUS_PENDING == gWrapNvInternalOpStatus)
   {
      /* Set internal state to initialize current request */
      /* A failure of previous job should not affect current request, therefore set the result to WRAPNV_E_PENDING */
      if (   (WRAPNV_E_OK == result)
          || (WRAPNV_E_NOT_OK == result))
      {
         gWrapNvInternalOpStatus = WRAPNV_OPSTATUS_INIT;
         result = WRAPNV_E_PENDING;
      }
   }

   return result;
}

/***********************************************************************************************************************
 *  WrapNv_DeleteAsync
 **********************************************************************************************************************/
/*! \brief       Delete specific NV block asynchronously
 *  \param[in]   id  Id of record
 *  \param[in]   idx  Dataset of block
 *  \param[in]   opStatus  Operation status of DCM
 *  \return      result     WRAPNV_E_OK on job completion, WRAPNV_E_NOT_OK if job execution failed,
                            WRAPNV_E_PENDING if job needs more time
 **********************************************************************************************************************/
FUNC(WrapNv_ReturnType, WRAPNV_CODE) WrapNv_DeleteAsync( uint16 id, uint16 idx, tWrapNvOpStatus opStatus)
{
   WrapNv_ReturnType result;
   tWrapNvOpStatus currentOpStatus;

   /* Initialize variable */
   currentOpStatus = opStatus;

   /* Check if this is a new write request , but th previous request is still pending (previous job has been "canceled") */
   if (   (WRAPNV_STATE_IDLE != gWrapNvCurrentState)
       && (WRAPNV_OPSTATUS_INIT == opStatus))
   {
      /* Remember that the init has been postponed until previous job finished and ... */
      gWrapNvInternalOpStatus = WRAPNV_OPSTATUS_PENDING;
      /* get status of previous job instead of initializing current write request */
      currentOpStatus = WRAPNV_OPSTATUS_PENDING;
   }

   /* Remap op-status to initialize the write request */
   if (WRAPNV_OPSTATUS_INIT == gWrapNvInternalOpStatus)
   {
      gWrapNvInternalOpStatus = WRAPNV_OPSTATUS_IDLE;
      currentOpStatus = WRAPNV_OPSTATUS_INIT;
   }

   /* Only init and pending states are allowed at this point */
   switch (currentOpStatus)
   {
      case WRAPNV_OPSTATUS_INIT:
      {
         result = WrapNv_Delete(id, idx);
         break;
      }
      case WRAPNV_OPSTATUS_PENDING:
      {
         result = WrapNv_MainFunction();
         break;
      }
      default:
      {
         result = WRAPNV_E_NOT_OK;
         break;
      }
   }

   /* Check if previous job is pending */
   if (WRAPNV_OPSTATUS_PENDING == gWrapNvInternalOpStatus)
   {
      /* Set internal state to initialize current request */
      /* A failure of previous job should not affect current request, therefore set the result to WRAPNV_E_PENDING */
      if (   (WRAPNV_E_OK == result)
          || (WRAPNV_E_NOT_OK == result))
      {
         gWrapNvInternalOpStatus = WRAPNV_OPSTATUS_INIT;
         result = WRAPNV_E_PENDING;
      }
   }

   return result;
}

# define WRAPNV_STOP_SEC_CODE
# include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* Module specific MISRA deviations:

   MD_WrapNv_0311:
      Reason:     Provide simplified function interface and reduce stack consumption by avoiding separate pointers for 
                  read and write buffer.
      Risk:       The content of the write buffer might be accidentally overwritten.
      Prevention: Covered by code review.

   MD_WrapNv_0342:
      Reason:     Usage of glue operator unifies naming scheme, without relying on massive use of copy&paste.
                  Additionally support for different use-cases can be easily achieved.
      Risk:       Not supported by outdated compilers.
      Prevention: No prevention required.

   MD_WrapNv_0883:
      Reason:     Different defines are set depending on configuration aspects of the project. #elif is chosen
                  to increase the readability.
      Risk:       No identifiable risk because repeated inclusion protection is done at the begin of the file.
      Prevention: No prevention required.

   MD_WrapNv_2003:
      Reason:     Intentionally switch case with out break statement to proceed the next step immediately
      Risk:       No identifiable risk.
      Prevention: Covered by code review.

   MD_WrapNv_3218:
      Reason:     The usage of variables depend on configuration aspects and may be used only once.
      Risk:       No identifiable risk.
      Prevention: No prevention required.

*/

/***********************************************************************************************************************
 *  END OF FILE: WRAPNV.C
 **********************************************************************************************************************/
