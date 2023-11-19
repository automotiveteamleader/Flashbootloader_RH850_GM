/* Kernbauer Version: 1.12 Konfiguration: FBL Erzeugungsgangnummer: 1 */

/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief         Application dependent routines
 *
 *  \note          Please note, that this file contains a collection of callback functions to be used with the
 *                 Flash Bootloader. These functions may influence the behavior of the bootloader in principle.
 *                 Therefore, great care must be taken to verify the correctness of the implementation.
 *                 The contents of the originally delivered files are only examples resp. implementation proposals.
 *                 With regard to the fact that these functions are meant for demonstration purposes only, Vector
 *                 Informatik's liability shall be expressly excluded in cases of ordinary negligence, to the extent
 *                 admissible by law or statute.
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
 *  Sebastian Loos                Shs           Vector Informatik GmbH
 *  Thomas Mueller                ThM           Vector Informatik GmbH
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY (KbFbl_apFrameOem.c, GM SLP5 and SLP6)
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  05.00.00   2014-01-31  AWh     -                First release
 *  05.00.01   2015-06-12  AWh     -                Update header information
 *  05.00.02   2015-06-26  AWh     -                No changes
 *  05.01.00   2016-03-16  Shs     ESCAN00088938    Added support for PATAC SLP2
 *  05.02.00   2016-04-16  Shs     ESCAN00089600    Added support for CommonData structure
 *  05.03.00   2016-05-03  Shs     ESCAN00089864    Added FblGetComDat_VerifySignatureFct() to Common Data
 *  05.04.00   2016-05-31  Shs     ESCAN00090230    MemMap Defines for CommonData Structure
 *             2016-06-30  Shs     ESCAN00090791    No changes
 *  05.05.00   2016-07-25  AWh     ESCAN00091152    Added Support for Non Kb Main, allow Msr Can driver
 *  05.06.00   2016-11-08  ThM     ESCAN00092691    Added compression configuration checks
 *  05.06.01   2016-12-01  Shs     ESCAN00093124    No changes
 *  05.06.02   2017-06-07  AWh     ESCAN00095463    No changes
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY (kbFbl_apAPI.c)
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  00.90.00   2003-12-10  CB                       First implementation
 *  00.91.00   2004-01-08  CB                       Decryption/decompression interface changed
 *                         CB                       Parameters for ApplFblVerifyChecksum changed
 *  01.00.00   2004-02-24  CB                       Release
 *  01.01.00   2004-03-24  Rr                       Added new functions
 *  01.02.00   2004-03-25  WM                       Minor changes and enhancements for new OEM
 *  01.03.00   2004-04-23  WM                       Added watchdog trigger functions
 *                         ACP                      Added ApplFblSecurityStatus
 *                         AMN                      Added ApplFblIsValidFlashBlock, and ApplFblReadBlock
 *  01.04.00   2004-05-07  CB                       Added ApplCanBusOff, ApplCanWakeUp
 *  01.05.00   2004-05-19  CB                       Minor changes
 *  01.06.00   2004-06-30  MHz                      No changes
 *  01.07.00   2004-07-22  CB                       Added ApplFblReadEcuIdentification,
 *                                                  ApplFblStoreTesterCode, ApplFblStoreProgrammingDate
 *  01.08.00   2004-08-06  WM                       Added ApplFblRWSecurityAccessDelayFlag
 *  01.09.00   2004-09-09  Rr                       Minor changes for ApplFblCanBusOff
 *  01.10.00   2004-09-16  WM                       Added PRE and POST section for ApplFblWDTrigger
 *  01.11.00   2004-10-25  AP      ESCAN00010007    Adaption to support Vector
 *                                                  component OSEK NM
 *  01.12.00   2004-12-23  AMN                      Changed prototype of ApplFblWriteDataById
 *                                                  Added an ifdef to ApplFblCanParamInit
 *                                                  Added function ApplFblPBLConfigSet
 *                                                  Corrected the paramters in ApplFblRWProgCounter
 *                                                  Define name changes PBL->CAN
 *                         ACP                      Version check reworked
 *                         CB                       Added fingerprint functions
 *  01.13.00   2005-01-14  CB                       Added several API functions
 *  01.14.00   2005-03-22  WM      ESCAN00011595    MISRA warnings
 *  01.15.00   2005-09-29  WM                       _PRE_ and _POST_ section for ApplFblWDTriggerEnd
 *                                                  V_VER.H consistency check added
 *  01.16.00   2005-12-08  WM      ESCAN00014594    FblApSrcApplFblTpParamInit token
 *                                                  added for TPMC
 *  01.17.00   2006-02-28  AMN                      Added ApplFblCopyToRAM and ApplFblIsValidAppInRAM
 *  01.18.00   2006-03-24  QPs                      Added ApplFblGetSubNodeRxCanId for Gateway
 *                                                  implementation
 *  01.19.00   2006-04-25  CB      ESCAN00015917    Return value added for
 *                                                  ApplFblInvalidateBlock
 *                         WM      ESCAN00015960    ApplFblVerifySegmentChecksum added
 *                         Ls                       added KBTok_FblApSrcApplFblCheckProgDependencies
 *                         QPs     ESCAN00016110    Added ApplCanBusOff for the Gateway
 *                         CB      ESCAN00016144    Return value added for
 *                                                  ApplFblInitDecryption
 *                         CB      ESCAN00016147    ApplFblStoreBootSwIdent added
 *                                 ESCAN00016171    ApplFblSetSecAccessDelayFlag,
 *                                                  ApplFblGetSecAccessDelayFlag and
 *                                                  ApplFblClrSecAccessDelayFlag added
 *  01.20.00   2006-05-18  ACP     ESCAN00016438    Added kStartupPreInit/PostInit
 *                                                  definition for new ApplFblStartup() parameter
 *  01.21.00   2006-06-29  FHE                      No changes
 *  01.22.00   2006-07-24  JHg                      Added V_CALLBACK_NEAR to watchdog trigger
 *                         CB      ESCAN00017003    Added ApplFblProgrammingStateChanged
 *                         AMN                      Added ApplFblValidateFile
 *                                                  Added a token to reference local prototypes
 *                                                  from the OEM layer
 *  01.23.00   2006-08-28  JHg                      Added token for additional watchdog functions
 *                         AMN     ESCAN00017479    Modified the API of the
 *                                                  ApplFblValidateApp for Misra compliance
 *                                                  Removed ApplFblIsValidFlashBlock because it's
 *                                                  not used anymore
 *  01.24.00   2006-12-06  FHE     ESCAN00018639    Added ApplFblCheckConditions
 *  01.25.00   2007-02-07  Hp      ESCAN00019129    Add support for CCP in the bootloader
 *                                                  Add PreHandler and presence-pattern support.
 *  01.26.00   2007-03-30  WM      ESCAN00020175    Added ApplFblCheckProgPreconditions
 *  01.27.00   2007-07-06  JHg                      Return value added to ApplFblDecryption and
 *                                                  ApplFblDeinitDecryption
 *  01.28.00   2007-07-27  Ls      ESCAN00022542    FBL_ENABLE_PRE_WDINIT_COMMENT added
 *  01.29.00   2008-03-07  Ci      ESCAN00014726    FBL_COPY_CODE_TO_RAM_ON_STARTUP
 *                                                  renamed to FBL_ENABLE_COPY..
 *                         Hp      ESCAN00014726    Support for memmap and FBL_ENABLE_COPY_CODE_TO_RAM_ON_STARTUP
 *                                                  to map ApplFblWDTrigger and ApplFblWDTriggerEnd to special location
 *                         JHg                      Update CAN driver Tx confirmation routines to RI 1.4 API
 *                         Hp      ESCAN00025029    Support Vector decompression module (optional)
 *  01.30.00   2008-03-28  JHg     ESCAN00025354    Remove COMMENT restriction from FBL_ENABLE_PRE_WDINIT
 *                                                  If enabled watchdog will be already initalized in ApplFblInit
 *                                 ESCAN00025355    FBL_ENABLE_PRE_TIMERINIT added
 *                                                  If enabled timer will be already initalized in ApplFblInit
 *  01.31.00   2008-06-13  Ci      ESCAN00025071    Extended function description of
 *                                                  ApplFblStartup() with kStartupStayInBoot
 *                                 ESCAN00027445    Added support for configurations
 *                                                  with multiple CAN channels
 *  01.32.00   2008-09-10  JHg     ESCAN00029860    Moved PRE_WDINIT and PRE_TIMERINIT to fbl_main
 *  01.33.00   2008-10-15  FHe     ESCAN00030640    Added support for VAG spec 1.2.1
 *  01.34.00   2009-03-13  JHg     ESCAN00032180    Added support for multichannel configurations
 *                                                  to ApplCanTxObjStart, ApplCanTxObjConfirmed
 *                                                  Updated description of ApplCanMsgReceived
 *                         FHe                      Restored old format of ApplFblCheckProgConditions
 *                         Ci      ESCAN00033475    Added gateway specific callback
 *                                                  functions (Gw_Route support)
 *  01.35.00   2009-03-12  QPs     ESCAN00033801    Added communication wrapper specific
 *                                                  callback functions
 *  01.36.00   2009-07-21  QPs     ESCAN00035282    Added DC SLP10 extension:
 *                                                  ApplFblControlFailSafeReaction
 *                         AWh     ESCAN00036825    Inconsistent return value type between
 *                                                  ReadDataByCommonId prototype and implementation
 *  01.37.00   2009-08-31  AWh     ESCAN00037405    No changes
 *                         QPs     ESCAN00037479    Added FrTrcv specific callback functions
 *  01.38.00   2009-09-18  Tbe     ESCAN00037856    Added Ethernet/DoIP specific
 *                                                  callback functions
 *  01.39.00   2009-12-02  Ci      ESCAN00038917    Adapted prototype of
 *                                                  ApplFblFatalError()
 *                                 ESCAN00038918    Added support for validation
 *                                                  struct handling
 *                                 ESCAN00038919    Added callback for flash erased
 *                                                  detection
 *  01.40.00   2010-02-15  Ci      ESCAN00040663    Enhanced flash erased detection
 *                                                  interface
 *                         Ci      ESCAN00040852    Replaced decryption interface by
 *                                                  data processing API
 *  01.41.00   2010-07-08  MVi     ESCAN00043736    Remove pragmas when memmap is used
 *                         Ci      ESCAN00043863    Added function ApplCanTpConfirmation
 *                         QPs     ESCAN00043896    Added support for LIN bootloader
 *  01.42.00   2010-07-23  CB      ESCAN00044299    Default implementation of decompression
 *  01.43.00   2010-09-15  Ci      ESCAN00044991    Added new Gw_Route API functions
 *  01.44.00   2010-12-10  MVi     ESCAN00047478    Enhanced flash erased detection interface
 *                                 ESCAN00047482    Added routine control succeeded interface
 *  01.45.00   2011-02-03  Ach     ESCAN00048191    Use normal fixed addressing without TPMC
 *  01.46.00   2011-07-14  Ci      ESCAN00052149    Adapted Gw_Route task interface
 *  01.47.00   2011-11-25  Hp      ESCAN00052787    Extend support for PresencePatterns
 *  01.48.00   2011-12-14  AWh     ESCAN00055586    Extend Api with ApplFblInvalidateApp function
 *  01.49.00   2012-02-14  Ach     ESCAN00056341    Added ApplFblStartApplication() function
 *  01.50.00   2012-02-22  Ach     ESCAN00052787    Extend support for PresencePatterns
 *                         MVi     ESCAN00056833    Extend support for PresencePatterns (HMC)
 *                                 ESCAN00056834    No changes
 *                         CB      ESCAN00056642    Added ApplFblCanPlatformToPhysChannel()
 *  01.51.00   2012-03-23  QPs     ESCAN00057230    Extend support for FlexRay
 *                         Ach     ESCAN00057636    Remove local memSegment variables
 *                         CB      ESCAN00057670    Added Suspend Programming API
 *  01.52.00   2012-03-30  QPs     ESCAN00058040    No changes
 *  01.53.00   2012-08-16  MVi     ESCAN00060433    Support for PresencePatterns
 *                                                  with 256 byte segment size
 *  01.54.00   2012-12-06  AWh     ESCAN00061619    Moved presence pattern macros to header
 *                         Ach     ESCAN00062912    Adapted comments to use Doxygen
 *  01.55.00   2013-01-24  Ach     ESCAN00064406    Support return value IO_E_ERASED for presence pattern
 *                         JHg     ESCAN00064421    Added callback ApplFblOnTransitionSession
 *                                 ESCAN00064437    Added callback ApplFblStateTask
 *  01.56.00   2013-02-08  ASe     ESCAN00064889    Reworked comments
 *  01.57.00   2014-01-21  ASe     ESCAN00072931    Fixed compiler warning (HMC)
 *  01.58.00   2014-02-24  MVi     ESCAN00073549    Removed HMC exceptions to support new mtab header
 *  01.58.01   2014-04-03  ASe     ESCAN00074701    Fixed comments
 *  01.59.00   2014-07-16  QPs     ESCAN00077127    Extend support for LIN
 *  01.60.00   2014-08-25  Dod     ESCAN00078033    Add callbacks ApplFblMemPreWrite/Erase and ApplFblGetLogBlkHash
 *  01.61.00   2015-01-13  Ach     ESCAN00080481    Change visibility of ApplFblChkModulePresence()
 *  01.62.00   2015-05-12  CB      ESCAN00082959    Added functions to read and store the TP target address
 *  01.63.00   2015-05-27  CB      ESCAN00083167    Additional callback functions for status backup
 *  01.64.00   2015-09-14  Ci      ESCAN00084704    Removed data processing default implementation
 *                         QPs     ESCAN00085203    Added callback ApplFblCheckTesterSourceAddr
 *  01.64.01   2015-12-15  TnS     ESCAN00087100    Moved initialization for Timer Interrupt use case to FblWd
 *  01.65.00   2016-01-11  ASe     ESCAN00087324    Presence pattern handling fails on word-addressed platforms
 *                         Rr      ESCAN00087414    Extend Renault SLP3 specific API for reading light LogZone
 *  01.66.00   2016-02-25  TnS     ESCAN00088471    MISRA related code changes
 *  01.67.00   2016-03-07  MVi     ESCAN00088759    Added ApplFblIsValidBlock
 *  01.68.00   2016-03-17  Shs     ESCAN00088957    No changes
 *  01.69.00   2016-05-09  HRf     ESCAN00089907    Encapsulate FlexRay specific code in ApplFblCwWakeUp()
 *  01.70.00   2016-05-17  Rie     ESCAN00090027    Performed MISRA analysis
 *  01.71.00   2016-06-27  Ach     ESCAN00090683    Support configuration switch FBL_CW_ENABLE_MULTIPLE_NODES
 *  01.72.00   2016-07-15  DlM     ESCAN00090996    Added function for programming voltage check
 *  01.73.00   2016-08-02  Ach     ESCAN00091282    Added ApplFblGetVerificationData()
 *  01.74.00   2016-08-24  Rie     ESCAN00091646    Added ApplFblCheckFuncTargetAddr()
 *  01.75.00   2016-10-04  CEl     ESCAN00092168    Added ApplFblSetLogBlkHash()
 *  01.76.00   2016-12-01  Ach     ESCAN00093116    Added length parameter to ApplFblGetVerificationData()
 *  01.77.00   2017-02-07  Ach     ESCAN00093891    Added constant definition for common data structure
 *  01.77.01   2017-06-22  Rie     ESCAN00095595    Fixed evaluation of the presence pattern
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  REVISION HISTORY (KbFbl_apOem.c, GM SLP5 and SLP6)
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  05.00.00   2014-01-31  AWh                      First release
 *  05.01.00   2014-02-26  AWh     ESCAN00073863    Add user callbacks for Non readable Device use case
 *                                 ESCAN00073906    Enable Multi processor use case
 *  05.02.00   2014-07-14  AWh     ESCAN00075304    Compiler Error: Sleep-Mode enabled
 *                                                  SLP6: Add DID information available at Vector
 *                                                        Move DIDs from diagnostic layer to callback layer
 *  05.03.00   2014-10-15  AWh     ESCAN00078893    SLP6: Check for Default session in Core Diag style
 *                         AWh     ESCAN00078899    SLP6: Update ApplFblReadDataByIdentifier to allow for Core Diag
 *                         AWh     ESCAN00079661    Add compression interface
 *                         Dod     ESCAN00079983    SLP5: Incorrect reaction to securityAccess sendKey request
 *                         AWh     ESCAN00080211    SLP6: On transition from Appl, remove setting targetaddress,
 *                                                   which is redundantly done in ComWrapper
 *                                 -                Rework after Review
 *  05.04.00   2015-01-23  AWh     ESCAN00080768    No changes
 *  05.05.00   2015-05-06  Dod     ESCAN00082852    DID $F0 reports wrong programmed state for calibrations
 *  05.06.00   2015-06-12  AWh     ESCAN00083305    Add missing ApplFblNVMWrite macros/functions for Sba and EcuId
 *                                 ESCAN00082260    Gm SLP6: Wrong length of DID F0F0 response.
 *  05.07.00   2015-07-06  AWh     ESCAN00083824    The bootloader does not download a compressed application and
 *                                                    reports Programming Error code Err_LengthExceeded
 *                         AWh     ESCAN00078397    SLP5: Add Default Nrcs for ReadMemoryByAddress
 *  05.08.00   2015-09-18  AWh     ESCAN00085306    Checksum calculation is skipped through unaligned buffer.
 *  05.08.01   2015-11-13  AWh     ESCAN00086481    SLP5: Avoid problems with not matching Can Id Types within
 *                                                   ApplFblCanParamInit
 *  05.09.00   2016-02-22  AWh     ESCAN00088480    Add support for basic NBID handling
 *  05.10.00   2016-03-16  Shs     ESCAN00088940    Added support for PATAC SLP2
 *  05.10.01   2016-03-29  AWh     ESCAN00089097    Use MEMCPY wrapper instead of direct access to memcpy
 *  05.10.02   2016-04-05  AWh     ESCAN00089233    Compiler warning: parameter "buffer" unused in functions
 *                                                   ApplFblNVMReadSBATicket, ApplFblNVMWriteECUID
 *  05.11.00   2016-05-06  AWh     ESCAN00089345    Eeprom init superfluous in Basic NVM configuration
 *                                 ESCAN00089826    Allow FblDiag_SecHdr If 3.xx.xx
 *                         Shs     ESCAN00089536    Compiler warning: possible truncation at implicit conversion
 *                         AWh     ESCAN00089901    Allow for FBLDIAGHDR_GM_VERSION 3.xx.xx
 *                                 ESCAN00089536    Compiler warning possible truncation at implicit conversion
 *  05.11.01   2016-06-10  Shs     ESCAN00090427    Compiler error: identifier "PARTID_OPSW_LOCAL" is undefined
 *                                 ESCAN00090568    Changed datatype of kRamTable
 *  05.12.00   2016-06-30  Shs     ESCAN00090793    Added support for ComWrapper 2.xx
 *                                 ESCAN00090819    Changed declaration of didTable
 *  05.13.00   2016-07-25  AWh     ESCAN00091151    Added Support for XCP and Non Kb Main
 *                                 ESCAN00091250    Add support for MSR Can driver
 *                                 ESCAN00090093    Basic NVM NBID Testcode in ApplFblInit() to early
 *  05.14.00   2016-10-31  AWh     ESCAN00092368    SLP6: Offer ApplFblGetProgrammedState for FF 01 handling
 *                         Shs     ESCAN00090118    Compiler warning: Result of function-call is ignored
 *                                 ESCAN00090116    Compiler warning: Result of function-call is ignored
 *                         ThM     ESCAN00092600    Added support for Lzma decompression
 *                         Shs     ESCAN00092589    XCP: added Support for XCP in GM SLP5
 *                         Shs     ESCAN00092625    No changes
 *                         Shs     ESCAN00092711    ApplFblStartup( ) does not implement a case for kStartupStayInBoot
 *                         ThM     ESCAN00092753    No changes
 *  05.14.01   2016-11-17  Shs     ESCAN00092866    XCP: Changes of tReProgChannel in Common Data
 *  05.14.02   2016-11-24  AWh     ESCAN00092997    Fix some configuration options
 *                         Shs     ESCAN00093013    Compiler warning: possible truncation at implicit conversion to type "unsigned short int"
 *  05.14.03   2016-12-01  Shs     ESCAN00093126    No changes
 *  05.14.04   2017-02-02  AWh     ESCAN00093273    Compiler error: fblStartParameter not defined
 *                                 ESCAN00093845    LZMA Compression: Downloaded compressed calibration not accepted
 *                         Shs     ESCAN00094519    [MSR-CAN onyl, e.g. for CAN-FD] Stay In Boot (un-) supported
 *                         ThM     ESCAN00094674    Added assertion to ApplFblRamIntegrityCheck
 *                         AWh     ESCAN00094743    Add Legacy FblHeader IF for customer availability in case of update
 *                                 ESCAN00094744    ProgrammedStateIndicator: In case application is not valid, report
 *                                                   calibration partition to be invalid
 *  05.14.05   2017-05-11  Shs     ESCAN00094961    Compiler error: argument of type: missing cast to (vuint16 * ) 
 *                                                   for fblAndApplSharedRam.keyNbid
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY (RH850 KbFbl_apHW.C)
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  00.90.00   2012-12-18  CB                       First implementation
 *  01.00.00   2013-06-07  CB                       Release
 *  01.01.00   2015-10-21  JLe     ESCAN00086043    No changes
 *  01.02.00   2016-05-03  Ci      ESCAN00080515    Incorrect PLL initialization
 *  01.03.00   2016-05-23  CB      ESCAN00090133    Support for E1x
 *  01.04.00   2016-08-03  Shs     ESCAN00091046    Added support for D1x
 *                         DlM     ESCAN00091205    Added support for F1K
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

