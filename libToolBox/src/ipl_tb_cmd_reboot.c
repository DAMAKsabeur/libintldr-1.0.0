
/** @brief no comment */
#define TOOLBOX_C
/*-----------------------------------------------------------------------------
 FILE DOCUMENTATION
-----------------------------------------------------------------------------*/
/**
 * @note    Copyright 2000-2011 SagemCom SAS All rights reserved
 *
 * @file    ipl_tb_cmd_reboot.c
 * @version Revision of last commit : $Rev: 100 $
 * @date    Date of last commit     : $Date: 2012-09-17 10:45:30 +0100 (Thu, 13 Jan 2011) $
 * @author  Author of last commit   : $Author: g601627 $
 * @addtogroup toolbox
 * @{
 */

/*-----------------------------------------------------------------------------
 CURRENT COMPONENT               INCLUSIONS in functional/alphabetical order
-----------------------------------------------------------------------------*/

#include "ipl_tb_internal.h"
#include "ipl_tb_l.h"
#include "opal_power.h"

/*-----------------------------------------------------------------------------
 LOCAL CONSTANTS                 DEFINITIONS in functional/alphabetical order
-----------------------------------------------------------------------------*/
#define MCU_CMD_POWER_SOC_POWERCYCLE 0x0D
#define MCU_ALARM_CMD 0x23
#define MCU_SET_STDBY_LVL 0xA9

/**
 * @param   wz_mbfield          cmd field
 * @param   *pb_arg              generic argument if used

 * @return  No return expected !!!
 *
 * @brief   Update Mailbox and reboot STB
 */
int32 m_IPL_TB_CmdReboot
(
   int32 wz_mbfield,
   char *pb_arg
)
{
#if defined (CONFIG_SILENT_UPGRADE_MB)
   INTLDR_Boot_screen_display_t BootScreenValue = INTLDR_GET_BOOT_SCREEN_FLAG_VALUE(); 
#endif
   if( INTLDR_Term() != MCM_OK )
   {
      IPL_PRT_DEBUG_PRINTF("Cannot read E2P data\r\n");
      return MCM_ERROR; 
   }
   else
   {
      IPL_PRT_DEBUG_PRINTF("mailBox has been updated\r\n");
   }

#ifndef CONFIG_BYPASS_OPAL_POWER
   opal_error_t result = OPAL_NO_ERROR;
   opal_handle_t  h_power = OPAL_NULL_HANDLE;

   result = opal_power_open(OPAL_POWER_SYSTEM, &h_power,  OPAL_IO_WRITE);
   if(result==OPAL_NO_ERROR)
   {
#if defined (CONFIG_SILENT_UPGRADE_MB)
      IPL_PRT_DEBUG_PRINTF("Boot screen value %d\n",BootScreenValue);
      if (BOOT_SCREEN_DISPLAY_OFF == BootScreenValue)
      {
            IPL_PRT_DEBUG_PRINTF("Silent upgrade reboot\r\n");
            sleep(1);
            result = opal_power_set_mode(h_power,OPAL_POWER_SOC);
            sleep(1);
            if(result!=OPAL_NO_ERROR)
            {
               IPL_PRT_DEBUG_PRINTF ("opal_power_set_mode:OPAL_POWER_RESET_SOC failed: %d  \r\n",result);
            }
            /*Wait till STB is switched off*/
            /*Klocwork issue: infinite loop expected as a normal behavior*/
            IPL_PRT_DEBUG_PRINTF ("Please reset your board.... \r\n");
            while(1);
      }
#endif
#if defined (CONFIG_SILENT_UPGRADE)
      result = opal_power_set_wake_up_delay(h_power, 10);
      if(result!=OPAL_NO_ERROR)
      {
         IPL_PRT_DEBUG_PRINTF ("opal_power_set_wake_up_delay failed: %d  \r\n",result);
      }

      result = opal_power_set_mode(h_power,OPAL_POWER_OFF);
      sleep(1);
      if(result!=OPAL_NO_ERROR)
      {
         IPL_PRT_DEBUG_PRINTF ("opal_power_set_mode:OPAL_POWER_OFF failed: %d  \r\n",result);
      }
#else
      result = opal_power_set_mode(h_power,OPAL_POWER_RESET);
      sleep(1);
      if(result!=OPAL_NO_ERROR)
      {
         IPL_PRT_DEBUG_PRINTF ("opal_power_set_mode:OPAL_POWER_RESET failed: %d  \r\n",result);
      }
#endif

      IPL_PRT_DEBUG_PRINTF ("Please reset your board.... \r\n");
      /*Wait till STB is switched off*/
      /*Klocwork issue: infinite loop expected as a normal behavior*/
      while(1);
   }
   IPL_PRT_DEBUG_PRINTF ("SET POWER OFF through OPAL failed\r\n");
#endif
   IPL_PRT_DEBUG_PRINTF ("Please reset your board \r\n");

   /*Wait till STB is switched off*/
   /*Klocwork issue: infinite loop expected as a normal behavior*/
   while(1);
} /* End of function 'm_IPL_TB_CmdReboot()' */
/** @} */

