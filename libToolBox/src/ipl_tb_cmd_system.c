
/** @brief no comment */
#define IPL_TB_CMD_SYSTEM_C

/*-----------------------------------------------------------------------------
 FILE DOCUMENTATION
-----------------------------------------------------------------------------*/
/**
 * @note    Copyright 2000-2011 SagemCom SAS All rights reserved
 *
 * @file    ipl_tb_cmd_system.c
 * @brief   m_IPL_TB_CmdSystem implementation
 * @version Revision of last commit : $Rev: 100 $
 * @date    Date of last commit     : $Date: 2012-03-15$
 * @author  Author of last commit   : $Author: g601627 $
 * @addtogroup IPL_TB
 * @{
 */

/*-----------------------------------------------------------------------------
 SYSTEM                          INCLUSIONS in functional/alphabetical order
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 FUNCTIONAL                      INCLUSIONS in functional/alphabetical order
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 CURRENT COMPONENT               INCLUSIONS in functional/alphabetical order
-----------------------------------------------------------------------------*/                                                             
#include "ipl_tb_l.h"
#include "ipl_tb_internal.h"



/**
 * @brief   execute system call from input string
 */

int32 m_IPL_TB_CmdSystem
(
   int32 wz_mbfield,
   char *pb_argv
)
{
   int32        		Cr=MCM_ERROR;

   if (pb_argv == NULL)
   {
      return -1;
   }
   else
   {
     IPL_PRT_DEBUG_PRINTF("Command is:%s",pb_argv);
     if(0!= INTLDR_SystemCmd(pb_argv))
     {
        IPL_PRT_DEBUG_PRINTF("\nsystem command failed\n");
     }
     Cr = MCM_OK;
   }
   return Cr;

} /* End of function 'm_IPL_TB_CmdRead()' */
/** @} */