#include "fbl_inc.h"

#if (FBL_PROJECT_STATE >= FBL_INTEGRATION)
# if defined ( FBL_TEST_SBA_TICKET )
#include "dummySba.h"
# endif
#endif

/***********************************************************************************************************************
 *  VERSION
 **********************************************************************************************************************/

#if ( FBLKBAPI_FRAME_GM_VERSION != 0x0506u )
# error "Error in fbl_ap.c: Source and Header file are inconsistent!"
#endif
#if ( FBLKBAPI_FRAME_GM_RELEASE_VERSION != 0x02u )
# error "Error in fbl_ap.c: Source and Header file are inconsistent!"
#endif

#if ( FBLKBAPI_FRAME_GM_VERSION != _FBLKBAPI_FRAME_OEM_VERSION ) || \
    ( FBLKBAPI_FRAME_GM_RELEASE_VERSION != _FBLKBAPI_FRAME_OEM_RELEASE_VERSION )
# error "Error in fbl_ap.c: Source and v_ver.h are inconsistent!"
#endif

#if ( FBLKBAPI_GM_VERSION != 0x0514u )
# error "Error in fbl_apXX.c: Source and Header file are inconsistent!"
#endif
#if ( FBLKBAPI_GM_RELEASE_VERSION != 0x05u )
# error "Error in fbl_apXX.c: Source and Header file are inconsistent!"
#endif

