/* Kernbauer Version: 1.12 Konfiguration: FBL Erzeugungsgangnummer: 1 */

/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief         Application dependent diagnostic routines
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
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY (KbFbl_apdiFrameOem.c, GM SLP5 and SLP6)
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  05.00.00   2014-01-31  AWh                      First release
 *  05.01.00   2014-07-14  AWh                      No changes
 *  05.02.00   2015-01-15  AWH     ESCAN00078960    Gm SLP6: Make allowed Tester addresses configurable
 *                                 ESCAN00080504    No changes
 *  05.02.01   2015-06-12  AWH     -                Update Header Information
 *  05.03.00   2016-03-16  Shs     ESCAN00088937    Added support for PATAC SLP2
 *  05.03.01   2016-03-31  Shs     ESCAN00089151    PATAC SLP2 supports two tester addresses
 *  05.03.02   2016-06-10  Shs     ESCAN00090494    No changes
 *  05.04.00   2016-06-30  Shs     ESCAN00090792    No changes
 *  05.05.00   2016-07-25  AWh     ESCAN00091150    GM Slp6: Add possible Tester F6, Add OTA RID callbacks
 *  05.05.01   2017-03-30  AWh     ESCAN00094560    GM Slp6: Remove Tester F6 from configuration
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
 *  INCLUDES
 **********************************************************************************************************************/

#include "fbl_inc.h"

/***********************************************************************************************************************
 *   VERSION
 **********************************************************************************************************************/

#if ( FBLKBAPI_FRAMEDIAG_GM_VERSION != 0x0505u )
# error "Error in fbl_apdi.c: Source and Header file are inconsistent!"
#endif
#if ( FBLKBAPI_FRAMEDIAG_GM_RELEASE_VERSION != 0x01u )
# error "Error in fbl_apdi.c: Source and Header file are inconsistent!"
#endif

#if ( FBLKBAPI_FRAMEDIAG_GM_VERSION != _FBLKBAPI_FRAMEDIAG_OEM_VERSION ) || \
    ( FBLKBAPI_FRAMEDIAG_GM_RELEASE_VERSION != _FBLKBAPI_FRAMEDIAG_OEM_RELEASE_VERSION )
# error "Error in fbl_apdi.c: Source and v_ver.h are inconsistent!"
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

#if ( FBLKBAPI_VERSION != 0x0171u ) || \
    ( FBLKBAPI_RELEASE_VERSION != 0x00u )
# error "Error in FBL_APxx.C: Source and header file are inconsistent!"
#endif
#if ( FBLKBAPI_VERSION != _FBLKBAPI_VERSION ) || \
    ( FBLKBAPI_RELEASE_VERSION != _FBLKBAPI_RELEASE_VERSION )
# error "Error in FBL_APxx.C: Source and V_VER.H are inconsistent!"
#endif

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

# define DIDSIZE             kDiagRslReadDataByIdentifier
# define GETDIDLEN(dataLen) (DIDSIZE+(dataLen))
#define CHECKPROGDEP_BYTE0_INTERNAL_MEM_ERROR 0x50u

#define CHECKPROGDEP_BYTE1_BYTE2_BIT1_SECACCESSCHECK  0x02u
#define CHECKPROGDEP_BYTE1_BYTE2_BIT2_NORMALCOMMCHECK 0x04u

/***********************************************************************************************************************
 *  TYPEDEFS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  LOCAL DATA
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  EXTERNAL DATA
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  PROTOTYPES
 **********************************************************************************************************************/

static tTpDataType ApplFblRdbidProgrammedStateInd( vuint8 * pbDiagData );

/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  ApplFblReadDataByIdentifier
 **********************************************************************************************************************/
/*! \brief         ReadDataByIdentifier service function.
 *  \param[in,out] pbDiagData Pointer to diag service data (after SID!)
 *  \param[in]     diagReqDataLen Service data length (without SID!)
 **********************************************************************************************************************/
