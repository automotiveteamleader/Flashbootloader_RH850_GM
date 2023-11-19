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
 *  Sebastian Loos                Shs           Vector Informatik GmbH
 *  Thomas Mueller                ThM           Vector Informatik GmbH
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY (KbFbl_apFrameOem.h, GM SLP5 and SLP6)
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  05.00.00   2014-01-31  AWh     -                First release
 *  05.00.01   2015-06-12  AWh     -                Update header information
 *  05.00.02   2015-06-26  AWh     -                Get ApplFblFatalError from upper layer
 *  05.01.00   2016-03-16  Shs     ESCAN00088938    Added support for PATAC SLP2
 *  05.02.00   2016-04-16  Shs     ESCAN00089600    Added support for CommonData structure
 *  05.03.00   2016-05-03  Shs     ESCAN00089864    No changes
 *  05.04.00   2016-05-31  Shs     ESCAN00090230    MemMap Defines for CommonData Structure
 *             2016-06-30  Shs     ESCAN00090791    Removed configuration check for ApplFblCanParamInit()
 *  05.05.00   2016-07-25  AWh     ESCAN00091152    Added Support for Non Kb Main, allow Msr Can driver
 *  05.06.00   2016-11-08  ThM     ESCAN00092691    No changes
 *  05.06.01   2016-12-01  Shs     ESCAN00093124    No changes
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  00.90.00   2003-12-10  CB                       First implementation
 *  00.91.00   2003-12-19  CB                       Decryption/decompression interface changed
 *  01.00.00   2004-02-24  CB                       Release
 *  01.01.00   2004-03-24  Rr                       Added new functions
 *  01.02.00   2004-03-25  WM                       Minor changes
 *  01.03.00   2004-04-23  WM                       Added watchdog trigger functions
 *                         ACP                      Added ApplFblSecurityStatus
 *                         AMN                      Added ApplFblIsValidFlashBlock, ApplFblReadBlock
 *  01.04.00   2004-05-07  CB                       Added ApplCanBusOff, ApplCanWakeUp
 *  01.05.00   2004-05-19  CB                       Minor changes
 *  01.06.00   2004-06-30  MHz     ESCAN00007536    Added prototype for function
 *                                                  ApplFblStatusGet()
 *                         WM                       Added prototype for ApplFblInitErrStatus()
 *  01.07.00   2004-07-22  CB                       Added ApplFblReadEcuIdentification,
 *                                                  ApplFblStoreTesterCode, ApplFblStoreProgrammingDate
 *  01.08.00   2004-08-06  WM                       Added ApplFblSecurityAccessDelayActive
 *  01.09.00   2004-09-09  Rr                       No changes
 *  01.10.00   2004-09-16  WM                       No changes
 *  01.11.00   2004-10-25  AP      ESCAN00010007    Adaptation to support Vector
 *                                                  component OSEK NM
 *  01.12.00   2004-12-23  AMN                      Changed prototype of ApFblWriteDataByIdentifier
 *                                                  Added ApplFblPBLConfigSet prototype
 *                         ACP                      Version check reworked
 *                         CB                       Added fingerprint functions
 *  01.13.00   2005-01-14  CB                       Added several API functions
 *  01.14.00   2005-03-22  WM      ESCAN00011595    MISRA warnings
 *  01.15.00   2005-09-29  WM                       No changes
 *  01.16.00   2005-12-08  WM      ESCAN00014594    FblApSrcApplFblTpParamInit token
 *                                                  added for TPMC
 *  01.17.00   2006-02-28  AMN                      Added tokens ApplFblCopyToRAM and
 *                                                  ApplFblIsValidAppInRAM
 *  01.18.00   2006-03-24  QPs                      Added ApplFblGetSubNodeRxCanId for gateway
 *                                                  implementation
 *  01.19.00   2006-04-25  CB      ESCAN00015917    Return value added for
 *                                                  ApplFblInvalidateBlock
 *                         WM      ESCAN00015960    Segment verification added
 *                         QPs     ESCAN00016110    Added ApplCanBusOff for the gateway
 *                         CB      ESCAN00016144    Return value added for
 *                                                  ApplFblInitDecryption
 *                         CB      ESCAN00016147    ApplFblStoreBootSwIdent added
 *                                 ESCAN00016171    ApplFblSetSecAccessDelayFlag,
 *                                                  ApplFblGetSecAccessDelayFlag and
 *                                                  ApplFblClrSecAccessDelayFlag added
 *  01.20.00   2006-05-18  ACP     ESCAN00016438    Added kStartupPreInit/PostInit
 *                                                  definition for new ApplFblStartup() parameter
 *  01.21.00   2006-06-29  FHE                      Added header for ApplFblCheckProgDependencies()
 *  01.22.00   2006-07-24  JHg                      Moved watchdog trigger function to non banked
 *                                                  memory for MCS12X
 *                                                  PRE_ and POST_FblApSrcApplFblWDTrigger
 *                         CB      ESCAN00017003    Added ApplFblProgrammingStateChanged
 *                         AMN                      added prototype for ApplFblValidateApp
 *  01.23.00   2006-08-10  JHg                      Added token for additional watchdog functions
 *                         AMN     ESCAN00017479    Modifed API of ApplFblValidateApp
 *                                                  for Misra compliance
 *  01.24.00   2006-12-06  FHE     ESCAN00018639    Added ApplFblCheckConditions
 *  01.25.00   2007-02-07  Hp      ESCAN00019129    Add support for CCP in the bootloader
 *                                                  ApplFblWriteDataByLocalId returns now vuint8.
 *                                                  Add PreHandler and presence-pattern support.
 *  01.26.00   2007-03-30  WM      ESCAN00020175    Added ApplFblControlFailSafeReaction
 *  01.27.00   2007-07-06  JHg                      Return value added to ApplFblDecryption and
 *                                                  ApplFblDeinitDecryption
 *  01.28.00   2007-07-27  Ls                       No changes
 *  01.29.00   2008-03-07  Ci      ESCAN00014726    No changes
 *                         Hp      ESCAN00014726    No changes
 *                         JHg                      No changes
 *                         Hp      ESCAN00025029    Support Vector decompression module (optional)
 *  01.30.00   2008-03-28  JHg     ESCAN00025354    No changes
 *                                 ESCAN00025355    No changes
 *  01.31.00   2008-06-13  Ci      ESCAN00025071    Extended function description of
 *                                                  ApplFblStartup() with kStartupStayInBoot
 *                                 ESCAN00027445    Added support for configurations
 *                                                  with multiple CAN channels
 *  01.32.00   2008-09-10  JHg     ESCAN00029860    No changes
 *  01.33.00   2008-10-15  FHe     ESCAN00030640    Added support for VAG spec 1.2.1
 *  01.34.00   2009-03-13  JHg     ESCAN00032180    Added support for multichannel configurations
 *                                                  to ApplCanTxObjStart, ApplCanTxObjConfirmed
 *                         FHe                      Restored old format of ApplFblCheckProgConditions
 *                         Ci      ESCAN00033475    Added gateway specific callback
 *                                                  functions (Gw_Route support)
 *  01.35.00   2009-03-12  QPs     ESCAN00033801    Added communication wrapper specific
 *                                                  callback functions
 *  01.36.00   2009-07-21  QPs     ESCAN00035282    Added DC SLP10 extension:
 *                                                  ApplFblControlFailSafeReaction