#if ( FBLKBAPI_GM_VERSION != _FBLKBAPI_OEM_VERSION ) || \
    ( FBLKBAPI_GM_RELEASE_VERSION != _FBLKBAPI_OEM_RELEASE_VERSION )
# error "Error in fbl_ap.c: Source and v_ver.h are inconsistent!"
#endif

#if defined( FBL_ENABLE_COMPRESSION_MODE )
# if( FBLDIAGHDR_GM_VERSION < 0x200u )
#  error "Error in fbl_hdr.c: Version smaller than 2.00 does not have compression interface."
# endif
#endif

#if ( FBLKBAPI_RH850RSCAN_VERSION != 0x0104 ) || \
    ( FBLKBAPI_RH850RSCAN_RELEASE_VERSION != 0x00 )
#error "Error in FBL_APxx.C: Source and header file are inconsistent!"
#endif
#if ( FBLKBAPI_RH850RSCAN_VERSION != _FBLKBAPI_HW_VERSION ) || \
    ( FBLKBAPI_RH850RSCAN_RELEASE_VERSION != _FBLKBAPI_HW_RELEASE_VERSION )
#error "Error in FBL_APxx.C: Source and V_VER.H are inconsistent!"
#endif

#if ( FBLKBAPI_VERSION != 0x0177u ) || \
    ( FBLKBAPI_RELEASE_VERSION != 0x01u )
# error "Error in FBL_APxx.C: Source and header file are inconsistent!"
#endif
#if ( FBLKBAPI_VERSION != _FBLKBAPI_VERSION ) || \
    ( FBLKBAPI_RELEASE_VERSION != _FBLKBAPI_RELEASE_VERSION )
# error "Error in FBL_APxx.C: Source and V_VER.H are inconsistent!"
#endif

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

# if defined( FBL_AP_GM_ENABLE_LEGACY_APPLICATION_IF )
 /* Set FBL_AP_GM_ENABLE_LEGACY_APPLICATION_IF if you have an application that you cannot change to new required comdat.c application IF
  * and instead want to maintain legacy FblHeader interface
  * - for transition from application to bootloader
  * - for using security verification routines in application from bootloader
  *
  *   It is recommended to only use this, if really necessary. In this case configure the version of FblDef (fbl_def.h) you
  *   referenced in your application below and make sure the correct legacy header ApplFblHeader is offered.
  */

/*# define FBLDEF_PREVIOUS_DELIVERY_VERSION 0x300u */
#  if !defined( FBLDEF_PREVIOUS_DELIVERY_VERSION )
#   error "Please define FBLDEF_PREVIOUS_DELIVERY_VERSION to your fbl_def.h version used in your legacy application."
#  endif /* !FBLDEF_PREVIOUS_DELIVERY_VERSION */
# endif /* FBL_AP_GM_ENABLE_LEGACY_APPLICATION_IF */

/***********************************************************************************************************************
 *  TYPEDEFS
 **********************************************************************************************************************/

#if defined( FBL_ENABLE_RAM_INTEGRITY_CHECK )
typedef struct
{
   tFblAddress begin;
   tFblAddress end;
} tRamTable;
#endif

typedef enum
{
   STARTUP_NO_EXECUTION = 0u,
   STARTUP_EXECUTION
} tApplFblStartupExec;

# if defined( FBL_AP_GM_ENABLE_LEGACY_APPLICATION_IF )
/* CanInit-table specific pointer types */
typedef V_MEMRAM1_FAR vuint8 V_MEMRAM2_FAR V_MEMRAM3 * tApplFblSbatPtr;

/** CAN initialization data structure */
typedef struct
{
   vuint8 TpTargetAddress;             /**< Target address of tester */
   tApplFblSbatPtr pSbat;              /**< can be used to pass Sbat (Ram buffer) location to Fbl */
   tFblAddress extendedInfo;           /**< address to some extended Info */
} tApplCanInitTable;

typedef MEMORY_HUGE MEMORY_FAR void (* tApplFblStrtFct)(V_MEMRAM1 tApplCanInitTable V_MEMRAM2 V_MEMRAM3 *);

#  if defined( FBL_AP_GM_ENABLE_LEGACY_COMMON_DATA )
/* Pointer to shared ROM constants */
typedef V_MEMROM1_FAR void V_MEMROM2_FAR V_MEMROM3 * tApplFblCommonDataPtr;
#  endif /* FBL_AP_GM_ENABLE_LEGACY_COMMON_DATA */

typedef V_MEMROM1_FAR vuint8 V_MEMROM2_FAR V_MEMROM3 * tApplFblHeaderRomArrayPtr;
typedef struct
{
   tApplFblHeaderRomArrayPtr pData;
   vuint16 kLength;
} tApplFblSecKey;

typedef struct tagApplFblHeader
{
#  if (FBLDEF_PREVIOUS_DELIVERY_VERSION < 0x308)
   /* since FBLDEF_VERSION  0x0308u the below elements have been removed from FblHeader. */
   vuint8      CS[2];
   vuint8      MID[2];
   vuint8      ECUNAME[8];
   vuint8      ECUID[16];
   vuint8      SUBJNAME[16];
   vuint8      SWMI[4];
   vuint8      DLS[2];
   vuint8      DCID[2];
   vuint8      NOAR[2];
   vuint8      AddressRegions[FBL_GMHEADER_NOAR_FBL * 8];
#  endif /* FBLDEF_PREVIOUS_DELIVERY_VERSION < 0x308 */
   tApplFblStrtFct FblStartFct;
#  if (FBLDEF_PREVIOUS_DELIVERY_VERSION < 0x306)
   /* since FBLDEF_VERSION  0x0306u the below elements are removed. */
   tExportFct  pSecVerifySigFct;
   tApplFblSecKey  kSecSigRsaMod;
   tApplFblSecKey  kSecSigRsaExp;
#  endif  /* FBLDEF_PREVIOUS_DELIVERY_VERSION < 0x306 */
   vuint8      kFblMainVersion;
   vuint8      kFblSubVersion;
   vuint8      kFblBugFixVersion;
   vuint8      kFblBuildVersion;
#  if defined( FBL_AP_GM_ENABLE_LEGACY_COMMON_DATA )
   /* Set FBL_AP_GM_ENABLE_LEGACY_COMMON_DATA if you require pFblCommonData member. */
   tApplFblCommonDataPtr  pFblCommonData;
#  endif /* FBL_AP_GM_ENABLE_LEGACY_COMMON_DATA */
} tApplFblHeader;

# endif /* FBL_AP_GM_ENABLE_LEGACY_APPLICATION_IF */

/***********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

V_MEMRAM0 V_MEMRAM1 vuint8 V_MEMRAM2 cmprLstIdx;

#if defined( FBL_ENABLE_COMPRESSION_MODE )
V_MEMROM0 V_MEMROM1 tCmprInterfaceGm V_MEMROM2 cmprLst[] = {
# if defined( DATA_TYPE_COMPRESSED_ARLE )
   {
     CmprArleInit,
     FblHdrReadCmprHeader,
     CmprArleDecompress,
     V_NULL
   }
# endif
# if defined( DATA_TYPE_COMPRESSED_ARLE ) && defined( DATA_TYPE_COMPRESSED_LZMA )
   ,
# endif
# if defined( DATA_TYPE_COMPRESSED_LZMA )
   {
     CmprLzmaInit,
     FblHdrReadCmprHeader,
     ApplFblCtrlDecompress,
     CmprLzmaDeinit
   }
# endif
# if defined( DATA_TYPE_COMPRESSED_USER )
#  error "Please addd user conpression API"
# endif
};
#endif /* FBL_ENABLE_COMPRESSION_MODE */

