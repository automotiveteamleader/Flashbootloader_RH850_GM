/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file          fbl_hdr.h
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
 *  01.00.00   2014-01-31  AWh                     First release
 *  01.01.00   2014-03-04  AWh     ESCAN00073546   No changes
 *                                 ESCAN00073567   Change to LibMem 2.xx interface; Minor Improvements
 *                                 ESCAN00073883   No changes
 *                                 ESCAN00073910   No changes
 *                         Dod     ESCAN00074067   No changes
 *  01.02.00   2014-07-14  Dod     ESCAN00075058   No changes
 *                         AWh     ESCAN00075272   GapFill need to use at least FLASH_SEGMENT_SIZE buffer size
 *                                 ESCAN00075353   No changes
 *                                 ESCAN00075678   No changes
 *                                 ESCAN00075685   No changes
 *                                 ESCAN00076569   SLP6: Download with more than one calibration in partition fails
 *                                 ESCAN00076911   Sba ticket is stored and read inclusive the data type
 *                                 ESCAN00077055   No changes
 *  01.03.00   2014-09-11  HRf     ESCAN00078350   Add cast to second parameter when FblMemGetInteger is called in
 *                                                 the context of FblHdrInitFblRomCheck
 *  01.04.00   2014-11-05  AWh     ESCAN00078423   HDR_GAPFILL_BUFF_SIZE is not defined
 *                         Vna     ESCAN00078603   Padding added to tagBlockRegion structure in order for proper byte
 *                                                 alignment. Aligned to 2 bytes for 16-bit and 4 byte for 32-bit micros
 *                         AWh     ESCAN00078852   No changes
 *                                 ESCAN00078855   No changes
 *                                 ESCAN00079183   No changes
 *                                 ESCAN00079328   SLP5: Order of calibration partition members is not checked
 *  02.00.00   2015-01-15  AWh     ESCAN00079553   Add Compression Interface
 *  02.00.01   2015-01-16  AWh     ESCAN00080628   Allow overwriting Data compression type
 *  02.01.00   2015-02-09  AWh     ESCAN00081143   Programming failure on transfer data
 *  02.02.00   2015-03-12  AWh     ESCAN00080365   Default FBL_HDR_MAX_S1_FILL_LEN to a value of 2
 *             2015-03-12  AWh     ESCAN00081761   No changes
 *                         AWh     ESCAN00081762   No changes
 *  02.03.00   2015-03-16  AWh     ESCAN00081831   No changes
 *                                 ESCAN00081857   No changes
 *  02.04.00   2015-06-10  AWh     ESCAN00083032   No changes
 *                                 ESCAN00083040   No changes
 *                                 ESCAN00083073   Restrict valid calibration partition Ids
 *  02.04.01   2015-06-12  AWh     ESCAN00083383   Remove superfluous Rcrrp sometimes seen on service 36 on the bus
 *  02.05.00   2015-07-23  AWh     ESCAN00083538   Check and set partition erased state.
 *                                 ESCAN00083750   No changes
 *                                 ESCAN00083822   No changes
 *                                 ESCAN00083993   No changes
 *  02.06.00   2016-02-19  AWh     ESCAN00088277   Added check to verify SEC_ENABLE_WORKSPACE_INTERNAL is enabled
 *                                 ESCAN00088419   Add basic NBID handling
 *                                 ESCAN00088430   Address precision lost and conversion in argument warnings
 *  02.06.01   2016-03-18  AWh     ESCAN00088818   No changes
 *                                 ESCAN00088973   No changes
 *                                 ESCAN00088984   No changes
 *  02.07.00   2016-03-17  Shs     ESCAN00088954   Added support for PATAC SLP2
 *  02.07.01   2016-04-04  ThM     ESCAN00089202   Fixed direct access problem to rom section
 *                         AWh     ESCAN00089235   No changes
 *  03.00.00   2016-04-08  AWh     ESCAN00089528   Complete rework to allow usage for extended use cases.
 *                                 ESCAN00089532   No changes
 *  03.01.00   2016-08-04  HRf     ESCAN00091043   No changes
 *                         AWh     ESCAN00091147   No changes
 *                                 ESCAN00091325   Allow Mcmp config with several slaves for non explicit erase use case
 *                                 ESCAN00091279   No changes
 *                                 ESCAN00089426   No changes
 *  03.01.01   2016-10-04  AWh     ESCAN00092167   No changes
 *  03.02.00   2016-10-31  ThM     ESCAN00092596   Added support for Lzma decompression
 **********************************************************************************************************************/

#ifndef __FBL_HDR_H__
#define __FBL_HDR_H__

/* PRQA S 0828 TAG_FblHdr_828 */ /* MD_MSR_1.1_828 */

/***********************************************************************************************************************
 *  VERSION
 **********************************************************************************************************************/

/* ##V_CFG_MANAGEMENT ##CQProject : FblDiag_SecHdr_Gm CQComponent : Implementation */
#define FBLDIAGHDR_GM_VERSION           0x0302u
#define FBLDIAGHDR_GM_RELEASE_VERSION   0x00u

/* Defines not available in Slave project through missing FblDiag */
#define FBL_VERSION                      _VECTOR_SIP_VERSION
#define FBL_RELEASE_VERSION              _VECTOR_SIP_RELEASE_VERSION

#define FBLOEM_GM_VERSION                FBL_VERSION
#define FBLOEM_GM_RELEASE_VERSION        FBL_RELEASE_VERSION

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

#include "fbl_apfb.h"

/* Default to FBL_HDR_ENABLE_BASIC_NVM_HANDLING, FBL_DISABLE_WRAPPER_NV
 * => Both need to be set before include of fbl_nbid.h
 */
#if !defined( FBL_ENABLE_WRAPPER_NV ) && !defined( FBL_DISABLE_WRAPPER_NV )
# define FBL_DISABLE_WRAPPER_NV
#endif
#if !defined( FBL_HDR_ENABLE_BASIC_NVM_HANDLING ) && !defined( FBL_HDR_DISABLE_BASIC_NVM_HANDLING )
# define FBL_HDR_ENABLE_BASIC_NVM_HANDLING
#endif
#if defined( FBL_HDR_ENABLE_BASIC_NVM_HANDLING )
# include "fbl_nbid.h"
#endif

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

# define FBL_HDR_ENABLE_EXPLICIT_ERASE

# define FBL_HDR_ENABLE_EXPLICIT_TRANSFEREND

# define FBL_HDR_ENABLE_EXPLICIT_UPDATEPSI

#if !defined( FBL_HDR_ENABLE_MULTIPLE_DOWNLOAD_SOURCES ) && !defined( FBL_HDR_DISABLE_MULTIPLE_DOWNLOAD_SOURCES )
# define FBL_HDR_DISABLE_MULTIPLE_DOWNLOAD_SOURCES
#endif

# if defined( MCMP_MODE_SLAVE ) && !defined( FBL_HDR_DISABLE_DEBUG_STATUS_STUBS )
/* Currently Debug status is not supported on slave */
# define FblErrStatSetError(error)
# define FblErrStatSetSId(id)
# define FblErrStatSetFlashDrvError(error)
# define FblErrDebugStatus(error)
# define FblErrDebugDriver(addr, error)
#endif

/* PRQA S 857 TAG_FblHdr_857 */ /* MD_MSR_1.1 */

/***********************************************************************************************************************
 *  DEFINES: PassThrough
 **********************************************************************************************************************/

/* Allow overwriting polling function, set to use case typical defaults */
#if !defined( FblHdrPollingTask )
/* Overwrite to FblHdrPollingTask on slave */
# if defined( MCMP_MODE_SLAVE )
#  define FblHdrPollingTask() FblLookForWatchdog()
#  define FblHdrGetPollingTask() FblLookForWatchdog
# else
#  define FblHdrPollingTask() FblRealTimeSupport()
#  define FblHdrGetPollingTask() FblRealTimeSupport
# endif /* MCMP_MODE_SLAVE */
#endif /* FblHdrPollingTask */

/* Allow overwriting Debug status macros, set to use case typical defaults */
#if !defined( FblHdrDebugStatus )
# if defined( MCMP_MODE_SLAVE )
#  define FblHdrDebugStatus(err)
# else
#  define FblHdrDebugStatus(err) FblErrDebugStatus(err) /* PRQA S 3453 */ /* MD_MSR_19.7 */
# endif
#endif
#if !defined( FblHdrStatSetError )
# if defined( MCMP_MODE_SLAVE )
#  define FblHdrStatSetError(err)
# else
#  define FblHdrStatSetError(err) FblErrStatSetError(err) /* PRQA S 3453 */ /* MD_MSR_19.7 */
# endif
#endif

/***********************************************************************************************************************
 *  DEFINES: Data Types
 **********************************************************************************************************************/

#define DATA_TYPE_PLAIN_V1 0x0101u
#define DATA_TYPE_PROTECTED 0x0200u
#define DATA_TYPE_SIGNED_V1 0x0301u
#define DATA_TYPE_COMPRESSED_V1 0x0401u
#define DATA_TYPE_COMPRESSED_V2 0x0402u
#define DATA_TYPE_ENCRYPTED 0x0500u
#define DATA_TYPE_DIFFERENCE 0x0600u

