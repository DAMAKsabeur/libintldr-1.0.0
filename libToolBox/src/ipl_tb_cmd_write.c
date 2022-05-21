/*-----------------------------------------------------------------------------
 FILE DOCUMENTATION
-----------------------------------------------------------------------------*/
/**
 * @note    Copyright 2000-2011 SagemCom SAS All rights reserved
 *
 * @file    ipl_tb_cmd_write.c
 * @brief   tool box write command Interface API definition
 * @version Revision of last commit : $Rev: 100 $
 * @date    Date of last commit     : $Date: 2011-08-04 10:45:30 +0100 (Thu, 4 Aug 2011) $
 * @author  Author of last commit   : $Author: g601627 $
 * @addtogroup INTLDR
 * @{
 */


/* ########################################################################## */
/* ### #include                                                               */
#include "ipl_tb_internal.h"
#include "ipl_tb_l.h"



int32 IPL_TB_WriteBlock(uint8 *p_DataRead,uint32 i_DataReadSize,IPL_TB_Device_t Mtd_Device,uint32 Block_Mapping,uint32 Block_Size)
{
   int32        Cr=MCM_ERROR;

   if (i_DataReadSize > Block_Size)
   {
      IPL_PRT_DEBUG_PRINTF ("Invalid file size: %ld > %ld \r\n",i_DataReadSize,Block_Size);
      Cr = MCM_ERROR;
   }
   else
   {
      if(p_DataRead != NULL)
      {
#if defined (CONFIG_IPL_MTD_NOR)
         if(IPL_TB_NOR_DEVICE == Mtd_Device)
         {
            uint8       *p_DataReadtmp=NULL;
            uint32      BlockMask=0xFFFFFFFF;
            uint32      i_StartAddress=0x0 ; 
            uint32      i_DataReadSizetmp=0x0 ;
            BlockMask = ~(CONFIG_NOR_BLOCK_SIZE-1);
            i_StartAddress = (Block_Mapping & BlockMask);
            
            /*Find lower address aligned*/
#ifdef IPL_DEBUG
            IPL_PRT_DEBUG_PRINTF("NOR block mask = %X \n",(unsigned int)BlockMask);
#endif

            /*Get size to read entire NOR */
            i_DataReadSizetmp = i_DataReadSize + (Block_Mapping - i_StartAddress);
            if(i_DataReadSizetmp % CONFIG_NOR_BLOCK_SIZE)
            {
               i_DataReadSizetmp = (i_DataReadSizetmp & BlockMask) + CONFIG_NOR_BLOCK_SIZE;
            }
            /*Save Nor block */
            p_DataReadtmp = malloc(i_DataReadSizetmp);
            if(p_DataReadtmp != NULL)
            {
               Cr = IPL_TB_NOR_Read(p_DataReadtmp,
                                   i_StartAddress,   
                                   i_DataReadSizetmp);

               /*Insert Data to write*/
               memcpy((p_DataReadtmp + (Block_Mapping - i_StartAddress)),p_DataRead,i_DataReadSize );

               if (Cr == MCM_OK)
               {
                  IPL_PRT_DEBUG_PRINTF ("Read OK \r\n");
                  Cr = IPL_TB_NOR_Erase(i_StartAddress,   
                                       i_DataReadSizetmp);
               }
               if (Cr == MCM_OK)
               {
                  IPL_PRT_DEBUG_PRINTF ("Erase OK \r\n");
                  Cr = IPL_TB_NOR_Write(p_DataReadtmp,
                                       i_StartAddress,   
                                       i_DataReadSizetmp);
               }
               if (Cr == MCM_OK)
               {  
                  IPL_PRT_DEBUG_PRINTF ("Write OK \r\n");
               }
               free(p_DataReadtmp);
            }
            else
            {
               IPL_PRT_DEBUG_PRINTF ("Allocation failed: %ld \r\n",i_DataReadSizetmp);
               Cr = MCM_ERROR;
            }
         }
         else /*NAND Block*/
#endif
         {
            Cr = IPL_TB_NAND_Erase( Mtd_Device,
                                    Block_Mapping,
                                    i_DataReadSize);

            if (Cr == MCM_OK)
            {
               IPL_PRT_DEBUG_PRINTF ("Erase OK \r\n");
               Cr = IPL_TB_NAND_Write(Mtd_Device,
                                    p_DataRead,
                                    Block_Mapping,
                                    i_DataReadSize);

               if (Cr == MCM_OK)
               {  
                  IPL_PRT_DEBUG_PRINTF ("Write OK \r\n");
               }
               else
               {
                  IPL_PRT_DEBUG_PRINTF ("IPL_TB_Write failed \r\n");
               }
            }
            else
            {
               IPL_PRT_DEBUG_PRINTF ("IPL_TB_Erase failed \r\n");
            }
         }
      }
      else
      {
         IPL_PRT_DEBUG_PRINTF ("Allocation failed: %ld \r\n",i_DataReadSize);
         Cr = MCM_ERROR;
      }
   }
   return Cr;
}
/* ########################################################################## */
/* ### Variables                                                              */