# if defined( FBL_AP_GM_ENABLE_LEGACY_APPLICATION_IF )
#  if defined( FBL_ENABLE_FBL_START )
void ApplFblStart( tApplCanInitTable *pCanInitTable );
#  endif /* FBL_ENABLE_FBL_START */

#  define FBLAP_LEGACY_FBLHEADER_START_SEC_CONST
#  include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
V_MEMROM0 V_MEMROM1 tApplFblHeader V_MEMROM2 ApplFblHeader =
{
#  if (FBLDEF_PREVIOUS_DELIVERY_VERSION < 0x308)
   FBL_GMHEADER_CSUM,                     /**< Module Checksum                           */
   FBL_GMHEADER_MODID_FBL,                /**< Module ID                                 */
   FBL_GMHEADER_ECUNAME,                  /**< Ecu Name                                  */
   FBL_GMHEADER_ECUID,                    /**< Ecu Id                                    */
   FBL_GMHEADER_SUBJNAME,                 /**< Subject name                              */
   FBL_GMHEADER_SWMI_FBL,                 /**< SoftWare Module Identifier                */
   FBL_GMHEADER_DLS_FBL,                  /**< Design Level Suffix                       */
   FBL_GMHEADER_DCID_FBL,                 /**< Data Compatibility Identifier             */
   {0x00u, FBL_GMHEADER_NOAR_FBL},        /**< Number Of Address Regions                 */
   FBL_GMHEADER_ADDR_REG,                 /**< Product Memory Address + Number Of Bytes  */
#  endif
#  if defined( FBL_ENABLE_FBL_START )
   ApplFblStart,                          /**< FBL start function                        */
#  else
   V_NULL,
#  endif
#  if (FBLDEF_PREVIOUS_DELIVERY_VERSION < 0x306)
   (tExportFct)SecM_VerifySignature, /* PRQA S 0313 */ /* MD_FblDef_Export */
   {SecM_RsaMod, sizeof(SecM_RsaMod)},
   {SecM_RsaExp, sizeof(SecM_RsaExp)},
#  endif
   (vuint8)(FBLOEM_GM_VERSION >> 8u),      /**< FBL main version                          */
   (vuint8)(FBLOEM_GM_VERSION & 0xffu),    /**< FBL sub version                           */
   (vuint8)FBLOEM_GM_RELEASE_VERSION,      /**< FBL bugfix version                        */
#  ifdef _VECTOR_SIP_BUILD_VERSION
   (vuint8)_VECTOR_SIP_BUILD_VERSION      /**< build version from v_ver.h                */
#  else
   (vuint8)FBL_FLASH_DELETED              /**< Align data                                */
#  endif
#  if defined( FBL_AP_GM_ENABLE_LEGACY_COMMON_DATA )
   ,fblApLegacyCommonData /* Please provide this data, if required */
#  endif
};
#  define FBLAP_LEGACY_FBLHEADER_STOP_SEC_CONST
#  include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */
# endif /* FBL_AP_GM_ENABLE_LEGACY_APPLICATION_IF */

#if defined( FBL_COMDAT_VERSION )
   /* fblCommonData is now offered through comdat.c, to be linked to Fbl project. */
#else
# define FBLAP_COMMON_DATA_START_SEC_DATA
# include "MemMap.h"
#  if defined( FBL_ENABLE_COMMON_DATA )

/* Common data structure definition                                        *
 * This information can be linked to fix location or accessed indirectly   *
 * via fixed linked FblHeader. This allows to share data with the          *
 * application.                                                            */
V_MEMROM0 V_MEMROM1 tFblCommonData V_MEMROM2 fblCommonData =
{
   /* Public RSA key for signature verificatio */
   {
      { SecM_RsaMod, sizeof(SecM_RsaMod) },
      { SecM_RsaExp, sizeof(SecM_RsaExp) }
   },
   (tExportFct)SecM_VerifySignature
};
#  endif /* FBL_ENABLE_COMMON_DATA */
# define FBLAP_COMMON_DATA_STOP_SEC_DATA
# include "MemMap.h"
#endif /* FBL_COMDAT_VERSION */

/***********************************************************************************************************************
 *  LOCAL DATA
 **********************************************************************************************************************/

#if defined( FBL_ENABLE_VECTOR_HW )
static vuint16 flashlight;
#endif

/***********************************************************************************************************************
 *  EXTERNAL DATA
 **********************************************************************************************************************/

/* No modification of fblEcuHandle should be required. */
#  if defined( FBL_CW_ENABLE_MULTIPLE_NODES )
/* Handle which specifies the set of communication parameters to use */
V_MEMRAM0 V_MEMRAM1 vuint8  V_MEMRAM2 fblEcuHandle;
#  endif /* FBL_CW_ENABLE_MULTIPLE_NODES */

/***********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/

# if defined( FBL_ENABLE_FBL_START ) && defined( FBL_AP_GM_ENABLE_LEGACY_APPLICATION_IF )
/***********************************************************************************************************************
 *  ApplFblStart
 **********************************************************************************************************************/
/*! \brief       Transition from application into flash boot loader
 *  \pre         Valid application program running
 *  \param[in]   pCanInitTable CanInitTable pointer with CAN initialization data
 **********************************************************************************************************************/
void ApplFblStart( tApplCanInitTable * pCanInitTable ) /* PRQA S 3673 */ /* MD_FblMain_3673 */
{
   /* PRQA S 1006, 3109, 3200, 3335 1 */ /* MD_FblMain_Portability */
   FblInterruptDisable();

   FblSetComDatRam_TargetAddr(pCanInitTable->TpTargetAddress);
   fblCommonDataAccessRam.pSbat = pCanInitTable->pSbat;

  /* If pCanInitTable->extendedInfo was used previously to report the programming channel, set
   * fblCommonDataAccessRam.reProgChannel = pCanInitTable->extendedInfo; */
   fblCommonDataAccessRam.reProgChannel = FBL_REPR_CH_UDS;

   /* Set FblStart magic flag */
   /* Please pay attention: The FblStart magic flag does not use     *
    * a shared memory area so that the buffer may overlap with       *
    * the application stack.                                         */
   FblSetFblStartMagicFlag(); /* PRQA S 3109 */ /* MD_CBD_14.3 */

   /* Generate reset */
   APPL_FBL_RESET();
}

# endif /* FBL_ENABLE_FBL_START  && FBL_AP_GM_ENABLE_LEGACY_APPLICATION_IF */

#if defined( DATA_TYPE_COMPRESSED_LZMA )
/***********************************************************************************************************************
 *  ApplFblCtrlDecompress
 **********************************************************************************************************************/
/*! \brief         This function wraps calls to Vector standard (de-)compression Api.
 *  \detail        Standard compression modules do not use the threshold parameter required for the Gm use case.
 *                 Do the threshold handling in this wrapper and forward standard decompression job to compression
 *                 module. Data shall only be output when surpassing threshold.
 *                 Also make sure CmprLzmaDecompress is called as often as required.
 *  \param[in,out] procParam Processing parameter data structure
 *  \param[in]     outThreshold data shall only be output when surpassing threshold
 *  \return        kFblOk/kFblFailed
 **********************************************************************************************************************/
tFblResult ApplFblCtrlDecompress( V_MEMRAM1 tProcParam V_MEMRAM2 V_MEMRAM3 * procParam, vuint16 outThreshold )
{
   tFblResult result = kFblFailed;
   vuint16 minRequiredOut;
   tProcParam tmpProc = *procParam;
   vuint16 inDataLen = procParam->dataLength;

   if (MODULE_DF_COMPR_HDR == procParam->mode)
   {
      /* minimum required output while parsing header */
      minRequiredOut = RAM_HEADER_LENGTH;
   }
   else
   {
      if (outThreshold > 0u)
      {
         minRequiredOut = outThreshold;
      }
      else
      {
         minRequiredOut = 1u;
      }
   }

   procParam->dataLength = 0u;
   procParam->dataOutLength = 0u;

   while (minRequiredOut > procParam->dataOutLength)
   {
      result = CmprLzmaDecompress( &tmpProc );

      /* (( 0u == tmpProc.dataLength) && ( 0u == tmpProc.dataOutLength)) means nothing is produced. */
      if ( (kFblFailed == result) || ((0u == tmpProc.dataLength) && (0u == tmpProc.dataOutLength)) )
      {
         result = kFblFailed;
         break;
      }

      /* Fetch output parameters */
      procParam->dataOutLength += tmpProc.dataOutLength;
      procParam->dataLength += tmpProc.dataLength;

      /* Prepare buffer data parameters for next round */
      tmpProc.dataBuffer = &(procParam->dataBuffer[procParam->dataLength]);
      tmpProc.dataOutBuffer = &(procParam->dataOutBuffer[procParam->dataOutLength]);
      tmpProc.dataOutMaxLength = procParam->dataOutMaxLength - procParam->dataOutLength;

      /* Prepare next tmpProc.dataLength for input parameter */
      tmpProc.dataLength = inDataLen - tmpProc.dataLength;
   }

   if( (0u != outThreshold) && (kFblFailed != result) )
   {
      procParam->dataOutLength -= outThreshold;
      procParam->dataOutMaxLength -= outThreshold;
      MEMCPY(procParam->dataOutBuffer, &procParam->dataOutBuffer[outThreshold], procParam->dataOutLength);
   }
   return result;
}
#endif /* DATA_TYPE_COMPRESSED_LZMA */

#if defined( FBL_ENABLE_DEBUG_STATUS )
/***********************************************************************************************************************
 *  ApplFblInitErrStatus
 **********************************************************************************************************************/
/*! \brief       Initialize error status information.
 **********************************************************************************************************************/
void ApplFblInitErrStatus( void )
{
   errStatFlashDrvVersion[0] = 0;
   errStatFlashDrvVersion[1] = 0;
   errStatFlashDrvVersion[2] = 0;
   errStatFlashDrvErrorCode = 0;
   errStatErrorCode = 0;
   errStatFblStates = 0;
   errStatLastServiceId = 0;
   errStatTpError = (vuint8)0;
   errStatFileName = (const vuint8 *)0;
   errStatLineNumber = (vuint16)0;
}
#endif