|   01.36.00               AWh     ESCAN00036825    No changes
 *  01.37.00   2009-08-31  AWh     ESCAN00037405    Inconsistent return value type between
 *                                                  WriteDataByCommonId prototype and implementation
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
 *  01.42.00   2010-07-23  CB      ESCAN00044299    No changes
 *  01.43.00   2010-09-15  Ci      ESCAN00044991    Added new Gw_Route API functions
 *  01.44.00   2010-12-10  MVi     ESCAN00047478    Enhanced flash erased detection interface
 *                                 ESCAN00047482    Added routine control succeeded interface
 *  01.45.00   2011-02-03  Ach     ESCAN00048191    No changes
 *  01.46.00   2011-07-14  Ci      ESCAN00052149    Adapted Gw_Route task interface
 *  01.47.00   2011-11-25  Hp      ESCAN00052787    Extend support for PresencePatterns
 *  01.48.00   2011-12-14  AWh     ESCAN00055586    Extend Api with ApplFblInvalidateApp function
 *  01.49.00   2012-02-14  Ach     ESCAN00056341    Added ApplFblStartApplication() function
 *  01.50.00   2012-02-22  Ach     ESCAN00052787    Extend support for PresencePatterns
 *                         MVi     ESCAN00056833    No changes
 *                                 ESCAN00056834    Added _EXPORT suffix to memmap defines
 *                         CB      ESCAN00056642    Added ApplFblCanPlatformToPhysChannel()
 *  01.51.00   2012-03-23  QPs     ESCAN00057230    Extend support for FlexRay
 *                         Ach     ESCAN00057636    No changes
 *                         CB      ESCAN00057670    Added Suspend Programming API
 *  01.52.00   2012-03-30  QPs     ESCAN00058040    Reworked PRE/POST tokens for ApplFblWDTrigger
 *  01.53.00   2012-08-16  MVi     ESCAN00060433    No changes
 *  01.54.00   2012-12-06  AWh     ESCAN00061619    Moved presence pattern macros to header
 *                         Ach     ESCAN00062912    Adapted comments to use Doxygen
 *  01.55.00   2013-01-24  Ach     ESCAN00064406    No changes
 *                         JHg     ESCAN00064421    Added callback ApplFblOnTransitionSession
 *                                 ESCAN00064437    Added callback ApplFblStateTask
 *  01.56.00   2013-02-08  ASe     ESCAN00064889    Reworked comments
 *  01.57.00   2014-01-21  ASe     ESCAN00072931    No changes
 *  01.58.00   2014-02-24  MVi     ESCAN00073549    No changes
 *  01.58.01   2014-04-03  ASe     ESCAN00074701    Fixed comments
 *  01.59.00   2014-07-16  QPs     ESCAN00077127    No changes
 *  01.60.00   2014-08-25  Dod     ESCAN00078033    Add callbacks ApplFblMemPreWrite/Erase and ApplFblGetLogBlkHash
 *  01.61.00   2015-01-13  Ach     ESCAN00080481    Change visibility of ApplFblChkModulePresence()
 *  01.62.00   2015-05-12  CB      ESCAN00082959    Added functions to read and store the TP target address
 *  01.63.00   2015-05-27  CB      ESCAN00083167    Additional callback functions for status backup
 *  01.64.00   2015-09-14  Ci      ESCAN00084704    No changes
 *                         QPs     ESCAN00085203    Added callback ApplFblCheckTesterSourceAddr
 *  01.64.01   2015-12-15  TnS     ESCAN00087100    Moved initialization for Timer Interrupt use case to FblWd
 *  01.65.00   2016-01-11  ASe     ESCAN00087324    No changes
 *                         Rr      ESCAN00087414    Extend Renault SLP3 specific API for reading light LogZone
 *  01.66.00   2016.02.25  TnS     ESCAN00088471    No changes
 *  01.67.00   2016-03-07  MVi     ESCAN00088759    Added ApplFblIsValidBlock
 *  01.68.00   2016-03-17  Shs     ESCAN00088957    Added support for PATAC SLP2
 *  01.69.00   2016-05-09  HRf     ESCAN00089907    No changes
 *  01.70.00   2016-05-17  Rie     ESCAN00090027    No changes
 *  01.71.00   2016-06-27  Ach     ESCAN00090683    Support configuration switch FBL_CW_ENABLE_MULTIPLE_NODES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  REVISION HISTORY (KbFbl_apOem.h, GM SLP5 and SLP6)
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  05.00.00   2014-01-31  AWh                      First release
 *  05.01.00   2014-02-27  AWh     ESCAN00073863    Add user callbacks for Non readable Device use case
 *                                 ESCAN00073906    Enable Multi processor use case
 *  05.02.00   2014-07-14  AWh     ESCAN00075304    No changes
 *                                                  SLP6: Add DID information available at Vector
 *                                                        Move DIDs from diagnostic layer to callback layer
 *                                                        Add ApplFblReadTpTargetAddr/ApplFblWriteTpTargetAddr
 *  05.03.00   2014-10-15  AWh     ESCAN00078893    No changes
 *                         AWh     ESCAN00078899    No changes
 *                         AWh     ESCAN00079661    Add compression interface
 *                         AWh     ESCAN00080211    No changes
 *                         Dod     ESCAN00079983    No changes
 *                                 -                Rework after Review
 *  05.04.00   2015-01-23  AWh     ESCAN00080768    Add HighSpeed/Lowspeed Macros required for single wire config
 *  05.05.00   2015-05-06  Dod     ESCAN00082852    No changes
 *  05.06.00   2015-06-12  AWh     ESCAN00083305    Add missing ApplFblNVMWrite macros/functions for Sba and EcuId
 *                                 ESCAN00082260    No changes
 *  05.07.00   2015-07-06  AWh     ESCAN00083824    No changes
 *                         AWh     ESCAN00078397    No changes
 *  05.08.00   2015-09-18  AWh     ESCAN00085306    Checksum calculation is skipped through unaligned buffer.
 *  05.08.01   2015-11-13  AWh     ESCAN00086481    No changes
 *  05.09.00   2016-02-22  AWh     ESCAN00088480    Add support for basic NBID handling
 *  05.10.00   2016-03-16  Shs     ESCAN00088940    Added support for PATAC SLP2
 *  05.10.01   2016-03-29  AWh     ESCAN00089097    No changes
 *  05.10.02   2016-04-05  AWh     ESCAN00089233    No changes
 *  05.11.00   2016-05-06  AWh     ESCAN00089345    No changes
 *                                 ESCAN00089826    No changes
 *                         Shs     ESCAN00089536    No changes
 *                         AWh     ESCAN00089901    No changes
 *                                 ESCAN00089536    No changes
 *  05.11.01   2016-06-10  Shs     ESCAN00090427    Compiler error: identifier "PARTID_OPSW_LOCAL" is undefined
 *                                 ESCAN00090568    No changes
 *  05.12.00   2016-06-30  Shs     ESCAN00090793    Added support for ComWrapper 2.xx
 *                                 ESCAN00090819    No changes
 *  05.13.00   2016-07-25  AWh     ESCAN00091151    Added Support for XCP and Non Kb Main
 *                                 ESCAN00091250    Add support for MSR Can driver
 *                                 ESCAN00090093    No changes
 *  05.14.00   2016-10-31  AWh     ESCAN00092368    SLP6: Offer ApplFblGetProgrammedState for FF 01 handling
 *                         Shs     ESCAN00090118    No changes
 *                                 ESCAN00090116    No changes
 *                         ThM     ESCAN00092600    Added support for Lzma decompression
 *                         Shs     ESCAN00092589    No changes
 *                         Shs     ESCAN00092625    Moved access macros for FblHeader elements to Common Data
 *                         Shs     ESCAN00092711    No changes
 *                         ThM     ESCAN00092753    No changes
 *  05.14.01   2016-11-17  Shs     ESCAN00092866    XCP: Changes of tReProgChannel in Common Data
 *  05.14.02   2016-11-24  AWh     ESCAN00092997    No changes
 *                         Shs     ESCAN00093013    No changes
 *  05.14.03   2016-12-01  Shs     ESCAN00093126    No changes
 *  05.14.04   2016-12-13  AWh     ESCAN00093273    No changes
 *                                 ESCAN00093845    No changes
 *                         Shs     ESCAN00094519    No changes
 *                         ThM     ESCAN00094674    No changes
 *                         AWh     ESCAN00094743    No changes
 *                                 ESCAN00094744    No changes
 *  05.14.05   2017-05-11  Shs     ESCAN00094961    No changes
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY (RH850 KbFbl_apHW.C)
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  00.90.00   2012-12-18  CB                       First implementation
 *  01.00.00   2013-06-07  CB                       Release
 *  01.01.00   2015-10-21  JLe     ESCAN00086043    [GM Only] Add HighSpeed and LowSpeed macros
 *  01.02.00   2016-05-03  Ci      ESCAN00080515    No changes
 *  01.03.00   2016-05-23  CB      ESCAN00090133    No changes
 *  01.04.00   2016-08-03  Shs     ESCAN00091046    No changes
                           DlM     ESCAN00091205    No changes
 **********************************************************************************************************************/

