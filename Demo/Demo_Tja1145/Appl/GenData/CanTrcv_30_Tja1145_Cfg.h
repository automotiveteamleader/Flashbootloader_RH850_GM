/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *
 *                 This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                 Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                 All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  LICENSE
 *  -------------------------------------------------------------------------------------------------------------------
 *            Module: CanTrcv
 *           Program: GM Global B (MSR_Gm_SLP1)
 *          Customer: Nexteer Automotive Corporation
 *       Expiry Date: Not restricted
 *  Ordered Derivat.: Renesas RH850 P1M R7F701363
 *    License Scope : The usage is restricted to CBD1500636_D02
 *
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *              File: CanTrcv_30_Tja1145_Cfg.h
 *   Generation Time: 2017-07-11 15:18:02
 *           Project: Tja1145 - Version 1.0
 *          Delivery: CBD1500636_D02
 *      Tool Version: DaVinci Configurator (beta) 5.14.11 SP1
 *
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 ! BETA VERSION                                                                                                       !
 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 ! These programs are fully operative programs. With regard to the fact that the programs are a beta-version only,    !
 ! Vector Informatik's liability shall be expressly excluded in cases of ordinary negligence, to the extent           !
 ! admissible by law or statute.                                                                                      !
 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
**********************************************************************************************************************/


#if !defined(CANTRCV_30_TJA1145_CFG_H)
#define CANTRCV_30_TJA1145_CFG_H

/**********************************************************************************************************************
  INCLUDES
**********************************************************************************************************************/
/* SREQ00010148 */
# include "ComStack_Types.h" 
# include "Can_GeneralTypes.h"


#include "Spi.h"
#include "CanIf.h"



/**********************************************************************************************************************
  VERSION DEFINES (Adapted with: ESCAN00086365)
**********************************************************************************************************************/
#define DRVTRANS_CAN_30_TJA1145_GENTOOL_CFG5_MAJOR_VERSION 0x03U
#define DRVTRANS_CAN_30_TJA1145_GENTOOL_CFG5_MINOR_VERSION 0x02U
#define DRVTRANS_CAN_30_TJA1145_GENTOOL_CFG5_PATCH_VERSION 0x01U

#define CANTRCV_30_TJA1145_GENTOOL_CFG5_BASE_COMP_VERSION 0x0104U
#define CANTRCV_30_TJA1145_GENTOOL_CFG5_HW_COMP_VERSION   0x0104U


/**********************************************************************************************************************
  SWITCHES (BASE)
**********************************************************************************************************************/
#define CANTRCV_30_TJA1145_MAX_CHANNEL             1U
#define CANTRCV_30_TJA1145_GENERAL_WAKE_UP_SUPPORT CANTRCV_30_TJA1145_WAKEUP_BY_POLLING
#define CANTRCV_30_TJA1145_WAKEUP_BY_BUS_USED      STD_ON
#define CANTRCV_30_TJA1145_GET_VERSION_INFO        STD_OFF
#define CANTRCV_30_TJA1145_DEV_ERROR_DETECT        STD_OFF
#define CANTRCV_30_TJA1145_DEV_ERROR_REPORT        STD_OFF
#define CANTRCV_30_TJA1145_ECUC_SAFE_BSW_CHECKS    STD_OFF
#define CANTRCV_30_TJA1145_PROD_ERROR_DETECT       STD_OFF 
#define CANTRCV_30_TJA1145_INSTANCE_ID             0
#define CANTRCV_30_TJA1145_HW_PN_SUPPORT           STD_ON
#define CANTRCV_30_TJA1145_BUS_ERR_FLAG            STD_OFF
#define CANTRCV_30_TJA1145_VERIFY_DATA             STD_ON  
#define CANTRCV_30_TJA1145_SPI_ACCESS_SYNCHRONOUS  STD_ON
#define CANTRCV_30_TJA1145_USE_ICU                 STD_OFF
#define CANTRCV_30_TJA1145_USE_EXTENDED_WU_SRC     STD_ON

#define CanTrcv_30_Tja1145_CanTrcvChannel 0U 


#ifndef CANTRCV_30_TJA1145_USE_DUMMY_FUNCTIONS
#define CANTRCV_30_TJA1145_USE_DUMMY_FUNCTIONS STD_OFF /* /MICROSAR/EcuC/EcucGeneral/DummyFunction */
#endif
#ifndef CANTRCV_30_TJA1145_USE_DUMMY_STATEMENT
#define CANTRCV_30_TJA1145_USE_DUMMY_STATEMENT STD_ON /* /MICROSAR/EcuC/EcucGeneral/DummyStatement */
#endif
#ifndef CANTRCV_30_TJA1145_DUMMY_STATEMENT
#define CANTRCV_30_TJA1145_DUMMY_STATEMENT(v) (v)=(v) /* PRQA S 3453 */ /* MD_MSR_19.7 */  /* /MICROSAR/EcuC/EcucGeneral/DummyStatementKind */
#endif
#ifndef CANTRCV_30_TJA1145_DUMMY_STATEMENT_CONST
#define CANTRCV_30_TJA1145_DUMMY_STATEMENT_CONST(v) (void)(v) /* PRQA S 3453 */ /* MD_MSR_19.7 */  /* /MICROSAR/EcuC/EcucGeneral/DummyStatementKind */
#endif
#ifndef CANTRCV_30_TJA1145_ATOMIC_BIT_ACCESS_IN_BITFIELD
#define CANTRCV_30_TJA1145_ATOMIC_BIT_ACCESS_IN_BITFIELD STD_OFF /* /MICROSAR/EcuC/EcucGeneral/AtomicBitAccessInBitfield */
#endif
#ifndef CANTRCV_30_TJA1145_ATOMIC_VARIABLE_ACCESS
#define CANTRCV_30_TJA1145_ATOMIC_VARIABLE_ACCESS 32U /* /MICROSAR/EcuC/EcucGeneral/AtomicVariableAccess */
#endif
#ifndef CANTRCV_30_TJA1145_PROCESSOR_RH850_1319
#define CANTRCV_30_TJA1145_PROCESSOR_RH850_1319
#endif
#ifndef CANTRCV_30_TJA1145_COMP_GREENHILLS
#define CANTRCV_30_TJA1145_COMP_GREENHILLS
#endif
#ifndef CANTRCV_30_TJA1145_GEN_GENERATOR_MSR
#define CANTRCV_30_TJA1145_GEN_GENERATOR_MSR
#endif
#ifndef CANTRCV_30_TJA1145_CPUTYPE_BITORDER_LSB2MSB
#define CANTRCV_30_TJA1145_CPUTYPE_BITORDER_LSB2MSB /* /MICROSAR/EcuC/EcucGeneral/BitOrder */
#endif
#ifndef CANTRCV_30_TJA1145_CONFIGURATION_VARIANT_PRECOMPILE
#define CANTRCV_30_TJA1145_CONFIGURATION_VARIANT_PRECOMPILE 1
#endif
#ifndef CANTRCV_30_TJA1145_CONFIGURATION_VARIANT_LINKTIME
#define CANTRCV_30_TJA1145_CONFIGURATION_VARIANT_LINKTIME 2
#endif
#ifndef CANTRCV_30_TJA1145_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE
#define CANTRCV_30_TJA1145_CONFIGURATION_VARIANT_POSTBUILD_LOADABLE 3
#endif
#ifndef CANTRCV_30_TJA1145_CONFIGURATION_VARIANT
#define CANTRCV_30_TJA1145_CONFIGURATION_VARIANT CANTRCV_30_TJA1145_CONFIGURATION_VARIANT_PRECOMPILE
#endif
#ifndef CANTRCV_30_TJA1145_POSTBUILD_VARIANT_SUPPORT
#define CANTRCV_30_TJA1145_POSTBUILD_VARIANT_SUPPORT STD_OFF
#endif


/**********************************************************************************************************************
  SWITCHES (HW SPECIFIC)
**********************************************************************************************************************/

# define CANTRCV_30_TJA1145_FD_TOLERANCE           STD_OFF



