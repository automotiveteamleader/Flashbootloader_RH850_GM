/* -----------------------------------------------------------------------------
  Filename:    fbl_cfg.h
  Description: Toolversion: 06.04.03.01.50.06.35.03.00.00
               
               Serial Number: CBD1500635
               Customer Info: Nexteer Automotive Corporation
                              Package: FBL Gm SLP6
                              Micro: Renesas RH850 P1M R7F701363
                              Compiler: Green Hills 2015.1.7
               
               
               Generator Fwk   : GENy 
               Generator Module: GenTool_GenyFblCanBase
               
               Configuration   : D:\usr\usage\Delivery\CBD15x\CBD1500635\Dxx\external\Demo\DemoFbl\Config\DemoFbl.gny
               
               ECU: 
                       TargetSystem: Hw_Rh850Cpu
                       Compiler:     GreenHills
                       Derivates:    P1M
               
               Channel "Channel0":
                       Databasefile: D:\usr\usage\Delivery\CBD15x\CBD1500635\DXX\external\Demo\DemoFbl\Config\demo_sw.dbc
                       Bussystem:    CAN
                       Manufacturer: General Motors
                       Node:         DUT

 ----------------------------------------------------------------------------- */
/* -----------------------------------------------------------------------------
  C O P Y R I G H T
 -------------------------------------------------------------------------------
  Copyright (c) 2001-2015 by Vector Informatik GmbH. All rights reserved.
 
  This software is copyright protected and proprietary to Vector Informatik 
  GmbH.
  
  Vector Informatik GmbH grants to you only those rights as set out in the 
  license conditions.
  
  All other rights remain with Vector Informatik GmbH.
 -------------------------------------------------------------------------------
 ----------------------------------------------------------------------------- */

#if !defined(__FBL_CFG_H__)
#define __FBL_CFG_H__

/* -----------------------------------------------------------------------------
    &&&~ 
 ----------------------------------------------------------------------------- */

#define FBL_DISABLE_STAY_IN_BOOT
#define FBL_USE_OWN_MEMCPY
#define FBL_WATCHDOG_ON
#define FBL_WATCHDOG_TIME                    (1 / FBL_REPEAT_CALL_CYCLE)
#define FBL_HEADER_ADDRESS                   0x0100u
#define FBL_ENABLE_APPL_TASK
#define SWM_DATA_MAX_NOAR                    10
#define FBL_DIAG_BUFFER_LENGTH               4095
#define FBL_ENABLE_SLEEPMODE
#define kFillChar                            0xC3u
#define FBL_DISABLE_MULTIPLE_MEM_DEVICES
#define FBL_ENABLE_FBL_START
#define FBL_ENABLE_COMMON_DATA
#define FBL_MTAB_NO_OF_BLOCKS                1
/* Data Processing */
#define FBL_DISABLE_DATA_PROCESSING
#define FBL_DISABLE_COMPRESSION_MODE
#define FBL_MEM_PROC_BUFFER_SIZE             0x04B0
/* Project State */
#define FBL_INTEGRATION                      2
#define FBL_PRODUCTION                       1
#define FBL_PROJECT_STATE                    FBL_INTEGRATION
#define FBL_ENABLE_SYSTEM_CHECK
#define FBL_ENABLE_DEBUG_STATUS
#define FBL_ENABLE_ASSERTION
/* FblLib_Mem */
#define FBL_DISABLE_PIPELINED_PROGRAMMING
#define FBL_MEM_WRITE_SEGMENTATION           0x0100
#define FBL_ENABLE_UNALIGNED_DATA_TRANSFER
/* CAN Identifier */

/* RH850 specific ************************************************************ */
#define FLASH_SIZE                           9000
#define FBL_TIMER_PRESCALER_VALUE            0x01
#define FBL_TIMER_RELOAD_VALUE               0x9C3F
#define FBL_SYSTEM_FREQUENCY                 160
#define FLASH_ENABLE_MACHINE_CHECK_ECC_DETECTION

#define CAN_BCFG                             0x140009
#define kFblCanBaseAdr                       0xFFD20000u
#define kFblCanMaxPhysChannels               6u
#define FBL_HW_DISABLE_ALTERNATIVE_CLOCK_SOURCE
#define FBL_CAN_0
#define kFblCanChannel                       0x00

/* OEM specific ************************************************************** */
/* FBL multiple nodes support: */
#define FBL_DISABLE_MULTIPLE_NODES
#define DIAG_NODE_ADDRESS                    0x45

