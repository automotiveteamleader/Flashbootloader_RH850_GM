/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief         OEM specific implementation of Diagnostic services supported in boot mode
 *                 Declaration of functions, variables, and constants
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
 *  01.00.00   2015-01-16  AWh     -                Creation
 *                         Dod     -                Update to be compatible with released fbl_hdr
 *                                 -                Modify UpdatePSI Routine ID and PEC and BIS DID IDs
 *                         AWh     -                Introduce Err_Undefined
 *                                 -                Remove assertion defines
 *                                 -                Correct routine Id response definition
 *                                 ESCAN00075381    No changes
 *  02.00.00   2015-01-16  AWh     ESCAN00078613    Use FblDiag_14229_Core component
 *                                 ESCAN00078404    Bootloader only accepts data parameter F3 for service 28
 *                                 ESCAN00078614    Compile error: FBL_ENABLE_CALIBRATION_MODULES and
 *                                                  FBL_ENABLE_DEBUG_STATUS need to be set
 *                                 ESCAN00078804    No changes
 *                                 ESCAN00078837    Remove Gap Fill support (now  completely in FblHdr)
 *                                 -                No changes
 *                                 ESCAN00078915    Stay In boot feature, rename DiagSetError to FblDiagSetError
 *                                 ESCAN00078613    Remove Sleepmode macros and vars now in core
 *                                 ESCAN00079275    Erase Routine request is not answered when transitioning from
 *                                                   application to boot.
 *  03.00.00   2015-01-16  AWh     ESCAN00079564    Api Change to allow using new FblSecHdr module that supports compression.
 *                                 ESCAN00079615    No changes
 *                                 ESCAN00079719    Change default tester node address to configured value instead of fixed
 *                                 ESCAN00079564    Corrections
 *                                 ESCAN00080449    Remove 0x27 0x07 subparam
 *  03.01.00   2015-06-12  AWh     ESCAN00082174    When Sending 0x10 0x02 within Default Session, the Fbl sends NRC 33
 *                                                   (now use FblDiag_14229_Core 2.xx branch )
 *  03.02.00   2015-07-23  AWh     ESCAN00083469    Prepare component for Swcp process
 *                                 ESCAN00083865    Assemble flashdriver set error to Debug status information
 *                                 ESCAN00083883    Insufficient functional response handling based on 2013 ISO 14229-1 version
 *  03.03.00   2015-12-04  AWh     ESCAN00085579    No changes
 *                                 ESCAN00085914    No changes
 *  03.04.00   2016-02-29  HRf     ESCAN00088540    Macro FBL_SLEEP_TIME shall be used only for projects with GENy
 *                                 ESCAN00088611    Support multiple diagnostic testers with FblWrapperCom_PduR
 *  03.05.00   2016-03-16  Shs     ESCAN00088916    Added support for PATAC SLP2
 *                                 ESCAN00089094    No changes
 *  04.00.00   2016-04-15  AWh     ESCAN00089529    Allow usage of FblDiag_SecHdr_Gm 3.xx.xx interface
 *  04.01.00   2016-11-10  AWh     ESCAN00090256    GB6000 changed requirements 2014_12_15 to 2016_03_02
 *                                 ESCAN00090901    GB6000 changed requirements per 2016_04_21 release
 *                                 ESCAN00091143    Changes to allow usage of non-Kb main and tool configured Cw
 *                                 ESCAN00091409    No changes
 *                                 ESCAN00092312    No changes
 *                         Dod     ESCAN00092740    No changes
 *  04.01.01   2016-11-12  AWh     ESCAN00092772    Correct V_GEN_GENERATOR_MSR use case, sleep default time 8s for GM
 *                                 ESCAN00092793    No changes
 *  04.01.02   2016-11-22  AWh     ESCAN00092937    Required FBL_ENABLE_SERVICE_POSTHANDLER is not defined
 *  04.01.03   2016-12-14  Shs     ESCAN00093225    Added Support for PATAC SLP2 GEM Security Access and InitExtReflash Routine
 *                                 ESCAN00093243    No changes
 *                         AWh     ESCAN00093284    Allow for more flexible 11-bit Rx handling
 *  04.01.04   2017-02-09  AWh     ESCAN00093739    Set RID 21E (ExtReflash) default configuration to disabled
 *                                 ESCAN00093934    No changes
 *                                 ESCAN00093935    Patac GEM: RID 21E (ExtReflash) does not have status response byte
 *  04.02.00   2017-03-09  Wyg     ESCAN00094278    Make StayInBoot message configurable
 *  04.02.01   2017-03-20  AWh     ESCAN00094415    No changes
 *                         AWh     ESCAN00093957    Gm wants event timer configuration of 20.000ms with GB6002 1.4.2
 **********************************************************************************************************************/

#ifndef __FBL_DIAG_OEM_H__
#define __FBL_DIAG_OEM_H__

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

#include "fbl_hdr.h"

/***********************************************************************************************************************
 *  VERSION
 **********************************************************************************************************************/

/* --- fbl_diag module version --- */
/* ##V_CFG_MANAGEMENT ##CQProject : FblDiag_14229_Gm CQComponent : Implementation */
#define FBLDIAG_14229_GM_VERSION                            0x0402u
#define FBLDIAG_14229_GM_RELEASE_VERSION                    0x01u

