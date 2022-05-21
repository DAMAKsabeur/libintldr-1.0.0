
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
X File          : ipl_tb_cmd_set_path.c
X Author        : Anthony CAMMAS
X Creation date : 13/02/2009
X Product       : ST                                                        
X Language      : C                               
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
*@@@eF
*/

/* ########################################################################## */
/* ### #include                                                               */
#include "ipl_tb_internal.h"
#define IPL_TB_USB_PATH "/mnt"

/* ########################################################################## */
/* ### Variables                                                              */

/* ########################################################################## */
/* ### Functions                                                              */

/*
*@@@bf
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
X                     Copyright (c) 2000 2009 SAGEM SA.
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
X Function      : m_IPL_TB_CmdCorrupt()
X Author        : Anthony CAMMAS
X Creation date : 13/02/2009
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
*@@@ef
*/
int32 m_IPL_TB_CmdSetPath
(
   int32 data,
   char *pb_argv
)
{
   char            usb_path[256];
   int32           Cr             = MCM_OK;
   IPL_TB_M_Ctx_t  *pCtx          = &IPL_TB_M_Ctx;
   if (pCtx->custom_path)
   {
      strcpy(usb_path,pCtx->usb_path);
   }
   else
   {
      strcpy(usb_path,IPL_TB_USB_PATH);
   }
   IPL_PRT_DEBUG_PRINTF("Former path: %s \r\n",usb_path);

   if (pb_argv == NULL)
   {
      IPL_PRT_DEBUG_PRINTF("Please specify a path.\n");
      return MCM_OK;
   }
   if (strcmp(pb_argv, "default") == 0)
   {
      pCtx->custom_path = 0;
   }
   else
   {
      pCtx->custom_path = 1;
      if (strcasecmp(pb_argv,"/")!=0)
      strcat(pb_argv,"/");
      strncpy(pCtx->usb_path,pb_argv,sizeof(pCtx->usb_path)-1);
      IPL_PRT_DEBUG_PRINTF("New Path : %s \r\n",pCtx->usb_path);
   }

   return Cr;

} /* End of function 'm_IPL_TB_CmdCorrupt()' */
/** @} */