/***********************************************************************************************************************
 *  DEFINES: Header field length
 **********************************************************************************************************************/

#if !defined( FBL_HDR_MAX_S1_FILL_LEN )
/** The LibMem requires an alignment of the SignedHeader inclusive the Plain Header data type of 4.
  * To achieve this a 2 byte fill is required. */
# define FBL_HDR_MAX_S1_FILL_LEN 2u
#endif

#define HDR_DATATYPE_LEN 2u /**< Data type field length */
#define HDR_NBROF_LEN 2u /**< Length for fields depicting the number of entries, regions, partitions, cal modules */
#define HDR_MID_LEN 2u /**< Module Id field length, used in several headers */
#define HDR_DCID_LEN 2u /**< Data Compatibility Id field length, used in several headers */
#define HDR_NBID_LEN 2u /**<  Not Before Id field length, used in several headers (key, application) */
#define HDR_ECUNAME_LEN 8u /**< Ecu name field length */
#define HDR_ECUID_LEN 16u /**< Ecu Id field length */

#define HDR_REGADDR_LEN 4u /**< Region Info address field length */
#define HDR_REGLEN_LEN 4u /**< Region Info length field length */

#define HDR_MSGDIGEST_LEN 32u /**< Message Digest field length */
#define HDR_SIGNATURE_LEN 256u /**< Signature field length (both root/signer signature) */
#define HDR_SUBJNAME_LEN 16u /**< Subject name field length */
#define HDR_CERTID_LEN 8u /**< Certificate Id field length ( the field itself is not used in the bootloader ) */
#define HDR_SIGNPKEY_LEN 256u /**< Public key field length */

#define HDR_CS_LEN 2u /**< Integrity Word == Checksum field length */
#define HDR_CFGOPT_LEN 2u /**< Configuration options field length */
#define HDR_DLS_LEN 2u /**< Design Level Suffix field length */
#define HDR_HEXPN_LEN 4u /**< Hex Part Number field length */
#define HDR_ASCIIPN_LEN 16u /**< Ascii Part Number field length */

/* Total length of data to store one region - address field + length field */
#define HDR_REGINFO_LEN (HDR_REGADDR_LEN + HDR_REGLEN_LEN)
/* Length of data that each calibration requires within the application header */
#define HDR_CALINFO_LEN (HDR_MID_LEN + HDR_DCID_LEN + HDR_REGINFO_LEN)
/* Length of the entire signed header */
#define HDR_SIGINFO_LEN (HDR_SUBJNAME_LEN + HDR_CERTID_LEN + HDR_NBROF_LEN + HDR_SIGNPKEY_LEN + HDR_SIGNATURE_LEN)

#define HDR_CMPR_DATALEN_LEN 6u /**< Compressed data length field length */

/* Length of the entire SBA ticket */
#define HDR_SBA_TICKET_LEN (HDR_DATATYPE_LEN + HDR_MID_LEN + HDR_ECUNAME_LEN + HDR_ECUID_LEN + HDR_SIGINFO_LEN + HDR_SIGNATURE_LEN)
#define HDR_SW_LOCATION_LEN (HDR_NBROF_LEN + (SWM_DATA_MAX_NOAR * HDR_REGINFO_LEN))
#define HDR_FBL_SIGNED_END_LEN (FBL_HDR_MAX_S1_FILL_LEN + HDR_DATATYPE_LEN)
/* Maximum possible length of RAM parsed part of a downloaded module (most RAM is required for an application module)
 * This is the Signed Header length + fill pattern length + PLAIN_V1 Data Type */
#define HDR_MODULE_MAX_RAW_LEN (HDR_SBA_TICKET_LEN + HDR_DCID_LEN + HDR_NBID_LEN + HDR_SW_LOCATION_LEN + HDR_MSGDIGEST_LEN + HDR_FBL_SIGNED_END_LEN)

#if defined ( FBL_ENABLE_COMPRESSION_MODE )
# if !defined( HDR_MODULE_MAX_POSS_COMPR_GROWTH )
/* Default consideration: Maximum possible growth when compression is applied is by 1/5 */
#  define HDR_MODULE_MAX_POSS_COMPR_GROWTH  (((HDR_MODULE_MAX_RAW_LEN - 1u) / 5u) + 1u)
# endif  /* HDR_MODULE_MAX_POSS_COMPR_GROWTH */
# define HDR_MODULE_MAX_LEN (HDR_MODULE_MAX_RAW_LEN + HDR_MODULE_MAX_POSS_COMPR_GROWTH)
#else
# define HDR_MODULE_MAX_LEN HDR_MODULE_MAX_RAW_LEN
#endif /* FBL_ENABLE_COMPRESSION_MODE */

/***********************************************************************************************************************
 *  DEFINES: Header field indices
 **********************************************************************************************************************/

/***** Generic data type index (always at the very start) ******/
#define HDR_DATYPE_IDX 0u /**< Datatype is envelop start field, in front of any header */

/***** Field static indexes in Signed Header Info  V1 ******/
/** Data Type field  */
#define S1_MID_IDX (HDR_DATYPE_IDX + HDR_DATATYPE_LEN) /**< Module-ID field index */
#define S1_DCID_IDX (S1_MID_IDX + HDR_MID_LEN) /**< Data-Compatibility-Identifier field index */
#define S1_ECUNAME_IDX (S1_DCID_IDX + HDR_DCID_LEN) /**< Ecu name App/Cal field index */
#define S1_ECUID_IDX (S1_ECUNAME_IDX + HDR_ECUNAME_LEN) /** Ecu Id field index */
#define S1_APP_NBID_IDX (S1_ECUID_IDX + HDR_ECUID_LEN) /**< App NBID field index */
#define S1_APP_SWLINFO_IDX (S1_APP_NBID_IDX + HDR_NBID_LEN) /**< App Sw Location Info struct index */

/* No static App indexes after Sw Location Info, further indexes are dynamic */
#define S1_CAL_MSGDIGEST_IDX (S1_ECUID_IDX + HDR_ECUID_LEN) /** Cal Message Digest index */
#define S1_CAL_SIGINFO_IDX (S1_CAL_MSGDIGEST_IDX + HDR_MSGDIGEST_LEN) /** Cal Signer Info index */
#define S1_CAL_SIGNATURE_IDX (S1_CAL_SIGINFO_IDX + HDR_SIGINFO_LEN) /** Cal Signature index */

/* For SBA-ticket different Indices have to be used (no starting HDR_DATYPE_IDX ) */
#define S1_SBA_MID_IDX S1_MID_IDX /**< SBA ticket Module-ID field index */
#define S1_SBA_ECUNAME_IDX (S1_SBA_MID_IDX + HDR_MID_LEN) /**< SBA ticket Ecu name index */
#define S1_SBA_SIGNINFO_IDX (S1_SBA_ECUNAME_IDX + (HDR_ECUNAME_LEN+HDR_ECUID_LEN)) /**< SBA ticket Signer Info index */
#define S1_SBA_SIGNATURE_IDX (S1_SBA_SIGNINFO_IDX + HDR_SIGINFO_LEN) /**< SBA ticket Signature index */

/***** Field static indexes in Signer Info ******/
#define SIGNINFO_SUBJNAME_IDX  0u /**< Signer Info Subject Name index */
#define SIGNINFO_CERTID_IDX (SIGNINFO_SUBJNAME_IDX + HDR_SUBJNAME_LEN) /**< Signer Info Certificate ID index */
#define SIGNINFO_KEYNBID_IDX (SIGNINFO_CERTID_IDX + HDR_CERTID_LEN) /**< Signer Info Key-NBID index */
#define SIGNINFO_SIGNPKEY_IDX (SIGNINFO_KEYNBID_IDX + HDR_NBID_LEN) /**< Signer Info Signer Public Key index */
#define SIGNINFO_ROOTSIGN_IDX (SIGNINFO_SIGNPKEY_IDX + HDR_SIGNPKEY_LEN) /**< Signer Info Root Signature index */

/***** Field static indexes in Plain Header V1 (programmed) ******/
#define P1_CS_IDX  0u /**< Integrity word field  */
#define P1_CFGOPT_IDX  (P1_CS_IDX + HDR_CS_LEN) /**< Configuration Options */
#define P1_MID_IDX  (P1_CFGOPT_IDX + HDR_CFGOPT_LEN) /**< Configuration Options */
#define P1_DCID_IDX (P1_MID_IDX + HDR_MID_LEN) /**< Data-Compatibility-Identifier field */
#define P1_APP_NBID_IDX (P1_DCID_IDX + HDR_DCID_LEN) /**< App NBID */
#define P1_APP_DLS_IDX (P1_APP_NBID_IDX + HDR_NBID_LEN) /**< DLS Appl behind App NBID */
#define P1_APP_HEXPN_IDX (P1_APP_DLS_IDX + HDR_DLS_LEN) /**< Hex Part number Appl */
#define P1_APP_ASCIIPN_IDX (P1_APP_HEXPN_IDX + HDR_HEXPN_LEN) /**< Asci Part number Appl */