void ApplFblReadDataByIdentifier( vuint8 * pbDiagData, tTpDataType diagReqDataLen )
{
   typedef struct {
      vuint16   did;
      vuint16   maxSize;
   } tDid;

   static V_MEMROM1 tDid V_MEMROM2 didTable[] =
   {
      /* DID, Response Length (give mere data length to GETDIDLEN() macro, that will add the identifier length required */
      { kDiagDidBootSoftwarePartNr, GETDIDLEN(HDR_HEXPN_LEN) }, /* GMBootSoftwarePartNumber */
      { kDiagDidBootSoftwareAlpha, GETDIDLEN(HDR_DLS_LEN) }, /* GMBootSoftwarePartNumberAlphaCode */

      { kDiagDidSwmi01, GETDIDLEN(HDR_HEXPN_LEN) }, /* GMSoftwareModuleIdentifier_01 */
      { kDiagDidSwmi02, GETDIDLEN(HDR_HEXPN_LEN) }, /* GMSoftwareModuleIdentifier_02 */
      { kDiagDidSwmi03, GETDIDLEN(HDR_HEXPN_LEN) }, /* GMSoftwareModuleIdentifier_03 */
      { kDiagDidSwmi04, GETDIDLEN(HDR_HEXPN_LEN) }, /* GMSoftwareModuleIdentifier_04 */
      { kDiagDidSwmi05, GETDIDLEN(HDR_HEXPN_LEN) }, /* GMSoftwareModuleIdentifier_05 */
      { kDiagDidSwmi06, GETDIDLEN(HDR_HEXPN_LEN) }, /* GMSoftwareModuleIdentifier_06 */
      { kDiagDidSwmi07, GETDIDLEN(HDR_HEXPN_LEN) }, /* GMSoftwareModuleIdentifier_07 */
      { kDiagDidSwmi08, GETDIDLEN(HDR_HEXPN_LEN) }, /* GMSoftwareModuleIdentifier_08 */
      { kDiagDidSwmi09, GETDIDLEN(HDR_HEXPN_LEN) }, /* GMSoftwareModuleIdentifier_09 */
      { kDiagDidSwmi10, GETDIDLEN(HDR_HEXPN_LEN) }, /* GMSoftwareModuleIdentifier_10 */

      { kDiagDidAlpha01, GETDIDLEN(HDR_DLS_LEN) }, /* GMSoftwareModuleIdentifierAlphaCode_01 */
      { kDiagDidAlpha02, GETDIDLEN(HDR_DLS_LEN) }, /* GMSoftwareModuleIdentifierAlphaCode_02 */
      { kDiagDidAlpha03, GETDIDLEN(HDR_DLS_LEN) }, /* GMSoftwareModuleIdentifierAlphaCode_03 */
      { kDiagDidAlpha04, GETDIDLEN(HDR_DLS_LEN) }, /* GMSoftwareModuleIdentifierAlphaCode_04 */
      { kDiagDidAlpha05, GETDIDLEN(HDR_DLS_LEN) }, /* GMSoftwareModuleIdentifierAlphaCode_05 */
      { kDiagDidAlpha06, GETDIDLEN(HDR_DLS_LEN) }, /* GMSoftwareModuleIdentifierAlphaCode_06 */
      { kDiagDidAlpha07, GETDIDLEN(HDR_DLS_LEN) }, /* GMSoftwareModuleIdentifierAlphaCode_07 */
      { kDiagDidAlpha08, GETDIDLEN(HDR_DLS_LEN) }, /* GMSoftwareModuleIdentifierAlphaCode_08 */
      { kDiagDidAlpha09, GETDIDLEN(HDR_DLS_LEN) }, /* GMSoftwareModuleIdentifierAlphaCode_09 */
      { kDiagDidAlpha10, GETDIDLEN(HDR_DLS_LEN) }, /* GMSoftwareModuleIdentifierAlphaCode_10 */

      { kDiagDidBootSwId, GETDIDLEN(20u) }, /* Bootsoftware Id; TODO: What is the max size? Check your CTRS. */

      { kDiagDidProgrammedStateIndicator, GETDIDLEN(FBL_MTAB_NO_OF_BLOCKS * PARTID_MASK_BASE) /* Will get overwritten with real length */ }, /* Programmed State Indicator */
      { kDiagDidProgramingErrorCode, GETDIDLEN(FblGetPecErrorLen()) }, /* ProgramingErrorCode (PEC) */
      { kDiagDidBootInitializationStatus, GETDIDLEN(FblGetBisLen()) }, /* Boot Initialization Status (BIS) */

      { kDiagDidEcuId, GETDIDLEN(HDR_ECUID_LEN)        }, /* ECU_ID (ECUI) */
      { kDiagDidBInfoSubjandECUName, GETDIDLEN(HDR_SUBJNAME_LEN + HDR_ECUNAME_LEN) }, /* Boot Info Block Subject Name and ECU Name  */
      { kDiagDidSignatureBypassAT, GETDIDLEN(HDR_SBA_TICKET_LEN)                   }, /* SBA - Ticket read out */
      { kDiagDidDiagnosticAddress, GETDIDLEN(1u) }, /* Diagnostic Address */
   };

   vuint16 diagRespDataLen;
   vuint16 didIdx;
   vuint16 outIdx;
   vuint16 outIdxAdd;
   vuint16 currentDid;
   vuint16 didNo;
   vuint8  didBuffer[(FBL_MAX_DID_COUNT << 1)];
   vuint16 i, j;
   vuint8* diagData;
   vuint8 mid;
   tFblAddress moduleHdrAddr;
   vuint8 hdrIdx;

   /*
      If necessary this check could be used to
      ensure a prior security access. It could
      also be used for certain data identifiers.
   */
   /*
   if (!GetSecurityUnlock())
   {
      DiagNRCSecurityAccessDenied();
      return;
   }
   */

   didNo = (vuint16) (diagReqDataLen / 2u);

   if (didNo > FBL_MAX_DID_COUNT)
   {
      DiagNRCIncorrectMessageLengthOrInvalidFormat();
      return;
   }

   for (i = 0u; i < diagReqDataLen; i++)
   {
      didBuffer[i] = pbDiagData[i];
   }

   outIdx = 0u;
   didIdx = 0u;
   for (i = 0u; i < didNo; i++)
   {
      FblHdrPollingTaskVoid();

      currentDid = (vuint16) didBuffer[didIdx];
      currentDid = (currentDid << 8) | (vuint16) didBuffer[didIdx + 1];

      diagData = &pbDiagData[outIdx];

      diagData[0] = didBuffer[didIdx];
      diagData[1] = didBuffer[didIdx + 1];

      didIdx += DIDSIZE;

      outIdxAdd = 0u;
      for (j = 0u; j < (sizeof(didTable)/sizeof(tDid)); j++)
      {
         if (didTable[j].did == currentDid)
         {
            outIdxAdd = didTable[j].maxSize;
            break;
         }
      }

      if (outIdxAdd != 0u)
      {
         outIdx += outIdxAdd;
         if ((outIdx) > (FBL_DIAG_BUFFER_LENGTH - 1u))
         {
            DiagNRCResponseTooLong();
            break;
         }

         switch (currentDid)
         {
            /* START of legacy SPS DIDs */

            case kDiagDidBootSoftwarePartNr:
               (void) FblReadBlock( (tFblAddress) FblGetComDat_SwmiAddr(), &diagData[DIDSIZE], HDR_HEXPN_LEN );
               break;
            case kDiagDidSwmi01:
            case kDiagDidSwmi02:
            case kDiagDidSwmi03:
            case kDiagDidSwmi04:
            case kDiagDidSwmi05:
            case kDiagDidSwmi06:
            case kDiagDidSwmi07:
            case kDiagDidSwmi08:
            case kDiagDidSwmi09:
            case kDiagDidSwmi10:
               mid = MODID_OPSW_1 + (diagData[(vuint8)1] - (kDiagDidSwmi01 & 0x00FF));
               moduleHdrAddr = FblHdrCheckModuleValidAndGetAddr(mid);

               if (HDR_NO_HEADER_ADDR_DEFINED == moduleHdrAddr)
               {
                  DiagNRCConditionsNotCorrect();
                  /* Leave loop (by setting i to max+1) in order to not check further DIDs */
                  i = didNo;
               }
               else
               {
                  hdrIdx = (kDiagDidSwmi01 == currentDid)? P1_APP_HEXPN_IDX : P1_CAL_HEXPN_IDX;
                  FblReadProm( (moduleHdrAddr+hdrIdx), &diagData[DIDSIZE], HDR_HEXPN_LEN );
               }
               break;
            case kDiagDidBootSoftwareAlpha:
               (void) FblReadBlock( (tFblAddress) FblGetComDat_DlsAddr(), &diagData[DIDSIZE], HDR_DLS_LEN );
               break;
            case kDiagDidAlpha01:
            case kDiagDidAlpha02:
            case kDiagDidAlpha03:
            case kDiagDidAlpha04:
            case kDiagDidAlpha05:
            case kDiagDidAlpha06:
            case kDiagDidAlpha07:
            case kDiagDidAlpha08:
            case kDiagDidAlpha09:
            case kDiagDidAlpha10:
               mid = MODID_OPSW_1 + (diagData[(vuint8)1] - (kDiagDidAlpha01 & 0x00FF));
               moduleHdrAddr = FblHdrCheckModuleValidAndGetAddr(mid);

               if (HDR_NO_HEADER_ADDR_DEFINED == moduleHdrAddr)
               {
                  DiagNRCConditionsNotCorrect();
                  /* Leave loop (by setting i to max+1) in order to not check further DIDs */
                  i = didNo;
               }
               else
               {
                  hdrIdx = (kDiagDidAlpha01 == currentDid)? P1_APP_DLS_IDX : P1_CAL_DLS_IDX;
                  FblReadProm( (moduleHdrAddr+hdrIdx), &diagData[DIDSIZE], HDR_DLS_LEN );
               }
               break;
            /* End of Legacy SPS DIDs*/

            case kDiagDidProgrammedStateIndicator:
               /* Overwrite defined max value with really required length*/
               outIdx -= outIdxAdd;
               outIdx += (vuint16) GETDIDLEN(ApplFblRdbidProgrammedStateInd(&diagData[DIDSIZE]));
               break;
            case kDiagDidProgramingErrorCode:
               diagData[DIDSIZE] = (vuint8)(FblHdrGetReportablePecError() >>  8u);
               diagData[DIDSIZE + 1u] = (vuint8)(FblHdrGetReportablePecError());
               break;
            case kDiagDidBootInitializationStatus:
               diagData[DIDSIZE] = FblGetBisLsbByte();
               diagData[DIDSIZE + 1u] = FblGetBisMsbByte();
               break;
            case kDiagDidEcuId:
               if (kFblOk != ApplFblNVMReadECUID(&diagData[DIDSIZE]))
               {
                  DiagNRCConditionsNotCorrect();
                  /* Leave loop (by setting i to max+1) in order to not check further DIDs */
                  i = didNo;
               }
               break;
            case kDiagDidBInfoSubjandECUName:
               (void) FblReadBlock( (tFblAddress) FblGetComDat_SubjNameAddr(), &diagData[DIDSIZE], HDR_SUBJNAME_LEN );
               (void) FblReadBlock( (tFblAddress) FblGetComDat_EcuNameAddr(), &diagData[DIDSIZE + HDR_SUBJNAME_LEN], HDR_ECUNAME_LEN );
               break;
            case kDiagDidSignatureBypassAT:
               if (kFblOk != ApplFblNVMReadSBATicket(&diagData[DIDSIZE]))
               {
                  DiagNRCConditionsNotCorrect();
                  /* Leave loop (by setting i to max+1) in order to not check further DIDs */
                  i = didNo;
               }
               break;
            case kDiagDidBootSwId:
               /* Format currently unknown by Vector. */
               /* Check Bootinfo array fields: DLS, SWMI,
                * compare kDiagDidBootSoftwarePartNr, kDiagDidBootSoftwareAlpha */
               /* outIdx -= outIdxAdd; *
                * outIdxAdd = GETDIDLEN(?) */
               DiagNRCConditionsNotCorrect();
               /* Leave loop (by setting i to max+1) in order to not check further DIDs */
               i = didNo;
               break;

            case kDiagDidDiagnosticAddress:
               diagData[DIDSIZE] = ApplFblGetNodeAddress();
               break;
            default:
               break;
         }
      }
   }

   if (outIdx == 0)
   {
      DiagNRCRequestOutOfRange();
   }

   diagRespDataLen = outIdx;

   /*
      Transmit response message:
      If a negative response has to be transmitted, the length parameter is
      ignored. In case of a positive response, the length parameter does not
      include the service id.
   */
   DiagProcessingDone(diagRespDataLen);
}  /* PRQA S 2006, 6010, 6030 */ /* MD_MSR_14.7, MD_MSR_STPTH, MD_MSR_STCYC */