/**********************************************************************************************************************
  GLOBAL CONSTANT MACROS
**********************************************************************************************************************/
/**********************************************************************************************************************
  CONFIGURATION CLASS: PRE_COMPILE
  SECTION: GLOBAL CONSTANT MACROS
**********************************************************************************************************************/
/** 
  \defgroup  CanTrcv_30_Tja1145PCDataSwitches  CanTrcv_30_Tja1145 Data Switches  (PRE_COMPILE)
  \brief  These defines are used to deactivate data and their processing.
  \{
*/ 
#define CANTRCV_30_TJA1145_CHANNEL                                    STD_OFF  /**< Deactivateable: 'CanTrcv_30_Tja1145_Channel' Reason: 'the struct is deactivated because all elements are deactivated.' */
#define CANTRCV_30_TJA1145_ICUCHANNELOFCHANNEL                        STD_OFF  /**< Deactivateable: 'CanTrcv_30_Tja1145_Channel.IcuChannel' Reason: 'No "ICU" is configured.' */
#define CANTRCV_30_TJA1145_ICUCHANNELSETOFCHANNEL                     STD_OFF  /**< Deactivateable: 'CanTrcv_30_Tja1145_Channel.IcuChannelSet' Reason: 'No "ICU" is configured.' */
#define CANTRCV_30_TJA1145_CHANNELUSED                                STD_ON
#define CANTRCV_30_TJA1145_FDTOLERANCEENABLED                         STD_OFF  /**< Deactivateable: 'CanTrcv_30_Tja1145_FdToleranceEnabled' Reason: '"Fd Tolerance" is deactivated.' */
#define CANTRCV_30_TJA1145_FINALMAGICNUMBER                           STD_OFF  /**< Deactivateable: 'CanTrcv_30_Tja1145_FinalMagicNumber' Reason: 'the module configuration does not support flashing of data.' */
#define CANTRCV_30_TJA1145_GENERATORCOMPATIBILITYVERSION              STD_ON
#define CANTRCV_30_TJA1145_INITDATAHASHCODE                           STD_OFF  /**< Deactivateable: 'CanTrcv_30_Tja1145_InitDataHashCode' Reason: 'the module configuration does not support flashing of data.' */
#define CANTRCV_30_TJA1145_PNCFG                                      STD_ON
#define CANTRCV_30_TJA1145_BAUDRATEOFPNCFG                            STD_ON
#define CANTRCV_30_TJA1145_CANID0OFPNCFG                              STD_ON
#define CANTRCV_30_TJA1145_CANID1OFPNCFG                              STD_ON
#define CANTRCV_30_TJA1145_CANID2OFPNCFG                              STD_ON
#define CANTRCV_30_TJA1145_CANID3OFPNCFG                              STD_ON
#define CANTRCV_30_TJA1145_CANIDMASK0OFPNCFG                          STD_ON
#define CANTRCV_30_TJA1145_CANIDMASK1OFPNCFG                          STD_ON
#define CANTRCV_30_TJA1145_CANIDMASK2OFPNCFG                          STD_ON
#define CANTRCV_30_TJA1145_CANIDMASK3OFPNCFG                          STD_ON
#define CANTRCV_30_TJA1145_FRAMECONTROLOFPNCFG                        STD_ON
#define CANTRCV_30_TJA1145_PNENABLED                                  STD_ON
#define CANTRCV_30_TJA1145_PNPAYLOADCFG                               STD_ON
#define CANTRCV_30_TJA1145_DATAMASK0OFPNPAYLOADCFG                    STD_ON
#define CANTRCV_30_TJA1145_DATAMASK1OFPNPAYLOADCFG                    STD_ON
#define CANTRCV_30_TJA1145_DATAMASK2OFPNPAYLOADCFG                    STD_ON
#define CANTRCV_30_TJA1145_DATAMASK3OFPNPAYLOADCFG                    STD_ON
#define CANTRCV_30_TJA1145_DATAMASK4OFPNPAYLOADCFG                    STD_ON
#define CANTRCV_30_TJA1145_DATAMASK5OFPNPAYLOADCFG                    STD_ON
#define CANTRCV_30_TJA1145_DATAMASK6OFPNPAYLOADCFG                    STD_ON
#define CANTRCV_30_TJA1145_DATAMASK7OFPNPAYLOADCFG                    STD_ON
#define CANTRCV_30_TJA1145_SIZEOFCHANNELUSED                          STD_ON
#define CANTRCV_30_TJA1145_SIZEOFPNCFG                                STD_ON
#define CANTRCV_30_TJA1145_SIZEOFPNENABLED                            STD_ON
#define CANTRCV_30_TJA1145_SIZEOFPNPAYLOADCFG                         STD_ON
#define CANTRCV_30_TJA1145_SIZEOFSPICHANNELCFG                        STD_ON
#define CANTRCV_30_TJA1145_SIZEOFSPISEQUENCECFG                       STD_ON
#define CANTRCV_30_TJA1145_SIZEOFWAKEUPBYBUSUSED                      STD_ON
#define CANTRCV_30_TJA1145_SIZEOFWAKEUPSOURCE                         STD_ON
#define CANTRCV_30_TJA1145_SIZEOFWUSRCPOR                             STD_ON
#define CANTRCV_30_TJA1145_SIZEOFWUSRCSYSERR                          STD_ON
#define CANTRCV_30_TJA1145_SPICHANNELCFG                              STD_ON
#define CANTRCV_30_TJA1145_CANTRCVSPICHRWDATA_0OFSPICHANNELCFG        STD_ON
#define CANTRCV_30_TJA1145_CANTRCVSPICHRWDATA_1OFSPICHANNELCFG        STD_ON
#define CANTRCV_30_TJA1145_CANTRCVSPICHRWDATA_2OFSPICHANNELCFG        STD_ON
#define CANTRCV_30_TJA1145_CANTRCVSPICHRWDATA_3OFSPICHANNELCFG        STD_ON
#define CANTRCV_30_TJA1145_SPISEQUENCECFG                             STD_ON
#define CANTRCV_30_TJA1145_CANTRCVSEQRWDATALARGEOFSPISEQUENCECFG      STD_ON
#define CANTRCV_30_TJA1145_CANTRCVSEQRWDATAMEDIUMOFSPISEQUENCECFG     STD_ON
#define CANTRCV_30_TJA1145_CANTRCVSEQRWDATANORMALOFSPISEQUENCECFG     STD_ON
#define CANTRCV_30_TJA1145_WAKEUPBYBUSUSED                            STD_ON
#define CANTRCV_30_TJA1145_WAKEUPSOURCE                               STD_ON
#define CANTRCV_30_TJA1145_WUSRCPOR                                   STD_ON
#define CANTRCV_30_TJA1145_WUSRCSYSERR                                STD_ON
#define CANTRCV_30_TJA1145_PCCONFIG                                   STD_ON
#define CANTRCV_30_TJA1145_CHANNELUSEDOFPCCONFIG                      STD_ON
#define CANTRCV_30_TJA1145_FINALMAGICNUMBEROFPCCONFIG                 STD_OFF  /**< Deactivateable: 'CanTrcv_30_Tja1145_PCConfig.FinalMagicNumber' Reason: 'the module configuration does not support flashing of data.' */
#define CANTRCV_30_TJA1145_GENERATORCOMPATIBILITYVERSIONOFPCCONFIG    STD_ON
#define CANTRCV_30_TJA1145_INITDATAHASHCODEOFPCCONFIG                 STD_OFF  /**< Deactivateable: 'CanTrcv_30_Tja1145_PCConfig.InitDataHashCode' Reason: 'the module configuration does not support flashing of data.' */
#define CANTRCV_30_TJA1145_PNCFGOFPCCONFIG                            STD_ON
#define CANTRCV_30_TJA1145_PNENABLEDOFPCCONFIG                        STD_ON
#define CANTRCV_30_TJA1145_PNPAYLOADCFGOFPCCONFIG                     STD_ON
#define CANTRCV_30_TJA1145_SIZEOFCHANNELUSEDOFPCCONFIG                STD_ON
#define CANTRCV_30_TJA1145_SIZEOFPNCFGOFPCCONFIG                      STD_ON
#define CANTRCV_30_TJA1145_SIZEOFPNENABLEDOFPCCONFIG                  STD_ON
#define CANTRCV_30_TJA1145_SIZEOFPNPAYLOADCFGOFPCCONFIG               STD_ON
#define CANTRCV_30_TJA1145_SIZEOFSPICHANNELCFGOFPCCONFIG              STD_ON
#define CANTRCV_30_TJA1145_SIZEOFSPISEQUENCECFGOFPCCONFIG             STD_ON
#define CANTRCV_30_TJA1145_SIZEOFWAKEUPBYBUSUSEDOFPCCONFIG            STD_ON
#define CANTRCV_30_TJA1145_SIZEOFWAKEUPSOURCEOFPCCONFIG               STD_ON
#define CANTRCV_30_TJA1145_SIZEOFWUSRCPOROFPCCONFIG                   STD_ON
#define CANTRCV_30_TJA1145_SIZEOFWUSRCSYSERROFPCCONFIG                STD_ON
#define CANTRCV_30_TJA1145_SPICHANNELCFGOFPCCONFIG                    STD_ON
#define CANTRCV_30_TJA1145_SPISEQUENCECFGOFPCCONFIG                   STD_ON
#define CANTRCV_30_TJA1145_WAKEUPBYBUSUSEDOFPCCONFIG                  STD_ON
#define CANTRCV_30_TJA1145_WAKEUPSOURCEOFPCCONFIG                     STD_ON
#define CANTRCV_30_TJA1145_WUSRCPOROFPCCONFIG                         STD_ON
#define CANTRCV_30_TJA1145_WUSRCSYSERROFPCCONFIG                      STD_ON
/** 
  \}
*/ 

/** 
  \defgroup  CanTrcv_30_Tja1145PCIsReducedToDefineDefines  CanTrcv_30_Tja1145 Is Reduced To Define Defines (PRE_COMPILE)
  \brief  If all values in a CONST array or an element in a CONST array of structs are equal, the define is STD_ON else STD_OFF.
  \{
*/ 
#define CANTRCV_30_TJA1145_ISDEF_CHANNELUSED                          STD_OFF
#define CANTRCV_30_TJA1145_ISDEF_BAUDRATEOFPNCFG                      STD_OFF
#define CANTRCV_30_TJA1145_ISDEF_CANID0OFPNCFG                        STD_OFF
#define CANTRCV_30_TJA1145_ISDEF_CANID1OFPNCFG                        STD_OFF
#define CANTRCV_30_TJA1145_ISDEF_CANID2OFPNCFG                        STD_OFF
#define CANTRCV_30_TJA1145_ISDEF_CANID3OFPNCFG                        STD_OFF
#define CANTRCV_30_TJA1145_ISDEF_CANIDMASK0OFPNCFG                    STD_OFF
#define CANTRCV_30_TJA1145_ISDEF_CANIDMASK1OFPNCFG                    STD_OFF
#define CANTRCV_30_TJA1145_ISDEF_CANIDMASK2OFPNCFG                    STD_OFF
#define CANTRCV_30_TJA1145_ISDEF_CANIDMASK3OFPNCFG                    STD_OFF
#define CANTRCV_30_TJA1145_ISDEF_FRAMECONTROLOFPNCFG                  STD_OFF
#define CANTRCV_30_TJA1145_ISDEF_PNENABLED                            STD_OFF
#define CANTRCV_30_TJA1145_ISDEF_DATAMASK0OFPNPAYLOADCFG              STD_OFF
#define CANTRCV_30_TJA1145_ISDEF_DATAMASK1OFPNPAYLOADCFG              STD_OFF
#define CANTRCV_30_TJA1145_ISDEF_DATAMASK2OFPNPAYLOADCFG              STD_OFF
#define CANTRCV_30_TJA1145_ISDEF_DATAMASK3OFPNPAYLOADCFG              STD_OFF
#define CANTRCV_30_TJA1145_ISDEF_DATAMASK4OFPNPAYLOADCFG              STD_OFF
#define CANTRCV_30_TJA1145_ISDEF_DATAMASK5OFPNPAYLOADCFG              STD_OFF
#define CANTRCV_30_TJA1145_ISDEF_DATAMASK6OFPNPAYLOADCFG              STD_OFF
#define CANTRCV_30_TJA1145_ISDEF_DATAMASK7OFPNPAYLOADCFG              STD_OFF
#define CANTRCV_30_TJA1145_ISDEF_CANTRCVSPICHRWDATA_0OFSPICHANNELCFG  STD_OFF
#define CANTRCV_30_TJA1145_ISDEF_CANTRCVSPICHRWDATA_1OFSPICHANNELCFG  STD_OFF
#define CANTRCV_30_TJA1145_ISDEF_CANTRCVSPICHRWDATA_2OFSPICHANNELCFG  STD_OFF
#define CANTRCV_30_TJA1145_ISDEF_CANTRCVSPICHRWDATA_3OFSPICHANNELCFG  STD_OFF
#define CANTRCV_30_TJA1145_ISDEF_CANTRCVSEQRWDATALARGEOFSPISEQUENCECFG STD_OFF
#define CANTRCV_30_TJA1145_ISDEF_CANTRCVSEQRWDATAMEDIUMOFSPISEQUENCECFG STD_OFF
#define CANTRCV_30_TJA1145_ISDEF_CANTRCVSEQRWDATANORMALOFSPISEQUENCECFG STD_OFF
#define CANTRCV_30_TJA1145_ISDEF_WAKEUPBYBUSUSED                      STD_OFF
#define CANTRCV_30_TJA1145_ISDEF_WAKEUPSOURCE                         STD_OFF
#define CANTRCV_30_TJA1145_ISDEF_WUSRCPOR                             STD_OFF
#define CANTRCV_30_TJA1145_ISDEF_WUSRCSYSERR                          STD_OFF
#define CANTRCV_30_TJA1145_ISDEF_CHANNELUSEDOFPCCONFIG                STD_ON
#define CANTRCV_30_TJA1145_ISDEF_PNCFGOFPCCONFIG                      STD_ON
#define CANTRCV_30_TJA1145_ISDEF_PNENABLEDOFPCCONFIG                  STD_ON
#define CANTRCV_30_TJA1145_ISDEF_PNPAYLOADCFGOFPCCONFIG               STD_ON
#define CANTRCV_30_TJA1145_ISDEF_SPICHANNELCFGOFPCCONFIG              STD_ON
#define CANTRCV_30_TJA1145_ISDEF_SPISEQUENCECFGOFPCCONFIG             STD_ON
#define CANTRCV_30_TJA1145_ISDEF_WAKEUPBYBUSUSEDOFPCCONFIG            STD_ON
#define CANTRCV_30_TJA1145_ISDEF_WAKEUPSOURCEOFPCCONFIG               STD_ON
#define CANTRCV_30_TJA1145_ISDEF_WUSRCPOROFPCCONFIG                   STD_ON
#define CANTRCV_30_TJA1145_ISDEF_WUSRCSYSERROFPCCONFIG                STD_ON
/** 
  \}
*/ 

