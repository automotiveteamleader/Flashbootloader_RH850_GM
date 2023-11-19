/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief        MemMap section definition to locate (link) data or code in memory
 *  \details      This file contains compiler specific section definition for every segment that can be mapped
 *                within the Flash Bootloader to a specific location.
 *
 *  \note
 *                Please note, that this file contains a collection of definitions to be used with the Flash Bootloader.
 *                This code may influence the behavior of the Bootloader in principle. Therefore, great care
 *                must be taken to verify the correctness of the implementation.
 *
 *                The contents of the originally delivered files are only examples resp. implementation proposals.
 *                With regard to the fact that these definitions are meant for demonstration purposes only,
 *                Vector Informatik's liability shall be expressly excluded in cases of ordinary negligence, to the
 *                extent admissible by law or statute.
 *
 *  -------------------------------------------------------------------------------------------------------------------
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \par Copyright
 *  \verbatim
 *  Copyright (c) 2017 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  THIS IS A GENERATED FILE
 *  Generator version : 01.07.00
 *  Generation date :   2017-07-10
 *********************************************************************************************************************/

/* PRQA S 0841 EOF */ /* MD_MSR_19.6 */
/* PRQA S 0883 EOF */ /* MD_MemMap_0883 */
/* PRQA S 3116 EOF */ /* MD_MSR_3.4 */

#define FBL_MEMMAP_ERROR

#if defined( CANTRCV_30_TJA1145_START_SEC_CODE )
   /* #pragma <enter your compiler specific pragma> */
# undef CANTRCV_30_TJA1145_START_SEC_CODE
# undef FBL_MEMMAP_ERROR
#endif

#if defined( CANTRCV_30_TJA1145_STOP_SEC_CODE )
   /* #pragma <enter your compiler specific pragma> */
# undef CANTRCV_30_TJA1145_STOP_SEC_CODE
# undef FBL_MEMMAP_ERROR
#endif

/* ----------------------------------------------------------------------------------------------------------------- */

#if defined( CANTRCV_30_TJA1145_START_SEC_CONST_32BIT )
   /* #pragma <enter your compiler specific pragma> */
# undef CANTRCV_30_TJA1145_START_SEC_CONST_32BIT
# undef FBL_MEMMAP_ERROR
#endif

#if defined( CANTRCV_30_TJA1145_STOP_SEC_CONST_32BIT )
   /* #pragma <enter your compiler specific pragma> */
# undef CANTRCV_30_TJA1145_STOP_SEC_CONST_32BIT
# undef FBL_MEMMAP_ERROR
#endif

/* ----------------------------------------------------------------------------------------------------------------- */

#if defined( CANTRCV_30_TJA1145_START_SEC_CONST_8BIT )
   /* #pragma <enter your compiler specific pragma> */
# undef CANTRCV_30_TJA1145_START_SEC_CONST_8BIT
# undef FBL_MEMMAP_ERROR
#endif

#if defined( CANTRCV_30_TJA1145_STOP_SEC_CONST_8BIT )
   /* #pragma <enter your compiler specific pragma> */
# undef CANTRCV_30_TJA1145_STOP_SEC_CONST_8BIT
# undef FBL_MEMMAP_ERROR
#endif

/* ----------------------------------------------------------------------------------------------------------------- */

#if defined( CANTRCV_30_TJA1145_START_SEC_CONST_UNSPECIFIED )
   /* #pragma <enter your compiler specific pragma> */
# undef CANTRCV_30_TJA1145_START_SEC_CONST_UNSPECIFIED
# undef FBL_MEMMAP_ERROR
#endif

#if defined( CANTRCV_30_TJA1145_STOP_SEC_CONST_UNSPECIFIED )
   /* #pragma <enter your compiler specific pragma> */
# undef CANTRCV_30_TJA1145_STOP_SEC_CONST_UNSPECIFIED
# undef FBL_MEMMAP_ERROR
#endif

/* ----------------------------------------------------------------------------------------------------------------- */

#if defined( CANTRCV_30_TJA1145_START_SEC_VAR_INIT_UNSPECIFIED )
   /* #pragma <enter your compiler specific pragma> */
# undef CANTRCV_30_TJA1145_START_SEC_VAR_INIT_UNSPECIFIED
# undef FBL_MEMMAP_ERROR
#endif

#if defined( CANTRCV_30_TJA1145_STOP_SEC_VAR_INIT_UNSPECIFIED )
   /* #pragma <enter your compiler specific pragma> */
# undef CANTRCV_30_TJA1145_STOP_SEC_VAR_INIT_UNSPECIFIED
# undef FBL_MEMMAP_ERROR
#endif

/* ----------------------------------------------------------------------------------------------------------------- */

#if defined( CANTRCV_30_TJA1145_START_SEC_VAR_NOINIT_UNSPECIFIED )
   /* #pragma <enter your compiler specific pragma> */
# undef CANTRCV_30_TJA1145_START_SEC_VAR_NOINIT_UNSPECIFIED
# undef FBL_MEMMAP_ERROR
#endif