/* Reference interface version */
/** Major interface version identifies incompatible changes */
#define FBL_DIAGCORE_API_REFERENCE_VERSION_MAJOR            0x02u
/** Minor interface version identifies backward compatible changes */
#define FBL_DIAGCORE_API_REFERENCE_VERSION_MINOR            0x06u
/** Release interface version identifies cosmetic changes */
#define FBL_DIAGCORE_API_REFERENCE_VERSION_RELEASE          0x00u

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

/* Generic helper macros, macros from core can be reused */
/* PRQA S 3453 2 */ /* MD_CBD_19.7 */
#define GET_HBYTE(word)                                     GET_ID_HIGH(word)
#define GET_LBYTE(word)                                     GET_ID_LOW(word)

#define DiagGetSid()                                       (diagServiceCurrent)

/***********************************************************************************************************************
 *  Core Diagnostic configuration
 **********************************************************************************************************************/

/* PRQA S 0828 EOF */ /* MD_MSR_1.1_828 */

#define FBL_DISABLE_SERVICE_PREHANDLER
#if !defined( FBL_ENABLE_SERVICE_POSTHANDLER ) && !defined( FBL_DISABLE_SERVICE_POSTHANDLER )
# define FBL_ENABLE_SERVICE_POSTHANDLER
#endif /* !FBL_ENABLE_SERVICE_POSTHANDLER  && !FBL_DISABLE_SERVICE_POSTHANDLER */
#define FBL_DIAG_ENABLE_OEM_READPROM
#define FBL_DIAG_ENABLE_OEM_SEGMENTNRGET
#define FBL_DIAG_ENABLE_OEM_INITPOWERON
#define FBL_DIAG_ENABLE_OEM_INIT
#define FBL_DIAG_ENABLE_14229_2013

#define ApplFblSecurityInit                                 FblDummyFunction
#define ApplFblSecuritySeedInit                             FblDummyFunction

/* kSecSigLength is unused in GM case, define it to pass Core check */
#define kSecSigLength                                       0u

/* Set default user subfunction configuration. */
#if !defined( FBL_ENABLE_USERSUBFUNCTION ) && !defined( FBL_DISABLE_USERSUBFUNCTION )
# define FBL_DISABLE_USERSUBFUNCTION
#endif
/***************** GB6000 version dependent configuration; Default: latest version (2016-04-21) **********************/
/* Set default: Ota Request Seed is supported. */
# if !defined ( FBL_DIAG_ENABLE_GM_REQUEST_SEED_OTA ) && !defined( FBL_DIAG_DISABLE_GM_REQUEST_SEED_OTA )
#  define FBL_DIAG_ENABLE_GM_REQUEST_SEED_OTA
# endif

/* Set default: Check Programming dependencies service supported. */
#  if !defined ( FBL_DIAG_ENABLE_GM_ROUTINE_CHECKPROGDEP ) && !defined( FBL_DIAG_DISABLE_GM_ROUTINE_CHECKPROGDEP )
#   define FBL_DIAG_ENABLE_GM_ROUTINE_CHECKPROGDEP
#  endif
/* Set default: Set Programming State Flag not supported (would only be required to be supported in CGM gateway). */
#  if !defined ( FBL_DIAG_ENABLE_GM_ROUTINE_SETPROGSTATEFLAG ) && !defined( FBL_DIAG_DISABLE_GM_ROUTINE_SETPROGSTATEFLAG )
#   define FBL_DIAG_DISABLE_GM_ROUTINE_SETPROGSTATEFLAG
#  endif
/* Set default: Diagnostic Initiate Extended Reflash not supported. */
#  if !defined ( FBL_DIAG_ENABLE_GM_ROUTINE_INITEXTREFLASH ) && !defined( FBL_DIAG_DISABLE_GM_ROUTINE_INITEXTREFLASH )
#   define FBL_DIAG_DISABLE_GM_ROUTINE_INITEXTREFLASH
#  endif
/***********************************************************************************************************************
 *  FblDiag_SecHdr configuration
 **********************************************************************************************************************/

# define FBL_MEM_DEFAULT_PREAMBLE_LENGTH_DIAG               2u
# define FBL_MEM_MAX_PREAMBLE_LENGTH_DIAG                   2u

/***********************************************************************************************************************
 *  State handling
 **********************************************************************************************************************/

/* Download sequence */
#define kDiagStateIdxDisableNormalCom                      (kDiagLastCoreStateIdx + 1u)
#define kDiagStateIdxSecurityUnlock                        (kDiagStateIdxDisableNormalCom + 1u)
# define kDiagLastOemStateIdx                               kDiagStateIdxSecurityUnlock

/* Download sequence states */
#define GetDisableNormalCom()                               GetDiagState(kDiagStateIdxDisableNormalCom)
#define GetSecurityUnlock()                                 GetDiagState(kDiagStateIdxSecurityUnlock)

/***********************************************************************************************************************
 *  Timeout handling
 **********************************************************************************************************************/
#if !defined( FBL_SLEEP_TIME )
#  define FBL_SLEEP_TIME                                    20000u /**< Wait 20s before entering sleep mode (specifications previous to GB6002 1.4.2 asked for 8s / 60s, overwrite the value if required) */
#endif

/***********************************************************************************************************************
 *  Service handling
 **********************************************************************************************************************/

