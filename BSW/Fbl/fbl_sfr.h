/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief        Compiler-independent special function register mapping of Renesas RH850/RL78
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
 *  Christian Baeuerle            CB            Vector Informatik GmbH
 *  Marcel Viole                  MVi           Vector Informatik GmbH
 *  Achim Strobelt                Ach           Vector Informatik GmbH
 *  Thomas Bezold                 TBe           Vector Informatik GmbH
 *  Christopher Elkins            CEl           Vector CANtech, Inc.
 *  Ralf Haegenlaeuer             HRf           Vector Informatik GmbH
 *  Andre Caspari                 Ci            Vector Informatik GmbH
 *  Anqi Du                       AND           Vector CANtech, Inc.
 *  Alexander Starke              ASe           Vector Informatik GmbH
 *  Quetty Palacios               QPs           Vector Informatik GmbH
 *  Sebastian Loos                Shs           Vector Informatik GmbH
 *  Johannes Krimmel              KJs           Vector Informatik GmbH
 *  Torben Stoessel               TnS           Vector Informatik GmbH
 *  Marco Riedl                   Rie           Vector Informatik GmbH
 *  Daniel Koebel                 DKl           Vector Informatik GmbH
 *  Daniel Moessner               DlM           Vector Informatik GmbH
 *  Johannes Rebmann              JRe           Vector Informatik GmbH
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  01.00.00   2013-06-05  CB                       Implementation
 *  01.01.00   2013-09-24  MVi     ESCAN00070496    Added IAR RL78 RS-CAN Lite
 *  01.02.00   2014-03-05  MVi                      Added comments, changed indention
 *                         Ach     ESCAN00073965    Support of P1X
 *                         TBe     ESCAN00074028    Added support for RL78 Renesas compiler
 *  01.05.00   2014-05-22  CB      ESCAN00075823    Added support for RH850 WindRiver/DiabData compiler
 *                                                   Harmonized version number with fbl_hw
 *  01.06.00   2014-07-01  CEl     ESCAN00076640    Added Code Flash ECC register definitions
 *  01.07.00   2014-08-01  HRf     ESCAN00077536    No change
 *                         CEl     ESCAN00077668    Added support for P1X ECC registers
 *  01.08.00   2014-08-27  Ci      ESCAN00078093    Added support for F1H (fbl_sfr.h)
 *                                 ESCAN00078095    Adapted configuration switches
 *                         CEl     ESCAN00078754    Added support for D1M (fbl_sfr.h)
 *                         Rr      ESCAN00078791    Added CAN clock registers
 *                                 ESCAN00078970    Harmonized ECC handling
 *                         CEl     ESCAN00079009    Add support for RZ
 *  01.09.00   2015-02-18  CB      ESCAN00079110    No changes
 *                         AND     ESCAN00079110    No changes
 *                         ASe     ESCAN00080122    Renamed port pin read register, added input buffer control register
 *                         Ci      ESCAN00080218    Added support for R1x (fbl_sfr.h)
 *                         Rr      ESCAN00080422    Correct definition of WRCSR register write macro
 *  01.10.00   2015-04-29  CB      ESCAN00082512    No changes
 *                         QPs     ESCAN00082784    No changes
 *  01.11.00   2015-07-09  Shs     ESCAN00083898    Corrected ProtWrite macros, added FBL_SW_RESET() macro
 *  01.11.01   2015-07-30  KJs     ESCAN00084288    Defined missing values for protection sequence
 *  01.11.02   2015-10-26  TnS     ESCAN00084351    No changes
 *  01.12.00   2015-10-29  TnS     ESCAN00086139    No changes
 *  01.13.00   2015-11-23  Shs     ESCAN00086688    Added Support for additional GPIOs on RH850 D1x
 *  01.13.01   2015-12-23  ASe     ESCAN00087278    Added support for external CAN clock source configuration
 *  01.13.02   2016-02-25  TnS     ESCAN00078239    SFRs of the PLL use wrong addresses
 *                                 ESCAN00088455    MISRA related changes
 *  01.13.03   2016-02-29  Rie     ESCAN00088610    No changes
 *  01.14.00   2016-03-18  CB      ESCAN00088977    Added defines for PBDCn- and PODCn- registers
 *  01.14.01   2016-04-05  TnS     ESCAN00089231    No changes
 *  01.15.00   2016-05-09  TnS     ESCAN00089926    Added SEG_CONT register
 *                                 ESCAN00089958    No changes
 *  01.16.00   2016-05-23  CB      ESCAN00090124    Support for E1x
 *  01.16.01   2016-05-23  DKl     ESCAN00090520    No changes
 *  01.17.00   2016-07-13  TnS     ESCAN00090720    Improve compliance to hardware user manual
 *                         CB      ESCAN00090982    No changes
 *  01.18.00   2016-07-18  Shs     ESCAN00091047    Added Support for D1x CLKFIX clock
 *             2016-07-21  TBe     ESCAN00090770    No changes
 *                                 ESCAN00091092    No changes
 *  01.19.00   2016-07-28  DlM     ESCAN00091227    Added support for F1K
 *             2016-08-02  JRe     ESCAN00091290    No changes
 *  01.19.01   2016-08-05  JRe     ESCAN00091339    No changes
 *  01.19.02   2016-11-18  DlM     ESCAN00092878    Added support for RL78 derivative F15
 *             2016-11-28  AWh     ESCAN00093053    No changes
 *  01.20.00   2017-01-09  DlM     ESCAN00093473    Added switch FBL_ENABLE_BUSTYPE_CAN for enable or disable CAN relevant code parts
 **********************************************************************************************************************/