#if defined( CANTRCV_30_TJA1145_STOP_SEC_VAR_NOINIT_UNSPECIFIED )
   /* #pragma <enter your compiler specific pragma> */
# undef CANTRCV_30_TJA1145_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# undef FBL_MEMMAP_ERROR
#endif

/* ----------------------------------------------------------------------------------------------------------------- */

#if defined( DRVCAN_START_SEC_CODE ) || \
    defined( DRVCAN_START_SEC_CODE_EXPORT )
   /* #pragma <enter your compiler specific pragma> */
# undef DRVCAN_START_SEC_CODE
# undef DRVCAN_START_SEC_CODE_EXPORT
# undef FBL_MEMMAP_ERROR
#endif

#if defined( DRVCAN_STOP_SEC_CODE ) || \
    defined( DRVCAN_STOP_SEC_CODE_EXPORT )
   /* #pragma <enter your compiler specific pragma> */
# undef DRVCAN_STOP_SEC_CODE
# undef DRVCAN_STOP_SEC_CODE_EXPORT
# undef FBL_MEMMAP_ERROR
#endif

/* ----------------------------------------------------------------------------------------------------------------- */

#if defined( FBLAP_COMMON_DATA_START_SEC_DATA )
   /* #pragma <enter your compiler specific pragma> */
# undef FBLAP_COMMON_DATA_START_SEC_DATA
# undef FBL_MEMMAP_ERROR
#endif

#if defined( FBLAP_COMMON_DATA_STOP_SEC_DATA )
   /* #pragma <enter your compiler specific pragma> */
# undef FBLAP_COMMON_DATA_STOP_SEC_DATA
# undef FBL_MEMMAP_ERROR
#endif

/* ----------------------------------------------------------------------------------------------------------------- */

#if defined( FBLAP_LEGACY_FBLHEADER_START_SEC_CONST )
   /* #pragma <enter your compiler specific pragma> */
# undef FBLAP_LEGACY_FBLHEADER_START_SEC_CONST
# undef FBL_MEMMAP_ERROR
#endif

#if defined( FBLAP_LEGACY_FBLHEADER_STOP_SEC_CONST )
   /* #pragma <enter your compiler specific pragma> */
# undef FBLAP_LEGACY_FBLHEADER_STOP_SEC_CONST
# undef FBL_MEMMAP_ERROR
#endif

/* ----------------------------------------------------------------------------------------------------------------- */

#if defined( FBLCOMDAT_COMMON_DATA_START_SEC_DATA )
# pragma ghs startdata
# pragma ghs section rodata=".COMMONDATA"
# undef FBLCOMDAT_COMMON_DATA_START_SEC_DATA
# undef FBL_MEMMAP_ERROR
#endif

#if defined( FBLCOMDAT_COMMON_DATA_STOP_SEC_DATA )
# pragma ghs section
# pragma ghs enddata
# undef FBLCOMDAT_COMMON_DATA_STOP_SEC_DATA
# undef FBL_MEMMAP_ERROR
#endif

/* ----------------------------------------------------------------------------------------------------------------- */

#if defined( FBLCOMDAT_SHARED_RAM_MEMORY_START_SEC_DATA )
# pragma ghs startdata
# pragma ghs section bss=".SHAREDRAM"
# undef FBLCOMDAT_SHARED_RAM_MEMORY_START_SEC_DATA
# undef FBL_MEMMAP_ERROR
#endif

#if defined( FBLCOMDAT_SHARED_RAM_MEMORY_STOP_SEC_DATA )
# pragma ghs section
# pragma ghs enddata
# undef FBLCOMDAT_SHARED_RAM_MEMORY_STOP_SEC_DATA
# undef FBL_MEMMAP_ERROR
#endif

/* ----------------------------------------------------------------------------------------------------------------- */

#if defined( FBLDIAG_RAMCODE_START_SEC_CODE )
   /* #pragma <enter your compiler specific pragma> */
# undef FBLDIAG_RAMCODE_START_SEC_CODE
# undef FBL_MEMMAP_ERROR
#endif

#if defined( FBLDIAG_RAMCODE_STOP_SEC_CODE )
   /* #pragma <enter your compiler specific pragma> */
# undef FBLDIAG_RAMCODE_STOP_SEC_CODE
# undef FBL_MEMMAP_ERROR
#endif

/* ----------------------------------------------------------------------------------------------------------------- */

#if defined( FBLDIAG_START_SEC_RAMCONST_UNSPECIFIED )
   /* #pragma <enter your compiler specific pragma> */
# undef FBLDIAG_START_SEC_RAMCONST_UNSPECIFIED
# undef FBL_MEMMAP_ERROR
#endif

#if defined( FBLDIAG_STOP_SEC_RAMCONST_UNSPECIFIED )
   /* #pragma <enter your compiler specific pragma> */