#if defined( FBL_ENABLE_WRITE_DATA_BY_IDENTIFIER )
/***********************************************************************************************************************
 *  ApplDiagWriteDataByIdentifier
 **********************************************************************************************************************/
/*! \brief         WriteDataByIdentifier service function.
 *  \param[in,out] pbDiagData Pointer to diag service data (after SID!)
 *  \param[in]     diagReqDataLen Service data length (without SID!)
 *  \return        kFblOk: WriteDataByIdentifer was successful, KFblFailed: WriteDataByIdentifier failed.
 **********************************************************************************************************************/
/* PRQA S 3673 1 */ /* MD_FblKbApi_3673 */
vuint8 ApplFblWriteDataByIdentifier( vuint8 * pbDiagData, tTpDataType diagReqDataLen )
{
#if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   pbDiagData = pbDiagData;
   diagReqDataLen = diagReqDataLen;
#endif

   switch (pbDiagData[(vuint8)0])
   {
      /*
       * Add cases for each supported data-identifier.
       * See ApplFblReadDataByIdentifier() for names of standard identifiers.
       */

      default:
         DiagNRCRequestOutOfRange();
         break;
   }

   return kFblOk;
}
#endif /* FBL_ENABLE_WRITE_DATA_BY_IDENTIFIER */
/***********************************************************************************************************************
 *  ApplFblRdbidProgrammedStateInd
 **********************************************************************************************************************/