#define P1_CAL_DLS_IDX (P1_DCID_IDX + HDR_DCID_LEN) /**< DLS Cal behind DCID */
#define P1_CAL_HEXPN_IDX (P1_CAL_DLS_IDX + HDR_DLS_LEN) /**< Hex Part number Cal */
#define P1_CAL_ASCIIPN_IDX (P1_CAL_HEXPN_IDX + HDR_HEXPN_LEN) /**< Asci Part number Cal */
/* No static App indexes after Ascii Pn, further indexes are dynamic */

/***** Field static indexes in Compressed Header ARLE/LZMA  ******/
#define CMPR_LEN_IDX 2u /**< Compressed data length field index, after data type of 2 bytes */
#define CMPR_LEN_USED_IDX 4u /**< Compressed data length field index of really used part, after 2 bytes unused */

/***********************************************************************************************************************
 *  DEFINES: Module Type handling
 **********************************************************************************************************************/

#define MOD_TYPE_APPL 0u
#define MOD_TYPE_CAL 1u
#define MOD_TYPE_RESERVED 2u
#define MOD_TYPE_NOTSUPPORTED 3u

#define IsCalDownLd() (MOD_TYPE_CAL == parsedModuleInfo.modType)

/***********************************************************************************************************************
 *  DEFINES: Module Id handling
 **********************************************************************************************************************/

#define MIN_MODULE_ID ((vuint8)1u)
#define MAX_MODULE_ID ((vuint8)99u)

/** Macros for evaluating type of module-id */
#define MODID_FBL_1 ((vuint16) 71u)
#define MODID_FBL_LAST ((vuint16) 80u)
#define MODID_SBA ((vuint16) 91u)

#define MODID_OPSW_1 ((vuint16) 1u)
#define MODID_OPSW_2 ((vuint16) 21u)
#define MODID_OPSW_3 ((vuint16) 31u)
#define MODID_OPSW_4 ((vuint16) 41u)

#if ( FBL_MTAB_NO_OF_BLOCKS > 1 )
# define START_CALID_OPSW_1 (MODID_OPSW_1 + 1u)
# define START_CALID_OPSW_2 (MODID_OPSW_2 + 1u)
# define START_CALID_OPSW_3 (MODID_OPSW_3 + 1u)
# define START_CALID_OPSW_4 (MODID_OPSW_4 + 1u)

# define END_CALID_OPSW_1 (MODID_OPSW_1 + 19u)
# define END_CALID_OPSW_2 (MODID_OPSW_2 + 9u)
# define END_CALID_OPSW_3 (MODID_OPSW_3 + 9u)
# define END_CALID_OPSW_4 (MODID_OPSW_4 + 9u)

#define IsIdOfOpsw1(id) (MODID_OPSW_1 == (id) )  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define IsIdOfOpsw2(id) (MODID_OPSW_2 == (id) )  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define IsIdOfOpsw3(id) (MODID_OPSW_3 == (id) )  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define IsIdOfOpsw4(id) (MODID_OPSW_4 == (id) )  /* PRQA S 3453 */ /* MD_MSR_19.7 */

# define IsIdOfCalForOpsw1(id)  ( (((id) >= START_CALID_OPSW_1) && ((id) <= END_CALID_OPSW_1) ) || IsResCalIdOpsw1(id) )  /* PRQA S 3453 */ /* MD_MSR_19.7 */
# define IsIdOfCalForOpsw2(id)  ( (((id) >= START_CALID_OPSW_2) && ((id) <= END_CALID_OPSW_2) ) || IsResCalIdOpsw2(id) )  /* PRQA S 3453 */ /* MD_MSR_19.7 */
# define IsIdOfCalForOpsw3(id)  ( (((id) >= START_CALID_OPSW_3) && ((id) <= END_CALID_OPSW_3) ) || IsResCalIdOpsw3(id) )  /* PRQA S 3453 */ /* MD_MSR_19.7 */
# define IsIdOfCalForOpsw4(id)  ( (((id) >= START_CALID_OPSW_4) && ((id) <= END_CALID_OPSW_4) ) || IsResCalIdOpsw4(id) )  /* PRQA S 3453 */ /* MD_MSR_19.7 */

# if !defined( IsResCalIdOpsw1 )
#  define IsResCalIdOpsw1(id) 0u
# endif
# if !defined( IsResCalIdOpsw2 )
#  define IsResCalIdOpsw2(id) 0u
# endif
# if !defined( IsResCalIdOpsw3 )
#  define IsResCalIdOpsw3(id) 0u
# endif
# if !defined( IsResCalIdOpsw4 )
#  define IsResCalIdOpsw4(id) 0u
# endif
#else /* ( FBL_MTAB_NO_OF_BLOCKS > 1 ) */
# if !defined( MODID_OPSW_LOCAL )
#  define MODID_OPSW_LOCAL        MODID_OPSW_1
#  define END_CALID_OPSW_LOCAL    (MODID_OPSW_1 + 19u)
# else
#  define END_CALID_OPSW_LOCAL    (MODID_OPSW_LOCAL + 9u)
# endif
# define START_CALID_OPSW_LOCAL (MODID_OPSW_LOCAL + 1u)

# if !defined( IsResCalIdOpswLocal )
#  define IsResCalIdOpswLocal(id) 0u
# endif
# define IsIdOfOpswLocal(id) (MODID_OPSW_LOCAL == (id) )  /* PRQA S 3453 */ /* MD_MSR_19.7 */
# define IsIdOfCalForOpswLocal(id)  ( (((id) >= START_CALID_OPSW_LOCAL) && ((id) <= END_CALID_OPSW_LOCAL) ) || IsResCalIdOpswLocal(id) )  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#endif /* ( FBL_MTAB_NO_OF_BLOCKS > 1 ) */

#if ( FBL_MTAB_NO_OF_BLOCKS > 1 )
# define ModIdOpSw(id)        ( IsIdOfOpsw1(id) || IsIdOfOpsw2(id) || IsIdOfOpsw3(id) || IsIdOfOpsw4(id) )  /* PRQA S 3453 */ /* MD_MSR_19.7 */
# define ModIdCalibration(id) ( IsIdOfCalForOpsw1(id) || IsIdOfCalForOpsw2(id) || IsIdOfCalForOpsw3(id) || IsIdOfCalForOpsw4(id) )  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#else
# define ModIdOpSw(id)        ( IsIdOfOpswLocal(id) )  /* PRQA S 3453 */ /* MD_MSR_19.7 */
# define ModIdCalibration(id) ( IsIdOfCalForOpswLocal(id) )  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#endif /* ( FBL_MTAB_NO_OF_BLOCKS > 1 ) */

#define ModIdFbl(id)          ( ((id) >= MODID_FBL_1) && ((id) <= MODID_FBL_LAST) )  /* PRQA S 3453 */ /* MD_MSR_19.7 */

#define ModIdSba(id)          ( (id) == (vuint16)MODID_SBA )  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define ModIddInRange(id)     ( (MIN_MODULE_ID <= (id)) && (MAX_MODULE_ID >=(id)) )  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define ModIdReserved(id)     ( ModIddInRange(id) && !( ModIdOpSw(id) || ModIdCalibration(id) ||  ModIdFbl(id) || ModIdSba(id)) )  /* PRQA S 3453 */ /* MD_MSR_19.7 */

/* Bits 15-7 are reserved, 6-0 contains mid */
#define FilterMid(mid)  ((mid) & 0x007Fu)  /* PRQA S 3453 */ /* MD_MSR_19.7 */

/***********************************************************************************************************************
 *  DEFINES: Partition handling
 **********************************************************************************************************************/

/* Macros for setting partition ID */
#define PARTID_BASEMOD  ((vuint8) 0x01u)
#define PARTID_MASK_BASE  (0x0Fu)
#define PARTID_MASK_CAL  (0xF0u)

/* Redefine this value to the locally programmed Opsw partition Id */
#if !defined( PARTID_OPSW_LOCAL )
# define PARTID_OPSW_LOCAL PARTID_OPSW_1
#endif

#define PARTID_OPSW_1   (PARTID_BASEMOD + 0x00u)
#define PARTID_OPSW_2   (PARTID_BASEMOD + 0x10u)
#define PARTID_OPSW_3   (PARTID_BASEMOD + 0x20u)
#define PARTID_OPSW_4   (PARTID_BASEMOD + 0x30u)

#if defined( FBL_ENABLE_CALIBRATION_MODULES )
/* changing base partition definition in this case */
# define FblHdrGetBasePartId(partId) ((vuint8)(((partId) & PARTID_MASK_CAL) + PARTID_BASEMOD)) /* PRQA S 3453 */ /* MD_MSR_19.7 */
#else
/* no base partition definitions in this case */
# define FblHdrGetBasePartId(partId) (partId)  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#endif

