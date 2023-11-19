/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file          fbl_hdr.c
 *  \brief         Secure Fbl header parsing module
 *                 Declaration of functions, variables, and constants
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
/**********************************************************************************************************************/

/***********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  --------------------------------------------------------------------------------------------------------------------
 *  Andreas Wenckebach            AWh           Vector Informatik GmbH
 *  Dennis O'Donnell              Dod           Vector CANtech, Inc.
 *  Ralf Haegenlaeuer             HRf           Vector Informatik GmbH
 *  Vijay Natarajan               Vna           Vector CANtech, Inc.
 *  Sebastian Loos                Shs           Vector Informatik GmbH
 *  Thomas Mueller                ThM           Vector Informatik GmbH
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id       Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  01.00.00   2014-01-31  AWh     -               First release
 *  01.01.00   2014-03-04  AWh     ESCAN00073546   Add Non readable device configuration
 *                                 ESCAN00073567   Change to LibMem 2.xx interface; Minor Improvements
 *                                 ESCAN00073883   Cal module may be considered invalid, when it actually is valid
 *                                 ESCAN00073910   Enable Multi processor use case
 *                         Dod     ESCAN00074067   Compile error when cal disabled and verify integrity word enabled
 *  01.02.00   2014-07-14  Dod     ESCAN00075058   Update latest version to be compatible with GM SLP6
 *                         AWh     ESCAN00075272   GapFill need to use the at least FLASH_SEGMENT_SIZE buffer size
 *                                 ESCAN00075353   Track Cal Part. Erase Segments for Gap Fill on Cal Part. update
 *                                 ESCAN00075678   SLP6: Add support for Pec Error Err_Undefined
 *                                 ESCAN00075685   SLP6: Do not clear Pec Error on retries
 *                                 ESCAN00076569   SLP6: Download with more than one calibration in partition fails
 *                                 ESCAN00076911   Sba ticket is stored and read inclusive the data type
 *                                 ESCAN00077055   Late initialization of pecErrorCode
 *  01.03.00   2014-09-11  HRf     ESCAN00078350   Add cast to second parameter when FblMemGetInteger is called in
 *                                                 the context of FblHdrInitFblRomCheck
 *  01.04.00   2014-11-05  AWh     ESCAN00078423   No changes
 *                         Vna     ESCAN00078603   No changes
 *                         AWh     ESCAN00078852   Compile error: FblMemGetInteger 2nd par need to be pointer type
 *                                 ESCAN00078855   No changes
 *                                 ESCAN00079183   SLP6: incorrect handling of calibration modules in wrong order
 *                                 ESCAN00079328   SLP5: Order of calibration partition members is not checked
 *  02.00.00   2015-01-15  AWh     ESCAN00079553   Add Compression Interface
 *  02.00.01   2015-01-16  AWh     ESCAN00080628   Allow overwriting Data compression type
 *  02.01.00   2015-02-09  AWh     ESCAN00081143   Programming failure on transfer data
 *  02.02.00   2015-03-12  AWh     ESCAN00080365   No changes
 *                         AWh     ESCAN00081761   SLP6: Err_Undefined reported instead of Err_Compression
 *                         AWh     ESCAN00081762   Multi Processor configuration: Allow for remote Gap Fill and remote
 *                                                 signature calculation configuration.
 *  02.03.00   2015-03-16  AWh     ESCAN00081831   SLP5: Application may get started with wrong cal data
 *                                 ESCAN00081857   Compression data underflow shall lead to Pec error
 *                                                 Err_MoreDataExpected instead of Err_LengthExceeded.
 *  02.04.00   2015-06-10  AWh     ESCAN00083032   SLP6: Valid SBAT is not found valid when FBL receives messages during startup
 *                                 ESCAN00083040   In case of invalid SBAT loaded, The Fbl can report a wrong programming error code
 *                                 ESCAN00083073   No changes
 *  02.04.01   2015-06-12  AWh     ESCAN00083383   Remove superfluous Rcrrp sometimes seen on service 36 on the bus
 *  02.05.00   2015-07-23  AWh     ESCAN00083538   Check and set partition erased state.
 *                                 ESCAN00083750   Stored App-NBID is evaluated even though valid SBAT is loaded
 *                                 ESCAN00083822   The bootloader does not download a compressed application and reports
 *                                                 Programming Error code Err_LengthExceeded
 *                                 ESCAN00083993   SLP5: Compiler error: Missing argument for macro DiagSetError
 *  02.06.00   2016-02-19  AWh     ESCAN00088277   No changes
 *                                 ESCAN00088419   Add basic NBID handling
 *                                 ESCAN00088430   Address precision lost and conversion in argument warnings
 *  02.06.01   2016-03-18  AWh     ESCAN00088818   Cal file not aligned to FLASH_SEGMENT_SIZE will lead to programming error
 *                                 ESCAN00088973   No changes
 *                                 ESCAN00088984   GM SLP5: Array access out of bounds
 *  02.07.00   2016-03-17  Shs     ESCAN00088954   Added support for PATAC SLP2
 *  02.07.01   2016-04-04  ThM     ESCAN00089202   Fixed direct access problem to rom section
 *                         AWh     ESCAN00089235   No changes
 *  03.00.00   2016-04-08  AWh     ESCAN00089528   Complete rework to allow usage for extended use cases.
 *                                 ESCAN00089532   GB6002: In case of invalid Cal partition Id, Pec HdrErr_GetCalInfo
 *                                                  is set instead of HdrErr_PartitionId
 *  03.01.00   2016-08-04  HRf     ESCAN00091043   Code does not compile due to missing define HDR_DOWNLOAD_SOURCE_IDLE
 *                         AWh     ESCAN00091147   Changes to allow usage of non-Kb main
 *                                 ESCAN00091325   Allow Mcmp config with several slaves for non explicit erase use case
 *                                 ESCAN00091279   No changes
 *                                 ESCAN00089426   Compiler warning: variable "calModInfoOffset" is possibly uninitialized
 *  03.01.01   2016-10-04  AWh     ESCAN00092167   SLP5: No compressed download through pass through interface possible
 *  03.02.00   2016-10-31  ThM     ESCAN00092596   Added support for Lzma decompression
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

#include "fbl_inc.h"
#include "SecMPar.h"

/***********************************************************************************************************************
 *  VERSION
 **********************************************************************************************************************/

#if ( FBLDIAGHDR_GM_VERSION != 0x0302 ) || \
    ( FBLDIAGHDR_GM_RELEASE_VERSION != 0x00u )
# error "Error in fbl_hdr.c: Source and Header file are inconsistent!"
#endif

#if ( FBLDIAGHDR_GM_VERSION != _FBLDIAGHDR_OEM_VERSION ) || \
    ( FBLDIAGHDR_GM_RELEASE_VERSION != _FBLDIAGHDR_OEM_RELEASE_VERSION )
# error "Error in fbl_hdr.c: Source and v_ver.h are inconsistent!"
#endif

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

#if ( FBL_MTAB_NO_OF_BLOCKS > 1 )
#else
# define FblHdrGetBasePartIdFromModuleId(x)  ((vuint8)PARTID_OPSW_LOCAL) /* PRQA S 3453 */ /* MD_MSR_19.7 */
#endif

#define HDR_DOWNLOAD_MODULE_NOT_ACCEPTED ((tTpDataType)0u)
#define HDR_FIRST_CAL_NBR ((vuint8)0x01u)
#define HDR_NO_PART_ID_DEFINED ((vuint8)0x00u)

/* PRQA S 3458 TAG_FblHdr_3458 */ /* MD_MSR_19.4 */
#if defined( FBL_HDR_ENABLE_EXPLICIT_ERASE ) || !defined( FBL_ENABLE_CALIBRATION_MODULES )
# define FblHdrStateSetIdle()                  {/* (1) */\
                                                  rcvBuffHandle.progStepState = HDR_PS_IDLE;\
                                               }
#else
# define FblHdrStateSetIdle()                  {/* (1) */\
                                                  rcvBuffHandle.progStepState = HDR_PS_IDLE;\
                                                  calPartActive = HDR_NO_CALPART_ACTIVE; /* If no explicit erase, switch back from cal sequence is required. */\
                                               }
#endif /* FBL_HDR_ENABLE_EXPLICIT_ERASE || !FBL_ENABLE_CALIBRATION_MODULES */
#define FblHdrStateSetDownloadExpected()       {/* (2) */\
                                                  rcvBuffHandle.progStepState = HDR_PS_DOWNLOAD_EXPECTED;\
                                               }
#define FblHdrStateSetTransferHdrExpected()    {/* (3) */\
                                                  rcvBuffHandle.progStepState = HDR_PS_HDR_EXPECTED;\
                                               }
#define FblHdrStateSetTransferNoHdrExpected()  {/* (4) */\
                                                  rcvBuffHandle.progStepState = HDR_PS_TRANSFER_EXPECTED;\
                                               }
#define FblHdrStateSetTransferEndExpected()    {/* (5) */\
                                                  rcvBuffHandle.progStepState = HDR_PS_TRANSFEREND_EXPECTED;\
                                               }

# define FblHdrStateSetUpdatePsiExpected()     {/* (6) */\
                                                  rcvBuffHandle.progStepState = HDR_PS_UPDATEPSI_EXPECTED;\
                                               }

#define MAX_FBL_HEADER_COMPARE_DATA          16u
#define MAX_FBL_HEADER_GET_INTEGER_DATA       4u

/* Sbi-flag */
#define SBI_FLAG_A_VALID ((vuint32)0xA5163829u)
#define SBI_FLAG_B_VALID (FblInvert32Bit(SBI_FLAG_A_VALID))

#define FblHdrClrSBITicketValid()  {\
                                      sbiFlag.flagA = 0u;\
                                      sbiFlag.flagB = 0u;\
                                   }
#define FblHdrSetSBITicketValid()  {\
                                      sbiFlag.flagA = SBI_FLAG_A_VALID;\
                                      sbiFlag.flagB = SBI_FLAG_B_VALID;\
                                   }
#define FblHdrGetSBITicketValid()  ( (SBI_FLAG_A_VALID == sbiFlag.flagA) && (SBI_FLAG_B_VALID == sbiFlag.flagB) )

/* Compatibility macros */
#if !defined( FblGetComDat_AddrRegionStartAddr )
# define FblGetComDat_AddrRegionStartAddr(element) (&FblHeader.AddressRegions[(element) * (HDR_REGADDR_LEN + HDR_REGLEN_LEN)])  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#endif
#if !defined( FblGetComDat_AddrRegionLength )
# define FblGetComDat_AddrRegionLength(element) (&FblHeader.AddressRegions[((element) * (HDR_REGADDR_LEN + HDR_REGLEN_LEN)) + HDR_REGADDR_LEN])  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#endif
/* PRQA L:TAG_FblHdr_3458 */

/***********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

typedef enum
{
   HDR_ECUID_DO_ZEROCHECK,  /* check against 16 byte zero array */
   HDR_ECUID_SKIP_ZEROCHECK /* no check against 16 byte zero array */
} tEcuIdZeroCheck;

typedef enum
{
   HDR_NO_CALPART_ACTIVE,   /* Init state */
   HDR_CALPART_START_SEQU,  /* Cal partition download initiated ( on erase in case of EXPLICIT_ERASE, pass through state if no EXPLICIT_ERASE) */
   HDR_CALPART_ACTIVE       /* Cal partition download ongoing */
} tCalPartState;

/***********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

V_MEMRAM0 V_MEMRAM1 tDownloadModuleInfo V_MEMRAM2 parsedModuleInfo; /**< Externally accessible Header info of currently parsed module */
V_MEMRAM0 V_MEMRAM1 tBootInitStatus V_MEMRAM2 fblHdrBis; /**< Externally accessible Boot Initialization Status */
V_MEMRAM0 V_MEMRAM1 tHdrRcvBuffHdl V_MEMRAM2 rcvBuffHandle; /**< store current buffer processing state */
V_MEMRAM0 V_MEMRAM1_NEAR FL_SegmentInfoType V_MEMRAM2_NEAR currDataSegInfo; /**< Actual region for flash programming */
V_MEMRAM0 V_MEMRAM1_NEAR vuint16 V_MEMRAM2_NEAR errPecExtErrorCode; /**< Current Program error code */
V_MEMRAM0 V_MEMRAM1_NEAR tLogBlockErasedState V_MEMRAM2_NEAR logBlockPartErasedStates[FBL_MTAB_NO_OF_BLOCKS]; /**< erased State Information per application partitition */
V_MEMRAM0 V_MEMRAM1_NEAR vuint8 V_MEMRAM2_NEAR fblProgrammedState; /**< Bit-flags containing results of memory-integrity checks and download under-run condition. */

# if defined( MCMP_MODE_SLAVE ) || defined( FBL_HDR_ENABLE_MEMSEGMENT_VAR )
V_MEMRAM0 V_MEMRAM1_NEAR vsint16 V_MEMRAM2_NEAR memSegment; /**< Index to currently processed FlashBlock */
# endif /* MCMP_MODE_SLAVE */

/***********************************************************************************************************************
 *  LOCAL DATA
 **********************************************************************************************************************/

# define FBL_HDR_SBIFLAG_START_SEC_DATA
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
V_MEMRAM0 static V_MEMRAM1 tSBIFlag V_MEMRAM2 sbiFlag; /**< Signature Bypass active? */
# define FBL_HDR_SBIFLAG_STOP_SEC_DATA
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#if defined( FBL_HDR_ENABLE_PASSTHROUGH )
V_MEMRAM0 static V_MEMRAM1 tHdrInterfaceType V_MEMRAM2 requSequType; /**< Store the requested Sequence Type */
V_MEMRAM0 static V_MEMRAM1 tFblResult V_MEMRAM2 isNewConnection;  /**< connection change happened? */ /* PRQA S 3218 */ /* MD_FblHdr_3218 */
#endif

V_MEMRAM0 static V_MEMRAM1 tNBIDInfo V_MEMRAM2 appNBIDInfo; /**< Application Not Before ID Info Data */
V_MEMRAM0 static V_MEMRAM1 tNBIDInfo V_MEMRAM2 keyNBIDInfo; /**< Key Not Before ID Info Data */

#if defined( FBL_ENABLE_CALIBRATION_MODULES )
V_MEMRAM0 static V_MEMRAM1 tCalPartitionInfo V_MEMRAM2 calPartitionInfo; /**< Externally accessible partition info of currently parsed module */
V_MEMRAM0 static V_MEMRAM1 tCalInfo V_MEMRAM2 calInfo; /**< Cal module Info */
V_MEMRAM0 static V_MEMRAM1 tCalPartState V_MEMRAM2 calPartActive; /**< Cal partition is currently updated */
#endif
#if defined( FBL_ENABLE_COMPRESSION_MODE )
V_MEMRAM0 static V_MEMRAM1 tFblLength V_MEMRAM2 compressedLength;  /* Length of compressed data content */
#endif
V_MEMRAM0 static V_MEMRAM1 vuint8 V_MEMRAM2 msgDigest[HDR_MSGDIGEST_LEN]; /**< Message Digest received in signed header and stored during module download */
V_MEMRAM0 static V_MEMRAM1 tPartitionBlockList V_MEMRAM2 partBlockList; /**< List of Flashblocks used by partitions */

/* Variables used to parse reception buffer */
V_MEMRAM0 static V_MEMRAM1 vuint8  V_MEMRAM2 currDataSegIdx; /**< Index to data segment currently programmed */

V_MEMRAM0 static V_MEMRAM1 tFblMemSegmentInfo V_MEMRAM2 currMemLibSegment; /**< Segment currently processed in MemLib */
V_MEMRAM0 static V_MEMRAM1 tFblMemBlockInfo V_MEMRAM2 blockInfo; /**< block currently processed in MemLib */
#if defined( FBL_ENABLE_SYSTEM_CHECK )
V_MEMRAM0 static V_MEMRAM1 tFblLength V_MEMRAM2 currDataSegInitialLen;    /**< Initial len of current data segment */
#endif
#if ( RAM_HEADER_LENGTH > 0 )
# define FBL_HDR_RAMHEADER_START_SEC_DATA
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
V_MEMRAM0 V_MEMRAM1 vuint8 V_MEMRAM2 FblRamHeader[RAM_HEADER_LENGTH]; /**< Header in RAM used for Sba ticket parsing, compression */
# define FBL_HDR_RAMHEADER_STOP_SEC_DATA
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
#endif
#if defined( FBL_HDR_ENABLE_MULTIPLE_DOWNLOAD_SOURCES )
V_MEMRAM0 static V_MEMRAM1 tDownloadSource V_MEMRAM2 currDownloadSource; /**< Source of current ongoing partition download. */
#endif

/***********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

static tFblResult FblHdrCheckMtabIncludesSegment( vuint8 partId, const V_MEMRAM1 FL_SegmentInfoType V_MEMRAM2 V_MEMRAM3 * segment );
static tFblResult FblHdrValidateSignature( const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * valRegStart, SecM_LengthType valRegLen,
                                           const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * signatureStart,
                                           const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * publicKey );
static tFblResult FblHdrChkIdPresent( vuint8 mid );
static const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * FblHdrCheckEnvelopesExtractSignedHeader( const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * rcvBuffer, tTpDataType rcvDataSize );
static vuint32 FblHdrGetIntegerFblHeader( vuint8 count, V_MEMROM1_FAR vuint8 V_MEMROM2_FAR V_MEMROM3 * pFblHeader );
static vuint32 FblHdrGetIntegerRom( tFblResult * result, vuintx count, tFblAddress addr );
static vuintx FblHdrPartSegmentListAddEntry( vuint8 partId, const FL_SegmentInfoType * segment );
#if defined( FBL_ENABLE_GAP_FILL )
static void FblHdrGetSortedApplRegions( FL_SegmentInfoType * retSeg, vuint8 regionNbr );
static tFblResult FblHdrFillRegion( tFblAddress inBeginAddr, tFblLength inLength );
static tFblBlockPartUsage FblHdrPartSegmentListGetBlockStatus( vsint16 blockN, vuint8 partId );
#endif
#if ( FBL_MTAB_NO_OF_BLOCKS > 1 )
static vuint8 FblHdrGetBasePartIdFromModuleId( vuint8 mid );
#endif

static tFblResult FblHdrInitRetry( tDownloadSource downloadSource );
static tTpDataType FblHdrTransferDataHeaderReceived( const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * signedHeaderAddr );
#if defined( FBL_ENABLE_CALIBRATION_MODULES )
static tFblResult FblHdrEraseCalPartition( vuint8 calPartId );
#endif
#if defined( FBL_HDR_ENABLE_EXPLICIT_TRANSFEREND )
#else
static tFblResult FblHdrTransferDataEnd( void );
#endif
#if defined( FBL_HDR_ENABLE_EXPLICIT_ERASE )
#else
static tFblResult FblHdrEraseRoutine( vuint8 partId, tDownloadSource downloadSource );
#endif
#if defined( FBL_HDR_ENABLE_EXPLICIT_UPDATEPSI )
#else
static tFblResult FblHdrUpdatePSI( vuint8 partId );
#endif
static tFblResult FblHdrPostFlashValidation( void );

static tTpDataType FblHdrProcessHeaderInfo( const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * signedHeaderAddr );
static void FblHdrSetMemLibMemBlockInfo( void );
static void FblHdrSetMemLibSegmentAddrLengthInfo( void );

/* Header Parsing */
static tFblResult FblHdrMemCompareFblHeader( const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * pRam, V_MEMROM1_FAR vuint8 V_MEMROM2_FAR V_MEMROM3 * pRom, vuint16 len );
static tFblResult FblHdrMemCompare( const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * pRam0, const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * pRam1, vuint16 len );
static tFblResult FblHdrCheckAndParseDownloadMid( const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * moduleAddr );
static tFblResult FblHdrCheckAppNBID( void );
static tFblResult FblHdrCheckAndParseSwLocInfo( const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * locInfoAddr );
static vuint8 const * FblHdrValidateSignerInfo( const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * signInfoAddr, vuint16 keyNbToCheck );
static tFblResult FblHdrCheckDownloadDcid( const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * dcidAddr );
static tFblResult FblHdrCheckEcuNameAndId( const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * ecuNameAddr, tEcuIdZeroCheck zeroCheck );
static tFblResult FblHdrInitBaseModInfo( vuint8 partId, V_MEMRAM1 tBaseModInfo V_MEMRAM2 V_MEMRAM3 * pBaseModInfo );
static tFblResult FblHdrValidatePlainHeader( void );
static tFblResult FblHdrCheckAndInitDownloadModuleInfo( const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * signedHdrAddr );
#if defined( FBL_ENABLE_CALIBRATION_MODULES )
static tFblResult FblHdrInitCalPartInfo( vuint8 calPartId, V_MEMRAM1 tCalPartitionInfo V_MEMRAM2 V_MEMRAM3 * pCalPartInfo );
static tFblResult FblHdrInitCalPartInfoAndCalInfo( vuint8 calMid, V_MEMRAM1 tCalPartitionInfo V_MEMRAM2 V_MEMRAM3 * pCalPartInfo,
                                            V_MEMRAM1 tCalInfo V_MEMRAM2 V_MEMRAM3 * pCalInfo );
static tFblResult FblHdrInitNextCalPartInfo( V_MEMRAM1 tCalPartitionInfo V_MEMRAM2 V_MEMRAM3 * pCalPartInfo );
static tFblResult FblHdrValidateCalPartInfo( void );
static tFblResult FblHdrValidateCalPartitionInfo_CalRegions( vuintx * incCals, const FL_SegmentInfoType * erReg, const tCalPartitionInfo * inPartInfo );
static tFblResult FblHdrSetCalModuleInfo( void );
# if defined( FBL_HDR_ENABLE_EXPLICIT_ERASE )
static tFblResult FblHdrPrepareDownloadCalPartionData( vuint8 calPartId );
# else
static tFblResult FblHdrPrepareDownloadCalPartionData( vuint8 calModId );
# endif /* FBL_HDR_ENABLE_EXPLICIT_ERASE */
#endif /* FBL_ENABLE_CALIBRATION_MODULES */
#if defined( FBL_ENABLE_CALIBRATION_MODULES ) || defined( FBL_ENABLE_VERIFY_INTEGRITY_WORD )
static tFblResult FblHdrGetRegionRomAndInc( FL_SegmentInfoType * segInfo, tFblAddress baseAddr, tFblAddress * relAddr );
#endif

/***********************************************************************************************************************
 *   LOCAL FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  FblHdrInitRetry
 **********************************************************************************************************************/
/*! \brief     Init when receiving Erase Mem Routine (explicit erase) / Request Download (non explicit erase)
 *             (possible scenario apart from standard flow: after error; retry programming sequence)
 **********************************************************************************************************************/