/** 
  \defgroup  CanTrcv_30_Tja1145PCEqualsAlwaysToDefines  CanTrcv_30_Tja1145 Equals Always To Defines (PRE_COMPILE)
  \brief  If all values in a CONST array or an element in a CONST array of structs are equal, the define contains the always equals value.
  \{
*/ 
#define CANTRCV_30_TJA1145_EQ2_CHANNELUSED                            
#define CANTRCV_30_TJA1145_EQ2_BAUDRATEOFPNCFG                        
#define CANTRCV_30_TJA1145_EQ2_CANID0OFPNCFG                          
#define CANTRCV_30_TJA1145_EQ2_CANID1OFPNCFG                          
#define CANTRCV_30_TJA1145_EQ2_CANID2OFPNCFG                          
#define CANTRCV_30_TJA1145_EQ2_CANID3OFPNCFG                          
#define CANTRCV_30_TJA1145_EQ2_CANIDMASK0OFPNCFG                      
#define CANTRCV_30_TJA1145_EQ2_CANIDMASK1OFPNCFG                      
#define CANTRCV_30_TJA1145_EQ2_CANIDMASK2OFPNCFG                      
#define CANTRCV_30_TJA1145_EQ2_CANIDMASK3OFPNCFG                      
#define CANTRCV_30_TJA1145_EQ2_FRAMECONTROLOFPNCFG                    
#define CANTRCV_30_TJA1145_EQ2_PNENABLED                              
#define CANTRCV_30_TJA1145_EQ2_DATAMASK0OFPNPAYLOADCFG                
#define CANTRCV_30_TJA1145_EQ2_DATAMASK1OFPNPAYLOADCFG                
#define CANTRCV_30_TJA1145_EQ2_DATAMASK2OFPNPAYLOADCFG                
#define CANTRCV_30_TJA1145_EQ2_DATAMASK3OFPNPAYLOADCFG                
#define CANTRCV_30_TJA1145_EQ2_DATAMASK4OFPNPAYLOADCFG                
#define CANTRCV_30_TJA1145_EQ2_DATAMASK5OFPNPAYLOADCFG                
#define CANTRCV_30_TJA1145_EQ2_DATAMASK6OFPNPAYLOADCFG                
#define CANTRCV_30_TJA1145_EQ2_DATAMASK7OFPNPAYLOADCFG                
#define CANTRCV_30_TJA1145_EQ2_CANTRCVSPICHRWDATA_0OFSPICHANNELCFG    
#define CANTRCV_30_TJA1145_EQ2_CANTRCVSPICHRWDATA_1OFSPICHANNELCFG    
#define CANTRCV_30_TJA1145_EQ2_CANTRCVSPICHRWDATA_2OFSPICHANNELCFG    
#define CANTRCV_30_TJA1145_EQ2_CANTRCVSPICHRWDATA_3OFSPICHANNELCFG    
#define CANTRCV_30_TJA1145_EQ2_CANTRCVSEQRWDATALARGEOFSPISEQUENCECFG  
#define CANTRCV_30_TJA1145_EQ2_CANTRCVSEQRWDATAMEDIUMOFSPISEQUENCECFG 
#define CANTRCV_30_TJA1145_EQ2_CANTRCVSEQRWDATANORMALOFSPISEQUENCECFG 
#define CANTRCV_30_TJA1145_EQ2_WAKEUPBYBUSUSED                        
#define CANTRCV_30_TJA1145_EQ2_WAKEUPSOURCE                           
#define CANTRCV_30_TJA1145_EQ2_WUSRCPOR                               
#define CANTRCV_30_TJA1145_EQ2_WUSRCSYSERR                            
#define CANTRCV_30_TJA1145_EQ2_CHANNELUSEDOFPCCONFIG                  CanTrcv_30_Tja1145_ChannelUsed
#define CANTRCV_30_TJA1145_EQ2_PNCFGOFPCCONFIG                        CanTrcv_30_Tja1145_PnCfg
#define CANTRCV_30_TJA1145_EQ2_PNENABLEDOFPCCONFIG                    CanTrcv_30_Tja1145_PnEnabled
#define CANTRCV_30_TJA1145_EQ2_PNPAYLOADCFGOFPCCONFIG                 CanTrcv_30_Tja1145_PnPayloadCfg
#define CANTRCV_30_TJA1145_EQ2_SPICHANNELCFGOFPCCONFIG                CanTrcv_30_Tja1145_SpiChannelCfg
#define CANTRCV_30_TJA1145_EQ2_SPISEQUENCECFGOFPCCONFIG               CanTrcv_30_Tja1145_SpiSequenceCfg
#define CANTRCV_30_TJA1145_EQ2_WAKEUPBYBUSUSEDOFPCCONFIG              CanTrcv_30_Tja1145_WakeupByBusUsed
#define CANTRCV_30_TJA1145_EQ2_WAKEUPSOURCEOFPCCONFIG                 CanTrcv_30_Tja1145_WakeupSource
#define CANTRCV_30_TJA1145_EQ2_WUSRCPOROFPCCONFIG                     CanTrcv_30_Tja1145_WuSrcPor
#define CANTRCV_30_TJA1145_EQ2_WUSRCSYSERROFPCCONFIG                  CanTrcv_30_Tja1145_WuSrcSyserr
/** 
  \}
*/ 

/** 
  \defgroup  CanTrcv_30_Tja1145PCSymbolicInitializationPointers  CanTrcv_30_Tja1145 Symbolic Initialization Pointers (PRE_COMPILE)
  \brief  Symbolic initialization pointers to be used in the call of a preinit or init function.
  \{
*/ 
#define CanTrcv_30_Tja1145_Config_Ptr                                 NULL_PTR  /**< symbolic identifier which shall be used to initialize 'CanTrcv_30_Tja1145' */
/** 
  \}
*/ 

/** 
  \defgroup  CanTrcv_30_Tja1145PCInitializationSymbols  CanTrcv_30_Tja1145 Initialization Symbols (PRE_COMPILE)
  \brief  Symbolic initialization pointers which may be used in the call of a preinit or init function. Please note, that the defined value can be a 'NULL_PTR' and the address operator is not usable.
  \{
*/ 
#define CanTrcv_30_Tja1145_Config                                     NULL_PTR  /**< symbolic identifier which could be used to initialize 'CanTrcv_30_Tja1145 */
/** 
  \}
*/ 

/** 
  \defgroup  CanTrcv_30_Tja1145PCGeneral  CanTrcv_30_Tja1145 General (PRE_COMPILE)
  \brief  General constant defines not associated with a group of defines.
  \{
*/ 
#define CANTRCV_30_TJA1145_CHECK_INIT_POINTER                         STD_OFF  /**< STD_ON if the init pointer shall not be used as NULL_PTR and a check shall validate this. */
#define CANTRCV_30_TJA1145_FINAL_MAGIC_NUMBER                         0x461EU  /**< the precompile constant to validate the size of the initialization structure at initialization time of CanTrcv_30_Tja1145 */
#define CANTRCV_30_TJA1145_INDIVIDUAL_POSTBUILD                       STD_OFF  /**< the precompile constant to check, that the module is individual postbuildable. The module 'CanTrcv_30_Tja1145' is not configured to be postbuild capable. */
#define CANTRCV_30_TJA1145_INIT_DATA                                  CANTRCV_30_TJA1145_CONST  /**< CompilerMemClassDefine for the initialization data. */
#define CANTRCV_30_TJA1145_INIT_DATA_HASH_CODE                        -633720322L  /**< the precompile constant to validate the initialization structure at initialization time of CanTrcv_30_Tja1145 with a hashcode. The seed value is '0x461EU' */
#define CANTRCV_30_TJA1145_USE_ECUM_BSW_ERROR_HOOK                    STD_OFF  /**< STD_ON if the EcuM_BswErrorHook shall be called in the ConfigPtr check. */
#define CANTRCV_30_TJA1145_USE_INIT_POINTER                           STD_OFF  /**< STD_ON if the init pointer CanTrcv_30_Tja1145 shall be used. */
/** 
  \}
*/ 