#define FblHdrGetFirstCalPartNbr(opSwPartition) ((vuint8) ((opSwPartition) + 1u)) /* PRQA S 3453 */ /* MD_MSR_19.7 */

/** Function to return partition Id for a partition Number 1..n inside a given Application header (opSwId) */
#if ( FBL_MTAB_NO_OF_BLOCKS > 1 )
# define PartIdOpSw(partId)        (((partId) == PARTID_OPSW_1) || ((partId) == PARTID_OPSW_2) || ((partId) == PARTID_OPSW_3) || ((partId) == PARTID_OPSW_4)) /* PRQA S 3453 */ /* MD_MSR_19.7 */
#else
# define PartIdOpSw(partId)        ((partId) == PARTID_OPSW_LOCAL ) /* PRQA S 3453 */ /* MD_MSR_19.7 */
#endif

#if defined( FBL_ENABLE_CALIBRATION_MODULES )
# if ( FBL_MTAB_NO_OF_BLOCKS > 1 )
#  define FblHdrGetPartNbrFromId(partId) ((partId) - FblHdrGetBasePartId(partId)) /* PRQA S 3453 */ /* MD_MSR_19.7 */
# else
#  define FblHdrGetPartNbrFromId(partId) ((partId) - PARTID_OPSW_LOCAL) /* PRQA S 3453 */ /* MD_MSR_19.7 */
# endif
#else
/* Partition number is always 0 if there are no calibration partitions. */
# define FblHdrGetPartNbrFromId(partId)  ((vuint8)0u) /* PRQA S 3453 */ /* MD_MSR_19.7 */
#endif

#if ( FBL_MTAB_NO_OF_BLOCKS > 1 )
# define MAX_PART_ID_CAL  (PARTID_OPSW_4 + PARTID_MASK_BASE)
#else
# define MAX_PART_ID_CAL  (PARTID_OPSW_LOCAL + PARTID_MASK_BASE)
#endif
#define PartIdCal(partId)     (((partId) <= MAX_PART_ID_CAL) && (((partId) & PARTID_MASK_BASE) > PARTID_BASEMOD)) /* PRQA S 3453 */ /* MD_MSR_19.7 */

#define FblHdrGetParsedModPartId() (parsedModuleInfo.partID)

#if !defined( FBL_HDR_ENABLE_EXPLICIT_ERASE ) ||\
     defined( FBL_ENABLE_GAP_FILL )
# define IsCalFirstModInPartition() (HDR_CALPOS_FIRST == (parsedModuleInfo.calPos & HDR_CALPOS_FIRST))  /* PRQA S 3453 */ /* MD_MSR_19.7 */
# define IsCalLastModInPartition() (HDR_CALPOS_LAST == (parsedModuleInfo.calPos & HDR_CALPOS_LAST)) /* PRQA S 3453 */ /* MD_MSR_19.7 */
#else
# define IsCalLastModInPartition() (HDR_CALPOS_LAST == parsedModuleInfo.calPos)
#endif

/* This is the maximum number of allowed gaps between partitions, it may be increased by user configuration file. */
#if !defined( HDR_MAX_PARTITION_TRACKED_REGIONS )
/* one entry is usually 5 byte and is required per to be large enough to hold (n+1+m)
 * - n appl regions (SWM_DATA_MAX_NOAR)
 * - 1 appl presence pattern
 * - m cal erase regions from all partitions
 */
# define HDR_MAX_PARTITION_TRACKED_REGIONS (SWM_DATA_MAX_NOAR + 1u + 10u)
#endif

/* This is the number of tracked erased state partitions per application. If you download more cal partitions, erased state is not tracked for these. */
#if defined( FBL_ENABLE_CALIBRATION_MODULES )
# if !defined( HDR_MAX_TRACKED_ERASED_STATE_PARTIONS )
#  define HDR_MAX_TRACKED_ERASED_STATE_PARTIONS 4u /* 1 application + n calibration partitions  */
# endif
#else
# define HDR_MAX_TRACKED_ERASED_STATE_PARTIONS 1u
#endif

#define HDR_GAPFILL_BUFF_SIZE_DEFAULT 0x40u
#if !defined( HDR_GAPFILL_BUFF_SIZE )
# if ( FLASH_SEGMENT_SIZE < HDR_GAPFILL_BUFF_SIZE_DEFAULT )
#  define HDR_GAPFILL_BUFF_SIZE HDR_GAPFILL_BUFF_SIZE_DEFAULT
# else
    /* In case of multiple mem devices, this will only work if internal flash has the largest Segment Size.
     * If this is not the case, the following will happen:
     *  - If assertions are enabled: assertion is hit during gap fill, else
     *  - LibMem will not write chunks within Gapfill feature.
     */
#  define HDR_GAPFILL_BUFF_SIZE FLASH_SEGMENT_SIZE
# endif
#endif

#define FblHdrErasedStateVar(blockNr, partNbr) (logBlockPartErasedStates[(blockNr)].partErasedState[(partNbr)]) /* PRQA S 3453 */ /* MD_MSR_19.7 */

/***********************************************************************************************************************
 *  DEFINES: PEC (Programming error code)
 **********************************************************************************************************************/
#define HDR_SIZE_PEC_ERROR  2u

/* DebugStatus will also be set to PecError.(no overwriting) */
#define FblSetPecErr(errNo)  {\
                                 errPecExtErrorCode = (errNo);\
                                 FblHdrDebugStatus(errNo);\
                             } /* PRQA S 3458 */ /* MD_MSR_19.4 */

/* Used if already set Pec shall not be overwritten. Can be used e.g. for setting LibMem (default/fallback) errors. */
#define FblSetPecErrIfNotYetSet(errNo)  {\
                                           if( HdrErr_NoError == errPecExtErrorCode )\
                                           {\
                                              errPecExtErrorCode = (errNo);\
                                              FblHdrDebugStatus(errNo);\
                                           }\
                                        } /* PRQA S 3458 */ /* MD_MSR_19.4 */

/* Also init Debug status */
#define FblClrPecErr() {\
                           errPecExtErrorCode = 0u;\
                           FblHdrStatSetError(0u);\
                       } /* PRQA S 3458 */ /* MD_MSR_19.4 */

/* This also returns Vector internals, for rpoerting use different representation. */
#define FblGetExtPecErr() (errPecExtErrorCode)

#define FblGetPecErrorLen() 2u

#if defined( FBL_ENABLE_CALIBRATION_MODULES )
# define  HdrErr_Revoke    (IsCalDownLd()? HdrErr_RevokeCal : HdrErr_RevokeSW)
# define  HdrErr_GetInfo   (IsCalDownLd()? HdrErr_GetCalInfo : HdrErr_GetAppInfo)
# define  HdrErr_Erase     (IsCalDownLd()? HdrErr_EraseCal : HdrErr_EraseSW)
# define  HdrErr_CID       (IsCalDownLd()? HdrErr_CCID : HdrErr_BCID)
#else
# define  HdrErr_Revoke  HdrErr_RevokeSW
# define  HdrErr_GetInfo HdrErr_GetAppInfo
# define  HdrErr_Erase   HdrErr_EraseSW
# define  HdrErr_CID     HdrErr_BCID
#endif