/* ControlDTCSetting */
#define kDiagSubDtcOn                                       0x01u  /**< Subservice ID - DTC on */
#define kDiagSubDtcOff                                      0x02u  /**< Subservice ID - DTC off */

/* Has been 0x03 in GB6000 2015_05_28 and earlier. Can be overwritten to previously valid, if required. */
#if !defined( kDiagSubRequestAllowedMask )
# define kDiagSubRequestAllowedMask                         0x01u  /**< communication Type mask: 0x01 -> Disable normal messages */
#endif

/* Security access */
#define kDiagSubRequestSeedService                          0x01u  /**< Subservice ID - Request seed serviceProgrammingandConfiguration*/
#define kDiagSubRequestSeedAssembly                         0x03u  /**< Subservice ID - Request seed assemblyPlantProgrammingandConfiguration */
#if defined ( FBL_DIAG_ENABLE_GM_REQUEST_SEED_OTA )
# define kDiagSubRequestSeedOta                             0x05u  /**< Subservice ID - Request seed overtheAirReflash */
#endif

#if !defined( kSecSeedLength )
# define kSecSeedLength                                     31u  /**< Length of seed provided by ECU */
#endif

/* Request parameter lengths excluding the service ID */
#define kDiagRqlDiagnosticSessionControlParameter           0x00u
#define kDiagRqlSecurityAccessSeedParameter                 0x00u
#define kDiagRqlSecurityAccessKeyParameter                  kSecKeyLength
#define kDiagRqlControlDTCSettingParameter                  0x00u
#define kDiagRqlRequestDownloadParameter                    0x08u
#define kDiagRqlTransferDataParameter                       0x00u

/* Request lengths excluding the service ID */
#define kDiagRqlDiagnosticSessionControl                   (0x01u + kDiagRqlDiagnosticSessionControlParameter)
#define kDiagRqlReadDataByIdentifier                        0x02u
#define kDiagRqlSecurityAccessSeed                         (0x01u + kDiagRqlSecurityAccessSeedParameter)
#define kDiagRqlSecurityAccessKey                          (0x01u + kDiagRqlSecurityAccessKeyParameter)
#define kDiagRqlCommunicationControl                        0x02u
#define kDiagRqlWriteDataByIdentifier                       0x02u
#define kDiagRqlWriteDataByIdentifierFingerPrint           (kDiagRqlWriteDataByIdentifier + kDiagRqlWriteDataByIdentifierFingerPrintParameter)
#define kDiagRqlRoutineControl                              0x03u
#if defined( FBL_ENABLE_STAY_IN_BOOT )
# define kDiagRqlRoutineControlForceBoot                     kDiagRqlRoutineControl
#endif
#define kDiagRqlRequestDownload                            (0x02u + kDiagRqlRequestDownloadParameter)
#define kDiagRqlTransferData                               (0x01u + kDiagRqlTransferDataParameter)
#define kDiagRqlRequestTransferExit                         0x00u
#define kDiagRqlTesterPresent                               0x01u
#define kDiagRqlControlDTCSetting                          (0x01u + kDiagRqlControlDTCSettingParameter)

/* Response parameter lengths excluding the service ID */
#define kDiagRslEcuResetParameter                           0x00u
#define kDiagRslDiagnosticSessionControlParameter           0x04u
#define kDiagRslSecurityAccessSeedParameter                 kSecSeedLength
#define kDiagRslSecurityAccessKeyParameter                  0x00u
#define kDiagRslTransferDataParameter                       0x00u
#define kDiagRslRequestTransferExitParameter                0x00u
#define kDiagRslRequDldMaxBlockLenParameter                 0x02u
/* Response lengths excluding the service ID */
#define kDiagRslDiagnosticSessionControl                   (0x01u + kDiagRslDiagnosticSessionControlParameter)
#define kDiagRslEcuReset                                   (0x01u + kDiagRslEcuResetParameter)
#define kDiagRslReadDataByIdentifier                        0x02u
#define kDiagRslSecurityAccessSeed                         (0x01u + kDiagRslSecurityAccessSeedParameter)
#define kDiagRslSecurityAccessKey                          (0x01u + kDiagRslSecurityAccessKeyParameter)
#define kDiagRslCommunicationControl                        0x01u
#define kDiagRslWriteDataByIdentifier                       0x02u
#define kDiagRslRoutineControl                              0x03u
#define kDiagRslRequestDownload                            (0x01u + kDiagRslRequDldMaxBlockLenParameter)
#define kDiagRslTransferData                               (0x01u + kDiagRslTransferDataParameter)
#define kDiagRslRequestTransferExit                        (0x00u + kDiagRslRequestTransferExitParameter)
#define kDiagRslTesterPresent                               0x01u
#define kDiagRslControlDTCSetting                           0x01u
#if defined( FBL_ENABLE_STAY_IN_BOOT )
# define kDiagRslRoutineControlStayInBoot                   0x03u
#endif

/* Access to DiagBuffer */
#define kDiagFmtServiceId                                   0x00u                      /**< Position of service id             */
#define kDiagFmtSubparam                                   (kDiagFmtServiceId + 1u)    /**< Position of DID                    */
#define kDiagFmtNegResponse                                (kDiagFmtSubparam + 1u)     /**< Position of negative response code */
#define kDiagFmtpbDiagData                                  kDiagFmtSubparam
#define kDiagFmtRoutineIdHigh                              (kDiagFmtSubparam + 1u)     /**< RoutineControl service formats */
#define kDiagFmtRoutineIdLow                               (kDiagFmtRoutineIdHigh + 1u)