#ifndef __FBL_AP_H__
#define __FBL_AP_H__

/***********************************************************************************************************************
 *  VERSION
 **********************************************************************************************************************/

/* ##V_CFG_MANAGEMENT ##CQProject : FblKbApi CQComponent : Implementation */
#define FBLKBAPI_VERSION                           0x0171u
#define FBLKBAPI_RELEASE_VERSION                   0x00u

/* ##V_CFG_MANAGEMENT ##CQProject : FblKbApi_GM CQComponent : Implementation */
#define FBLKBAPI_GM_VERSION           0x0514u
#define FBLKBAPI_GM_RELEASE_VERSION   0x05u

/* ##V_CFG_MANAGEMENT ##CQProject : FblKbApi_Rh850Rscan CQComponent : Implementation */
#define FBLKBAPI_RH850RSCAN_VERSION             0x0104u
#define FBLKBAPI_RH850RSCAN_RELEASE_VERSION     0x00u

/* ##V_CFG_MANAGEMENT ##CQProject : FblKbApi_Frame_GM CQComponent : Implementation */
#define FBLKBAPI_FRAME_GM_VERSION           0x0506u
#define FBLKBAPI_FRAME_GM_RELEASE_VERSION   0x01u

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

#define APPL_FBL_RESET() (ApplFblReset())
/* For GM: Initialize for high-speed programming mode */
#define ApplFblDiagHighSpeed() {             \
   fblCanIdTable.BCFG = CAN_BCFG_Highspeed;  \
   }
