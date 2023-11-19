/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2012 - 2016 by Vector Informatik GmbH.                                           All rights reserved.
 *
 *                 This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                 Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                 All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  LICENSE
 *  -------------------------------------------------------------------------------------------------------------------
 *            Module: Fee
 *           Program: GM Global B / CAN/LIN/IP (MSR_Gm_SLP1)
 *          Customer: Nexteer Automotive Corporation
 *       Expiry Date: Not restricted
 *  Ordered Derivat.: Renesas RH850 P1M
 *    License Scope : The usage is restricted to CBD1500636_D00
 *
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *              File: Fee_Lcfg.c
 *   Generation Time: 2016-05-21 12:46:53
 *           Project: EIP_GlobalB - Version 1.0
 *          Delivery: CBD1500636_D00
 *      Tool Version: DaVinci Configurator (beta) 5.10.26 SP3
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


/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Std_Types.h"
#include "Fee.h"
#include "Fee_PrivateCfg.h"
#include "Fee_Int.h"
#include "Fee_Partition.h"

/**********************************************************************************************************************
 * VERSION IDENTIFICATION
 *********************************************************************************************************************/
# define FEE_CFG_C_MAJOR_VERSION                                        (8u)
# define FEE_CFG_C_MINOR_VERSION                                        (0u)
# define FEE_CFG_C_PATCH_VERSION                                        (0u)

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/
#if (  (FEE_SW_MAJOR_VERSION != (8u)) \
    || (FEE_SW_MINOR_VERSION != (0u)) )
# error "Vendor specific version numbers of Fee_Lcfg.c and Fee.h are inconsistent"
#endif

#if (  (FEE_CFG_MAJOR_VERSION != (8u)) \
    || (FEE_CFG_MINOR_VERSION != (0u)) )
# error "Version numbers of Fee_Lcfg.c and Fee_Cfg.h are inconsistent!"
#endif

#if (  (FEE_PRIVATE_CFG_MAJOR_VERSION != (8u)) \
    || (FEE_PRIVATE_CFG_MINOR_VERSION != (0u)) )
# error "Version numbers of Fee_Lcfg.c and Fee_PrivateCfg.h are inconsistent!"
#endif

#define FEE_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"      /* PRQA S 5087 */ /* MD_MSR_19.1 */

static VAR(struct Fee_PartitionsRamDataStruct, FEE_VAR_NOINIT) Fee_PartitionRamData[FEE_NUMBER_OF_PARTITIONS];

#define FEE_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"      /* PRQA S 5087 */ /* MD_MSR_19.1 */


/**********************************************************************************************************************
 *  GLOBAL DATA CONSTANT
 *********************************************************************************************************************/
#define FEE_START_SEC_APPL_CONFIG_UNSPECIFIED
#include "MemMap.h"     /* PRQA S 5087 */ /* MD_MSR_19.1 */

  /*<FEE_BLOCK_CONFIGURATION>*/