/***********************************************************************************************************************
 *  ApplFblInit
 **********************************************************************************************************************/
/*! \brief       Function is called before any other FBL initialization
 **********************************************************************************************************************/
void ApplFblInit( void )
{
   /* 16 MHz Main Osc --> 160 MHz PLL */
   /* PLL is configured to constant multiplier of 10 */
   FBL_PROT_WRITE1(FBL_CKSC0CTL, 0x02ul);    /* MainOSC is selected as PLL input clock */
   while (FBL_CKSC0STAT != 0x03ul);

   /* PLL > CPU clock */
   FBL_PROT_WRITE1(FBL_CKSC0DIV, 0x03u);
   while (FBL_CLKD0STAT != 0x03ul);

#if defined( FBL_ENABLE_VECTOR_HW )

   /* Initialize MCU_TOD for power supply */
   FBL_P(0)      |=   (1 <<  13);               /* Set Pin 0.13 to High */
   FBL_PM(0)     &=   FblInvert16Bit(1 <<  13); /* Set Pin 0.13 as output */

   /* Initialize CAN I/O Ports */
   /* Channel 0 Rx P2_0 (109)*/
   FBL_PFCAE2    &=   FblInvert16Bit(1 << 0); /* Clear bit 0 of PFCAE0 */
   FBL_PFCE2     &=   FblInvert16Bit(1 << 0); /* Clear bit 0 in PFCE0 for CAN mode */
   FBL_PFC2      &=   FblInvert16Bit(1 << 0); /* Set bit 0 to select CAN mode */
   FBL_PM2       |=                 (1 << 0); /* Set Pin 0 as input */
   FBL_PMC2      |=                 (1 << 0); /* Enable alternative port function */

   /* Channel 0 Tx P2_1 (110) */
   FBL_PFCAE2    &=   FblInvert16Bit(1 << 1); /* Clear bit 1 of PFCAE0 */
   FBL_PFCE2     &=   FblInvert16Bit(1 << 1); /* Clear bit 1 in PFCE0 for CAN mode */
   FBL_PFC2      &=   FblInvert16Bit(1 << 1); /* Set bit 1 to select CAN mode */
   FBL_PM2       &=   FblInvert16Bit(1 << 1); /* Set Pin 1 as output */
   FBL_PMC2      |=                 (1 << 1); /* Enable alternative port function */

   /* LEDs test pins (VEBN00986, EvalBoard P1M) */
   FBL_P(5)      &=   FblInvert16Bit(1 <<  8); /* Set Pin 5.8 to low */
   FBL_PM(5)     &=   FblInvert16Bit(1 <<  8); /* Set Pin 5.8 as output */
   FBL_PMC(5)    &=   FblInvert16Bit(1 <<  8); /* Set Pin 5.8 as output */
   FBL_P(0)      &=   FblInvert16Bit(1 <<  2); /* Set Pin 0.2 to low */
   FBL_PM(0)     &=   FblInvert16Bit(1 <<  2); /* Set Pin 0.2 as output */
   FBL_P(0)      &=   FblInvert16Bit(1 << 13); /* Set Pin 0.13 to low */
   FBL_PM(0)     &=   FblInvert16Bit(1 << 13); /* Set Pin 0.13 as output */
   FBL_P(1)      &=   FblInvert16Bit(1 <<  1); /* Set Pin 1.1 to low */
   FBL_PM(1)     &=   FblInvert16Bit(1 <<  1); /* Set Pin 1.1 as output */
   
   /* Watchdog Testpin */
   FBL_P(5)      &=   FblInvert16Bit(1 <<  10); /* Set Pin 5.10 to low */
   FBL_PM(5)     &=   FblInvert16Bit(1 <<  10); /* Set Pin 5.10 as output */


   /* Initialize CSIH2 Ports */
   /* P3_4 - CSIH2CSS2 */
   FBL_PFCAE(3)    &=   FblInvert16Bit(1 << 4);   /* Clear bit 4 - Alternative output mode 2 */
   FBL_PFCE(3)     &=   FblInvert16Bit(1 << 4);   /* Clear bit 4 - Alternative output mode 2 */
   FBL_PFC(3)      |=                 (1 << 4);   /* Set bit 4   - Alternative output mode 2 */
   FBL_PM(3)       &=   FblInvert16Bit(1 << 4);   /* Clear Pin 3.4 as output */
   FBL_PMC(3)      |=                 (1 << 4);   /* Enable alternative port function */

   /* P1_4 - CSIH2SCO */
   FBL_PFCAE1    &=   FblInvert16Bit(1 << 4);   /* Clear bit 4 - Alternative output mode 4 */
   FBL_PFCE1     |=                 (1 << 4);   /* Set bit 4   - Alternative output mode 4 */
   FBL_PFC1      |=                 (1 << 4);   /* Set bit 4   - Alternative output mode 4 */
   FBL_PM1       &=   FblInvert16Bit(1 << 4);   /* Clear bit 4 - Alternative output mode 4  */
   FBL_PMC1      |=                 (1 << 4);   /* Enable alternative port function */

   /* P1_3 - CSIH2SO */
   FBL_PFCAE1    &=   FblInvert16Bit(1 << 3);   /* Clear bit 3 - Alternative output mode 4 */
   FBL_PFCE1     |=                 (1 << 3);   /* Clear bit 3 - Alternative output mode 4 */
   FBL_PFC1      |=                 (1 << 3);   /* Set bit 3   - Alternative output mode 4 */
   FBL_PM1       &=   FblInvert16Bit(1 << 3);   /* Clear bit 3 - Alternative output mode 4  */
   FBL_PMC1      |=                 (1 << 3);   /* Enable alternative port function */

   /* P1_2 - CSIH2SI */
   FBL_PFCAE1    &=   FblInvert16Bit(1 << 2);   /* Clear bit 2 - Alternative input mode 4 */
   FBL_PFCE1     |=                 (1 << 2);   /* Clear bit 2 - Alternative input mode 4 */
   FBL_PFC1      |=                 (1 << 2);   /* Set bit 2   - Alternative input mode 4 */
   FBL_PM1       |=                 (1 << 2);   /* Clear bit 2 - Alternative input mode 4 */
   FBL_PMC1      |=                 (1 << 2);   /* Enable alternative port function */
#endif

#if defined( FBL_HDR_ENABLE_BASIC_NVM_HANDLING )
   /* Basic Nv driver init happens in fbl_hdr.c */
#else /* Basic NVM disabled */
   /*Initialize Fee */
   (void) ApplFblNvInit();
# if defined( FBL_ENABLE_WRAPPER_NV )
   /* Remove this, if you do not use Eeprom */
   //(void) EepromDriver_InitSync(V_NULL);
# endif
#endif /* FBL_HDR_ENABLE_BASIC_NVM_HANDLING */

/* The following elements FBL_TEST_... can be used during development for test purpose */
#if ( FBL_PROJECT_STATE >= FBL_INTEGRATION )
# if defined( FBL_TEST_SBA_TICKET )
   /* This can be used for test purpose to Write SBA ticket from flash sbaBlk0 to NVM */
   (void)ApplFblNVMWriteSBATicket((vuint8 *)sbaBlk0);
# endif
# if defined( FBL_TEST_ECU_ID )
   /* This can be used for test purpose to Write Ecu-Id.
    * Note: FblGetComDat_EcuIdAddr() could be used to fullfill the requirement, if the
    * content is modified in the hexfile for each controller part to be unique.
    */
   (void)ApplFblNVMWriteECUID((vuint8 *)FblGetComDat_EcuIdAddr());
# endif
# if defined( FBL_TEST_KEY_NBID )
   {
      tNBIDInfo dummyKeyNbidInfo;
      dummyKeyNbidInfo.received = 0x0000;
      /* This can be used for test purpose to Write initial KEY_NBID */
      (void)ApplFblNVMWriteKeyNBID(&dummyKeyNbidInfo);
   }
# endif
# if defined( FBL_TEST_APP_NBID )
   {
      tNBIDInfo dummyAppNbidInfo;
      dummyAppNbidInfo.received = 0x0000;
      /* This can be used for test purpose to Write initial APP_NBID */
      (void)ApplFblNVMWriteAppNBID(&dummyAppNbidInfo);
   }
# endif
#endif
}

/***********************************************************************************************************************
 *  ApplFblStartup
 **********************************************************************************************************************/
/*! \brief       Bootloader initialization callback
 *  \details     This function is called during the Bootloader initialization if the application software is
 *               not started. It can be used to perform initializations which are not needed if the application
 *               software is started. Please note that the function is called several times with different
 *               initposition values. Timer and hardware can be accessed in post-init phase only.
 *  \param[in]   initposition Initialization state of Bootloader.
 **********************************************************************************************************************/
