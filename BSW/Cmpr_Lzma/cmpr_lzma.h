/***********************************************************************************************************************
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/** \file
 *  \brief       Lzma Decompression Wrapper for implementation of emCompression component
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
 *  Thomas Mueller                ThM           Vector Informatik GmbH
 *  --------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version    Date        Author  Change Id        Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  01.00.00   2016-11-05  ThM     -                Creation
 *  01.00.01   2016-11-17  ThM     ESCAN00092841    Added compression mode switch
 **********************************************************************************************************************/

#ifndef __CMPR_LZMA_H__
#define __CMPR_LZMA_H__

/***********************************************************************************************************************
 *  VERSION
 **********************************************************************************************************************/

/* ##V_CFG_MANAGEMENT ##CQProject : SysService_CmprLzma CQComponent : Impl_Wrapper */
#define SYSSERVICE_CMPRLZMA_VERSION           0x0100u
#define SYSSERVICE_CMPRLZMA_RELEASE_VERSION   0x01u

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

#define COMPRESS_LZMA_EOS_INVALID 0x00
#define COMPRESS_LZMA_EOS_VALID   0x01

# if defined( FBL_ENABLE_COMPRESSION_MODE )

/***********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

tFblResult CmprLzmaInit( void );
tFblResult CmprLzmaDeinit( V_MEMRAM1 tProcParam V_MEMRAM2 V_MEMRAM3 * procParam );
tFblResult CmprLzmaDecompress( V_MEMRAM1 tProcParam V_MEMRAM2 V_MEMRAM3 * procParam );

# endif /* FBL_ENABLE_COMPRESSION_MODE */
#endif /* __CMPR_LZMA_H__ */

/***********************************************************************************************************************
 *  END OF FILE: CMPR_LZMA.H
 **********************************************************************************************************************/