#ifndef __FBL_SFR_H__
#define __FBL_SFR_H__

/***********************************************************************************************************************
 *  VERSION
 **********************************************************************************************************************/

#define FBL_SFR_VERSION             0x0120u
#define FBL_SFR_RELEASE_VERSION     0x00u

/***********************************************************************************************************************
 *  DEFINES AND MACROS
 **********************************************************************************************************************/
/* PRQA S 0828 EOF */ /* MD_MSR_1.1_828 */

#if defined( V_CPU_RH850 )
/* ---- Clock Generation and PLL related section --------------------------------------------------- */
# define FBL_CKSC0CTL               (*(volatile vuint32*)0xFFF89080ul)
# define FBL_CKSC0STAT              (*(volatile vuint32*)0xFFF89084ul)
# define FBL_CKSC0DIV               (*(volatile vuint32*)0xFFF89180ul)
# define FBL_CLKD0STAT              (*(volatile vuint32*)0xFFF89184ul)

# define FBL_CKSC1CTL               (*(volatile vuint32*)0xFFF890C0ul)
# define FBL_CKSC1STAT              (*(volatile vuint32*)0xFFF890C4ul)
# define FBL_CKSC1DIV               (*(volatile vuint32*)0xFFF89188ul)
# define FBL_CLKD1STAT              (*(volatile vuint32*)0xFFF8918Cul)

/* Protection register */
# define FBL_PROT1PHCMD             (*(volatile vuint32*)0xFFF8B000ul)  /**< Protection command register 1 */
# define FBL_PROT1PS                (*(volatile vuint32*)0xFFF8B004ul)  /**< Write sequence status register 1*/

/* Flash related registers ------------------------------------------------------------------------- */
#define FBL_FLMDCNT                 (*(volatile vuint32*)0xFFA00000ul)  /**< FLMD cnt register                     */
#define FBL_FLMDPCMD                (*(volatile vuint32*)0xFFA00004ul)  /**< FLMD protection command register      */
#define FBL_FLMDPS                  (*(volatile vuint32*)0xFFA00008ul)  /**< FLMD protection error status register */

/* System Error Notification Setting Registers ----------------------------------------------------- */
# define FBL_SEG_CONT               (*(volatile vuint16*)0xFFFF8C00ul)  /**< SYSERR Exception Notification Setting */
# define FBL_SEG_FLAG               (*(volatile vuint16*)0xFFFF8C02ul)  /**< SYSERR Source Storing Registers */

/* Code flash ECC flag positions in SEG_CONT and SEG_FLAG registers */
# define kSegContRome               0x0002u
# define kSegFlagRomf               0x0002u

/* Code Flash ECC related registers ---------------------------------------------------------------- */
#define kEccProtWrite               0x4000u
#define kEccNotificationDisable     0x00u
#define kEccCorrectionDisable       0x0002u
#define kEccClearError              0x01u

# define kEccUncorrectableError     0x0002u

# define FBL_ECC_base               (0xFFC62000ul)                      /**< Code flash ECC base address*/

# define FBL_ECC_base_VCI           (FBL_ECC_base + 0x0200u)            /**< Code flash VCI ECC base address */
# define FBL_ECC_base_PE1           (FBL_ECC_base + 0x0400u)            /**< Code flash PE1 ECC base address*/
# define FBL_ECC_base_PE2           (FBL_ECC_base + 0x0600u)            /**< Code flash PE1 ECC base address*/
# define FBL_ECC_base_PCU           (FBL_ECC_base + 0x0800u)            /**< Code flash PCU ECC base address*/

#  define FBL_ECC_base_used       FBL_ECC_base_PE1

/*
   ECC matrix
   ==========
   Types:
   #1   single core, SEC-DED, interrupt notifications
   #2   multi  core, SED-DED, interrupt notification
   #2*  single core, SED-DED, ECM notification, no test support, only 32 bit access
   #2** multi  core, SED-DED, address parity, ECM notification
*/

# define FBL_CFECCCTL_OFFSET        0x0000u /* Type #1: CFECCCTL; Type #2: CFECCCTL_PE<x> */
# define FBL_CFERRINT_OFFSET        0x0004u /* Type #2(+): CFERRINT_PE<x> */
# define FBL_CFSTCLR_OFFSET         0x0008u /* CFSTCLR_PE<x> */
# define FBL_CFFSTERSTR_OFFSET      0x0010u /* CF1STERSTR_PE<x> */

/* configure only ECC safe read relevant registers - Bootloader specific handling */
#define FBL_CODE_FLASH_ECC_CTL       (*(volatile tFblEccCtl*)   (FBL_ECC_base_used + FBL_CFECCCTL_OFFSET  ))  /**< Code flash ECC control register */
#define FBL_CODE_FLASH_ECC_ERRINT    (*(volatile tFblEccErrInt*)(FBL_ECC_base_used + FBL_CFERRINT_OFFSET  ))  /**< Code flash error notification control register */
# define FBL_CODE_FLASH_ECC_STCLR    (*(volatile vuint32*)      (FBL_ECC_base_used + FBL_CFSTCLR_OFFSET   ))  /**< Code flash status clear register */
# define FBL_CODE_FLASH_ECC_FSTERSTR (*(volatile vuint32*)      (FBL_ECC_base_used + FBL_CFFSTERSTR_OFFSET))  /**< Code flash ECC control register */

/* FBL_CODE_FLASH_ECC_ERROR_CLEAR clears error status, error overflow and error address (not applicable for Type #1) */
# define FBL_CODE_FLASH_ECC_ERROR_CLEAR()    FBL_CODE_FLASH_ECC_STCLR  = kEccClearError