void ApplFblStartup( vuint8 initposition )
{
#if defined( FBL_ENABLE_ROM_INTEGRITY_CHECK ) || defined( FBL_ENABLE_RAM_INTEGRITY_CHECK )
   tApplFblStartupExec doMemcheck = STARTUP_NO_EXECUTION;
#endif /* FBL_ENABLE_ROM_INTEGRITY_CHECK || FBL_ENABLE_RAM_INTEGRITY_CHECK */
#if defined( FBL_TEST_KEY_NBID ) || defined( FBL_TEST_APP_NBID )
   tApplFblStartupExec doNbidTest = STARTUP_NO_EXECUTION;
#endif

   switch (initposition)
   {
      case (kFblInitPreCallback | kFblInitBaseInitialization):
      {
         break;
      }
      case (kFblInitPreCallback | kFblInitFblCommunication):
      {
         break;
      }
      case (kFblInitPreCallback | kFblInitFblFinalize):
      {
         break;
      }
      case (kFblInitPostCallback | kFblInitBaseInitialization):
      {
         /* Basic hardware initialization */
         ApplFblInit();
         break;
      }
      case (kFblInitPostCallback | kFblInitFblCommunication):
      {
#if defined( FBL_TEST_KEY_NBID ) || defined( FBL_TEST_APP_NBID )
         doNbidTest = (tApplFblStartupExec) FblMainGetStartFromReset(); /* Avoid test code execution if started from application */
#endif
         break;
      }
      case (kFblInitPostCallback | kFblInitFblFinalize):
      {
# if defined( FBL_ENABLE_ROM_INTEGRITY_CHECK ) || defined( FBL_ENABLE_RAM_INTEGRITY_CHECK )
            doMemcheck = (tApplFblStartupExec) FblMainGetStartFromReset(); /* Avoid check if started from application */
# endif /* FBL_ENABLE_ROM_INTEGRITY_CHECK || FBL_ENABLE_RAM_INTEGRITY_CHECK */
         break;
      }
      default:
      {
         assertFblInternal(0u, kFblSysAssertParameterOutOfRange);
         break;
      }
   }

/* The following elements FBL_TEST_... can be used during development for test purpose */
#if ( FBL_PROJECT_STATE >= FBL_INTEGRATION )
# if defined( FBL_TEST_KEY_NBID ) || defined( FBL_TEST_APP_NBID )
    if (STARTUP_EXECUTION == doNbidTest)
    {
#  if defined( FBL_HDR_ENABLE_BASIC_NVM_HANDLING )
       /* Early init NBID associated memory driver */
       (void) FblNbid_InitSync(V_NULL);
#  endif /* FBL_HDR_ENABLE_BASIC_NVM_HANDLING */

#  if defined( FBL_TEST_KEY_NBID )
       {
          tNBIDInfo dummyKeyNbidInfo;
          dummyKeyNbidInfo.received = 0x0000;
          /* This can be used for test purpose to Write initial KEY_NBID
           * Note that we write here after checking the SBAT, so for SBAT checking the new value is not used. */
          (void)ApplFblNVMWriteKeyNBID(&dummyKeyNbidInfo);
       }
#  endif /* FBL_TEST_KEY_NBID */
#  if defined( FBL_TEST_APP_NBID )
       {
          tNBIDInfo dummyAppNbidInfo;
          dummyAppNbidInfo.received = 0x0000;
#   if ( FBL_MTAB_NO_OF_BLOCKS > 1 )
          /* Add further code for MODID_OPSW_3, MODID_OPSW_4 if required */
          dummyAppNbidInfo.MID = MODID_OPSW_2;
          (void)ApplFblNVMWriteAppNBID(&dummyAppNbidInfo);
          dummyAppNbidInfo.MID = MODID_OPSW_1;
#   endif
          /* This can be used for test purpose to Write initial APP_NBID */
          (void)ApplFblNVMWriteAppNBID(&dummyAppNbidInfo);
       }
#  endif /* FBL_TEST_APP_NBID */

#  if defined( FBL_HDR_ENABLE_BASIC_NVM_HANDLING )
       /* Again deinit NBID associated memory driver */
       (void) FblNbid_DeinitSync(V_NULL);
#  endif /* FBL_HDR_ENABLE_BASIC_NVM_HANDLING */
    }
# endif /* defined( FBL_TEST_KEY_NBID ) || defined( FBL_TEST_APP_NBID ) */
#endif /* FBL_PROJECT_STATE >= FBL_INTEGRATION */

#if defined( FBL_ENABLE_ROM_INTEGRITY_CHECK ) || defined( FBL_ENABLE_RAM_INTEGRITY_CHECK )
    if (STARTUP_EXECUTION == doMemcheck)
    {
# if defined( FBL_ENABLE_ROM_INTEGRITY_CHECK )
       ApplFblRomIntegrityCheck();

       /* Update BIS information according to redundant fblProgrammedState flags */
       if ( kProgrammedStateRomError == (fblProgrammedState & kProgrammedStateRomError))
       {
          FblSetBisLsbByte(SBILSB_ERR_BOOTROMCHK);
       }
# endif /* FBL_ENABLE_ROM_INTEGRITY_CHECK */
# if defined( FBL_ENABLE_RAM_INTEGRITY_CHECK )
       ApplFblRamIntegrityCheck();
       /* Update BIS information according to redundant fblProgrammedState flags */
       if ( kProgrammedStateRamError == (fblProgrammedState & kProgrammedStateRamError))
       {
          FblSetBisLsbByte(SBILSB_ERR_RAMCHK);
       }
# endif /* FBL_ENABLE_RAM_INTEGRITY_CHECK */
    }
#endif /* FBL_ENABLE_ROM_INTEGRITY_CHECK || FBL_ENABLE_RAM_INTEGRITY_CHECK */
}

#if defined( FBL_ENABLE_MULTIPLE_NODES ) || \
    defined( FBL_ENABLE_CAN_CONFIGURATION) || \
    defined( FBL_CW_ENABLE_MULTIPLE_NODES )
/***********************************************************************************************************************
 *  ApplFblCanParamInit
 **********************************************************************************************************************/
/*! \brief       Overwrite default CAN parameters
 *  \details     This function is called before CAN controller initialization so that default initialization values
 *               can be overridden according to the boot mode.
 *               If this function is empty, the default/reset settings are used to initialize the CAN controller.
 *  \pre         fblCanIdTable has to be initialized with default/reset
 **********************************************************************************************************************/
void ApplFblCanParamInit( void )
{
# if defined( FBL_CW_ENABLE_MULTIPLE_NODES ) ||\
     defined( FBL_ENABLE_MULTIPLE_NODES )
   /* Set fblEcuHandle with desired ECU Id based on I/O */
   /* Be careful that this matches the Operating S/W */
   fblEcuHandle = 0u;
# endif /* ENABLE_MULTIPLE_NODES */
}
#endif

#if defined( FBL_USE_FBL_AS_DEMOAPPL )
#else
/***********************************************************************************************************************
 *  ApplFblStartApplication
 **********************************************************************************************************************/
/*! \brief       This function is called to start the application.
 *  \details     Please note: The function will never return because control is handed over to the application.
 *  \pre         Application validity has been checked, Application is valid
 **********************************************************************************************************************/
void ApplFblStartApplication( void )
{
   /* Start application */
   JSR_APPL();

#if defined( FBL_ENABLE_SYSTEM_CHECK )
   /* Code should never be reached */
   while (1)
   {
      ;
   }
#endif /* FBL_ENABLE_SYSTEM_CHECK */
}
#endif

/***********************************************************************************************************************
 *  ApplFblTpErrorInd - GM Specific
 **********************************************************************************************************************/
/*! \brief       Sends negative-response for TP Layer errors
 *  \param[in]   tpErrorCode Indicates cause of TP error.
 **********************************************************************************************************************/
void ApplFblTpErrorInd( vuint8 tpErrorCode )
{
   /* Consider what happens if called for TP tx error */
   /* Consider what happens if TP timeout due to loss of connection (attempt to tx will fail) */

   DiagNRCGeneralProgrammingFailure();
#if defined( FBL_ENABLE_DEBUG_STATUS )
   FblErrDebugStatus(kDiagErrTPFailed);
   errStatTpError = tpErrorCode;
#endif

   DiagProcessingDone(0);
}

/***********************************************************************************************************************
 *  ApplFblCheckProgConditions
 **********************************************************************************************************************/
/*! \brief       This function is called after receiving the service request sessionControl ProgrammingSession to check
 *               the programming conditions like reprogramming counter, ambient temperature, programming voltage, etc.
 *  \return      If all conditions are correct, the function returns kFblOk, otherwise kFblFailed.
 **********************************************************************************************************************/
tFblResult ApplFblCheckProgConditions( void )
{
   return (kFblOk);
}

/***********************************************************************************************************************
 *  ApplTrcvrNormalMode
 **********************************************************************************************************************/
/*! \brief       Activate transceiver if necessary (e.g. pin configuration ENABLE = 1, _STDBY = 1)
 **********************************************************************************************************************/
void ApplTrcvrNormalMode( void )
{
	/* Integrator TODO: Add code here to set the transceiver to normal mode */

#if !defined( FBL_USE_FBL_AS_DEMOAPPL )
	tFblResult result;
	result = kFblOk;
   
	/* Initialize TJA1145 CAN Transceiver */
	result = FblCanTrcvTja1145Init();

# if defined ( FBL_ENABLE_ASSERTION )
	if(kFblOk != result)
	{
		ApplFblFatalError(kFblAssertConditionCorrect);
	}
# endif /* FBL_ENABLE_ASSERTION */
	/* Place TJA1145 in Normal Mode */
	result = FblCanTrcvTja1145NormalMode();

# if defined ( FBL_ENABLE_ASSERTION )
	if(kFblOk != result)
	{
		ApplFblFatalError(kFblAssertConditionCorrect);
	}
# endif /* FBL_ENABLE_ASSERTION */
#endif /* FBL_USE_FBL_AS_DEMOAPPL */
}

/***********************************************************************************************************************
 *  ApplTrcvrSleepMode
 **********************************************************************************************************************/
/*! \brief       Turn off the transceiver to enter sleep mode (e.g. pin configuration ENABLE = 1, _STDBY = 0)
 **********************************************************************************************************************/
void ApplTrcvrSleepMode( void )
{
#if !defined( FBL_USE_FBL_AS_DEMOAPPL )
   tFblResult result = kFblOk;

   result = FblCanTrcvTja1145SleepMode();
# if defined( FBL_ENABLE_ASSERTION )
   if(kFblOk != result)
   {
      ApplFblFatalError(kFblAssertConditionCorrect);
   }
# endif
#endif
}

/***********************************************************************************************************************
 *  ApplFblSetVfp
 **********************************************************************************************************************/
/*! \brief       Function is called when the FBL enters programming mode.
 *  \details     Port bits have to be set so that the required programming voltage is available.
 **********************************************************************************************************************/
void ApplFblSetVfp( void )
{
   /* Turn on flash programming voltage (VFP) */
   /* FLMD0 Handling:
      The FLMD0 can be handled by internal pull up (default) or external pin.
      How the FLMD0 has to be handled depends on the external wiring/circuit.
      Please keep in mind that a connected NWIRE debugger or FP5 use/drive this wire, too.
      Please check the controller data sheet for further details
   */
   FBL_PROT_WRITE_FLMDCNT( 0x01u);
}

/***********************************************************************************************************************
 *  ApplFblResetVfp
 **********************************************************************************************************************/
/*! \brief       This function is called when the programming is done.
 **********************************************************************************************************************/
void ApplFblResetVfp( void )
{
   /* Turn off flash programming voltage (VFP) */
   FBL_PROT_WRITE_FLMDCNT( 0x00u);
}

/***********************************************************************************************************************
 *  ApplFblReset
 **********************************************************************************************************************/