/**********************************************************************************************************************
  CONFIGURATION CLASS: LINK
  SECTION: GLOBAL CONSTANT MACROS
**********************************************************************************************************************/
/** 
  \defgroup  CanTrcv_30_Tja1145LTDataSwitches  CanTrcv_30_Tja1145 Data Switches  (LINK)
  \brief  These defines are used to deactivate data and their processing.
  \{
*/ 
#define CANTRCV_30_TJA1145_LTCONFIG                                   STD_OFF  /**< Deactivateable: 'CanTrcv_30_Tja1145_LTConfig' Reason: 'the struct is deactivated because all elements are deactivated in all variants.' */
/** 
  \}
*/ 


/**********************************************************************************************************************
  CONFIGURATION CLASS: POST_BUILD
  SECTION: GLOBAL CONSTANT MACROS
**********************************************************************************************************************/
/** 
  \defgroup  CanTrcv_30_Tja1145PBDataSwitches  CanTrcv_30_Tja1145 Data Switches  (POST_BUILD)
  \brief  These defines are used to deactivate data and their processing.
  \{
*/ 
#define CANTRCV_30_TJA1145_PBCONFIG                                   STD_OFF  /**< Deactivateable: 'CanTrcv_30_Tja1145_PBConfig' Reason: 'the module configuration is VARIANT_PRE_COMPILE.' */
#define CANTRCV_30_TJA1145_LTCONFIGIDXOFPBCONFIG                      STD_OFF  /**< Deactivateable: 'CanTrcv_30_Tja1145_PBConfig.LTConfigIdx' Reason: 'the module configuration is VARIANT_PRE_COMPILE.' */
#define CANTRCV_30_TJA1145_PCCONFIGIDXOFPBCONFIG                      STD_OFF  /**< Deactivateable: 'CanTrcv_30_Tja1145_PBConfig.PCConfigIdx' Reason: 'the module configuration is VARIANT_PRE_COMPILE.' */
/** 
  \}
*/ 



/**********************************************************************************************************************
  GLOBAL FUNCTION MACROS
**********************************************************************************************************************/
/** 
  \defgroup  DataAccessMacros  Data Access Macros
  \brief  generated data access macros to abstract the generated data from the code to read and write CONST or VAR data.
  \{
*/ 
  /* PRQA S 3453 MACROS_3453 */  /* MD_CSL_3453 */
/**********************************************************************************************************************
  CONFIGURATION CLASS: PRE_COMPILE
  SECTION: GLOBAL FUNCTION MACROS
**********************************************************************************************************************/
/** 
  \defgroup  CanTrcv_30_Tja1145PCGetConstantDuplicatedRootDataMacros  CanTrcv_30_Tja1145 Get Constant Duplicated Root Data Macros (PRE_COMPILE)
  \brief  These macros can be used to read deduplicated by constance root data elements.
  \{
*/ 
#define CanTrcv_30_Tja1145_GetChannelUsedOfPCConfig()                 CanTrcv_30_Tja1145_ChannelUsed  /**< the pointer to CanTrcv_30_Tja1145_ChannelUsed */
#define CanTrcv_30_Tja1145_GetGeneratorCompatibilityVersionOfPCConfig() 0x01040104UL
#define CanTrcv_30_Tja1145_GetPnCfgOfPCConfig()                       CanTrcv_30_Tja1145_PnCfg  /**< the pointer to CanTrcv_30_Tja1145_PnCfg */
#define CanTrcv_30_Tja1145_GetPnEnabledOfPCConfig()                   CanTrcv_30_Tja1145_PnEnabled  /**< the pointer to CanTrcv_30_Tja1145_PnEnabled */
#define CanTrcv_30_Tja1145_GetPnPayloadCfgOfPCConfig()                CanTrcv_30_Tja1145_PnPayloadCfg  /**< the pointer to CanTrcv_30_Tja1145_PnPayloadCfg */
#define CanTrcv_30_Tja1145_GetSizeOfChannelUsedOfPCConfig()           1U  /**< the number of accomplishable value elements in CanTrcv_30_Tja1145_ChannelUsed */
#define CanTrcv_30_Tja1145_GetSizeOfPnCfgOfPCConfig()                 1U  /**< the number of accomplishable value elements in CanTrcv_30_Tja1145_PnCfg */
#define CanTrcv_30_Tja1145_GetSizeOfPnEnabledOfPCConfig()             1U  /**< the number of accomplishable value elements in CanTrcv_30_Tja1145_PnEnabled */
#define CanTrcv_30_Tja1145_GetSizeOfPnPayloadCfgOfPCConfig()          1U  /**< the number of accomplishable value elements in CanTrcv_30_Tja1145_PnPayloadCfg */
#define CanTrcv_30_Tja1145_GetSizeOfSpiChannelCfgOfPCConfig()         1U  /**< the number of accomplishable value elements in CanTrcv_30_Tja1145_SpiChannelCfg */
#define CanTrcv_30_Tja1145_GetSizeOfSpiSequenceCfgOfPCConfig()        1U  /**< the number of accomplishable value elements in CanTrcv_30_Tja1145_SpiSequenceCfg */
#define CanTrcv_30_Tja1145_GetSizeOfWakeupByBusUsedOfPCConfig()       1U  /**< the number of accomplishable value elements in CanTrcv_30_Tja1145_WakeupByBusUsed */
#define CanTrcv_30_Tja1145_GetSizeOfWakeupSourceOfPCConfig()          1U  /**< the number of accomplishable value elements in CanTrcv_30_Tja1145_WakeupSource */
#define CanTrcv_30_Tja1145_GetSizeOfWuSrcPorOfPCConfig()              1U  /**< the number of accomplishable value elements in CanTrcv_30_Tja1145_WuSrcPor */
#define CanTrcv_30_Tja1145_GetSizeOfWuSrcSyserrOfPCConfig()           1U  /**< the number of accomplishable value elements in CanTrcv_30_Tja1145_WuSrcSyserr */
#define CanTrcv_30_Tja1145_GetSpiChannelCfgOfPCConfig()               CanTrcv_30_Tja1145_SpiChannelCfg  /**< the pointer to CanTrcv_30_Tja1145_SpiChannelCfg */
#define CanTrcv_30_Tja1145_GetSpiSequenceCfgOfPCConfig()              CanTrcv_30_Tja1145_SpiSequenceCfg  /**< the pointer to CanTrcv_30_Tja1145_SpiSequenceCfg */
#define CanTrcv_30_Tja1145_GetWakeupByBusUsedOfPCConfig()             CanTrcv_30_Tja1145_WakeupByBusUsed  /**< the pointer to CanTrcv_30_Tja1145_WakeupByBusUsed */
#define CanTrcv_30_Tja1145_GetWakeupSourceOfPCConfig()                CanTrcv_30_Tja1145_WakeupSource  /**< the pointer to CanTrcv_30_Tja1145_WakeupSource */
#define CanTrcv_30_Tja1145_GetWuSrcPorOfPCConfig()                    CanTrcv_30_Tja1145_WuSrcPor  /**< the pointer to CanTrcv_30_Tja1145_WuSrcPor */
#define CanTrcv_30_Tja1145_GetWuSrcSyserrOfPCConfig()                 CanTrcv_30_Tja1145_WuSrcSyserr  /**< the pointer to CanTrcv_30_Tja1145_WuSrcSyserr */
/** 
  \}
*/ 