typedef vuint32 tFblEccCtl;
typedef vuint32 tFblEccErrInt;
/* Data Flash ECC related registers ---------------------------------------------------------------- */
#define kEccDFProtWrite              0x4000u
#define kEccDFNotificationDisable    0x00u
#define kEccDFCorrectionDisable      0x0002u
#define kEccDFClearError             0x01u
#define kEccDFUncorrectableError     0x02u

#define FBL_ECC_DF_base              (0xFFC62C00ul)                    /**< Data flash ECC base address*/

#define FBL_DFECCCTL_OFFSET          0x0000u
#define FBL_DFERSTR_OFFSET           0x0004u
#define FBL_DFERSTC_OFFSET           0x0008u
#define FBL_DFERRINT_OFFSET          0x0014u

#define FBL_DATA_FLASH_ECC_CTL       (*(volatile tFblEccDFCtl*)   (FBL_ECC_DF_base + FBL_DFECCCTL_OFFSET  ))  /**< Data flash ECC control register */
#define FBL_DATA_FLASH_ECC_ERSTR     (*(volatile vuint8*)         (FBL_ECC_DF_base + FBL_DFERSTR_OFFSET   ))  /**< Data flash ECC error status register */
#define FBL_DATA_FLASH_ECC_ERSTC     (*(volatile vuint8*)         (FBL_ECC_DF_base + FBL_DFERSTC_OFFSET   ))  /**< Data flash ECC error status clear register */
#define FBL_DATA_FLASH_ECC_ERRINT    (*(volatile tFblEccErrDFInt*)(FBL_ECC_DF_base + FBL_DFERRINT_OFFSET  ))  /**< Data flash ECC error notification control register */

#define FBL_DATA_FLASH_ECC_ERROR_CLEAR()    FBL_DATA_FLASH_ECC_ERSTC  = kEccDFClearError

typedef vuint16 tFblEccDFCtl;
typedef vuint8  tFblEccErrDFInt;
/* ------------------------------------------------------------------------------------------------- */

# define FBL_FHVE3               (*(volatile vuint32*)0xFFF82410ul)   /**< FHVE3 control register */
# define FBL_FHVE15              (*(volatile vuint32*)0xFFF8A430ul)   /**< FHVE15 control register */

/* Protected write macro */
#define FBL_PROT_WRITEX(protreg, statreg, reg, val) do {                              /* Write to protected register */ \
                                                          (protreg)=(vuint32)0xA5u;   /* unlock protection */ \
                                                          (reg)=(vuint32)(val);       /* Write value */ \
                                                          (reg)=FblInvert32Bit(val);  /* Write inverse value */ \
                                                          (reg)=(vuint32)(val);       /* Write value */ \
                                                       } while (((statreg) & 0x01u) == 0x01u); /* repeat if write was not successfull */

# define FBL_PROT_WRITE1(reg, val)   FBL_PROT_WRITEX(FBL_PROT1PHCMD, FBL_PROT1PS, reg, val) /* Protected write macro PROT1PHCMD*/
/* FLMDPCMD -> FLMDCNT */
#define FBL_PROT_WRITE_FLMDCNT(val)  FBL_PROT_WRITEX(FBL_FLMDPCMD, FBL_FLMDPS, FBL_FLMDCNT, val) /* Protected write macro FLMDCNT*/
/* Port registers ---------------------------------------------------------------------------------- */

# define FBL_PORTn_base   0xFFC10000ul

# define FBL_P(n)        (*(volatile vuint16*)(FBL_PORTn_base + 0x0000u + ((n)*0x40u))) /**< Pn Port register */
# define FBL_PSR(n)      (*(volatile vuint16*)(FBL_PORTn_base + 0x0004u + ((n)*0x40u))) /**< Pn Port set/reset register */
# define FBL_PPR(n)      (*(volatile vuint16*)(FBL_PORTn_base + 0x000Cu + ((n)*0x40u))) /**< Pn Port pin read register */
# define FBL_PM(n)       (*(volatile vuint16*)(FBL_PORTn_base + 0x0010u + ((n)*0x40u))) /**< Pn Port mode register */
# define FBL_PMC(n)      (*(volatile vuint16*)(FBL_PORTn_base + 0x0014u + ((n)*0x40u))) /**< Pn Port mode control register */
# define FBL_PFC(n)      (*(volatile vuint16*)(FBL_PORTn_base + 0x0018u + ((n)*0x40u))) /**< Pn Port function control register */
# define FBL_PFCE(n)     (*(volatile vuint16*)(FBL_PORTn_base + 0x001Cu + ((n)*0x40u))) /**< Pn Port function expansion register */
# define FBL_PFCAE(n)    (*(volatile vuint16*)(FBL_PORTn_base + 0x0028u + ((n)*0x40u))) /**< Pn Port function addition expansion register */
# define FBL_PIBC(n)     (*(volatile vuint16*)(FBL_PORTn_base + 0x4000u + ((n)*0x40u))) /**< Pn Port input buffer control register */
# define FBL_PBDC(n)     (*(volatile vuint16*)(FBL_PORTn_base + 0x4004u + ((n)*0x40u))) /**< Pn Port bi-direction control register */
# define FBL_PIPC(n)     (*(volatile vuint16*)(FBL_PORTn_base + 0x4008u + ((n)*0x40u))) /**< Pn Port IP control register */
# define FBL_PODC(n)     (*(volatile vuint32*)(FBL_PORTn_base + 0x4014u + ((n)*0x40u))) /**< Pn Port Open Drain control register */