#define HdrErr_NoError          ((vuint16)0x0000u) /* No errors. */
#define HdrErr_PartitionId      ((vuint16)0x0001u) /* Indicates  an  attempt  to  erase/program  a  non-supported partition. */
#define HdrErr_SwNotPresent     ((vuint16)0x0002u) /* This  error  indicates  an  attempt  to  program  a calibration  module  while  the  ECU  is  not  fully programmed with valid application software. */
#define HdrErr_RevokeSW         ((vuint16)0x0003u) /* This  error  indicates  a  failed  attempt  to  revoke  the Application  SW  partition  (i.e.  set  the  PSI  to  a "Revoked" state). */
#define HdrErr_EraseSW          ((vuint16)0x0004u) /* This  error  indicates  a  failed  attempt  to  erase  the Application SW partition. */
#define HdrErr_GetAppInfo       ((vuint16)0x0005u) /* Indicates an error reading the Application SW Info */
#define HdrErr_RevokeCal        ((vuint16)0x0006u) /* This  error  indicates  a  failed  attempt  to  revoke  the calibration  partition  (i.e.  set  the  PSI  to  a  "Revoked" state). */
#define HdrErr_EraseCal         ((vuint16)0x0007u) /* This  error  indicates  a  failed  attempt  to  erase  the calibration partition. */
#define HdrErr_GetCalInfo       ((vuint16)0x0008u) /* Indicates an error reading the Cal Info */
#define HdrErr_UpdatePSI        ((vuint16)0x0009u) /* This error indicates a failed attempt to set the PSI to a "Programmed" State. */
#define HdrErr_DataType         ((vuint16)0x000Au) /* Data Type not supported */
#define HdrErr_Compression      ((vuint16)0x000Bu) /* Error while decompressing the data stream. */
#define HdrErr_LengthExceeded   ((vuint16)0x000Cu) /* The received file length exceeds the expected length. This PEC is set when the bootloader received more data in the  Transfer  Data service than  the  allocated space. */
#define HdrErr_MoreDataExpected ((vuint16)0x000Du) /* Indicates the length of the programmable file is below the expected length. This PEC is set when the client send  a Transfer End service while the bootloader is expecting more data to be written to flash. */
#define HdrErr_FlashWrite       ((vuint16)0x000Eu) /* Error writing a block to the flash memory */
#define HdrErr_ModuleID         ((vuint16)0x000Fu) /* Module ID is not expected or out of sequence */
#define HdrErr_BCID             ((vuint16)0x0010u) /* Application SW is not compatible with bootloader */
#define HdrErr_CCID             ((vuint16)0x0011u) /* Calibration Module is not compatible with Application SW */
#define HdrErr_EcuName          ((vuint16)0x0012u) /* The  ECU  name in  the flash file  does not  match the one stored in the bootloader Info block */
#define HdrErr_EcuId            ((vuint16)0x0013u) /* The flash file contains a non-zero ECU ID that does not match the ECU ID */
#define HdrErr_SwRegion         ((vuint16)0x0014u) /* The Application SW Region Info is out of the reserved region as defined in the Boot Info block */
#define HdrErr_Signature        ((vuint16)0x0015u) /* The signature on the header is invalid */
#define HdrErr_AppNBID          ((vuint16)0x0016u) /* The  Application  SW  has  an  older  security  version than the current programmed version. */
#define HdrErr_SubjectName      ((vuint16)0x0017u) /* The  Subject  Name  in  the  file  does  not  match  the expected subject name stored in the Boot Info block */
#define HdrErr_KeyNBID          ((vuint16)0x0018u) /* The Cert  key  has an  older security version than the previously used one known to the boot loader. */
#define HdrErr_Cert             ((vuint16)0x0019u) /* Invalid Cert signature */
#define HdrErr_CalRegion        ((vuint16)0x001Au) /* The Cal Region Info is out of the reserved region as defined in the Boot Info block */
#define HdrErr_MD               ((vuint16)0x001Bu) /* The  message  digest  calculated  over  the  flashed content  does  not  match  the  Message  Digest  in  the signed header */
#define HdrErr_RootSignature    ((vuint16)0x001Cu) /* The Signer Info certificate has invalid signature */
#define HdrErr_MsgOutOfSequence ((vuint16)0x001Du) /* General  error indicating that  the bootloader receives a message out of sequence during the programming session */
# define HdrErr_Undefined       ((vuint16)0x0020u) /* Undefined error. Use to map any error (e.g. supplier or ECU specific) that is not defined above */

#define HdrErr_ExtBase                         0x8000u  /* Used for Vector introduced Pec values */
#define HdrErr_ExtStdOff                       0x1000u
#define HdrErr_ExtMapOff                       0x2000u
#define HdrErr_ExtBaseVec                      (HdrErr_ExtBase|HdrErr_ExtStdOff)  /* Used for Vector introduced Pec values */
#define HdrErr_ExtBaseMap                      (HdrErr_ExtBase|HdrErr_ExtMapOff)  /* Used for Vector introduced Pec values, that map to standard Pec (restricted to 0x1-0xff) */
#define HdrErr_ExtBaseMapMask                   0x00FFu                           /* Mask out basic GM defined Pec errors (0xu1-0xff) */

/* Undifferentiated mappings */
#define HdrErr_ExtSwNotPresentOnTransfer           ((vuint16)HdrErr_ExtBaseMap|HdrErr_SwNotPresent)
#define HdrErr_ExtMoreComprDataExpected            ((vuint16)HdrErr_ExtBaseMap|HdrErr_MoreDataExpected)
#define HdrErr_ExtGetAppInfoValidatePlain          ((vuint16)HdrErr_ExtBaseMap|HdrErr_GetAppInfo)
#define HdrErr_ExtP1DataTypeNotPresent             ((vuint16)HdrErr_ExtBaseMap|HdrErr_DataType)

/* HdrErr_MsgOutOfSequence maps. */
#define HdrErr_ExtRequestDownloadMsgOutOfSequence  ((vuint16)HdrErr_ExtBaseMap|0x0400u|HdrErr_MsgOutOfSequence)
#define HdrErr_ExtUpdatePsiMsgOutOfSequence        ((vuint16)HdrErr_ExtBaseMap|0x0500u|HdrErr_MsgOutOfSequence)
#define HdrErr_ExtTransferDataMsgOutOfSequence     ((vuint16)HdrErr_ExtBaseMap|0x0600u|HdrErr_MsgOutOfSequence)
#define HdrErr_ExtTransferDataEndMsgOutOfSequence  ((vuint16)HdrErr_ExtBaseMap|0x0700u|HdrErr_MsgOutOfSequence)
#define HdrErr_ExtMemoryNotErased                  ((vuint16)HdrErr_ExtBaseMap|0x0800u|HdrErr_MsgOutOfSequence)

/* HdrErr_FlashWrite maps. */
#define HdrErr_ExtWriteAppNbidFailed               ((vuint16)HdrErr_ExtBaseMap|0x0900u|HdrErr_FlashWrite)
#define HdrErr_ExtWriteKeyNbidFailed               ((vuint16)HdrErr_ExtBaseMap|0x0A00u|HdrErr_FlashWrite)
#define HdrErr_ExtWriteUnusedBytesFailed           ((vuint16)HdrErr_ExtBaseMap|0x0B00u|HdrErr_FlashWrite)

/* Ext errors without mapping */
#define HdrErr_ExtTransferTooSmall                 ((vuint16)HdrErr_ExtBaseVec|0x0001u)
#define HdrErr_ExtValueOutOfRange                  ((vuint16)HdrErr_ExtBaseVec|0x0002u)
#define HdrErr_ExtTransferDataSequenceCounter      ((vuint16)HdrErr_ExtBaseVec|0x0004u)
#define HdrErr_ExtP1AppNBIDInconsistent            ((vuint16)HdrErr_ExtBaseVec|0x0005u)
#define HdrErr_ExtP1SegNbrInconsistent             ((vuint16)HdrErr_ExtBaseVec|0x0006u)
#define HdrErr_ExtP1SegRangeInconsistent           ((vuint16)HdrErr_ExtBaseVec|0x0007u)
#define HdrErr_ExtLibMemErase                      ((vuint16)HdrErr_ExtBaseVec|0x0008u)
#define HdrErr_ExtFlashWriteDriverNotReady         ((vuint16)HdrErr_ExtBaseVec|0x0009u)
#define HdrErr_ExtEraseDriverNotReady              ((vuint16)HdrErr_ExtBaseVec|0x000Au)
#define HdrErr_ExtDownloadOngoing                  ((vuint16)HdrErr_ExtBaseVec|0x000Bu)

#define HdrErr_ExtNrcConditionsNotCorrect          ((vuint16)HdrErr_ExtBaseVec|0x0022u)
#define HdrErr_ExtNrcRequestSequenceError          ((vuint16)HdrErr_ExtBaseVec|0x0024u)
#define HdrErr_ExtNrcRequestOutOfRange             ((vuint16)HdrErr_ExtBaseVec|0x0031u)
#define HdrErr_ExtNrcTransferDataSuspended         ((vuint16)HdrErr_ExtBaseVec|0x0071u)
#define HdrErr_ExtNrcGeneralProgrammingFailure     ((vuint16)HdrErr_ExtBaseVec|0x0072u)

/***********************************************************************************************************************
 *  DEFINES: Boot Initialization Status masks and access macros
 **********************************************************************************************************************/

#define SBILSB_ERR_BOOTROMCHK ((vuint8) 0x01u)
#define SBILSB_ERR_RAMCHK ((vuint8) 0x02u)

#define SBIMSB_ERR_ECUNAME ((vuint8) 0x01u)
#define SBIMSB_ERR_ECUID ((vuint8) 0x02u)
#define SBIMSB_ERR_SIGNATURE ((vuint8) 0x04u)
#define SBIMSB_ERR_CERT ((vuint8) 0x08u)
#define SBIMSB_SBI_VALID ((vuint8) 0x80u)

#define FblClrBisBytes()         {\
                                   FblClrBisLsbByte();FblClrBisMsbByte();\
                                 } /* PRQA S 3458 */ /* MD_MSR_19.4 */
#define FblSetBisLsbByte(nbr)    {\
                                   fblHdrBis.integrity |= (nbr);\
                                 } /* PRQA S 3458 */ /* MD_MSR_19.4 */
#define FblClrBisLsbByte()       {\
                                    fblHdrBis.integrity = 0u;\
                                 } /* PRQA S 3458 */ /* MD_MSR_19.4 */
#define FblGetBisLsbByte()       (fblHdrBis.integrity)
#define FblSetBisMsbByte(nbr)    {\
                                   fblHdrBis.signBypass |= (nbr);\
                                 } /* PRQA S 3458 */ /* MD_MSR_19.4 */