/** 
  \defgroup  CanTrcv_30_Tja1145PCGetDataMacros  CanTrcv_30_Tja1145 Get Data Macros (PRE_COMPILE)
  \brief  These macros can be used to read CONST and VAR data.
  \{
*/ 
#define CanTrcv_30_Tja1145_IsChannelUsed(Index)                       ((CanTrcv_30_Tja1145_GetChannelUsedOfPCConfig()[(Index)]) != FALSE)
#define CanTrcv_30_Tja1145_GetBaudrateOfPnCfg(Index)                  (CanTrcv_30_Tja1145_GetPnCfgOfPCConfig()[(Index)].BaudrateOfPnCfg)
#define CanTrcv_30_Tja1145_GetCanId0OfPnCfg(Index)                    (CanTrcv_30_Tja1145_GetPnCfgOfPCConfig()[(Index)].CanId0OfPnCfg)
#define CanTrcv_30_Tja1145_GetCanId1OfPnCfg(Index)                    (CanTrcv_30_Tja1145_GetPnCfgOfPCConfig()[(Index)].CanId1OfPnCfg)
#define CanTrcv_30_Tja1145_GetCanId2OfPnCfg(Index)                    (CanTrcv_30_Tja1145_GetPnCfgOfPCConfig()[(Index)].CanId2OfPnCfg)
#define CanTrcv_30_Tja1145_GetCanId3OfPnCfg(Index)                    (CanTrcv_30_Tja1145_GetPnCfgOfPCConfig()[(Index)].CanId3OfPnCfg)
#define CanTrcv_30_Tja1145_GetCanIdMask0OfPnCfg(Index)                (CanTrcv_30_Tja1145_GetPnCfgOfPCConfig()[(Index)].CanIdMask0OfPnCfg)
#define CanTrcv_30_Tja1145_GetCanIdMask1OfPnCfg(Index)                (CanTrcv_30_Tja1145_GetPnCfgOfPCConfig()[(Index)].CanIdMask1OfPnCfg)
#define CanTrcv_30_Tja1145_GetCanIdMask2OfPnCfg(Index)                (CanTrcv_30_Tja1145_GetPnCfgOfPCConfig()[(Index)].CanIdMask2OfPnCfg)
#define CanTrcv_30_Tja1145_GetCanIdMask3OfPnCfg(Index)                (CanTrcv_30_Tja1145_GetPnCfgOfPCConfig()[(Index)].CanIdMask3OfPnCfg)
#define CanTrcv_30_Tja1145_GetFrameControlOfPnCfg(Index)              (CanTrcv_30_Tja1145_GetPnCfgOfPCConfig()[(Index)].FrameControlOfPnCfg)
#define CanTrcv_30_Tja1145_IsPnEnabled(Index)                         ((CanTrcv_30_Tja1145_GetPnEnabledOfPCConfig()[(Index)]) != FALSE)
#define CanTrcv_30_Tja1145_GetDataMask0OfPnPayloadCfg(Index)          (CanTrcv_30_Tja1145_GetPnPayloadCfgOfPCConfig()[(Index)].DataMask0OfPnPayloadCfg)
#define CanTrcv_30_Tja1145_GetDataMask1OfPnPayloadCfg(Index)          (CanTrcv_30_Tja1145_GetPnPayloadCfgOfPCConfig()[(Index)].DataMask1OfPnPayloadCfg)
#define CanTrcv_30_Tja1145_GetDataMask2OfPnPayloadCfg(Index)          (CanTrcv_30_Tja1145_GetPnPayloadCfgOfPCConfig()[(Index)].DataMask2OfPnPayloadCfg)
#define CanTrcv_30_Tja1145_GetDataMask3OfPnPayloadCfg(Index)          (CanTrcv_30_Tja1145_GetPnPayloadCfgOfPCConfig()[(Index)].DataMask3OfPnPayloadCfg)
#define CanTrcv_30_Tja1145_GetDataMask4OfPnPayloadCfg(Index)          (CanTrcv_30_Tja1145_GetPnPayloadCfgOfPCConfig()[(Index)].DataMask4OfPnPayloadCfg)
#define CanTrcv_30_Tja1145_GetDataMask5OfPnPayloadCfg(Index)          (CanTrcv_30_Tja1145_GetPnPayloadCfgOfPCConfig()[(Index)].DataMask5OfPnPayloadCfg)
#define CanTrcv_30_Tja1145_GetDataMask6OfPnPayloadCfg(Index)          (CanTrcv_30_Tja1145_GetPnPayloadCfgOfPCConfig()[(Index)].DataMask6OfPnPayloadCfg)
#define CanTrcv_30_Tja1145_GetDataMask7OfPnPayloadCfg(Index)          (CanTrcv_30_Tja1145_GetPnPayloadCfgOfPCConfig()[(Index)].DataMask7OfPnPayloadCfg)
#define CanTrcv_30_Tja1145_GetCanTrcvSpiChRWData_0OfSpiChannelCfg(Index) (CanTrcv_30_Tja1145_GetSpiChannelCfgOfPCConfig()[(Index)].CanTrcvSpiChRWData_0OfSpiChannelCfg)
#define CanTrcv_30_Tja1145_GetCanTrcvSpiChRWData_1OfSpiChannelCfg(Index) (CanTrcv_30_Tja1145_GetSpiChannelCfgOfPCConfig()[(Index)].CanTrcvSpiChRWData_1OfSpiChannelCfg)
#define CanTrcv_30_Tja1145_GetCanTrcvSpiChRWData_2OfSpiChannelCfg(Index) (CanTrcv_30_Tja1145_GetSpiChannelCfgOfPCConfig()[(Index)].CanTrcvSpiChRWData_2OfSpiChannelCfg)
#define CanTrcv_30_Tja1145_GetCanTrcvSpiChRWData_3OfSpiChannelCfg(Index) (CanTrcv_30_Tja1145_GetSpiChannelCfgOfPCConfig()[(Index)].CanTrcvSpiChRWData_3OfSpiChannelCfg)
#define CanTrcv_30_Tja1145_GetCanTrcvSeqRWDataLargeOfSpiSequenceCfg(Index) (CanTrcv_30_Tja1145_GetSpiSequenceCfgOfPCConfig()[(Index)].CanTrcvSeqRWDataLargeOfSpiSequenceCfg)
#define CanTrcv_30_Tja1145_GetCanTrcvSeqRWDataMediumOfSpiSequenceCfg(Index) (CanTrcv_30_Tja1145_GetSpiSequenceCfgOfPCConfig()[(Index)].CanTrcvSeqRWDataMediumOfSpiSequenceCfg)
#define CanTrcv_30_Tja1145_GetCanTrcvSeqRWDataNormalOfSpiSequenceCfg(Index) (CanTrcv_30_Tja1145_GetSpiSequenceCfgOfPCConfig()[(Index)].CanTrcvSeqRWDataNormalOfSpiSequenceCfg)
#define CanTrcv_30_Tja1145_IsWakeupByBusUsed(Index)                   ((CanTrcv_30_Tja1145_GetWakeupByBusUsedOfPCConfig()[(Index)]) != FALSE)
#define CanTrcv_30_Tja1145_GetWakeupSource(Index)                     (CanTrcv_30_Tja1145_GetWakeupSourceOfPCConfig()[(Index)])
#define CanTrcv_30_Tja1145_GetWuSrcPor(Index)                         (CanTrcv_30_Tja1145_GetWuSrcPorOfPCConfig()[(Index)])
#define CanTrcv_30_Tja1145_GetWuSrcSyserr(Index)                      (CanTrcv_30_Tja1145_GetWuSrcSyserrOfPCConfig()[(Index)])
/** 
  \}
*/ 

/** 
  \defgroup  CanTrcv_30_Tja1145PCGetDeduplicatedDataMacros  CanTrcv_30_Tja1145 Get Deduplicated Data Macros (PRE_COMPILE)
  \brief  These macros can be used to read deduplicated data elements.
  \{
*/ 
#define CanTrcv_30_Tja1145_GetGeneratorCompatibilityVersion()         CanTrcv_30_Tja1145_GetGeneratorCompatibilityVersionOfPCConfig()
#define CanTrcv_30_Tja1145_GetSizeOfChannelUsed()                     CanTrcv_30_Tja1145_GetSizeOfChannelUsedOfPCConfig()
#define CanTrcv_30_Tja1145_GetSizeOfPnCfg()                           CanTrcv_30_Tja1145_GetSizeOfPnCfgOfPCConfig()
#define CanTrcv_30_Tja1145_GetSizeOfPnEnabled()                       CanTrcv_30_Tja1145_GetSizeOfPnEnabledOfPCConfig()
#define CanTrcv_30_Tja1145_GetSizeOfPnPayloadCfg()                    CanTrcv_30_Tja1145_GetSizeOfPnPayloadCfgOfPCConfig()
#define CanTrcv_30_Tja1145_GetSizeOfSpiChannelCfg()                   CanTrcv_30_Tja1145_GetSizeOfSpiChannelCfgOfPCConfig()
#define CanTrcv_30_Tja1145_GetSizeOfSpiSequenceCfg()                  CanTrcv_30_Tja1145_GetSizeOfSpiSequenceCfgOfPCConfig()
#define CanTrcv_30_Tja1145_GetSizeOfWakeupByBusUsed()                 CanTrcv_30_Tja1145_GetSizeOfWakeupByBusUsedOfPCConfig()
#define CanTrcv_30_Tja1145_GetSizeOfWakeupSource()                    CanTrcv_30_Tja1145_GetSizeOfWakeupSourceOfPCConfig()
#define CanTrcv_30_Tja1145_GetSizeOfWuSrcPor()                        CanTrcv_30_Tja1145_GetSizeOfWuSrcPorOfPCConfig()
#define CanTrcv_30_Tja1145_GetSizeOfWuSrcSyserr()                     CanTrcv_30_Tja1145_GetSizeOfWuSrcSyserrOfPCConfig()
/** 
  \}
*/ 