# undef FBLDIAG_STOP_SEC_RAMCONST_UNSPECIFIED
# undef FBL_MEMMAP_ERROR
#endif

/* ----------------------------------------------------------------------------------------------------------------- */

#if defined( FBLHEADER_START_SEC_CONST )
# pragma ghs startdata
# pragma ghs section rodata=".FBLHEADER"
# undef FBLHEADER_START_SEC_CONST
# undef FBL_MEMMAP_ERROR
#endif

#if defined( FBLHEADER_STOP_SEC_CONST )
# pragma ghs section
# pragma ghs enddata
# undef FBLHEADER_STOP_SEC_CONST
# undef FBL_MEMMAP_ERROR
#endif

/* ----------------------------------------------------------------------------------------------------------------- */

#if defined( FBLHW_START_SEC_CODE )
   /* #pragma <enter your compiler specific pragma> */
# undef FBLHW_START_SEC_CODE
# undef FBL_MEMMAP_ERROR
#endif

#if defined( FBLHW_STOP_SEC_CODE )
   /* #pragma <enter your compiler specific pragma> */
# undef FBLHW_STOP_SEC_CODE
# undef FBL_MEMMAP_ERROR
#endif

/* ----------------------------------------------------------------------------------------------------------------- */

#if defined( FBLLIB_MEM_RAMCODE_START_SEC_CODE ) || \
    defined( FBLLIB_MEM_RAMCODE_START_SEC_CODE_EXPORT )
   /* #pragma <enter your compiler specific pragma> */
# undef FBLLIB_MEM_RAMCODE_START_SEC_CODE
# undef FBLLIB_MEM_RAMCODE_START_SEC_CODE_EXPORT
# undef FBL_MEMMAP_ERROR
#endif

#if defined( FBLLIB_MEM_RAMCODE_STOP_SEC_CODE ) || \
    defined( FBLLIB_MEM_RAMCODE_STOP_SEC_CODE_EXPORT )
   /* #pragma <enter your compiler specific pragma> */
# undef FBLLIB_MEM_RAMCODE_STOP_SEC_CODE
# undef FBLLIB_MEM_RAMCODE_STOP_SEC_CODE_EXPORT
# undef FBL_MEMMAP_ERROR
#endif

/* ----------------------------------------------------------------------------------------------------------------- */

#if defined( FBLLIB_MEM_START_SEC_CONST )
   /* #pragma <enter your compiler specific pragma> */
# undef FBLLIB_MEM_START_SEC_CONST
# undef FBL_MEMMAP_ERROR
#endif

#if defined( FBLLIB_MEM_STOP_SEC_CONST )
   /* #pragma <enter your compiler specific pragma> */
# undef FBLLIB_MEM_STOP_SEC_CONST
# undef FBL_MEMMAP_ERROR
#endif

/* ----------------------------------------------------------------------------------------------------------------- */

#if defined( FBLLIB_MEM_START_SEC_VAR ) || \
    defined( FBLLIB_MEM_START_SEC_VAR_EXPORT )
   /* #pragma <enter your compiler specific pragma> */
# undef FBLLIB_MEM_START_SEC_VAR
# undef FBLLIB_MEM_START_SEC_VAR_EXPORT
# undef FBL_MEMMAP_ERROR
#endif

#if defined( FBLLIB_MEM_STOP_SEC_VAR ) || \
    defined( FBLLIB_MEM_STOP_SEC_VAR_EXPORT )
   /* #pragma <enter your compiler specific pragma> */
# undef FBLLIB_MEM_STOP_SEC_VAR
# undef FBLLIB_MEM_STOP_SEC_VAR_EXPORT
# undef FBL_MEMMAP_ERROR
#endif

/* ----------------------------------------------------------------------------------------------------------------- */

#if defined( FBLMIO_START_SEC_CODE )
   /* #pragma <enter your compiler specific pragma> */
# undef FBLMIO_START_SEC_CODE
# undef FBL_MEMMAP_ERROR
#endif

#if defined( FBLMIO_STOP_SEC_CODE )
   /* #pragma <enter your compiler specific pragma> */
# undef FBLMIO_STOP_SEC_CODE
# undef FBL_MEMMAP_ERROR
#endif

/* ----------------------------------------------------------------------------------------------------------------- */

#if defined( FBLTP_RAMCODE_START_SEC_CODE )
   /* #pragma <enter your compiler specific pragma> */
# undef FBLTP_RAMCODE_START_SEC_CODE
# undef FBL_MEMMAP_ERROR
#endif

#if defined( FBLTP_RAMCODE_STOP_SEC_CODE )
   /* #pragma <enter your compiler specific pragma> */
# undef FBLTP_RAMCODE_STOP_SEC_CODE
# undef FBL_MEMMAP_ERROR
#endif

/* ----------------------------------------------------------------------------------------------------------------- */

#if defined( FBL_APPLVECT_START_SEC_CONST )
   /* #pragma <enter your compiler specific pragma> */