#define FBL_P0           FBL_P(0u)     /**< P0 Port register */
#define FBL_PSR0         FBL_PSR(0u)   /**< P0 Port set/reset register */
#define FBL_PPR0         FBL_PPR(0u)   /**< P0 Port pin read register */
#define FBL_PM0          FBL_PM(0u)    /**< P0 Port mode register */
#define FBL_PMC0         FBL_PMC(0u)   /**< P0 Port mode control register */
#define FBL_PFC0         FBL_PFC(0u)   /**< P0 Port function control register */
#define FBL_PFCE0        FBL_PFCE(0u)  /**< P0 Port function expansion register */
#define FBL_PFCAE0       FBL_PFCAE(0u) /**< P0 Port function addition expansion register */
#define FBL_PIBC0        FBL_PIBC(0u)  /**< P0 Port input buffer control register */
#define FBL_PBDC0        FBL_PBDC(0u)  /**< P0 Port bi-direction control register */
#define FBL_PIPC0        FBL_PIPC(0u)  /**< P0 Port IP control register */
#define FBL_PODC0        FBL_PODC(0u)  /**< P0 Port Open Drain control register */

#define FBL_P1           FBL_P(1u)     /**< P1 Port register */
#define FBL_PSR1         FBL_PSR(1u)   /**< P1 Port set/reset register */
#define FBL_PPR1         FBL_PPR(1u)   /**< P1 Port pin read register */
#define FBL_PM1          FBL_PM(1u)    /**< P1 Port mode register */
#define FBL_PMC1         FBL_PMC(1u)   /**< P1 Port mode control register */
#define FBL_PFC1         FBL_PFC(1u)   /**< P1 Port function control register */
#define FBL_PFCE1        FBL_PFCE(1u)  /**< P1 Port function expansion register */
#define FBL_PFCAE1       FBL_PFCAE(1u) /**< P1 Port function addition expansion register */
#define FBL_PIBC1        FBL_PIBC(1u)  /**< P1 Port input buffer control register */
#define FBL_PBDC1        FBL_PBDC(1u)  /**< P1 Port bi-direction control register */
#define FBL_PIPC1        FBL_PIPC(1u)  /**< P1 Port IP control register */
#define FBL_PODC1        FBL_PODC(1u)  /**< P1 Port Open Drain control register */

#define FBL_P2           FBL_P(2u)     /**< P2 Port register */
#define FBL_PSR2         FBL_PSR(2u)   /**< P2 Port set/reset register */
#define FBL_PPR2         FBL_PPR(2u)   /**< P2 Port pin read register */
#define FBL_PM2          FBL_PM(2u)    /**< P2 Port mode register */
#define FBL_PMC2         FBL_PMC(2u)   /**< P2 Port mode control register */
#define FBL_PFC2         FBL_PFC(2u)   /**< P2 Port function control register */
#define FBL_PFCE2        FBL_PFCE(2u)  /**< P2 Port function expansion register */
#define FBL_PFCAE2       FBL_PFCAE(2u) /**< P2 Port function addition expansion register */
#define FBL_PIBC2        FBL_PIBC(2u)  /**< P2 Port input buffer control register */
#define FBL_PBDC2        FBL_PBDC(2u)  /**< P2 Port bi-direction control register */
#define FBL_PIPC2        FBL_PIPC(2u)  /**< P2 Port IP control register */
#define FBL_PODC2        FBL_PODC(2u)  /**< P2 Port Open Drain control register */

#define FBL_P8           FBL_P(8u)     /**< P8 Port register */
#define FBL_PSR8         FBL_PSR(8u)   /**< P8 Port set/reset register */
#define FBL_PPR8         FBL_PPR(8u)   /**< P8 Port pin read register */
#define FBL_PM8          FBL_PM(8u)    /**< P8 Port mode register */
#define FBL_PMC8         FBL_PMC(8u)   /**< P8 Port mode control register */
#define FBL_PFC8         FBL_PFC(8u)   /**< P8 Port function control register */
#define FBL_PFCE8        FBL_PFCE(8u)  /**< P8 Port function expansion register */
#define FBL_PFCAE8       FBL_PFCAE(8u) /**< P8 Port function addition expansion register */
#define FBL_PIBC8        FBL_PIBC(8u)  /**< P8 Port input buffer control register */
#define FBL_PBDC8        FBL_PBDC(8u)  /**< P8 Port bi-direction control register */
#define FBL_PIPC8        FBL_PIPC(8u)  /**< P8 Port IP control register */
#define FBL_PODC8        FBL_PODC(8u)  /**< P8 Port Open Drain control register */

#define FBL_P9           FBL_P(9u)     /**< P9 Port register */
#define FBL_PSR9         FBL_PSR(9u)   /**< P9 Port set/reset register */
#define FBL_PPR9         FBL_PPR(9u)   /**< P9 Port pin read register */
#define FBL_PM9          FBL_PM(9u)    /**< P9 Port mode register */
#define FBL_PMC9         FBL_PMC(9u)   /**< P9 Port mode control register */
#define FBL_PFC9         FBL_PFC(9u)   /**< P9 Port function control register */
#define FBL_PFCE9        FBL_PFCE(9u)  /**< P9 Port function expansion register */
#define FBL_PFCAE9       FBL_PFCAE(9u) /**< P9 Port function addition expansion register */
#define FBL_PIBC9        FBL_PIBC(9u)  /**< P9 Port input buffer control register */
#define FBL_PBDC9        FBL_PBDC(9u)  /**< P9 Port bi-direction control register */
#define FBL_PIPC9        FBL_PIPC(9u)  /**< P9 Port IP control register */
#define FBL_PODC9        FBL_PODC(9u)  /**< P9 Port Open Drain control register */