/** 
  \defgroup  CanTrcv_30_Tja1145PCHasMacros  CanTrcv_30_Tja1145 Has Macros (PRE_COMPILE)
  \brief  These macros can be used to detect at runtime a deactivated piece of information. TRUE in the CONFIGURATION_VARIANT PRE-COMPILE, TRUE or FALSE in the CONFIGURATION_VARIANT POST-BUILD.
  \{
*/ 
#define CanTrcv_30_Tja1145_HasChannelUsed()                           (TRUE != FALSE)
#define CanTrcv_30_Tja1145_HasGeneratorCompatibilityVersion()         (TRUE != FALSE)
#define CanTrcv_30_Tja1145_HasPnCfg()                                 (TRUE != FALSE)
#define CanTrcv_30_Tja1145_HasBaudrateOfPnCfg()                       (TRUE != FALSE)
#define CanTrcv_30_Tja1145_HasCanId0OfPnCfg()                         (TRUE != FALSE)
#define CanTrcv_30_Tja1145_HasCanId1OfPnCfg()                         (TRUE != FALSE)
#define CanTrcv_30_Tja1145_HasCanId2OfPnCfg()                         (TRUE != FALSE)
#define CanTrcv_30_Tja1145_HasCanId3OfPnCfg()                         (TRUE != FALSE)
#define CanTrcv_30_Tja1145_HasCanIdMask0OfPnCfg()                     (TRUE != FALSE)
#define CanTrcv_30_Tja1145_HasCanIdMask1OfPnCfg()                     (TRUE != FALSE)
#define CanTrcv_30_Tja1145_HasCanIdMask2OfPnCfg()                     (TRUE != FALSE)
#define CanTrcv_30_Tja1145_HasCanIdMask3OfPnCfg()                     (TRUE != FALSE)
#define CanTrcv_30_Tja1145_HasFrameControlOfPnCfg()                   (TRUE != FALSE)
#define CanTrcv_30_Tja1145_HasPnEnabled()                             (TRUE != FALSE)
#define CanTrcv_30_Tja1145_HasPnPayloadCfg()                          (TRUE != FALSE)
#define CanTrcv_30_Tja1145_HasDataMask0OfPnPayloadCfg()               (TRUE != FALSE)
#define CanTrcv_30_Tja1145_HasDataMask1OfPnPayloadCfg()               (TRUE != FALSE)
#define CanTrcv_30_Tja1145_HasDataMask2OfPnPayloadCfg()               (TRUE != FALSE)
#define CanTrcv_30_Tja1145_HasDataMask3OfPnPayloadCfg()               (TRUE != FALSE)
#define CanTrcv_30_Tja1145_HasDataMask4OfPnPayloadCfg()               (TRUE != FALSE)
#define CanTrcv_30_Tja1145_HasDataMask5OfPnPayloadCfg()               (TRUE != FALSE)
#define CanTrcv_30_Tja1145_HasDataMask6OfPnPayloadCfg()               (TRUE != FALSE)
#define CanTrcv_30_Tja1145_HasDataMask7OfPnPayloadCfg()               (TRUE != FALSE)
#define CanTrcv_30_Tja1145_HasSizeOfChannelUsed()                     (TRUE != FALSE)
#define CanTrcv_30_Tja1145_HasSizeOfPnCfg()                           (TRUE != FALSE)
#define CanTrcv_30_Tja1145_HasSizeOfPnEnabled()                       (TRUE != FALSE)
#define CanTrcv_30_Tja1145_HasSizeOfPnPayloadCfg()                    (TRUE != FALSE)
#define CanTrcv_30_Tja1145_HasSizeOfSpiChannelCfg()                   (TRUE != FALSE)
#define CanTrcv_30_Tja1145_HasSizeOfSpiSequenceCfg()                  (TRUE != FALSE)
#define CanTrcv_30_Tja1145_HasSizeOfWakeupByBusUsed()                 (TRUE != FALSE)
#define CanTrcv_30_Tja1145_HasSizeOfWakeupSource()                    (TRUE != FALSE)
#define CanTrcv_30_Tja1145_HasSizeOfWuSrcPor()                        (TRUE != FALSE)
#define CanTrcv_30_Tja1145_HasSizeOfWuSrcSyserr()                     (TRUE != FALSE)
#define CanTrcv_30_Tja1145_HasSpiChannelCfg()                         (TRUE != FALSE)
#define CanTrcv_30_Tja1145_HasCanTrcvSpiChRWData_0OfSpiChannelCfg()   (TRUE != FALSE)
#define CanTrcv_30_Tja1145_HasCanTrcvSpiChRWData_1OfSpiChannelCfg()   (TRUE != FALSE)
#define CanTrcv_30_Tja1145_HasCanTrcvSpiChRWData_2OfSpiChannelCfg()   (TRUE != FALSE)
#define CanTrcv_30_Tja1145_HasCanTrcvSpiChRWData_3OfSpiChannelCfg()   (TRUE != FALSE)
#define CanTrcv_30_Tja1145_HasSpiSequenceCfg()                        (TRUE != FALSE)
#define CanTrcv_30_Tja1145_HasCanTrcvSeqRWDataLargeOfSpiSequenceCfg() (TRUE != FALSE)
#define CanTrcv_30_Tja1145_HasCanTrcvSeqRWDataMediumOfSpiSequenceCfg() (TRUE != FALSE)
#define CanTrcv_30_Tja1145_HasCanTrcvSeqRWDataNormalOfSpiSequenceCfg() (TRUE != FALSE)
#define CanTrcv_30_Tja1145_HasWakeupByBusUsed()                       (TRUE != FALSE)
#define CanTrcv_30_Tja1145_HasWakeupSource()                          (TRUE != FALSE)
#define CanTrcv_30_Tja1145_HasWuSrcPor()                              (TRUE != FALSE)
#define CanTrcv_30_Tja1145_HasWuSrcSyserr()                           (TRUE != FALSE)
#define CanTrcv_30_Tja1145_HasPCConfig()                              (TRUE != FALSE)
#define CanTrcv_30_Tja1145_HasChannelUsedOfPCConfig()                 (TRUE != FALSE)
#define CanTrcv_30_Tja1145_HasGeneratorCompatibilityVersionOfPCConfig() (TRUE != FALSE)
#define CanTrcv_30_Tja1145_HasPnCfgOfPCConfig()                       (TRUE != FALSE)
#define CanTrcv_30_Tja1145_HasPnEnabledOfPCConfig()                   (TRUE != FALSE)
#define CanTrcv_30_Tja1145_HasPnPayloadCfgOfPCConfig()                (TRUE != FALSE)
#define CanTrcv_30_Tja1145_HasSizeOfChannelUsedOfPCConfig()           (TRUE != FALSE)
#define CanTrcv_30_Tja1145_HasSizeOfPnCfgOfPCConfig()                 (TRUE != FALSE)
#define CanTrcv_30_Tja1145_HasSizeOfPnEnabledOfPCConfig()             (TRUE != FALSE)
#define CanTrcv_30_Tja1145_HasSizeOfPnPayloadCfgOfPCConfig()          (TRUE != FALSE)
#define CanTrcv_30_Tja1145_HasSizeOfSpiChannelCfgOfPCConfig()         (TRUE != FALSE)
#define CanTrcv_30_Tja1145_HasSizeOfSpiSequenceCfgOfPCConfig()        (TRUE != FALSE)
#define CanTrcv_30_Tja1145_HasSizeOfWakeupByBusUsedOfPCConfig()       (TRUE != FALSE)
#define CanTrcv_30_Tja1145_HasSizeOfWakeupSourceOfPCConfig()          (TRUE != FALSE)
#define CanTrcv_30_Tja1145_HasSizeOfWuSrcPorOfPCConfig()              (TRUE != FALSE)
#define CanTrcv_30_Tja1145_HasSizeOfWuSrcSyserrOfPCConfig()           (TRUE != FALSE)
#define CanTrcv_30_Tja1145_HasSpiChannelCfgOfPCConfig()               (TRUE != FALSE)
#define CanTrcv_30_Tja1145_HasSpiSequenceCfgOfPCConfig()              (TRUE != FALSE)
#define CanTrcv_30_Tja1145_HasWakeupByBusUsedOfPCConfig()             (TRUE != FALSE)
#define CanTrcv_30_Tja1145_HasWakeupSourceOfPCConfig()                (TRUE != FALSE)
#define CanTrcv_30_Tja1145_HasWuSrcPorOfPCConfig()                    (TRUE != FALSE)
#define CanTrcv_30_Tja1145_HasWuSrcSyserrOfPCConfig()                 (TRUE != FALSE)
/** 
  \}
*/ 

  /* PRQA L:MACROS_3453 */
/** 
  \}
*/ 

/**********************************************************************************************************************
  CONFIGURATION CLASS: PRE_COMPILE
  SECTION: GLOBAL ACCESS FUNCTION MACROS
**********************************************************************************************************************/

/** 
  \defgroup  DataAccessMacros  Data Access Macros
  \brief  generated data access macros to abstract the generated data from the code to read and write CONST or VAR data.
  \{
*/ 
  /* PRQA S 3453 MACROS_3453 */  /* MD_CSL_3453 */
/**********************************************************************************************************************
  CONFIGURATION CLASS: LINK
  SECTION: GLOBAL FUNCTION MACROS
**********************************************************************************************************************/
  /* PRQA L:MACROS_3453 */
/** 
  \}
*/ 

/**********************************************************************************************************************
  CONFIGURATION CLASS: LINK
  SECTION: GLOBAL ACCESS FUNCTION MACROS
**********************************************************************************************************************/

/** 
  \defgroup  DataAccessMacros  Data Access Macros
  \brief  generated data access macros to abstract the generated data from the code to read and write CONST or VAR data.
  \{
*/ 
  /* PRQA S 3453 MACROS_3453 */  /* MD_CSL_3453 */
/**********************************************************************************************************************
  CONFIGURATION CLASS: POST_BUILD
  SECTION: GLOBAL FUNCTION MACROS
**********************************************************************************************************************/
  /* PRQA L:MACROS_3453 */
/** 
  \}
*/ 

/**********************************************************************************************************************
  CONFIGURATION CLASS: POST_BUILD
  SECTION: GLOBAL ACCESS FUNCTION MACROS
**********************************************************************************************************************/


/**********************************************************************************************************************
  GLOBAL DATA TYPES AND STRUCTURES
**********************************************************************************************************************/
/**********************************************************************************************************************
  CONFIGURATION CLASS: PRE_COMPILE
  SECTION: GLOBAL SIMPLE DATA TYPES AND STRUCTURES
**********************************************************************************************************************/
/** 
  \defgroup  CanTrcv_30_Tja1145PCIterableTypes  CanTrcv_30_Tja1145 Iterable Types (PRE_COMPILE)
  \brief  These type definitions are used to iterate over an array with least processor cycles for variable access as possible.
  \{
*/ 
/**   \brief  type used to iterate CanTrcv_30_Tja1145_ChannelUsed */
typedef uint8_least CanTrcv_30_Tja1145_ChannelUsedIterType;

/**   \brief  type used to iterate CanTrcv_30_Tja1145_PnCfg */
typedef uint8_least CanTrcv_30_Tja1145_PnCfgIterType;

/**   \brief  type used to iterate CanTrcv_30_Tja1145_PnEnabled */
typedef uint8_least CanTrcv_30_Tja1145_PnEnabledIterType;

/**   \brief  type used to iterate CanTrcv_30_Tja1145_PnPayloadCfg */
typedef uint8_least CanTrcv_30_Tja1145_PnPayloadCfgIterType;

/**   \brief  type used to iterate CanTrcv_30_Tja1145_SpiChannelCfg */
typedef uint8_least CanTrcv_30_Tja1145_SpiChannelCfgIterType;

/**   \brief  type used to iterate CanTrcv_30_Tja1145_SpiSequenceCfg */
typedef uint8_least CanTrcv_30_Tja1145_SpiSequenceCfgIterType;

/**   \brief  type used to iterate CanTrcv_30_Tja1145_WakeupByBusUsed */
typedef uint8_least CanTrcv_30_Tja1145_WakeupByBusUsedIterType;

/**   \brief  type used to iterate CanTrcv_30_Tja1145_WakeupSource */
typedef uint8_least CanTrcv_30_Tja1145_WakeupSourceIterType;

/**   \brief  type used to iterate CanTrcv_30_Tja1145_WuSrcPor */
typedef uint8_least CanTrcv_30_Tja1145_WuSrcPorIterType;

/**   \brief  type used to iterate CanTrcv_30_Tja1145_WuSrcSyserr */
typedef uint8_least CanTrcv_30_Tja1145_WuSrcSyserrIterType;

/** 
  \}
*/ 

/** 
  \defgroup  CanTrcv_30_Tja1145PCValueTypes  CanTrcv_30_Tja1145 Value Types (PRE_COMPILE)
  \brief  These type definitions are used for value based data representations.
  \{
*/ 
/**   \brief  value based type definition for CanTrcv_30_Tja1145_ChannelUsed */
typedef boolean CanTrcv_30_Tja1145_ChannelUsedType;

/**   \brief  value based type definition for CanTrcv_30_Tja1145_GeneratorCompatibilityVersion */
typedef uint32 CanTrcv_30_Tja1145_GeneratorCompatibilityVersionType;

/**   \brief  value based type definition for CanTrcv_30_Tja1145_BaudrateOfPnCfg */
typedef uint8 CanTrcv_30_Tja1145_BaudrateOfPnCfgType;

/**   \brief  value based type definition for CanTrcv_30_Tja1145_CanId0OfPnCfg */
typedef uint8 CanTrcv_30_Tja1145_CanId0OfPnCfgType;

