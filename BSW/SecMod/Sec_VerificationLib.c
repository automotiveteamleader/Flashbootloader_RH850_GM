/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief        Implementation of the verification component of the HIS security module - Signature verification
 *
 *  \description  Offers signature/checksum verification primitives based on hash, HMAC and RSA
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

/***********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  --------------------------------------------------------------------------------------------------------------------
 *  Joern Herwig                  JHg           Vector Informatik GmbH
 *  Alexander Starke              ASe           Vector Informatik GmbH
 *  Christian Baeuerle            CB            Vector Informatik GmbH
 *  Andreas Wenckebach            AWh           Vector Informatik GmbH
 *  Thomas Mueller                ThM           Vector Informatik GmbH
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  01.00.00   2013-06-20  JHg     ESCAN00068213    Initial release based on FblSecMod_Vector 02.02.00
 *  02.00.00   2013-11-08  JHg     ESCAN00071179    Major refactoring
 *                                                   Moved to separate sub-package
 *                                                   Renamed file name (was SecMVerSig.c)
 *                                                   Split into source and library parts
 *                                                   Select available verification primitives at integration-time
 *  02.00.01   2013-12-19  JHg     ESCAN00072260    No changes
 *                         JHg     ESCAN00072561    No changes
 *  02.00.02   2014-01-20  JHg     ESCAN00073088    No changes
 *  02.01.00   2014-01-28  ASe     ESCAN00073047    No changes
 *  02.02.00   2014-02-10  JHg     ESCAN00073560    No changes
 *                         JHg     ESCAN00073355    Resolved compiler warnings
 *  02.02.01   2014-05-28  JHg     ESCAN00074792    No changes
 *                         JHg     ESCAN00073918    Use "offsetof" macro from standard definitions
 *  02.02.02   2015-02-09  CB      ESCAN00078641    No changes
 *  02.02.03   2015-07-29  AWh     ESCAN00084130    No changes
 *  02.03.00   2015-10-12  JHg     ESCAN00085807    Added support for RSASSA-PSS signatures
 *                         JHg     ESCAN00085808    Removed compile time switches for external workspace support
 *  02.04.00   2015-11-03  JHg     ESCAN00086240    No changes
 *                         JHg     ESCAN00086241    No changes
 *  02.04.01   2015-12-18  JHg     ESCAN00087162    No changes
 *  02.04.02   2016-04-25  ThM     ESCAN00083969    Changed include order to prevent marco NULL redefinition error
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

/* File has to be delivered as object code */
#define SEC_OBJECT_CODE_ONLY

/* Required for "offsetof" macro */
#include <stddef.h>

/* Security module configuration settings */
#include "Sec_Inc.h"

/* Global definitions for security module */
#include "Sec_Types.h"

/* Verification interface */
#include "Sec_Verification.h"
#include "Sec_VerificationLib.h"

/* ES library access */
#include "ESLib.h"

/**********************************************************************************************************************
 *  VERSION
 *********************************************************************************************************************/

#if ( SYSSERVICE_SECMODHIS_VERIFICATION_VERSION != 0x0204u ) || \
    ( SYSSERVICE_SECMODHIS_VERIFICATION_RELEASE_VERSION != 0x02u ) || \
    ( SYSSERVICE_SECMODHIS_VERIFICATIONLIB_VERSION != 0x0204u ) || \
    ( SYSSERVICE_SECMODHIS_VERIFICATIONLIB_RELEASE_VERSION != 0x02u )
# error "Error in SEC_VERIFICATIONLIB.C: Source and header file are inconsistent!"
#endif

/**********************************************************************************************************************
 *  DEFINES
 *********************************************************************************************************************/

/** Null pointer to ESLib workspace */
#define SEC_ES_WORKSPACE_NULL             ((V_MEMRAM1 eslt_WorkSpace V_MEMRAM2 V_MEMRAM3 *)V_NULL)