/*! \brief       This function is called when a ReadDataByIdentifier (0x1A) service is requested with data identifier
 *               ProgrammedStateIndicator (0xF0)
 *  \details     This functions retrieves the programmed state of the software partitions.
 *  \param[out]  pbDiagData   buffer to store partition data
 *  \return      length of data stored in buffer
 **********************************************************************************************************************/
static tTpDataType ApplFblRdbidProgrammedStateInd(vuint8 * pbDiagData)
{
   vuint8 index = 0u;
   vuint8 opSwPartId;

#if defined( FBL_ENABLE_CALIBRATION_MODULES )
   vuint8 lastCalPartitionId;
   vuint8 calPartId;
#endif /* FBL_ENABLE_CALIBRATION_MODULES */

#if ( FBL_MTAB_NO_OF_BLOCKS > 1 )
   vuint8 i;
   /* Check the PSI of each application (and its partitions) */
   for(i = 0u; i < FBL_MTAB_NO_OF_BLOCKS; i++)
   {
      if (PartIdOpSw(FblLogicalBlockTable.logicalBlock[i].blockIndex))
      {
         opSwPartId = FblLogicalBlockTable.logicalBlock[i].blockIndex;
#else
         opSwPartId = PARTID_OPSW_LOCAL;
#endif
         /* Set operating software partition Id and PSI state in response buffer */
         pbDiagData[index] = opSwPartId;
         index++;
         pbDiagData[index] = (vuint8)ApplFblChkPSIState(opSwPartId);
         index++;

#if defined( FBL_ENABLE_CALIBRATION_MODULES )
         /* Only check calibration partitions if the operating software is present */
         if (pbDiagData[index - 1] == PSI_PART_PRESENT)
         {
            /* Get lastCalPartitionId; will be (lastCalPartitionId == opSwPartId) in case there are no Cal partitions. */
            lastCalPartitionId = opSwPartId + FblHdrGetNoOfCalPartitions(opSwPartId);

            /* Check the PSI of each calibration partition assigned this operating software */
            for (calPartId = opSwPartId + 1u; calPartId <= lastCalPartitionId; calPartId++)
            {
               FblHdrPollingTaskVoid();

               /* Set calibration partition Id and PSI state in response buffer */
               pbDiagData[index] = calPartId;
               index++;
               pbDiagData[index] = (vuint8)ApplFblChkPSIState(calPartId);
               index++;
            }
         }
         else
         {
            /* Report Cal partition PSI */
            pbDiagData[index] = opSwPartId + 1u;
            index++;
            pbDiagData[index] = PSI_PART_INVALID;
            index++;
         }
#endif /* FBL_ENABLE_CALIBRATION_MODULES */

#if ( FBL_MTAB_NO_OF_BLOCKS > 1 )
      } /* PartIdOpSw(FblLogicalBlockTable.logicalBlock[i].blockIndex) */
   } /* for(i = 0u; i < FBL_MTAB_NO_OF_BLOCKS; i++) */
#endif

   /* index is equal to the number of data bytes added to the response buffer */
   return (tTpDataType)index;
}
# if defined( FBL_DIAG_ENABLE_GM_ROUTINE_CHECKPROGDEP )
/***********************************************************************************************************************
 *  ApplFblRdbidProgrammedStateInd
 **********************************************************************************************************************/
