/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief         Definition of common data structures which are shared between
 *                 bootloader and application software
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
 *
 *  \par Note
 *  \verbatim
 *  Please note, that this file contains a collection of callback functions to be used with the Flash Bootloader.
 *  These functions may influence the behaviour of the bootloader in principle.
 *  Therefore, great care must be taken to verify the correctness of the implementation.
 *
 *  The contents of the originally delivered files are only examples resp. implementation proposals.
 *  With regard to the fact that these functions are meant for demonstration purposes only, Vector Informatik´s
 *  liability shall be expressly excluded in cases of ordinary negligence, to the extent admissible by law or statute.
 *  \endverbatim
 */
/**********************************************************************************************************************/

/***********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  --------------------------------------------------------------------------------------------------------------------
 *  Sebastian Loos                Shs           Vector Informatik GmbH
 *  Andreas Wenckebach            AWh           Vector Informatik GmbH
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  01.00.00   2016-04-19  Shs     -                Initial version
 *  01.01.00   2016-04-28  AWh     -                Add explicit includes, add comment to VGEN_ENABLE_CANFBL usage
 *  01.02.00   2016-05-03  Shs     -                Added FblGetComDat_VerifySignatureFct() to Common Data
 *  01.03.00   2016-07-13  AWh     -                Rename fblCommonData to fblCommonDataAccess
 *  02.00.00   2016-07-26  AWh     -                Rename fblCommonData to fblCommonDataAccess
 *  02.00.01   0216-11-17  Shs     -                Edit tReProgChannel elements
 *  02.01.00   2016-11-18  AWh     -                Add fblAndApplSharedRam, remove configurable comdat elements (csum)
**********************************************************************************************************************/

#ifndef __COMDAT_H__
#define __COMDAT_H__

#include "fbl_def.h"

/***********************************************************************************************************************
 *  VERSION
 **********************************************************************************************************************/

# define FBL_COMDAT_VERSION                     0x0201u
# define FBL_COMDAT_RELEASE_VERSION             0x00u

/***********************************************************************************************************************
 *  DEFINES COMMON DATA RAM
 **********************************************************************************************************************/
/* Common data access macro. Can be used in Bootloader and Application uniformely, if application data structure of
 * type tFblSharedRam is linked to same address like fblAndApplSharedRam
 */
#if defined( FBL_ENABLE_FBL_START )
# define  fblCommonDataAccessRam                      (*((V_MEMRAM1_FAR tFblSharedRam V_MEMRAM2_FAR V_MEMRAM3 *)fblCommonDataAccess.pCommonDataRam))

/* Overwrite this macro, and e.g. map it to a function in case you do not use fblCommonDataAccessRam.targetAddress default format */
#if !defined( FblGetFblConFormatFromApplFormat )
# define FblGetFblConFormatFromApplFormat(x)          (x)
#endif

/* Macro FblGetComDatRam_TargetAddr only used in Fbl */
# define  FblGetComDatRam_TargetAddr()                (FblGetFblConFormatFromApplFormat(fblCommonDataAccessRam.targetAddress))

/* FblSetComDatRam_TargetAddr: Several possible formats may be passed, check tFblSharedRam, targetAddress description */
# define  FblSetComDatRam_TargetAddr(addr)            {\
                                                         fblCommonDataAccessRam.targetAddress=(addr);\
                                                      }
#else
# error "Decide where to read and set tester target address"
# define  FblGetComDatRam_TargetAddr()                0u
# define  FblSetComDatRam_TargetAddr(addr)            { /* Set Target address */ }
#endif /* FBL_ENABLE_FBL_START */

/***********************************************************************************************************************
 *  DEFINES COMMON DATA ROM
 **********************************************************************************************************************/

# define  FBL_COMDAT_VERIFICATION_PATTERN      0x03A56972u

/** Common data access macro. Can be used in Bootloader and Application uniformely. */
# define  fblCommonDataAccess                  (*((V_MEMROM1_FAR tFblCommonData V_MEMROM2_FAR V_MEMROM3 *)GetFblCommonDataPtr()))