static tFblResult FblHdrInitRetry( tDownloadSource downloadSource )
{
   vuintx i;
   tFblResult retVal = kFblOk;

#if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   (void)downloadSource; /* PRQA S 3112 */ /* MD_MSR_14.2 */
#endif

#if defined( FBL_HDR_ENABLE_MULTIPLE_DOWNLOAD_SOURCES )
   if (downloadSource == currDownloadSource)
   {
      /* Retry while keeping downloadSource */
   }
   else if (!FblHdrStateIsIdle())
   {
      /* Download ongoing from different source, further download not accepted. */
      FblSetPecErr(HdrErr_ExtDownloadOngoing);
      retVal = kFblFailed;
   }
   else
   {
      /* Different download source requesting use of module, or still in init state */
      currDownloadSource = downloadSource;
   }

   if (kFblOk == retVal)
#endif
   {
      /* Init partBlockList */
      for(i = 0u; i < HDR_MAX_PARTITION_TRACKED_REGIONS ; i++)
      {
        FblHdrPollingTaskVoid();
        partBlockList.segList[i].startBlk = kFblHdrMemSegmNotFound;
      }

      FblClrPecErr(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

#if defined( FBL_HDR_ENABLE_EXPLICIT_ERASE )
# if defined( FBL_ENABLE_CALIBRATION_MODULES )
      calPartActive = HDR_NO_CALPART_ACTIVE;
# endif
#endif
   }

   return retVal;
}

#if defined( FBL_ENABLE_CALIBRATION_MODULES )
/***********************************************************************************************************************
 *  FblHdrValidateCalPartInfo_CalRegions
 **********************************************************************************************************************/
/*! \brief       Validate application partition info section Cal Regions
 *  \details     This is validated:
 *                - Check all calibration segments inside erase regions
 *  \pre         These preconditions need to apply (else kFblFailed is returned)
 *                - Any Cal Region is included in only one Partition Region
 *  \param[in]   incCals Number of Cals included in Erase Segments (receive current counter and update with current Region)
 *  \param[in]   erReg Erase Region to be checked
 *  \param[in]   inPartInfo partition information pointer
 *  \return      kFblOk=Success; kFblFailed=Failure
 **********************************************************************************************************************/
static tFblResult FblHdrValidateCalPartitionInfo_CalRegions( vuintx * incCals, const FL_SegmentInfoType * erReg,
                                                             const tCalPartitionInfo * inPartInfo )
{
   tFblResult retVal = kFblOk;
   FL_SegmentInfoType calProgSegment;
   vuintx locNbrCals = inPartInfo->nbrOfCals;
   /* Start with offset to Cal Module Info 1 */
   tFblAddress calModInfoOffset = HDR_NBROF_LEN + (inPartInfo->nbrOfRgns * HDR_REGINFO_LEN) + HDR_NBROF_LEN;

   /* Check which calibration regions are included in this block */
   while ((kFblOk == retVal) && (locNbrCals > 0u))
   {
      {
         FblHdrPollingTaskVoid();

         calModInfoOffset+= HDR_MID_LEN + HDR_DCID_LEN;
         retVal = FblHdrGetRegionRomAndInc(&calProgSegment, inPartInfo->partInfoAddr, &calModInfoOffset);

         /* Determine if Cal Segment is included in Erase Segment */
         if (FBL_UTIL_CHECK_RANGE_CONTAINED_LEN(calProgSegment.targetAddress, calProgSegment.length, erReg->targetAddress, erReg->length))
         {
            (*incCals)++;
         }

         locNbrCals--;
      }
   }
   return retVal;
}

/***********************************************************************************************************************
 *  FblHdrValidateCalPartInfo
 **********************************************************************************************************************/
/*! \brief       Validate application partition info section
 *  \details     This is validated:
 *                - Check all partition (erase) regions valid
 *                  + check inside mtab
 *                  + check overlapping appl blocks
 *                  + check in order and not overlapping with previous region
 *                - Check all calibration segments inside erase regions
 *                - Check PSI address not overlapping with segments
 *  \pre         These preconditions need to apply (else kFblFailed is returned)
 *                - Cal Space outside of Application Space (no calibration segments interleaving application segments)
 *                - Cal Region information sections are in order
 *                - Any Cal Region is included in only one Partition Region
 *  \return      kFblOk=Success; kFblFailed=Failure
 **********************************************************************************************************************/
static tFblResult FblHdrValidateCalPartInfo( void )
{
   tFblResult retVal;
   /* It is not allowed to have calibration regions between Application regions */
   tCalPartitionInfo locPartInfo;
   vuint8 partCounter;
   vuint8 opSwPartId = FblHdrGetBasePartId(parsedModuleInfo.partID);

   locPartInfo.partID = 0u;
   /* Initialize to min #partitions to allow enter loop. Gets overwritten after call to FblHdrInitCalPartInfo */
   retVal = FblHdrInitCalPartInfo(FblHdrGetFirstCalPartNbr(opSwPartId), &locPartInfo);

   for (partCounter = 0u; (partCounter < locPartInfo.nbrOfPrts) && (kFblOk == retVal); partCounter++)
   {
      /* Start with offset to Region Info 1 */
      tFblAddress regInfoOffset = HDR_NBROF_LEN;
      vuintx nBrOfRegions;
      vuintx calsInPartition = 0u;
      vsint16 previosRegLastCalBlock = kFblHdrMemSegmNotFound;

      retVal = FblHdrInitCalPartInfo(FblHdrGetFirstCalPartNbr(opSwPartId) + partCounter, &locPartInfo);
      nBrOfRegions = locPartInfo.nbrOfRgns;

      while ((kFblOk == retVal) && (nBrOfRegions > 0u))
      {
         FL_SegmentInfoType partErReg; /* Partition erase region */

         FblHdrPollingTaskVoid();

         retVal = FblHdrGetRegionRomAndInc(&partErReg, locPartInfo.partInfoAddr, &regInfoOffset);
         if (kFblOk == retVal)
         {
            /* Check blocks valid, in order and not overlapping with other partitions */
            vuintx regionIdx = FblHdrPartSegmentListAddEntry(locPartInfo.partID, &partErReg);

            if ((HDR_PARTITION_REGION_ERROR == regionIdx)
                 || (partBlockList.segList[regionIdx].startBlk <= previosRegLastCalBlock))
            {
               /* Pec error set externally */
               retVal = kFblFailed;
            }
            else
            {
               if (1u == nBrOfRegions)
               {
                  /* Reduce length by Presence Pattern, to automatically check no overlapping */
                  partErReg.length -= (2u * FBL_PP_SEGMENT_SIZE);
               }

               retVal = FblHdrValidateCalPartitionInfo_CalRegions(&calsInPartition, &partErReg, &locPartInfo);
               previosRegLastCalBlock = partBlockList.segList[regionIdx].endBlk;
               nBrOfRegions--;
            }
         }
      } /* while (nBrOfRegions > 0) */

      /* Check all calibration segments inside erase regions. */
      if (calsInPartition != locPartInfo.nbrOfCals)
      {
         /* Pec error set externally */
         retVal = kFblFailed;
      }
   } /* for (partCounter = 0 ..*/

   return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif

/***********************************************************************************************************************
 *  FblHdrValidatePlainHeader
 **********************************************************************************************************************/
/*! \brief       Check plain/programmed header to contain valid data
 *  \details     Checks Cal partition info and data segments
 *  \return      kFblOk=Success; kFblFailed=Failure
 **********************************************************************************************************************/
static tFblResult FblHdrValidatePlainHeader( void )
{
   tFblResult retVal;

#if defined( FBL_ENABLE_CALIBRATION_MODULES )
   if (MOD_TYPE_CAL == parsedModuleInfo.modType)
   {
      retVal = kFblOk;
   }
   else
#endif
   {
      vuint16 plainAppNBID;
      tBaseModInfo locBaseModInfo;

      retVal = FblHdrInitBaseModInfo(parsedModuleInfo.partID, &locBaseModInfo);

      if (kFblOk == retVal)
      {
         plainAppNBID = (vuint16)FblHdrGetIntegerRom(&retVal, HDR_NBID_LEN, locBaseModInfo.startAddr + P1_APP_NBID_IDX);

         if ((kFblOk != retVal) || (appNBIDInfo.received != plainAppNBID))
         {
            FblSetPecErr(HdrErr_ExtP1AppNBIDInconsistent); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
            retVal = kFblFailed;
         }
         else
         {
#if defined( FBL_ENABLE_VERIFY_INTEGRITY_WORD )
            FL_SegmentInfoType progRegion;

            /* Are the Regions programmed equal to the regions used? Read regions and compare segmentlist */
            if (parsedModuleInfo.segmentList.nrOfSegments != locBaseModInfo.nbrOfRgns)
            {
               FblSetPecErr(HdrErr_ExtP1SegNbrInconsistent); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
               retVal = kFblFailed;
            }
            else
            {
               vuintx i = 0u;
               locBaseModInfo.swLocInfoAddr+= HDR_NBROF_LEN;

               while ((locBaseModInfo.nbrOfRgns > 0u) && (kFblOk == retVal))
               {
                  FblHdrPollingTaskVoid();

                  retVal = FblHdrGetRegionRomAndInc(&progRegion,(tFblAddress)0u, &locBaseModInfo.swLocInfoAddr);
                  if (kFblOk == retVal)
                  {
                     if ((  parsedModuleInfo.segmentList.segmentInfo[i].targetAddress != progRegion.targetAddress)
                           || (parsedModuleInfo.segmentList.segmentInfo[i].length != progRegion.length))
                     {
                        FblSetPecErr(HdrErr_ExtP1SegRangeInconsistent); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
                        retVal = kFblFailed;
                     }
                  }
                  i++;
                  locBaseModInfo.nbrOfRgns--;
               }
            } /* if (parsedModuleInfo.segmentList.nrOfSegments != locBaseModInfo.nbrOfRgns) */

            if (kFblOk == retVal)
#endif
            {
               if (0u != FblHdrGetIntegerRom(&retVal, HDR_NBROF_LEN, locBaseModInfo.nbPartAddr))
               {
                  assertFblInternal(kFblOk == retVal, kFblAssertInternalIlegalState); /* PRQA S 2214 */ /* MD_FblAssert_Assertion */
#if defined( FBL_ENABLE_CALIBRATION_MODULES )
                  retVal = FblHdrValidateCalPartInfo();
                  if (kFblOk != retVal)
                  {
                     FblSetPecErr(HdrErr_CalRegion); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
                  }
#else
                  /* No Partition Information expected If calibration Modules are not supported. */
                  FblSetPecErr(HdrErr_Undefined); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
                  retVal = kFblFailed;
#endif
               } /* in else case result may be kFblFailed. This will set HdrErr_ExtGetAppInfoValidatePlain */
            }
         }
      }

      if ((kFblOk != retVal) && (HdrErr_NoError == FblGetExtPecErr()))
      {
         /* Set Default Error */
         FblSetPecErr(HdrErr_ExtGetAppInfoValidatePlain); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
      }
   } /* (MOD_TYPE_CAL != parsedModuleInfo.modType) */

   return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/***********************************************************************************************************************
 *  FblHdrPostFlashValidation
 **********************************************************************************************************************/
/*! \brief       Perform Application / Calibration Post Flash Validation Actions
 *  \return      kFblOk=Success; kFblFailed=Failure
 **********************************************************************************************************************/
static tFblResult FblHdrPostFlashValidation( void )
{
   tFblResult retVal;
   tFblMemStatus memLibResult;

   tFblMemBlockVerifyData  verifyData;
   tFblMemVerifyStatus verifyErrorCode = SECM_VER_ERROR;

   if (kFblOk != FblHdrValidatePlainHeader())
   {
      retVal = kFblFailed;
   }
   else
   {
      {
         verifyData.verifyDataInput.length  = 0u;
         verifyData.verifyDataInput.data    = V_NULL;
         verifyData.verifyDataPipe.length   = HDR_MSGDIGEST_LEN;
         verifyData.verifyDataPipe.data     = msgDigest;
      }
      verifyData.verifyDataOutput.length = HDR_MSGDIGEST_LEN;
      verifyData.verifyDataOutput.data   = msgDigest;

      memLibResult = FblMemBlockVerify(&verifyData, &verifyErrorCode);
      if ((kFblMemStatus_Ok != memLibResult) && (!((FblHdrGetSBITicketValid()) && (SECM_VER_SIG == verifyErrorCode))))
      {
         if (SECM_VER_SIG == verifyErrorCode)
         {
            FblSetPecErr(HdrErr_MD); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
         }
#if defined( FBL_ENABLE_VERIFY_INTEGRITY_WORD )
         else if (SECM_VER_CRC == verifyErrorCode)
         {
            /* Overwrite Pec */
            FblSetPecErr(HdrErr_GetInfo); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
         }
#endif
         else
         {
            FblSetPecErrIfNotYetSet((vuint16)memLibResult); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
         }
         retVal = kFblFailed;
      }
      else
      {
         /* LibmemRes == kFblOk || Flag overwhelming Digest ((SbaFlag == true) && (verifyError ==SECM_VER_SIG)) */
         retVal = ApplFblFillGaps();
         if (kFblOk != retVal)
         {
            /* this is used as Pec error */
            FblSetPecErr(HdrErr_ExtWriteUnusedBytesFailed); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
         }
      }
   } /* if (kFblOk != FblHdrValidatePlainHeader()()) */

   return retVal;
}

/***********************************************************************************************************************
 *  FblHdrValidateSignature
 **********************************************************************************************************************/
/*! \brief       validate Signature inside Application/Calibration/SBA Signed Header
 *  \param[in]   valRegStart Start of region to validate Signature on
 *  \param[in]   valRegLen Length of region to validate Signature on
 *  \param[in]   signatureStart Start of Signature (Validation region ends here)
 *  \param[in]   publicKey The public key to be used to validate signature
 *  \return      kFblOk=Success; kFblFailed=Failure
 **********************************************************************************************************************/
static tFblResult FblHdrValidateSignature( const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * valRegStart, SecM_LengthType valRegLen,
                                           const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * signatureStart,
                                           const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * publicKey )
{
   SecM_SignatureParamType sigParam;
   tFblResult retVal = kFblFailed;
   SecM_AsymKeyType asymKey;

   asymKey.shared.data = publicKey;
   asymKey.shared.size = sizeof(SecM_RsaMod);
   /* Macro APPL_FBL_READ_SEC_DATA can be used to copy SecM_Production_RsaExp to RAM if required */
   asymKey.individual.data = APPL_FBL_READ_SEC_DATA(SecM_RsaExp);
   asymKey.individual.size = sizeof(SecM_RsaExp);

   sigParam.key = &asymKey;

   sigParam.currentDataLength = V_NULL;
   sigParam.currentHash.length = 0u;
   sigParam.currentHash.sigResultBuffer = 0u;
   sigParam.wdTriggerFct = (FL_WDTriggerFctType) FblHdrPollingTaskVoid;
   sigParam.sigState = kHashInit;
   sigParam.sigSourceBuffer = V_NULL;
   sigParam.sigByteCount = 0u;

   if (SECM_OK == SecM_VerifySignature(&sigParam))
   {
      sigParam.sigState = kHashCompute;
      sigParam.sigSourceBuffer = valRegStart;
      sigParam.sigByteCount = valRegLen;

      if (SECM_OK == SecM_VerifySignature(&sigParam))
      {
         sigParam.sigState = kHashFinalize;
         sigParam.sigSourceBuffer = V_NULL;
         sigParam.sigByteCount = 0u;

         if (SECM_OK == SecM_VerifySignature(&sigParam))
         {
            sigParam.sigState = kSigVerify;
            sigParam.sigSourceBuffer = signatureStart;
            sigParam.sigByteCount = HDR_SIGNATURE_LEN;

            if (SECM_OK == SecM_VerifySignature(&sigParam))
            {
               /* pec error is set externally */
               retVal = kFblOk;
            }
         }
      }
   }

   return retVal;
}

/***********************************************************************************************************************
*  FblHdrMemCompareFblHeader
***********************************************************************************************************************/
/*! \brief      Compare RAM byte array content to ROM byte array content
*  \param[in]   pRam pointer to RAM for comparison
*  \param[in]   pRom pointer to ROM for comparison
*  \param[in]   len length of data for comparison
*  \return      kFblOk - Return this if none of the bytes within len are different between both arrays.
*               kFblFailed - Return this if at leas one of the bytes within len are different between both arrays.
***********************************************************************************************************************/
static tFblResult FblHdrMemCompareFblHeader( const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * pRam, V_MEMROM1_FAR vuint8 V_MEMROM2_FAR V_MEMROM3 * pRom, vuint16 len )
{
   vuint8 cmprBuffer[MAX_FBL_HEADER_COMPARE_DATA];
   tFblResult retVal;
   vuintx idx;

   if (0u == len)
   {
      retVal = kFblOk;
   }
   else
   {
      /* assertion if param len is larger than 16 bytes */
      assertFblInternal((len <= MAX_FBL_HEADER_COMPARE_DATA), kFblAssertInternalIlegalState); /* PRQA S 2214 */ /* MD_FblAssert_Assertion */

      /* copy fbl header data to ram cmprBuffer */
      for (idx = 0u; idx < len; idx++)
      {
         cmprBuffer[idx] = pRom[idx];
      }

      retVal = FblHdrMemCompare(pRam, cmprBuffer, len);
   }

   return retVal;
}

/***********************************************************************************************************************
 *  FblHdrMemCompare
 **********************************************************************************************************************/
/*! \brief       Compare RAM byte array content to other RAM byte array content
 *  \param[in]   pRam0 pointer1 to RAM for comparison
 *  \param[in]   pRam1 pointer2 to RAM for comparison
 *  \param[in]   len length of data for comparison
 *  \return      kFblOk - Return this if none of the bytes within len are different between both arrays.
 *               kFblFailed - Return this if at leas one of the bytes within len are different between both arrays.
 **********************************************************************************************************************/
static tFblResult FblHdrMemCompare( const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * pRam0, const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * pRam1, vuint16 len )
{
   tFblResult retVal = kFblOk;

   while (len > 0u)
   {
      FblHdrPollingTaskInLoop(len, 0x20u); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      len--;
      if (pRam0[len] != pRam1[len])
      {
         retVal = kFblFailed;
         break;
      }
   }

   return retVal;
}

/***********************************************************************************************************************
 *  FblHdrCheckMtabIncludesSegment
 **********************************************************************************************************************/
/*! \brief       Returns kFblOk if the Modules Logical Block table entry includes the FlashBlock, kFblFailed if not.
 *  \param[in]   partId partition Id of the module requesting for (any partition Id, including calibration)
 *  \param[in]   segment the Segment to check to be included
 *  \return      kFblOk=Success; kFblFailed=Failure
 **********************************************************************************************************************/
static tFblResult FblHdrCheckMtabIncludesSegment( vuint8 partId, const V_MEMRAM1 FL_SegmentInfoType V_MEMRAM2 V_MEMRAM3 * segment )
{
   tFblResult retVal = kFblFailed;
   vuint8 blockN = GetBlockNbrFromPartId(partId);

#if ( FBL_MTAB_NO_OF_BLOCKS > 1 )
   if (HDR_UNDEFINED_LOG_BLOCK == blockN)
   {
      /* kFblFailed, do nothing */
   }
   else
#else
# if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   (void) partId; /* Not used in this configuration */ /* PRQA S 3112 */ /* MD_MSR_14.2 */
# endif
#endif
   {
      tFblAddress segmentStart = segment->targetAddress;
      tFblLength segmentLen = segment->length;
      tFblAddress blockStart =  FblLogicalBlockTable.logicalBlock[blockN].blockStartAddress;
      tFblLength blockLen =  FblLogicalBlockTable.logicalBlock[blockN].blockLength;

      if (FBL_UTIL_CHECK_RANGE_CONTAINED_LEN(segmentStart, segmentLen, blockStart, blockLen))
      {
         retVal = kFblOk;
      }
   }

   return retVal;
}

#if ( FBL_MTAB_NO_OF_BLOCKS > 1 )
/***********************************************************************************************************************
 *  FblHdrGetBasePartIdFromModuleId
 **********************************************************************************************************************/
/*! \brief       Retrieves the base partition Id for all types of modules.
 *  \details     This is the application partition ID for all calibration files
    \pre         mid is valid
 *  \param[in]   mid Module ID ( including calibration IDs ) for which base Partition ID shall be returned
 *  \return      Base module partition ID
 **********************************************************************************************************************/
static vuint8 FblHdrGetBasePartIdFromModuleId( vuint8 mid )
{
   vuint8 partId;
   vuint8 baseMid;

#if defined( FBL_ENABLE_CALIBRATION_MODULES )
   if (IsIdOfCalForOpsw1(mid))
   {
      baseMid = MODID_OPSW_1;
   }
   else if (IsIdOfCalForOpsw2(mid))
   {
      baseMid = MODID_OPSW_2;
   }
   else if (IsIdOfCalForOpsw3(mid) )
   {
      baseMid = MODID_OPSW_3;
   }
   else if (IsIdOfCalForOpsw4(mid) )
   {
      baseMid = MODID_OPSW_4;
   }
   else
#endif
   {
      baseMid = mid;
   }

   if (IsIdOfOpsw1(baseMid))
   {
      partId = PARTID_OPSW_1;
   }
   else if (IsIdOfOpsw2(baseMid))
   {
      partId = PARTID_OPSW_2;
   }
   else if (IsIdOfOpsw3(baseMid) )
   {
      partId = PARTID_OPSW_3;
   }
   else if (IsIdOfOpsw4(baseMid) )
   {
      partId = PARTID_OPSW_4;
   }
   else
   {
      partId = HDR_NO_PART_ID_DEFINED;
   }
   return partId;
}
#endif

/***********************************************************************************************************************
 *  FblHdrChkIdPresent
 **********************************************************************************************************************/
/*! \brief       Check on given module ID the presence of the module.
    \details     Helper function to get present state for given module
    \pre         mid is valid
 *  \param[in]   mid Module ID of any module
 *  \return      kFblOk=Success; kFblFailed=Failure
 **********************************************************************************************************************/
static tFblResult FblHdrChkIdPresent( vuint8 mid )
{
   tPartPresState psiState;
# if !defined( FBL_ENABLE_CALIBRATION_MODULES ) && !defined( FBL_ENABLE_ASSERTION )
  (void) mid;
# endif

   assertFblInternal((MOD_TYPE_NOTSUPPORTED != FblGetModType(mid)), kFblAssertParameterValid); /* PRQA S 2214 */ /* MD_FblAssert_Assertion */

   /* Parameter of FblHdrGetBasePartIdFromModuleId() is only used in case calibration modules are supported */
   psiState = ApplFblChkPSIState(FblHdrGetBasePartIdFromModuleId(mid));

#if defined( FBL_ENABLE_CALIBRATION_MODULES )
   if (MOD_TYPE_CAL == FblGetModType(mid))
   {
      tCalPartitionInfo calPinfo;
      tCalInfo dummyCalInfo;

      calPinfo.partID = 0u;
      /* Only if base module is valid, partition info can be parsed */
      if (PSI_PART_PRESENT == psiState)
      {
         if (kFblOk == FblHdrInitCalPartInfoAndCalInfo(mid, &calPinfo, &dummyCalInfo))
         {
            psiState = ApplFblChkPSIState(calPinfo.partID);
         }
         else
         {
            psiState = PSI_PART_INVALID;
         }
      }
   }
#endif

   return (tFblResult)((PSI_PART_PRESENT != psiState)? kFblFailed : kFblOk);
}

/***********************************************************************************************************************
 *  FblHdrCheckEnvelopesExtractSignedHeader
 **********************************************************************************************************************/
/*! \brief       Returns pointer to buffer containing Signed header information required during download.
 *  \details     Checks envelope information and extracts Signed header information in case it is
 *               received compressed / encrypted.
 *  \param[in]   rcvBuffer Received data buffer
 *  \param[in]   rcvDataSize Size of received data package
 *  \return      Pointer to Signed header information
 **********************************************************************************************************************/
static const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * FblHdrCheckEnvelopesExtractSignedHeader( const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * rcvBuffer, tTpDataType rcvDataSize )
{
   const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * retBuffer = V_NULL;
   vuint16 locDataType = (vuint16)FblMemGetInteger(HDR_DATATYPE_LEN, &rcvBuffer[HDR_DATYPE_IDX]);

   switch (locDataType)
   {
      case DATA_TYPE_SIGNED_V1:
         {
            if (rcvDataSize < HDR_MODULE_MAX_RAW_LEN)
            {
               /* Very small cal files may hit here. These have to be filled to have at minimum (SWM_DATA_MAX_NOAR * HDR_REGINFO_LEN)
                * content length */
               FblSetPecErr(HdrErr_ExtTransferTooSmall); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
            }
            else
            {
               currMemLibSegment.dataFormat = MODULE_DF_RAW;
               retBuffer = rcvBuffer;
            }
            break;
         }
#if defined( FBL_ENABLE_COMPRESSION_MODE )
# if defined( DATA_TYPE_COMPRESSED_ARLE )
         /* Data type compression ARLE */
         case DATA_TYPE_COMPRESSED_V1:
         {
            cmprLstIdx = COMPRESSION_TYPE_ARLE;
            break;
         }
# endif /* DATA_TYPE_COMPRESSED_ARLE */
# if defined( DATA_TYPE_COMPRESSED_LZMA )
         /* Data type compression LZMA */
         case DATA_TYPE_COMPRESSED_V2:
         {
            cmprLstIdx = COMPRESSION_TYPE_LZMA;
            break;
         }
# endif /* DATA_TYPE_COMPRESSED_LZMA */
# if defined( DATA_TYPE_COMPRESSED_USER )
         case DATA_TYPE_COMPRESSED_USER:
         {
            cmprLstIdx = COMPRESSION_TYPE_USER;
            break;
         }
# endif /* DATA_TYPE_COMPRESSED_USER */
#endif /* FBL_ENABLE_COMPRESSION_MODE */
      default:
         {
# if defined( FBL_HDR_USER_DATA_TYPE )
            /* Input rcvBuffer, get retBuffer and dataFormat */
            ApplFblCheckEnvelopesExtractSignedHeader(retBuffer, rcvBuffer, &currMemLibSegment.dataFormat);
            if (V_NULL == retBuffer)
# endif
            {
               FblSetPecErr(HdrErr_DataType); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
            }
            break;
         }
   }

#if defined( FBL_ENABLE_COMPRESSION_MODE )
   if( (locDataType == DATA_TYPE_COMPRESSED_V1) || (locDataType == DATA_TYPE_COMPRESSED_V2) )
   {
      tProcParam envProcParam;

      /* Initialize compressedLength to either 0 (if value is n/a) or compressed length, read start of compressed data in header. */
      if (kFblOk != ApplFblCmprReadHeader(&compressedLength, rcvBuffer, (vuint16*)&rcvBuffHandle.buffIdx))
      {
         FblSetPecErr(HdrErr_Compression); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
      }
      else
      {
         envProcParam.dataBuffer = (V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3* ) &rcvBuffer[rcvBuffHandle.buffIdx]; /* PRQA S 0311 */ /* MD_FblHdr_0311 */
         envProcParam.dataLength = rcvDataSize; /* The complete signed header part need always to get in within first transferData */
         envProcParam.dataOutBuffer = FblRamHeader;
         envProcParam.dataOutLength = 0u;
         envProcParam.dataOutMaxLength = RAM_HEADER_LENGTH;
         envProcParam.wdTriggerFct = FblHdrGetPollingTask();
         envProcParam.mode = MODULE_DF_COMPR_HDR;

         if (    (kFblOk != ApplFblCmprInit()) /* PRQA S 3415 */ /* MD_FblHdr_3415_2 */
               || (kFblOk != ApplFblInitDataProcessing(&envProcParam))
               || (kFblOk != ApplFblDataProcessing(&envProcParam))
            )
         {
            FblSetPecErr(HdrErr_Compression); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
         }
         else if (envProcParam.dataOutLength < HDR_MODULE_MAX_RAW_LEN)
         {
            /* Very small cal files may hit here. These have to be filled to have at minimum (SWM_DATA_MAX_NOAR * HDR_REGINFO_LEN)
               * content length */
            FblSetPecErr(HdrErr_ExtTransferTooSmall); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
         }
         else
         {
            /* Deinit is not allowed to produce any data, all data wanted is already decompressed. */
            envProcParam.dataOutMaxLength = 0u;
            if (kFblOk != ApplFblDeinitDataProcessing(&envProcParam))
            {
               FblSetPecErr(HdrErr_Compression); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
            }

            locDataType = (vuint16)FblMemGetInteger(HDR_DATATYPE_LEN, &FblRamHeader[HDR_DATYPE_IDX]);

            if (DATA_TYPE_SIGNED_V1 != locDataType)
            {
               FblSetPecErr(HdrErr_DataType); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
            }
            else
            {
               /* Compression flag shall be set here. */
               currMemLibSegment.dataFormat = MODULE_DF_COMPR;

               /* Call ApplFblCmprInit() again to allow resetting of internal states (restart decompression) */
               if (kFblOk == ApplFblCmprInit())
               {
                  retBuffer = FblRamHeader;
               }
            }
         }
      }
   }
#endif /* FBL_ENABLE_COMPRESSION_MODE */

   return retBuffer;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */ /* PRQA S 6030 */ /* MD_MSR_STCYC */

/***********************************************************************************************************************
 *  FblHdrGetIntegerFblHeader
 **********************************************************************************************************************/
/*! \brief       This function ensures that FblMemGetInteger is called with RAM buffer.
 *               The function extracts 'count' bytes from pFblHeader and converts it into an integer
 *  \param[in]   count Number of bytes to extract from array, count is not allowed to be bigger than 4u
 *  \param[in]   addr Address in memory where data is located
 *  \return      Extracted integer number
 **********************************************************************************************************************/
static vuint32 FblHdrGetIntegerFblHeader( vuint8 count, V_MEMROM1_FAR vuint8 V_MEMROM2_FAR V_MEMROM3 * pFblHeader )
{
   vuint8 getBuffer[MAX_FBL_HEADER_GET_INTEGER_DATA];
   vuint32 retVal;
   vuint8 idx;

   /* assertion if param count is larger than 16 bytes */
   assertFblInternal((count <= MAX_FBL_HEADER_GET_INTEGER_DATA), kFblAssertInternalIlegalState); /* PRQA S 2214 */ /* MD_FblAssert_Assertion */

   for (idx = 0; idx < count; idx++)
   {
      getBuffer[idx] = pFblHeader[idx];
   }

   retVal = FblMemGetInteger(count, getBuffer);

   return retVal;
}

/***********************************************************************************************************************
 *  FblHdrGetIntegerRom
 **********************************************************************************************************************/
/*! \brief       This function extracts 'count' bytes from addr and converts it into an integer
 *  \param[in,out]  result Allows to indicate an error to the caller. The value is unchanged in the case of
 *                  positive result. It is set kFblFailed in case of error. This allows for checking result only after
 *                  a sequence of calls and distinguish "all Ok" from "one failed".
 *  \param[in]   count Number of bytes to extract from array
 *  \param[in]   addr Address in memory where data is located
 *  \return      Extracted integer number
 **********************************************************************************************************************/
static vuint32 FblHdrGetIntegerRom( tFblResult * result, vuintx count, tFblAddress addr )
{
   vuint8 getBuffer[MAX_FBL_HEADER_GET_INTEGER_DATA];
   vuint32 retVal = 0u;

   /* assertion if param count is larger than 4 bytes */
   assertFblInternal((count <= MAX_FBL_HEADER_GET_INTEGER_DATA), kFblAssertInternalIlegalState); /* PRQA S 2214 */ /* MD_FblAssert_Assertion */

   /* Read from memory */
   if (MAX_FBL_HEADER_GET_INTEGER_DATA != FblReadProm(addr, getBuffer, MAX_FBL_HEADER_GET_INTEGER_DATA))
   {
     *result = kFblFailed;
     /* keep "0" for retVal */
   }
   else
   {
      retVal = FblMemGetInteger(count, getBuffer);
   }

   return retVal;
}

#if defined( FBL_ENABLE_CALIBRATION_MODULES ) || defined( FBL_ENABLE_VERIFY_INTEGRITY_WORD )
/***********************************************************************************************************************
 *  FblHdrGetRegionRomAndInc
 **********************************************************************************************************************/
/*! \brief       This functions extracts a full segment information from ROM.
 *  \details     It is used in calibration file configuration to read Information from Application Plain Header.
 *  \param[out]  segInfo read Segment info will be written to here
 *  \param[in]   baseAddr constant offset to be added to relAddr
 *  \param[in,out] relAddr variable addr part that is incremented after having read segment information parts
 *  \return      kFblOk/kFblFailed read result
 **********************************************************************************************************************/
static tFblResult FblHdrGetRegionRomAndInc( FL_SegmentInfoType * segInfo, tFblAddress baseAddr, tFblAddress * relAddr )
{
   tFblResult retVal = kFblOk;

   segInfo->targetAddress = FblHdrGetIntegerRom(&retVal, HDR_REGADDR_LEN, baseAddr + *relAddr);
   *relAddr+= HDR_REGADDR_LEN;
   segInfo->length = FblHdrGetIntegerRom(&retVal, HDR_REGLEN_LEN, baseAddr + *relAddr);
   *relAddr+= HDR_REGLEN_LEN;
   return retVal;
}
#endif

#if defined( FBL_ENABLE_GAP_FILL )
/***********************************************************************************************************************
 *  FblHdrPartSegmentListGetBlockStatus
 **********************************************************************************************************************/
/*! \brief       Read from partBlockList FlashBlock usage information.
 *               The information might be:
 *               - HDR_BLOCK_USED_BY_PARTITION
 *               - HDR_BLOCK_UNUSED_AND_IN_OTHER_PARTITION
 *               - HDR_BLOCK_UNUSED_AND_IN_BASEPARTITION
 *               - HDR_BLOCK_UNUSED_NOT_IN_BASEPARTITION
 *  \param[in]   blockNr FlashBlock Nbr for which usage information is required
 *  \param[in]   partId Partition for which usage information is required
 *  \return      Block usage information
 **********************************************************************************************************************/
static tFblBlockPartUsage FblHdrPartSegmentListGetBlockStatus( vsint16 blockN, vuint8 partId )
{
   vuintx i;
   FL_SegmentInfoType segment;
   tFblBlockPartUsage retVal = HDR_BLOCK_UNUSED_NOT_IN_BASEPARTITION;

   for (i = 0u; i < HDR_MAX_PARTITION_TRACKED_REGIONS; i++)
   {
      FblHdrPollingTaskVoid();

      if (    (kFblHdrMemSegmNotFound != partBlockList.segList[i].startBlk)
           && (blockN >= partBlockList.segList[i].startBlk)
           && (blockN <= (partBlockList.segList[i].endBlk)))
      {
#if defined( FBL_ENABLE_CALIBRATION_MODULES ) || ( FBL_MTAB_NO_OF_BLOCKS > 1 )
         if (partId != partBlockList.segList[i].partID)
         {
             retVal = HDR_BLOCK_UNUSED_AND_IN_OTHER_PARTITION;
         }
         else
#endif
         {
             retVal = HDR_BLOCK_USED_BY_PARTITION;
         }
         break;
      }
   }

   if (HDR_BLOCK_UNUSED_NOT_IN_BASEPARTITION == retVal)
   {
      segment.targetAddress = FlashBlock[blockN].begin;
      segment.length = FBL_UTIL_GET_RANGE_LEN(FlashBlock[blockN].begin, FlashBlock[blockN].end);

      if (kFblOk == FblHdrCheckMtabIncludesSegment(partId, &segment))
      {
         retVal = HDR_BLOCK_UNUSED_AND_IN_BASEPARTITION;
      }
   }

   return retVal;
}
#endif /* FBL_ENABLE_GAP_FILL */

/***********************************************************************************************************************
 *  FblHdrPartSegmentListAddEntry
 **********************************************************************************************************************/
/*! \brief       This functions checks the Segment information and updates partBlockList list, if the region is accepted.
 *               Checks:
 *               - covered by FlashBlock table
 *               - covered by Logical Block Table
 *               - not overlapping with already read in partition information (cal region <-> appl region)
 *  \param[in]   partId Partition ID part to add to list *
 *  \param[in]   segment Segment in partition to add to list
 *  \return      kFblOk Adding entry successful
 *               kFblFailed Adding entry failed, or overlapping of partition regions
 **********************************************************************************************************************/
static vuintx FblHdrPartSegmentListAddEntry( vuint8 partId, const FL_SegmentInfoType * segment )
{
   vuintx i;
   vuintx retVal = HDR_PARTITION_REGION_UNDEF;
   tPartitionSegment newSeg;

#if defined( FBL_ENABLE_CALIBRATION_MODULES ) || ( FBL_MTAB_NO_OF_BLOCKS > 1 )
   newSeg.partID = partId;
#endif
   newSeg.startBlk = FblMemSegmentNrGet(segment->targetAddress);
   newSeg.endBlk = FblMemSegmentNrGet(FBL_UTIL_GET_RANGE_END(segment->targetAddress, segment->length));

   if (     (kFblOk != FblHdrCheckMtabIncludesSegment(partId, segment))
         || (kFblHdrMemSegmNotFound == newSeg.startBlk)
         || (kFblHdrMemSegmNotFound == newSeg.endBlk))
   {
      retVal = HDR_PARTITION_REGION_ERROR;
   }

   for (i = 0u; (i < HDR_MAX_PARTITION_TRACKED_REGIONS) && (HDR_PARTITION_REGION_UNDEF == retVal); i++)
   {
      FblHdrPollingTaskVoid();

      if (kFblHdrMemSegmNotFound == partBlockList.segList[i].startBlk) /* "kFblHdrMemSegmNotFound" marks unused elements */
      {
         /* Add entry and leave loop */
         partBlockList.segList[i] = newSeg;
         retVal = i;
      }
      else
      {
#if defined( FBL_ENABLE_ASSERTION ) && (defined( FBL_ENABLE_CALIBRATION_MODULES ) || ( FBL_MTAB_NO_OF_BLOCKS > 1 ))
         if (!FBL_UTIL_CHECK_RANGES_EXCLUSIVE_ADDR(newSeg.startBlk, newSeg.endBlk,
                                                   partBlockList.segList[i].startBlk, partBlockList.segList[i].endBlk))
         {
            if (newSeg.partID != partBlockList.segList[i].partID)
            {
               /* Different partitions need to use different physical Flash blocks */
               retVal = HDR_PARTITION_REGION_ERROR;
            }
         } /* if !(FBL_UTIL .. */
#else
          /* loop further */
#endif /* FBL_ENABLE_ASSERTION */
      } /* i == HDR_MAX_PARTITION_TRACKED_REGIONS */
   } /* for ..*/

   if (i == HDR_MAX_PARTITION_TRACKED_REGIONS)
   {
      retVal = HDR_PARTITION_REGION_ERROR;
   }

   return retVal;
}

/***********************************************************************************************************************
 *  FblHdrValidateSignerInfo
 **********************************************************************************************************************/
/*! \brief       Validate Signer Info
 *  \pre         This function is called after module signer info is loaded to signInfoAddr buffer location
 *  \param[in]   signInfoAddr address to signer Info information in download buffer
 *  \return      Address of public key stored in signer info.  In case of error, V_NULL is returned
 **********************************************************************************************************************/
static vuint8 const * FblHdrValidateSignerInfo( const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * signInfoAddr, vuint16 keyNbToCheck )
{
   tFblResult cmpResult;
   vuint8 const * retVal = V_NULL;

   /* Subject name = Stored? */
   cmpResult = FblHdrMemCompareFblHeader( &signInfoAddr[SIGNINFO_SUBJNAME_IDX], FblGetComDat_SubjNameAddr(), HDR_SUBJNAME_LEN );
   if (kFblOk != cmpResult)
   {
      FblSetPecErr(HdrErr_SubjectName); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
   }
   /* KeyNBID needs to be read before Signature Verification */
   else if (     (kFblOk != ApplFblNVMReadKeyNBID(&keyNBIDInfo)) /* PRQA S 0306 */ /* MD_FblHdr_0306 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
              || (keyNbToCheck < keyNBIDInfo.stored))
   {
      FblSetPecErr(HdrErr_KeyNBID); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
   }
   /* Validate Signer Info using Root Public key from Boot Info Block and Root Signature from Signer Info downloaded */
   /* Macro APPL_FBL_READ_SEC_DATA can be used to copy SecM_Production_RsaExp to RAM if required */
   else if (kFblOk != FblHdrValidateSignature(&signInfoAddr[0], SIGNINFO_ROOTSIGN_IDX,
                                              &signInfoAddr[SIGNINFO_ROOTSIGN_IDX], APPL_FBL_READ_SEC_DATA(SecM_RsaMod)))
   {
      FblSetPecErr(HdrErr_RootSignature); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
   }
   else
   {
      retVal = &signInfoAddr[SIGNINFO_SIGNPKEY_IDX];
   }

   return retVal;
}

/***********************************************************************************************************************
 *  FblHdrCheckAppNBID
 **********************************************************************************************************************/
/*! \brief       Check application NBID
 *  \pre         This function is called only after module download header information was received.
 *  \return      kFblOk=Success; kFblFailed=Failure
 **********************************************************************************************************************/
static tFblResult FblHdrCheckAppNBID( void )
{
   tFblResult retVal = kFblFailed;

   if (kFblOk == ApplFblNVMReadAppNBID(&appNBIDInfo)) /* PRQA S 0310 */ /* MD_FblHdr_0310 */
   {
      if (appNBIDInfo.received >= appNBIDInfo.stored)
      {
         retVal = kFblOk;
      }
   }
   else
   {
      /* After discussion with GM: security is more important than continuation of download */
      /* Make sure ApplFblNVMReadAppNBID is as reliable as possible! */
   }

   /* pec error is set externally */
   return retVal;
}

/***********************************************************************************************************************
 *  FblHdrCheckAndParseSwLocInfo
 **********************************************************************************************************************/
/*! \brief       Initialize and check application Location Info
 *  \details     Check this:
 *                 - SW address ranges are compared to boot Info block.
 *                 - Not overlapping PSI information
 *                 - Check FblHdrGetBaseModuleHdrAddr() implementation return the same value than the address
 *                   downloaded (to ensure data can be accessed later on)
 *  \pre         This function is called only after module download header information was received.
 *  \param[in]   locInfoAddr address to Location Info information in download buffer
 *  \return      kFblOk=Success; kFblFailed=Failure
 **********************************************************************************************************************/
static tFblResult FblHdrCheckAndParseSwLocInfo( const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * locInfoAddr )
{
   vuintx i;
   vuintx parseIdx;
   FL_SegmentInfoType prPtn;
   tFblResult retVal;

   parsedModuleInfo.segmentList.nrOfSegments = (vuint8)FblMemGetInteger(HDR_NBROF_LEN, locInfoAddr);
   parseIdx = HDR_NBROF_LEN;

   if (parsedModuleInfo.segmentList.nrOfSegments > SWM_DATA_MAX_NOAR)
   {
      FblSetPecErr(HdrErr_SwRegion); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
      retVal = kFblFailed;
   }
   else
   {
      retVal = kFblOk;

      /* Prepare PSI data structures: transform address and length information to block information */
      FblHdrGetPsiRegion(parsedModuleInfo.partID, &prPtn.targetAddress, &prPtn.length);

      /* Extract the Address-Region table */
      for (i = 0u; (kFblOk == retVal) && (i < parsedModuleInfo.segmentList.nrOfSegments); ++i)
      {
         FL_SegmentInfoType currSeg;

         FblHdrPollingTaskVoid();

         currSeg.targetAddress = (tFblAddress) FblMemGetInteger(HDR_REGADDR_LEN, &locInfoAddr[parseIdx]);
         parseIdx += HDR_REGADDR_LEN;
         currSeg.length = (tFblLength) FblMemGetInteger(HDR_REGLEN_LEN, &locInfoAddr[parseIdx]);
         parseIdx += HDR_REGLEN_LEN;

         /* Check region is not overlapping with PSI region and included in Boot Info space */
         if (    (HDR_PARTITION_REGION_ERROR == FblHdrPartSegmentListAddEntry(parsedModuleInfo.partID, &currSeg))
               || (! FBL_UTIL_CHECK_RANGES_EXCLUSIVE_LEN(prPtn.targetAddress, prPtn.length, /* PRQA S 3321 */ /* MD_FblHdr_3321 */
                     currSeg.targetAddress, currSeg.length))
            )
         {
            FblSetPecErr(HdrErr_SwRegion); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
            retVal = kFblFailed;
         }
         else
         {
            parsedModuleInfo.segmentList.segmentInfo[i] = currSeg;
         }
      }
      parsedModuleInfo.segmentList.nrOfSegments = (vuint8)i;

      /* Add Pressence Pattern Region to take into account for FlashBlock partition usage check */
      if (HDR_PARTITION_REGION_ERROR == FblHdrPartSegmentListAddEntry(parsedModuleInfo.partID, &prPtn))
      {
         FblSetPecErr(HdrErr_SwRegion); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
         retVal = kFblFailed;
      }

      /* if FblHdrGetBaseModuleHdrAddr returns NO_HEADER_ADDR_DEFINED, it will never match parsedModuleInfo.segmentList.segmentInfo[0].targetAddress */
      if (parsedModuleInfo.segmentList.segmentInfo[0].targetAddress != FblHdrGetBaseModuleHdrAddr(parsedModuleInfo.partID) )
      {
         FblSetPecErr(HdrErr_SwRegion); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
         retVal = kFblFailed;
      }
   }

   return retVal;
}

/***********************************************************************************************************************
 *  FblHdrCheckEcuNameAndId
 **********************************************************************************************************************/
/*! \brief       Check for Ecu name and Id
 *  \pre         This function is called only after module download header information was received.
 *  \param[in]   ecuNameAddr address to Ecu Name information in download buffer
 *  \param[in]   zeroCheck perform zero-check or not
 *  \return      kFblOk=Success; kFblFailed=Failure
 **********************************************************************************************************************/
static tFblResult FblHdrCheckEcuNameAndId( const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * ecuNameAddr, tEcuIdZeroCheck zeroCheck )
{
   tFblResult retVal = kFblFailed;
   const vuint8 zeroesId[HDR_ECUID_LEN] = {0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u};
   vuint8 ecuIdVal[HDR_ECUID_LEN];

   if (kFblOk != FblHdrMemCompareFblHeader(ecuNameAddr, FblGetComDat_EcuNameAddr(), HDR_ECUNAME_LEN ))
   {
      FblSetPecErr(HdrErr_EcuName); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
   }
   else
   {
      if (zeroCheck == HDR_ECUID_DO_ZEROCHECK)
      {
         if (kFblOk == FblHdrMemCompare(&ecuNameAddr[HDR_ECUNAME_LEN], &zeroesId[0], HDR_ECUID_LEN ))
         {
            retVal = kFblOk;
         }
      }

      if (kFblOk != retVal)
      {
         if (kFblOk == ApplFblNVMReadECUID(ecuIdVal)) /* PRQA S 0306 */ /* MD_FblHdr_0306 */
         {
            if (kFblOk == FblHdrMemCompare(&ecuNameAddr[HDR_ECUNAME_LEN], &ecuIdVal[0], HDR_ECUID_LEN))
            {
               retVal = kFblOk;
            }
         }
      }

      if (kFblOk != retVal)
      {
         FblSetPecErr(HdrErr_EcuId); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
      }
   }
   return retVal;
}

/***********************************************************************************************************************
 *  FblHdrCheckDownloadDcid
 **********************************************************************************************************************/
/*! \brief       Check for compatibility of downloaded module
 *  \pre         This function is called only after module download header information was received.
 *  \param[in]   dcidAddr address to Dcid information in download buffer
 *  \return      kFblOk=Success; kFblFailed=Failure
 **********************************************************************************************************************/
static tFblResult FblHdrCheckDownloadDcid( const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * dcidAddr )
{
   tFblResult retVal = kFblOk;
   vuint16 locDcidNewMod;
   vuint16 locDcidRefMod;

#if defined( FBL_ENABLE_CALIBRATION_MODULES )
   if (MOD_TYPE_CAL == parsedModuleInfo.modType)
   {
      /* if FblHdrGetIntegerRom fails, locDcidRefMod gets 0, this will lead to an error in the checks below */
      locDcidRefMod = (vuint16)FblHdrGetIntegerRom(&retVal, HDR_DCID_LEN, calInfo.startAddr + HDR_MID_LEN);
   }
   else
#endif
   {
      locDcidRefMod = (vuint16)FblHdrGetIntegerFblHeader(HDR_DCID_LEN, FblGetComDat_DcidAddr());
   }

   locDcidNewMod = (vuint16)FblMemGetInteger(HDR_DCID_LEN, dcidAddr);

   /* If DCID is available, verify the data-compatibility ID */
   if (
#if defined( FBL_ENABLE_CALIBRATION_MODULES )
         (kFblOk == retVal) && /* FblHdrGetIntegerRom may have set retVal to kFblFailed */
#endif
         (locDcidNewMod != (vuint16)0xFFFFu) && (locDcidRefMod != (vuint16)0xFFFFu)
     )
   {
      if (locDcidNewMod != locDcidRefMod)
      {
         retVal = kFblFailed;
      }
   }

   return retVal;
}

/***********************************************************************************************************************
 *  FblHdrInitBaseModInfo
 **********************************************************************************************************************/
/*! \brief       Initialize pBaseModInfo with all relevant information for Base Sw Module
 *  \details     Parse Application programmed header Information not related to Cal partitions
 *  \pre         application is programmed
 *  \param[in]   partId partition ID (possible values: 0x01-0x0F, 0x11-0x19, 0x21-0x29, 0x31-0x39)
 *  \param[in,out]  pBaseModInfo pointer to Application Info related information
 *  \return      kFblOk=Success; kFblFailed=Failure
 **********************************************************************************************************************/
static tFblResult FblHdrInitBaseModInfo( vuint8 partId, V_MEMRAM1 tBaseModInfo V_MEMRAM2 V_MEMRAM3 * pBaseModInfo )
{
   tFblResult retVal = kFblOk;
   vuint16 locParseIdx = P1_APP_ASCIIPN_IDX;
   tFblAddress modHdrAddr;

   /* Get header address of op/sw */
   modHdrAddr = FblHdrGetBaseModuleHdrAddr(FblHdrGetBasePartId(partId));

   if (HDR_NO_HEADER_ADDR_DEFINED == modHdrAddr)
   {
      retVal = kFblFailed;
   }
   else
   {
      if (ASCII_PN_PRESENT == (ASCII_PN_PRESENT & FblHdrGetIntegerRom(&retVal, HDR_CFGOPT_LEN, modHdrAddr + P1_CFGOPT_IDX)))
      {
         locParseIdx += HDR_ASCIIPN_LEN;
      }
   }

   if (kFblOk != retVal)
   {
      /* Error reading the application SW Info */
      FblSetPecErr(HdrErr_GetAppInfo); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
   }

   /* pBaseModInfo->swLocInfoAddr now points to loc info section->Num of SW Regions */
   pBaseModInfo->startAddr = modHdrAddr;
   pBaseModInfo->swLocInfoAddr = modHdrAddr + locParseIdx; /* PRQA S 3382 */ /* MD_FblHdr_3382*/
   pBaseModInfo->nbrOfRgns = (vuint8)FblHdrGetIntegerRom(&retVal, HDR_NBROF_LEN, modHdrAddr + locParseIdx); /* PRQA S 3382 */ /* MD_FblHdr_3382.3 */
   pBaseModInfo->nbPartAddr = pBaseModInfo->swLocInfoAddr + HDR_NBROF_LEN +
                             (pBaseModInfo->nbrOfRgns * (HDR_REGADDR_LEN + HDR_REGLEN_LEN));
   return retVal;
}

#if defined( FBL_ENABLE_CALIBRATION_MODULES )
/***********************************************************************************************************************
 *  FblHdrInitNextCalPartInfo
 **********************************************************************************************************************/
/*! \brief       Read partition info data from partition Number
 *  \details     Initialize calPartInfo based on knowing the address of the partition info in memory.
 *  \pre         pCalPartInfo is initialized with partId, partInfoAddr and lengthPart
 *  \param[in,out]  pCalPartInfo pointer to "Partition Info" related information
 *  \return      kFblOk=Success; kFblFailed=Failure
 **********************************************************************************************************************/
static tFblResult FblHdrInitNextCalPartInfo( V_MEMRAM1 tCalPartitionInfo V_MEMRAM2 V_MEMRAM3 * pCalPartInfo )
{
   tFblResult retVal = kFblOk;

   if (FblHdrGetPartNbrFromId(pCalPartInfo->partID) > pCalPartInfo->nbrOfPrts)
   {
      /* There is no further partition Info */
      retVal = kFblFailed;
   }
   else
   {
      /* Set address to start of next partition info block */
      pCalPartInfo->partInfoAddr += pCalPartInfo->lengthPart;

      /* partInfoAddr now points to "partition Info x->Num of Regions" */
      pCalPartInfo->nbrOfRgns = (vuint8)FblHdrGetIntegerRom(&retVal, HDR_NBROF_LEN, (pCalPartInfo->partInfoAddr));

      /* lengthPart points to region information for the last partition region */
      pCalPartInfo->lengthPart = (vuint16)(HDR_NBROF_LEN + ((pCalPartInfo->nbrOfRgns - 1u) * (HDR_REGINFO_LEN)));

      {
         /* set psiStart address */
         pCalPartInfo->psiStart = (tFblAddress) FblHdrGetIntegerRom(&retVal, HDR_REGADDR_LEN, pCalPartInfo->partInfoAddr +
               pCalPartInfo->lengthPart);
         pCalPartInfo->psiStart += (tFblAddress) FblHdrGetIntegerRom(&retVal, HDR_REGLEN_LEN, pCalPartInfo->partInfoAddr +
               pCalPartInfo->lengthPart + HDR_REGADDR_LEN );
         pCalPartInfo->psiStart -= (2u * FBL_PP_SEGMENT_SIZE);
      }

      /* lengthPart points to "partition Info x->Num of Calibrations" */
      pCalPartInfo->lengthPart += HDR_REGINFO_LEN;

      /* set number of calibrations */
      pCalPartInfo->nbrOfCals = (vuint8)FblHdrGetIntegerRom(&retVal, HDR_NBROF_LEN, (pCalPartInfo->partInfoAddr +
                                                              pCalPartInfo->lengthPart));

      /* lengthPart is the total length of this partition info block */
      pCalPartInfo->lengthPart += (vuint16)(HDR_NBROF_LEN + (pCalPartInfo->nbrOfCals * HDR_CALINFO_LEN));
   }

   return retVal;
}

/***********************************************************************************************************************
 *  FblHdrInitCalPartInfo
 **********************************************************************************************************************/
/*! \brief       Initialize calPartInfo with all relevant information for calPartId
 *  \details     Parse Application programmed header from and sequentially all partitions that are located before the
 *               partition, which is intended to read up to the requested partition. Once the requested partition info
 *               location is determined, initialize calPartInfo based on partition information.
 *  \pre         application is programmed, pCalPartInfo.partID is initialized
 *  \param[in]   calPartId partition ID of a calibration partition (possible values: 0x02-0x0F, 0x12-0x19, 0x22-0x29, 0x32-0x39)
 *  \param[in,out]  pCalPartInfo pointer to "Partition Info" related information
 *  \return      kFblOk=Success; kFblFailed=Failure
 **********************************************************************************************************************/
static tFblResult FblHdrInitCalPartInfo( vuint8 calPartId, V_MEMRAM1 tCalPartitionInfo V_MEMRAM2 V_MEMRAM3 * pCalPartInfo )
{
   tFblResult retVal = kFblOk;

   /* Calibration partition ID cannot be 0 or 1 */
   assertFblInternal(PartIdCal(calPartId), kFblAssertParameterValid); /* PRQA S 2214 */ /* MD_FblAssert_Assertion */

   /* Check if we already have partition information for the current partition */
   if (calPartId == pCalPartInfo->partID)
   {
      /* Partition information already initialized, nothing to do  */
   }
   /* Check if we already have partition information for the previous calibration partition */
   else if ((calPartId - 1u) == pCalPartInfo->partID)
   {
      /* Get partition information for current partition */
      pCalPartInfo->partID = calPartId;
      retVal = FblHdrInitNextCalPartInfo(pCalPartInfo);
   }
   else
   {
      /* Get Application Module Info to retrieve partition info start */
      tBaseModInfo locApplModInfo;
      retVal = FblHdrInitBaseModInfo(calPartId, &locApplModInfo);
      pCalPartInfo->nbrOfPrts = (vuint8)FblHdrGetIntegerRom(&retVal, HDR_NBROF_LEN, locApplModInfo.nbPartAddr);

      /* Check if any failures occured, any partitions exist, current partition is within bounds */
      if ((kFblOk != retVal) || (FblHdrGetPartNbrFromId(calPartId) > pCalPartInfo->nbrOfPrts))
      {
         retVal = kFblFailed;
      }
      else
      {
         assertFblInternal(0u != pCalPartInfo->nbrOfPrts, kFblAssertInternalIlegalState);  /* PRQA S 2214 */ /* MD_FblAssert_Assertion */

         /* Setting of partition Id validates the information */
         pCalPartInfo->partID = calPartId;
         /* pCalPartInfo->partInfoAddr now points to "partition Info 1->Num of Regions" */
         pCalPartInfo->partInfoAddr = locApplModInfo.nbPartAddr + HDR_NBROF_LEN;
         pCalPartInfo->lengthPart = 0u;

         while (( FblHdrGetPartNbrFromId(calPartId) > 0u) && (kFblOk == retVal))
         {
            FblHdrPollingTaskVoid();

            retVal = FblHdrInitNextCalPartInfo(pCalPartInfo);
            calPartId--;
         }
      }
   }

   if (kFblOk != retVal)
   {
      /* Invalidate partition information */
      pCalPartInfo->partID = 0u;
   }

   return retVal;
}

/***********************************************************************************************************************
 *  FblHdrInitCalPartInfoAndCalInfo
 **********************************************************************************************************************/
/*! \brief       Init cal ID related "Partion Info" and "Cal Info" data from programmed application plain header.
 *  \details     Calls FblHdrInitCalPartInfo in order to initialize "Partition Info" to pCalPartInfo. Use that information
 *               to init pCalInfo for requested calMid.
 *  \pre         application is programmed
 *  \param[in]   calMid Calibration Module ID for which data shall be initialized
 *  \param[in,out]  pCalPartInfo pointer to "Partition Info" related information
 *  \param[out]  pCalInfo pointer to "Calibration Info" related information
 *  \return      kFblOk=Success; kFblFailed=Failure
 **********************************************************************************************************************/
static tFblResult FblHdrInitCalPartInfoAndCalInfo( vuint8 calMid, V_MEMRAM1 tCalPartitionInfo V_MEMRAM2 V_MEMRAM3 * pCalPartInfo,
                                            V_MEMRAM1 tCalInfo V_MEMRAM2 V_MEMRAM3 * pCalInfo )
{
   tFblResult retVal = kFblOk;
   vuint8 calMidFound = 0u;
   tFblAddress currCalInfoAddress;
   vuint8 currCalMid;
   vuint8 currCalModNbr;
   vuint8 partCounter; /* start counting partitions with "1..n", will increase before get */
   vuint8 opSwPartId;

   assertFblInternal((MOD_TYPE_CAL == FblGetModType(calMid)), kFblAssertInternalIlegalState); /* PRQA S 2214 */ /* MD_FblAssert_Assertion */

   /* Parameter of FblHdrGetBasePartIdFromModuleId is only used in case of Multi Processor configuration */
   opSwPartId = FblHdrGetBasePartIdFromModuleId(calMid);

   /* for nbr part, check nbr calibrations for mid */
   for (partCounter = 0u; (calMidFound == 0u) && (kFblOk == retVal); partCounter++)
   {
      FblHdrPollingTaskVoid();

      retVal = FblHdrInitCalPartInfo(FblHdrGetFirstCalPartNbr(opSwPartId) + partCounter, pCalPartInfo);
      if (kFblOk == retVal)
      {
         currCalInfoAddress = pCalPartInfo->partInfoAddr + (pCalPartInfo->nbrOfRgns * HDR_REGINFO_LEN) + (2*HDR_NBROF_LEN);
         for(currCalModNbr = HDR_FIRST_CAL_NBR; (calMidFound == 0u) && (currCalModNbr <= pCalPartInfo->nbrOfCals); currCalModNbr++)
         {
            FblHdrPollingTaskVoid();

            currCalMid = (vuint8)FilterMid(FblHdrGetIntegerRom(&retVal, HDR_MID_LEN, currCalInfoAddress));
            if (kFblOk == retVal)
            {
               if (calMid == currCalMid)
               {
                  pCalInfo->startAddr = currCalInfoAddress;
                  pCalInfo->calNbr = currCalModNbr;

                  /* retVal may be failed, in case FblHdrGetIntegerRom did not read correctly */
                  calMidFound = 1u;
               }
               else
               {
                  currCalInfoAddress += HDR_CALINFO_LEN;
               }
            }
         }
      }
   }

   if (calMidFound != 1u)
   {
      retVal = kFblFailed;
   }

   return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/***********************************************************************************************************************
 *  FblHdrSetCalModuleInfo
 **********************************************************************************************************************/
/*! \brief       Set parsedModuleInfo fields calPos, segmentList and partId from application partition information.
 *  \pre         application is programmed
 *  \return      kFblOk=Success; kFblFailed=Failure
 **********************************************************************************************************************/
static tFblResult FblHdrSetCalModuleInfo( void )
{
   tFblResult retVal = kFblOk;
   tFblAddress currReadAddr;
   vuint8 expModId;

   parsedModuleInfo.segmentList.nrOfSegments = 1u;

   if (calInfo.calNbr > calPartitionInfo.nbrOfCals)
   {
      FblSetPecErr(HdrErr_ModuleID); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
      retVal = kFblFailed;
   }
   else
   {
      expModId = (vuint8)FilterMid(FblHdrGetIntegerRom(&retVal, HDR_MID_LEN, calInfo.startAddr));
      if (parsedModuleInfo.MID != expModId)
      {
         FblSetPecErr(HdrErr_ModuleID); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
         retVal = kFblFailed;
      }
      else
      {
         /* Init Cal Position */
         parsedModuleInfo.calPos = HDR_CALPOS_IRRELEVANT;
#if defined( FBL_HDR_ENABLE_EXPLICIT_ERASE ) &&\
    !defined( FBL_ENABLE_GAP_FILL )
         if (calInfo.calNbr == calPartitionInfo.nbrOfCals)
         {
            parsedModuleInfo.calPos = HDR_CALPOS_LAST;
         }
#else
         /* If both flags are set position is HDR_CALPOS_SINGLE */
         if (calInfo.calNbr == calPartitionInfo.nbrOfCals)
         {
            parsedModuleInfo.calPos |= HDR_CALPOS_LAST;
         }
         if (HDR_FIRST_CAL_NBR == calInfo.calNbr )
         {
            parsedModuleInfo.calPos |= HDR_CALPOS_FIRST;
         }
#endif

         currReadAddr = calInfo.startAddr + HDR_MID_LEN + HDR_DCID_LEN;
         retVal = FblHdrGetRegionRomAndInc(&parsedModuleInfo.segmentList.segmentInfo[0], (tFblAddress)0u, &currReadAddr);

#if defined( FBL_HDR_ENABLE_EXPLICIT_ERASE )
#else
         parsedModuleInfo.partID = calPartitionInfo.partID;
#endif
      }
   }

   return retVal;
}
#endif /* #if defined( FBL_ENABLE_CALIBRATION_MODULES ) */

/***********************************************************************************************************************
 *  FblHdrCheckAndParseDownloadMid
 **********************************************************************************************************************/
/*! \brief       Check and parse Mid and Mid related information
 *  \pre         This function is called only after SWM header information was received.
 *  \param[in]   moduleAddr Address where Module is in Memory
 *  \return      kFblOk=Success; kFblFailed=Failure
 **********************************************************************************************************************/
static tFblResult FblHdrCheckAndParseDownloadMid( const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * moduleAddr )
{
   tFblResult retVal = kFblOk;
   parsedModuleInfo.MID = (vuint8)FilterMid(FblMemGetInteger(HDR_MID_LEN, moduleAddr));
   parsedModuleInfo.modType = FblGetModType(parsedModuleInfo.MID);

   /* Unexpected App module ID will lead to MOD_TYPE_NOTSUPPORTED */
   if (MOD_TYPE_NOTSUPPORTED == parsedModuleInfo.modType)
   {
      FblSetPecErr(HdrErr_ModuleID); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
      retVal = kFblFailed;
   }
   else
   {
#if defined( FBL_HDR_ENABLE_EXPLICIT_ERASE )
      if (MOD_TYPE_APPL == parsedModuleInfo.modType)
      {
         /* Check for correct Partition initialized (equivalent to expModId check inside FblHdrSetCalModuleInfo )  */
         if (FblHdrGetBasePartIdFromModuleId(parsedModuleInfo.MID) != parsedModuleInfo.partID)
         {
            FblSetPecErr(HdrErr_ModuleID); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
            retVal = kFblFailed;
         }
      }
#else
      /* parsedModuleInfo.partID is going to be overwritten for calibration modules */
      parsedModuleInfo.partID = FblHdrGetBasePartIdFromModuleId(parsedModuleInfo.MID);
#endif /* FBL_HDR_ENABLE_EXPLICIT_ERASE */
#if defined( FBL_ENABLE_CALIBRATION_MODULES )
      if (MOD_TYPE_CAL == parsedModuleInfo.modType)
      {
         if (PSI_PART_PRESENT != ApplFblChkPSIState(FblHdrGetBasePartIdFromModuleId(parsedModuleInfo.MID)))
         {
            FblSetPecErr(HdrErr_ExtSwNotPresentOnTransfer); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
            retVal = kFblFailed;
         }
         else
         {
# if defined( FBL_HDR_ENABLE_EXPLICIT_ERASE )
# else
            /* Pec error set internally in case of failure */
            retVal = FblHdrPrepareDownloadCalPartionData(parsedModuleInfo.MID);
            if (kFblOk == retVal )
# endif
            {
               if (HDR_NO_CALPART_ACTIVE == calPartActive )
               {
# if defined( FBL_HDR_ENABLE_EXPLICIT_ERASE )
                  /* this is expected to be never reached. */
                  assertFblInternal(0u, kFblAssertInternalIlegalState); /* PRQA S 2214 */ /* MD_FblAssert_Assertion */
#else
                  /*  If the Cal ID received is not the first to be programmed for that partition, the bootloader
                   *  shall return an error indicating a non-supported partition ID.
                   */
                  FblSetPecErr(HdrErr_PartitionId); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
                  retVal = kFblFailed;
#endif
               }
               else
               {
                  if (HDR_CALPART_START_SEQU == calPartActive )
                  {
                     /* Proceed to cal partition active state  */
                     calPartActive = HDR_CALPART_ACTIVE;
                  }
                  else
                  {
                     calInfo.calNbr++;
                     calInfo.startAddr +=HDR_CALINFO_LEN;
                  }
                  retVal = FblHdrSetCalModuleInfo();
               }
            }
         }
      }
      else
      {
         /* Nothing to do */
      }
#endif /* FBL_ENABLE_CALIBRATION_MODULES */
   }

   return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/***********************************************************************************************************************
 *  FblHdrCheckAndInitDownloadModuleInfo
 **********************************************************************************************************************/
/*! \brief       Check Signed header elements and Init parsedModuleInfo structure.
 *  \details     Checks: Mid, BCID/CCID, ECU name, ECU ID, Sw location Info
 *  \pre         This function is called only after SWM header information was received.
 *  \param[in]   signedHdrAddr Address where Signed Header Information is in memory
 *  \return      kFblOk=Success; kFblFailed=Failure
 **********************************************************************************************************************/
static tFblResult FblHdrCheckAndInitDownloadModuleInfo( const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * signedHdrAddr )
{
   tFblResult retVal = kFblFailed;

   if (kFblOk != FblHdrCheckAndParseDownloadMid( &signedHdrAddr[S1_MID_IDX] ))
   {
      /* Do nothing, pec error set internally */
   }
   else if (kFblOk != FblHdrCheckDownloadDcid( &signedHdrAddr[S1_DCID_IDX] ))
   {
      FblSetPecErr(HdrErr_CID); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
   }
   else if (kFblOk != FblHdrCheckEcuNameAndId( &signedHdrAddr[S1_ECUNAME_IDX], HDR_ECUID_DO_ZEROCHECK ))
   {
      /* Do nothing, pec error set internally */
   }
   else
   {

#if defined( FBL_ENABLE_CALIBRATION_MODULES )
      if (MOD_TYPE_CAL == parsedModuleInfo.modType)
      {
         retVal = kFblOk;
      }
      else
#endif
      {
         /* Pec is set internally */
         if (kFblOk == FblHdrCheckAndParseSwLocInfo( &signedHdrAddr[S1_APP_SWLINFO_IDX]))
         {
            retVal = kFblOk;
         }
      }
   }

   return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/***********************************************************************************************************************
 *  FblHdrSetMemLibMemBlockInfo
 **********************************************************************************************************************/
/*! \brief       Set LibMem segment address and length information
 **********************************************************************************************************************/
static void FblHdrSetMemLibMemBlockInfo( void )
{
   static SecM_VerifyParamType verifyParam;
   static SecM_SignatureParamType verifyParamPipelined;
   static vuint32 dataLengthVar;

   /* Init verifyParam data */
   verifyParam.segmentList = parsedModuleInfo.segmentList;

   verifyParam.wdTriggerFct = (FL_WDTriggerFctType)FblHdrPollingTaskVoid;
   verifyParam.readMemory = (tFblMemVerifyReadFct)FblReadProm;

   /* Init RoutinePipe Param */
   verifyParamPipelined.wdTriggerFct = (FL_WDTriggerFctType)FblHdrPollingTaskVoid;
   verifyParamPipelined.currentDataLength = &dataLengthVar;
   verifyParamPipelined.currentHash.length = 0u;
   verifyParamPipelined.currentHash.sigResultBuffer = 0u;

   blockInfo.verifyRoutineOutput.function = (tFblMemVerifyFctOutput)(FblLogicalBlockTable.logicalBlock[parsedModuleInfo.blockNbr].verifyROM); /* PRQA S 0313 */ /* MD_FblDef_Export */
   blockInfo.verifyRoutineOutput.param    = &verifyParam;

   /* Check for correct GENy config */
   assertFblGen(((tVerificationFct)FblLogicalBlockTable.logicalBlock[0].verifyROM == FblHdrVerifyIntegrity), kFblAssertConditionCorrect); /* PRQA S 0313 */ /* MD_FblDef_Export */ /* PRQA S 2214 */ /* MD_FblAssert_Assertion */

   {
      blockInfo.verifyRoutineInput.function  = (tFblMemVerifyFctInput) 0u;
      blockInfo.verifyRoutineInput.param     = V_NULL;
      blockInfo.verifyRoutinePipe.function   = (tFblMemVerifyFctInput)(FblLogicalBlockTable.logicalBlock[parsedModuleInfo.blockNbr].verifyRAM); /* PRQA S 0313 */ /* MD_FblDef_Export */
      blockInfo.verifyRoutinePipe.param      = &verifyParamPipelined;
   }

   blockInfo.segmentList         = &verifyParam.segmentList;
   blockInfo.maxSegments         = SWM_DATA_MAX_NOAR;
   blockInfo.readFct             = verifyParam.readMemory;

   blockInfo.targetAddress = FblLogicalBlockTable.logicalBlock[parsedModuleInfo.blockNbr].blockStartAddress;
   blockInfo.targetLength = FblLogicalBlockTable.logicalBlock[parsedModuleInfo.blockNbr].blockLength;

   /* This is not used */
   blockInfo.logicalAddress = parsedModuleInfo.blockNbr;
   blockInfo.logicalLength = 0u;
}

/***********************************************************************************************************************
 *  FblHdrSetMemLibSegmentAddrLengthInfo
 **********************************************************************************************************************/
/*! \brief       Set LibMem segment address and length information
 **********************************************************************************************************************/
static void FblHdrSetMemLibSegmentAddrLengthInfo( void )
{
   currMemLibSegment.targetAddress  = currDataSegInfo.targetAddress;
   currMemLibSegment.targetLength   = currDataSegInfo.length;
   /* Logical address and length not used */
   currMemLibSegment.logicalAddress = 0u;
   currMemLibSegment.logicalLength  = 0u;
}

/***********************************************************************************************************************
 *  FblHdrProcessHeaderInfo
 **********************************************************************************************************************/
/*! \brief       Init basic header information for currently downloaded module to parsedModuleInfo structure,
                 compare to compatibilty check to referrence module.
 *  \pre         This function is called only after SWM header information was received.
 *  \param[in]   signedHeaderAddr Address where Signed Header Information is in memory
 *  \return      Index of Plain Header inside rcvBuffer
 **********************************************************************************************************************/
static tTpDataType FblHdrProcessHeaderInfo( const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * signedHeaderAddr )
{
   tTpDataType retVal = HDR_DOWNLOAD_MODULE_NOT_ACCEPTED;
   tFblResult valResult;
   vuint8 const * signerPublicKeyAddr;
   vuint16 idxMsgDigest;
   vuint16 idxSignerInfo;
   vuint16 idxSignature;
   vuint16 locKeyNbidToCheck;

   if (kFblOk == FblHdrCheckAndInitDownloadModuleInfo(signedHeaderAddr))
   {
      /* Validate 1.Signer Info and 2. Signed Header Signature */
#if defined( FBL_ENABLE_CALIBRATION_MODULES )
      if (MOD_TYPE_CAL == parsedModuleInfo.modType)
      {
         idxMsgDigest = (vuint16)S1_CAL_MSGDIGEST_IDX;
      }
      else
#endif
      {
         idxMsgDigest = (vuint16)(S1_APP_SWLINFO_IDX + 2u + (8u * parsedModuleInfo.segmentList.nrOfSegments));
      }

      idxSignerInfo = (vuint16)(idxMsgDigest + HDR_MSGDIGEST_LEN);
      idxSignature = (vuint16)(idxSignerInfo + HDR_SIGINFO_LEN);

      locKeyNbidToCheck = (vuint16)FblMemGetInteger(HDR_NBID_LEN, &signedHeaderAddr[idxSignerInfo + SIGNINFO_KEYNBID_IDX]);
      signerPublicKeyAddr = FblHdrValidateSignerInfo( &signedHeaderAddr[idxSignerInfo], locKeyNbidToCheck );

      if (V_NULL != signerPublicKeyAddr)
      {
         /* Keep largest value during update */
         if (locKeyNbidToCheck > keyNBIDInfo.received)
         {
            keyNBIDInfo.received = locKeyNbidToCheck;
         }

         {
            valResult = FblHdrValidateSignature(&signedHeaderAddr[S1_MID_IDX], (idxSignature - S1_MID_IDX),
                        &signedHeaderAddr[idxSignature], signerPublicKeyAddr);
         }

#if ( FBL_MTAB_NO_OF_BLOCKS > 1 )
         appNBIDInfo.MID = parsedModuleInfo.MID;
#endif
         /* Init stored appNBID */
         appNBIDInfo.stored = 0u;
         appNBIDInfo.received = (vuint16)FblMemGetInteger(HDR_NBID_LEN, &signedHeaderAddr[S1_APP_NBID_IDX]);

         if (FblHdrGetSBITicketValid())
         {
            retVal = (vuint16)(idxSignature + HDR_SIGNATURE_LEN);
            appNBIDInfo.stored = appNBIDInfo.received;
         }
         else if (kFblOk == valResult)
         {
            if (
#if defined( FBL_ENABLE_CALIBRATION_MODULES )
                  (MOD_TYPE_APPL != parsedModuleInfo.modType) || /* PRQA S 3415 */ /* MD_FblHdr_3415 */
#endif
                  (kFblOk == FblHdrCheckAppNBID())
               )
            {
               retVal = (vuint16)(idxSignature + HDR_SIGNATURE_LEN);
            }
            else
            {
               FblSetPecErr(HdrErr_AppNBID); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
            }
         }
         else
         {
            FblSetPecErr(HdrErr_Signature); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
         }
      }

      /* Keep message digest for later verification */
      (void) MEMCPY(msgDigest, &signedHeaderAddr[idxMsgDigest], sizeof(msgDigest));
   }
   return retVal;
}  /* PRQA S 6080 */ /* MD_MSR_STMIF */

/***********************************************************************************************************************
 *  FblHdrTransferDataHeaderReceived
 **********************************************************************************************************************/
/*! \brief       This function is called on every first transfer data received for a new module.
 *  \details     It parses the header information inside the buffer data and checks for the received module type.
 *  \param[in]   signedHeaderAddr Address where Signed Header Information is in memory
 *  \return      Index of Plain Header inside rcvBuffer
 **********************************************************************************************************************/
static tTpDataType FblHdrTransferDataHeaderReceived( const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * signedHeaderAddr )
{
   tTpDataType retVal = HDR_DOWNLOAD_MODULE_NOT_ACCEPTED;
   tTpDataType hdrBytePosition;
   tTpDataType byteCounter;
   tFblMemStatus memLibResult;
   vuint16 dataTypeParse;

   FblHdrPollingTaskVoid();

   hdrBytePosition = FblHdrProcessHeaderInfo(signedHeaderAddr);

   if (HDR_DOWNLOAD_MODULE_NOT_ACCEPTED != hdrBytePosition)
   {
      /* Initialize to value different from DATA_TYPE_PLAIN_V1*/
      dataTypeParse = DATA_TYPE_SIGNED_V1;

      /* Parse until DATA_TYPE_PLAIN_V1 information */
      for (byteCounter = 0u; (byteCounter <= FBL_HDR_MAX_S1_FILL_LEN) && (DATA_TYPE_PLAIN_V1 != dataTypeParse); byteCounter += 2u)
      {
         FblHdrPollingTaskVoid();

         dataTypeParse = (vuint16)FblMemGetInteger(HDR_DATATYPE_LEN, &signedHeaderAddr[hdrBytePosition + byteCounter]);
      }

      if (dataTypeParse != DATA_TYPE_PLAIN_V1)
      {
         FblSetPecErr(HdrErr_ExtP1DataTypeNotPresent); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
         hdrBytePosition = HDR_DOWNLOAD_MODULE_NOT_ACCEPTED;
      }
      else
      {
         hdrBytePosition += byteCounter;
      }
   }

#if defined( FBL_HDR_ENABLE_EXPLICIT_ERASE )
#else
   /* Do implicit erase here */
   if (HDR_DOWNLOAD_MODULE_NOT_ACCEPTED != hdrBytePosition)
   {
# if defined( FBL_ENABLE_CALIBRATION_MODULES )
      if (( !IsCalDownLd() ) || ( IsCalFirstModInPartition() ))
# endif
      {
#if defined( FBL_HDR_ENABLE_MULTIPLE_DOWNLOAD_SOURCES )
         if (kFblOk != FblHdrEraseRoutine(parsedModuleInfo.partID, currDownloadSource))
#else
         if (kFblOk != FblHdrEraseRoutine(parsedModuleInfo.partID, HDR_DOWNLOAD_SOURCE_DIAG))
#endif
         {
            hdrBytePosition = HDR_DOWNLOAD_MODULE_NOT_ACCEPTED;
         }
      }
   }
#endif

   /* Modules applicable for erasure are checked inside function,
      for non applicable modules function returns kFblOk. */
   if (HDR_DOWNLOAD_MODULE_NOT_ACCEPTED != hdrBytePosition)
   {
      currDataSegIdx = 0u;
      currDataSegInfo = parsedModuleInfo.segmentList.segmentInfo[currDataSegIdx];
      currMemLibSegment.type = kFblMemType_ROM;

#if defined( FBL_ENABLE_SYSTEM_CHECK )
      currDataSegInitialLen = currDataSegInfo.length;
#endif

      FblHdrPollingTaskVoid();

      /* Indicate the start of a module download */
      FblHdrSetMemLibMemBlockInfo();
      memLibResult = FblMemBlockStartIndication(&blockInfo);
      if (kFblMemStatus_Ok != memLibResult)
      {
         FblSetPecErrIfNotYetSet((vuint16)memLibResult); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
      }
      else
      {
         /* Indicate start of new segment */
         FblHdrSetMemLibSegmentAddrLengthInfo();
         memLibResult = FblMemSegmentStartIndication(&currMemLibSegment);
         if (kFblMemStatus_Ok != memLibResult)
         {
            FblSetPecErrIfNotYetSet((vuint16)memLibResult);  /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
         }
         else
         {
            /* Return byte position in header. */
            retVal = hdrBytePosition;
         }
      }
   } /* if (HDR_DOWNLOAD_MODULE_NOT_ACCEPTED != hdrBytePosition) */

   return retVal;
} /* PRQA S 6050 */ /* MD_MSR_STCAL */ /* PRQA S 6030 */ /* MD_MSR_STCYC */ /* PRQA S 6010 */ /* MD_MSR_STPTH */

#if defined( FBL_ENABLE_GAP_FILL )
/***********************************************************************************************************************
 *  FblHdrFillRegion
 **********************************************************************************************************************/
/*! \brief       This function writes fill bytes to a region within one FlashBlock.
 *  \param[in]   inBeginAddr start address of region to fill
 *  \param[in]   inLength length of region to fill
 *  \return      kFblOk=Success; kFblFailed=Failure
 **********************************************************************************************************************/
static tFblResult FblHdrFillRegion( tFblAddress inBeginAddr, tFblLength inLength )
{
   vuint16 i;
   tFblResult retVal = kFblOk;
   tFblAddress fillChunkLen;
   vuint8 fillBuffer[HDR_GAPFILL_BUFF_SIZE];

   /* This assertion gets hit when some external memory device is used with higher Segment size than internal flash
    * (Writing fails through alignment issues, if assertion is disabled).
    * Please manually define HDR_GAPFILL_BUFF_SIZE as multiple of minimum write segment size for this device
    * to resolve this.
    */
#if defined( FBL_ENABLE_MULTIPLE_MEM_DEVICES )
   assertFblInternal( (((HDR_GAPFILL_BUFF_SIZE / MemDriver_SegmentSize) * MemDriver_SegmentSize) == HDR_GAPFILL_BUFF_SIZE), kFblAssertInternalIlegalState); /* PRQA S 2214 */ /* MD_FblAssert_Assertion */
#endif

   /* call FblMemSegmentNrGet() to init MemDriver_SegmentSize */
   memSegment = FblMemSegmentNrGet(inBeginAddr);

   if (inLength == 0u)
   {
      /* nothing to do */
   }
   else if (FblMemSegmentNrGet(FBL_UTIL_GET_RANGE_END(inBeginAddr, inLength)) != memSegment)
   {
      /* FblFillRegion() allows only to write within one Flashblock */
      retVal = kFblFailed;
   }
   else
   {
      {
#if ( HDR_GAPFILL_BUFF_SIZE <= 0x40 )
         FblHdrPollingTaskVoid();
#endif
         for (i = 0u; i < HDR_GAPFILL_BUFF_SIZE; ++i)
         {
#if ( HDR_GAPFILL_BUFF_SIZE > 0x40 )
            FblHdrPollingTaskInLoop(i, HDR_GAPFILL_BUFF_SIZE)  /* PRQA S 3109 */ /* MD_MSR_14.3 */
#endif
            fillBuffer[i] = kFillChar;
         }

         while (inLength > 0u)
         {
            FblHdrPollingTaskVoid();

            fillChunkLen = ((inLength > HDR_GAPFILL_BUFF_SIZE) ? HDR_GAPFILL_BUFF_SIZE : inLength);

            if (kFblMemStatus_Ok != FblMemProgramBuffer(inBeginAddr, &fillChunkLen, fillBuffer))
            {
               retVal = kFblFailed;
               break;
            }
            inLength -= fillChunkLen;
            inBeginAddr += fillChunkLen;
         }
      }
   }

   return retVal;
}

/***********************************************************************************************************************
 *  FblHdrGetSortedApplRegions
 **********************************************************************************************************************/
/*! \brief       Return requested Appl-Region in sorted order (0-first, 1-2nd, 2 ..)
 *               Appl Regions may be unsorted because of "Header segment must always lead" requirement.
 *  \param[out]   retSeg reference to requested sorted Region number
 *  \param[in]   regionNbr the (sorted) region number to read
 *  \return      kFblOk if regions were sucessfully saved.
 *               kFblFailed otherwise
 **********************************************************************************************************************/
static void FblHdrGetSortedApplRegions( FL_SegmentInfoType * retSeg, vuint8 regionNbr )
{
   vuintx hdrElemtRealNbr = 0u;
   vuintx idxToRequiredRegion;
   vuintx i;

   for (i = 0u; i < (vuintx)(parsedModuleInfo.segmentList.nrOfSegments - 1u); i++)
   {
      if (parsedModuleInfo.segmentList.segmentInfo[i].targetAddress > parsedModuleInfo.segmentList.segmentInfo[i + 1u].targetAddress)
      {
         FblHdrPollingTaskVoid();
         /* There is just one region that is potentially not sorted (Header Segment)
          * If a segment start address > successor start address, then according to A) the successor is  the header address
          */
         hdrElemtRealNbr = i + 1u;
         break;
      }
   }

   /* In case the elements are sorted, hdrElemtRealNbr stays 0 */
   if (regionNbr == hdrElemtRealNbr)
   {
      /* The header element is the (hdrElemtRealNbr)th element in sorted order, but the first element in the list */
      idxToRequiredRegion = 0u;
   }
   else if (regionNbr < hdrElemtRealNbr)
   {
      /* Elements below the (hdrElemtRealNbr) element are to be found with offset 1 (header segment is placed in front) */
      idxToRequiredRegion = regionNbr + 1u;
   }
   else
   {
      /* Elements after the (hdrElemtRealNbr) element are sorted */
      idxToRequiredRegion = regionNbr;
   }

   *retSeg = parsedModuleInfo.segmentList.segmentInfo[idxToRequiredRegion];
}
#endif /* FBL_ENABLE_GAP_FILL */

#if defined( FBL_ENABLE_CALIBRATION_MODULES )
# if defined( FBL_HDR_ENABLE_EXPLICIT_ERASE )
/***********************************************************************************************************************
 *  FblHdrPrepareDownloadCalPartionData
 **********************************************************************************************************************/
/*! \brief       Set Calibration Partition Data
 *  \details     This function is called upon reception of an erase routine for a given partition.
 *               Partition properties will be loaded to calPartitionInfo.
 *  \return      kFblOk = data successfully set; kFblFailed = else
 *  \param[in]   calPartId Calibration partition ID
 **********************************************************************************************************************/
static tFblResult FblHdrPrepareDownloadCalPartionData( vuint8 calPartId )
# else
/***********************************************************************************************************************
 *  FblHdrPrepareDownloadCalPartionData
 **********************************************************************************************************************/
/*! \brief       Set Calibration Partition Data
 *  \details      This function is called upon reception of a new calibration file.
 *               Partition properties will be loaded to calPartitionInfo. It is checked if the download is the first
 *               module of a partition or not. If it is the first module, calInfo and calPartActive are initialized
 *               additionally. Note: The first cal file received will trigger erasure of the partition in
 *               FblHdrTransferDataHeaderReceived.
 *  \return      kFblOk = data successfully set; kFblFailed = else
 *  \param[in]   calModId Calibration module ID
 **********************************************************************************************************************/
static tFblResult FblHdrPrepareDownloadCalPartionData( vuint8 calModId )
# endif
{
   tFblResult retVal;

# if defined( FBL_HDR_ENABLE_EXPLICIT_ERASE )
   retVal = FblHdrInitCalPartInfo(calPartId, &calPartitionInfo);
   if (kFblOk != retVal)
   {
      FblSetPecErr(HdrErr_PartitionId); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
      retVal = kFblFailed;
   }
# else
   tCalInfo V_MEMRAM2 readReceivedModCalInfo;
   retVal = FblHdrInitCalPartInfoAndCalInfo(calModId, &calPartitionInfo, &readReceivedModCalInfo);
   if (kFblOk != retVal)
   {
      FblSetPecErr(HdrErr_GetCalInfo); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
      retVal = kFblFailed;
   }
   else if (HDR_FIRST_CAL_NBR != readReceivedModCalInfo.calNbr)
   {
      /* calinfo already initialized. Increment calInfo within FblHdrCheckAndParseDownloadMid. */
   }
# endif
   else
   {
      calInfo.calNbr = HDR_FIRST_CAL_NBR;
      calInfo.startAddr = calPartitionInfo.partInfoAddr + (calPartitionInfo.nbrOfRgns * HDR_REGINFO_LEN) + (2u * HDR_NBROF_LEN);
      calPartActive = HDR_CALPART_START_SEQU;
   }

   return retVal;
}

/***********************************************************************************************************************
 *  FblHdrEraseCalPartition
 **********************************************************************************************************************/
/*! \brief       This function will erase a single calibration partition
 *  \pre         Operating software must be valid, SLP5: calPartitionInfo needs to be initialized.
 *  \param[in]   calPartId Calibration partition ID
 *  \return      kFblOk = erase succeeded; kFblFailed = erase failed
 **********************************************************************************************************************/
static tFblResult FblHdrEraseCalPartition( vuint8 calPartId )
{
   tFblResult retVal = kFblOk;
   tFblMemBlockInfo blockInfoErase;
   vuint16 locNbOfReg;
   tFblAddress locAddrIdx;
   tFlashStatus locErasedState = kFlashNotErased;
   vuint8 partNbr = FblHdrGetPartNbrFromId(calPartId);

# if defined( FBL_HDR_ENABLE_EXPLICIT_ERASE )
   /* Application software presence has to be checked already here. */
   if (PSI_PART_PRESENT != ApplFblChkPSIState(FblHdrGetBasePartId(calPartId)))
   {
      FblSetPecErr(HdrErr_SwNotPresent); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
      retVal = kFblFailed;
   }
   /* In case of application, only application PSI information is revoked */
   else if (kFblOk != FblHdrPrepareDownloadCalPartionData(calPartId))
   {
      /* Pec is set internally */
      retVal = kFblFailed;
   }
   else
# endif /* FBL_HDR_ENABLE_EXPLICIT_ERASE */
   {
      /* Always invalidate block, also if locErasedState below indicates block is erased. */
      if (kFblOk != ApplFblInvalidateBlock(*FblGetBlockDescriptor(calPartId)))
      {
         FblSetPecErr(HdrErr_Revoke); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
         retVal = kFblFailed;
      }
      else
      {
        /* Always erase in the case partNbr >= HDR_MAX_TRACKED_ERASED_STATE_PARTIONS (status is not be tracked). */
         if ((HDR_MAX_TRACKED_ERASED_STATE_PARTIONS) > partNbr)
         {
            locErasedState = FblHdrErasedStateVar(parsedModuleInfo.blockNbr, partNbr);
         }

         locAddrIdx = calPartitionInfo.partInfoAddr + HDR_NBROF_LEN;
         locNbOfReg = calPartitionInfo.nbrOfRgns;

         while ((locNbOfReg > 0u) && (kFblOk == retVal))
         {
            FL_SegmentInfoType tmpSeg;

            FblHdrPollingTaskVoid();

            /* Get the start address and length of the calibration partition */
            retVal = FblHdrGetRegionRomAndInc(&tmpSeg, (tFblAddress)0u, &locAddrIdx);

            if (kFblOk == retVal)
            {
# if defined( FBL_ENABLE_GAP_FILL )
               /* Track Partition Erase Segments for allowing Gap Fill. */
               (void) FblHdrPartSegmentListAddEntry(calPartId, &tmpSeg);
# endif
               blockInfoErase.targetAddress = tmpSeg.targetAddress;
               blockInfoErase.targetLength = tmpSeg.length;

               /* Erase partition */
               if (kFlashNotErased == locErasedState)
               {
                  if (kFblMemStatus_Ok != FblMemBlockEraseIndication(&blockInfoErase))
                  {
                     FblSetPecErr(HdrErr_EraseCal); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
                     retVal = kFblFailed;
                  }
               }
            }
            locNbOfReg--;
         } /* while (..) */

         if (     (kFlashNotErased == locErasedState)
               && (kFblOk == retVal))
         {
            /*  Status will not be tracked in the case partNbr >= HDR_MAX_TRACKED_ERASED_STATE_PARTIONS. */
            if ((HDR_MAX_TRACKED_ERASED_STATE_PARTIONS) > partNbr)
            {
               FblHdrErasedStateVar(parsedModuleInfo.blockNbr, partNbr) = kFlashErased;
            }
         }
      }
   }
   return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */ /* PRQA S 6050 */ /* MD_MSR_STCAL */ /* PRQA S 6030 */ /* MD_MSR_STCYC */ /* PRQA S 6010 */ /* MD_MSR_STPTH */
#endif /* defined( FBL_ENABLE_CALIBRATION_MODULES ) */

/***********************************************************************************************************************
 *  FblDiagTransferDataEnd
 **********************************************************************************************************************/
/*! \brief       When the last address-region of a module is downloaded, this function is called to handle the
 *               post-transfer tasks, such as checksum verification, signature verification, validation and device
 *               driver initialization.
 *  \return      kFblOk if the transferred data was successfully verified, kFblFailed if the verification failed.
 **********************************************************************************************************************/
#if defined( FBL_HDR_ENABLE_EXPLICIT_TRANSFEREND )
tFblResult FblHdrTransferDataEnd( void )
#else
static tFblResult FblHdrTransferDataEnd( void )
#endif
{
   tFblResult retVal = kFblFailed;
   tFblMemStatus memLibResult;
#if defined( FBL_HDR_ENABLE_PASSTHROUGH ) && defined( FBL_HDR_ENABLE_EXPLICIT_TRANSFEREND )
   tMcmpMemResult mcmpRetVal;
#endif

      /* Do sequence and parameter checks */
   if (FblHdrStateIsTransferExpected())
   {
      FblSetPecErr(HdrErr_MoreDataExpected);  /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
   }
   else if (!FblHdrStateIsTransferEndExpected())
   {
      FblSetPecErr(HdrErr_ExtTransferDataEndMsgOutOfSequence);  /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
   }
#if defined( FBL_HDR_ENABLE_PASSTHROUGH ) && defined( FBL_HDR_ENABLE_EXPLICIT_TRANSFEREND )
   else if (kFblOk == ApplFblHdrIsPassThroughActive(parsedModuleInfo, &isNewConnection))
   {
      /* Pass request to configured routine */
      mcmpRetVal = ApplFblHdrTransferDataEnd(&retVal, &errPecExtErrorCode, &rcvBuffHandle.progStepState);
      if (kMcmpMemResult_Ok != mcmpRetVal)
      {
         FblSetPecErr(HdrErr_Undefined); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
         retVal = kFblFailed;
      }
   }
#endif
   else
   {
      memLibResult = FblMemBlockEndIndication();
      if (kFblMemStatus_Ok != memLibResult)
      {
         FblSetPecErrIfNotYetSet((vuint16)memLibResult); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
      }
      else
      {
         retVal = FblHdrPostFlashValidation();

         if (kFblOk == retVal)
         {
# if defined( FBL_ENABLE_CALIBRATION_MODULES )
            if (IsCalDownLd() && (!IsCalLastModInPartition()))
            {
               FblHdrStateSetDownloadExpected(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
            }
            else
# endif
            {
               FblHdrStateSetUpdatePsiExpected(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
#if defined( FBL_HDR_ENABLE_EXPLICIT_UPDATEPSI )
#else
               retVal = FblHdrUpdatePSI(FblHdrGetParsedModPartId());
#endif
            }
         }
      }
   }

   if (kFblOk != retVal)
   {
      FblHdrStateSetIdle(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
   }

   return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/***********************************************************************************************************************
 *  FblHdrUpdatePSI
 **********************************************************************************************************************/
/*! \brief       Update PSI information in memory. Also write App/Key-NBID data
 *  \return      kFblOk=Success; kFblFailed=Failure
 **********************************************************************************************************************/
#if defined( FBL_HDR_ENABLE_EXPLICIT_UPDATEPSI )
tFblResult FblHdrUpdatePSI( vuint8 partId )
#else
static tFblResult FblHdrUpdatePSI( vuint8 partId )
#endif
{
   tFblResult retVal = kFblOk;
#if defined( FBL_HDR_ENABLE_PASSTHROUGH ) && defined( FBL_HDR_ENABLE_EXPLICIT_UPDATEPSI )
   tMcmpMemResult mcmpRetVal;
#endif

   if ((FblHdrGetParsedModPartId() != partId) || (!FblHdrStateIsUpdatePsiExpected()))
   {
      FblSetPecErr(HdrErr_ExtUpdatePsiMsgOutOfSequence); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
      retVal = kFblFailed;
   }
#if defined( FBL_HDR_ENABLE_PASSTHROUGH ) && defined( FBL_HDR_ENABLE_EXPLICIT_UPDATEPSI )
   else if (kFblOk == ApplFblHdrIsPassThroughActive(parsedModuleInfo, &isNewConnection))
   {
      /* Pass request to configured routine */
      mcmpRetVal = ApplFblHdrUpdatePsi(partId, &retVal, &errPecExtErrorCode);
      if (kMcmpMemResult_Ok != mcmpRetVal)
      {
         FblSetPecErr(HdrErr_Undefined); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
         retVal = kFblFailed;
      }
   }
#endif
   else
   {
#if defined( FBL_ENABLE_CALIBRATION_MODULES )
      if (MOD_TYPE_CAL != parsedModuleInfo.modType)
#endif
      {
         /* Update NBID information. */
         if (appNBIDInfo.received > appNBIDInfo.stored)
         {
            if (kFblOk != ApplFblNVMWriteAppNBID(&appNBIDInfo)) /* PRQA S 0310 */ /* MD_FblHdr_0310 */
            {
               FblSetPecErr(HdrErr_ExtWriteAppNbidFailed); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
               retVal = kFblFailed;
            }
         }
      }
      if (keyNBIDInfo.received > keyNBIDInfo.stored)
      {
         if (kFblOk != ApplFblNVMWriteKeyNBID(&keyNBIDInfo)) /* PRQA S 0306 */ /* MD_FblHdr_0306 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
         {
            FblSetPecErr(HdrErr_ExtWriteKeyNbidFailed); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
            retVal = kFblFailed;
         }
      }

      if (kFblOk != ApplFblValidateBlock(*FblGetBlockDescriptor(parsedModuleInfo.partID)))
      {
         FblSetPecErr(HdrErr_UpdatePSI); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
         retVal = kFblFailed;
      }
      else
      {
      }
   }

   FblHdrStateSetIdle(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
   return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/

#if defined( FBL_ENABLE_COMPRESSION_MODE )
/***********************************************************************************************************************
 *  FblHdrReadCmprHeader
 **********************************************************************************************************************/
/*! \brief       Read GM specified compression header
 *  \param[out]  cmprLength Length of the compressed data
 *  \param[in]   cmprBuffer data processing input data
 *  \param[out]  cmprDataOffset offset to the start of the compressed data
 *  \return      kFblOk Compression Header successfully read
 *               kFblFailed Compression Header reading failed
 **********************************************************************************************************************/
tFblResult FblHdrReadCmprHeader( V_MEMRAM1 tFblLength V_MEMRAM2 V_MEMRAM3 * cmprLength,
                                   const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * cmprBuffer,
                                   V_MEMRAM1 vuint16 V_MEMRAM2 V_MEMRAM3 * cmprDataOffset )
{
   tFblResult retVal = kFblFailed;
   vuint16 highCmprLen = (vuint16) FblMemGetInteger(CMPR_LEN_USED_IDX-CMPR_LEN_IDX, &cmprBuffer[CMPR_LEN_IDX]);

   *cmprDataOffset = CMPR_LEN_IDX + HDR_CMPR_DATALEN_LEN;
   *cmprLength = FblMemGetInteger(4u, &cmprBuffer[CMPR_LEN_USED_IDX]);

   /* Only 4 byte length dataType supported currently.*/
   if (0u == highCmprLen)
   {
      retVal = kFblOk;
   }

   return retVal;
}
#endif /* FBL_ENABLE_COMPRESSION_MODE */

/* PRQA S 2006 EOF */ /* MD_MSR_14.7 */

/***********************************************************************************************************************
 *  FblHdrInitPowerOn
 **********************************************************************************************************************/
/*! \brief       Init Power on for Header module
 **********************************************************************************************************************/
void FblHdrInitPowerOn( void )
{
   static FL_SegmentInfoType downloadSegments[SWM_DATA_MAX_NOAR];
   vuintx i;
   vuintx j;

   fblProgrammedState = 0u;

   /* Initialize parsedModuleInfo segmentInfo pointer */
   parsedModuleInfo.segmentList.segmentInfo = downloadSegments;

   /* Init as this value is compared previous to being written */
   keyNBIDInfo.received = 0u;

#if defined( FBL_ENABLE_CALIBRATION_MODULES )
   calPartitionInfo.partID = 0u;
#endif

   /* Init volatile erased state with default value "kFlashNotErased". */
   for(i = 0u; i < FBL_MTAB_NO_OF_BLOCKS; i++)
   {
      for(j = 0u; j < HDR_MAX_TRACKED_ERASED_STATE_PARTIONS; j++)
      {
         logBlockPartErasedStates[i].partErasedState[j] = kFlashNotErased;
      }
   }

#if ( FBL_MTAB_NO_OF_BLOCKS > 1 )
#else
   assertFblUser((  (MODID_OPSW_LOCAL == MODID_OPSW_1)  /* PRQA S 2214 */ /* MD_FblAssert_Assertion */ /* PRQA S 3325, 3201 */ /* MD_FblHdr_3325_3201 */
                  ||(MODID_OPSW_LOCAL == MODID_OPSW_2)
                  ||(MODID_OPSW_LOCAL == MODID_OPSW_3)
                  ||(MODID_OPSW_LOCAL == MODID_OPSW_4)), kFblAssertVariableInValidState);
#endif

#if defined( FBL_HDR_ENABLE_MULTIPLE_DOWNLOAD_SOURCES )
   currDownloadSource = HDR_DOWNLOAD_SOURCE_INIT;
#endif

   FblHdrStateSetIdle(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
}

/***********************************************************************************************************************
 *  FblHdrInit
 **********************************************************************************************************************/
/*! \brief       Init Header module, check SBA ticket
 **********************************************************************************************************************/
void FblHdrInit( tHdrInterfaceType sequenceType )
{
   const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * signerPublicKeyAddr;

#if defined( FBL_HDR_ENABLE_PASSTHROUGH )
   /* Store sequence type to be able to pass it to remote part as soon as remote address can be determined. */
   requSequType = sequenceType;
#else
# if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   /* Currently unused, fixed to sequence. */
   (void)sequenceType; /* PRQA S 3112 */ /* MD_MSR_14.2 */
# endif
#endif

   FblHdrClrSBITicketValid(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
   FblClrBisBytes(); /* PRQA S 3109 */ /* MD_MSR_14.3 */

#if defined( FBL_HDR_ENABLE_EXPLICIT_ERASE )
#else
# if defined( FBL_ENABLE_CALIBRATION_MODULES )
   /* Initialize Cal Partition state (start with not active state) */
   calPartActive = HDR_NO_CALPART_ACTIVE;
# endif
#endif

#if defined( FBL_HDR_ENABLE_BASIC_NVM_HANDLING )
   FblNbidInit();
#endif

#if defined( MCMP_MODE_SLAVE )
   /* Call to HdrInit is controlled from master, restore Bis memory information */
# if defined( FBL_ENABLE_ROM_INTEGRITY_CHECK )
   /* Update BIS information according to fblProgrammedState flags set in Ap - layer */
   if (kProgrammedStateRomError == (fblProgrammedState & kProgrammedStateRomError))
   {
       FblSetBisLsbByte(SBILSB_ERR_BOOTROMCHK);
   }
#endif /* FBL_ENABLE_ROM_INTEGRITY_CHECK */
# if defined( FBL_ENABLE_RAM_INTEGRITY_CHECK )
   if (kProgrammedStateRamError == (fblProgrammedState & kProgrammedStateRamError))
   {
       FblSetBisLsbByte(SBILSB_ERR_RAMCHK);
   }
# endif /* FBL_ENABLE_RAM_INTEGRITY_CHECK */
#endif /* MCMP_MODE_SLAVE */

   /********************************************************************/
   /* Read and test SBA ticket                                         */
   /********************************************************************/

   /* Use SBA_TICKET_PARSE_BUFFER during init to read SBA ticket. HDR_SBA_TICKET_LEN (822 bytes) are to be read */
   if (kFblOk != ApplFblNVMReadSBATicket(SBA_TICKET_PARSE_BUFFER))  /* PRQA S 0306 */ /* MD_FblHdr_0306 */
   {
      /* Do nothing */
   }
   else if (MODID_SBA != FilterMid(FblMemGetInteger(HDR_MID_LEN, &SBA_TICKET_PARSE_BUFFER[S1_SBA_MID_IDX])))
   {
      /* Do nothing */
   }
   else if (kFblOk != FblHdrCheckEcuNameAndId(&SBA_TICKET_PARSE_BUFFER[S1_SBA_ECUNAME_IDX], HDR_ECUID_SKIP_ZEROCHECK))
   {
      /* Read error from  FblHdrCheckEcuNameAndId */
      assertFblInternal( (HdrErr_EcuId == FblGetExtPecErr()) || (HdrErr_EcuName == FblGetExtPecErr()), kFblAssertInternalIlegalState); /* PRQA S 2214 */ /* MD_FblAssert_Assertion */
      FblSetBisMsbByte( (vuint8)((HdrErr_EcuId == FblGetExtPecErr())? SBIMSB_ERR_ECUID : SBIMSB_ERR_ECUNAME) ); /* PRQA S 3109 */ /* MD_MSR_14.3 */
   }
   else
   {
      vuint16 locKeyNbidToCheck = (vuint16)FblMemGetInteger(HDR_NBID_LEN, &SBA_TICKET_PARSE_BUFFER[S1_SBA_SIGNINFO_IDX + SIGNINFO_KEYNBID_IDX]);
      signerPublicKeyAddr = FblHdrValidateSignerInfo(&SBA_TICKET_PARSE_BUFFER[S1_SBA_SIGNINFO_IDX], locKeyNbidToCheck);
      if (V_NULL == signerPublicKeyAddr)
      {
         /* Overwrite error info ( for SBA error values are not correctly set ) */
         FblSetBisMsbByte( SBIMSB_ERR_CERT ); /* PRQA S 3109 */ /* MD_MSR_14.3 */

      }
      else
      {
         if (kFblOk != FblHdrValidateSignature(&SBA_TICKET_PARSE_BUFFER[S1_SBA_MID_IDX], S1_SBA_SIGNATURE_IDX - S1_SBA_MID_IDX,
                                               &SBA_TICKET_PARSE_BUFFER[S1_SBA_SIGNATURE_IDX], signerPublicKeyAddr))
         {
            FblSetBisMsbByte(SBIMSB_ERR_SIGNATURE); /* PRQA S 3109 */ /* MD_MSR_14.3 */
         }
         else
         {
            FblHdrSetSBITicketValid(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
            FblSetBisMsbByte(SBIMSB_SBI_VALID); /* PRQA S 3109 */ /* MD_MSR_14.3 */
         }
      }
   }

   /* Ensure ECU starts with no Pec error set. */
   FblClrPecErr(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
} /* PRQA S 6050 */ /* MD_MSR_STCAL */ /* PRQA S 6080 */ /* MD_MSR_STMIF */

/***********************************************************************************************************************
 *  FblHdrGetReportablePecError
 **********************************************************************************************************************/
/*! \brief       Returns reportable Pec Error code
 *  \return      Global B GB6002/Global A defined Programming Error Code (PEC)
 **********************************************************************************************************************/
vuint16 FblHdrGetReportablePecError( void )
{
   vuint16 retVal;

   if (HdrErr_ExtBase == (errPecExtErrorCode & (HdrErr_ExtBase)))
   {
      /* If HdrErr_ExtBase is set a filtering need to happen to only allow reporting of GM defined Pec
       * values and no internally used ones.
       */
      if (HdrErr_ExtMapOff == (errPecExtErrorCode & (HdrErr_ExtMapOff)))
      {
         /* For internal Pec where HdrErr_ExtMapOff is set, report Basic GM defined Pec part */
         retVal = (vuint16)(errPecExtErrorCode & HdrErr_ExtBaseMapMask);
      }
      else
      {
         /* Set HdrErr_Undefined if HdrErr_ExtBase bit is set and HdrErr_ExtMapOff not. */
         retVal = (vuint16)HdrErr_Undefined;
      }
   }
   else
   {
      retVal = (vuint16)errPecExtErrorCode;
   }

   return retVal;
}

#if ( FBL_MTAB_NO_OF_BLOCKS > 1 )
/***********************************************************************************************************************
 *  GetBlockNbrFromPartId
 **********************************************************************************************************************/
/*! \brief       Returns BlockNumber from partId;
 *  \detail      partId of application is configured in logicalBlock[n].blockIndex element in order to help to address
 *               shared application/calibration space represented by a logical block.
 *               Call FblHdrGetBasePartId on input partId to allow comparision in any case.
 *
 *  \param[in]   partId some partition ID for which logical block table mapping shall be queried
 *  \return      HDR_UNDEFINED_LOG_BLOCK - no associated block found;
                 Block number of found associated logical block.
 **********************************************************************************************************************/
vuint8 GetBlockNbrFromPartId( vuint8 partId )
{
   vuint8 retVal = HDR_UNDEFINED_LOG_BLOCK;
   vuint8 i;

   FblHdrPollingTaskVoid();

   for(i = 0u; i < FBL_MTAB_NO_OF_BLOCKS; i++)
   {
      if (FblLogicalBlockTable.logicalBlock[i].blockIndex == FblHdrGetBasePartId(partId))
      {
         /* Partition ID for Application1 has to be first entry in Logical Block Table  */
         assertFblUser( ((0x00u == FblLogicalBlockTable.logicalBlock[i].blockNr) || /* PRQA S 2214 */ /* MD_FblAssert_Assertion */
               (PARTID_OPSW_LOCAL != FblLogicalBlockTable.logicalBlock[i].blockIndex)), kFblAssertAppl1NotFirstBlock);
         assertFblUser( ((0x00u != FblLogicalBlockTable.logicalBlock[i].blockNr) || /* PRQA S 2214 */ /* MD_FblAssert_Assertion */
               (PARTID_OPSW_LOCAL == FblLogicalBlockTable.logicalBlock[i].blockIndex)) , kFblAssertAppl1NotFirstBlock);
         retVal = FblLogicalBlockTable.logicalBlock[i].blockNr;
         break;
      }
   }

   return retVal;
}
#endif

/***********************************************************************************************************************
 *  FblHdrPollingTaskVoid
 **********************************************************************************************************************/
/*! \brief       Wrap FblHdrPollingTask with function not returning a value
 **********************************************************************************************************************/
void FblHdrPollingTaskVoid( void )
{
   (void)FblHdrPollingTask();
}

/***********************************************************************************************************************
 *  FblGetBlockDescriptor
 **********************************************************************************************************************/
/*! \brief       Prepares a Dummy block descriptor that holds the module id required to satisfy the presence pattern API
 *  \param[in]   partId id that sets current blockNr
 *  \return      Dummy block descriptor prepared.
 **********************************************************************************************************************/
V_MEMRAM1 tBlockDescriptor V_MEMRAM2 V_MEMRAM3 * FblGetBlockDescriptor( vuint8 partId )
{
   static  V_MEMRAM1 tBlockDescriptor V_MEMRAM2 dummyBlockDescriptor;
   dummyBlockDescriptor.blockNr = partId;
   return &dummyBlockDescriptor;
}

/***********************************************************************************************************************
 *  FblMemSegmentNrGet - GM specific
 **********************************************************************************************************************/
/*! \brief       Get the number of the corresponding block for the given address
 *  \param[in]   address address to find
 *  \return      Index of flash block; kFblHdrMemSegmNotFound, if not found
 **********************************************************************************************************************/
vsint16 FblMemSegmentNrGet( tFblAddress address )
{
   vsint16 locMemSeg;

   locMemSeg = (vsint16) (kNrOfFlashBlock - 1u);

   while ((locMemSeg >= 0) && ((address < FlashBlock[locMemSeg].begin) || (address > FlashBlock[locMemSeg].end)))
   {
      /* locMemSeg is always > 0 here. */
      FblHdrPollingTaskInLoop((vuint16)locMemSeg, 0x10u); /* PRQA S 3109 */ /* MD_MSR_14.3 */
      locMemSeg--;
   }

   return locMemSeg;
}

/***********************************************************************************************************************
 *  FblReadProm
 **********************************************************************************************************************/
/*! \brief       This function checks the FlashBlock table and copies a given amount of bytes from the Flash memory.
 *  \details     If no entry in FlashBlock table is found, no device driver can be selected and
 *               the function will return the failed result 0.
 *  \param[in]   address Memory address to read out
 *  \param[out]  buffer target buffer
 *  \param[in]   length number of bytes to read
 *  \return      Number of actually copied bytes
 **********************************************************************************************************************/
tFblLength FblReadProm( tFblAddress address, vuint8* buffer, tFblLength length )
{
   tFblLength  actualReadCount = 0u;
   tFblLength  currentReadCount;
   tFblLength  lenInFlashBlock;

   memSegment = FblMemSegmentNrGet(address);
   if (kFblHdrMemSegmNotFound == memSegment)
   {
      return 0u;
   }

   /* Because the device drive can change on boundaries of FlashBlock[] we need to read all data from each
    * individual FlashBlock */
   while (length > 0u)
   {
      FblHdrPollingTaskVoid();

      memSegment = FblMemSegmentNrGet(address);
      if (kFblHdrMemSegmNotFound != memSegment)
      {
         lenInFlashBlock = (FlashBlock[memSegment].end - address) + 1u;
         /* If number of data exceeds current block, we are only allowed to read to the end of the current block. */
         currentReadCount = (( length > lenInFlashBlock )? lenInFlashBlock : length);

         /* Get the memory data through the device driver's read function */
         if (IO_E_OK != MemDriver_RReadSync((IO_MemPtrType)buffer,(IO_SizeType)currentReadCount,(IO_PositionType)address))
         {
            /* Read failure. End of read operation */
            length = 0u;
         }
         else
         {
            /* Read operation successful. Prepare for end or next loop */
            actualReadCount += currentReadCount;
            buffer           = &buffer[currentReadCount];
            address         += currentReadCount;
            length          -= currentReadCount;
         }
      }
      else
      {
         length = 0u;
      }
   }
   return actualReadCount;
}

/***********************************************************************************************************************
 *  FblHdrPipelinedVerifyIntegrity
 **********************************************************************************************************************/
/*! \brief       Perform Application / Calibration verify actions (depending on configuration)
 *               on ROM data after single transfer reception (FBL_MEM_ENABLE_VERIFY_INPUT/non readable device:
 *               on RAM data if ApplFblNrdGetDeviceReadSupport returns failed, check FblHdrPostFlashValidation )
 *  \param[in]   verifyParam verify parameter structure
 *  \return      SECM_VER_OK / SECM_VER_ERROR
 **********************************************************************************************************************/
SecM_StatusType FblHdrPipelinedVerifyIntegrity( V_MEMRAM1 SecM_SignatureParamType V_MEMRAM2 V_MEMRAM3 * verifyParam ) /* PRQA S 3112 */ /* MD_MSR_14.2 */
{
   SecM_StatusType retVal;

#if defined( FBL_MEM_ENABLE_VERIFY_PIPELINED ) 
   /* Verify Hash / Digest */
   retVal = SecM_VerifyClassDDD(verifyParam);
#else
   retVal = SECM_VER_OK;
#endif
   return retVal;
}

/***********************************************************************************************************************
 *  FblHdrVerifyIntegrity
 **********************************************************************************************************************/
/*! \brief       Perform Application / Calibration verify actions (depending on configuration)
 *               on ROM data after whole module transfer. (FBL_MEM_ENABLE_VERIFY_INPUT/non readable device:
 *               call user callback ApplFblNrdVerify for device specific verification )
 *  \param[in]   verifyParam verify parameter structure
 *  \return      SECM_VER_OK / SECM_VER_ERROR
 **********************************************************************************************************************/
SecM_StatusType FblHdrVerifyIntegrity( V_MEMRAM1 SecM_VerifyParamType V_MEMRAM2 V_MEMRAM3* verifyParam ) /* PRQA S 3673 */ /* MD_FblHdr_3673 */
{
   SecM_StatusType retVal;
#if defined( FBL_MEM_ENABLE_VERIFY_PIPELINED )
# if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   (void) verifyParam; /* Not used in this configuration */ /* PRQA S 3112 */ /* MD_MSR_14.2 */
# endif
#endif

#if defined( FBL_MEM_ENABLE_VERIFY_PIPELINED ) 
   {
      retVal = SECM_VER_OK;
   }

# if defined( FBL_ENABLE_VERIFY_INTEGRITY_WORD )
   /* Verify Integrity word */
   (void) SecM_InitVerification(V_NULL);
   retVal = SecM_VerificationClassVendor(verifyParam);
   (void) SecM_DeinitVerification(V_NULL);
# endif
#else
   /* Verify Hash / Digest (and optionally Integrity word, if FBL_ENABLE_VERIFY_INTEGRITY_WORD is set) */
   (void) SecM_InitVerification(V_NULL);
   retVal = SecM_Verification(verifyParam);
   (void) SecM_DeinitVerification(V_NULL);
#endif /* defined( FBL_MEM_ENABLE_VERIFY_PIPELINED ) || defined( FBL_MEM_ENABLE_VERIFY_INPUT ) */

   return retVal;
}

/***********************************************************************************************************************
 *  FblHdrGetBaseModuleHdrAddr
 **********************************************************************************************************************/
/*! \brief       Retrieve the header address for all base modules stored in memory.
 *  \pre         NO_HEADER_ADDR_DEFINED == 0xFFFFFFFF is not allowed for header address
 *  \param[in]   partId Module Identifier of the base module address to retrieve.
 *  \return      Base module address
 **********************************************************************************************************************/
tFblAddress FblHdrGetBaseModuleHdrAddr( vuint8 partId )
{
   tFblAddress retVal = HDR_NO_HEADER_ADDR_DEFINED;

   if (PartIdOpSw(partId))
   {
      vuint8 blockN = GetBlockNbrFromPartId(partId);
#if ( FBL_MTAB_NO_OF_BLOCKS > 1 )
      if (HDR_UNDEFINED_LOG_BLOCK != blockN )
#endif
      {
         retVal = ApplFblGetModuleHeaderAddress(blockN);
      }
   }
   else
   {
      /* Nothing to do */
   }
   return retVal;
}

/***********************************************************************************************************************
 *  FblHdrCheckModuleValidAndGetAddr
 **********************************************************************************************************************/
/*! \brief       Return address of where downloaded header is stored in memory, if module is valid;
 *               return NO_HEADER_ADDR_DEFINED else.
 *  \details     This function is also called on mid input from external. The mid therefore has to be checked to be
                 of supported module type first.
 *  \param[in]   mid Module ID (any ID, including calibration IDs)
 *  \return      Address where header is stored.
 **********************************************************************************************************************/
tFblAddress FblHdrCheckModuleValidAndGetAddr( vuint8 mid )
{
   tFblAddress retVal = HDR_NO_HEADER_ADDR_DEFINED;

   if (MOD_TYPE_NOTSUPPORTED != FblGetModType(mid))
   {
      if (kFblOk == FblHdrChkIdPresent(mid))
      {
#if defined( FBL_ENABLE_CALIBRATION_MODULES )
         if (MOD_TYPE_CAL == FblGetModType(mid))
         {
            tCalInfo locCalInfo;
            tCalPartitionInfo locPartInfo;
            tFblResult readRetVal = kFblOk;

            locPartInfo.partID = 0u;
            {
               if (kFblOk == FblHdrInitCalPartInfoAndCalInfo(mid, &locPartInfo, &locCalInfo))
               {
                  /* Get the start address of the calibration module */
                  retVal = FblHdrGetIntegerRom(&readRetVal, HDR_REGADDR_LEN, (locCalInfo.startAddr + HDR_MID_LEN + HDR_DCID_LEN));

                  if (kFblOk != readRetVal)
                  {
                     retVal = HDR_NO_HEADER_ADDR_DEFINED;
                  }
               }
            }
         }
         else
#endif
         {
            retVal = FblHdrGetBaseModuleHdrAddr(FblHdrGetBasePartIdFromModuleId(mid));
         }
      }
   }

   return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/***********************************************************************************************************************
 *  FblGetModType
 **********************************************************************************************************************/
/*! \brief       Translates ID to Module Type
 *  \param[in]   mid Module ID (any ID, including calibration IDs)
 *  \return      Module Type
 **********************************************************************************************************************/
vuint8 FblGetModType( vuint8 mid )
{
   vuint8 locModType;

   if (ModIdOpSw(mid))
   {
      locModType = MOD_TYPE_APPL;
   }
#if defined( FBL_ENABLE_CALIBRATION_MODULES )
   /* not supported in case of non multiple modules configuration */
   else if (ModIdCalibration(mid))
   {
      locModType = MOD_TYPE_CAL;
   }
#endif
   else
   {
      locModType = MOD_TYPE_NOTSUPPORTED;
   }

   return locModType;
}

/* PRQA S 0715 TAG_FblHdr_0715 */ /* MD_MSR_1.1_715 */
/***********************************************************************************************************************
 *  FblHdrTransferDataProcess
 **********************************************************************************************************************/
/*! \brief       This function is to be called for newly received data.
 *  \details     It checks current rcvBuffHandle.progStepState and decides if the currently processed transfer is the
 *               first, or any later transfer. FblHdrTransferDataHeaderReceived() is called for first
 *               transfer. TransferData Exit is allowed when all expected byte have been received.
 *               The function interfaces with the LibMem module to program the received data.
 *  \param[in]   rcvBuffer received data bytes
 *  \param[in]   rcvDataSize received data bytes
 **********************************************************************************************************************/
tFblResult FblHdrTransferDataProcess( const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * rcvBuffer, tTpDataType rcvDataSize )
{
   tFblMemStatus memLibResult;
   tTpDataType bytesForDataInd;
   tTpDataType leftDataSize;
   tFblResult retVal = kFblOk;
   vuint8 partNbr;
#if defined( FBL_HDR_ENABLE_PASSTHROUGH )
   tMcmpMemResult mcmpRetVal;
#endif

   if (FblHdrStateIsTransferEndExpected())
   {
      FblSetPecErr(HdrErr_LengthExceeded); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
      retVal = kFblFailed;
   }
   else if (!FblHdrStateIsTransferExpected())
   {
      FblSetPecErr(HdrErr_ExtTransferDataMsgOutOfSequence); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
      retVal = kFblFailed;
   }
#if defined( FBL_HDR_ENABLE_PASSTHROUGH ) && defined( FBL_HDR_ENABLE_EXPLICIT_ERASE )
   else if ( kFblOk == ApplFblHdrIsPassThroughActive(parsedModuleInfo, &isNewConnection))
   {
      /* Pass request to configured routine */
      mcmpRetVal = ApplFblHdrTransferDataProcess(rcvDataSize, rcvBuffer, &retVal, &errPecExtErrorCode, &rcvBuffHandle.progStepState);
      if (kMcmpMemResult_Ok != mcmpRetVal)
      {
         FblSetPecErr(HdrErr_ExtNrcConditionsNotCorrect); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
         retVal = kFblFailed;
      }
   }
#endif /* defined( FBL_HDR_ENABLE_PASSTHROUGH ) && defined( FBL_HDR_ENABLE_EXPLICIT_ERASE ) */
   else
   {
      /* Init data processing state */
      rcvBuffHandle.buffIdx = 0u;
      rcvBuffHandle.progDataOffset = 0u;

      if (FblHdrStateIsTransferHdrExpected())
      {
         const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * pSignedHeader;
# if defined( FBL_ENABLE_COMPRESSION_MODE )
         /* compressedLength will read out from compression header and set in FblHdrCheckEnvelopesExtractSignedHeader */
         compressedLength = 0u;
# endif

         /* Check for compression type/ signed type, forward rcvBuffer to Signed header structure */
         pSignedHeader = FblHdrCheckEnvelopesExtractSignedHeader(rcvBuffer, rcvDataSize);

         if (V_NULL == pSignedHeader)
         {
            /* Pec Error set in FblHdrCheckEnvelopesExtractSignedHeader */
            retVal = kFblFailed;
         }
         else
         {
#if defined( FBL_HDR_ENABLE_PASSTHROUGH ) && !defined( FBL_HDR_ENABLE_EXPLICIT_ERASE )
            /* Early init Module ID Info to received ID information, do not filter User part of Module ID here. */
            parsedModuleInfo.MID = (vuint8) FblMemGetInteger(HDR_MID_LEN, &pSignedHeader[S1_MID_IDX] );

            if (kFblOk == ApplFblHdrIsPassThroughActive(parsedModuleInfo, &isNewConnection))
            {
               if (kFblOk == isNewConnection)
               {
                  /* Initialize remote header module after slave address information is available */
                  mcmpRetVal = ApplFblHdrInit((vuint8)requSequType);
                  if (kMcmpMemResult_Ok != mcmpRetVal)
                  {
                     FblSetPecErr(HdrErr_ExtNrcConditionsNotCorrect); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
                     retVal = kFblFailed;
                  }
               }

               if (kFblOk == retVal)
               {
                  /* Send RequestDownload through interface */
                  mcmpRetVal = ApplFblHdrRequestDownload(HDR_DOWNLOAD_SOURCE_REMOTE, &retVal, &errPecExtErrorCode);
                  if (kMcmpMemResult_Ok != mcmpRetVal)
                  {
                     FblSetPecErr(HdrErr_ExtNrcConditionsNotCorrect); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
                     retVal = kFblFailed;
                  }
               }
            }
            else
#endif /* defined( FBL_HDR_ENABLE_PASSTHROUGH ) && !defined( FBL_HDR_ENABLE_EXPLICIT_ERASE ) */
            {
               rcvBuffHandle.progDataOffset = FblHdrTransferDataHeaderReceived(pSignedHeader);
               if (HDR_DOWNLOAD_MODULE_NOT_ACCEPTED == rcvBuffHandle.progDataOffset)
               {
                  retVal = kFblFailed;
               }
               else
               {
#if defined( FBL_ENABLE_DATA_PROCESSING )
                  if (FblHdrModIsRawData())
#endif
                  {
                     /* no data processing interface is being called, add offset directly, not via Dfi */
                     rcvBuffHandle.buffIdx += rcvBuffHandle.progDataOffset;
                  }

                  partNbr = FblHdrGetPartNbrFromId(parsedModuleInfo.partID);
                  /* Clear erased state flag before starting to program. */
#if defined( FBL_ENABLE_CALIBRATION_MODULES )
                  if ((HDR_MAX_TRACKED_ERASED_STATE_PARTIONS) > partNbr)
#endif
                  {
                     FblHdrErasedStateVar(parsedModuleInfo.blockNbr, partNbr) = kFlashNotErased;
                  }
                  FblHdrStateSetTransferNoHdrExpected(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
               }
            }
         }
      }

#if defined( FBL_HDR_ENABLE_PASSTHROUGH ) && !defined( FBL_HDR_ENABLE_EXPLICIT_ERASE )
      if (kFblOk == ApplFblHdrIsPassThroughActive(parsedModuleInfo, &isNewConnection))
      {
         /* Pass request to configured routine */
         mcmpRetVal = ApplFblHdrTransferDataProcess(rcvDataSize, rcvBuffer, &retVal, &errPecExtErrorCode, &rcvBuffHandle.progStepState);
         if (kMcmpMemResult_Ok != mcmpRetVal)
         {
            FblSetPecErr(HdrErr_ExtNrcConditionsNotCorrect); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
            retVal = kFblFailed;
         }
      }
      else
#endif
      {
         if (FblHdrStateIsTransferNoHdrExpected())
         {
            leftDataSize = (tTpDataType) (rcvDataSize - rcvBuffHandle.buffIdx);

# if defined( FBL_ENABLE_COMPRESSION_MODE )
            if (compressedLength >= (tFblLength) leftDataSize)
            {
               compressedLength -= (tFblLength) leftDataSize;
            }
            else if (compressedLength != 0u)
            {
               FblSetPecErr(HdrErr_LengthExceeded); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
               retVal = kFblFailed;
            }
            else
            {
               /* For Raw data and/or compression not using compressedLength it will be always 0. */
            }
# endif /* FBL_ENABLE_COMPRESSION_MODE */

            /* Only count consumed bytes == received bytes in case of no data processing */
            while ( ((leftDataSize > 0u) && (kFblOk == retVal))
#if defined( FBL_ENABLE_COMPRESSION_MODE )
                     || ((!FblHdrModIsRawData()) && (0u == compressedLength) && (currDataSegIdx < parsedModuleInfo.segmentList.nrOfSegments))
#endif
                  )
            {
               FblHdrPollingTaskVoid();

               bytesForDataInd = (tTpDataType) FblHdrGetDataIndBytes(leftDataSize);

               memLibResult = FblMemDataIndication((tFblMemConstRamData)rcvBuffer, (tFblLength)rcvBuffHandle.buffIdx, (tFblLength)bytesForDataInd);
               if (kFblMemStatus_Ok != memLibResult)
               {
                  FblSetPecErrIfNotYetSet((vuint16)memLibResult); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
                  retVal = kFblFailed;
                  break;
               }

#if defined( FBL_ENABLE_DATA_PROCESSING )
               if (FblHdrModIsRawData())
#endif /* FBL_ENABLE_DATA_PROCESSING */
               {
                  /* Update segment info.
                   * Note:This is done behind data processing interface in case received data is not raw. */
                  FblHdrConsumeCurrSegmentBytes(bytesForDataInd); /* PRQA S 3109 */ /* MD_MSR_14.3 */
                  rcvBuffHandle.buffIdx += bytesForDataInd;
                  /* Always reset progDataOffset after first programmed data chunk ( which includes the header ) */
                  rcvBuffHandle.progDataOffset = 0u;
               }

               /* rcvBuffHandle.buffIdx has been updated (inside FblMemDataIndication in case of compression ), also update leftDataSize */
               leftDataSize = (tTpDataType) (rcvDataSize - rcvBuffHandle.buffIdx);

               if (  (currDataSegInfo.length == (tFblLength)0u)
#if defined( FBL_ENABLE_COMPRESSION_MODE )
                   || ((!FblHdrModIsRawData()) && (compressedLength == (tFblLength)0u))
#endif
                   )
               {
                  tFblLength addrRegionLength;
                  memLibResult = FblMemSegmentEndIndication(&addrRegionLength);

                  assertFblInternal((currDataSegInfo.length == 0),kFblAssertInternalIlegalState); /* PRQA S 2214 */ /* MD_FblAssert_Assertion */
                  currDataSegIdx++;

                  if (kFblMemStatus_Ok != memLibResult)
                  {
                     FblSetPecErrIfNotYetSet((vuint16)memLibResult); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
                     retVal = kFblFailed;
                  }
#if defined( FBL_ENABLE_SYSTEM_CHECK )
                  else if (addrRegionLength != currDataSegInitialLen)
                  {
                     FblSetPecErr(HdrErr_ExtValueOutOfRange); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
                     retVal = kFblFailed;
                  }
#endif /* FBL_ENABLE_SYSTEM_CHECK */
                  else if (currDataSegIdx < parsedModuleInfo.segmentList.nrOfSegments)
                  {
                     /* Start of new segment */
                     currDataSegInfo = parsedModuleInfo.segmentList.segmentInfo[currDataSegIdx];
#if defined( FBL_ENABLE_SYSTEM_CHECK )
                     currDataSegInitialLen = currDataSegInfo.length;
#endif

                     FblHdrSetMemLibSegmentAddrLengthInfo();
#if defined( FBL_ENABLE_DATA_PROCESSING )
                     if (  (!FblHdrModIsRawData()) )
                     {
                        if (currDataSegIdx == (parsedModuleInfo.segmentList.nrOfSegments - 1))
                        {
                          /* Indicate this is the last segment being processed */
                          currMemLibSegment.dataFormat |= MODULE_DF_LAST_SEGMENT;
                        }
                     }
#endif /* FBL_ENABLE_DATA_PROCESSING */
                     memLibResult = FblMemSegmentStartIndication(&currMemLibSegment);
                     if (kFblMemStatus_Ok != memLibResult)
                     {
                        /* Pec Error set internally */
                        FblSetPecErrIfNotYetSet((vuint16)memLibResult); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
                        retVal = kFblFailed;
                     }
                  }
                  else
                  {
                     /* no further segments; Still data? */
                     if (     (leftDataSize != 0u)
#if defined( FBL_ENABLE_COMPRESSION_MODE )
                           || (compressedLength != 0u)
#endif
                        )
                     {
                        FblSetPecErr(HdrErr_LengthExceeded); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
                        retVal = kFblFailed;
                     }
                     else
                     {
                        /* Module programming is successfully completed. Set State to allow FblHdrTransferDataEnd Routine. */
                        FblHdrStateSetTransferEndExpected(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
#if defined( FBL_HDR_ENABLE_EXPLICIT_TRANSFEREND )
#else
                        /* Pec Error set internally */
                        retVal = FblHdrTransferDataEnd();
#endif
                     }
                  } /* if (kFblMemStatus_Ok != memLibResult) */
               } /* if (currDataSegInfo.length == (tFblLength)0u) */
            } /* while ((leftDataSize > 0u) && (kFblOk == retVal)) */
         } /* if (FblHdrStateIsTransferNoHdrExpected()) */
      } /* if ApplFblHdrIsPassThroughActive(parsedModuleInfo, &isNewConnection) */
   } /* if (FblHdrStateIsTransferEndExpected()) */

   if (kFblOk != retVal)
   {
      FblHdrStateSetIdle(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
   }
   else
   {
      /* State is either kept or already changed above (FblHdrStateSetTransferEndExpected()) */
   }

   return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */ /* PRQA S 6050 */ /* MD_MSR_STCAL */ /* PRQA S 6030 */ /* MD_MSR_STCYC */ /* PRQA S 6010 */ /* MD_MSR_STPTH */
/* PRQA L:TAG_FblHdr_0715 */

/***********************************************************************************************************************
 *  FblHdrGetPsiRegion
 **********************************************************************************************************************/
/*! \brief       This function will return Psi region information
 *  \param[in]   partId Partition ID (any, including calibration)
 *  \param[out]   pPsiAddr PSI Region start address
 *  \param[out]   pPsiLen PSI Region length
 **********************************************************************************************************************/
void FblHdrGetPsiRegion( vuint8 partId, V_MEMRAM1 IO_PositionType V_MEMRAM2 V_MEMRAM3 * pPsiAddr,
                         V_MEMRAM1 IO_SizeType V_MEMRAM2 V_MEMRAM3 * pPsiLen )
{
   *pPsiAddr = HDR_NO_PP_ADDRESS_DEFINED;
   *pPsiLen = 0u;

   if (PartIdOpSw(partId))
   {
      /* Partition is application */
      ApplFblGetBaseModulePPRegion(partId, pPsiAddr, pPsiLen);
   }
#if defined( FBL_ENABLE_CALIBRATION_MODULES )
   /* Partition is calibration */
   else if (PartIdCal(partId))
   {
       if (kFblOk == FblHdrInitCalPartInfo(partId, &calPartitionInfo))
       {
          *pPsiAddr = calPartitionInfo.psiStart;
          *pPsiLen  = 2*FBL_PP_SEGMENT_SIZE;
       }
   }
   else
   {
      /* Do nothing */
   }
#endif
}

#if defined( FBL_ENABLE_ROM_INTEGRITY_CHECK )
/***********************************************************************************************************************
 *  FblHdrInitFblRomCheck
 **********************************************************************************************************************/
/*! \brief       This function will read the Fbl header segment information to parsedModuleInfo.
 *  \pre         Only allowed to be called during init, not during programming
 *               (parsedModuleInfo structure is reserved then)
 *  \param[out]   segList put the FblHeader configured Region here.
 **********************************************************************************************************************/
void FblHdrInitFblRomCheck( V_MEMRAM1 FL_SegmentListType V_MEMRAM2 V_MEMRAM3 * segList )
{
   vuintx i;
   /* Extract number of address regions */
   segList->nrOfSegments = FBL_GMHEADER_NOAR_FBL;
   for(i = 0u; i < segList->nrOfSegments; i++)
   {
      segList->segmentInfo[i].targetAddress = FblHdrGetIntegerFblHeader(HDR_REGADDR_LEN, FblGetComDat_AddrRegionStartAddr(i));
      segList->segmentInfo[i].length = FblHdrGetIntegerFblHeader(HDR_REGLEN_LEN, FblGetComDat_AddrRegionLength(i));
   }
}
#endif /* FBL_ENABLE_ROM_INTEGRITY_CHECK */

#if defined( FBL_ENABLE_GAP_FILL )
/***********************************************************************************************************************
 *  FblHdrFillGaps
 **********************************************************************************************************************/
/*! \brief       This function guarantees that all memory associated to a module is being filled with
 *               the configured fill pattern.
 *  \return      kFblOk if regions were sucessfully saved.
 *               kFblFailed otherwise
 **********************************************************************************************************************/
tFblResult FblHdrFillGaps( void )
{
   vsint16 i;
   IO_PositionType prPtnAddr;
   IO_SizeType prPtnLen;
   tFblResult retVal = kFblOk;

   tFblBlockPartUsage location;
   tFlashBlock currFlashBlock;
   vuint8 currRegion;
   vuint8 regionNumber;
   FL_SegmentInfoType currSeg;
#if defined( FBL_ENABLE_CALIBRATION_MODULES )
   tFblAddress calModInfoOffset = 0u;
   tFblAddress calModInfoBase = calPartitionInfo.partInfoAddr +
                                (calPartitionInfo.nbrOfRgns * HDR_REGINFO_LEN) + (2u * HDR_NBROF_LEN);

   /* Unused bytes for all calibration files are filled on download of calibration file with lowest ID. */
   if ((MOD_TYPE_CAL == parsedModuleInfo.modType) && (!(IsCalFirstModInPartition())))
   {
      /* Do nothing */
   }
   else
# endif
   {
      for (i = 0u; i < kNrOfFlashBlock; ++i)
      {
         currFlashBlock = FlashBlock[i];
         location = FblHdrPartSegmentListGetBlockStatus(i, parsedModuleInfo.partID);

         if ((HDR_BLOCK_UNUSED_NOT_IN_BASEPARTITION == location) || (HDR_BLOCK_UNUSED_AND_IN_OTHER_PARTITION == location))
         {
            /* Prevent fill */
            currFlashBlock.begin = currFlashBlock.end;
         }
         else if (HDR_BLOCK_UNUSED_AND_IN_BASEPARTITION == location)
         {
#if defined( FBL_ENABLE_CALIBRATION_MODULES )
            if (MOD_TYPE_CAL == parsedModuleInfo.modType)
            {
               /* Prevent fill */
               currFlashBlock.begin = currFlashBlock.end;
            }
            else
#endif
            {
               /* Fill the whole Block. */
            }
         }
         else
         {

            /********************************************************************/
            /* Get the number of regions written within the partition. */
            /********************************************************************/
#if defined( FBL_ENABLE_CALIBRATION_MODULES )
            if (parsedModuleInfo.modType == MOD_TYPE_CAL)
            {
               regionNumber = calPartitionInfo.nbrOfCals;
               calModInfoOffset = 0u;
            }
            else
#endif
            {
               regionNumber = parsedModuleInfo.segmentList.nrOfSegments;
            }

            for (currRegion = 0u; currRegion < regionNumber; currRegion++)
            {
               FblHdrPollingTaskVoid();

               /********************************************************************/
               /* Init currSeg to either next sorted cal module/Appl region        */
               /********************************************************************/
#if defined( FBL_ENABLE_CALIBRATION_MODULES )
               if (parsedModuleInfo.modType == MOD_TYPE_CAL)
               {
                  /* Get current cal file region */
                  calModInfoOffset+=HDR_MID_LEN + HDR_DCID_LEN; /* PRQA S 3353 */ /* MD_FblHdr_3353 */
                  /* Result does not have to be evaluated, it has been previously verified. */
                  (void) FblHdrGetRegionRomAndInc(&currSeg, calModInfoBase, &calModInfoOffset);
               }
               else
#endif
               {
                  FblHdrGetSortedApplRegions(&currSeg, currRegion);
               }

               /********************************************************************/
               /* Check currSeg position relative to Flashblock                    */
               /********************************************************************/
               if (!FBL_UTIL_CHECK_RANGES_EXCLUSIVE_ADDR(currFlashBlock.begin, currFlashBlock.end, currSeg.targetAddress,
                        FBL_UTIL_GET_RANGE_END(currSeg.targetAddress, currSeg.length)))
               {
                  tFblLength currFlashBlockLen = FBL_UTIL_GET_RANGE_LEN(currFlashBlock.begin, currFlashBlock.end);
                  if (1 == FBL_UTIL_CHECK_ADDRESS_IN_RANGE_LEN(currSeg.targetAddress, currFlashBlock.begin, currFlashBlockLen))
                  {
                     /* MemDriver_SegmentSize used below requires correct setting of memSegment */
                     memSegment = FblMemSegmentNrGet(currSeg.targetAddress);

                     /* fill block start to segment Start */
                     if (kFblOk != FblHdrFillRegion(currFlashBlock.begin,
                              FblHdrGetAlignedAddr(currSeg.targetAddress, MemDriver_SegmentSize)-currFlashBlock.begin))
                     {
                        retVal = kFblFailed;
                        break;
                     }
                  }
                  currFlashBlock.begin = FblHdrGetAlignedAddr((currSeg.targetAddress + currSeg.length), MemDriver_SegmentSize);
               }
            } /* for( currRegion ..*/
         } /* (location == HDR_BLOCK_UNUSED_NOT_IN_BASEPARTITION) */

         FblHdrGetPsiRegion(parsedModuleInfo.partID, &prPtnAddr, &prPtnLen);
         if (!FBL_UTIL_CHECK_RANGES_EXCLUSIVE_ADDR(currFlashBlock.begin, currFlashBlock.end, prPtnAddr,
                  FBL_UTIL_GET_RANGE_END(prPtnAddr, prPtnLen)))
         {
            /* Only fill up to Presence Pattern Region (expected to be at end of Flashblock).
             * If there is memory behind pattern, it wont get filled (ilegal configuration).
             */
            assertFblGen((FBL_UTIL_GET_RANGE_END(prPtnAddr, prPtnLen) == currFlashBlock.end), kFblAssertConditionCorrect); /* PRQA S 2214 */ /* MD_FblAssert_Assertion */
            currFlashBlock.end = prPtnAddr - 1u;
         }

         /* Fill remaining block to end */
         if (currFlashBlock.begin < currFlashBlock.end)
         {
            /* MemDriver_SegmentSize used below requires correct setting of memSegment */
            memSegment = FblMemSegmentNrGet(currFlashBlock.begin);

            /* Fill last remaining gap in block ( if applicable, currFlashBlock.begin may be equal to currFlashBlock.end ) */
            if (kFblOk != FblHdrFillRegion(currFlashBlock.begin, FBL_UTIL_GET_RANGE_LEN(currFlashBlock.begin, currFlashBlock.end)))
            {
               retVal = kFblFailed;
               break;
            }
         }
      } /* for (i = 0; i < kNrOfFlashBlock .. */
   }
   return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */ /* PRQA S 6050 */ /* MD_MSR_STCAL */ /* PRQA S 6030 */ /* MD_MSR_STCYC */ /* PRQA S 6010 */ /* MD_MSR_STPTH */
#endif /* FBL_ENABLE_GAP_FILL */

#if defined( FBL_ENABLE_CALIBRATION_MODULES )
/***********************************************************************************************************************
 *  FblHdrGetCalPartitionValid
 **********************************************************************************************************************/
/*! \brief       This function will check if all calibration partitions of an operating software are valid
 *  \pre         Operating software must be valid
 *  \param[in]   opSwPartId Operating software partition ID
 *  \return      kFblOk = all calibration partitions are valid; kFblFailed = at least 1 calibration partition is invalid
 **********************************************************************************************************************/
tFblResult FblHdrGetCalPartitionValid( vuint8 opSwPartId )
{
   vuint8 partCounter = 0u;
   vuint8 retVal = kFblOk;
   vuint8 numbPartitions;

#if defined( FBL_ENABLE_CALIBRATION_MODULES )
   assertFblInternal((opSwPartId == FblHdrGetBasePartId(opSwPartId)), kFblAssertInternalIlegalState); /* PRQA S 2214 */ /* MD_FblAssert_Assertion */
#endif

   /* Initialize first partition and get the number of partitions */
   numbPartitions = FblHdrGetNoOfCalPartitions(opSwPartId);

   /* Loop through all partitions */
   while ((partCounter < numbPartitions) && (kFblOk == retVal))
   {
      FblHdrPollingTaskVoid();

     /* Check if current partition is valid */
      if (PSI_PART_PRESENT != ApplFblChkPSIState(FblHdrGetFirstCalPartNbr(opSwPartId) + partCounter))
      {
         retVal = kFblFailed;
      }

      partCounter++;
   }

   /* Reset calPartitionInfo.partID; This is usefull if Invalid Application Presence pattern is found
    * in memory and calPartion Info is ilegally initialized. This would lead to errorness state during development.
    */
   calPartitionInfo.partID = 0u;

   return retVal;
}

/***********************************************************************************************************************
 *  FblHdrGetNoOfCalPartitions
 **********************************************************************************************************************/
/*! \brief       This function will return the number of calibration partitions available for a specific operating
 *               software
 *  \pre         Operating software must be valid
 *  \param[in]   opSwPartId Operating software partition ID
 *  \return      Number of calibration partitions
 **********************************************************************************************************************/
vuint8 FblHdrGetNoOfCalPartitions( vuint8 opSwPartId )
{
   vuint8 numbPartitions;
   /* No overflow possible as opSwPartId will never be 0xFFu */
   vuint8 calPart1 = (vuint8)(opSwPartId + 1u);

   if (kFblOk == FblHdrInitCalPartInfo(calPart1, &calPartitionInfo))
   {
      numbPartitions = calPartitionInfo.nbrOfPrts;
   }
   else
   {
      numbPartitions = 0u;
   }

   return numbPartitions;
}
#endif /* FBL_ENABLE_CALIBRATION_MODULES */

/***********************************************************************************************************************
 *  FblHdrRequestDownload
 **********************************************************************************************************************/
/*! \brief
 *  \param[in]   partId The ID of the partition to be erased
 *  \return      kFblOk=Success; kFblFailed=Failure
 **********************************************************************************************************************/
tFblResult FblHdrRequestDownload( tDownloadSource downloadSource )
{
   tFblResult retVal;
#if defined( FBL_HDR_ENABLE_PASSTHROUGH ) && defined( FBL_HDR_ENABLE_EXPLICIT_ERASE )
   tMcmpMemResult mcmpRetVal;
#endif

#if defined( FBL_HDR_ENABLE_EXPLICIT_ERASE )
# if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   (void)downloadSource; /* PRQA S 3112 */ /* MD_MSR_14.2 */
# endif
#else
   retVal = FblHdrInitRetry(downloadSource);
   if (kFblOk == retVal)
#endif /* FBL_HDR_ENABLE_EXPLICIT_ERASE */
   {
#if defined( FBL_HDR_ENABLE_EXPLICIT_ERASE )
# if defined( FBL_HDR_ENABLE_PASSTHROUGH )
      if (kFblOk == ApplFblHdrIsPassThroughActive(parsedModuleInfo, &isNewConnection))
      {
         /* Pass request to configured routine */
         mcmpRetVal = ApplFblHdrRequestDownload(HDR_DOWNLOAD_SOURCE_REMOTE, &retVal, &errPecExtErrorCode);
         if (kMcmpMemResult_Ok != mcmpRetVal)
         {
            FblSetPecErr(HdrErr_Undefined); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
            retVal = kFblFailed;
         }
      }
      else
# endif /* FBL_HDR_ENABLE_PASSTHROUGH */
      {
         if (FblHdrStateIsIdle())
         {
            FblSetPecErr(HdrErr_ExtMemoryNotErased); /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
            retVal = kFblFailed;
         }
         else if (!FblHdrStateIsDownloadExpected())
         {
            FblSetPecErr(HdrErr_ExtRequestDownloadMsgOutOfSequence);  /* PRQA S 3109 */ /* MD_CBD_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
            retVal = kFblFailed;
         }
         else
         {
            retVal = kFblOk;
         }
      }

      if (kFblOk != retVal)
      {
         FblHdrStateSetIdle(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
      }
      else
      {
         FblHdrStateSetTransferHdrExpected(); /* PRQA S 3109 */ /* MD_CBD_14.3 */
      }
#else
      FblHdrStateSetTransferHdrExpected(); /* PRQA S 3109 */ /* MD_CBD_14.3 */
#endif /* FBL_HDR_ENABLE_EXPLICIT_ERASE */
   }

   return retVal;
}

/***********************************************************************************************************************
 *  FblHdrEraseRoutine
 **********************************************************************************************************************/
/*! \brief       Invalidates the module and erases all blocks defined by the FlashBlock
 *               table that are occupied by the module being downloaded.
 *  \details     This routine will only do its job on programmable Modules.
 *               It checks the parsedModuleInfo addresses against FlashBlock
 *               table, and also checks if the corresponding module is allowed to erase a block.
 *  \param[in]   partId The ID of the partition to be erased
 *  \return      kFblOk=Success; kFblFailed=Failure
 **********************************************************************************************************************/
#if defined( FBL_HDR_ENABLE_EXPLICIT_ERASE )
tFblResult FblHdrEraseRoutine( vuint8 partId, tDownloadSource downloadSource )
#else
static tFblResult FblHdrEraseRoutine( vuint8 partId, tDownloadSource downloadSource )
#endif
{
   tFblResult retVal;
#if defined( FBL_HDR_ENABLE_PASSTHROUGH ) && defined( FBL_HDR_ENABLE_EXPLICIT_ERASE )
   tMcmpMemResult mcmpRetVal;
#endif

#if defined( FBL_HDR_ENABLE_EXPLICIT_ERASE )
   retVal = FblHdrInitRetry(downloadSource);
   if (kFblOk == retVal)
#else
# if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   (void)downloadSource; /* PRQA S 3112 */ /* MD_MSR_14.2 */
# endif
#endif
   {
      parsedModuleInfo.partID = partId;
      parsedModuleInfo.blockNbr = GetBlockNbrFromPartId(partId);

#if defined( FBL_HDR_ENABLE_PASSTHROUGH ) && defined( FBL_HDR_ENABLE_EXPLICIT_ERASE )
      if (kFblOk == ApplFblHdrIsPassThroughActive(parsedModuleInfo, &isNewConnection))
      {
         if (kFblOk == isNewConnection)
         {
            /* Initialize remote header module after slave address information is available */
            mcmpRetVal = ApplFblHdrInit((vuint8)requSequType);
            if (kMcmpMemResult_Ok != mcmpRetVal)
            {
               FblSetPecErr(HdrErr_ExtNrcConditionsNotCorrect); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
               retVal = kFblFailed;
            }
         }

         if (kFblOk == retVal)
         {
            /* Pass request to configured routine */
            mcmpRetVal = ApplFblHdrEraseRoutine(parsedModuleInfo.partID, downloadSource, &retVal, &errPecExtErrorCode);
            if (kMcmpMemResult_Ok != mcmpRetVal)
            {
               FblSetPecErr(HdrErr_Undefined); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
               retVal = kFblFailed;
            }
         }
      }
      else
#endif
      {
         retVal = kFblFailed;

         if (PartIdOpSw(partId))
         {
#if ( FBL_MTAB_NO_OF_BLOCKS > 1 )
            if (HDR_UNDEFINED_LOG_BLOCK == parsedModuleInfo.blockNbr)
            {
               FblSetPecErr(HdrErr_PartitionId); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
            }
            else
#endif
            {
               /* Erase entire logical block for corresponding application */
               blockInfo.targetAddress = FblLogicalBlockTable.logicalBlock[parsedModuleInfo.blockNbr].blockStartAddress;
               blockInfo.targetLength = FblLogicalBlockTable.logicalBlock[parsedModuleInfo.blockNbr].blockLength;

               /* In case of application, only application PSI information is revoked.
                * Always invalidate block, also if locErasedState below indicates block is erased.
                */
               if (kFblOk != ApplFblInvalidateBlock(*FblGetBlockDescriptor(partId)))
               {
                  FblSetPecErr(HdrErr_Revoke); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
               }
               else if (kFlashErased == FblHdrErasedStateVar(parsedModuleInfo.blockNbr, 0u))
               {
                  /* No erase needed. Note: In this state the calibration partitions are always erased, too. */
                  retVal = kFblOk;
               }
               else if (kFblMemStatus_Ok != FblMemBlockEraseIndication(&blockInfo))
               {
                  FblSetPecErr(HdrErr_EraseSW); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
               }
               else
               {
                  vuint8 partNbr = 0u;

                  /* Set Application partition and all associated cal partitions (if applicable) to erased state */
#if defined( FBL_ENABLE_CALIBRATION_MODULES )
                  for(;partNbr < HDR_MAX_TRACKED_ERASED_STATE_PARTIONS; partNbr++)
#endif
                  {
                     FblHdrErasedStateVar(parsedModuleInfo.blockNbr, partNbr) = kFlashErased;
                  }
                  retVal = kFblOk;
               }
            }
         }
#if defined( FBL_ENABLE_CALIBRATION_MODULES )
         else if (PartIdCal(partId))
         {
            /* Pec error set internally */
            retVal = FblHdrEraseCalPartition(partId);
         }
#endif
         else
         {
            FblSetPecErr(HdrErr_PartitionId); /* PRQA S 3109 */ /* MD_MSR_14.3 */ /* PRQA S 0310 */ /* MD_FblHdr_0310 */
         }
      }

      if (kFblOk != retVal)
      {
         FblHdrStateSetIdle(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
      }
      else
      {
#if defined( FBL_HDR_ENABLE_EXPLICIT_ERASE )
         FblHdrStateSetDownloadExpected(); /* PRQA S 3109 */ /* MD_MSR_14.3 */
#else
         /* Keep current state */
#endif
      }
   }

   return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */ /* PRQA S 6030 */ /* MD_MSR_STCYC */ /* PRQA S 6050 */ /* MD_MSR_STCAL */

/***********************************************************************************************************************
 *  FblCalculateCsum16Bit
 **********************************************************************************************************************/
/*! \brief       Create 16-Bit checksum over one RAM buffer
 *  \details     Words are treated as 16-bit according to the native CPU-type (little/big endian)
 *  \param[in]   buffer pointer to data on which checksum is to be calculated
 *  \param[in]   len Size of data for checksum calculation in bytes
 *  \param[in]   csum Start value of the checksum
 *  \return      Calculated checksum
 **********************************************************************************************************************/
vuint16 FblCalculateCsum16Bit( const V_MEMRAM1 vuint16 V_MEMRAM2 V_MEMRAM3 * buffer, vuint32 len, vuint16 csum )
{
   vuint32 i = 0u;

   if (0x01u == ((vuintx)buffer & 0x01u)) /* PRQA S 0306 */ /* MD_FblHdr_0306 */
   {
      /* Avoid unaligned data access */
      csum = 0u;
   }
   else
   {
      /* Prevent endless loop when length is odd */
      while (len > 1u)
      {
         csum += buffer[i];
         i++;
         len -= 2u;

         FblHdrPollingTaskInLoop(len, 0x40u);  /* PRQA S 3109 */ /* MD_MSR_14.3 */
      }
   }
   return csum;
}

#if defined( FBL_ENABLE_VERIFY_INTEGRITY_WORD )
/***********************************************************************************************************************
 *  FblHdrCheckDldIntegrityWord
 **********************************************************************************************************************/
/*! \brief       Verify download data against Plain Header Integrity word.
 *  \details     This is done additionally to the hash verification inside FblHdrPostFlashValidation.
 *               Plain Header Integrity word might be used later to verify the application correctness.
 *  \return      kFblOk=Success; kFblFailed=Failure
 **********************************************************************************************************************/
SecM_StatusType FblHdrCheckDldIntegrityWord( V_MEMRAM1 SecM_SignatureParamType V_MEMRAM2 V_MEMRAM3 * pVerifyParam ) /* PRQA S 3673 */ /* MD_FblHdr_3673 */
{
   SecM_StatusType retVal = SECM_VER_CRC;
   static vuint16 checksum;

      switch (pVerifyParam->sigState)
      {
         case kHashInit:
         {
            /* Init workSpace */
            checksum = 0x0000u;
            retVal = SECM_VER_OK;
            break;
         }
         case kHashCompute:
         {
            /* Segment list from download can be reused, it is checked to be identical to programmed */
            if (kFblOk == ApplFblUpdateChecksum(&checksum, pVerifyParam->sigByteCount, pVerifyParam->sigSourceBuffer))
            {
               retVal = SECM_VER_OK;
            }
            break;
         }
         case kHashFinalize:
         {
            /* Do nothing */
            retVal = SECM_VER_OK;
            break;
         }
         case kSigVerify:
         {
            if (kFblOk == ApplFblFinalizeChecksum(&checksum))
            {
               retVal = SECM_VER_OK;
            }
            break;
         }
         default:
         {
            break;
         }
      }

   return retVal;
}
#endif

/* module specific MISRA deviations:

   MD_FblHdr_3382
     Reason: Check for overflow of modHdrAddr + locParseIdx is not necessary, a configuration using such a critical value would be catched early
             when trying to download such an application.
     Risk: No risk.
     Prevention: No prvention.

   MD_FblHdr_3112:
     Reason: Dummy statements have been added to avoid compiler warnings because of unused parameters. Unused parameters may exist due to its usage in
             a different configured use case.
     Risk: Unused parameters add to program complexity just by being there.
     Prevention: Dummy statements can be turned off externally (V_DISABLE_USE_DUMMY_STATEMENT).

   MD_FblHdr_3321:
     Reason: prPtn.length is not unset. It is set in the call to FblHdrGetPsiRegion above.
     Risk: No risk
     Prevention: No prevention

   MD_FblHdr_3353:
     Reason: calModInfoOffset is not unset. It is set and used only if (parsedModuleInfo.modType == MOD_TYPE_CAL). In the case (parsedModuleInfo.modType != MOD_TYPE_CAL)
             it is neither set nor used.
     Risk: No risk
     Prevention: No prevention

   MD_FblHdr_3673:
     Reason: Standard Api that cannot be changed.
     Risk: No risk
     Prevention: No prevention

   MD_FblHdr_3415:
     Reason: The intended behavior is: Call the function only for Application modules.
     Risk: No risk
     Prevention: No prevention

   MD_FblHdr_3415_2:
     Reason: The intended behavior is: Call the Data processing function after Init functions.
             Leave if any of these fail.
     Risk: No risk
     Prevention: No prevention

   MD_FblHdr_0310:
     Reason: This cast is used intentionally.
     Risk: No risk, only used in Development feature
     Prevention: No prevention

   MD_FblHdr_0311
     Reason: Data processing interface does not change pointer, though its data type does allow changes.
     Risk: No risk
     Prevention: No prevention

   MD_FblHdr_0306:
     Reason: Address conversion between integer values and pointers is required to allow for hardware independent configuration and address range checks.
     Risk: The size of integer required to hold the result of a pointer cast is implementation-defined.
     Prevention: The size of the respective integer data type which holds the address value is adapted on a hardware specific basis.

   MD_FblHdr_3201:
     Reason: This cast is used intentionally.
     Risk: No risk, only used in Development feature
     Prevention: No prevention

   MD_FblHdr_3355_58:
     Reason: The macro is called from different places. At some places the check is required.
     Risk: No risk
     Prevention: No prevention

   MD_FblHdr_3325_3201:
     Reason: The macro can be mapped to variable in certain cases, therefore a preprocessor check is not possible.
     Risk: No risk.
     Prevention: No prevention.

   MD_FblHdr_3218:
     Reason: The variable is only used at one location for one of several module configuration options.
     Risk: Scope is larger than required (whole file instead of one function). Some other function could access
      the variable.
     Prevention: No prevention

*/

/***********************************************************************************************************************
 *  END OF FILE: FBL_HDR.C
 **********************************************************************************************************************/

