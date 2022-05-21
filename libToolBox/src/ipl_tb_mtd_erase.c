/**
 * @param   i_StartAddress                Start address of the area to erase
 * @param   i_Size                        Size of the area to erase
 *
 * @return  MCM_OK                        if no error occured
 * @return  INTLDR_ERROR_MTD_ERASE_FLASH  if an error occured
 *
 * @brief   Erase NOR flash area.
 */
#include "ipl_tb_internal.h"

#if defined CONFIG_IPL_MTD_NOR 
int32 IPL_TB_NOR_Erase(const uint32             i_StartAddress, 
                       const uint32             i_Size)
{
   int32                 vRet = MCM_ERROR;
   IPL_TB_M_Ctx_t *pCtx    = &IPL_TB_M_Ctx;
   struct erase_info_user sMtdEraseInfo;
   uint32                SectorSize             = -1;
   uint32                LastDst                = -1;
   uint32                Size                   = i_Size;
   uint32                EraseSize              = -1;
   uint32                Percent        = -1;
   int h;

   if (pCtx->mtd_available == 1)
   {
      LastDst = i_StartAddress + i_Size;

      SectorSize  = CONFIG_NOR_BLOCK_SIZE;
      for(;;)
      {
         if(i_StartAddress % SectorSize)
         {
            IPL_PRT_DEBUG_PRINTF("Erase area is not aligned on block size : %ld\n",SectorSize);
            vRet=MCM_ERROR;
            break;
         }

         sMtdEraseInfo.start  = i_StartAddress;
         sMtdEraseInfo.length = SectorSize;

         h    = open(pCtx->mtd_strDevPath,O_RDWR);      if(h<0)   {break;}
         IPL_PRT_DEBUG_PRINTF("Erasing NOR flash: 00%%");

         while (Size > 0) /** break if write operation is not successful*/
         {
            if((sMtdEraseInfo.start + SectorSize) > LastDst)
            {
               IPL_PRT_DEBUG_PRINTF("Can't write last NOR Block (risk to overide a module partition)\n");
               break; /*Erase should not overide next module partition*/
            }
            vRet = ioctl(h,MEMERASE,&sMtdEraseInfo);
            if(vRet)
            {
               IPL_PRT_DEBUG_PRINTF("Erase NOR error\n");
               break;
            }
            else
            {
               EraseSize = (Size > SectorSize) ? SectorSize : Size;
               sMtdEraseInfo.start += SectorSize;
               Size                -= EraseSize;

               Percent =(uint32) ((uint64)(i_Size - Size) * (uint64)100 / (uint64)i_Size);
               IPL_PRT_DEBUG_PRINTF("\b\b\b%.2ld%%",Percent);
            }
         }

         IPL_PRT_DEBUG_PRINTF("\r\n");
         vRet = close(h);
         break;
      }
   }
   else
   {
      IPL_PRT_DEBUG_PRINTF("Mtd FullNor Partition not available to execute this command\n");
      return MCM_ERROR;
   }

   return (vRet >= 0) ? MCM_OK : INTLDR_ERROR_MTD_ERASE_FLASH;
}
#endif


#if defined(CONFIG_IPL_TB_NAND_SC)
/**
 * @param   i_StartAddress                Start address of the area to erase
 * @param   i_Size                        Size of the area to erase
 * @param   i_MaxSize                     Maximum size of area
 *
 * @return  MCM_OK                        if no error occured
 * @return  INTLDR_ERROR_MTD_ERASE_FLASH  if an error occured
 *
 * @brief   Erase NAND flash area.
 */