#define FBL_P10          FBL_P(10u)    /**< P10 Port register */
#define FBL_PSR10        FBL_PSR(10u)  /**< P10 Port set/reset register */
#define FBL_PPR10        FBL_PPR(10u)  /**< P10 Port pin read register */
#define FBL_PM10         FBL_PM(10u)   /**< P10 Port mode register */
#define FBL_PMC10        FBL_PMC(10u)  /**< P10 Port mode control register */
#define FBL_PFC10        FBL_PFC(10u)  /**< P10 Port function control register */
#define FBL_PFCE10       FBL_PFCE(10u) /**< P10 Port function expansion register */
#define FBL_PFCAE10      FBL_PFCAE(10u)/**< P10 Port function addition expansion register */
#define FBL_PIBC10       FBL_PIBC(10u) /**< P10 Port input buffer control register */
#define FBL_PBDC10       FBL_PBDC(10u) /**< P10 Port bi-direction control register */
#define FBL_PIPC10       FBL_PIPC(10u) /**< P10 Port IP control register */
#define FBL_PODC10       FBL_PODC(10u) /**< P10 Port Open Drain control register */

#define FBL_P11          FBL_P(11u)    /**< P11 Port register */
#define FBL_PSR11        FBL_PSR(11u)  /**< P11 Port set/reset register */
#define FBL_PPR11        FBL_PPR(11u)  /**< P11 Port pin read register */
#define FBL_PM11         FBL_PM(11u)   /**< P11 Port mode register */
#define FBL_PMC11        FBL_PMC(11u)  /**< P11 Port mode control register */
#define FBL_PFC11        FBL_PFC(11u)  /**< P11 Port function control register */
#define FBL_PFCE11       FBL_PFCE(11u) /**< P11 Port function expansion register */
#define FBL_PFCAE11      FBL_PFCAE(11u)/**< P11 Port function addition expansion register */
#define FBL_PIBC11       FBL_PIBC(11u) /**< P11 Port input buffer control register */
#define FBL_PBDC11       FBL_PBDC(11u) /**< P11 Port bi-direction control register */
#define FBL_PIPC11       FBL_PIPC(11u) /**< P11 Port IP control register */
#define FBL_PODC11       FBL_PODC(11u) /**< P11 Port Open Drain control register */

#define FBL_P12          FBL_P(12u)    /**< P12 Port register */
#define FBL_PSR12        FBL_PSR(12u)  /**< P12 Port set/reset register */
#define FBL_PPR12        FBL_PPR(12u)  /**< P12 Port pin read register */
#define FBL_PM12         FBL_PM(12u)   /**< P12 Port mode register */
#define FBL_PMC12        FBL_PMC(12u)  /**< P12 Port mode control register */
#define FBL_PFC12        FBL_PFC(12u)  /**< P12 Port function control register */
#define FBL_PFCE12       FBL_PFCE(12u) /**< P12 Port function expansion register */
#define FBL_PFCAE12      FBL_PFCAE(12u)/**< P12 Port function addition expansion register */
#define FBL_PIBC12       FBL_PIBC(12u) /**< P12 Port input buffer control register */
#define FBL_PBDC12       FBL_PBDC(12u) /**< P12 Port bi-direction control register */
#define FBL_PIPC12       FBL_PIPC(12u) /**< P12 Port IP control register */
#define FBL_PODC12       FBL_PODC(12u) /**< P12 Port Open Drain control register */

#define FBL_P18          FBL_P(18u)    /**< P18 Port register */
#define FBL_PSR18        FBL_PSR(18u)  /**< P18 Port set/reset register */
#define FBL_PPR18        FBL_PPR(18u)  /**< P18 Port pin read register */
#define FBL_PM18         FBL_PM(18u)   /**< P18 Port mode register */
#define FBL_PMC18        FBL_PMC(18u)  /**< P18 Port mode control register */
#define FBL_PFC18        FBL_PFC(18u)  /**< P18 Port function control register */
#define FBL_PFCE18       FBL_PFCE(18u) /**< P18 Port function expansion register */
#define FBL_PFCAE18      FBL_PFCAE(18u)/**< P18 Port function addition expansion register */
#define FBL_PIBC18       FBL_PIBC(18u) /**< P18 Port input buffer control register */
#define FBL_PBDC18       FBL_PBDC(18u) /**< P18 Port bi-direction control register */
#define FBL_PIPC18       FBL_PIPC(18u) /**< P18 Port IP control register */
#define FBL_PODC18       FBL_PODC(18u) /**< P18 Port Open Drain control register */

#define FBL_P20          FBL_P(20u)    /**< P20 Port register */
#define FBL_PSR20        FBL_PSR(20u)  /**< P20 Port set/reset register */
#define FBL_PPR20        FBL_PPR(20u)  /**< P20 Port pin read register */
#define FBL_PM20         FBL_PM(20u)   /**< P20 Port mode register */
#define FBL_PMC20        FBL_PMC(20u)  /**< P20 Port mode control register */
#define FBL_PFC20        FBL_PFC(20u)  /**< P20 Port function control register */
#define FBL_PFCE20       FBL_PFCE(20u) /**< P20 Port function expansion register */
#define FBL_PFCAE20      FBL_PFCAE(20u)/**< P20 Port function addition expansion register */
#define FBL_PIBC20       FBL_PIBC(20u) /**< P20 Port input buffer control register */
#define FBL_PBDC20       FBL_PBDC(20u) /**< P20 Port bi-direction control register */
#define FBL_PIPC20       FBL_PIPC(20u) /**< P20 Port IP control register */
#define FBL_PODC20       FBL_PODC(20u) /**< P20 Port Open Drain control register */

