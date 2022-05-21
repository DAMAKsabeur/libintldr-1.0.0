
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
X File          : ipl_tb_cmd_quit.c
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
#include "ipl_tb_l.h"

/* ########################################################################## */
/* ### Variables                                                              */

/* ########################################################################## */
/* ### Functions                                                              */

/*
*@@@bf
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
X                     Copyright (c) 2000 2009 SAGEM SA.
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
X Function      : m_IPL_TB_CmdQuit()
X Author        : Anthony CAMMAS
X Creation date : 08/03/2009
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
*@@@ef
*/
int32 m_IPL_TB_CmdQuit
(
   int32 wz_mbfield,
   char *pb_arg
)
{
   char tcz_display[IPL_TB_DISPLAY_SIZE];

   strncpy(tcz_display, IPL_TB_BOOT_DISPLAY, IPL_TB_DISPLAY_SIZE-1);

   /* Not implemented
	  f_FPANEL_DISPLAY_Write( tcz_display, 1, IPL_TB_DISPLAY_SIZE,
                           DISPLAY_CLEAR,DISPLAY_DIMMING_HIGH); 
	*/

   bm_IPL_TB_quit_flag = MCM_TRUE;

   return MCM_OK;

} /* End of function 'm_IPL_TB_CmdQuit()' */
/** @} */