/* ########################################################################## */
/* ### Functions                                                              */

/**
 * @return  MCM_OK                     if no error occured.
 * @return  MCM_KO                     if an error occured.
 *
 * @brief   Write an image in the right memory area
 */
int32 m_IPL_TB_CmdWrite
(
   int32 wz_mbfield,
   char *pb_argv
)
{
   int32 		Cr=MCM_ERROR;
   uint8 		*p_DataRead = NULL;
   uint32 		i_DataReadSize=0x0 ;
   uint32 		BlockAddress = 0; 
   uint32 		BlockSize = 0;
   FILE			*input_file = NULL;
   char 		input_filename[32];
   char 		usb_path[256];
   IPL_TB_M_Ctx_t 	*pCtx = &IPL_TB_M_Ctx;
   IPL_TB_Device_t 	MtdDevice = 0; 

   if (pCtx->custom_path)
   {
      strcpy(usb_path,pCtx->usb_path);
   }
   else
   {
      strcpy(usb_path,IPL_TB_USB_PATH);
   }
#ifdef IPL_DEBUG
      IPL_PRT_DEBUG_PRINTF ("Path used: %s \r\n",usb_path);
#endif

   if (pb_argv == NULL)
   {
      return -1;
   }
   if (strcmp(pb_argv, "ldr1") == 0)
   {
      strcpy(input_filename,"ldr1.bin");
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
      strcpy(input_filename,"ldr2.bin");
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
      strcpy(input_filename,"kernel.bin");
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
      strcpy(input_filename,"fw.bin");
#if defined CONFIG_IPL_MTD_NAND_PARTITION_NAND
      MtdDevice = IPL_TB_KERNEL_MODULE;
#else
      MtdDevice = CONFIG_MAPPING_KERNEL_DEVICE;
#endif
      BlockAddress = CONFIG_MAPPING_KERNEL;
      BlockSize = CONFIG_MAPPING_KERNEL_BB_SIZE;
   }
   else if (strcmp(pb_argv, "rootfs") == 0)
#ifdef CONFIG_IPL_UPD_ROOT_FS_DOWNLOADABLE   
   {
      strcpy(input_filename,"rootfs.bin");
      MtdDevice = CONFIG_MAPPING_ROOT_FS_DEVICE;
      BlockAddress = CONFIG_MAPPING_ROOT_FS;
      BlockSize = CONFIG_MAPPING_ROOT_FS_BB_SIZE;
   }
#else
   IPL_PRT_DEBUG_PRINTF("\r\nRootfs cannot be written in FT configuration\r\n");
#endif
   else if (strcmp(pb_argv, "ra") == 0)
#ifdef CONFIG_IPL_UPD_RA_DOWNLOADABLE
   {
      strcpy(input_filename,"ra.bin");
#if defined CONFIG_IPL_MTD_NAND_PARTITION_NAND
      MtdDevice = IPL_TB_RA_MODULE;
#else
      MtdDevice = CONFIG_MAPPING_RA_DEVICE;
#endif
      BlockAddress = CONFIG_MAPPING_RA;
      BlockSize = CONFIG_MAPPING_RA_BB_SIZE;
   }
#else
      IPL_PRT_DEBUG_PRINTF("\r\nRA cannot be read for current configuration\r\n");
#endif
   else if (strcmp(pb_argv, "da2") == 0)
#ifdef CONFIG_IPL_UPD_DA2_DOWNLOADABLE
   {
      strcpy(input_filename,"da2.bin");
      MtdDevice = CONFIG_MAPPING_DA2_DEVICE;
      BlockAddress = CONFIG_MAPPING_DA2;
      BlockSize = CONFIG_MAPPING_DA2_SIZE;
   }
#else
      IPL_PRT_DEBUG_PRINTF("\r\nDA2 cannot be read in FT configuration\r\n");
#endif
   else if (strcmp(pb_argv, "da2R") == 0)
#ifdef CONFIG_IPL_UPD_DA2_DOWNLOADABLE
   {
      strcpy(input_filename,"da2R.bin");
      MtdDevice = CONFIG_MAPPING_DA2_DEVICE;
      BlockAddress = CONFIG_MAPPING_DA2_REDUNDANT;
      BlockSize = CONFIG_MAPPING_DA2_SIZE;
   }
#else
      IPL_PRT_DEBUG_PRINTF("\r\nDA2R cannot be read in FT configuration\r\n");
#endif
   else if (strcmp(pb_argv, "e2p") == 0)
   {
      strcpy(input_filename,"e2p.bin");
      MtdDevice = CONFIG_MAPPING_E2P_DEVICE;
      BlockAddress = CONFIG_MAPPING_E2P;
      BlockSize = CONFIG_MAPPING_E2P_SIZE;
   }
   
#ifdef CONFIG_IPL_MTD_NOR    
   else if (strcmp(pb_argv, "fd") == 0)
   {
      strcpy(input_filename,"fd.bin");
      MtdDevice = CONFIG_MAPPING_FD_DEVICE;
      BlockAddress = CONFIG_MAPPING_FD;
      BlockSize = CONFIG_MAPPING_FD_SIZE;
   }
  
   else if (strcmp(pb_argv, "bf") == 0)
   {
      strcpy(input_filename,"bf.bin");
      MtdDevice = CONFIG_MAPPING_BF_DEVICE;
      BlockAddress = CONFIG_MAPPING_BF;
      BlockSize = CONFIG_MAPPING_BF_SIZE;
   }
     
   else if (strcmp(pb_argv, "nomen") == 0)
   {
      strcpy(input_filename,"nomen.bin");
      MtdDevice = CONFIG_MAPPING_NOMEN_DEVICE;
      BlockAddress = CONFIG_MAPPING_NOMEN;
      BlockSize = CONFIG_MAPPING_NOMEN_SIZE;
   }
   else if (strcmp(pb_argv, "secdata") == 0)
   {
      strcpy(input_filename,"secdata.bin");
      MtdDevice = CONFIG_MAPPING_SEC_DATA_DEVICE;
      BlockAddress = CONFIG_MAPPING_SEC_DATA;
      BlockSize = CONFIG_MAPPING_SEC_DATA_SIZE;
   }
   else if (strcmp(pb_argv, "boot") == 0)
   {
      strcpy(input_filename,"boot.bin");
      MtdDevice = CONFIG_MAPPING_BOOT_DEVICE;
      BlockAddress = CONFIG_MAPPING_BOOT;
      BlockSize = CONFIG_MAPPING_BOOT_SIZE;
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

   IPL_PRT_DEBUG_PRINTF("\r\nWRITING %s\r\n",input_filename);
   if ( (strlen(usb_path) + strlen(input_filename) ) < sizeof(usb_path))
   {
      /*Klocwork:Array 'usb_path' of size 256 may use index value(s) 0..286*/
      /*Previous test before to apend filename protect from buffer overflow*/
      strncat(usb_path,input_filename,strlen(input_filename));
      input_file = fopen(usb_path, "r");
   }
   else
   {
      IPL_PRT_DEBUG_PRINTF("\r\n File name is too long : %s\r\n",input_filename);
   }

   if (input_file == NULL)
   {
      IPL_PRT_DEBUG_PRINTF ("Impossible to access to %s\r\n",usb_path);
      Cr = MCM_ERROR;
   }
   else
   {
      fseek(input_file, 0, SEEK_END);
      i_DataReadSize += ftell(input_file);
      fseek(input_file, 0, SEEK_SET);
      p_DataRead = malloc(i_DataReadSize);
      if (p_DataRead != NULL)
      {
         if(1!= fread (p_DataRead,i_DataReadSize,1, input_file))
         {
            IPL_PRT_DEBUG_PRINTF("[m_IPL_TB_CmdWrite] Impossible to read all module size: %x\r\n",i_DataReadSize);
         }
         else
         {
         Cr = IPL_TB_WriteBlock(p_DataRead, i_DataReadSize, MtdDevice, BlockAddress, BlockSize);
         }
         free(p_DataRead);
      }
      fclose(input_file);
   }

   return Cr;

} /* End of function 'm_IPL_TB_CmdWrite()' */
/** @} */