#define FBL_DISABLE_DIRECT_MEMORY_ACCESS
#define FBL_ENABLE_CAN_CONFIGURATION
#define FBL_ENABLE_RAM_INTEGRITY_CHECK
#define FBL_RAM_INTEGRITY_START_ADDR         0x00u
#define FBL_RAM_INTEGRITY_END_ADDR           0x00u
#define FBL_ENABLE_ROM_INTEGRITY_CHECK
#define FBL_ENABLE_GAP_FILL
#define FBL_RESET_MS_DELAY                   30
#define FBL_DISABLE_INITIATE_DIAG_OP
#define FBL_DISABLE_READ_MEMORY_BY_ADDRESS
#define FBL_DISABLE_WRITE_DATA_BY_IDENTIFIER
#define FBL_ENABLE_SECURITY_ACCESS
#define kDiagRqlReadMemoryByAddressAddressParameter 4
#define kDiagRqlRequestDownloadMemorySizeParameter 4
#define kDiagRqlTransferDataStartingAddressParameter 4
#define FBL_DISABLE_HIGHSPEED_MODE
#define FBL_ENABLE_CALIBRATION_MODULES
#define FBL_DISABLE_MULTI_PROCESSOR
#define FBL_GMHEADER_CSUM                    { 0x00, 0x00 }
#define FBL_GMHEADER_MODID_FBL               { 0x00, 0x47 }
#define FBL_GMHEADER_SWMI_FBL                { 0x11, 0x22, 0x33, 0x44 }
#define FBL_GMHEADER_DLS_FBL                 { 0x00, 0x00 }
#define FBL_GMHEADER_DCID_FBL                { 0x80, 0x00 }
/* Ecu Name 'DemoFbl1' in ASCII */
#define FBL_GMHEADER_ECUNAME                 { 0x44, 0x65, 0x6d, 0x6f, 0x46, 0x62, 0x6c, 0x31 }
#define FBL_GMHEADER_ECUID                   { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01 }
/* Subject Name 'Engine' in ASCII */
#define FBL_GMHEADER_SUBJNAME                { 0x45, 0x6e, 0x67, 0x69, 0x6e, 0x65, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
#define FBL_GMHEADER_NOAR_FBL                1
#define FBL_GMHEADER_ADDR_REG                { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }


/* -----------------------------------------------------------------------------
    &&&~ 
 ----------------------------------------------------------------------------- */

/* User Config File ********************************************************** */
/***********************************************************************************************************************
 *   This is the GENy OEM user config file for GM SLP6 GENy (_MandatoryDeliverPreconfig.cfg).
 *   This file should be modified according to your requirements. It need to be added to your GENy configuration
 *   to FblDrvCan_XX->User Config File
 *......................................................................................................................
 * History    2014-06     01.00.00   A. Wenckebach (Vector)     Creation
 *            2014-09-26  01.00.01   A. Wenckebach              changed default tester node address from F1 to F5
 *            2014-10-16  01.00.02   A. Wenckebach              Move fix elements to preconfig
 *            2014-11-26  01.00.03   A. Wenckebach              Correct comment, add undef for FBL_CAN_FUNC_RXID
 *            2014-12-12  01.01.00   A. Wenckebach              Add multiple nodes use case.
 *            2015-06-10  01.02.00   A. Wenckebach              Add example to overwrite P2 timeout.
 *            2016-04-15  01.03.00   A. Wenckebach              Consider GB6002 different P2 timing definitions.
 *            2016-07-25  01.04.00   A. Wenckebach              Changes towards Cw 2.xx config, add tester node addr 6
 *            2016-12-14  01.05.00   A. Wenckebach              Allow for more flexible 11-bit CanId configuration
 *            2017-03-30  01.06.00   A. Wenckebach              Update default 11-bit configto GB6000 2017 (January 27th)
 *                                                               Remove tester F6
 *            2017-04-04  01.07.00   A. Wenckebach              Correct internal versions and release dates
 *            2017-04-07  01.08.00   A. Wenckebach              Correct wrongly set comment
 ***********************************************************************************************************************/

/**************************************************/
/** Diagnostic Timing related                    **/
/**************************************************/

/* Overwrite default P2*Server. The default according to GB6002 V1.3 is set automatically, without overwriting necessary.
 * Note that in GB6002 versions prior to V1.3 5000ms timing was inherited from GB6000. */
/* #define FBL_DIAG_TIME_P3MAX 5000u */

/**************************************************/
/** CAN Id related items                         **/
/**************************************************/