#define FblClrBisMsbByte()       {\
                                   fblHdrBis.signBypass = 0u;\
                                 } /* PRQA S 3458 */ /* MD_MSR_19.4 */
#define FblGetBisMsbByte()       (fblHdrBis.signBypass)

#define FblGetBisLen()            (sizeof(fblHdrBis.integrity) + sizeof(fblHdrBis.signBypass))

/***********************************************************************************************************************
 *  DEFINES: Data Processing
 **********************************************************************************************************************/
/* Global States, that are available also for actions behind Pass through interface, only use these for upper layers
 * like Diag */
#define FblHdrStateIsIdle()                   (HDR_PS_IDLE == rcvBuffHandle.progStepState)  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define FblHdrStateIsDownloadExpected()       (HDR_PS_DOWNLOAD_EXPECTED == rcvBuffHandle.progStepState)  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define FblHdrStateIsTransferExpected()       (FblHdrStateIsTransferHdrExpected() || FblHdrStateIsTransferNoHdrExpected())  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define FblHdrStateIsTransferEndExpected()    (HDR_PS_TRANSFEREND_EXPECTED == rcvBuffHandle.progStepState)  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define FblHdrStateIsUpdatePsiExpected()      (HDR_PS_UPDATEPSI_EXPECTED == rcvBuffHandle.progStepState)  /* PRQA S 3453 */ /* MD_MSR_19.7 */

/* Global States that are not visible behind Pass through interface, only use for layers below this module/ independent
 * from Pass through */
#define FblHdrStateIsTransferHdrExpected()    (HDR_PS_HDR_EXPECTED == rcvBuffHandle.progStepState)  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define FblHdrStateIsTransferNoHdrExpected()  (HDR_PS_TRANSFER_EXPECTED == rcvBuffHandle.progStepState)  /* PRQA S 3453 */ /* MD_MSR_19.7 */

/* For Mcmp handling */
#define FblHdrGetProgStep()                (rcvBuffHandle.progStepState)
#define FblHdrSetProgStep(progstep)        {\
                                              rcvBuffHandle.progStepState = (progStep);\
                                           } /* PRQA S 3458 */ /* MD_MSR_19.4 */

#if defined( FBL_ENABLE_COMPRESSION_MODE )
# define RAM_HEADER_LENGTH (HDR_MODULE_MAX_RAW_LEN) /* This is the maximum buffer needed to decompress the signed header */
# if ( FBL_MEM_PROC_BUFFER_SIZE < RAM_HEADER_LENGTH )
#  error "GENy parameter to small: Data processing buffer size. Please enlarge this parameter."
# endif
# define SBA_TICKET_PARSE_BUFFER FblRamHeader /* Reuse FblRamHeader for Sba ticket parsing in any case. */
#else
# if !defined( SBA_TICKET_PARSE_BUFFER )
#  define SBA_TICKET_PARSE_BUFFER FblRamHeader /* Per default use FblRamHeader for Sba ticket parsing. */
#  define RAM_HEADER_LENGTH HDR_SBA_TICKET_LEN /* RAM Header is used for Sba ticket Parsing only. */
# else
#  define RAM_HEADER_LENGTH 0u /* FblRamHeader is not used/required. */
# endif /* !defined( SBA_TICKET_PARSE_BUFFER ) */
#endif /* defined( FBL_ENABLE_COMPRESSION_MODE ) */

/***********************************************************************************************************************
 *  DEFINES: FBL_UTIL
 **********************************************************************************************************************/
#define FblHdrIsAddrAlignedToBinVal(address, segSize)           (( (address) & ( (segSize) - 1u)) == 0u) /* PRQA S 3453 */ /* MD_CBD_19.7 */
#define FblHdrGetBytesToNextAlignedAddress(address, segSize)    ((!FblHdrIsAddrAlignedToBinVal(address, segSize)) ? ((segSize) - (((address) & ((segSize) - 1u)))) : 0u) /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define FblHdrGetAlignedAddr(address, segSize)                  ((address) + FblHdrGetBytesToNextAlignedAddress(address, segSize)) /* PRQA S 3453 */ /* MD_MSR_19.7 */

/*
 * Macro for calling FblHdrPollingTask() every n-th loop cycle. This is
 * required for short loops which will be slowed down significantly if the
 * function is called every loop cycle.
 */
#if !defined( FblHdrPollingTaskInLoop )
# define FblHdrPollingTaskInLoop(loopCtr, loopfrequency) { \
                                                       if (((loopCtr) & ((loopfrequency) - 1u)) == 0u) \
                                                       { \
                                                          FblHdrPollingTaskVoid(); \
                                                       } \
                                                    } /* PRQA S 3458 */ /* MD_MSR_19.4 */
#endif

#if !defined( FBL_UTIL_CHECK_RANGE_OVERFLOW_LEN )
# define FBL_UTIL_CHECK_RANGE_OVERFLOW_LEN(rangeStart, rangeLength) \
   (((rangeLength) > 0u) && ((rangeStart) > (((FBL_ADDR_TYPE)(-1)) - ((rangeLength) - 1u))))  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#endif

#if !defined( FBL_UTIL_CHECK_ADDRESS_IN_RANGE_LEN )
# define FBL_UTIL_CHECK_ADDRESS_IN_RANGE_LEN(address, rangeStart, rangeLength) \
   (((address) >= (rangeStart)) && (((address) - (rangeStart)) < (rangeLength)))  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#endif

#if !defined( FBL_UTIL_CHECK_RANGE_CONTAINED_LEN )
# define FBL_UTIL_CHECK_RANGE_CONTAINED_LEN(address, length, rangeStart, rangeLength) \
   (   FBL_UTIL_CHECK_ADDRESS_IN_RANGE_LEN(address, rangeStart, rangeLength)          \
    && ((length) <= (rangeLength))                                                    \
    && (((address) - (rangeStart)) <= ((rangeLength) - (length))))  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#endif

#if !defined( FBL_UTIL_CHECK_RANGES_EXCLUSIVE_ADDR )
# define FBL_UTIL_CHECK_RANGES_EXCLUSIVE_ADDR(address, endAddress, rangeStart, rangeEnd ) \
   (((endAddress) < (rangeStart) ) || ((address) > (rangeEnd)))  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#endif

#if !defined( FBL_UTIL_CHECK_RANGES_EXCLUSIVE_LEN )
# define FBL_UTIL_CHECK_RANGES_EXCLUSIVE_LEN(address, length, rangeStart, rangeLength ) \
   FBL_UTIL_CHECK_RANGES_EXCLUSIVE_ADDR((address), (address) + ((length) - 1u), (rangeStart), (rangeStart) + ((rangeLength) - 1u))  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#endif

#if !defined( FBL_UTIL_GET_RANGE_LEN )
# define FBL_UTIL_GET_RANGE_LEN(startAddress, endAddresss) (((endAddresss) - (startAddress)) + (tFblAddress)1u)  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#endif

#if !defined( FBL_UTIL_GET_RANGE_END )
# define FBL_UTIL_GET_RANGE_END(startAddress, rangeLen) ((startAddress) + ((rangeLen) - 1u))  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#endif

#define kFblHdrMemSegmNotFound ((vsint16) -1)

/***********************************************************************************************************************
 *  DEFINES: Optional User functions
 **********************************************************************************************************************/

#if !defined( APPL_FBL_READ_SEC_DATA )
/* This function can be implemented in case the controller requires to differentiate between RAM and ROM pointers.
 * The default just passes a ROM pointer to the security module, whereas a RAM pointer is expected, which is
 * no problem for usual machines.
 */
# define APPL_FBL_READ_SEC_DATA(romLocation) (romLocation)  /* PRQA S 3453 */ /* MD_MSR_19.7 */
#endif

/***********************************************************************************************************************
 *  DEFINES: Other
 **********************************************************************************************************************/

/* Define compression type for compression list */
#if defined( FBL_ENABLE_COMPRESSION_MODE )
/* For differentiation, any compression type used need to be defined in any case
 * The values of all used compressions must be in the required order of cmprLst
 */

# if defined( DATA_TYPE_COMPRESSED_ARLE )
#  define COMPRESSION_TYPE_ARLE 0u
#  define COMPRESSION_TYPE_LZMA 1u
# else
#  define COMPRESSION_TYPE_LZMA 0u
#  define COMPRESSION_TYPE_ARLE 1u
# endif
#endif /* FBL_ENABLE_COMPRESSION_MODE */

/* Gm validation check states */
#define kDiagProgStateFullyProgrammed              0x00u
#define kDiagProgStateNoSoftwareOrCal              0x01u
#define kDiagProgStateNoCalibration                0x02u

/* Bit-masks for boot initialization status functionality */
#define kProgrammedStateRomError                   0x01u
#define kProgrammedStateRamError                   0x02u

#if defined( FBL_ENABLE_DATA_PROCESSING )
# define FblHdrModIsRawData() (MODULE_DF_RAW == currMemLibSegment.dataFormat)
#else
# define FblHdrModIsRawData() 1u
#endif

