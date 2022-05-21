

/*-----------------------------------------------------------------------------
 FILE DOCUMENTATION
-----------------------------------------------------------------------------*/
/**
 * @note    Copyright 2000-2012 SagemCom SAS All rights reserved
 *
 * @file    ipl_tb_cmd_corrupt.c
 * @brief   IPL_TB implementation
 * @version Revision of last commit : 14
 * @date    Date of last commit     : 2012-02-06 16:15:34 +0100 (Mon, 06 Feb 2012)
 * @author  Author of last commit   : Jean-Louis BONNAFFE 
 * @addtogroup IPL_TB
 * @{
 */

/*-----------------------------------------------------------------------------
 SYSTEM                          INCLUSIONS in functional/alphabetical order
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 FUNCTIONAL                      INCLUSIONS in functional/alphabetical order
-----------------------------------------------------------------------------*/
#include "ipl_tb_internal.h"
#include "ipl_tb.h"
#include "ipl_tb_l.h"

/*-----------------------------------------------------------------------------
 STATIC FUNCTIONS                DEFINITIONS in functional/alphabetical order
-----------------------------------------------------------------------------*/
static int32 IPL_TB_CorruptBlock(IPL_TB_Device_t,uint32,uint32);

/** 
 * @brief     Corrupt the block's 5 first bytes with a constant pattern
 * @param     Mtd_Device      IPL_TB_NOR_DEVICE, IPL_TB_NAND_DEVICE, IPL_TB_EEPROM_DEVICE 
 * @param     Block_Mapping   Block start address
 * @parameter Block_Size      Block size
 * @return    MCM_OK if no error occurred
 * @return    MCM_ERROR if an error occured
 */
static int32 IPL_TB_CorruptBlock(IPL_TB_Device_t Mtd_Device,uint32 Block_Mapping,uint32 Block_Size)
{
   int32 Cr= MCM_OK;
   uint8 pattern = 0x34;
   uint8* data_buffer = NULL;
   uint32       	corrupt_size = 0x400;
   if (Block_Size<corrupt_size)
   {
      IPL_PRT_DEBUG_PRINTF("Block size too small for corruption\r\n");
      return MCM_ERROR;
   }

   data_buffer = malloc(corrupt_size);
   
   if(data_buffer != NULL)
   {
      Cr = IPL_TB_ReadBlock(Mtd_Device,
                            data_buffer,
                            Block_Mapping,
                            corrupt_size);
   }
   else
   {
     return MCM_ERROR;
   }

   if(Cr == MCM_OK)
   {
      memset(data_buffer,pattern,corrupt_size);
   }

   if(Cr == MCM_OK)
   {
      Cr = IPL_TB_WriteBlock(data_buffer,
                             corrupt_size,
                             Mtd_Device,
                             Block_Mapping,
                             corrupt_size);
   }

   free(data_buffer);
   return Cr; 
}


/*-----------------------------------------------------------------------------
 INTERNAL FUNCTIONS              DEFINITIONS in functional/alphabetical order
-----------------------------------------------------------------------------*/

/** 
 * @brief    Corrupt the memory block indicated by the parameter.
 */
int32 m_IPL_TB_CmdCorrupt
(
   int32 wz_mbfield,
   char *pb_argv
)
{
   IPL_TB_Device_t 	device;
   uint32       	offset;
   uint32       	size;
   int32        	Cr = MCM_OK;

   if (pb_argv == NULL)
   {
      IPL_PRT_DEBUG_PRINTF("No flash area specified\r\n");
      return MCM_ERROR;
   }
#if 0
/* Dangerous cmd !!!*/
/*
   if (strcmp(pb_argv, "boot") == 0)
   {
      device = CONFIG_MAPPING_BOOT_DEVICE;
      offset = CONFIG_MAPPING_BOOT;
      size   = CONFIG_MAPPING_BOOT_SIZE;
   }
*/
   else if (strcmp(pb_argv, "ldr1") == 0)
   {
      device = CONFIG_MAPPING_LOADER_DEVICE;
      offset = CONFIG_MAPPING_LOADER;
      size   = CONFIG_MAPPING_LOADER_BB_SIZE;
   }
   else if (strcmp(pb_argv, "ldr2") == 0)
   {
      device = CONFIG_MAPPING_LOADER_2_DEVICE;
      offset = CONFIG_MAPPING_LOADER_2;
      size   = CONFIG_MAPPING_LOADER_BB_SIZE;
   }
   else if (strcmp(pb_argv, "kernel") == 0)
   {
      device = CONFIG_MAPPING_KERNEL_DEVICE;
      offset = CONFIG_MAPPING_KERNEL;
      size   = CONFIG_MAPPING_KERNEL_BB_SIZE;
   }
   else if (strcmp(pb_argv, "e2p") == 0)
   {
      device = CONFIG_MAPPING_E2P_DEVICE;
      offset = CONFIG_MAPPING_E2P;
      size   = CONFIG_MAPPING_E2P_SIZE;
   }
#ifdef CONFIG_IPL_MTD_NOR
   else if (strcmp(pb_argv, "fd") == 0)
   {
      device = CONFIG_MAPPING_FD_DEVICE;
      offset = CONFIG_MAPPING_FD;
      size   = CONFIG_MAPPING_FD_SIZE;
   }
  
   else if (strcmp(pb_argv, "bf") == 0)
   {
      device = CONFIG_MAPPING_BF_DEVICE;
      offset = CONFIG_MAPPING_BF;
      size   = CONFIG_MAPPING_BF_SIZE;
   }

   else if (strcmp(pb_argv, "nomen") == 0)
   {
      device = CONFIG_MAPPING_NOMEN_DEVICE;
      offset = CONFIG_MAPPING_NOMEN;
      size   = CONFIG_MAPPING_NOMEN_SIZE;
   }
#endif
   
#ifdef CONFIG_IPL_UPD_RA_DOWNLOADABLE
   else if (strcmp(pb_argv, "ra") == 0)
   {
      device = CONFIG_MAPPING_RA_DEVICE;
      offset = CONFIG_MAPPING_RA;
      size   = CONFIG_MAPPING_RA_BB_SIZE;
   }
#endif

   else
   {
      IPL_PRT_DEBUG_PRINTF("Unknown flash area\r\n");
      return -1;
   }

   Cr = IPL_TB_CorruptBlock(device, offset, size);
#endif
   return Cr;

} 

/** @} */