#  pragma ghs startdata
#  pragma ghs section rodata=".APPLVECT"
# undef FBL_APPLVECT_START_SEC_CONST
# undef FBL_MEMMAP_ERROR
#endif

#if defined( FBL_APPLVECT_STOP_SEC_CONST )
   /* #pragma <enter your compiler specific pragma> */
#  pragma ghs section
#  pragma ghs enddata
# undef FBL_APPLVECT_STOP_SEC_CONST
# undef FBL_MEMMAP_ERROR
#endif

/* ----------------------------------------------------------------------------------------------------------------- */

#if defined( FBL_CW_CFG_START_SEC_CONST )
   /* #pragma <enter your compiler specific pragma> */
# undef FBL_CW_CFG_START_SEC_CONST
# undef FBL_MEMMAP_ERROR
#endif

#if defined( FBL_CW_CFG_STOP_SEC_CONST )
   /* #pragma <enter your compiler specific pragma> */
# undef FBL_CW_CFG_STOP_SEC_CONST
# undef FBL_MEMMAP_ERROR
#endif

/* ----------------------------------------------------------------------------------------------------------------- */

#if defined( FBL_CW_FULL_START_SEC_CODE )
   /* #pragma <enter your compiler specific pragma> */
# undef FBL_CW_FULL_START_SEC_CODE
# undef FBL_MEMMAP_ERROR
#endif

#if defined( FBL_CW_FULL_STOP_SEC_CODE )
   /* #pragma <enter your compiler specific pragma> */
# undef FBL_CW_FULL_STOP_SEC_CODE
# undef FBL_MEMMAP_ERROR
#endif

/* ----------------------------------------------------------------------------------------------------------------- */

#if defined( FBL_CW_TASK_START_SEC_CODE )
# pragma ghs startdata
# pragma ghs section text=".RAMCODE"
# undef FBL_CW_TASK_START_SEC_CODE
# undef FBL_MEMMAP_ERROR
#endif

#if defined( FBL_CW_TASK_STOP_SEC_CODE )
# pragma ghs section
# pragma ghs enddata
# undef FBL_CW_TASK_STOP_SEC_CODE
# undef FBL_MEMMAP_ERROR
#endif

/* ----------------------------------------------------------------------------------------------------------------- */

#if defined( FBL_FBT_START_SEC_CONST )
   /* #pragma <enter your compiler specific pragma> */
# undef FBL_FBT_START_SEC_CONST
# undef FBL_MEMMAP_ERROR
#endif

#if defined( FBL_FBT_STOP_SEC_CONST )
   /* #pragma <enter your compiler specific pragma> */
# undef FBL_FBT_STOP_SEC_CONST
# undef FBL_MEMMAP_ERROR
#endif

/* ----------------------------------------------------------------------------------------------------------------- */

#if defined( FBL_HDR_RAMHEADER_START_SEC_DATA )
   /* #pragma <enter your compiler specific pragma> */
# undef FBL_HDR_RAMHEADER_START_SEC_DATA
# undef FBL_MEMMAP_ERROR
#endif

#if defined( FBL_HDR_RAMHEADER_STOP_SEC_DATA )
   /* #pragma <enter your compiler specific pragma> */
# undef FBL_HDR_RAMHEADER_STOP_SEC_DATA
# undef FBL_MEMMAP_ERROR
#endif

/* ----------------------------------------------------------------------------------------------------------------- */

#if defined( FBL_HDR_SBIFLAG_START_SEC_DATA )
   /* #pragma <enter your compiler specific pragma> */
# undef FBL_HDR_SBIFLAG_START_SEC_DATA
# undef FBL_MEMMAP_ERROR
#endif

#if defined( FBL_HDR_SBIFLAG_STOP_SEC_DATA )
   /* #pragma <enter your compiler specific pragma> */
# undef FBL_HDR_SBIFLAG_STOP_SEC_DATA
# undef FBL_MEMMAP_ERROR
#endif

/* ----------------------------------------------------------------------------------------------------------------- */

#if defined( FBL_LBT_START_SEC_CONST )
   /* #pragma <enter your compiler specific pragma> */
# undef FBL_LBT_START_SEC_CONST
# undef FBL_MEMMAP_ERROR
#endif

#if defined( FBL_LBT_STOP_SEC_CONST )
   /* #pragma <enter your compiler specific pragma> */
# undef FBL_LBT_STOP_SEC_CONST
# undef FBL_MEMMAP_ERROR
#endif

/* ----------------------------------------------------------------------------------------------------------------- */

#if defined( FBL_MAIN_MAGICFLAG_START_SEC_VAR )
# pragma ghs startdata
# pragma ghs section bss=".MAGICFLAG"
# undef FBL_MAIN_MAGICFLAG_START_SEC_VAR
# undef FBL_MEMMAP_ERROR
#endif

#if defined( FBL_MAIN_MAGICFLAG_STOP_SEC_VAR )
# pragma ghs section
# pragma ghs enddata
# undef FBL_MAIN_MAGICFLAG_STOP_SEC_VAR
# undef FBL_MEMMAP_ERROR
#endif

