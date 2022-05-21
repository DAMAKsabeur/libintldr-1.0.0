
/** @brief no comment */
#define IPL_TB_DEBUG_PRINTF_C
/*-----------------------------------------------------------------------------
 FILE DOCUMENTATION
-----------------------------------------------------------------------------*/
/**
 * @note    Copyright 2000-2011 SagemCom SAS All rights reserved
 *
 * @file    ipl_prt_debug_printf.c
 * @brief   IPL_PRT_DEBUG_Printf() implementation
 * @version Revision of last commit : $Rev: 100 $
 * @date    Date of last commit     : $Date: 2011-08-04 10:45:30 +0100 (Thu, 4 Aug 2011) $
 * @author  Author of last commit   : $Author: g601627 $
 * @addtogroup IPL_PRT
 * @{
 */

/*-----------------------------------------------------------------------------
 SYSTEM                          INCLUSIONS in functional/alphabetical order
-----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
 CURRENT COMPONENT               INCLUSIONS in functional/alphabetical order
-----------------------------------------------------------------------------*/
#include "ipl_tb_internal.h"

#define IPL_TB_M_BUFSIZE 512

/**
 * @brief   Prints debug traces on the serial line.
 */
void IPL_PRT_DEBUG_Printf(char *i_pFormat, ...)
{
   va_list           ArgList;
   char Buffer[IPL_TB_M_BUFSIZE];
   int      h = -1;
   h    = open  (CONFIG_PRT_DEBUG_OUTPUT_FILE, O_RDWR);
   
   if (h!=-1)
   {
      /* Format the message for DEBUG */
      va_start(ArgList, i_pFormat);
      vsnprintf(Buffer, IPL_TB_M_BUFSIZE, i_pFormat, ArgList);
      va_end(ArgList);
      Buffer[IPL_TB_M_BUFSIZE - 1] = '\0';
   
   if (-1 == write (h, Buffer, strlen(Buffer))){
      printf ("\n[IPL_PRT_DEBUG_Printf] serial write failed \r\n");
   }
      close (h);
   }
}

/** @} */