/** Comdat feature availability check macros */
# define  FblGetComDat_IsPresent()             (FBL_COMDAT_VERIFICATION_PATTERN == fblCommonDataAccess.verifyPattern)
# define  FblGetComDat_IsCompatible()          (FBL_COMDAT_VERSION == fblCommonDataAccess.majorVersion)

/* Function access macros for key access */
# define  FblGetComDat_SigRsaMod()             (fblCommonDataAccess.bootInfoBlock.secSigRsaKey.modulus.pData)
# define  FblGetComDat_SigRsaModLen()          (fblCommonDataAccess.bootInfoBlock.secSigRsaKey.modulus.size)
# define  FblGetComDat_SigRsaExp()             (fblCommonDataAccess.bootInfoBlock.secSigRsaKey.exponent.pData)
# define  FblGetComDat_SigRsaExpLen()          (fblCommonDataAccess.bootInfoBlock.secSigRsaKey.exponent.size)

# define  FblGetComDat_VerifySignatureFct(a)   (*((pSecVerifySignatureFct)fblCommonDataAccess.pSecVerifySigFct))(a)

# define  FblGetComDat_DCID0()                 (fblCommonDataAccess.bootInfoBlock.dcid[0])
# define  FblGetComDat_DCID1()                 (fblCommonDataAccess.bootInfoBlock.dcid[1])
# define  FblGetComDat_SWMI(i)                 (fblCommonDataAccess.bootInfoBlock.swmi[(i)])
# define  FblGetComDat_SWMI0()                 (fblCommonDataAccess.bootInfoBlock.swmi[0])
# define  FblGetComDat_SWMI1()                 (fblCommonDataAccess.bootInfoBlock.swmi[1])
# define  FblGetComDat_SWMI2()                 (fblCommonDataAccess.bootInfoBlock.swmi[2])
# define  FblGetComDat_SWMI3()                 (fblCommonDataAccess.bootInfoBlock.swmi[3])
# define  FblGetComDat_DLS0()                  (fblCommonDataAccess.bootInfoBlock.dls[0])
# define  FblGetComDat_DLS1()                  (fblCommonDataAccess.bootInfoBlock.dls[1])
# define  FblGetComDat_EcuNameAddr()           (fblCommonDataAccess.bootInfoBlock.ecuName)
# define  FblGetComDat_SubjNameAddr()          (fblCommonDataAccess.bootInfoBlock.subjName)
# define  FblGetComDat_SwmiAddr()              (fblCommonDataAccess.bootInfoBlock.swmi)
# define  FblGetComDat_DlsAddr()               (fblCommonDataAccess.bootInfoBlock.dls)
# define  FblGetComDat_DcidAddr()              (fblCommonDataAccess.bootInfoBlock.dcid)

# define  FblGetComDat_EcuIdAddr()             (fblCommonDataAccess.ecuId)

# define  FblGetComDat_Checksum()                   (fblCommonDataAccess.romCheckDataBlock.checksum)
# define  FblGetComDat_AddrRegions()                (fblCommonDataAccess.romCheckDataBlock.addressRegions)
# define  FblGetComDat_AddrRegionStartAddr(element) (&FblGetComDat_AddrRegions()[ (element) * (HDR_REGADDR_LEN + HDR_REGLEN_LEN)])
# define  FblGetComDat_AddrRegionLength(element)    (&FblGetComDat_AddrRegions()[((element) * (HDR_REGADDR_LEN + HDR_REGLEN_LEN)) + HDR_REGADDR_LEN])

/* Legacy macros, follow legacy name scheme */
# define  GetFblDCID0()                        FblGetComDat_DCID0()
# define  GetFblDCID1()                        FblGetComDat_DCID1()
# define  GetFblSWMI(i)                        FblGetComDat_SWMI(i)
# define  GetFblSWMI0()                        FblGetComDat_SWMI0()
# define  GetFblSWMI1()                        FblGetComDat_SWMI1()
# define  GetFblSWMI2()                        FblGetComDat_SWMI2()
# define  GetFblSWMI3()                        FblGetComDat_SWMI3()
# define  GetFblDLS0()                         FblGetComDat_DLS0()
# define  GetFblDLS1()                         FblGetComDat_DLS1()
# define  GetFblEcuNameAddr()                  FblGetComDat_EcuNameAddr()
# define  GetFblSubjNameAddr()                 FblGetComDat_SubjNameAddr()
# define  GetFblEcuIdAddr()                    FblGetComDat_EcuIdAddr()