#define FblHdrGetDataInCurrSegment(byteNumber)  (tTpDataType) ((((tFblLength)(byteNumber) > currDataSegInfo.length))?currDataSegInfo.length : (byteNumber)) /* PRQA S 3453 */ /* MD_MSR_19.7 */
#define FblHdrConsumeCurrSegmentBytes(bytesToConsume) {\
                                                         currDataSegInfo.targetAddress += (bytesToConsume);\
                                                         currDataSegInfo.length -= (bytesToConsume);\
                                                      } /* PRQA S 3458 */ /* MD_MSR_19.4 */

# define FblHdrGetDataIndBytes(byteNumber) ((FblHdrModIsRawData())? FblHdrGetDataInCurrSegment(byteNumber):(byteNumber)) /* PRQA S 3453 */ /* MD_MSR_19.7 */

#if defined( FBL_ENABLE_VERIFY_INTEGRITY_WORD )
# define FblHdrCheckDldIntegrityWord SecM_VerifyClassVendor
#endif

#define HDR_NO_HEADER_ADDR_DEFINED ((tFblAddress)0xFFFFFFFFu)
#define HDR_NO_PP_ADDRESS_DEFINED ((tFblAddress)0xFFFFFFFFu)
#define HDR_UNDEFINED_LOG_BLOCK ((vuint8)0xFFu)
#define HDR_PARTITION_REGION_ERROR ((vuintx)0xFFu)
#define HDR_PARTITION_REGION_UNDEF ((vuintx)0xFEu)

#define ASCII_PN_PRESENT ((vuint16)0x0001u)
#define ASCII_PN_NOT_PRESENT ((vuint16)0x0000u)

/* PRQA L:TAG_FblHdr_857 */
/***********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

/* The function pointer type memory qualifiers have to be similar to tExportFct */
# if defined( FBL_ENABLE_BANKING )
typedef FBL_CALL_TYPE SecM_StatusType  (*FBL_CALL_TYPE tVerificationFct) ( SecM_VerifyParamType *verifyParam );
# else
typedef MEMORY_FAR SecM_StatusType  (*tVerificationFct) ( SecM_VerifyParamType *verifyParam );
# endif /* FBL_ENABLE_BANKING */

typedef enum
{
    MODULE_DF_RAW = 0x00u /**< data format is raw */
#if defined( FBL_ENABLE_DATA_PROCESSING )
   ,MODULE_DF_COMPR = 0x01 /**< data format is compressed */
   ,MODULE_DF_ENCR = 0x02 /**< data format is encrypted */
   ,MODULE_DF_HDR = 0x10 /**< only to be used as mask: add HDR property to any data processing */
   ,MODULE_DF_COMPR_HDR = 0x11 /**< data format is compressed, only process header */
   ,MODULE_DF_ENCR_HDR = 0x12 /**< data format is encrypted, only process header */
   ,MODULE_DF_LAST_SEGMENT = 0x20 /**< only to be used as mask: add LAST_SEGMENT property to any data processing */
   ,MODULE_DF_COMPR_LAST_SEGMENT = 0x21  /**< data format is compressed, this is the last segment */
   ,MODULE_DF_ENCR_LAST_SEGMENT = 0x22 /**< data format is encrypted, this is the last segment */
#endif
} tdataFormat;

typedef enum
{
    HDR_CALPOS_IRRELEVANT = 0x00u /* If the position is not relevant */
#if defined( FBL_HDR_DISABLE_EXPLICIT_ERASE ) ||\
    defined( FBL_ENABLE_GAP_FILL )
   ,HDR_CALPOS_FIRST = 0x01u
   ,HDR_CALPOS_LAST = 0x04u
   ,HDR_CALPOS_SINGLE = 0x05u  /** This is (HDR_CALPOS_FIRST|HDR_CALPOS_LAST) */
#else
   ,HDR_CALPOS_LAST = 0x04u
#endif
} tCalPosType;

#if defined( FBL_ENABLE_GAP_FILL )
typedef enum
{
   HDR_BLOCK_USED_BY_PARTITION = 0x01u,
   HDR_BLOCK_UNUSED_AND_IN_OTHER_PARTITION = 0x02u,
   HDR_BLOCK_UNUSED_AND_IN_BASEPARTITION = 0x03u,
   HDR_BLOCK_UNUSED_NOT_IN_BASEPARTITION = 0x04u
} tFblBlockPartUsage;
#endif

typedef enum
{
   /* Explicitly set values as this type may be exchanged via Mcmp */
   HDR_PS_IDLE = 0u, /* No programming step ongoing. */
   HDR_PS_DOWNLOAD_EXPECTED = 1u, /* Expect call to FblHdrRequestDownload() */
   HDR_PS_HDR_EXPECTED = 2u, /* Expect call to FblHdrTransferDataProcess() with Header information */
   HDR_PS_TRANSFER_EXPECTED = 3u, /* Expect call to FblHdrTransferDataProcess after having received Header information */
   HDR_PS_TRANSFEREND_EXPECTED = 4u, /* Expect call to FblHdrTransferDataEnd() */
   HDR_PS_UPDATEPSI_EXPECTED =5u /* Expect call to FblHdrUpdatePSI() */
} tProgStepState;

/* These functions depending on the configuration, are called implicitely internally or need to be called explicitly
 * from external: FblHdrEraseRoutine, FblHdrTransferDataEnd, FblHdrUpdatePSI */
typedef enum
{
   /* Explicitly set values as this type may be exchanged via Mcmp */
   HDR_INTERFACE_TYPE_ALL_EXPLICIT = 0u,
   HDR_INTERFACE_TYPE_ALL_IMPLICIT = 1u,
   HDR_INTERFACE_TYPE_ONLY_PSI_IMPLICIT = 2u,
   HDR_INTERFACE_TYPE_DUMMY_END_MARKER =3u

} tHdrInterfaceType;

typedef enum
{
   /* Explicitly set values as this type may be exchanged via Mcmp */
   HDR_DOWNLOAD_SOURCE_INIT = 0u,    /* Initialization value. */
   HDR_DOWNLOAD_SOURCE_DIAG = 1u,         /* Download Source 0 Reserved for Diag layer */
   HDR_DOWNLOAD_SOURCE_REMOTE = 2u,       /* Download Source 1 Reserved for remote request (through pass through If, typically Mcmp) */
   HDR_DOWNLOAD_SOURCE_GENERIC_2 = 3u,
   HDR_DOWNLOAD_SOURCE_GENERIC_3 = 4u,
   HDR_DOWNLOAD_SOURCE_GENERIC_4 = 5u
} tDownloadSource;

/* Erased state structure is per Logical Block table entry (Appl+Cal) */
typedef struct
{
  tFlashStatus partErasedState[HDR_MAX_TRACKED_ERASED_STATE_PARTIONS]; /**< +1 for application partition. */
} tLogBlockErasedState;

typedef struct
{
   vuint32 flagA; /**< SBI flag information A */
   vuint32 flagB; /**< SBI flag information B */
} tSBIFlag;

typedef struct
{
   vuint8 integrity; /**< integrity flag */
   vuint8 signBypass; /**< signature bypass status */
} tBootInitStatus;

typedef struct
{
#if ( FBL_MTAB_NO_OF_BLOCKS > 1 )
   vuint8 MID; /**< Module-ID */
#endif
   vuint16 stored; /**< stored Value */
   vuint16 received; /**< received Value */
} tNBIDInfo;

/* Structure for download module information */
typedef struct
{
   vuint8         modType;         /**< module Type */
   vuint8         MID;             /**< Module-ID */
   vuint8         partID;          /**< Partition-ID */
   vuint8         blockNbr;        /**< Logical Block Number associated to */
#if defined( FBL_ENABLE_CALIBRATION_MODULES )
   tCalPosType    calPos;          /**< Cal Position */
#endif
   FL_SegmentListType segmentList;
} tDownloadModuleInfo;

typedef struct
{
   vuint8 nbrOfRgns; /**< Number of regions */
   tFblAddress startAddr; /**< Module start address */
   tFblAddress swLocInfoAddr; /**< start address of Sw Location Info section in Memory */
   tFblAddress nbPartAddr; /**< start address of Number of partitions field */
} tBaseModInfo;

#if defined( FBL_ENABLE_CALIBRATION_MODULES )
/* Structure for partition information */
typedef struct
{
   vuint8 partID; /**< Partition-ID */
   vuint8 nbrOfPrts; /**< Number of partitions */
   vuint8 nbrOfRgns; /**< Number of regions */
   vuint8 nbrOfCals; /**< Number of cals */
   tFblAddress partInfoAddr; /**< start address of partition Info section for this Id in Memory */
   vuint16 lengthPart;  /**< length of partition info block */
   tFblAddress psiStart; /**< start address of psi for this partition */
} tCalPartitionInfo;

typedef struct
{
   vuint8 calNbr; /**< Current calibration file number in partition 1..n */
   tFblAddress startAddr; /**< Start address of "Cal Info" structure in memory */
} tCalInfo;
#endif