/*! \brief       This function is called on FF01 CheckProgrammingDependencies Routine
 *  \details     This is only an example support for the Routine, check it is used in the intended way for Fbl.
 *               (GB6000/GB6001 do not contain clear Fbl requirements)
 *  \param[out]  responseBytes bytes to report
 *  \return      length of data stored in buffer
 **********************************************************************************************************************/
tFblResult ApplFblCheckProgDependencies(V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 * responseBytes)
{
   /* This is only example support for Programming Depencies Service. The values given here are result of
    * communication with Gm, as GB6001 does not give required hints what is needed.
    */

   /* Set Byte 0: ECU Current Programmed State
    *  Report Bis state or programmedstate. */
   if (FblGetBisLsbByte() != 0u)
   {
      responseBytes[0u] = CHECKPROGDEP_BYTE0_INTERNAL_MEM_ERROR;
   }
   else
   {
      responseBytes[0u] = ApplFblGetProgrammedState();
   }

   /* Byte 1: Programming Conditional Flags Supported
    *         Report that security access and normal communication check is supported  */
   responseBytes[1u] = 0x00 | CHECKPROGDEP_BYTE1_BYTE2_BIT1_SECACCESSCHECK | CHECKPROGDEP_BYTE1_BYTE2_BIT2_NORMALCOMMCHECK;

   /* Byte 2: Programming Conditional Flags Status: all 0, nothing supported
    *         Report check states for security access and normal communication  */
   responseBytes[2u] = 0x00u;
   if (!GetSecurityUnlock())
   {
	  responseBytes[2u] |= CHECKPROGDEP_BYTE1_BYTE2_BIT1_SECACCESSCHECK; /* Security Access not yet unlocked. */
   }
   if (!GetDisableNormalCom())
   {
	  responseBytes[2u] |= CHECKPROGDEP_BYTE1_BYTE2_BIT2_NORMALCOMMCHECK; /* Disable Normal communcation not yet received. */
   }

   /* Byte 3: Current Programming Capability
    * Report bits 0-2 set.
    *                Bit2   : Bootloader cannot read the flag. However, if in boot mode, the flag must be set previously.
    *                         Report 1.
    *                Bit1,0 : Progarmming Conditions should be fullfilled in bootmode (checked in appl for
    *                         this Ecu) Report 1 for both.
    *                */
   responseBytes[3u] = 0x07u;

   /* Byte 4: Additional Programming Conditional Flags Status
    * - Note => should be well prepared */
   responseBytes[4u] = 0x00u;

   return kFblOk;
}
# endif /* FBL_DIAG_ENABLE_GM_ROUTINE_CHECKPROGDEP */
/***********************************************************************************************************************
 *  ApplDiagUserService
 **********************************************************************************************************************/