/* For GM: Initialize for low-speed programming mode */
#define ApplFblDiagLowSpeed() {   \
   fblCanIdTable.BCFG = CAN_BCFG; \
   }

#if defined( FBL_ENABLE_COMPRESSION_MODE )
# if defined( DATA_TYPE_COMPRESSED_ARLE )
#  include "cmpr.h"
# endif
# if defined( DATA_TYPE_COMPRESSED_LZMA )
#  include "cmpr_lzma.h"
# endif

typedef V_API_NEAR tFblResult (*tCmprInit)( void );
typedef V_API_NEAR tFblResult (*tCmprReadCmprHdr)( V_MEMRAM1 tFblLength V_MEMRAM2 V_MEMRAM3 *, const V_MEMRAM1 vuint8 V_MEMRAM2 V_MEMRAM3 *, V_MEMRAM1 vuint16 V_MEMRAM2 V_MEMRAM3 * );
typedef V_API_NEAR tFblResult (*tCmprDecompress)( V_MEMRAM1 tProcParam V_MEMRAM2 V_MEMRAM3 *, vuint16 );
typedef V_API_NEAR tFblResult (*tCmprDeinit)( V_MEMRAM1 tProcParam V_MEMRAM2 V_MEMRAM3 * );

typedef struct
{
   tCmprInit               init;
   tCmprReadCmprHdr        readCmprHdr;
   tCmprDecompress         decompress;
   tCmprDeinit             deinit;
} tCmprInterfaceGm;