static CONST(struct Fee_BlockConfigStruct, FEE_APPL_CONFIG) Fee_BlockConfig_at[] =
{
  { /*  Block: FeeRte_NvmBlock_CmnMfgSrv_NxtrMfgEnaCntr  */ 
    6U /*  index of the block in the linktable  */ , 
    1U /*  payload length  */ , 
    1U /*  number of datasets  */ , 
    FEE0 /*  partition where the block resides in  */ , 
    1U /*  the exponent of the number of instances per chunk (2^n)-1  */ , 
    FALSE /*  immediate data  */ , 
    FALSE /*  critical data  */ 
  }, 
  { /*  Block: FeeRte_NvmBlock_PartNr_PartNrBasMdlPartNr  */ 
    23U /*  index of the block in the linktable  */ , 
    8U /*  payload length  */ , 
    1U /*  number of datasets  */ , 
    FEE0 /*  partition where the block resides in  */ , 
    1U /*  the exponent of the number of instances per chunk (2^n)-1  */ , 
    FALSE /*  immediate data  */ , 
    FALSE /*  critical data  */ 
  }, 
  { /*  Block: FeeRte_NvmBlock_PartNr_PartNrEndMdlPartNr  */ 
    22U /*  index of the block in the linktable  */ , 
    8U /*  payload length  */ , 
    1U /*  number of datasets  */ , 
    FEE0 /*  partition where the block resides in  */ , 
    1U /*  the exponent of the number of instances per chunk (2^n)-1  */ , 
    FALSE /*  immediate data  */ , 
    FALSE /*  critical data  */ 
  }, 
  { /*  Block: FeeEPS_DIAG_MON_NvmSbat  */ 
    44U /*  index of the block in the linktable  */ , 
    822U /*  payload length  */ , 
    1U /*  number of datasets  */ , 
    FEE0 /*  partition where the block resides in  */ , 
    1U /*  the exponent of the number of instances per chunk (2^n)-1  */ , 
    FALSE /*  immediate data  */ , 
    FALSE /*  critical data  */ 
  }, 
  { /*  Block: FeeEPS_DIAG_MON_NvmEcuId  */ 
    43U /*  index of the block in the linktable  */ , 
    16U /*  payload length  */ , 
    1U /*  number of datasets  */ , 
    FEE0 /*  partition where the block resides in  */ , 
    1U /*  the exponent of the number of instances per chunk (2^n)-1  */ , 
    FALSE /*  immediate data  */ , 
    FALSE /*  critical data  */ 
  }, 
  { /*  Block: FeeRte_NvmBlock_AppNbId  */ 
    42U /*  index of the block in the linktable  */ , 
    2U /*  payload length  */ , 
    1U /*  number of datasets  */ , 
    FEE0 /*  partition where the block resides in  */ , 
    1U /*  the exponent of the number of instances per chunk (2^n)-1  */ , 
    FALSE /*  immediate data  */ , 
    FALSE /*  critical data  */ 
  }, 
  { /*  Block: FeeEPS_DIAG_MON_NvmKeyNbid  */ 
    41U /*  index of the block in the linktable  */ , 
    2U /*  payload length  */ , 
    1U /*  number of datasets  */ , 
    FEE0 /*  partition where the block resides in  */ , 
    1U /*  the exponent of the number of instances per chunk (2^n)-1  */ , 
    FALSE /*  immediate data  */ , 
    FALSE /*  critical data  */ 
  }, 
  { /*  Block: FeeConfigBlock  */ 
    0U /*  index of the block in the linktable  */ , 
    4U /*  payload length  */ , 
    2U /*  number of datasets  */ , 
    FEE0 /*  partition where the block resides in  */ , 
    1U /*  the exponent of the number of instances per chunk (2^n)-1  */ , 
    FALSE /*  immediate data  */ , 
    FALSE /*  critical data  */ 
  }
};
/*</FEE_BLOCK_CONFIGURATION>*/