/** Offset of struct member relative to beginning of struct */
#if defined( offsetof )                                              /* PRQA S 5120 */ /* MD_SecVerificationLib_5120 */
# define SEC_MEMBER_OFFSET(type, member)  (offsetof(type, member))   /* PRQA S 3410 */ /* MD_SecVerificationLib_3410 */ /* PRQA S 5120 */ /* MD_SecVerificationLib_5120 */ /* PRQA S 3453 */ /* MD_CBD_19.7 */
#else
/* "offsetof" macro not available, use fall-back */
# define SEC_MEMBER_OFFSET(type, member)  ((SecM_LengthType)((SecM_SizeType)(&((type *)0)->member)))  /* PRQA S 3410 */ /* MD_SecVerificationLib_3410 */ /* PRQA S 3453 */ /* MD_CBD_19.7 */
#endif

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/** Common parameters of verification primitives */
typedef struct
{
   /** Size of complete workspace (including ESLib workspace)*/
   SecM_LengthType   workspaceSize;
   /** Offset of ESLib workspace
    *  Depends on size of basic workspace defined in source (contains message digest and default key reference)
    *  and member alignment */
   SecM_LengthType   eslibWorkspaceOffset;
   /** Size of message digest */
   SecM_LengthType   digestLength;
} SecM_VerifyPrimitiveType;

/** Pointer to ESLib hash initialization function */
typedef eslt_ErrorCode (* SecM_EsLibHashInitType)     ( eslt_WorkSpace * );
/** Pointer to ESLib hash update function */
typedef eslt_ErrorCode (* SecM_EsLibHashUpdateType)   ( eslt_WorkSpace *, const eslt_Length, const eslt_Byte * );
/** Pointer to ESLib hash finalization function */
typedef eslt_ErrorCode (* SecM_EsLibHashFinalizeType) ( eslt_WorkSpace *, eslt_Byte * );

/** Parameters of hash primitive */
typedef struct
{
   SecM_EsLibHashInitType     init;       /**< Pointer to initialization function */
   SecM_EsLibHashUpdateType   update;     /**< Pointer to update function */
   SecM_EsLibHashFinalizeType finalize;   /**< Pointer to finalization function */

   SecM_VerifyPrimitiveType   common;     /**< Common parameters */
} SecM_VerifyHashPrimitiveType;

/** Pointer to ESLib signature initialization function */
typedef eslt_ErrorCode (* SecM_EsLibSigInitType)      ( eslt_WorkSpace *, eslt_Length, eslt_pRomByte, eslt_Length, eslt_pRomByte );
/** Pointer to ESLib signature update function */
typedef eslt_ErrorCode (* SecM_EsLibSigUpdateType)    ( eslt_WorkSpace *, eslt_Length, const eslt_Byte * );
/** Pointer to ESLib signature finalization function */
typedef eslt_ErrorCode (* SecM_EsLibSigFinalizeType)  ( eslt_WorkSpace *, eslt_Byte * );
/** Pointer to ESLib signature verification function */
typedef eslt_ErrorCode (* SecM_EsLibSigVerifyType)    ( eslt_WorkSpace *, const eslt_Byte *, eslt_Length, const eslt_Byte * );

/** Parameters of signature primitive */
typedef struct
{
   SecM_EsLibSigInitType      init;       /**< Pointer to initialization function */
   SecM_EsLibSigUpdateType    update;     /**< Pointer to update function */
   SecM_EsLibSigFinalizeType  finalize;   /**< Pointer to finalization function */
   SecM_EsLibSigVerifyType    verify;     /**< Pointer to verification function */

   SecM_VerifyPrimitiveType   common;     /**< Common parameters */
} SecM_VerifySigPrimitiveType;

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

static SecM_StatusType SecM_CompareDigest( SecM_ConstRamDataType pCalculatedData, SecM_LengthType calculatedSize,
   SecM_ConstRamDataType pProvidedData, SecM_LengthType providedSize );