/*! \brief         Call-back function for user diagnostic services. pbDiagData points to subfunction.
 *  \details       This function is called when a diagnostic service has been received that is not managed by the FBL
 *                 itself -> can be managed by user.
 *  \param[in,out] pbDiagData Pointer to diag service data (after SID!)
 *  \param[in]     diagReqDataLen Service data length (without SID!)
 **********************************************************************************************************************/
/* PRQA S 3673 1 */ /* MD_FblKbApi_3673 */
void ApplDiagUserService( vuint8 * pbDiagData, tTpDataType diagReqDataLen )
{
#if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   (void)pbDiagData;
   (void)diagReqDataLen;
#endif

   switch (DiagGetRequestSId())
   {
      default:
      {
         DiagNRCServiceNotSupported();
         break;
      }
   }
}
/***********************************************************************************************************************
 *  ApplDiagUserSubFunction
 **********************************************************************************************************************/
/*! \brief         Call-back function for user diagnostic services. pbDiagData points to subfunction.
 *  \details       This function is called when the bootloader manages the diagnostic service, but the sub-parameter
 *                 is unknown -> subparameter of the service can be managed by user.
 *  \param[in,out] pbDiagData Pointer to diag service data (after SID!)
 *  \param[in]     diagReqDataLen Service data length (without SID!!)
 **********************************************************************************************************************/
