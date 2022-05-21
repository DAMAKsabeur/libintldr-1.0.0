
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
X File          : ipl_tb_cmd_corrupt.c
X Author        : Anthony CAMMAS
X Creation date : 13/02/2009
X Product       : ST                                                        
X Language      : C                               
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
*@@@eF
*/

/* ########################################################################## */
/* ### #include                                                               */
#include "ipl_tb_l.h"
#include "ipl_tb_internal.h"


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
int32 IPL_TB_ReadBlock(       IPL_TB_Device_t          i_DeviceType,
                              uint8*             const o_pData,
                        const uint32                   i_StartAddress,
                        const uint32                   i_Size)
{
   int32        Cr=MCM_ERROR;


#if defined (CONFIG_IPL_MTD_NOR)
   if(i_DeviceType == IPL_TB_NOR_DEVICE)
   {
      Cr = IPL_TB_NOR_Read(o_pData,
             i_StartAddress,
             i_Size);
   }
   else
#endif
   {
      Cr = IPL_TB_NAND_Read(i_DeviceType,
             o_pData,
             i_StartAddress,
             i_Size);
   }
   return Cr;
}

int32 m_IPL_TB_CmdRead
(
   int32 wz_mbfield,
   char *pb_argv
)
{
   int32        		Cr=MCM_ERROR;
   uint8       			*p_DataRead=NULL;
   uint32 			BlockAddress = CONFIG_MAPPING_KERNEL;
   uint32 			BlockSize = 0;
   FILE        			*output_file;
   char        			output_filename[32];
   char        			usb_path[256];
   IPL_TB_M_Ctx_t 		*pCtx = &IPL_TB_M_Ctx;
   IPL_TB_Device_t   		MtdDevice = 0;
   if (pCtx->custom_path)
   {
      strcpy(usb_path,pCtx->usb_path);
   }
   else
   {
      strcpy(usb_path,IPL_TB_USB_PATH);
   }
#ifdef IPL_DEBUG
      IPL_PRT_DEBUG_PRINTF("Current path: %s \r\n",usb_path);
#endif
   if (pb_argv == NULL)
   {
      return -1;
   }
   if (strcmp(pb_argv, "ldr1") == 0)
   {
      strcpy(output_filename,"ldr1_dump.bin");
#if defined CONFIG_IPL_MTD_NAND_PARTITION_NAND
      MtdDevice = IPL_TB_DOWNLOAD_MODULE1;
#else
      MtdDevice = CONFIG_MAPPING_LOADER_DEVICE;
#endif
      BlockAddress = CONFIG_MAPPING_LOADER;
      BlockSize = CONFIG_MAPPING_LOADER_BB_SIZE; 
   }
   else if (strcmp(pb_argv, "ldr2") == 0)
   {
      strcpy(output_filename,"ldr2_dump.bin");
#if defined CONFIG_IPL_MTD_NAND_PARTITION_NAND
      MtdDevice = IPL_TB_DOWNLOAD_MODULE2;
#else
      MtdDevice = CONFIG_MAPPING_LOADER_2_DEVICE;
#endif
      BlockAddress = CONFIG_MAPPING_LOADER_2;
      BlockSize = CONFIG_MAPPING_LOADER_BB_SIZE; 
   }
   else if (strcmp(pb_argv, "kernel") == 0)
   {
      strcpy(output_filename,"kernel_dump.bin");
#if defined CONFIG_IPL_MTD_NAND_PARTITION_NAND
      MtdDevice = IPL_TB_KERNEL_MODULE;
#else
      MtdDevice = CONFIG_MAPPING_KERNEL_DEVICE;
#endif
      BlockAddress = CONFIG_MAPPING_KERNEL;
      BlockSize = CONFIG_MAPPING_KERNEL_BB_SIZE; 
   }
   else if (strcmp(pb_argv, "fw") == 0)
   {
      strcpy(output_filename,"fw_dump.bin");
#if defined CONFIG_IPL_MTD_NAND_PARTITION_NAND
      MtdDevice = IPL_TB_KERNEL_MODULE;
#else
      MtdDevice = CONFIG_MAPPING_KERNEL_DEVICE;
#endif
      BlockAddress = CONFIG_MAPPING_KERNEL;
      BlockSize = CONFIG_MAPPING_KERNEL_BB_SIZE; 
   }
   else if (strcmp(pb_argv, "rootfs") == 0)
#if CONFIG_IPL_UPD_ROOT_FS_DOWNLOADABLE
   {
      strcpy(output_filename,"rootfs_dump.bin");
      MtdDevice = CONFIG_MAPPING_ROOT_FS_DEVICE;
      BlockAddress = CONFIG_MAPPING_ROOT_FS;
      BlockSize = CONFIG_MAPPING_ROOT_FS_BB_SIZE; 
   }
#else
   IPL_PRT_DEBUG_PRINTF("\r\nRootfs cannot be read in current configuration\r\n");
#endif
   else if (strcmp(pb_argv, "da2") == 0)
#if CONFIG_IPL_UPD_DA2_DOWNLOADABLE
   {
      strcpy(output_filename,"da2_dump.bin");
      MtdDevice = CONFIG_MAPPING_DA2_DEVICE;
      BlockAddress = CONFIG_MAPPING_DA2;
      BlockSize = CONFIG_MAPPING_DA2_SIZE;	   
   }
#else
   IPL_PRT_DEBUG_PRINTF("\r\nDA2 cannot be read in current configuration\r\n");
#endif
   else if (strcmp(pb_argv, "fw2") == 0)
#if CONFIG_IPL_UPD_KERNEL_2_DOWNLOADABLE
   {
      strcpy(output_filename,"fw2_dump.bin");
      MtdDevice = CONFIG_MAPPING_KERNEL_2_DEVICE;
      BlockAddress = CONFIG_MAPPING_KERNEL_2;
      BlockSize = CONFIG_MAPPING_KERNEL_2_SIZE;	   
   }
#else
   IPL_PRT_DEBUG_PRINTF("\r\nKernel 2 cannot be read in current configuration\r\n");
#endif
   else if (strcmp(pb_argv, "daR2") == 0)
#if CONFIG_IPL_UPD_DA2_DOWNLOADABLE
   {
      strcpy(output_filename,"da2R_dump.bin");
      MtdDevice = CONFIG_MAPPING_DA2_DEVICE;
      BlockAddress = CONFIG_MAPPING_DA2_REDUNDANT;
      BlockSize = CONFIG_MAPPING_DA2_SIZE;	   
   }
#else
   IPL_PRT_DEBUG_PRINTF("\r\nDA2R cannot be read in current configuration\r\n");
#endif
   else if (strcmp(pb_argv, "e2p") == 0)
   {
      strcpy(output_filename,"e2p_dump.bin");
      MtdDevice = CONFIG_MAPPING_E2P_DEVICE;
      BlockAddress = CONFIG_MAPPING_E2P;
      BlockSize = CONFIG_MAPPING_E2P_SIZE;	   
   }
   
#ifdef CONFIG_IPL_MTD_NOR   
   else if (strcmp(pb_argv, "fd") == 0)
   {
      strcpy(output_filename,"fd_dump.bin");
      MtdDevice = CONFIG_MAPPING_FD_DEVICE;
      BlockAddress = CONFIG_MAPPING_FD;
      BlockSize = CONFIG_MAPPING_FD_SIZE;	   
   }
   
   else if (strcmp(pb_argv, "bf") == 0)
   {
      strcpy(output_filename,"bf_dump.bin");
      MtdDevice = CONFIG_MAPPING_BF_DEVICE;
      BlockAddress = CONFIG_MAPPING_BF;
      BlockSize = CONFIG_MAPPING_BF_SIZE;	   
   }
     
   else if (strcmp(pb_argv, "nomen") == 0)
   {
      strcpy(output_filename,"nomen_dump.bin");
      MtdDevice = CONFIG_MAPPING_NOMEN_DEVICE;
      BlockAddress = CONFIG_MAPPING_NOMEN;
      BlockSize = CONFIG_MAPPING_NOMEN_SIZE;	   
   }
  
   else if (strcmp(pb_argv, "boot") == 0)
   {
      strcpy(output_filename,"boot_dump.bin");
      MtdDevice = CONFIG_MAPPING_BOOT_DEVICE;
      BlockAddress = CONFIG_MAPPING_BOOT;
      BlockSize = CONFIG_MAPPING_BOOT_SIZE; 
   }
   else if (strcmp(pb_argv, "secdata") == 0)
   {
      strcpy(output_filename,"secdata_dump.bin");
      MtdDevice = CONFIG_MAPPING_SEC_DATA_DEVICE;
      BlockAddress = CONFIG_MAPPING_SEC_DATA;
      BlockSize = CONFIG_MAPPING_SEC_DATA_SIZE;
   }
#endif 
   else
   {
      IPL_PRT_DEBUG_PRINTF("\r\nBlock name was not recognized.\r\n");
      return MCM_ERROR;
   }
   if (BlockSize == 0)
   {
      IPL_PRT_DEBUG_PRINTF("\r\nBlock name not supported for this project.\r\n");
      return MCM_ERROR;
   }

   IPL_PRT_DEBUG_PRINTF("\r\nCOPYING to %s\r\n",output_filename);
   if ( (strlen(usb_path) + strlen(output_filename) ) < sizeof(usb_path))
   {
      /*Klocwork:Array 'usb_path' of size 256 may use index value(s) 0..286*/
      /*Previous test before to apend filename protect from buffer overflow*/
      strncat(usb_path,output_filename,strlen(output_filename));
   }
   else
   {
      IPL_PRT_DEBUG_PRINTF("\r\n File name is too long : %s\r\n",output_filename);
   }
        
   p_DataRead = malloc(BlockSize);
   if(p_DataRead != NULL)
   { 
      Cr=IPL_TB_ReadBlock(MtdDevice, p_DataRead, BlockAddress, BlockSize);
   }
   else
      IPL_PRT_DEBUG_PRINTF("\r\nmalloc failed : %d\r\n",BlockSize);

   if(MCM_OK == Cr)
   {
      /* Copy the download file to usb key */
      output_file = fopen(usb_path, "wb");
      if (output_file == NULL)
      {
         IPL_PRT_DEBUG_PRINTF("Impossible to write to %s\r\n",usb_path);
         Cr = MCM_ERROR;
      }
      else
      {
         IPL_PRT_DEBUG_PRINTF("Writing dump file...");
         if ( 1 == fwrite(p_DataRead, BlockSize, 1, output_file) )
         {
            IPL_PRT_DEBUG_PRINTF("done\n");
         }
         else
         {
            IPL_PRT_DEBUG_PRINTF("failed\n");
            Cr = MCM_ERROR;
         }
         fclose(output_file);            
      }
   }
   else
   {
      IPL_PRT_DEBUG_PRINTF("Read MTD device failed !\n");
   }

   free(p_DataRead);

   return Cr;

} /* End of function 'm_IPL_TB_CmdRead()' */
/** @} */