static void SecM_GetWorkspace( const V_MEMRAM1 SecM_SignatureParamType V_MEMRAM2 V_MEMRAM3 * pVerifyParam,
   V_MEMROM1 SecM_VerifyPrimitiveType V_MEMROM2 V_MEMROM3 * pPrimitive,
   V_MEMRAM1 SecM_WorkspaceType V_MEMRAM2 V_MEMRAM3 * pWorkspace );
static V_MEMRAM1 eslt_WorkSpace V_MEMRAM2 V_MEMRAM3 * SecM_InitEsWorkspace(
   const V_MEMRAM1 SecM_SignatureParamType V_MEMRAM2 V_MEMRAM3 * pVerifyParam,
   V_MEMROM1 SecM_VerifyPrimitiveType V_MEMROM2 V_MEMROM3 * pPrimitive );
static SecM_StatusType SecM_VerifyHash( V_MEMRAM1 SecM_SignatureParamType V_MEMRAM2 V_MEMRAM3 * pVerifyParam,
   V_MEMROM1 SecM_VerifyHashPrimitiveType V_MEMROM2 V_MEMROM3 * pPrimitive );
static SecM_StatusType SecM_VerifySig( V_MEMRAM1 SecM_SignatureParamType V_MEMRAM2 V_MEMRAM3 * pVerifyParam,
   V_MEMROM1 SecM_VerifySigPrimitiveType V_MEMROM2 V_MEMROM3 * pPrimitive );

/**********************************************************************************************************************
 *  LOCAL DATA
 *********************************************************************************************************************/

/* PRQA S 3218 TAG_SecVerificationLib_3218_1 */ /* MD_SecVerificationLib_3218 */

/** Configuration for SHA-256 hash primitive */
V_MEMROM0 static V_MEMROM1 SecM_VerifyHashPrimitiveType V_MEMROM2 hashConfigSha256 =
{
   (SecM_EsLibHashInitType)esl_initSHA256,                                          /* PRQA S 0313 */ /* MD_SecVerificationLib_0313 */
   (SecM_EsLibHashUpdateType)esl_updateSHA256,                                      /* PRQA S 0313 */ /* MD_SecVerificationLib_0313 */
   (SecM_EsLibHashFinalizeType)esl_finalizeSHA256,                                  /* PRQA S 0313 */ /* MD_SecVerificationLib_0313 */
   {
      sizeof(SecM_WorkspaceHashSha256Type),
      (SecM_LengthType)SEC_MEMBER_OFFSET(SecM_WorkspaceHashSha256Type, esLib),      /* PRQA S 0306 */ /* MD_SecVerificationLib_0306_2 */
      ESL_SIZEOF_SHA256_DIGEST
   }
};

/** Configuration for SHA-256 RSA V1.5 signature primitive */
V_MEMROM0 static V_MEMROM1 SecM_VerifySigPrimitiveType V_MEMROM2 sigConfigRsaV15Sha256 =
{
   (SecM_EsLibSigInitType)esl_initVerifyRSASHA256_V15,                              /* PRQA S 0313 */ /* MD_SecVerificationLib_0313 */
   (SecM_EsLibSigUpdateType)esl_updateVerifyRSASHA256_V15,                          /* PRQA S 0313 */ /* MD_SecVerificationLib_0313 */
   (SecM_EsLibSigFinalizeType)esl_finalizeHashVerifyRSASHA256_V15,                  /* PRQA S 0313 */ /* MD_SecVerificationLib_0313 */
   (SecM_EsLibSigVerifyType)esl_verifySigVerifyRSASHA256_V15,                       /* PRQA S 0313 */ /* MD_SecVerificationLib_0313 */
   {
      sizeof(SecM_WorkspaceSigRsaV15Sha256Type),
      (SecM_LengthType)SEC_MEMBER_OFFSET(SecM_WorkspaceSigRsaV15Sha256Type, esLib), /* PRQA S 0306 */ /* MD_SecVerificationLib_0306_2 */
      ESL_SIZEOF_SHA256_DIGEST
   }
};