/**   \brief  value based type definition for CanTrcv_30_Tja1145_CanId1OfPnCfg */
typedef uint8 CanTrcv_30_Tja1145_CanId1OfPnCfgType;

/**   \brief  value based type definition for CanTrcv_30_Tja1145_CanId2OfPnCfg */
typedef uint8 CanTrcv_30_Tja1145_CanId2OfPnCfgType;

/**   \brief  value based type definition for CanTrcv_30_Tja1145_CanId3OfPnCfg */
typedef uint8 CanTrcv_30_Tja1145_CanId3OfPnCfgType;

/**   \brief  value based type definition for CanTrcv_30_Tja1145_CanIdMask0OfPnCfg */
typedef uint8 CanTrcv_30_Tja1145_CanIdMask0OfPnCfgType;

/**   \brief  value based type definition for CanTrcv_30_Tja1145_CanIdMask1OfPnCfg */
typedef uint8 CanTrcv_30_Tja1145_CanIdMask1OfPnCfgType;

/**   \brief  value based type definition for CanTrcv_30_Tja1145_CanIdMask2OfPnCfg */
typedef uint8 CanTrcv_30_Tja1145_CanIdMask2OfPnCfgType;

/**   \brief  value based type definition for CanTrcv_30_Tja1145_CanIdMask3OfPnCfg */
typedef uint8 CanTrcv_30_Tja1145_CanIdMask3OfPnCfgType;

/**   \brief  value based type definition for CanTrcv_30_Tja1145_FrameControlOfPnCfg */
typedef uint8 CanTrcv_30_Tja1145_FrameControlOfPnCfgType;

/**   \brief  value based type definition for CanTrcv_30_Tja1145_PnEnabled */
typedef boolean CanTrcv_30_Tja1145_PnEnabledType;

/**   \brief  value based type definition for CanTrcv_30_Tja1145_DataMask0OfPnPayloadCfg */
typedef uint8 CanTrcv_30_Tja1145_DataMask0OfPnPayloadCfgType;

/**   \brief  value based type definition for CanTrcv_30_Tja1145_DataMask1OfPnPayloadCfg */
typedef uint8 CanTrcv_30_Tja1145_DataMask1OfPnPayloadCfgType;

/**   \brief  value based type definition for CanTrcv_30_Tja1145_DataMask2OfPnPayloadCfg */
typedef uint8 CanTrcv_30_Tja1145_DataMask2OfPnPayloadCfgType;

/**   \brief  value based type definition for CanTrcv_30_Tja1145_DataMask3OfPnPayloadCfg */
typedef uint8 CanTrcv_30_Tja1145_DataMask3OfPnPayloadCfgType;

/**   \brief  value based type definition for CanTrcv_30_Tja1145_DataMask4OfPnPayloadCfg */
typedef uint8 CanTrcv_30_Tja1145_DataMask4OfPnPayloadCfgType;

/**   \brief  value based type definition for CanTrcv_30_Tja1145_DataMask5OfPnPayloadCfg */
typedef uint8 CanTrcv_30_Tja1145_DataMask5OfPnPayloadCfgType;

/**   \brief  value based type definition for CanTrcv_30_Tja1145_DataMask6OfPnPayloadCfg */
typedef uint8 CanTrcv_30_Tja1145_DataMask6OfPnPayloadCfgType;

/**   \brief  value based type definition for CanTrcv_30_Tja1145_DataMask7OfPnPayloadCfg */
typedef uint8 CanTrcv_30_Tja1145_DataMask7OfPnPayloadCfgType;

/**   \brief  value based type definition for CanTrcv_30_Tja1145_SizeOfChannelUsed */
typedef uint16 CanTrcv_30_Tja1145_SizeOfChannelUsedType;

/**   \brief  value based type definition for CanTrcv_30_Tja1145_SizeOfPnCfg */
typedef uint16 CanTrcv_30_Tja1145_SizeOfPnCfgType;

/**   \brief  value based type definition for CanTrcv_30_Tja1145_SizeOfPnEnabled */
typedef uint16 CanTrcv_30_Tja1145_SizeOfPnEnabledType;

/**   \brief  value based type definition for CanTrcv_30_Tja1145_SizeOfPnPayloadCfg */
typedef uint16 CanTrcv_30_Tja1145_SizeOfPnPayloadCfgType;

/**   \brief  value based type definition for CanTrcv_30_Tja1145_SizeOfSpiChannelCfg */
typedef uint16 CanTrcv_30_Tja1145_SizeOfSpiChannelCfgType;

/**   \brief  value based type definition for CanTrcv_30_Tja1145_SizeOfSpiSequenceCfg */
typedef uint16 CanTrcv_30_Tja1145_SizeOfSpiSequenceCfgType;

/**   \brief  value based type definition for CanTrcv_30_Tja1145_SizeOfWakeupByBusUsed */
typedef uint16 CanTrcv_30_Tja1145_SizeOfWakeupByBusUsedType;

/**   \brief  value based type definition for CanTrcv_30_Tja1145_SizeOfWakeupSource */
typedef uint16 CanTrcv_30_Tja1145_SizeOfWakeupSourceType;

/**   \brief  value based type definition for CanTrcv_30_Tja1145_SizeOfWuSrcPor */
typedef uint16 CanTrcv_30_Tja1145_SizeOfWuSrcPorType;

/**   \brief  value based type definition for CanTrcv_30_Tja1145_SizeOfWuSrcSyserr */
typedef uint16 CanTrcv_30_Tja1145_SizeOfWuSrcSyserrType;

/**   \brief  value based type definition for CanTrcv_30_Tja1145_WakeupByBusUsed */
typedef boolean CanTrcv_30_Tja1145_WakeupByBusUsedType;

/** 
  \}
*/ 

/**********************************************************************************************************************
  CONFIGURATION CLASS: PRE_COMPILE
  SECTION: GLOBAL COMPLEX DATA TYPES AND STRUCTURES
**********************************************************************************************************************/
/** 
  \defgroup  CanTrcv_30_Tja1145PCStructTypes  CanTrcv_30_Tja1145 Struct Types (PRE_COMPILE)
  \brief  These type definitions are used for structured data representations.
  \{
*/ 
/**   \brief  type used in CanTrcv_30_Tja1145_PnCfg */
typedef struct sCanTrcv_30_Tja1145_PnCfgType
{
  CanTrcv_30_Tja1145_BaudrateOfPnCfgType BaudrateOfPnCfg;  /**< Baudrate register */
  CanTrcv_30_Tja1145_CanId0OfPnCfgType CanId0OfPnCfg;  /**< CAN identifier register 0 [Std: don't care, Ext: ID07..ID00] */
  CanTrcv_30_Tja1145_CanId1OfPnCfgType CanId1OfPnCfg;  /**< CAN identifier register 1 [Std: don't care, Ext: ID15..ID08] */
  CanTrcv_30_Tja1145_CanId2OfPnCfgType CanId2OfPnCfg;  /**< CAN identifier register 2 [Std: ID05..ID00: Ext: ID23..ID16] */
  CanTrcv_30_Tja1145_CanId3OfPnCfgType CanId3OfPnCfg;  /**< CAN identifier register 3 [Std: ID10..ID06, Ext: ID28..ID24] */
  CanTrcv_30_Tja1145_CanIdMask0OfPnCfgType CanIdMask0OfPnCfg;  /**< CAN identifier mask register 0 [Std: don't care, Ext: ID07..ID00] */
  CanTrcv_30_Tja1145_CanIdMask1OfPnCfgType CanIdMask1OfPnCfg;  /**< CAN identifier mask register 1 [Std: don't care, Ext: ID15..ID08] */
  CanTrcv_30_Tja1145_CanIdMask2OfPnCfgType CanIdMask2OfPnCfg;  /**< CAN identifier mask register 2 [Std: ID05..ID00: Ext: ID23..ID16] */
  CanTrcv_30_Tja1145_CanIdMask3OfPnCfgType CanIdMask3OfPnCfg;  /**< CAN identifier mask register 3 [Std: ID10..ID06, Ext: ID28..ID24]  */
  CanTrcv_30_Tja1145_FrameControlOfPnCfgType FrameControlOfPnCfg;  /**< Frame control register (IDE, PNMD, DLC) */
} CanTrcv_30_Tja1145_PnCfgType;

/**   \brief  type used in CanTrcv_30_Tja1145_PnPayloadCfg */
typedef struct sCanTrcv_30_Tja1145_PnPayloadCfgType
{
  CanTrcv_30_Tja1145_DataMask0OfPnPayloadCfgType DataMask0OfPnPayloadCfg;  /**< Data mask register 0 */
  CanTrcv_30_Tja1145_DataMask1OfPnPayloadCfgType DataMask1OfPnPayloadCfg;  /**< Data mask register 1 */
  CanTrcv_30_Tja1145_DataMask2OfPnPayloadCfgType DataMask2OfPnPayloadCfg;  /**< Data mask register 2 */
  CanTrcv_30_Tja1145_DataMask3OfPnPayloadCfgType DataMask3OfPnPayloadCfg;  /**< Data mask register 3 */
  CanTrcv_30_Tja1145_DataMask4OfPnPayloadCfgType DataMask4OfPnPayloadCfg;  /**< Data mask register 4 */
  CanTrcv_30_Tja1145_DataMask5OfPnPayloadCfgType DataMask5OfPnPayloadCfg;  /**< Data mask register 5 */
  CanTrcv_30_Tja1145_DataMask6OfPnPayloadCfgType DataMask6OfPnPayloadCfg;  /**< Data mask register 6 */
  CanTrcv_30_Tja1145_DataMask7OfPnPayloadCfgType DataMask7OfPnPayloadCfg;  /**< Data mask register 7 */
} CanTrcv_30_Tja1145_PnPayloadCfgType;

/**   \brief  type used in CanTrcv_30_Tja1145_SpiChannelCfg */
typedef struct sCanTrcv_30_Tja1145_SpiChannelCfgType
{
  Spi_ChannelType CanTrcvSpiChRWData_0OfSpiChannelCfg;
  Spi_ChannelType CanTrcvSpiChRWData_1OfSpiChannelCfg;
  Spi_ChannelType CanTrcvSpiChRWData_2OfSpiChannelCfg;
  Spi_ChannelType CanTrcvSpiChRWData_3OfSpiChannelCfg;
} CanTrcv_30_Tja1145_SpiChannelCfgType;