/*! \brief       This function performs a reset.
 *  \details     The function will wait until the reset is performed. Please note: This function will
 *               never return to the calling code.
 **********************************************************************************************************************/
void ApplFblReset( void )
{
   /* Cause software reset */
   FBL_SW_RESET();
}

#if defined( FBL_ENABLE_APPL_TASK ) || \
    defined( FBL_ENABLE_APPL_TIMER_TASK )
/***********************************************************************************************************************
 *  ApplFblTask
 **********************************************************************************************************************/
/*! \brief       1ms user task.
 *  \details     This function is called every millisecond if the Bootloader is in idle state. It is not called during
 *               flash operations.
 **********************************************************************************************************************/
void ApplFblTask( void )
{
   flashlight++;
   if (flashlight > 500)
   {
      flashlight = 0;
      /* Test Message */
      /*FblCanTransmit(kFblTpTxHandle);*/
#if defined( FBL_ENABLE_VECTOR_HW )
      /* Toggle LED */
#endif
   }
}
#endif /* FBL_ENABLE_APPL_TASK || FBL_ENABLE_APPL_TIMER_TASK */

#if defined( FBL_DEF_ENABLE_NON_KB_MAIN )
#if defined( FBL_ENABLE_APPL_TASK ) ||\
    defined( FBL_ENABLE_APPL_STATE_TASK )
/***********************************************************************************************************************
 *  ApplFblStateTask
 **********************************************************************************************************************/
/*! \brief       Background user task.
 *  \details     This function is polled if the Bootloader is in idle state. It is not called during flash operations.
 **********************************************************************************************************************/
void ApplFblStateTask( void )
{
}
#endif /* FBL_ENABLE_APPL_TASK || FBL_ENABLE_APPL_STATE_TASK */
#endif /* FBL_DEF_ENABLE_NON_KB_MAIN */

#if defined( FBL_ENABLE_DATA_PROCESSING )
/***********************************************************************************************************************
 *  ApplFblCheckDataFormatIdentifier
 **********************************************************************************************************************/
/*! \brief       Return kFblOk for all valid data format identifiers.
 *  \param[in]   formatId Format Id from Request-Download service request.
 **********************************************************************************************************************/
tFblResult ApplFblCheckDataFormatIdentifier(vuint8 formatId)
{
   return kFblFailed;
}

/***********************************************************************************************************************
 *  ApplFblInitDataProcessing
 **********************************************************************************************************************/
/*! \brief         This function is called to initialize the application specific data processing function.
 *  \param[in,out] procParam Processing parameter data structure
 *  \return        kFblOk/kFblFailed
 **********************************************************************************************************************/
tFblResult ApplFblInitDataProcessing( tProcParam * procParam )
{
   tFblResult result = kFblFailed;

   /* No init to compression module on segment base. */
#if defined( FBL_ENABLE_COMPRESSION_MODE )
   if ( MODULE_DF_COMPR == (procParam->mode & MODULE_DF_COMPR) )
   {
      result = kFblOk;
   }
#endif
   return result;
}

/***********************************************************************************************************************
 *  ApplFblDataProcessing
 **********************************************************************************************************************/
/*! \brief         Data processing function.
 *  \pre           Data processing has to be initialized by call of ApplFblInitDataProcessing
 *  \param[in,out] procParam Processing parameter data structure
 *  \return        kFblOk/kFblFailed
 **********************************************************************************************************************/
tFblResult ApplFblDataProcessing( tProcParam * procParam )
{
   tFblResult result  = kFblFailed;
#if defined( FBL_ENABLE_COMPRESSION_MODE )
   vuint16 inMaxOut = procParam->dataOutMaxLength;
   vuint16 altMaxOut;

   assertFblInternal(rcvBuffHandle.progDataOffset <= HDR_MODULE_MAX_RAW_LEN, kFblSysAssertParameterOutOfRange);

   if ( MODULE_DF_COMPR == (procParam->mode & MODULE_DF_COMPR) )
   {
      /* either MODULE_DF_COMPR or MODULE_DF_COMPR_HDR is set here */

      if( MODULE_DF_COMPR_HDR != (procParam->mode & MODULE_DF_COMPR_HDR) )
      {
         /* Only consume bytes up to Segment border. */
         /* Add rcvBuffHandle.progDataOffset to guarantee that the header is consumed in first run. */
         altMaxOut = (vuint16) FblHdrGetDataInCurrSegment(procParam->dataOutMaxLength) + rcvBuffHandle.progDataOffset;

         if(altMaxOut < procParam->dataOutMaxLength )
         {
            procParam->dataOutMaxLength = altMaxOut;
         }
      }

      if (procParam->dataOutMaxLength > 0u)
      {
         /* Perform decompression */
         /* Decompression stops if input-buffer is empty or output-buffer is full */
         result = ApplFblDecompress(procParam, rcvBuffHandle.progDataOffset);

         if (kFblOk != result)
         {
            FblSetPecErr(HdrErr_Compression);
         }
         if (  ( MODULE_DF_COMPR == procParam->mode )
            || ( MODULE_DF_COMPR_LAST_SEGMENT == procParam->mode ) )
         {
            FblHdrConsumeCurrSegmentBytes(procParam->dataOutLength);
            rcvBuffHandle.buffIdx += procParam->dataLength;
            /* Always reset progDataOffset after first programmed data chunk ( which includes the header ) */
            rcvBuffHandle.progDataOffset = 0u;
         }
      }
      else
      {
         /* Consume all bytes passed in to allow segment switch above Libmem: All required data in current segment is consumed.  */
         result = kFblOk;
      }
      procParam->dataOutMaxLength = inMaxOut;
   }
#endif

   return result;
}

/***********************************************************************************************************************
 *  ApplFblDeinitDataProcessing
 **********************************************************************************************************************/
/*! \brief         Deinitialize data processing function.
 *  \pre           Data processing has to be initialized by call of ApplFblInitDataProcessing
 *  \param[in,out] procParam Processing parameter data structure
 *  \return        kFblOk/kFblFailed
 **********************************************************************************************************************/
tFblResult ApplFblDeinitDataProcessing( tProcParam * procParam )
{
   tFblResult result = kFblOk;

#if defined( FBL_ENABLE_COMPRESSION_MODE )
   if (ApplFblCmprDeinit != V_NULL)
   {
      if( MODULE_DF_COMPR_LAST_SEGMENT == procParam->mode )
      {
         result = ApplFblCmprDeinit(procParam);
      }
   }
#endif /* FBL_ENABLE_COMPRESSION_MODE */

   return result;
}
#endif /* FBL_ENABLE_DATA_PROCESSING */

# define DRVCAN_START_SEC_CODE
# include "memmap.h"   /* PRQA S 5087 *//* MD_MSR_19.1 */
/***********************************************************************************************************************
 *  ApplFblCanWakeUp
 **********************************************************************************************************************/
/*! \brief       This call-back function is called if a wake-up condition on the CAN bus is detected during sleep mode
 *               of the CAN controller.
 **********************************************************************************************************************/
void ApplFblCanWakeUp( void )
{
}
/***********************************************************************************************************************
 *  ApplFblCanBusOff
 **********************************************************************************************************************/
/*! \brief       This call-back function is called if the CAN controller enters bus-off state.
 **********************************************************************************************************************/
void ApplFblCanBusOff( void )
{
}

# define DRVCAN_STOP_SEC_CODE
# include "memmap.h"   /* PRQA S 5087 *//* MD_MSR_19.1 */

#if defined( FBL_ENABLE_ROM_INTEGRITY_CHECK )
/***********************************************************************************************************************
 *  ApplFblRomIntegrityCheck
 **********************************************************************************************************************/
/*! \brief       This routine is responsible for calculating the checksum of the bootloader and comparing it
 *               to the expected value stored in the FBL header. If the values do not match,
 *               state variable used by ApplFblReportProgrammedState must be updated to reflect the error.
 **********************************************************************************************************************/
void ApplFblRomIntegrityCheck( void )
{
   vuint16 checksum = 0u;
   vuint8 noar;
   vuint8 buffer[16u + 1u]; /* 16 used bytes, 1-byte preserved to always allow 2-byte alignment. */
   vuintx buffIdx = ((vuintx)buffer) & 0x01u; /* Index to 2-byte aligned address */

   /* check if checksum structure is located 16-Bit-aligned
    * This is a precondition for the ROM Integrity Checksum algorithm
    */
   assertFblInternal( (0u == (((vuint32)(& FblGetComDat_Checksum())) & 0x1u)), kFblSysAssertParameterOutOfRange);

   FblHdrInitFblRomCheck(&parsedModuleInfo.segmentList);

   /* Integration OPTION:
    * ------------------
    * Try to only use one region for Fbl Integrity check. If this is not possible,
    * add additional region in case Fbl is splitted into two regions manually here.
    * (The first GENy configured region is set inside FblHdrInitFblRomCheck())
    *
    * E.g.: Add 2nd region:
    * ----------------------------------------------------------------
    * parsedModuleInfo.segmentList.nrOfSegments = 2;
    * parsedModuleInfo.segmentList.segmentInfo[1].targetAddress = 0xYYYYYYYY;
    * parsedModuleInfo.segmentList.segmentInfo[1].length = 0xZZZZZZZZ;
    */

   /* Calculate over all address-regions */
   for (noar = 0u; noar < parsedModuleInfo.segmentList.nrOfSegments; noar++)
   {
      tFblAddress addr = parsedModuleInfo.segmentList.segmentInfo[noar].targetAddress;
      tFblLength len = parsedModuleInfo.segmentList.segmentInfo[noar].length;

      while (len > 0u)
      {
         tFblLength readLen = (tFblLength) ( (len < (sizeof(buffer)-1u))?  len : (sizeof(buffer)-1u));

         if (FlashDriver_RReadSync((IO_MemPtrType)&buffer[buffIdx], (IO_SizeType)readLen, (IO_PositionType)addr) != IO_E_OK)
         {
            fblProgrammedState |= kProgrammedStateRomError;
         }
         if (kFblOk != ApplFblUpdateChecksum(&checksum, (SecM_LengthType)readLen, &buffer[buffIdx]))
         {
            fblProgrammedState |= kProgrammedStateRomError;
         }

         addr += readLen;
         len  -= readLen;
      }
   }

   if (kFblOk != ApplFblFinalizeChecksum(&checksum))
   {
      fblProgrammedState |= kProgrammedStateRomError;
   }
}
#endif /* FBL_ENABLE_ROM_INTEGRITY_CHECK */