/* PRQA L:TAG_SecVerificationLib_3218_1 */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  SecM_CompareDigest
 *********************************************************************************************************************/
/*! \brief         Compare calculated messages digest against provided one
 *  \details       Digests have to be of same size, byte-wise comparison performed
 *  \param[in]     pCalculatedData Pointer to calculated message digest
 *  \param[in]     calculatedSize Size of calculated message digest
 *  \param[in]     pProvidedData Pointer to provided message digest
 *  \param[in]     providedSize Size of provided message digest
 *  \return        SECM_VER_OK if digest values match
 *                 SECM_VER_ERROR if digest have differing size
 *                 SECM_VER_SIG if digest values differ
 *********************************************************************************************************************/
static SecM_StatusType SecM_CompareDigest( SecM_ConstRamDataType pCalculatedData, SecM_LengthType calculatedSize,
   SecM_ConstRamDataType pProvidedData, SecM_LengthType providedSize )
{
   SecM_StatusType result;
   SecM_LengthType index;

   result = SECM_VER_ERROR;

   /* Sizes have to be equal for a match */
   if (calculatedSize == providedSize)
   {
      /* Assume match */
      result = SECM_VER_OK;

      /* Compare all bytes */
      for (index = 0u; index < calculatedSize; index++)
      {
         if (pCalculatedData[index] != pProvidedData[index])
         {
            /* Values differ, report error */
            result = SECM_VER_SIG;

            break;
         }
      }
   }

   return result;
}

/**********************************************************************************************************************
 *  SecM_GetWorkspace
 *********************************************************************************************************************/
/*! \brief         Verify workspace information stored in verifcation parameter
 *  \param[in]     pVerifyParam Pointer to parameter structure for signature verification
 *                   Member currentHash must contain reference to buffer used as workspace
 *  \param[in]     pPrimitive Pointer to parameter structure for hash primitive
 *  \param[out]    pWorkspace Pointer to workspace information
 *********************************************************************************************************************/
static void SecM_GetWorkspace( const V_MEMRAM1 SecM_SignatureParamType V_MEMRAM2 V_MEMRAM3 * pVerifyParam,
   V_MEMROM1 SecM_VerifyPrimitiveType V_MEMROM2 V_MEMROM3 * pPrimitive,
   V_MEMRAM1 SecM_WorkspaceType V_MEMRAM2 V_MEMRAM3 * pWorkspace )
{
   /* Provided workspace has to be large enough */
   if (pVerifyParam->currentHash.length < pPrimitive->workspaceSize)
   {
      /* Invalid workspace configuration */
      pWorkspace->data  = SEC_DEFAULT_WORKSPACE;
      pWorkspace->size  = SEC_DEFAULT_WORKSPACE_SIZE;
   }
   else
   {
      /* Return provided workspace information */
      pWorkspace->data  = (SecM_WorkspacePtrType)pVerifyParam->currentHash.sigResultBuffer; /* PRQA S 0306 */ /* MD_SecVerificationLib_0306_1 */
      pWorkspace->size  = (SecM_LengthType)pVerifyParam->currentHash.length;
   }
}

/**********************************************************************************************************************
 *  SecM_InitEsWorkspace
 *********************************************************************************************************************/
/*! \brief         Verify ESLib workspace and initialize if necessary
 *  \details       Checks whether given workspace fullfills the size requirements of the verification primitive
 *                 During operation SEC_HASH_INIT the workspace header is initialized
 *  \param[in]     pVerifyParam Pointer to parameter structure for signature verification
 *                   Member currentHash must contain reference to buffer used as workspace
 *                   (size at least equal pPrimitive->common.workspaceSize)
 *  \param[in]     pPrimitive Pointer to parameter structure for hash primitive
 *  \return        Pointer to ESLib workspace if successfull
 *                 SEC_ES_WORKSPACE_NULL otherwise
 *********************************************************************************************************************/