#define kDiagPartIdLen 1u
/* Request lengths excluding the service ID for Routines (OEM dependent) */
#define kDiagRqlRoutineControlEraseMemory                  (kDiagRqlRoutineControl + kDiagPartIdLen)
#define kDiagRqlRoutineControlUpdatePSI                    (kDiagRqlRoutineControl + kDiagPartIdLen)
#  if !defined( kDiagRqlRoutineControlCheckProgDep )
#   define kDiagRqlRoutineControlCheckProgDep              (kDiagRqlRoutineControl)
#  endif
#  if !defined( kDiagRqlRoutineControlSetProgStateFlag )
#   define kDiagRqlRoutineControlSetProgStateFlag          (kDiagRqlRoutineControl + 8u)
#  endif
# if !defined( kDiagRqlRoutineControlInitExtReflash )
#  define kDiagRqlRoutineControlInitExtReflash             (kDiagRqlRoutineControl)
# endif

#define kDiagRslRoutineControlEraseMemory                   kDiagRslRoutineControl
#define kDiagRslRoutineControlUpdatePSI                     kDiagRslRoutineControl
#  if !defined( kDiagRslRoutineControlCheckProgDep )
#   define kDiagRslRoutineControlCheckProgDep              (kDiagRslRoutineControl + 5u)
#  endif
#  if !defined( kDiagRslRoutineControlSetProgStateFlag )
#   define kDiagRslRoutineControlSetProgStateFlag          (kDiagRslRoutineControl)
#  endif
# if !defined( kDiagRslRoutineControlInitExtReflash )
#  define kDiagRslRoutineControlInitExtReflash             (kDiagRslRoutineControl + 1u)
# endif

/* Diagnostic service index to pbDiagParam service handler Parameter and customer API */

/* General */
#define kDiagLocFmtSubparam                                 0u                            /**< Position of DID */
#define kDiagLocFmtNegResponse                             (kDiagLocFmtSubparam + 1u)     /**< Position of negative response code */
#define kDiagLocFmtSubfunction                              kDiagLocFmtSubparam                        /**< Position of Subfunction */
/* Transfer Data */
#define kDiagLocFmtSequenceCnt                              0u                            /**< Position of sequence counter */
#define kDiagLocFmtDataOffset                              (kDiagLocFmtSequenceCnt + 1u)  /**< Offset to download data in TransferData frame */
/* Request Download */
#define kDiagLocFmtFormatOffset                             0x01u                         /**< Position of RequestDownload Alfid  */
#define kDiagLocFmtAddrOffset                               0x02u                         /**< Position of RequestDownload memAddr*/
/* Routine Control */
#define kDiagLocFmtRoutineIdHigh                           (kDiagLocFmtSubparam + 1u)     /**< RoutineControl service formats */
#define kDiagLocFmtRoutineIdLow                            (kDiagLocFmtRoutineIdHigh + 1u)
#define kDiagLocFmtRoutinePartInfo                         (kDiagLocFmtRoutineIdLow + 1u)
#  define kDiagLocFmtRoutineCheckProgDepStatus             (kDiagLocFmtRoutineIdLow + 1u)
#  define kDiagLocFmtRoutineSetProgStateFlagStatus         (kDiagLocFmtRoutineIdLow + 1u)
# define kDiagLocFmtRoutineInitExtReflash                  (kDiagLocFmtRoutineIdLow + 1u)

/* Security Access */
#define kDiagLocFmtSeedKeyStart                            (kDiagLocFmtSubparam + 1u)     /**< Start index of seed/key value */
/* Read Data By Identifier */
#define kDiagLocFmtDid                                      kDiagLocFmtSubparam           /**< Start index of Did */
#define kDiagLocFmtDidResp                                 (kDiagLocFmtSubparam + 2u)     /**< Start of location where to place Response Bytes */

/* -- Defines for address and length format identifier -- */
# if !defined(kDiagRequestDownloadALFI)
#  define kDiagRequestDownloadALFI                          0x44u
# endif

/***********************************************************************************************************************
 *  Gm specific bootloader Routine IDs and DIDs
 **********************************************************************************************************************/

#define kDiagRoutineIdEraseMemory                           0xFF00u
#define kDiagRoutineIdEraseMemoryHigh                       GET_ID_HIGH(kDiagRoutineIdEraseMemory)
#define kDiagRoutineIdEraseMemoryLow                        GET_ID_LOW(kDiagRoutineIdEraseMemory)

#define kDiagRoutineIdUpdatePSI                             0x0209u
#define kDiagRoutineIdUpdatePSIHigh                         GET_ID_HIGH(kDiagRoutineIdUpdatePSI)
#define kDiagRoutineIdUpdatePSILow                          GET_ID_LOW(kDiagRoutineIdUpdatePSI)

#  define kDiagRoutineIdCheckProgDep                         0xFF01u
#  define kDiagRoutineIdCheckProgDepHigh                     GET_ID_HIGH(kDiagRoutineIdCheckProgDep)
#  define kDiagRoutineIdCheckProgDepLow                      GET_ID_LOW(kDiagRoutineIdCheckProgDep)