#if defined( FBL_ENABLE_RAM_INTEGRITY_CHECK )
/***********************************************************************************************************************
 *  ApplFblRamIntegrityCheck
 **********************************************************************************************************************/
/*! \brief       This routine is responsible for verifying that the Random-Access-Memory (RAM) device(s)
                 are operating correctly. If an error is detected, the state variable used by
 *               ApplFblReportProgrammedState must be updated.
 **********************************************************************************************************************/
void ApplFblRamIntegrityCheck( void )
{
   volatile const tRamTable kRamTable[] =
   {
      /* Edit this to match the start/end address of RAM */
      /* Add further entries to the below GENy generated entry if multiple regions are required */
      { FBL_RAM_INTEGRITY_START_ADDR, FBL_RAM_INTEGRITY_END_ADDR }
   };

   static volatile vuint8 *pCurrAddr;
   static volatile vuint8 tmp;
   volatile vuint8 i;

   /* Some compiler require kRamTable declared as static, if assertion hits change to static. */
   assertFblInternal((kRamTable[0].begin == FBL_RAM_INTEGRITY_START_ADDR), kFblAssertInternalIlegalState);
   assertFblInternal((kRamTable[0].end == FBL_RAM_INTEGRITY_END_ADDR), kFblAssertInternalIlegalState);

   /* check local variables */
   tmp = 0x55;
   if (tmp != 0x55)
   {
      fblProgrammedState |= kProgrammedStateRamError;
      return;
   }
   tmp = 0xAA;
   if (tmp != 0xAA)
   {
      fblProgrammedState |= kProgrammedStateRamError;
      return;
   }

   pCurrAddr = (volatile vuint8 *)0;
   if (pCurrAddr != 0)
   {
      fblProgrammedState |= kProgrammedStateRamError;
      return;
   }
   pCurrAddr = (volatile vuint8 *) - 1;
   if (pCurrAddr != (volatile vuint8 *) - 1)
   {
      fblProgrammedState |= kProgrammedStateRamError;
      return;
   }

   /*
    * Note: This implementation only checks for stuck data bits.
    *       It does not check for shorted data bits.
    *       It does not check for stuck/shorted address lines.
    */
   for (i=0; i < (vuint8)((sizeof(kRamTable)/sizeof(tRamTable))); ++i)
   {
      pCurrAddr = (volatile vuint8 *)kRamTable[i].begin;
      do
      {
         (void)FblLookForWatchdog();

         if (( (pCurrAddr < (volatile vuint8 *)&pCurrAddr) ||
               (pCurrAddr >= (volatile vuint8 *)(&((&pCurrAddr)[1]))) ) &&
               (pCurrAddr != &tmp))
         {
            tmp = *pCurrAddr;

            *pCurrAddr = 0x55;
            if (*pCurrAddr != 0x55)
            {
               fblProgrammedState |= kProgrammedStateRamError;
            }

            *pCurrAddr = 0xAA;
            if (*pCurrAddr != 0xAA)
            {
               fblProgrammedState |= kProgrammedStateRamError;
            }

            *pCurrAddr = tmp;
         }

         /* Increment address */
         pCurrAddr = &pCurrAddr[1];
      } while ((pCurrAddr <= (volatile vuint8 *)kRamTable[i].end) && ((fblProgrammedState & kProgrammedStateRamError) == 0));
   }
}
#endif /* FBL_ENABLE_RAM_INTEGRITY_CHECK */

/***********************************************************************************************************************
 *  ApplFblSleepModeAllowed
 **********************************************************************************************************************/
/*! \brief       This function is called when the FBL-Active-timer expires.
 *  \details     The function return value decides, if the bootloader can now enter the STOP instruction and can recover
 *               from there (kFblOk) or not (kFblFailed).
 *  \return      kFblOk if controller can enter sleepmode and can recover from stop-mode normally.
 *               kFblFailed otherwise.
 **********************************************************************************************************************/
tFblResult ApplFblSleepModeAllowed( void )
{
   tFblResult retVal = kFblFailed;
   if (GetDiagDefaultDiagSession())
   {
# if defined( FBL_ENABLE_WAKEUP_INT )
      /* Default: Returns if the default bootloader IntTable (Fbl_Applvect resp. APPLVECT_FROM_BOOT
       * is present or not. Note that the APPLVECT_FROM_BOOT includes a reset-entry to the boot. */
      retVal = (FblCheckBootVectTableIsValid())? kFblOk : kFblFailed;
#else
      /* In case wakeup is not per interrupt sleep mode is always allowed. This also applies in case
       * of a interrupt base register used. */
      retVal = kFblOk;
#endif /* FBL_CLIENT_GM_SLP6_COMMENT || FBL_CLIENT_PATAC_SLP2_COMMENT */
   }

   return retVal;
}

/***********************************************************************************************************************
 *  ApplFblEnterStopMode
 **********************************************************************************************************************/
/*! \brief       This function is called before the FBL enters the sleep mode.
 *  \details     It is important that all peripheral interrupts are disabled by this function, because interrupts
 *               are enabled after this function to handle the wakeup interrupt.
 **********************************************************************************************************************/
void ApplFblEnterStopMode( void )
{
#if !defined( FBL_USE_FBL_AS_DEMOAPPL )
   /* example below will get TJA1145 transceiver to sleep and wake it up on example/ dummy configured message */
   
   tFblResult wakeup = kFblFailed;

   /* Check if boot loader vector table is inside flash */
   if(ApplFblSleepModeAllowed() == kFblOk)
   {
      /*********************************************************/
      /* Customer/Integration TODO: Implement Low Power Mode   */
      /*********************************************************/
      if (kFblOk == FblCanTrcvTja1145SleepMode())
      {
         /* The below code is only executed if transceiver does not cut power supply */
         do
         {
            FblLookForWatchdogVoid();
            CanTrcv_30_Tja1145_MainFunction();

            /* Check current wakeup state */
            if ( E_OK == CanTrcv_30_Tja1145_CheckWakeup(FBL_CAN_TRCV_TJA1145_HANDLE_PARAM) )
            {
               if (E_OK == FblCanTrcvTja1145NormalMode())
               {
                  wakeup = kFblOk;
               }
            }
         } while (kFblOk != wakeup);
      }

   } /* if(ApplFblSleepModeAllowed() == kFblOk) */
#endif
}

#if defined( FBL_ENABLE_ASSERTION )
/***********************************************************************************************************************
 *  ApplFblFatalError
 **********************************************************************************************************************/
/*! \brief       If the bootloader assertions are enabled, this function will be called in case an invalid bootloader
 *               state is encountered.
 *  \param[in]   errorCode Code number of the encountered assertion
 *  \param[in]   module Name of the affected module (Only if extended info is enabled)
 *  \param[in]   line Line number where the assertion occurred (Only if extended info is enabled)
 **********************************************************************************************************************/
void ApplFblFatalError( FBL_DECL_ASSERT_EXTENDED_INFO(vuint8 errorCode) )
{

   /* Change this variable in debugger in order to see where function was called. */
   volatile vuint8 keepInWhileAllowToExit = 1u;

#if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   (void)errorCode;
#endif

   while (0u != keepInWhileAllowToExit)
   {
      /* Stay in endless loop in order to indicate error condition */
      (void)FblLookForWatchdog();
   }
}
#endif

/***********************************************************************************************************************
 *  ApplFblCheckConditions
 **********************************************************************************************************************/
/*! \brief         Check conditions for diagnostic services
 *  \details       This function is called for each diagnostic service.
 *  \param[in,out] pbDiagData Pointer to diagnostic data buffer
 *  \param[in]     diagReqDataLen The request data length
 *  \return        kFblOk or kFblFailed
 **********************************************************************************************************************/
/* PRQA S 3673 1 */ /* MD_FblKbApi_3673 */
tFblResult ApplFblCheckConditions( vuint8 * pbDiagData, tTpDataType diagReqDataLen )
{
   tFblResult result = kFblOk;

#if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   (void)pbDiagData;
   (void)diagReqDataLen;
#endif

   /* Parse Service IDs */

   /*
   switch (DiagGetRequestSId())
   {
      case kDiagSidDiagnosticSessionControl:
         if (...)
         {
            DiagNRCConditionsNotCorrect();
            return kFblFailed;
         }
         break;
      case kDiagSidEcuReset:
         break;
      case kDiagSidReadDataByIdentifier:
         break;
      case kDiagSidSecurityAccess:
         break;
      case kDiagSidCommunicationControl:
         break;
      case kDiagSidWriteDataByIdentifier:
         break;
      case kDiagSidRoutineControl:
         break;
      case kDiagSidRequestDownload:
         break;
      case kDiagSidTransferData:
         if (...)
         {
            DiagNRCVoltageTooHigh();
            result = kFblFailed;
         }
         else if  (...)
         {
            DiagNRCVoltageTooLow();
            result = kFblFailed;
         }
         else
         {

         }
         break;
      case kDiagSidRequestTransferExit:
         break;
      case kDiagSidTesterPresent:
         break;
      case kDiagSidControlDTCSetting:
         break;
      default:
         break;
   }
   */

   return result;
}

#if defined( FBL_ENABLE_COMPRESSION_MODE )
# if defined( DATA_TYPE_COMPRESSED_ARLE ) || defined( DATA_TYPE_COMPRESSED_LZMA ) || defined( DATA_TYPE_COMPRESSED_USER )
# else
#  error "Please choose one or more of the available compressions DATA_TYPE_COMPRESSED_ARLE, DATA_TYPE_COMPRESSED_LZMA,"
#  error "DATA_TYPE_COMPRESSED_USER or disable compression in GENy."
# endif
#else
# if defined( DATA_TYPE_COMPRESSED_ARLE ) || defined( DATA_TYPE_COMPRESSED_LZMA ) || defined( DATA_TYPE_COMPRESSED_USER )
#  error "Please disable all specific compressions DATA_TYPE_COMPRESSED_ARLE, DATA_TYPE_COMPRESSED_LZMA,"
#  error  "DATA_TYPE_COMPRESSED_USER or enable compression in GENy."
# endif
#endif

/***********************************************************************************************************************
 *  END OF FILE: FBL_AP.C
 **********************************************************************************************************************/