static V_MEMRAM1 eslt_WorkSpace V_MEMRAM2 V_MEMRAM3 * SecM_InitEsWorkspace(
   const V_MEMRAM1 SecM_SignatureParamType V_MEMRAM2 V_MEMRAM3 * pVerifyParam,
   V_MEMROM1 SecM_VerifyPrimitiveType V_MEMROM2 V_MEMROM3 * pPrimitive )
{
   SecM_WorkspaceType workspaceInfo;
   SecM_RamDataType pWorkspaceData;
   V_MEMRAM1 eslt_WorkSpace V_MEMRAM2 V_MEMRAM3 * pWorkspaceEs;

   /* Get workspace information */
   SecM_GetWorkspace(pVerifyParam, pPrimitive, &workspaceInfo);
   /* Initialize ESLib workspace pointer */
   pWorkspaceEs = SEC_ES_WORKSPACE_NULL;

   /* Valid workspace present? */
   if (SEC_DEFAULT_WORKSPACE_SIZE != workspaceInfo.size)
   {
      /* Interpret workspace as byte pointer */
      pWorkspaceData = (SecM_RamDataType)workspaceInfo.data;                                                            /* PRQA S 0310 */ /* MD_SecVerificationLib_0310 */
      /* ESLib workspace located behind basic workspace information, apply offset */
      pWorkspaceEs = (V_MEMRAM1 eslt_WorkSpace V_MEMRAM2 V_MEMRAM3 *)&pWorkspaceData[pPrimitive->eslibWorkspaceOffset]; /* PRQA S 0310 */ /* MD_SecVerificationLib_0310 */ /* PRQA S 3305 */ /* MD_SecVerificationLib_3305 */

      /* Special handling for initialization operation */
      if (SEC_HASH_INIT == pVerifyParam->sigState)
      {
         /* Init ESLib workspace header */
         if (ESL_ERC_NO_ERROR != esl_initWorkSpaceHeader(&pWorkspaceEs->header,
            workspaceInfo.size - (pPrimitive->eslibWorkspaceOffset + sizeof(eslt_WorkSpaceHeader)), pVerifyParam->wdTriggerFct))
         {
            /* Initialization failed, indicate error by returning null pointer */
            pWorkspaceEs = SEC_ES_WORKSPACE_NULL;
         }
      }
   }

   return pWorkspaceEs;
}

/**********************************************************************************************************************
 *  SecM_VerifyHash
 *********************************************************************************************************************/
/*! \brief         Calculate and verify hash using given hash algorithm
 *  \details       See SecM_VerifySignature for details
 *  \param[in]     pVerifyParam Pointer to parameter structure for signature verification
 *                   Member currentHash must contain reference to buffer used as workspace
 *                   (size at least equal pPrimitive->common.workspaceSize)
 *  \param[in]     pPrimitive Pointer to parameter structure for hash primitive
 *  \return        SECM_VER_OK if verification operation successful
 *                 SECM_VER_ERROR if error occured during verification
 *                 SECM_VER_CRC if CRC verification failed
 *                 SECM_VER_SIG if HMAC/Signature verification failed
 *********************************************************************************************************************/