#  define kDiagRoutineIdSetProgStateFlag                     0x0258u
#  define kDiagRoutineIdSetProgStateFlagHigh                 GET_ID_HIGH(kDiagRoutineIdSetProgStateFlag)
#  define kDiagRoutineIdSetProgStateFlagLow                  GET_ID_LOW(kDiagRoutineIdSetProgStateFlag)

# define kDiagRoutineIdInitExtReflash                       0x021Eu
# define kDiagRoutineIdInitExtReflashHigh                   GET_ID_HIGH(kDiagRoutineIdInitExtReflash)
# define kDiagRoutineIdInitExtReflashLow                    GET_ID_LOW(kDiagRoutineIdInitExtReflash)

#if defined( FBL_ENABLE_STAY_IN_BOOT )
# define kDiagRoutineIdStayInBoot                           0xF518u
# define kDiagRoutineIdStayInBootHigh                       GET_ID_HIGH(kDiagRoutineIdStayInBoot)
# define kDiagRoutineIdStayInBootLow                        GET_ID_LOW(kDiagRoutineIdStayInBoot)
#endif /* FBL_ENABLE_STAY_IN_BOOT */

/* This is a Vector internal define used for DebugStatus */
#define kDiagDidReadLastError                              ((vuint16) 0xCF00u)

/***********************************************************************************************************************
 * Generic Diag parameters
 **********************************************************************************************************************/

#if defined( FBL_ENABLE_UNALIGNED_DATA_TRANSFER )
# define kDiagTransferDataBufferLen                        FBL_DIAG_BUFFER_LENGTH
#else
/* "2" is the length of the TransferData  "SID + subParam" */
# define kDiagTransferDataBufferLen                       (((FBL_DIAG_BUFFER_LENGTH - 2u) & FblInvert16bit(FBL_DIAG_SEGMENT_SIZE - 1u)) + 2u)
#endif /* FBL_ENABLE_UNALIGNED_DATA_TRANSFER */
#define GetDiagRsParamRequDlMaxBlockLen1()                 GET_HBYTE(kDiagTransferDataBufferLen)
#define GetDiagRsParamRequDlMaxBlockLen2()                 GET_LBYTE(kDiagTransferDataBufferLen)

/* Session Control Response Bytes */
#define GetDiagRsParamSessControl1()                      (GET_HBYTE(kDiagSessionTimingP2))
#define GetDiagRsParamSessControl2()                      (GET_LBYTE(kDiagSessionTimingP2))
#define GetDiagRsParamSessControl3()                      (GET_HBYTE(kDiagSessionTimingP2Star))
#define GetDiagRsParamSessControl4()                      (GET_LBYTE(kDiagSessionTimingP2Star))

/***********************************************************************************************************************
 * Oem Specific functionality
 **********************************************************************************************************************/

#define kFblDiagMemSegmNotFound ((vsint16) - 1)

#define kDiagInitErasePartition 0u

/* PRQA S 3453 2 */ /* MD_CBD_19.7 */
#define FblIsAddrAlignedToBinVal(address, segSize)        ( ( (address) & ( (segSize) - 1u)) == 0u)
#define GetBytesToNextAlignedAddress(address, segSize)    ( (!FblIsAddrAlignedToBinVal(address, segSize))?\
                                                              ((segSize) - (((address) & ((segSize) - 1u)))) : 0u\
                                                          )  /* PRQA S 3453 */ /* MD_CBD_19.7 */

#if !defined( kDiagSigBufLength )
# define kDiagSigBufLength                                 0x01u
#endif

#if defined( V_GEN_GENERATOR_MSR )
#else
# define FBL_CW_ENABLE_NORMAL_FIXED_ALLOWING_11BIT_RX
/* Below legacy defines for Comwrapper_Can versions < 02.05.00 and != 02.01.81*/
# if defined(DIAG_TESTER_NODE_ADDR1_USED)
#  define FBL_CW_FIXED_RXHDL0_TARGET_ADDR DIAG_TESTER_NODE_ADDR1_USED
#  define FBLCW_FIXED_RXHDL0_TARGET_ADDR FBL_CW_FIXED_RXHDL0_TARGET_ADDR /* Compatibility define */
# endif /* DIAG_TESTER_NODE_ADDR1_USED */
#endif

#if defined( FBL_ENABLE_DEBUG_STATUS )
/* Vector Oem Gm specific Debug status information error codes */
#define kDiagErrCsumCheckError                             0x0700u
#define kDiagErrCodeNotAligned                             0x0F00u
#define kDiagErrNoMemoryRegionFound                        0x1000u
#define kDiagErrFlashcodeInitFailed                        0x1100u
#define kDiagErrCopyWatchdog                               0x1200u
#define kDiagErrFlashVerify                                0x1300u

#define kDiagErrValidationFailed                           0x1400u
#define kDiagErrDownloadInProgress                         0x1500u
#define kDiagErrIncompatibleModule                         0x1600u
#define kDiagErrFillFailed                                 0x1900u
#define kDiagErrValueOutOfRange                            0x1B00u
#define kDiagErrFlashReadFailed                            0x1C00u
#define kDiagErrTPFailed                                   0x1D00u