/* PRQA S 3673 1 */ /* MD_FblKbApi_3673 */
void ApplDiagUserSubFunction( vuint8 * pbDiagData, tTpDataType diagReqDataLen )
{
#if defined( V_ENABLE_USE_DUMMY_STATEMENT )
   (void)pbDiagData;
   (void)diagReqDataLen;
#endif

   /* If you want to support different sub functions, comment out this code */
   switch (DiagGetRequestSId())
   {
      default:
      {
         DiagNRCSubFunctionNotSupported();
         break;
      }
   }
}

#  if defined( FBL_ENABLE_NORMAL_FIXED_ADDRESSING )
/***********************************************************************************************************************
 *  ApplFblCheckTesterSourceAddr
 **********************************************************************************************************************/
/*! \brief         Check for valid Tester Ids.
 *  \details       Configure the valid tester IDs for your ECU inside kValidTesterIdTable.
 *                 In case of a valid ID FblDiagCheckTesterSourceAddr needs to be called and its result is returned, else
 *
 *  \param[in]     testerId TesterId to check
 **********************************************************************************************************************/
tFblResult ApplFblCheckTesterSourceAddr( vuint8 testerId )
{
   tFblResult retVal = kFblFailed;
   vuintx i;

   /* Add/Remove allowed Tester IDs for your ECU. Consider also hw filtering by setting
    * FBL_CAN_PHYS_MASK / FBL_CAN_FUNC_MASK accordingly.
    */
   static V_MEMROM1 vuint8 V_MEMROM2 kValidTesterIdTable[] =
   {
     (vuint8) DIAG_TESTER_NODE_ADDR1,
     (vuint8) DIAG_TESTER_NODE_ADDR2,
     (vuint8) DIAG_TESTER_NODE_ADDR3,
     (vuint8) DIAG_TESTER_NODE_ADDR4,
     (vuint8) DIAG_TESTER_NODE_ADDR5
   };

   for (i=0u; i<sizeof(kValidTesterIdTable); ++i)
   {
      if (kValidTesterIdTable[i] == testerId)
      {
         retVal = FblDiagCheckTesterSourceAddr(testerId);
         break;
      }
   }

   return retVal;
}
#  endif /* FBL_ENABLE_NORMAL_FIXED_ADDRESSING */

/***********************************************************************************************************************
 *  END OF FILE: KBFBL_APDIFRAMEOEM.C
 **********************************************************************************************************************/