typedef struct
{
   vsint16 startBlk;
   vsint16 endBlk;
#if defined( FBL_ENABLE_CALIBRATION_MODULES ) || ( FBL_MTAB_NO_OF_BLOCKS > 1 )
   vuint8 partID;
# if defined( C_CPUTYPE_32BIT ) || defined( C_CPUTYPE_16BIT )
   vuint8 padding1; /* 2 byte alignment for structure */
# endif
# if defined( C_CPUTYPE_32BIT )
   vuint16 padding2; /* 4 byte alignment for structure */
# endif
#endif
} tPartitionSegment;

typedef struct
{
   tPartitionSegment segList[HDR_MAX_PARTITION_TRACKED_REGIONS];
} tPartitionBlockList;

typedef struct
{
   tTpDataType buffIdx;
   tProgStepState progStepState;
   vuint16 progDataOffset; /* Offset to next programming Data to be indicated */
} tHdrRcvBuffHdl;

/***********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

V_MEMRAM0 extern V_MEMRAM1 tDownloadModuleInfo V_MEMRAM2 parsedModuleInfo;
V_MEMRAM0 extern V_MEMRAM1 tBootInitStatus V_MEMRAM2 fblHdrBis;
V_MEMRAM0 extern V_MEMRAM1 tHdrRcvBuffHdl V_MEMRAM2 rcvBuffHandle;
V_MEMRAM0 extern V_MEMRAM1_NEAR FL_SegmentInfoType V_MEMRAM2_NEAR currDataSegInfo;
V_MEMRAM0 extern V_MEMRAM1_NEAR vuint16 V_MEMRAM2_NEAR errPecExtErrorCode;
V_MEMRAM0 extern V_MEMRAM1_NEAR tLogBlockErasedState V_MEMRAM2_NEAR logBlockPartErasedStates[FBL_MTAB_NO_OF_BLOCKS];
V_MEMRAM0 extern V_MEMRAM1_NEAR vuint8 V_MEMRAM2_NEAR fblProgrammedState;

# if defined( MCMP_MODE_SLAVE ) || defined( FBL_HDR_ENABLE_MEMSEGMENT_VAR )
V_MEMRAM0 extern V_MEMRAM1_NEAR vsint16 V_MEMRAM2_NEAR memSegment;
# endif /* MCMP_MODE_SLAVE */

#if ( RAM_HEADER_LENGTH > 0 )
# define FBL_HDR_RAMHEADER_START_SEC_DATA
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
V_MEMRAM0 extern V_MEMRAM1 vuint8 V_MEMRAM2 FblRamHeader[RAM_HEADER_LENGTH]; /**< Header in RAM used for Sba ticket parsing, compression */
# define FBL_HDR_RAMHEADER_STOP_SEC_DATA
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
#endif

/***********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

#if defined( FBL_ENABLE_COMPRESSION_MODE )
tFblResult FblHdrReadCmprHeader( V_MEMRAM1 tFblLength V_MEMRAM2 V_MEMRAM3 * cmprLength,
                                 const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * cmprBuffer,
                                 V_MEMRAM1 vuint16 V_MEMRAM2 V_MEMRAM3 * cmprDataOffset );
#endif /* FBL_ENABLE_COMPRESSION_MODE */

/* FblHdr Init Routines */
void FblHdrInitPowerOn( void );
void FblHdrInit( tHdrInterfaceType sequenceType );

/* Checkers Setters and Getters */
vuint8 FblGetModType( vuint8 mid );
void FblHdrGetPsiRegion( vuint8 partId, V_MEMRAM1 IO_PositionType V_MEMRAM2 V_MEMRAM3 * pPsiAddr, V_MEMRAM1 IO_SizeType V_MEMRAM2 V_MEMRAM3 * pPsiLen );
tFblAddress FblHdrGetBaseModuleHdrAddr( vuint8 partId );
V_MEMRAM1 tBlockDescriptor V_MEMRAM2 V_MEMRAM3 * FblGetBlockDescriptor( vuint8 partId );
#if ( FBL_MTAB_NO_OF_BLOCKS > 1 )
vuint8 GetBlockNbrFromPartId( vuint8 partId );
#else
#define GetBlockNbrFromPartId(partId) ((vuint8)0u) /* PRQA S 3453 */ /* MD_MSR_19.7 */
#endif
#if defined( MCMP_MODE_SLAVE )
/* These usually are defined in Core Diagnostic layer, in case of slave there is no Diag layer available. */
vsint16 FblMemSegmentNrGet( tFblAddress address );
tFblLength  FblReadProm( tFblAddress address, vuint8* buffer, tFblLength length );
#endif
tFblAddress FblHdrCheckModuleValidAndGetAddr( vuint8 mid );
#if defined( FBL_ENABLE_CALIBRATION_MODULES )
tFblResult FblHdrGetCalPartitionValid( vuint8 opSwPartId );
vuint8 FblHdrGetNoOfCalPartitions( vuint8 opSwPartId );
#endif
vuint16 FblHdrGetReportablePecError( void );

/* Fbl Hdr Services, in order. */
tFblResult FblHdrRequestDownload( tDownloadSource downloadSource );
#if defined( FBL_HDR_ENABLE_EXPLICIT_ERASE )
tFblResult FblHdrEraseRoutine( vuint8 partId, tDownloadSource downloadSource );
#endif
tFblResult FblHdrTransferDataProcess( const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * rcvBuffer, tTpDataType rcvDataSize );
#if defined( FBL_HDR_ENABLE_EXPLICIT_TRANSFEREND )
tFblResult FblHdrTransferDataEnd( void );
#endif
#if defined( FBL_HDR_ENABLE_EXPLICIT_UPDATEPSI )
tFblResult FblHdrUpdatePSI( vuint8 partId );
#endif

#if defined( FBL_ENABLE_GAP_FILL )
tFblResult FblHdrFillGaps( void );
#endif
#if defined( FBL_ENABLE_ROM_INTEGRITY_CHECK )
void FblHdrInitFblRomCheck( V_MEMRAM1 FL_SegmentListType V_MEMRAM2 V_MEMRAM3 * segList );
#endif
vuint16 FblCalculateCsum16Bit( const V_MEMRAM1 vuint16 V_MEMRAM2 V_MEMRAM3 * buffer, vuint32 len, vuint16 csum );

SecM_StatusType FblHdrVerifyIntegrity( V_MEMRAM1 SecM_VerifyParamType V_MEMRAM2 V_MEMRAM3 * verifyParam );
SecM_StatusType FblHdrPipelinedVerifyIntegrity( V_MEMRAM1 SecM_SignatureParamType V_MEMRAM2 V_MEMRAM3 * verifyParam );
void FblHdrPollingTaskVoid( void );

#if ( (!defined( FBL_ENABLE_CALIBRATION_MODULES ) && !defined( FBL_DISABLE_CALIBRATION_MODULES ) ) ||\
      (defined( FBL_ENABLE_CALIBRATION_MODULES ) && defined( FBL_DISABLE_CALIBRATION_MODULES ) ) )
# error "Error in fbl_cfg.h: Missing or incorrect usage of Calibration Modules feature switch."
#endif

#if ( (!defined( FBL_ENABLE_DATA_PROCESSING ) && !defined( FBL_DISABLE_DATA_PROCESSING ) ) ||\
      (defined( FBL_ENABLE_DATA_PROCESSING ) && defined( FBL_DISABLE_DATA_PROCESSING ) ) )
# error "Error in fbl_cfg.h: Missing or incorrect usage of Data processing interface switch."
#endif

#if ( (defined( FBL_ENABLE_COMPRESSION_MODE ) && !defined( FBL_ENABLE_DATA_PROCESSING ) ) ||\
      (defined( FBL_ENABLE_COMPRESSION_MODE ) && defined( FBL_DISABLE_DATA_PROCESSING ) ) )
# error "Error in fbl_cfg.h: Compression requires Data processing switch to be active."
#endif

#if ( FBL_DIAG_BUFFER_LENGTH < (HDR_MODULE_MAX_LEN) )
# error "Error in fbl_cfg.h: FBL_DIAG_BUFFER_LENGTH is not large enough. Check minimum configuration."
#endif

#if defined( FBL_ENABLE_ROM_INTEGRITY_CHECK )
# if ( FBL_GMHEADER_NOAR_FBL > SWM_DATA_MAX_NOAR )
#  error "SWM_DATA_MAX_NOAR is defined to small. FBL_GMHEADER_NOAR_FBL does not fit in."
# endif
#endif

#if ( !defined( SEC_ENABLE_WORKSPACE_EXTERNAL ) || !defined( SEC_ENABLE_WORKSPACE_INTERNAL ) )
# error "Both workspace external and internal need to be set."
#endif

/* PRQA L:TAG_FblHdr_828 */

#endif /* __FBL_HDR_H__ */

/***********************************************************************************************************************
 *  END OF FILE: FBL_HDR.H
 **********************************************************************************************************************/