#define kDiagErrSignatureVerification                      0x4000u
#define kDiagErrWrongSignVersionOrKeyId                    0x4100u
#define kDiagErrNoApplPresent                              0x4200u
#define kDiagErrPPOverlapping                              0x4300u
#define kDiagErrNbidFailed                                 0x4400u
#define kDiagErrProgConditionsWrong                        0x4700u
#define kDiagErrSequenceCounter                            0x4800u
#define kDiagErrAlfi                                       0x4900u
#define kDiagErrCommCtrlParam                              0x4B00u

/* kDiagErrXXX values > 0x8000 are reserved for mem errors, check fbl_mem_oem.h */

/* Macros for error status reporting */
# define FblErrStatSetError(error)                        (errStatErrorCode = (error)) /* PRQA S 3453 */ /* MD_CBD_19.7 */
# define FblErrStatSetSId(id)                             (errStatLastServiceId = (id)) /* PRQA S 3453 */ /* MD_CBD_19.7 */
# define FblErrStatSetFlashDrvError(error)                (errStatFlashDrvErrorCode = (error)) /* PRQA S 3453 */ /* MD_CBD_19.7 */

# if defined( __FILE__ ) && defined( __LINE__ )
#  define FblErrDebugStatus(error)            {\
                                                 FblErrDebug((error), (tDebugFileName) __FILE__, __LINE__);\
                                              } /* PRQA S 3458 */ /* MD_CBD_19.4 */
# else
#  define FblErrDebugStatus(error)            {\
                                                 FblErrDebug((error), 0, 0);\
                                              } /* PRQA S 3458 */ /* MD_CBD_19.4 */
# endif

#else
# define FblErrStatSetError(error)
# define FblErrStatSetSId(id)
# define FblErrStatSetFlashDrvError(error)
# define FblErrDebugStatus(error)
# define FblErrDebugDriver(addr, error)
# endif

/* Compatibility define for kb main */
#if defined( FBL_DEF_ENABLE_NON_KB_MAIN )
#else
# define FblMainGetStartFromAppl()      (START_FROM_APPL == (GetFblMode() & START_FROM_APPL))
#endif

/***********************************************************************************************************************
 *  TYPEDEFS
 **********************************************************************************************************************/

#if defined( FBL_ENABLE_DEBUG_STATUS )
typedef int tDebugLineNbr;
typedef V_MEMROM1 vuint8 V_MEMROM2 V_MEMROM3 * tDebugFileName;
#endif

/***********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

#if defined( FBL_ENABLE_STAY_IN_BOOT )
vuint8      FblDiagCheckStartMsg(const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 *pData, vuintx length);
#endif  /* FBL_ENABLE_STAY_IN_BOOT */

#if defined( V_GEN_GENERATOR_MSR )
#else
vuint8 FblDiagGetTesterAddr(void);
#endif

/***********************************************************************************************************************
 *    Initialization handling of diagnostic module
 **********************************************************************************************************************/
#define FblDiagOemInitStartFromAppl() {\
                                         SetSecurityUnlock();\
                                      } /* PRQA S 3458 */ /* MD_CBD_19.4 */

/***********************************************************************************************************************
 * Oem specific Support functions
 **********************************************************************************************************************/

tFblResult FblDiagCheckTesterSourceAddr(vuint8 sourceId);
#if defined( FBL_ENABLE_DEBUG_STATUS )
void FblErrDebug( vuint16 error, tDebugFileName fileName, tDebugLineNbr lineNumber );
void FblErrDebugDriver( tFblAddress addr, vuint16 error );
#endif

#define FBLDIAG_RAMCODE_START_SEC_CODE
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */
void FblDummyFunction(void);
# define FBLDIAG_RAMCODE_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/***********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/
/* declaration used for user service */
#if defined( FBL_ENABLE_STAY_IN_BOOT )
V_MEMROM0 extern V_MEMROM1 vuint8 V_MEMROM2 fblDiagStayInBootMsg[]; /* PRQA S 3684 */ /* MD_FblDiag_3684 */
V_MEMROM0 extern V_MEMROM1 vuintx V_MEMROM2 kSizeOfStayInBootMsg;
#endif  /* FBL_ENABLE_STAY_IN_BOOT */

/* Diagnostics Node Address */
V_MEMRAM0 extern V_MEMRAM1 vuint8 V_MEMRAM2 diagNodeAddr;

#if defined( FBL_ENABLE_DEBUG_STATUS )
/* Common variables for error status reporting */
V_MEMRAM0 extern V_MEMRAM1      vuint16            V_MEMRAM2      errStatErrorCode;
V_MEMRAM0 extern V_MEMRAM1      vuint16            V_MEMRAM2      errStatFblStates;
V_MEMRAM0 extern V_MEMRAM1      vuint8             V_MEMRAM2      errStatLastServiceId;
V_MEMRAM0 extern V_MEMRAM1      vuint8             V_MEMRAM2      errStatFlashDrvVersion[3u];
V_MEMRAM0 extern V_MEMRAM1      vuint16            V_MEMRAM2      errStatFlashDrvErrorCode;
V_MEMRAM0 extern V_MEMRAM1      tBlockDescriptor   V_MEMRAM2      errStatDescriptor;

