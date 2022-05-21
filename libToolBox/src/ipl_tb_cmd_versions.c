
/*
*@@@bF
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
X             Copyright (c) 2000 2009 SAGEM SA.
X                                                                           X
X  SAGEM S.A. confidential - Disclosure under NDA only                      X
X  Copyright (c) SAGEM S.A. 2003-2008. All Rights Reserved                  X
X  The copyright to the computer program(s) herein is the property of       X
X  SAGEM S.A., France. The program(s) may be used and/or copied only with   X
X  the written permission of SAGEM S.A. or in accordance with the terms and X 
X  condition stipulated in the agreement/contract under which the program(s)X
X  have been supplied. This copyright notice must not be removed            X
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
X File          : ipl_tb_cmd_versions.c
X Author        : Anthony CAMMAS
X Creation date : 08/03/2009
X Product       : ST                                                        
X Language      : C                               
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
*@@@eF
*/

/* ########################################################################## */
/* ### #include                                                               */

#include "ipl_tb_internal.h"
#include "ipl_tb.h"
#include "ipl_tb_l.h"


/* ########################################################################## */
/* ### Variables                                                              */

/* ########################################################################## */
/* ### Functions                                                              */
static int32 f_IPL_TB_CmdVersionsPrint(void);

int32 m_IPL_TB_CmdVersions
(
   int32 wz_mbfield,
   char *pb_arg
)
{

   //if (pb_arg == NULL)
   {
      return f_IPL_TB_CmdVersionsPrint();
   }
   // TODO : implement setting versions
   /*else
   {
      return f_IPL_TB_CmdVersionsSet(pb_arg);
   }*/
} 

/*
*@@@bf
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
X                     Copyright (c) 2000 2009 SAGEM SA.
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
X Function      : f_IPL_TB_CmdVersionsPrint()
X Author        : Anthony CAMMAS
X Creation date : 08/03/2009
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
*@@@ef
*/
static int32 f_IPL_TB_CmdVersionsPrint
(
   void
)
{
#ifndef CONFIG_INTLDR_NO_MODULE_ID
   uint8                            tbz_id[INTLDR_MAX_MODULE_NAME];
#endif
   uint32                           wz_version;

   IPL_PRT_DEBUG_PRINTF("  BSL 1\r\n");
#ifndef CONFIG_INTLDR_NO_MODULE_ID
   if(!INTLDR_GetModuleName(INTLDR_MODULE_TYPE_LOADER,(char* const)tbz_id, INTLDR_MAX_MODULE_NAME))
   {
      IPL_PRT_DEBUG_PRINTF("    loader_id_string                     = %s\r\n",
            tbz_id);
   }
   else
   {
      IPL_PRT_DEBUG_PRINTF("    invalid loader_id_string !\r\n");
   }
#endif
   if(! INTLDR_GetModuleVersion(INTLDR_MODULE_TYPE_LOADER, &wz_version))
   {
      IPL_PRT_DEBUG_PRINTF("    loader_software_module_version       = %d\r\n",
         wz_version);
   }
   else
   {
      IPL_PRT_DEBUG_PRINTF("    invalid loader_software_module_version !\r\n");
   }


   IPL_PRT_DEBUG_PRINTF("  FIRMWARE\r\n");
#ifndef CONFIG_INTLDR_NO_MODULE_ID
   if(!INTLDR_GetModuleName(INTLDR_MODULE_TYPE_KERNEL,(char* const)tbz_id, INTLDR_MAX_MODULE_NAME))
   {
      IPL_PRT_DEBUG_PRINTF("    firmware_id_string                    = %s\r\n", tbz_id);
   }
   else
   {
      IPL_PRT_DEBUG_PRINTF("    invalid firmware_id_string !\r\n");
   }
#endif
   if(! INTLDR_GetModuleVersion(INTLDR_MODULE_TYPE_KERNEL, &wz_version))
   {
      IPL_PRT_DEBUG_PRINTF("    firmware_software_module_version       = %d\r\n",
         wz_version);
   }
   else
   {
      IPL_PRT_DEBUG_PRINTF("    invalid firmware_software_module_version !\r\n");
   }
   
   
#if CONFIG_IPL_UPD_KERNEL_2_DOWNLOADABLE

#ifndef CONFIG_INTLDR_NO_MODULE_ID
   if(!INTLDR_GetModuleName(INTLDR_MODULE_TYPE_KERNEL2,(char* const)tbz_id, INTLDR_MAX_MODULE_NAME))
   {
      IPL_PRT_DEBUG_PRINTF("    firmware_2_id_string                    = %s\r\n", tbz_id);
   }
   else
   {
      IPL_PRT_DEBUG_PRINTF("    invalid firmware_2_id_string !\r\n");
   }
#endif
   if(! INTLDR_GetModuleVersion(INTLDR_MODULE_TYPE_KERNEL2, &wz_version))
   {
      IPL_PRT_DEBUG_PRINTF("    firmware_2_software_module_version       = %d\r\n",
         wz_version);
   }
   else
   {
      IPL_PRT_DEBUG_PRINTF("    invalid firmware_2_software_module_version !\r\n");
   }
#endif // CONFIG_IPL_UPD_KERNEL_2_DOWNLOADABLE

#if CONFIG_IPL_UPD_RA_DOWNLOADABLE
   IPL_PRT_DEBUG_PRINTF("  RESIDENT APPLICATION\r\n");
#ifndef CONFIG_INTLDR_NO_MODULE_ID
   if(!INTLDR_GetModuleName(INTLDR_MODULE_TYPE_RA,(char* const)tbz_id, INTLDR_MAX_MODULE_NAME))
   {
      IPL_PRT_DEBUG_PRINTF("    ra_id_string                    = %s\r\n",tbz_id);
   }
   else
   {
      IPL_PRT_DEBUG_PRINTF("    invalid firmware_id_string !\r\n");
   }
#endif
   if(! INTLDR_GetModuleVersion(INTLDR_MODULE_TYPE_RA, &wz_version))
   {
      IPL_PRT_DEBUG_PRINTF("    ra_software_module_version       = %d\r\n",
         wz_version);
   }
   else
   {
      IPL_PRT_DEBUG_PRINTF("    invalid ra_software_module_version !\r\n");
   }
#endif
#if CONFIG_IPL_UPD_ROOT_FS_DOWNLOADABLE
   IPL_PRT_DEBUG_PRINTF("  ROOT_FS\r\n");
   INTLDR_GetModuleName(INTLDR_MODULE_TYPE_ROOT_FS,(char* const)tbz_id, INTLDR_MAX_MODULE_NAME);
   IPL_PRT_DEBUG_PRINTF("    root_fs_id_string                    = %s\r\n",tbz_id);
   if(! INTLDR_GetModuleVersion(INTLDR_MODULE_TYPE_RA, &wz_version))
   {
      IPL_PRT_DEBUG_PRINTF("    root_fs_module_version       = %d\r\n",
         wz_version);
   }
   else
   {
      IPL_PRT_DEBUG_PRINTF("    invalid root_fs_module_version !\r\n");
   }
#endif

   return MCM_OK;

} /* End of function 'f_IPL_TB_CmdVersionsPrint()' */