/* Describe the partitions */
static CONST(struct Fee_PartitionConfigStruct, FEE_APPL_CONFIG) Fee_PartitionConfig_at[FEE_NUMBER_OF_PARTITIONS] =
{
  {
    &Fee_PartitionRamData[0].partition_t, 
    {
      0UL /*  FSS threshold  */ , 
      0UL /*  BSS threshold  */ 
    }, 
    { /*  AssociatedSectors_at  */ 
      { /*  LowerSector  */ 
        &Fee_PartitionRamData[0].sector_at[0], 
        0xFFFFFFFFUL /*  Fls Erase Value  */ , 
        { /*  physical sector  */ 
          0x00000000UL /*  physical sector start address  */ , 
          0x00002000UL /*  physical sector size  */ 
        }, 
        { /*  configured sector  */ 
          0x00000000UL /*  configured sector start address  */ , 
          0x00002000UL /*  configured sector size  */ 
        }, 
        { /*  alignments  */ 
          3U /*  write alignment  */ , 
          3U /*  address alignment  */ 
        }
      }, 
      { /*  UpperSector  */ 
        &Fee_PartitionRamData[0].sector_at[1], 
        0xFFFFFFFFUL /*  Fls Erase Value  */ , 
        { /*  physical sector  */ 
          0x00002000UL /*  physical sector start address  */ , 
          0x00002000UL /*  physical sector size  */ 
        }, 
        { /*  configured sector  */ 
          0x00002000UL /*  configured sector start address  */ , 
          0x00002000UL /*  configured sector size  */ 
        }, 
        { /*  alignments  */ 
          3U /*  write alignment  */ , 
          3U /*  address alignment  */ 
        }
      }
    }, 
    (45U), 
    0U
  } /*  ramData_pt  */ , 
  {
    &Fee_PartitionRamData[1].partition_t, 
    {
      0UL /*  FSS threshold  */ , 
      0UL /*  BSS threshold  */ 
    }, 
    { /*  AssociatedSectors_at  */ 
      { /*  LowerSector  */ 
        &Fee_PartitionRamData[1].sector_at[0], 
        0xFFFFFFFFUL /*  Fls Erase Value  */ , 
        { /*  physical sector  */ 
          0x00004000UL /*  physical sector start address  */ , 
          0x00002000UL /*  physical sector size  */ 
        }, 
        { /*  configured sector  */ 
          0x00004000UL /*  configured sector start address  */ , 
          0x00002000UL /*  configured sector size  */ 
        }, 
        { /*  alignments  */ 
          3U /*  write alignment  */ , 
          3U /*  address alignment  */ 
        }
      }, 
      { /*  UpperSector  */ 
        &Fee_PartitionRamData[1].sector_at[1], 
        0xFFFFFFFFUL /*  Fls Erase Value  */ , 
        { /*  physical sector  */ 
          0x00006000UL /*  physical sector start address  */ , 
          0x00002000UL /*  physical sector size  */ 
        }, 
        { /*  configured sector  */ 
          0x00006000UL /*  configured sector start address  */ , 
          0x00002000UL /*  configured sector size  */ 
        }, 
        { /*  alignments  */ 
          3U /*  write alignment  */ , 
          3U /*  address alignment  */ 
        }
      }
    }, 
    (1U), 
    1U
  } /*  ramData_pt  */ 
};

CONST(struct Fee_ConfigStruct, FEE_APPL_CONFIG) Fee_Config =
{
    Fee_BlockConfig_at,
    Fee_PartitionConfig_at,
    (0x0008U /*  number of blocks  */ ),
    FEE_NUMBER_OF_PARTITIONS,
    (1U /*  dataset selection bits  */ ),
    (TRUE /*  fbl config  */ )
};

#define FEE_STOP_SEC_APPL_CONFIG_UNSPECIFIED
#include "MemMap.h"  /* PRQA S 5087 */ /* MD_MSR_19.1 */

#define FEE_START_SEC_CODE
#include "MemMap.h"      /* PRQA S 5087 */ /* MD_MSR_19.1 */

FUNC(Fee_ErrorCallbackReturnType, FEE_APPL_CODE) Fee_ErrorCallbackNotification(uint8 partitionId, Fee_SectorErrorType err)
{
#if (STD_ON == FEE_USE_APPL_ERROR_CALLBACK)
     return  Appl_FeeCriticalErrorCallback(partitionId, err);
#else
     /* check partitionId -> avoid waning about unused parameter */
     return (Fee_ErrorCallbackReturnType)(
                        ((partitionId < FEE_NUMBER_OF_PARTITIONS) && (err != FEE_SECTOR_FORMAT_FAILED)) ?
                                                       FEE_ERRCBK_RESOLVE_AUTOMATICALLY : FEE_ERRCBK_REJECT_WRITE);
#endif
}

#define FEE_STOP_SEC_CODE
#include "MemMap.h"      /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  END OF FILE: Fee_Lcfg.c
 *********************************************************************************************************************/
 