/* Additional, GM specific variables for error status reporting */
V_MEMRAM0 extern V_MEMRAM1 vuint8 V_MEMRAM2 errStatTpError;
V_MEMRAM0 extern V_MEMRAM1 vuint8 V_MEMRAM2 errStatHaveDriver;
V_MEMRAM0 extern V_MEMRAM1 tFblAddress V_MEMRAM2 errStatAddress;
V_MEMRAM0 extern V_MEMRAM1 tDebugFileName V_MEMRAM2 errStatFileName;
V_MEMRAM0 extern V_MEMRAM1 tDebugLineNbr V_MEMRAM2 errStatLineNumber;
#endif

/***********************************************************************************************************************
 *  PLAUSIBILITY CHECKS
 **********************************************************************************************************************/

#if defined( FBL_ENABLE_SERVICE_PREHANDLER )
# error "Currently not supported."
#endif

#if !(defined(FBL_ENABLE_DATA_PROCESSING) || defined(FBL_DISABLE_DATA_PROCESSING))
# error "Error in FBL_CFG.H: Usage of data processing interface not defined"
#endif

#if ((!defined( FBL_WATCHDOG_OFF ) && !defined( FBL_WATCHDOG_ON )) || \
     (defined( FBL_WATCHDOG_OFF ) && defined( FBL_WATCHDOG_ON )))
# error "Error in fbl_cfg.h: Missing or incorrect usage of watchdog"
#endif

#if !defined( FBL_WATCHDOG_TIME )
# error "Error in fbl_cfg.h: Usage of fbl-watchdog-time not specified"
#endif

#if ((!defined( FBL_ENABLE_DEBUG_STATUS ) && !defined( FBL_DISABLE_DEBUG_STATUS )) || \
     (defined( FBL_ENABLE_DEBUG_STATUS ) && defined( FBL_DISABLE_DEBUG_STATUS )))
# error "Error in fbl_cfg.h: Missing or incorrect usage of debug-status"
#endif

#if ((!defined( FBL_ENABLE_SYSTEM_CHECK ) && !defined( FBL_DISABLE_SYSTEM_CHECK )) || \
     (defined( FBL_ENABLE_SYSTEM_CHECK ) && defined( FBL_DISABLE_SYSTEM_CHECK )))
# error "Error in fbl_cfg.h: Missing or incorrect usage of system-check"
#endif

#if ((!defined( FBL_ENABLE_APPL_TASK ) && !defined( FBL_DISABLE_APPL_TASK ))||\
     (defined( FBL_ENABLE_APPL_TASK ) && defined( FBL_DISABLE_APPL_TASK )))
# error "Error in fbl_cfg.h: Missing or incorrect usage of appl-task"
#endif

#if ((!defined( FBL_ENABLE_STAY_IN_BOOT ) && !defined( FBL_DISABLE_STAY_IN_BOOT )) || \
     (defined( FBL_ENABLE_STAY_IN_BOOT ) && defined( FBL_DISABLE_STAY_IN_BOOT )))
# error "Error in fbl_cfg.h: Missing or incorrect usage of stay-in-boot"
#endif

#if ((!defined( FBL_ENABLE_SLEEPMODE ) && !defined( FBL_DISABLE_SLEEPMODE )) || \
     (defined( FBL_ENABLE_SLEEPMODE ) && defined( FBL_DISABLE_SLEEPMODE )))
# error "Error in fbl_cfg.h: Missing or incorrect usage of sleep-mode"
#endif

#if (FBL_PROJECT_STATE == FBL_PRODUCTION)
# if defined( FBL_ENABLE_STAY_IN_BOOT )
#  error "Error in fbl_cfg.h: Stay-in-boot feature needs to be disabled for production."
# endif
#endif /* (FBL_PROJECT_STATE == FBL_PRODUCTION) */

#if ((!defined( FBL_ENABLE_ROM_INTEGRITY_CHECK ) && !defined( FBL_DISABLE_ROM_INTEGRITY_CHECK )) || \
      (defined( FBL_ENABLE_ROM_INTEGRITY_CHECK ) && defined( FBL_DISABLE_ROM_INTEGRITY_CHECK )))
# error "Error in fbl_cfg.h: Missing or incorrect usage of rom-integrity-check"
#endif

#if ((!defined( FBL_ENABLE_RAM_INTEGRITY_CHECK ) && !defined( FBL_DISABLE_RAM_INTEGRITY_CHECK )) || \
      (defined( FBL_ENABLE_RAM_INTEGRITY_CHECK ) && defined( FBL_DISABLE_RAM_INTEGRITY_CHECK )))
# error "Error in fbl_cfg.h: Missing or incorrect usage of ram-integrity-check"
#endif

#if ((!defined( FBL_ENABLE_SECURITY_ACCESS ) && !defined( FBL_DISABLE_SECURITY_ACCESS )) || \
     (defined( FBL_ENABLE_SECURITY_ACCESS ) && defined( FBL_DISABLE_SECURITY_ACCESS )))
# error "Error in fbl_cfg.h: Missing or incorrect usage of security-access"
#endif

#if ((!defined( FBL_ENABLE_MULTIPLE_MEM_DEVICES ) && !defined( FBL_DISABLE_MULTIPLE_MEM_DEVICES )) || \
      (defined( FBL_ENABLE_MULTIPLE_MEM_DEVICES ) && defined( FBL_DISABLE_MULTIPLE_MEM_DEVICES )))
# error "Error in fbl_cfg.h: Missing or incorrect usage of multiple-memory-devices"
#endif