/* The below functions are defined if you ordered the Vector Compression interface,
 * the interface has to be implemented by yourself else.
 */
# define ApplFblCmprInit         (*cmprLst[cmprLstIdx].init)
# define ApplFblCmprReadHeader   (*cmprLst[cmprLstIdx].readCmprHdr)
# define ApplFblDecompress       (*cmprLst[cmprLstIdx].decompress)
# define ApplFblCmprDeinit       (*cmprLst[cmprLstIdx].deinit)
#endif

# define ApplFblBusSleep()                        ApplFblEnterStopMode()

/***********************************************************************************************************************
 *  GLOBAL TYPE DEFINITIONS AND STRUCTURES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

void ApplFblInit( void );
void ApplFblStartup( vuint8 initposition );
#if defined( FBL_ENABLE_MULTIPLE_NODES ) || \
    defined( FBL_ENABLE_CAN_CONFIGURATION) || \
    defined( FBL_CW_ENABLE_MULTIPLE_NODES )
void ApplFblCanParamInit( void );
#endif

#if defined( FBL_USE_FBL_AS_DEMOAPPL )
#else
void ApplFblStartApplication( void );
#endif
tFblResult ApplFblCheckProgConditions( void );
void ApplTrcvrNormalMode( void );
void ApplTrcvrSleepMode( void );
void ApplFblSetVfp( void );
void ApplFblResetVfp( void );
void ApplFblReset( void );

#if defined( FBL_ENABLE_APPL_TASK )
void ApplFblTask( void );
#endif

#if defined( FBL_DEF_ENABLE_NON_KB_MAIN )
# if defined( FBL_ENABLE_APPL_TASK ) ||\
     defined( FBL_ENABLE_APPL_STATE_TASK )
void ApplFblStateTask( void );
# endif
#endif

#if defined( FBL_ENABLE_DATA_PROCESSING )
tFblResult ApplFblCheckDataFormatIdentifier(vuint8 formatId);
tFblResult ApplFblInitDataProcessing( tProcParam * procParam );
tFblResult ApplFblDataProcessing( tProcParam * procParam );
tFblResult ApplFblDeinitDataProcessing( tProcParam * procParam );
#endif /* FBL_ENABLE_DATA_PROCESSING */