/* ----------------------------------------------------------------------------------------------------------------- */

#if defined( FBL_SPI_RENESAS_CSIH_CANCEL_START_SEC_CODE )
   /* #pragma <enter your compiler specific pragma> */
# undef FBL_SPI_RENESAS_CSIH_CANCEL_START_SEC_CODE
# undef FBL_MEMMAP_ERROR
#endif

#if defined( FBL_SPI_RENESAS_CSIH_CANCEL_STOP_SEC_CODE )
   /* #pragma <enter your compiler specific pragma> */
# undef FBL_SPI_RENESAS_CSIH_CANCEL_STOP_SEC_CODE
# undef FBL_MEMMAP_ERROR
#endif

/* ----------------------------------------------------------------------------------------------------------------- */

#if defined( FBL_SPI_RENESAS_CSIH_CHANGE_CONFIGURATION_START_SEC_CODE )
   /* #pragma <enter your compiler specific pragma> */
# undef FBL_SPI_RENESAS_CSIH_CHANGE_CONFIGURATION_START_SEC_CODE
# undef FBL_MEMMAP_ERROR
#endif

#if defined( FBL_SPI_RENESAS_CSIH_CHANGE_CONFIGURATION_STOP_SEC_CODE )
   /* #pragma <enter your compiler specific pragma> */
# undef FBL_SPI_RENESAS_CSIH_CHANGE_CONFIGURATION_STOP_SEC_CODE
# undef FBL_MEMMAP_ERROR
#endif

/* ----------------------------------------------------------------------------------------------------------------- */

#if defined( FBL_SPI_RENESAS_CSIH_CONFIG_PARAM_START_SEC_CONST ) || \
    defined( FBL_SPI_RENESAS_CSIH_CONFIG_PARAM_START_SEC_CONST_EXPORT )
   /* #pragma <enter your compiler specific pragma> */
# undef FBL_SPI_RENESAS_CSIH_CONFIG_PARAM_START_SEC_CONST
# undef FBL_SPI_RENESAS_CSIH_CONFIG_PARAM_START_SEC_CONST_EXPORT
# undef FBL_MEMMAP_ERROR
#endif

#if defined( FBL_SPI_RENESAS_CSIH_CONFIG_PARAM_STOP_SEC_CONST ) || \
    defined( FBL_SPI_RENESAS_CSIH_CONFIG_PARAM_STOP_SEC_CONST_EXPORT )
   /* #pragma <enter your compiler specific pragma> */
# undef FBL_SPI_RENESAS_CSIH_CONFIG_PARAM_STOP_SEC_CONST
# undef FBL_SPI_RENESAS_CSIH_CONFIG_PARAM_STOP_SEC_CONST_EXPORT
# undef FBL_MEMMAP_ERROR
#endif

/* ----------------------------------------------------------------------------------------------------------------- */

#if defined( FBL_SPI_RENESAS_CSIH_DEINIT_START_SEC_CODE )
   /* #pragma <enter your compiler specific pragma> */
# undef FBL_SPI_RENESAS_CSIH_DEINIT_START_SEC_CODE
# undef FBL_MEMMAP_ERROR
#endif

#if defined( FBL_SPI_RENESAS_CSIH_DEINIT_STOP_SEC_CODE )
   /* #pragma <enter your compiler specific pragma> */
# undef FBL_SPI_RENESAS_CSIH_DEINIT_STOP_SEC_CODE
# undef FBL_MEMMAP_ERROR
#endif

/* ----------------------------------------------------------------------------------------------------------------- */

#if defined( FBL_SPI_RENESAS_CSIH_GET_TRANSFER_STATUS_START_SEC_CODE )
   /* #pragma <enter your compiler specific pragma> */
# undef FBL_SPI_RENESAS_CSIH_GET_TRANSFER_STATUS_START_SEC_CODE
# undef FBL_MEMMAP_ERROR
#endif

#if defined( FBL_SPI_RENESAS_CSIH_GET_TRANSFER_STATUS_STOP_SEC_CODE )
   /* #pragma <enter your compiler specific pragma> */
# undef FBL_SPI_RENESAS_CSIH_GET_TRANSFER_STATUS_STOP_SEC_CODE
# undef FBL_MEMMAP_ERROR
#endif

/* ----------------------------------------------------------------------------------------------------------------- */

#if defined( FBL_SPI_RENESAS_CSIH_INIT_INTERNAL_START_SEC_CODE )
   /* #pragma <enter your compiler specific pragma> */
# undef FBL_SPI_RENESAS_CSIH_INIT_INTERNAL_START_SEC_CODE
# undef FBL_MEMMAP_ERROR
#endif

#if defined( FBL_SPI_RENESAS_CSIH_INIT_INTERNAL_STOP_SEC_CODE )
   /* #pragma <enter your compiler specific pragma> */
# undef FBL_SPI_RENESAS_CSIH_INIT_INTERNAL_STOP_SEC_CODE
# undef FBL_MEMMAP_ERROR
#endif