#if ((!defined( FBL_ENABLE_GAP_FILL ) && !defined( FBL_DISABLE_GAP_FILL )) || \
     (defined( FBL_ENABLE_GAP_FILL ) && defined( FBL_DISABLE_GAP_FILL )))
# error "Error in fbl_cfg.h: Missing or incorrect usage of gap-fill"
#endif

#if !defined( FBL_ENABLE_FBL_START ) && !defined( FBL_DISABLE_FBL_START )
# error "Error in fbl_cfg.h: FBL-Start must be specified"
#endif

#if !defined( SWM_DATA_MAX_NOAR )
# error "Error in fbl_cfg.h: Usage of swm-data-max-noar not specified"
#endif

#if (SWM_DATA_MAX_NOAR < 2u) || (SWM_DATA_MAX_NOAR > 255u)
# error "Error in fbl_cfg.h: Unsupported value of SWM_DATA_MAX_NOAR"
#endif

#if !defined( FLASH_SIZE )
# error "Error in fbl_cfg.h: Usage of flash-size not specified"
#endif

#if (FLASH_SIZE < 1u)
# error "Error in fbl_cfg.h: Unsupported value of FLASH_SIZE"
#endif

#if !defined( FBL_DIAG_BUFFER_LENGTH )
# error "Error in fbl_cfg.h: Usage of fbl-diag-buffer-length not specified"
#endif

#if (FBL_DIAG_BUFFER_LENGTH > 4095u)
# error "Error in fbl_cfg.h: Unsupported value of FBL_DIAG_BUFFER_LENGTH"
#endif

# if (!defined ( FBL_DIAG_ENABLE_GM_REQUEST_SEED_OTA ) && !defined( FBL_DIAG_DISABLE_GM_REQUEST_SEED_OTA )) || \
    (defined( FBL_DIAG_ENABLE_GM_REQUEST_SEED_OTA ) && defined( FBL_DIAG_DISABLE_GM_REQUEST_SEED_OTA ))
#  error "Error: Missing or incorrect usage of switch FBL_DIAG_ENABLE_GM_REQUEST_SEED_OTA"
# endif

#if defined( FBL_ENABLE_SERVICE_POSTHANDLER )
#else
# error "Posthandler configuration is mandatory! Please filter for FBL_ENABLE_SERVICE_POSTHANDLER and enable FBL_ENABLE_SERVICE_POSTHANDLER."
#endif /* FBL_ENABLE_SERVICE_POSTHANDLER */

# if (!defined ( FBL_DIAG_ENABLE_GM_ROUTINE_CHECKPROGDEP ) && !defined( FBL_DIAG_DISABLE_GM_ROUTINE_CHECKPROGDEP )) || \
     (defined( FBL_DIAG_ENABLE_GM_ROUTINE_CHECKPROGDEP ) && defined( FBL_DIAG_DISABLE_GM_ROUTINE_CHECKPROGDEP ))
#  error "Error: Missing or incorrect usage of switch FBL_DIAG_ENABLE_GM_ROUTINE_CHECKPROGDEP"
# endif /* FBL_DIAG_ENABLE_GM_ROUTINE_CHECKPROGDEP */
# if (!defined ( FBL_DIAG_ENABLE_GM_ROUTINE_SETPROGSTATEFLAG ) && !defined( FBL_DIAG_DISABLE_GM_ROUTINE_SETPROGSTATEFLAG )) || \
     (defined( FBL_DIAG_ENABLE_GM_ROUTINE_SETPROGSTATEFLAG ) && defined( FBL_DIAG_DISABLE_GM_ROUTINE_SETPROGSTATEFLAG ))
#  error "Error: Missing or incorrect usage of switch FBL_DIAG_ENABLE_GM_ROUTINE_SETPROGSTATEFLAG"
# endif /* FBL_DIAG_ENABLE_GM_ROUTINE_SETPROGSTATEFLAG */
# if (!defined ( FBL_DIAG_ENABLE_GM_ROUTINE_INITEXTREFLASH ) && !defined( FBL_DIAG_DISABLE_GM_ROUTINE_INITEXTREFLASH )) || \
     (defined( FBL_DIAG_ENABLE_GM_ROUTINE_INITEXTREFLASH ) && defined( FBL_DIAG_DISABLE_GM_ROUTINE_INITEXTREFLASH ))
#  error "Error: Missing or incorrect usage of switch FBL_DIAG_ENABLE_GM_ROUTINE_INITEXTREFLASH"
# endif /* FBL_DIAG_ENABLE_GM_ROUTINE_INITEXTREFLASH */

#if !defined( V_GEN_GENERATOR_MSR )
# if !defined( FBL_CW_ENABLE_RECEPTION_IN_STATE_TASK )
#  error "FBL_CW_ENABLE_RECEPTION_IN_STATE_TASK must be defined in order to allow for correct pipelining action."
# endif /* FBL_CW_ENABLE_RECEPTION_IN_STATE_TASK */
#endif /* V_GEN_GENERATOR_MSR */

/* Diagnostic buffer size configuration is required to be equal to Tp buffer size configuration. */
#if (FBL_DIAG_BUFFER_LENGTH != kFblTpBufferSize)
# error "Diagnostic and Tp buffer size configuration do not match."
#endif

#endif /* __FBL_DIAG_OEM_H__ */
/***********************************************************************************************************************
 *  END OF FILE: FBL_DIAGOEM.H
 **********************************************************************************************************************/