/***********************************************************************************************************************
 *  TYPEDEFS COMMON DATA RAM
 **********************************************************************************************************************/

/* Reprogramming channel assignment. Add further values if required
 */
typedef enum
{
   FBL_REPR_CH_UDS,
   FBL_REPR_CH_XCP,
   FBL_REPR_CH_MCMP
   /* Add further entries, if required. */
} tReProgChannel;

typedef vuint16 tTargetAddr;
typedef V_MEMRAM1_FAR vuint8 V_MEMRAM2_FAR V_MEMRAM3 * tFblSbatPtr;

/** Common Ram Data Structure, typically used to share Ram data from Application->Fbl.
 *  Sharing Fbl->Application can also be possible */
typedef struct
{
   /* targetAddress Appl to Fbl pass options
    * ======================================
    *
    * Check wether you have FblWrapperCom_PduR configuration or FblWrapperCom_CAN configuration for available
    * targetAddress options. FblWrapperCom_PduR is present if MSR communication driver is used, FblWrapperCom_CAN else.
    *
    *
    * FblWrapperCom_PduR options:  PASS_OPTION_PDUR_1) pass Fbl PduR connection Id (FblCw_Cfg.c entry) from Appl to Fbl
    * --------------------------                       => In Fbl nothing to be done
    *                                                     ( This is the DEFAULT: FblGetFblConFormatFromApplFormat has to
    *                                                       just return targetAddress itself )
    *
    *                              PASS_OPTION_PDUR_2) pass client diagn. address (tester 0xF1-0xF6) from Appl to Fbl
    *                                                  => In Fbl client diagn. addr has to be translated to
    *                                                     Fbl PduR connection Id (FblCw_Cfg.c entry)
    *                                                     ( FblGetFblConFormatFromApplFormat has to do the translation )
    *
    *                              PASS_OPTION_PDUR_3)  pass application connection Id in targetAddress
    *                                                  => In Fbl this has to be translated to Fbl PduR connection Id (FblCw_Cfg.c entry)
    *                                                     ( FblGetFblConFormatFromApplFormat has to do the translation )
    *
    *
    * FblWrapperCom_CAN options:   PASS_OPTION_CAN_1)  pass client diagnostic address (tester address 0xF1-0xF6) from Appl to Fbl
    * -------------------------                        => In Fbl nothing to be done
    *                                                  ( This is the DEFAULT: FblGetFblConFormatFromApplFormat has to
    *                                                    just return targetAddress itself )
    *
    *                              PASS_OPTION_CAN_2)  pass application connection Id in targetAddress
    *                                                  => In Fbl this has to be translated to client diagnostic address (tester address 0xF1-0xF6)
    *                                                     ( FblGetFblConFormatFromApplFormat has to do the translation )
    *
    */
   tTargetAddr     targetAddress;          /**< pass client diagnostic information from Appl to Fbl (check above format options)  */

   tFblSbatPtr     pSbat;                  /**< pass Sbat (Ram buffer) location from Appl to Fbl */
   tReProgChannel  reProgChannel;          /**< pass programming channel info from Appl to Fbl */
   vuint16         keyNbid;                /**< pass Fbl keyNBID from Fbl to Appl (usable for SBAT verification) */
   /* Add further elements if desired to share with Fbl. */
} tFblSharedRam;

/***********************************************************************************************************************
 *  TYPEDEFS COMMON DATA ROM
 **********************************************************************************************************************/

/** Security key description */
typedef struct
{
   V_MEMROM1_FAR vuint8 V_MEMROM2_FAR V_MEMROM3 * pData;    /**< Pointer to key data */
   vuint16 size;                                            /**< Size of key data */
} tFblCommonSecKey;

/** RSA key pair description */
typedef struct
{
   tFblCommonSecKey  modulus;       /**< RSA key modulus */
   tFblCommonSecKey  exponent;      /**< RSA key exponent */
} tFblCommonSecRsaKey;