int32 IPL_TB_NAND_Erase(IPL_TB_Device_t         i_DeviceType,
                        const uint32            i_StartAddress,
                        const uint32            i_Size)
{
   mtd_info_t        mtd_info;
   uint64            Address     = -1;
   uint32            LastAddress = -1;
   uint32            Size        = -1;
   uint32            TotalSize   = -1;
   uint32            Percent     = -1;
   int32             vRet        = -1;
   int32             Cr          = MCM_ERROR;
   struct erase_info_user erase_info;
   int h;

   FILE * f;
   int minor;
   int size;
   int erasesize;
   int vMatchNb;
   char name[IPL_TB_INFOS_MAXCAR_PERLINE];
   char Mtdname[IPL_TB_INFOS_MAXCAR_PERLINE];
   char strLine[IPL_TB_INFOS_MAXCAR_PERLINE];
   char Devname[IPL_TB_INFOS_MAXCAR_PERLINE];

   /* No filesystems are implemented. NAND partitions are seen as a succession of blocks.        */
   /* When a bad block is detected, it is marked as bad in Bad Block Table (done in NAND driver).*/
   
   /* 1 - The block was marked as bad before the reading, then it means                          */
   /*     that this block was bad during last writing, so we just skip to                        */
   /* 	 the next NAND block to read the next datas of the module. 			                      */
   /* 2 - The block was first detected during the reading operation, then                        */
   /* 	 it means the read datas are corrupted and can't be recovered.                          */
   /*     In this case, we just return from the function.                                        */

   /*Find mtd path*/
   f=fopen("/proc/mtd","r");
   if(f== NULL) 
   {
      IPL_PRT_DEBUG_PRINTF("Could not open /proc/mtd\n");
      return MCM_ERROR;
   }
   switch ((IPL_TB_Device_t)i_DeviceType)
   {
      case IPL_TB_NAND_DEVICE:
         strcpy(Devname, CONFIG_IPL_MTD_MMC_DEVICE_NAME);
        break;
#if defined CONFIG_IPL_MTD_NAND_PARTITION_NAND
      case IPL_TB_NOE2P_DEVICE:
         strcpy(Devname, CONFIG_IPL_MTD_NOE2P_DEVICE_NAME);
        break;
      case IPL_TB_DOWNLOAD_MODULE1:
         strcpy(Devname, CONFIG_IPL_MTD_DOWNLOADER_DEVICE_NAME);
        break;
      case IPL_TB_RA_MODULE:
         strcpy(Devname, CONFIG_IPL_MTD_RA_DEVICE_NAME);
        break;
      case IPL_TB_KERNEL_MODULE:
         strcpy(Devname, CONFIG_IPL_MTD_KERNEL_DEVICE_NAME);
        break;
      case IPL_TB_KERNEL_2_MODULE:
         strcpy(Devname, CONFIG_IPL_MTD_KERNEL_2_DEVICE_NAME);
        break;
      case IPL_TB_DOWNLOAD_MODULE2:
         strcpy(Devname, CONFIG_IPL_MTD_DOWNLOADER2_DEVICE_NAME);
        break;
      case IPL_TB_BG_MODULE:
         strcpy(Devname, CONFIG_IPL_MTD_BG_DEVICE_NAME);
        break;
#if defined CONFIG_LOADER_NO_NOR 
/* Not implemented */
         !!!! NAND SLC cannot be used without NOR !!!
#endif 
#endif
      default:
         IPL_PRT_DEBUG_PRINTF("INVALID NAND PARTITION\n");
         fclose(f);
         return MCM_ERROR;
         break;
   }
   while(0 != fgets (strLine,IPL_TB_INFOS_MAXCAR_PERLINE,f))
   {
      /* First line is a dummy line : "dev:    size   erasesize  name\n" */
      /* (see .../kernel/drivers/mtd/mtdcore.c)                          */
      vMatchNb=sscanf (strLine,"mtd%d: %8x %8x %s",&minor,&size,&erasesize,name);

      /* Scanf pattern above should return 4 values (i.e. minor, size, erasesize and name) */
      if(IPL_TB_INFOS_NB==vMatchNb)
      {
         if(!strcmp(Devname,name))
         {
            sprintf(Mtdname,"/dev/mtd%d",minor);
            Cr = MCM_OK;
         }
      }
   }
                           
   if(MCM_OK != fclose(f))
   {
      IPL_PRT_DEBUG_PRINTF("Init MTD Close error\n");
      return MCM_ERROR;
   }

   /*Read Nand*/
   if(MCM_OK != Cr)
   {
      IPL_PRT_DEBUG_PRINTF("No NAND device Found\n");
      return MCM_ERROR;
   }

   IPL_PRT_DEBUG_PRINTF("[IPL_TB_NAND_Erase] Access device %s \n",Mtdname);
   /* open mtd-nand device */
   h = open(Mtdname,O_RDWR);
   if(h<0)
   {
      IPL_PRT_DEBUG_PRINTF("ERROR IPL_MTD_F_EraseNand (MTD BUSY)\n");
      return (INTLDR_ERROR_MTD_WRITE_FLASH);
   }

   /* Get NAND geometry informations */
   vRet = ioctl(h, MEMGETINFO, &mtd_info);
   if (vRet != 0)
   {
      IPL_PRT_DEBUG_PRINTF("ERROR IPL_MTD_F_WriteNand IOCTLMEMGETINFO vRet= 0x%08x\r\n", vRet);
      Cr = INTLDR_ERROR_MTD_WRITE_FLASH;
   }

   /* Erase block and take care of NAND Bad Block Table*/ 
   if (Cr == MCM_OK)
   {
      Address     = i_StartAddress;
      LastAddress = i_StartAddress + mtd_info.size;
      /* Round up size with a complete number of blocks */
      Size     = (i_Size + (mtd_info.erasesize - 1)) & ~(mtd_info.erasesize - 1);
      TotalSize= Size;

      while ((Cr == MCM_OK) &&
             (Address < LastAddress) &&
             (Size > 0))
      {
         /* Check block status in BBT */
		   vRet = ioctl(h, MEMGETBADBLOCK, &Address);
         switch (vRet)
         {
            case 0:
               {
                  /* Block is okay, continue with erasing */
                  break;
               }
            case 1:
               {
                  /* Block is marked as bad, continue with next block */
                  IPL_PRT_DEBUG_PRINTF("IPL_MTD_F_EraseNand MEMGETBADBLOCK (Bad block) @0x%08x \r\n",(unsigned)Address);
                  Address += mtd_info.erasesize;
                  continue;
               }
            default:
               {
                  /* Should never happen */
                  IPL_PRT_DEBUG_PRINTF("IPL_MTD_F_EraseNand MEMGETBADBLOCK (Unknown): %x\r\n", vRet);
                  Cr = INTLDR_ERROR_MTD_ERASE_FLASH;
               }
         }

         if (Cr == MCM_OK)
         {
            erase_info.start = (uint32)Address;
            erase_info.length = mtd_info.erasesize;
            
            /* Erase block */
            vRet = ioctl(h, MEMERASE, &erase_info);
            switch (vRet)
            {
               case MCM_OK: /* Erase succesfull */
                  {
                	  Address  += mtd_info.erasesize;
                	  Size     -= mtd_info.erasesize;

                    Percent =(uint32) ((uint64)(TotalSize - Size) * (uint64)100 / (uint64)TotalSize);
                    IPL_PRT_DEBUG_PRINTF("\b\b\b%.2ld%%",Percent);
                     break;
                  }
               case NAND_ERROR_ERASE: 
                  {
                     IPL_PRT_DEBUG_PRINTF("ERROR IPL_MTD_F_EraseNand (Erase)\r\n");

                     /* Try again with next block */
                     Address += mtd_info.erasesize;
                     continue;
                  }
               default:
                  {
                  {
                     /* Undefined NAND error case*/
                     /* NAND driver should have marked block as BAD*/
                     vRet = ioctl(h, MEMGETBADBLOCK, &Address);
                     switch (vRet)
                     {
                        case 0:
                           /* Should never happen, BLOCK OK after Erase KO */
                           IPL_PRT_DEBUG_PRINTF("ERROR IPL_MTD_F_EraseNand MEMGETBADBLOCK return OK after Erase Nand KO !!! @0x%08x \r\n",(unsigned)Address);
                           Cr = INTLDR_ERROR_MTD_ERASE_FLASH;
                           break;
                        case 1:
                           /* Block is marked as bad, continue with next block */
                           IPL_PRT_DEBUG_PRINTF("IPL_MTD_F_EraseNand MEMGETBADBLOCK (Bad block) @0x%08x \r\n",(unsigned)Address);
                           Address += mtd_info.erasesize;
                          continue;
                        default:
                           /* Should never happen */
                           IPL_PRT_DEBUG_PRINTF("ERROR IPL_MTD_F_EraseNand MEMGETBADBLOCK (Unknown): %x \r\n",vRet);
                           Cr = INTLDR_ERROR_MTD_ERASE_FLASH;
                     }
                  }
                  }
            }
         }
      }

      if (Size != 0)
      {
         Cr = INTLDR_ERROR_MTD_ERASE_FLASH;
      }
   }

   /* Close the mtd device */
   if(MCM_OK != close(h))
   {
      IPL_PRT_DEBUG_PRINTF("ERROR IPL_MTD_F_EraseNand MTD Close\r\n");
      Cr = INTLDR_ERROR_MTD_ERASE_FLASH;
   }

   return (Cr == MCM_OK) ? MCM_OK : INTLDR_ERROR_MTD_ERASE_FLASH;
}
#else
/**
 * @param   i_StartAddress                Start address of the area to erase
 * @param   i_Size                        Size of the area to erase
 *
 * @return  MCM_OK                        if no error occured
 * @return  INTLDR_ERROR_MTD_ERASE_FLASH  if an error occured
 *
 * @brief   Erase NAND flash area.
 */