static SecM_StatusType SecM_VerifyHash( V_MEMRAM1 SecM_SignatureParamType V_MEMRAM2 V_MEMRAM3 * pVerifyParam,  /* PRQA S 3673 */ /* MD_SecVerificationLib_3673 */
   V_MEMROM1 SecM_VerifyHashPrimitiveType V_MEMROM2 V_MEMROM3 * pPrimitive )
{
   SecM_StatusType      result;
   SecM_WorkspaceType   workspaceInfo;
   V_MEMRAM1 eslt_WorkSpace V_MEMRAM2 V_MEMRAM3 * pWorkspaceEs;
   eslt_ErrorCode errorCode;

   result      = SECM_VER_ERROR;
   errorCode   = ESL_ERC_ERROR;
   /* Check ESLib workspace */
   pWorkspaceEs   = SecM_InitEsWorkspace(pVerifyParam, &pPrimitive->common);

   if (SEC_ES_WORKSPACE_NULL != pWorkspaceEs)
   {
      /* Get workspace information */
      SecM_GetWorkspace(pVerifyParam, &pPrimitive->common, &workspaceInfo);

#if defined( SEC_ENABLE_VERIFICATION_DATA_LENGTH )
      /* Update member currentDataLength of verification parameter */
      SecM_UpdateDataLength(pVerifyParam);
#endif /* SEC_ENABLE_VERIFICATION_DATA_LENGTH */

      switch (pVerifyParam->sigState)
      {
         case SEC_HASH_INIT:
         {
            /* Init verification */
            errorCode = pPrimitive->init(pWorkspaceEs);

            break;
         }
         case SEC_HASH_COMPUTE:
         {
            /* Update verification with data in buffer */
            errorCode = pPrimitive->update(pWorkspaceEs, pVerifyParam->sigByteCount, pVerifyParam->sigSourceBuffer);

            break;
         }
         case SEC_HASH_FINALIZE:
         {
            /* Finalize verification
               Remark: Digest value located at very beginning of workspace */
            errorCode = pPrimitive->finalize(pWorkspaceEs, workspaceInfo.data->digest);

            break;
         }
         case SEC_SIG_VERIFY:
         {
            /* Finalize verification by comparing calculated and provided digest
               Remark: Digest value located at very beginning of workspace */
            result = SecM_CompareDigest(workspaceInfo.data->digest, pPrimitive->common.digestLength,
               pVerifyParam->sigSourceBuffer, pVerifyParam->sigByteCount );

            break;
         }
         default:
         {
            break;
         }
      }
   }

   /* Remap error code in case ESLib operation was successful */
   if (ESL_ERC_NO_ERROR == errorCode)
   {
      result = SECM_VER_OK;
   }
   else
   {
      /* ESLib operation failed or result was assigned directly */
   }

   return result;
}

/**********************************************************************************************************************
 *  SecM_VerifySig
 *********************************************************************************************************************/
/*! \brief         Calculate and verify RSA signature using given hash algorithm
 *  \details       See SecM_VerifySignature for details
 *  \param[in]     pVerifyParam Pointer to parameter structure for signature verification
 *                   Member currentHash must contain reference to buffer used as workspace
 *                   (size at least equal pPrimitive->common.workspaceSize)
 *  \param[in]     pPrimitive Pointer to parameter structure for hash primitive
 *  \return        SECM_VER_OK if verification operation successful
 *                 SECM_VER_ERROR if error occured during verification
 *                 SECM_VER_CRC if CRC verification failed
 *                 SECM_VER_SIG if HMAC/Signature verification failed
 *********************************************************************************************************************/