/* Unset DIAG_NODE_ADDRESS */
#if defined ( DIAG_NODE_ADDRESS )
# undef DIAG_NODE_ADDRESS
#endif

/* The below addresses are used for software tester id filtering in ApplFblCheckTesterSourceAddr() */
#define DIAG_TESTER_NODE_ADDR1               0x000000F1ul /* Can offboard Id1 */
#define DIAG_TESTER_NODE_ADDR2               0x000000F2ul /* Can offboard Id2 */
#define DIAG_TESTER_NODE_ADDR3               0x000000F3ul /* Can onboard Id1 */
#define DIAG_TESTER_NODE_ADDR4               0x000000F4ul /* Ethernet onboard Id1 */
#define DIAG_TESTER_NODE_ADDR5               0x000000F5ul /* Etherned offboard Id1 */

/* Used for 11-bit requests, response will be  */
#define FBL_CW_FIXED_FLOWCONTROL_TARGET_ADDR   0x000000FCul  /* Target Address used for Flow Control */
#define FBL_DIAG_RESP_TARGADDR_11BIT_INIT      0x000000F1ul  /* Shall be tester F1 after clarification with GM */
#if !defined(FBL_CW_FIXED_RESPONSE_TARGET_ADDR)
# define FBL_CW_FIXED_RESPONSE_TARGET_ADDR     FblDiagGetTesterAddr()
#endif /* FBL_CW_FIXED_RESPONSE_TARGET_ADDR */

/* Customer TODO:
 * Please choose DIAG_NODE_ADDRESS for your ECU (DIAG_NODE_ADDRESS_2ND in case of Multiple Nodes configuration)  */
#define DIAG_NODE_ADDRESS                    0x00000045ul
# define DIAG_NODE_ADDRESS_2ND               0x00000046ul

/**************************************************/
/** Secured download related configuration items **/
/**************************************************/
/* Develop feature to verify Integrity word in header. Check User Docu for further information*/
/* #define FBL_ENABLE_VERIFY_INTEGRITY_WORD */


/**************************************************/
/** NVM (test) configuration items               **/
/**************************************************/
/* Develop features to populate NVM on bootloader start  Check User Docu for further information*/

/* This can be used for test purpose to Write SBA ticket from flash sbaBlk0 to NVM */
/* #define  FBL_TEST_SBA_TICKET */

/* This can be used for test purpose to Write Ecu-Id (if you dont change it: from FblHeader).
 * Note: The FblHeader field ECUID could be used to fullfill the requirement, if the
 * content is modified in the hexfile for each controller part to be unique.
 */
/* #define  FBL_TEST_ECU_ID */

/* writes 0x0000 key NBID on startup */
/* #define  FBL_TEST_KEY_NBID */

/* writes 0x0000 app NBID on startup */
/* #define FBL_TEST_APP_NBID */

/**************************************************/
/** Other items                                  **/
/**************************************************/
#if defined( FBL_USE_FBL_AS_DEMOAPPL )
# include "fbl_demo_appl.h"
#endif

#if defined (FBL_ENABLE_COMPRESSION_MODE)
/* Add the compression method that fits to the delivery */
/* # define DATA_TYPE_COMPRESSED_ARLE */
# define DATA_TYPE_COMPRESSED_LZMA
#endif

/* Hardware specific: Required to (re-)use Settings that have been used with the environment @Vector */
#define FBL_ENABLE_VECTOR_HW
#define CUSTOMER_HW

/* Enable Fee Wrapper Functions */
/* #define FBL_APNV_USE_FUNCTIONS */

/* Enable Fee NVM Handling */
/* #define FBL_HDR_DISABLE_BASIC_NVM_HANDLING */

#if !defined( FBL_HDR_DISABLE_BASIC_NVM_HANDLING )
/* No EEPROM or FEE is used. Handle NBIDs in Flash */
/* Reserve XXXX bytes length. Allows for max XXXX byte/(2 * MEM_SEGMENT_SIZE) = YYYY updates each. */
# define FBL_NBID_MEMORY_BASE        0x10000ul /* Start of NBID area (erased, protected mem, compare TechRef) */
# define FBL_NBID_TOTAL_MEMORY_LEN   0x10000ul /* XXXX bytes length */
#endif

/**********************************************************/
/** End of user conf. file _MandatoryDeliverPreconfig.cfg */
/**********************************************************/
/* User Section ************************************************************** */
#define FBL_ENABLE_CAN_CONFIRMATION
/* Increase default buffer size to allow Sba ticket to fit in.
 * (This is only relevant in case of Dummy Eeprom usage, usually this value can be ignored). */