/**   \brief  type used in CanTrcv_30_Tja1145_SpiSequenceCfg */
typedef struct sCanTrcv_30_Tja1145_SpiSequenceCfgType
{
  Spi_SequenceType CanTrcvSeqRWDataLargeOfSpiSequenceCfg;
  Spi_SequenceType CanTrcvSeqRWDataMediumOfSpiSequenceCfg;
  Spi_SequenceType CanTrcvSeqRWDataNormalOfSpiSequenceCfg;
} CanTrcv_30_Tja1145_SpiSequenceCfgType;

/** 
  \}
*/ 

/** 
  \defgroup  CanTrcv_30_Tja1145PCRootValueTypes  CanTrcv_30_Tja1145 Root Value Types (PRE_COMPILE)
  \brief  These type definitions are used for value representations in root arrays.
  \{
*/ 
/**   \brief  type used in CanTrcv_30_Tja1145_PCConfig */
typedef struct sCanTrcv_30_Tja1145_PCConfigType
{
  uint8 CanTrcv_30_Tja1145_PCConfigNeverUsed;  /**< dummy entry for the structure in the configuration variant precompile which is not used by the code. */
} CanTrcv_30_Tja1145_PCConfigType;

typedef CanTrcv_30_Tja1145_PCConfigType CanTrcv_30_Tja1145_ConfigType;  /**< A structure type is present for data in each configuration class. This typedef redefines the probably different name to the specified one. */

/** 
  \}
*/ 


/**********************************************************************************************************************
  CONFIGURATION CLASS: LINK
  SECTION: GLOBAL SIMPLE DATA TYPES AND STRUCTURES
**********************************************************************************************************************/
/**********************************************************************************************************************
  CONFIGURATION CLASS: LINK
  SECTION: GLOBAL COMPLEX DATA TYPES AND STRUCTURES
**********************************************************************************************************************/

/**********************************************************************************************************************
  CONFIGURATION CLASS: POST_BUILD
  SECTION: GLOBAL SIMPLE DATA TYPES AND STRUCTURES
**********************************************************************************************************************/
/**********************************************************************************************************************
  CONFIGURATION CLASS: POST_BUILD
  SECTION: GLOBAL COMPLEX DATA TYPES AND STRUCTURES
**********************************************************************************************************************/


/**********************************************************************************************************************
  GLOBAL DATA PROTOTYPES
**********************************************************************************************************************/
/**********************************************************************************************************************
  CONFIGURATION CLASS: PRE_COMPILE
  SECTION: GLOBAL DATA PROTOTYPES
**********************************************************************************************************************/
/**********************************************************************************************************************
  CanTrcv_30_Tja1145_ChannelUsed
**********************************************************************************************************************/
#define CANTRCV_30_TJA1145_START_SEC_CONST_8BIT
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
extern CONST(CanTrcv_30_Tja1145_ChannelUsedType, CANTRCV_30_TJA1145_CONST) CanTrcv_30_Tja1145_ChannelUsed[1];
#define CANTRCV_30_TJA1145_STOP_SEC_CONST_8BIT
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  CanTrcv_30_Tja1145_PnCfg
**********************************************************************************************************************/
/** 
  \var    CanTrcv_30_Tja1145_PnCfg
  \details
  Element         Description
  Baudrate        Baudrate register
  CanId0          CAN identifier register 0 [Std: don't care, Ext: ID07..ID00]
  CanId1          CAN identifier register 1 [Std: don't care, Ext: ID15..ID08]
  CanId2          CAN identifier register 2 [Std: ID05..ID00: Ext: ID23..ID16]
  CanId3          CAN identifier register 3 [Std: ID10..ID06, Ext: ID28..ID24]
  CanIdMask0      CAN identifier mask register 0 [Std: don't care, Ext: ID07..ID00]
  CanIdMask1      CAN identifier mask register 1 [Std: don't care, Ext: ID15..ID08]
  CanIdMask2      CAN identifier mask register 2 [Std: ID05..ID00: Ext: ID23..ID16]
  CanIdMask3      CAN identifier mask register 3 [Std: ID10..ID06, Ext: ID28..ID24] 
  FrameControl    Frame control register (IDE, PNMD, DLC)
*/ 
#define CANTRCV_30_TJA1145_START_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
extern CONST(CanTrcv_30_Tja1145_PnCfgType, CANTRCV_30_TJA1145_CONST) CanTrcv_30_Tja1145_PnCfg[1];
#define CANTRCV_30_TJA1145_STOP_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  CanTrcv_30_Tja1145_PnEnabled
**********************************************************************************************************************/
#define CANTRCV_30_TJA1145_START_SEC_CONST_8BIT
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
extern CONST(CanTrcv_30_Tja1145_PnEnabledType, CANTRCV_30_TJA1145_CONST) CanTrcv_30_Tja1145_PnEnabled[1];
#define CANTRCV_30_TJA1145_STOP_SEC_CONST_8BIT
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  CanTrcv_30_Tja1145_PnPayloadCfg
**********************************************************************************************************************/
/** 
  \var    CanTrcv_30_Tja1145_PnPayloadCfg
  \details
  Element      Description
  DataMask0    Data mask register 0
  DataMask1    Data mask register 1
  DataMask2    Data mask register 2
  DataMask3    Data mask register 3
  DataMask4    Data mask register 4
  DataMask5    Data mask register 5
  DataMask6    Data mask register 6
  DataMask7    Data mask register 7
*/ 
#define CANTRCV_30_TJA1145_START_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
extern CONST(CanTrcv_30_Tja1145_PnPayloadCfgType, CANTRCV_30_TJA1145_CONST) CanTrcv_30_Tja1145_PnPayloadCfg[1];
#define CANTRCV_30_TJA1145_STOP_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  CanTrcv_30_Tja1145_SpiChannelCfg
**********************************************************************************************************************/
/** 
  \var    CanTrcv_30_Tja1145_SpiChannelCfg
  \details
  Element                 Description
  CanTrcvSpiChRWData_0
  CanTrcvSpiChRWData_1
  CanTrcvSpiChRWData_2
  CanTrcvSpiChRWData_3
*/ 
#define CANTRCV_30_TJA1145_START_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
extern CONST(CanTrcv_30_Tja1145_SpiChannelCfgType, CANTRCV_30_TJA1145_CONST) CanTrcv_30_Tja1145_SpiChannelCfg[1];  /* PRQA S 0777 */  /* MD_MSR_5.1_777 */
#define CANTRCV_30_TJA1145_STOP_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  CanTrcv_30_Tja1145_SpiSequenceCfg
**********************************************************************************************************************/
/** 
  \var    CanTrcv_30_Tja1145_SpiSequenceCfg
  \details
  Element                   Description
  CanTrcvSeqRWDataLarge 
  CanTrcvSeqRWDataMedium
  CanTrcvSeqRWDataNormal
*/ 
#define CANTRCV_30_TJA1145_START_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
extern CONST(CanTrcv_30_Tja1145_SpiSequenceCfgType, CANTRCV_30_TJA1145_CONST) CanTrcv_30_Tja1145_SpiSequenceCfg[1];  /* PRQA S 0777 */  /* MD_MSR_5.1_777 */
#define CANTRCV_30_TJA1145_STOP_SEC_CONST_UNSPECIFIED
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  CanTrcv_30_Tja1145_WakeupByBusUsed
**********************************************************************************************************************/
#define CANTRCV_30_TJA1145_START_SEC_CONST_8BIT
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
extern CONST(CanTrcv_30_Tja1145_WakeupByBusUsedType, CANTRCV_30_TJA1145_CONST) CanTrcv_30_Tja1145_WakeupByBusUsed[1];  /* PRQA S 0777 */  /* MD_MSR_5.1_777 */
#define CANTRCV_30_TJA1145_STOP_SEC_CONST_8BIT
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  CanTrcv_30_Tja1145_WakeupSource
**********************************************************************************************************************/
#define CANTRCV_30_TJA1145_START_SEC_CONST_32BIT
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
extern CONST(EcuM_WakeupSourceType, CANTRCV_30_TJA1145_CONST) CanTrcv_30_Tja1145_WakeupSource[1];
#define CANTRCV_30_TJA1145_STOP_SEC_CONST_32BIT
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  CanTrcv_30_Tja1145_WuSrcPor
**********************************************************************************************************************/
#define CANTRCV_30_TJA1145_START_SEC_CONST_32BIT
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
extern CONST(EcuM_WakeupSourceType, CANTRCV_30_TJA1145_CONST) CanTrcv_30_Tja1145_WuSrcPor[1];
#define CANTRCV_30_TJA1145_STOP_SEC_CONST_32BIT
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */

/**********************************************************************************************************************
  CanTrcv_30_Tja1145_WuSrcSyserr
**********************************************************************************************************************/
#define CANTRCV_30_TJA1145_START_SEC_CONST_32BIT
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */
extern CONST(EcuM_WakeupSourceType, CANTRCV_30_TJA1145_CONST) CanTrcv_30_Tja1145_WuSrcSyserr[1];
#define CANTRCV_30_TJA1145_STOP_SEC_CONST_32BIT
/*lint -save -esym(961, 19.1) */
#include "MemMap.h"  /* PRQA S 5087 */  /* MD_MSR_19.1 */
/*lint -restore */


/**********************************************************************************************************************
  CONFIGURATION CLASS: LINK
  SECTION: GLOBAL DATA PROTOTYPES
**********************************************************************************************************************/

/**********************************************************************************************************************
  CONFIGURATION CLASS: POST_BUILD
  SECTION: GLOBAL DATA PROTOTYPES
**********************************************************************************************************************/


/**********************************************************************************************************************
  GLOBAL FUNCTION PROTOTYPES
**********************************************************************************************************************/
/**********************************************************************************************************************
  CONFIGURATION CLASS: PRE_COMPILE
  SECTION: GLOBAL FUNCTION PROTOTYPES
**********************************************************************************************************************/

/**********************************************************************************************************************
  CONFIGURATION CLASS: LINK
  SECTION: GLOBAL FUNCTION PROTOTYPES
**********************************************************************************************************************/

/**********************************************************************************************************************
  CONFIGURATION CLASS: POST_BUILD
  SECTION: GLOBAL FUNCTION PROTOTYPES
**********************************************************************************************************************/





#endif /* CANTRCV_30_TJA1145_CFG_H */