int32 IPL_TB_NAND_Erase(IPL_TB_Device_t          i_DeviceType,
                        const uint32            i_StartAddress,
                        const uint32            i_Size)
{
   int32             vRet        = -1;
   uint32 	         SectorSize  = -1;
   uint32 	         CurrentDst  = -1;
   uint32 	         Size 	      = -1;
   uint32 	         WriteSize   = -1;
   uint32            Percent     = -1;
   uint8*            pData     = NULL;
   int h;
   char Devpath[IPL_TB_INFOS_MAXCAR_PERLINE];

   CurrentDst  = i_StartAddress;
   Size        = i_Size;
   SectorSize  = CONFIG_MMC_BLOCK_SIZE;


   switch ((IPL_TB_Device_t)i_DeviceType)
   {
      case IPL_TB_NAND_DEVICE:
         strcpy(Devpath, CONFIG_IPL_MTD_MMC_DEVICE_NAME);
        break;
#if defined CONFIG_IPL_MTD_NAND_PARTITION_NAND
      case IPL_TB_DOWNLOAD_MODULE1:
         strcpy(Devpath, CONFIG_IPL_MTD_DOWNLOADER_DEVICE_PATH);
        break;
      case IPL_TB_RA_MODULE:
         strcpy(Devpath, CONFIG_IPL_MTD_RA_DEVICE_PATH);
        break;
      case IPL_TB_KERNEL_MODULE:
         strcpy(Devpath, CONFIG_IPL_MTD_KERNEL_DEVICE_PATH);
        break;
      case IPL_TB_DOWNLOAD_MODULE2:
         strcpy(Devpath, CONFIG_IPL_MTD_DOWNLOADER2_DEVICE_PATH);
        break;
      case IPL_TB_BG_MODULE:
         strcpy(Devpath, CONFIG_IPL_MTD_BG_DEVICE_PATH);
        break;
#if defined CONFIG_LOADER_NO_NOR
      case IPL_TB_NOMEN_HW_DEVICE:
         strcpy(Devpath, CONFIG_IPL_EMMC_NOMEN_HW_DEVICE_PATH);
        break;
      case IPL_TB_FACTORY_DATA_DEVICE:
         strcpy(Devpath, CONFIG_IPL_EMMC_FACTORY_DATA_DEVICE_PATH);
        break;
      case IPL_TB_DA2_DEVICE:
         strcpy(Devpath, CONFIG_IPL_EMMC_DA2_DEVICE_PATH);
        break;
      case IPL_TB_SEC_DATA_DEVICE:
         strcpy(Devpath, CONFIG_IPL_EMMC_SEC_DATA_DEVICE_PATH);
        break;
#endif
#endif
      default:
         IPL_PRT_DEBUG_PRINTF("INVALID NAND PARTITION\n");
         return MCM_ERROR;
         break;
   }

   pData = malloc(SectorSize);
   if(pData)
   {
	   memset(pData, 0xFF, SectorSize);

      for(;;)
      {
         h    = open(Devpath,O_RDWR);  
         if(h<0)
         {
            vRet=MCM_ERROR;
            IPL_PRT_DEBUG_PRINTF("%s , MMC Busy\n",Devpath);
            break;
         }
            
         while (Size > 0) /** breack if write operation is not successful*/
         {
            WriteSize = (Size > SectorSize) ? SectorSize : Size;

            vRet = lseek(h, CurrentDst, SEEK_SET);              
            if(vRet<0)
            {
               IPL_PRT_DEBUG_PRINTF("MMC seek error\n");
               break;
            }
            vRet = write(h, pData, WriteSize);
            if(vRet<0) 
            {
               IPL_PRT_DEBUG_PRINTF("MMC write error\n");
               break;
            }
            else
            {
               CurrentDst += WriteSize;
               Size       -= WriteSize;

               Percent = (uint32)((uint64)(i_Size - Size) * (uint64)100 / (uint64)i_Size);
               IPL_PRT_DEBUG_PRINTF("\b\b\b%.2ld%%",Percent);
            }
         }

         vRet = close(h);
         if(MCM_OK != vRet)
         {
            IPL_PRT_DEBUG_PRINTF("Erase Close error\n");
            break;
         }
         
         break;
      }
      free(pData);
   }
   else
   {
      IPL_PRT_DEBUG_PRINTF("Erase Nand alloc error\n");
      return INTLDR_ERROR_MTD_ERASE_FLASH;   
   } 

   return (vRet >= 0) ? MCM_OK : INTLDR_ERROR_MTD_ERASE_FLASH;
}
#endif

