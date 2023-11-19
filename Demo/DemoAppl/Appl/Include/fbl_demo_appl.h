/*********************************************************************
 * This file needs to be included in fbl_cfg.h for Demo Application.
 ********************************************************************/

/* This removes application start */
#define FBL_NO_JSR_APPL

/* This disables the FblHeader-address check */
#if defined( FBL_ENABLE_ASSERTION )
# undef FBL_ENABLE_ASSERTION
# define FBL_DISABLE_ASSERTION
#endif

/* This overwrites configuration to always enable
 * - permanent Diagnostic Identifiers, which are mandatory for any application */
#if defined( FBL_DISABLE_CAN_CONFIGURATION )
# undef FBL_DISABLE_CAN_CONFIGURATION
# define FBL_ENABLE_CAN_CONFIGURATION
#endif

/* Stub functionality which is not available */
//#define ApplFblStartApplication()

/* This cannot be checked because of absent applvect. Is not required in Appl-context anyway.. */
#if defined(FblCheckBootVectTableIsValid)
# undef FblCheckBootVectTableIsValid
# define FblCheckBootVectTableIsValid() 0
#endif

/* No need for compression in DemoAppl */
#if defined (FBL_ENABLE_COMPRESSION_MODE)
# undef FBL_ENABLE_COMPRESSION_MODE
# define FBL_DISABLE_COMPRESSION_MODE
#endif

/* Do not write to Fee */
#undef   FBL_TEST_ECU_ID
#undef   FBL_TEST_KEY_NBID
#undef   FBL_TEST_APP_NBID

#include "fbl_jmp_to_fbl.h"


#if defined( USE_LEGACY_FBL_APPL_INTERFACE )
/* Set FBL_HEADER_ADDRESS to location of legacy header, like it would be in unchanged application. */
#undef FBL_HEADER_ADDRESS
#define FBL_HEADER_ADDRESS                   0x0300u
#else
#endif