/** BootInfoBlock (those elements that need to be explicitly accessed), feel free to extend it to desired additional elements. */
typedef struct
{
   tFblCommonSecRsaKey  secSigRsaKey;   /**< RSA key pair description used for signature verification */
   vuint8               subjName[16];   /**< Subject Name */
   vuint8               ecuName[8];     /**< Ecu name */
   vuint8               dcid[2];        /**< Data Compatibility Identifier for Fbl (BCID) */
   /* Application space and calibration space is configured in Logical Block Table  */
   vuint8               dls[2];         /**< Design Level Suffix */
   vuint8               swmi[4];        /**< SoftWare Module Identifier/Hex Part Number */
   /* AscII Partnumber can be derived from Part Number. Add an element, e.g. ASCIIPN[16], if you prefere redundancy. */
   vuint8               bitDiffTimeout; /**<  Bit Difference Timer Timeout. (0.1s unit) 0->disabled. */
} tBootInfoBlock;

/* BootInfoBlock (those elements that need to be explicitly accessed), feel free to extend it to desired additional elements. */
typedef struct
{
   vuint8               checksum[2];        /**< Module Checksum */
   /* Product Memory Address + Number Of Bytes */
   vuint8               addressRegions[FBL_GMHEADER_NOAR_FBL * 8];
} tRomcheckDataBlock;

/* Common data structure exported in FBL header */
typedef struct
{
   tRomcheckDataBlock  romCheckDataBlock;    /**< Data block for Fbl Romcheck required elements */
   vuint32             verifyPattern;        /**< Verify pattern: Allows check for the presence of structure; use FblGetComDat_IsPresent() */
   vuint16             majorVersion;         /**< Major version: Allow for compatibility check; use FblGetComDat_IsCompatible() */
   vuint8              mid[2];               /**< Fbl Module ID */
   vuint8              ecuId[16];            /**< Ecu Id Dummy field: Used for DemoFbl, check Techref for usage options. */
   tBootInfoBlock      bootInfoBlock;        /**< Fbl accessed elements of GB6002 BootInfoBlock */
   tExportFct          pSecVerifySigFct;     /**< Function for signature verification */
#if defined( FBL_ENABLE_FBL_START )
# if defined( FBL_DEF_ENABLE_NON_KB_MAIN )
   V_MEMRAM1_FAR tFblSharedRam V_MEMRAM2_FAR V_MEMRAM3 * pCommonDataRam; /* Pointer to common Ram data */
# else
   V_MEMRAM1_FAR tCanInitTable V_MEMRAM2_FAR V_MEMRAM3 * pCanInitTable;  /* Pointer to common Ram Data */
# endif
#else
  /* You may decide to use tFblSharedRam or a similar structure also in this case. */
#endif /* FBL_ENABLE_FBL_START */
} tFblCommonData;

/* VGEN_ENABLE_CANFBL is automatically set in bootloader context from v_cfg.h */
#if defined( VGEN_ENABLE_CANFBL )
/***********************************************************************************************************************
 *  GLOBAL DATA (Only in Fbl project)
 **********************************************************************************************************************/
 #define FBLCOMDAT_SHARED_RAM_MEMORY_START_SEC_DATA
#include "MemMap.h"
V_MEMRAM0 extern volatile V_MEMRAM1 tFblSharedRam V_MEMRAM2 fblAndApplSharedRam;
#define FBLCOMDAT_SHARED_RAM_MEMORY_STOP_SEC_DATA
#include "MemMap.h"

# if defined( FBL_ENABLE_COMMON_DATA )
# define FBLCOMDAT_COMMON_DATA_START_SEC_DATA
# include "MemMap.h"
/* Process data shared between application and Bootloader */
V_MEMROM0 extern V_MEMROM1 tFblCommonData V_MEMROM2 fblCommonData;
# define FBLCOMDAT_COMMON_DATA_STOP_SEC_DATA
# include "MemMap.h"
# endif /* FBL_ENABLE_COMMON_DATA */
#endif /* VGEN_ENABLE_CANFBL */

#endif /* __COMDAT_H__ */

/***********************************************************************************************************************
 *  END OF FILE: _COMDAT.H
 **********************************************************************************************************************/