#define FBL_P21          FBL_P(21u)    /**< P21 Port register */
#define FBL_PSR21        FBL_PSR(21u)  /**< P21 Port set/reset register */
#define FBL_PRR21        FBL_PRR(21u)  /**< P21 Port pin read register */
#define FBL_PM21         FBL_PM(21u)   /**< P21 Port mode register */
#define FBL_PMC21        FBL_PMC(21u)  /**< P21 Port mode control register */
#define FBL_PFC21        FBL_PFC(21u)  /**< P21 Port function control register */
#define FBL_PFCE21       FBL_PFCE(21u) /**< P21 Port function expansion register */
#define FBL_PIBC21       FBL_PIBC(21u) /**< P21 Port input buffer control register*/
#define FBL_PBDC21       FBL_PBDC(21u) /**< P21 Port bi-direction control register */
#define FBL_PIPC21       FBL_PIPC(21u) /**< P21 Port IP control register */
#define FBL_PU21         FBL_PU(21u)   /**< Pull-up option register */
#define FBL_PDSC21       FBL_PDSC(21u) /**< Port drive strength control register */
#define FBL_PODC21       FBL_PODC(21u) /**< P21 Port Open Drain control register */

#define FBL_P44          FBL_P(44u)    /**< P44 Port register */
#define FBL_PM44         FBL_PM(44u)   /**< P44 Port mode register */

#define FBL_P45          FBL_P(45u)    /**< P45 Port register */
#define FBL_PM45         FBL_PM(45u)   /**< P45 Port mode register */

# define FBL_JPORT0_BASE_ADDRESS  0xFFC20000ul /**< JPort group number n == 0 */
# define FBL_JPORTM0_OFFSET       0x00000010ul /**< JPortM offset */

# define FBL_JP0       (*(volatile vuint8*)(FBL_JPORT0_BASE_ADDRESS + 0x00u ))               /**< JP0 Port register*/
# define FBL_JPM0      (*(volatile vuint8*)(FBL_JPORT0_BASE_ADDRESS + FBL_JPORTM0_OFFSET ))  /**< JP0 Port mode register*/

/* Timer registers --------------------------------------------------------------------------------- */
# define FBL_TAUDn_user_base   0xFFE20000ul  /* TAUDn_base */
# define FBL_TAUXn_user_base   FBL_TAUDn_user_base

#define FBL_TAUX0CDR0    (*(volatile vuint16*)(FBL_TAUXn_user_base + 0x0000u)) /**< TAUX0 channel data register 0 */
#define FBL_TAUX0CDR1    (*(volatile vuint16*)(FBL_TAUXn_user_base + 0x0004u)) /**< TAUX0 channel data register 1 */
#define FBL_TAUX0CDR2    (*(volatile vuint16*)(FBL_TAUXn_user_base + 0x0008u)) /**< TAUX0 channel data register 2 */

#define FBL_TAUX0CNT0    (*(volatile vuint16*)(FBL_TAUXn_user_base + 0x0080u)) /**< TAUX0 channel counter register 0 */
#define FBL_TAUX0CNT1    (*(volatile vuint16*)(FBL_TAUXn_user_base + 0x0084u)) /**< TAUX0 channel counter register 1 */
#define FBL_TAUX0CNT2    (*(volatile vuint16*)(FBL_TAUXn_user_base + 0x0088u)) /**< TAUX0 channel counter register 2 */

#define FBL_TAUX0CMOR0   (*(volatile vuint16*)(FBL_TAUXn_user_base + 0x0200u)) /**< TAUX0 channel mode OS register 0 */
#define FBL_TAUX0CMOR1   (*(volatile vuint16*)(FBL_TAUXn_user_base + 0x0204u)) /**< TAUX0 channel mode OS register 1 */
#define FBL_TAUX0CMOR2   (*(volatile vuint16*)(FBL_TAUXn_user_base + 0x0208u)) /**< TAUX0 channel mode OS register 2 */

#define FBL_TAUX0CMUR0   (*(volatile vuint16*)(FBL_TAUXn_user_base + 0x00C0u)) /**< TAUX0 channel mode user register 0 */
#define FBL_TAUX0CMUR1   (*(volatile vuint16*)(FBL_TAUXn_user_base + 0x00C4u)) /**< TAUX0 channel mode user register 1 */
#define FBL_TAUX0CMUR2   (*(volatile vuint16*)(FBL_TAUXn_user_base + 0x00C8u)) /**< TAUX0 channel mode user register 2 */

#define FBL_TAUX0CSR0    (*(volatile vuint16*)(FBL_TAUXn_user_base + 0x0140u)) /**< TAUX0 channel status register 0 */
#define FBL_TAUX0CSR1    (*(volatile vuint16*)(FBL_TAUXn_user_base + 0x0144u)) /**< TAUX0 channel status register 1 */
#define FBL_TAUX0CSR2    (*(volatile vuint16*)(FBL_TAUXn_user_base + 0x0148u)) /**< TAUX0 channel status register 2 */

#define FBL_TAUX0TPS     (*(volatile vuint16*)(FBL_TAUXn_user_base + 0x0240u)) /**< TAUX0 channel prescaler clock divider register */
#define FBL_TAUX0TS      (*(volatile vuint16*)(FBL_TAUXn_user_base + 0x01C4u)) /**< TAUX0 channel start trigger register*/
#define FBL_TAUX0TT      (*(volatile vuint16*)(FBL_TAUXn_user_base + 0x01C8u)) /**< TAUX0 channel stop trigger register*/