static SecM_StatusType SecM_VerifySig( V_MEMRAM1 SecM_SignatureParamType V_MEMRAM2 V_MEMRAM3 * pVerifyParam,   /* PRQA S 3673 */ /* MD_SecVerificationLib_3673 */
   V_MEMROM1 SecM_VerifySigPrimitiveType V_MEMROM2 V_MEMROM3 * pPrimitive )
{
   SecM_StatusType      result;
   SecM_WorkspaceType   workspaceInfo;
   V_MEMRAM1 eslt_WorkSpace V_MEMRAM2 V_MEMRAM3 *     pWorkspaceEs;
   V_MEMRAM1 SecM_AsymKeyType V_MEMRAM2 V_MEMRAM3 *   pKeyPair;
   eslt_ErrorCode errorCode;

   errorCode      = ESL_ERC_ERROR;
   /* Check ESLib workspace */
   pWorkspaceEs   = SecM_InitEsWorkspace(pVerifyParam, &pPrimitive->common);

   if (SEC_ES_WORKSPACE_NULL != pWorkspaceEs)
   {
      /* Get workspace and key information */
      SecM_GetWorkspace(pVerifyParam, &pPrimitive->common, &workspaceInfo);
      pKeyPair = (V_MEMRAM1 SecM_AsymKeyType V_MEMRAM2 V_MEMRAM3 *)pVerifyParam->key;

#if defined( SEC_ENABLE_VERIFICATION_DATA_LENGTH )
      /* Update member currentDataLength of verification parameter */
      SecM_UpdateDataLength(pVerifyParam);
#endif /* SEC_ENABLE_VERIFICATION_DATA_LENGTH */

      switch (pVerifyParam->sigState)
      {
         case SEC_HASH_INIT:
         {
            /* Init verification */
            errorCode = pPrimitive->init(pWorkspaceEs, pKeyPair->shared.size, pKeyPair->shared.data,
               pKeyPair->individual.size, pKeyPair->individual.data);

            break;
         }
         case SEC_HASH_COMPUTE:
         {
            /* Update verification with data in buffer */
            errorCode = pPrimitive->update(pWorkspaceEs, pVerifyParam->sigByteCount, pVerifyParam->sigSourceBuffer);

            break;
         }
         case SEC_HASH_FINALIZE:
         {
            /* Finalize verification
               Remark: Digest value located at very beginning of workspace */
            errorCode = pPrimitive->finalize(pWorkspaceEs, workspaceInfo.data->digest);

            break;
         }
         case SEC_SIG_VERIFY:
         {
            /* Verify signature using previously calculated message digest
               Remark: Digest value located at very beginning of workspace */
            errorCode =  pPrimitive->verify(pWorkspaceEs, workspaceInfo.data->digest, pVerifyParam->sigByteCount,
               pVerifyParam->sigSourceBuffer);

            break;
         }
         default:
         {
            break;
         }
      }
   }

   /* Remap ESLib error code */
   switch (errorCode)
   {
      case ESL_ERC_NO_ERROR:
      {
         /* Verification successful */
         result = SECM_VER_OK;

         break;
      }
      case ESL_ERC_RSA_SIGNATURE_INVALID:
      {
         /* Verification failed */
         result = SECM_VER_SIG;

         break;
      }
      default:
      {
         /* Operation failed */
         result = SECM_VER_ERROR;

         break;
      }
   }

   return result;
}

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  SecM_VerifyHashSha256
 *********************************************************************************************************************/
/*! \brief         Calculate and verify hash using SHA-256 hash algorithm
 *  \details       See SecM_VerifySignature for details
 *  \param[in]     pVerifyParam Pointer to parameter structure for signature verification
 *                   Member currentHash must contain reference to buffer used as workspace
 *                   (size at least equal SecM_WorkspaceHashSha256Type)
 *  \return        SECM_VER_OK if verification operation successful
 *                 SECM_VER_ERROR if error occured during verification
 *                 SECM_VER_CRC if CRC verification failed
 *                 SECM_VER_SIG if HMAC/Signature verification failed
 *********************************************************************************************************************/
SecM_StatusType SecM_VerifyHashSha256( V_MEMRAM1 SecM_SignatureParamType V_MEMRAM2 V_MEMRAM3 * pVerifyParam )
{
   return SecM_VerifyHash(pVerifyParam, &hashConfigSha256);
}

/**********************************************************************************************************************
 *  SecM_VerifySigRsaV15Sha256
 *********************************************************************************************************************/