/* ----------------------------------------------------------------------------------------------------------------- */

#if defined( FBL_SPI_RENESAS_CSIH_INIT_START_SEC_CODE )
   /* #pragma <enter your compiler specific pragma> */
# undef FBL_SPI_RENESAS_CSIH_INIT_START_SEC_CODE
# undef FBL_MEMMAP_ERROR
#endif

#if defined( FBL_SPI_RENESAS_CSIH_INIT_STOP_SEC_CODE )
   /* #pragma <enter your compiler specific pragma> */
# undef FBL_SPI_RENESAS_CSIH_INIT_STOP_SEC_CODE
# undef FBL_MEMMAP_ERROR
#endif

/* ----------------------------------------------------------------------------------------------------------------- */

#if defined( FBL_SPI_RENESAS_CSIH_SET_TRANSFER_MODE_START_SEC_CODE )
   /* #pragma <enter your compiler specific pragma> */
# undef FBL_SPI_RENESAS_CSIH_SET_TRANSFER_MODE_START_SEC_CODE
# undef FBL_MEMMAP_ERROR
#endif

#if defined( FBL_SPI_RENESAS_CSIH_SET_TRANSFER_MODE_STOP_SEC_CODE )
   /* #pragma <enter your compiler specific pragma> */
# undef FBL_SPI_RENESAS_CSIH_SET_TRANSFER_MODE_STOP_SEC_CODE
# undef FBL_MEMMAP_ERROR
#endif

/* ----------------------------------------------------------------------------------------------------------------- */

#if defined( FBL_SPI_RENESAS_CSIH_START_SEC_CONST ) || \
    defined( FBL_SPI_RENESAS_CSIH_START_SEC_CONST_EXPORT )
   /* #pragma <enter your compiler specific pragma> */
# undef FBL_SPI_RENESAS_CSIH_START_SEC_CONST
# undef FBL_SPI_RENESAS_CSIH_START_SEC_CONST_EXPORT
# undef FBL_MEMMAP_ERROR
#endif

#if defined( FBL_SPI_RENESAS_CSIH_STOP_SEC_CONST ) || \
    defined( FBL_SPI_RENESAS_CSIH_STOP_SEC_CONST_EXPORT )
   /* #pragma <enter your compiler specific pragma> */
# undef FBL_SPI_RENESAS_CSIH_STOP_SEC_CONST
# undef FBL_SPI_RENESAS_CSIH_STOP_SEC_CONST_EXPORT
# undef FBL_MEMMAP_ERROR
#endif

/* ----------------------------------------------------------------------------------------------------------------- */

#if defined( FBL_SPI_RENESAS_CSIH_TASK_START_SEC_CODE )
   /* #pragma <enter your compiler specific pragma> */
# undef FBL_SPI_RENESAS_CSIH_TASK_START_SEC_CODE
# undef FBL_MEMMAP_ERROR
#endif

#if defined( FBL_SPI_RENESAS_CSIH_TASK_STOP_SEC_CODE )
   /* #pragma <enter your compiler specific pragma> */
# undef FBL_SPI_RENESAS_CSIH_TASK_STOP_SEC_CODE
# undef FBL_MEMMAP_ERROR
#endif

/* ----------------------------------------------------------------------------------------------------------------- */

#if defined( FBL_SPI_RENESAS_CSIH_TRANSFER_ASYNC_START_SEC_CODE )
   /* #pragma <enter your compiler specific pragma> */
# undef FBL_SPI_RENESAS_CSIH_TRANSFER_ASYNC_START_SEC_CODE
# undef FBL_MEMMAP_ERROR
#endif

#if defined( FBL_SPI_RENESAS_CSIH_TRANSFER_ASYNC_STOP_SEC_CODE )
   /* #pragma <enter your compiler specific pragma> */
# undef FBL_SPI_RENESAS_CSIH_TRANSFER_ASYNC_STOP_SEC_CODE
# undef FBL_MEMMAP_ERROR
#endif

/* ----------------------------------------------------------------------------------------------------------------- */

#if defined( FBL_SPI_RENESAS_CSIH_TRANSFER_SYNC_START_SEC_CODE )
   /* #pragma <enter your compiler specific pragma> */
# undef FBL_SPI_RENESAS_CSIH_TRANSFER_SYNC_START_SEC_CODE
# undef FBL_MEMMAP_ERROR
#endif

#if defined( FBL_SPI_RENESAS_CSIH_TRANSFER_SYNC_STOP_SEC_CODE )
   /* #pragma <enter your compiler specific pragma> */
# undef FBL_SPI_RENESAS_CSIH_TRANSFER_SYNC_STOP_SEC_CODE
# undef FBL_MEMMAP_ERROR
#endif

/* ----------------------------------------------------------------------------------------------------------------- */

#if defined( FBL_UPD_AP_RAMCODE_START_SEC_CODE )
   /* #pragma <enter your compiler specific pragma> */