#define FBL_TAUX0RDE     (*(volatile vuint16*)(FBL_TAUXn_user_base + 0x0260u)) /**< TAUX0 channel reload data enable */
#define FBL_TAUX0RDM     (*(volatile vuint16*)(FBL_TAUXn_user_base + 0x0264u)) /**< TAUX0 channel reload data mode */
#define FBL_TAUX0RDS     (*(volatile vuint16*)(FBL_TAUXn_user_base + 0x0268u)) /**< TAUX0 channel reload data control channel select */
#define FBL_TAUX0RDC     (*(volatile vuint16*)(FBL_TAUXn_user_base + 0x026Cu)) /**< TAUX0 channel reload data control */

/* Interrupt control registers --------------------------------------------------------------------- */

# define FBL_ICTAUX0I0   (*(volatile vuint16*)0xFFFFB11Aul) /**< TAUX0 interrupt control register */

/* Reset cause registers --------------------------------------------------------------------------- */
# define FBL_RESF        (*(volatile vuint32*)0xFFF82800ul) /**< RESF - Init with 0x00000000 by POCRES */
# define FBL_RESFC       (*(volatile vuint32*)0xFFF82808ul) /**< RESFC                                 */
# define FBL_SWRESA      (*(volatile vuint32*)0xFFF8AC18ul) /**< SWRESA                                */
/* Software Reset ---------------------------------------------------------------------------------- */
#define FBL_SW_RESET() FBL_PROT_WRITE1(FBL_SWRESA, 0x01); /* Cause software reset */
/* Watchdog related registers ---------------------------------------------------------------------- */

/* Base addresses */
# define FBL_WDTA0       0xFFD74000ul                              /**< WDTA0 Base address */

/* WDTA0 */
#define FBL_WDTA0WDTE    (*(volatile vuint8 *)(FBL_WDTA0 + 0x00u)) /**< WDTA enable register */
#define FBL_WDTA0EVAC    (*(volatile vuint8 *)(FBL_WDTA0 + 0x04u)) /**< WDTA enable VAC register */
#define FBL_WDTA0REF     (*(volatile vuint8 *)(FBL_WDTA0 + 0x08u)) /**< WDTA reference value register */
#define FBL_WDTA0MD      (*(volatile vuint8 *)(FBL_WDTA0 + 0x0Cu)) /**< WDTA mode register */
/* WDTA1 */
#endif /* V_CPU_RH850 */

#if defined( FBL_ENABLE_BUSTYPE_CAN )
/* ---- CAN related section ------------------------------------------------------------------------ */

#  if defined( V_CPU_RH850 )
#   if !defined( MEMORY_CAN )
#    define MEMORY_CAN
#   endif

/* Constants of CAN-cell structure */
#   define kCanMaxPhysChannelsCell     8u
#   define kCanMaxRxFifosCell          8u
#   define kCanMaxTxFifosCell          3u  /* per channel */
#   define kCanMaxRxBufCell            128u
#   define kCanMaxTxBufCell            16u /* per channel */
#   define kCanNumberOfSharedReg8Cell  2u
#   define kCanNumberOfSharedReg16Cell 4u
#   define kCanMaxHwObjectsCell        288u
#   define kCanMaxRuleAccessCell       16u
#   define kCanMaxRamTestAccessCell    64u
#   define kCanRegSize                 vuint32
#  endif /* V_CPU_RH850 */

#  if defined( V_CPU_RH850 )
#   define mRuleCode(idx)        Can->Rule[idx].Code
#   define mRuleMask(idx)        Can->Rule[idx].Mask
#   define mRuleBuf(idx)         Can->Rule[idx].Buf
#   define mRuleFifo(idx)        Can->Rule[idx].Fifo
#   define mCRAMTR(idx)          Can->CRAMTR[idx]
#   define mBufId(idx)           Can->Buf[idx].Id
#   define mBufDlc(idx)          Can->Buf[idx].Dlc
#   define mBufbData(bidx,didx)  Can->Buf[bidx].u.bData[didx]
#   define mBufwData(bidx,didx)  Can->Buf[bidx].u.wData[didx]
#   define mBufiData(bidx,didx)  Can->Buf[bidx].u.iData[didx]
#   define mTHAR(idx)            Can->THAR[idx]
#  endif /* V_CPU_RH850 */

/***********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

typedef volatile struct    /**< control and status*/
{
   volatile vuint32 BCFG;
   volatile vuint32 CR;
   volatile vuint32 SR;
   volatile vuint32 EF;
} tCanChCtrl;

typedef volatile struct    /**< FIFO control */
{
   volatile vuint32 TRFCR[kCanMaxTxFifosCell];
} tCanChFC;

typedef volatile struct    /**< FIFO status */
{
   volatile kCanRegSize  TRFSR[kCanMaxTxFifosCell];
} tCanChFS;

typedef volatile struct    /**< FIFO pointer control */
{
   volatile kCanRegSize  TRFPCR[kCanMaxTxFifosCell];
} tCanChFPC;

typedef volatile struct    /**< Buffer control */
{
   volatile vuint8 TBCR[kCanMaxTxBufCell];
} tCanChBC;

typedef volatile struct    /**< Buffer status */
{
   volatile vuint8 TBSR[kCanMaxTxBufCell];
} tCanChBS;