#if defined( FBL_ENABLE_DEBUG_STATUS )
void ApplFblInitErrStatus( void );
#endif

# define DRVCAN_START_SEC_CODE_EXPORT
# include "memmap.h"   /* PRQA S 5087 *//* MD_MSR_19.1 */
void ApplFblCanWakeUp( void );
void ApplFblCanBusOff( void );
# define DRVCAN_STOP_SEC_CODE_EXPORT
# include "memmap.h"   /* PRQA S 5087 *//* MD_MSR_19.1 */

void ApplFblTpErrorInd( vuint8 tpErrorCode );

#if defined( FBL_ENABLE_ROM_INTEGRITY_CHECK )
void ApplFblRomIntegrityCheck( void );
#endif

#if defined( FBL_ENABLE_RAM_INTEGRITY_CHECK )
void ApplFblRamIntegrityCheck( void );
#endif

tFblResult ApplFblSleepModeAllowed( void );
void ApplFblEnterStopMode( void );

tFblResult ApplFblCheckConditions( vuint8 * pbDiagData, tTpDataType diagReqDataLen );

#if defined( FBL_ENABLE_ASSERTION )
void ApplFblFatalError( FBL_DECL_ASSERT_EXTENDED_INFO(vuint8 errorCode) );
#endif

/***********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

#if defined( FBL_ENABLE_COMPRESSION_MODE )
V_MEMROM0 extern V_MEMROM1_FAR tCmprInterfaceGm V_MEMROM2_FAR cmprLst[];
tFblResult ApplFblCtrlDecompress( V_MEMRAM1 tProcParam V_MEMRAM2 V_MEMRAM3 * procParam, vuint16 outThreshold );
V_MEMRAM0 extern V_MEMRAM1 vuint8 V_MEMRAM2 cmprLstIdx;
#endif /* FBL_ENABLE_COMPRESSION_MODE */

/* No modification of fblEcuHandle should be required. */
#  if defined( FBL_CW_ENABLE_MULTIPLE_NODES )
/* Handle which specifies the set of communication parameters to use */
V_MEMRAM0 extern V_MEMRAM1 vuint8  V_MEMRAM2 fblEcuHandle;
#  endif /* FBL_CW_ENABLE_MULTIPLE_NODES */

#if defined( FBL_COMDAT_VERSION )
   /* fblCommonData export is now offered through comdat.h */
#else
# define FBLAP_COMMON_DATA_START_SEC_DATA
# include "MemMap.h"

# if defined( FBL_ENABLE_COMMON_DATA )
/* Process data shared between application and Bootloader */
V_MEMROM0 extern V_MEMROM1 tFblCommonData V_MEMROM2 fblCommonData;
# endif /* FBL_ENABLE_COMMON_DATA */

# define FBLAP_COMMON_DATA_STOP_SEC_DATA
# include "MemMap.h"
#endif /* FBL_COMDAT_VERSION */

#endif /* __FBL_AP_H__ */

/***********************************************************************************************************************
 *  END OF FILE: FBL_AP.H
 **********************************************************************************************************************/