/*! \brief         Calculate and verify RSA V1.5 signature using SHA-256 hash algorithm
 *  \details       See SecM_VerifySignature for details
 *  \param[in]     pVerifyParam Pointer to parameter structure for signature verification
 *                   Member currentHash must contain reference to buffer used as workspace
 *                   (size at least equal SecM_WorkspaceSigSha256Type)
 *  \return        SECM_VER_OK if verification operation successful
 *                 SECM_VER_ERROR if error occured during verification
 *                 SECM_VER_CRC if CRC verification failed
 *                 SECM_VER_SIG if HMAC/Signature verification failed
 *********************************************************************************************************************/
SecM_StatusType SecM_VerifySigRsaV15Sha256( V_MEMRAM1 SecM_SignatureParamType V_MEMRAM2 V_MEMRAM3 * pVerifyParam )
{
   return SecM_VerifySig(pVerifyParam, &sigConfigRsaV15Sha256);
}

/**********************************************************************************************************************
 *  MISRA
 *********************************************************************************************************************/

/* Module specific MISRA deviations:

   MD_SecVerificationLib_3218:
     Reason: The configuration constants of this module are kept at a central location for a better overview and maintenance.
      Scope is larger than required (whole file instead of one function).
     Risk: No identifiable risk.
     Prevention: No prevention required.

   MD_SecVerificationLib_0313:
      Reason: Function with prototype (eslt_WorkSpace<primitive> *, ...) is cast to function pointer with prototype
       ( eslt_WorkSpace * ). Otherwise no common function pointer type could be defined for all functions of a certain
       primitives group (e.g. HMAC calculation).
       eslt_WorkSpace<primitive> is compatible with eslt_WorkSpace, so casting is no problem.
      Risk: Cast to incompatible function pointer type.
      Prevention: All changes to the affected code parts have to be reviewed by the module responsible.

   MD_SecVerificationLib_0306_1:
      Reason: HIS security module specification defines reference to buffer to be stored as unsigned 32-bit value instead
       of pointer type.
      Risk: The size of integer required to hold the result of a pointer cast is implementation defined.
      Prevention: 32 bit handles all current use-cases. Pay special attention when 64 bit ECUs are introduced.

   MD_SecVerificationLib_0306_2:
      Reason: Member address of struct located at address zero is used to determine offset of member relative to beginning
       of struct.
      Risk: No identifiable risk, as casted value isn't used as an actual address but only as an offset.
      Prevention: No prevention required.

   MD_SecVerificationLib_3410:
      Reason: Struct type passed as parameter to macro. Cannot be enclosed in parentheses.
       eslt_WorkSpace<primitive> is compatible with eslt_WorkSpace, so casting is no problem.
      Risk: No identifiable risk.
      Prevention: No prevention required.

   MD_SecVerificationLib_0310:
      Reason: References to workspaces are passed as generic pointer which is casted back to the actual type on use.
      Risk: Passing of pointer with less strict alignment than required by workspace type.
      Prevention: Generic pointer type references basic structure to enforce alignment required by most workspace types.

   MD_SecVerificationLib_3305:
      Reason: Byte-oriented pointer is cast to ESLib workspace pointer which enforces a stricter alignment depending on
       the platform. Correct alignment is ensured by use of aligned basic workspace structure and offset taken from
       actual workspace type.
      Risk: Incorrect workspace offset breaks alignment requirements.
      Prevention: All changes to the affected code parts have to be reviewed by the module responsible.

   MD_SecVerificationLib_3673:
     Reason: Parameter or parts of it are actually modified inside the calling hierarchy.
     Risk: No identifiable risk.
     Prevention: No prevention required.

   MD_SecVerificationLib_5120:
      Reason: References to workspaces are passed as generic pointer which is casted back to the actual type on use.
       Message digest has to be placed at the very beginning of the workspace. As length depends on primitive,
       offset of to ESLib workspace differs.
      Risk: Struct type and member have to be compatible, otherwise use of macro can lead to undefined behavior.
      Prevention: All changes to the affected code parts have to be reviewed by the module responsible.
*/

/**********************************************************************************************************************
 *  END OF FILE: SEC_VERIFICATIONLIB.C
 *********************************************************************************************************************/