typedef volatile struct
{
   volatile vuint32      Code;            /**< Rule register A - ID code */
   volatile vuint32      Mask;            /**< Rule register B - ID mask */
   volatile vuint32      Buf;             /**< Rule register C - DLC, label, buffer */
# if defined( V_CPU_RH850 )
   volatile vuint32      Fifo;            /**< Rule register D - FIFO */
# endif
} tCanRule;

typedef volatile struct
{
   volatile vuint32      Id;              /**< Buffer/FIFO register A - ID */
# if defined( V_CPU_RH850 )
   volatile kCanRegSize  Dlc;             /**< Buffer/FIFO register B - DLC, label, timestamp */
#  endif
   union{ /* PRQA S 0750 */ /* MD_MSR_18.4 */
      volatile vuint8    bData[8];        /**< Buffer/FIFO register C and D - Data */
      volatile vuint16   wData[4];        /**< Buffer/FIFO register C and D - Data */
      volatile vuint32   iData[2];        /**< Buffer/FIFO register C and D - Data */
   } u;
} tCanMsgObj;

#  if defined( V_CPU_RH850 )
typedef volatile struct
{
   tCanChCtrl            ChCtrl[kCanMaxPhysChannelsCell];       /**< Control and status */
   volatile kCanRegSize  Unused01[1];
   volatile vuint32      CGCFG;                                 /**< CAN global configuration register */
   volatile vuint32      CGCR;                                  /**< CAN global control register */
   volatile kCanRegSize  CGSR;                                  /**< CAN global status register */
   volatile kCanRegSize  CGEF;                                  /**< CAN global error flag register */
   volatile kCanRegSize  CTS;                                   /**< Global timestamp counter register */
   volatile kCanRegSize  CRECR;                                 /**< Global acceptance filter list entry control register */
   volatile kCanRegSize  CRNCFG[kCanNumberOfSharedReg8Cell];
   volatile kCanRegSize  CRBNUM;
   volatile kCanRegSize  CRBRCF[kCanNumberOfSharedReg16Cell];
   volatile kCanRegSize  CRFCR[kCanMaxRxFifosCell];             /**< Fifo control and status */
   volatile kCanRegSize  CRFSR[kCanMaxRxFifosCell];
   volatile kCanRegSize  CRFPCR[kCanMaxRxFifosCell];
   tCanChFC              ChFC[kCanMaxPhysChannelsCell];
   tCanChFS              ChFS[kCanMaxPhysChannelsCell];
   tCanChFPC             ChFPC[kCanMaxPhysChannelsCell];
   volatile kCanRegSize  CFESR;
   volatile kCanRegSize  CFFSR;
   volatile kCanRegSize  CFMLSR;
   volatile kCanRegSize  CRFISR;
   volatile kCanRegSize  CTRFRISR;
   volatile kCanRegSize  CTRFTISR;
   tCanChBC              ChBC[kCanMaxPhysChannelsCell];         /**< Buffer control and status */
   tCanChBS              ChBS[kCanMaxPhysChannelsCell];
   volatile kCanRegSize  CTBTRSR[kCanNumberOfSharedReg16Cell];
   volatile kCanRegSize  CTBARSR[kCanNumberOfSharedReg16Cell];
   volatile kCanRegSize  CTBTCSR[kCanNumberOfSharedReg16Cell];
   volatile kCanRegSize  CTBASR[kCanNumberOfSharedReg16Cell];
   volatile kCanRegSize  CTBIER[kCanNumberOfSharedReg16Cell];
   volatile kCanRegSize  TQCR[kCanMaxPhysChannelsCell];         /**< Tx queue control and status */
   volatile kCanRegSize  TQSR[kCanMaxPhysChannelsCell];
   volatile kCanRegSize  TQPCR[kCanMaxPhysChannelsCell];
   volatile kCanRegSize  THCR[kCanMaxPhysChannelsCell];         /**< Tx history control and status */
   volatile kCanRegSize  THSR[kCanMaxPhysChannelsCell];
   volatile kCanRegSize  THPCR[kCanMaxPhysChannelsCell];
   volatile kCanRegSize  CGTINTSTS[kCanNumberOfSharedReg8Cell]; /**< Tx interrupt status, global test */
   volatile kCanRegSize  CGTCFG;
   volatile kCanRegSize  CGTCR;
   volatile kCanRegSize  Unused02[3];
   volatile kCanRegSize  CGTPUR;
   volatile kCanRegSize  Unused03[32];
   tCanRule              Rule[kCanMaxRuleAccessCell];           /**< Reception rules */
   tCanMsgObj            Buf[kCanMaxHwObjectsCell];             /**< Mailbox access */
   volatile kCanRegSize  THAR[kCanMaxPhysChannelsCell];
   volatile kCanRegSize  Unused04[56];
   volatile vuint32      CRAMTR[kCanMaxRamTestAccessCell];      /**< Ram test */
} tCanCell;
#  endif /* V_CPU_RH850 */

typedef tCanCell * tCanCellPtr;

#endif /* FBL_ENABLE_BUSTYPE_CAN */

#endif /* __FBL_SFR_H__ */

/* Module specific MISRA deviations:

   MD_FblSfr_MemoryMappedRegister:
      Reason:     No identifiable risk as casts are used to define SFRs here.
      Risk:       [I] Cast between a pointer to volatile object and an integral type.
      Prevention: No prevention required.

   MD_FblSfr_3410:
      Reason:     This macro is used as type qualifier and must not have parentheses.
      Risk:       Side effects which result in unintended code behavior.
      Prevention: Careful reviews.
 */
/***********************************************************************************************************************
 *  END OF FILE: FBL_SFR.H
 **********************************************************************************************************************/