# undef FBL_UPD_AP_RAMCODE_START_SEC_CODE
# undef FBL_MEMMAP_ERROR
#endif

#if defined( FBL_UPD_AP_RAMCODE_STOP_SEC_CODE )
   /* #pragma <enter your compiler specific pragma> */
# undef FBL_UPD_AP_RAMCODE_STOP_SEC_CODE
# undef FBL_MEMMAP_ERROR
#endif

/* ----------------------------------------------------------------------------------------------------------------- */

#if defined( FBL_UPD_MAIN_START_SEC_VAR )
   /* #pragma <enter your compiler specific pragma> */
# undef FBL_UPD_MAIN_START_SEC_VAR
# undef FBL_MEMMAP_ERROR
#endif

#if defined( FBL_UPD_MAIN_STOP_SEC_VAR )
   /* #pragma <enter your compiler specific pragma> */
# undef FBL_UPD_MAIN_STOP_SEC_VAR
# undef FBL_MEMMAP_ERROR
#endif

/* ----------------------------------------------------------------------------------------------------------------- */

#if defined( FBL_UPD_RAMCODE_START_SEC_CODE )
   /* #pragma <enter your compiler specific pragma> */
# undef FBL_UPD_RAMCODE_START_SEC_CODE
# undef FBL_MEMMAP_ERROR
#endif

#if defined( FBL_UPD_RAMCODE_STOP_SEC_CODE )
   /* #pragma <enter your compiler specific pragma> */
# undef FBL_UPD_RAMCODE_STOP_SEC_CODE
# undef FBL_MEMMAP_ERROR
#endif

/* ----------------------------------------------------------------------------------------------------------------- */

#if defined( FBL_UPD_START_SEC_BUFFER_VAR )
   /* #pragma <enter your compiler specific pragma> */
# undef FBL_UPD_START_SEC_BUFFER_VAR
# undef FBL_MEMMAP_ERROR
#endif

#if defined( FBL_UPD_STOP_SEC_BUFFER_VAR )
   /* #pragma <enter your compiler specific pragma> */
# undef FBL_UPD_STOP_SEC_BUFFER_VAR
# undef FBL_MEMMAP_ERROR
#endif

/* ----------------------------------------------------------------------------------------------------------------- */

#if defined( FLASHCODE_START_SEC_VAR ) || \
    defined( FLASHCODE_START_SEC_VAR_EXPORT )
# pragma ghs startdata
# pragma ghs section bss=".FLASHDRV"
# undef FLASHCODE_START_SEC_VAR
# undef FLASHCODE_START_SEC_VAR_EXPORT
# undef FBL_MEMMAP_ERROR
#endif

#if defined( FLASHCODE_STOP_SEC_VAR ) || \
    defined( FLASHCODE_STOP_SEC_VAR_EXPORT )
# pragma ghs section
# pragma ghs enddata
# undef FLASHCODE_STOP_SEC_VAR
# undef FLASHCODE_STOP_SEC_VAR_EXPORT
# undef FBL_MEMMAP_ERROR
#endif

/* ----------------------------------------------------------------------------------------------------------------- */

#if defined( INITWATCHDOG_START_SEC_CODE ) || \
    defined( INITWATCHDOG_START_SEC_CODE_EXPORT )
   /* #pragma <enter your compiler specific pragma> */
# undef INITWATCHDOG_START_SEC_CODE
# undef INITWATCHDOG_START_SEC_CODE_EXPORT
# undef FBL_MEMMAP_ERROR
#endif

#if defined( INITWATCHDOG_STOP_SEC_CODE ) || \
    defined( INITWATCHDOG_STOP_SEC_CODE_EXPORT )
   /* #pragma <enter your compiler specific pragma> */
# undef INITWATCHDOG_STOP_SEC_CODE
# undef INITWATCHDOG_STOP_SEC_CODE_EXPORT
# undef FBL_MEMMAP_ERROR
#endif

/* ----------------------------------------------------------------------------------------------------------------- */

#if defined( LOOKFORWATCHDOG_START_SEC_CODE ) || \
    defined( LOOKFORWATCHDOG_START_SEC_CODE_EXPORT )
# pragma ghs startdata
# pragma ghs section text=".RAMCODE"
# undef LOOKFORWATCHDOG_START_SEC_CODE
# undef LOOKFORWATCHDOG_START_SEC_CODE_EXPORT
# undef FBL_MEMMAP_ERROR
#endif

#if defined( LOOKFORWATCHDOG_STOP_SEC_CODE ) || \
    defined( LOOKFORWATCHDOG_STOP_SEC_CODE_EXPORT )
   /* #pragma <enter your compiler specific pragma> */
# pragma ghs section
# pragma ghs enddata
# undef LOOKFORWATCHDOG_STOP_SEC_CODE
# undef LOOKFORWATCHDOG_STOP_SEC_CODE_EXPORT
# undef FBL_MEMMAP_ERROR
#endif