#define EEP_BUFFER_SIZE 0x400

/* Set Diagnostic session timing ( overwritable via user preconfig, e.g. for OBD/EOBD relevant devices ) */
#if !defined( FBL_DIAG_TIME_P2MAX )
# define FBL_DIAG_TIME_P2MAX 100u /* GB6002 V1.3 defines 100ms max P2Server timing. */
#endif
#if !defined( FBL_DIAG_TIME_P3MAX )
# define FBL_DIAG_TIME_P3MAX 30000u /* GB6002 V1.3 defines 30000ms max P2*Server timing. */
#endif

/* Define Id configuration */
#if defined ( FBL_ENABLE_STDID_OPTIMIZATION )
# undef FBL_ENABLE_STDID_OPTIMIZATION
#endif
#if defined ( FBL_ENABLE_EXTID_OPTIMIZATION )
# undef FBL_ENABLE_EXTID_OPTIMIZATION
#endif
#if !defined ( FBL_DISABLE_STDID_OPTIMIZATION )
# define FBL_DISABLE_STDID_OPTIMIZATION
#endif
#if !defined ( FBL_DISABLE_EXTID_OPTIMIZATION )
# define FBL_DISABLE_EXTID_OPTIMIZATION
#endif

#if defined( FBL_ENABLE_MULTIPLE_NODES )
# undef FBL_ENABLE_MULTIPLE_NODES
# define FBL_CW_ENABLE_MULTIPLE_NODES
#endif

#define kCanTxOk kFblCanTxOk

#if defined( FBL_ENABLE_STAY_IN_BOOT )
# if !defined( CAN_RX_SLOT_STARTMSG )
/* For some hw files this is 'CAN_RX_SLOT_RANGE_0', please overwrite CAN_RX_SLOT_STARTMSG to this, *
 * if you get a compile error for the below                                                        */
#  define CAN_RX_SLOT_STARTMSG CAN_RX_RANGE_0
# endif /* CAN_RX_SLOT_STARTMSG */
#endif /* FBL_ENABLE_STAY_IN_BOOT */


/* Security module related */
#if defined ( FBL_ENABLE_VERIFY_INTEGRITY_WORD )
  /* SEC_ENABLE_SECURITY_CLASS_VENDOR required when using FBL_ENABLE_VERIFY_INTEGRITY_WORD */
# define SEC_ENABLE_SECURITY_CLASS_VENDOR
#endif
#define SEC_ENABLE_WORKSPACE_INTERNAL
#define SEC_ENABLE_WORKSPACE_EXTERNAL
#define SEC_ENABLE_VERIFICATION_KEY_EXTERNAL
#define SEC_ENABLE_KEY_LOCATION_RAM
#define SEC_SIZE_CHECKSUM_VENDOR          2u
#define SEC_SECURITY_CLASS_VERIFY         SEC_CLASS_CCC
#define SEC_SECURITY_CLASS_VERIFICATION   SEC_CLASS_DDD

#define FBL_ENABLE_KEY_EXPORT
#define FBL_ENABLE_SECMOD_VECTOR
#define FBLCW_ENABLE_RECEPTION_IN_STATE_TASK
#define FBL_CW_ENABLE_RECEPTION_IN_STATE_TASK

#if defined( FBL_ENABLE_APPL_TASK )
# if !defined( FBL_DISABLE_APPL_STATE_TASK )
#  define FBL_ENABLE_APPL_STATE_TASK
# endif
# if !defined( FBL_DISABLE_APPL_TIMER_TASK )
#  define FBL_ENABLE_APPL_TIMER_TASK
# endif
#endif

#if defined( VGEN_ENABLE_XCP )
# define FBL_ENABLE_XCP
#endif
/* *************************************************************************** */


/* begin Fileversion check */
#ifndef SKIP_MAGIC_NUMBER
#ifdef MAGIC_NUMBER
  #if MAGIC_NUMBER != 260732447
      #error "The magic number of the generated file <D:\usr\usage\Delivery\CBD15x\CBD1500635\Dxx\external\Demo\DemoFbl\Appl\GenData\fbl_cfg.h> is different. Please check time and date of generated files!"
  #endif
#else
  #define MAGIC_NUMBER 260732447
#endif  /* MAGIC_NUMBER */
#endif  /* SKIP_MAGIC_NUMBER */

/* end Fileversion check */

#endif /* __FBL_CFG_H__ */