/* ----------------------------------------------------------------------------------------------------------------- */

#if defined( MEMCPY_START_SEC_CODE )
   /* #pragma <enter your compiler specific pragma> */
# undef MEMCPY_START_SEC_CODE
# undef FBL_MEMMAP_ERROR
#endif

#if defined( MEMCPY_STOP_SEC_CODE )
   /* #pragma <enter your compiler specific pragma> */
# undef MEMCPY_STOP_SEC_CODE
# undef FBL_MEMMAP_ERROR
#endif

/* ----------------------------------------------------------------------------------------------------------------- */

#if defined( SECMPAR_START_SEC_DATA )
   /* #pragma <enter your compiler specific pragma> */
# undef SECMPAR_START_SEC_DATA
# undef FBL_MEMMAP_ERROR
#endif

#if defined( SECMPAR_STOP_SEC_DATA )
   /* #pragma <enter your compiler specific pragma> */
# undef SECMPAR_STOP_SEC_DATA
# undef FBL_MEMMAP_ERROR
#endif

/* ----------------------------------------------------------------------------------------------------------------- */

#if defined( SECMWS_START_SEC_DATA ) || \
    defined( SECMWS_START_SEC_DATA_EXPORT )
   /* #pragma <enter your compiler specific pragma> */
# undef SECMWS_START_SEC_DATA
# undef SECMWS_START_SEC_DATA_EXPORT
# undef FBL_MEMMAP_ERROR
#endif

#if defined( SECMWS_STOP_SEC_DATA ) || \
    defined( SECMWS_STOP_SEC_DATA_EXPORT )
   /* #pragma <enter your compiler specific pragma> */
# undef SECMWS_STOP_SEC_DATA
# undef SECMWS_STOP_SEC_DATA_EXPORT
# undef FBL_MEMMAP_ERROR
#endif

/* ----------------------------------------------------------------------------------------------------------------- */

#if defined( WDTRIGGER_START_SEC_CODE ) || \
    defined( WDTRIGGER_START_SEC_CODE_EXPORT )
# pragma ghs startdata
# pragma ghs section text=".RAMCODE"
# undef WDTRIGGER_START_SEC_CODE
# undef WDTRIGGER_START_SEC_CODE_EXPORT
# undef FBL_MEMMAP_ERROR
#endif

#if defined( WDTRIGGER_STOP_SEC_CODE ) || \
    defined( WDTRIGGER_STOP_SEC_CODE_EXPORT )
# pragma ghs section
# pragma ghs enddata
# undef WDTRIGGER_STOP_SEC_CODE
# undef WDTRIGGER_STOP_SEC_CODE_EXPORT
# undef FBL_MEMMAP_ERROR
#endif

/* ----------------------------------------------------------------------------------------------------------------- */

#if defined( WRAPNV_START_SEC_CODE )
   /* #pragma <enter your compiler specific pragma> */
# undef WRAPNV_START_SEC_CODE
# undef FBL_MEMMAP_ERROR
#endif

#if defined( WRAPNV_STOP_SEC_CODE )
   /* #pragma <enter your compiler specific pragma> */
# undef WRAPNV_STOP_SEC_CODE
# undef FBL_MEMMAP_ERROR
#endif

/* ----------------------------------------------------------------------------------------------------------------- */

#if defined( WRAPNV_START_SEC_CONST_UNSPECIFIED )
   /* #pragma <enter your compiler specific pragma> */
# undef WRAPNV_START_SEC_CONST_UNSPECIFIED
# undef FBL_MEMMAP_ERROR
#endif

#if defined( WRAPNV_STOP_SEC_CONST_UNSPECIFIED )
   /* #pragma <enter your compiler specific pragma> */
# undef WRAPNV_STOP_SEC_CONST_UNSPECIFIED
# undef FBL_MEMMAP_ERROR
#endif

/* ----------------------------------------------------------------------------------------------------------------- */

#if defined( WRAPNV_START_SEC_VAR_NOINIT_UNSPECIFIED )
   /* #pragma <enter your compiler specific pragma> */
# undef WRAPNV_START_SEC_VAR_NOINIT_UNSPECIFIED
# undef FBL_MEMMAP_ERROR
#endif

#if defined( WRAPNV_STOP_SEC_VAR_NOINIT_UNSPECIFIED )
   /* #pragma <enter your compiler specific pragma> */
# undef WRAPNV_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# undef FBL_MEMMAP_ERROR
#endif

/* ----------------------------------------------------------------------------------------------------------------- */

#if defined( FBL_MEMMAP_ERROR )
# error "Error in MemMap.h: MemMap section is not included!"
#endif

/* Module specific MISRA deviations:

   MD_MemMap_0883:
      Reason:     This file has to be included multiple times explicitly, due to the memory mapping mechanism.
      Risk:       No risk due to the undef pragmas and error pragma.
      Prevention: No prevention.

*/

/**********************************************************************************************************************
 *  END OF FILE: MEMMAP.H
 *********************************************************************************************************************/
