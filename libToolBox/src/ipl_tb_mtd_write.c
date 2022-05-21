/**
 * @param   i_pData                        Data buffer array.
 * @param   i_StartAddress                 Start address of the area to write to
 * @param   i_Size                         Number of the datas to be write
 *
 * @return  MCM_OK                         if no error occured
 * @return  INTLDR_ERROR_MTD_TOO_BIG_FLASH if datas to write don't fit in area
 * @return  INTLDR_ERROR_MTD_WRITE_FLASH   if an error occured
 *
 * @brief   Writes data to NOR flash.
 */
#include "ipl_tb_internal.h"
#include <errno.h>

#if defined CONFIG_IPL_MTD_NOR 
int32 IPL_TB_NOR_Write(  const uint8*           const i_pData,
                         const uint32                 i_StartAddress,
                         const uint32                 i_Size)
{
   int32              vRet = MCM_OK;
   IPL_TB_M_Ctx_t *pCtx    = &IPL_TB_M_Ctx;
   uint32                SectorSize  = -1; 
   uint32                CurrentSrc  = (uint32)i_pData;
   uint32                CurrentDst  = i_StartAddress;
   uint32                Size         = i_Size;
   uint32                WriteSize   = -1; 
   uint32                Percent     = -1; 
   int h;

   if (pCtx->mtd_available == 1)
   {
      SectorSize = CONFIG_NOR_BLOCK_SIZE;

      for(;;)
      {   
         h    = open(pCtx->mtd_strDevPath,O_RDWR);      if(h<0)    {vRet=MCM_ERROR; break;}
         IPL_PRT_DEBUG_PRINTF("Writing NOR flash: 00%%");
       
         while (Size > 0) /** breack if write operation is not successful*/
         {
            WriteSize = (Size > SectorSize) ? SectorSize : Size;
            vRet = lseek(h, CurrentDst, SEEK_SET);
            if(vRet<0)
            {
               IPL_PRT_DEBUG_PRINTF("NOR device : seek error\n");
               break;
            }
            vRet = write(h, (uint8 *)CurrentSrc, WriteSize);
            if(vRet<0) 
            {
               IPL_PRT_DEBUG_PRINTF("NOR device : write error\n");
               break;
            }
            else
            {
               CurrentSrc += vRet;
               CurrentDst += vRet;
               Size       -= vRet;

               Percent = (uint32)((uint64)(i_Size - Size) * (uint64)100 / (uint64)i_Size);
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
   return (vRet == MCM_OK) ? MCM_OK : INTLDR_ERROR_MTD_WRITE_FLASH;

}
#endif
#if defined(CONFIG_IPL_TB_NAND_SC)
/**
 * @param   i_pData        Data buffer array.
 * @param   i_StartAddress Start address of the area to write to
 * @param   i_Size         Number of the datas to be write
 *
 * @return  MCM_OK                               if no error occured
 * @return  INTLDR_ERROR_MTD_TOO_BIG_FLASH       if datas to write don't fit in area
   @return  INTLDR_ERROR_MTD_TOO_MANY_BAD_BLOCKS if datas can't fit in partition because of bad blocks
 * @return  INTLDR_ERROR_MTD_WRITE_FLASH         otherwise
 *
 * @brief   Writes data to NAND flash.
*/
int32 IPL_TB_NAND_Write(IPL_TB_Device_t    i_DeviceType,
                        const uint8* const          i_pData,
                        const uint32                i_StartAddress,
                        const uint32                i_Size)
{
   mtd_info_t        mtd_info;
   uint32            NbPagesPerBlock    = -1;
   uint32            CurrentSrc             = -1;
   uint64            CurrentDst             = -1;
   uint32            LastDst                = -1;
   uint32            Size                   = -1;
   uint32            lastblockSize          = -1;
   uint32            page_index             = -1;
   int32             vRet                   = -1;
   int 	 	         Cr                     = MCM_ERROR;
   void              *pBuffer               = NULL;
   uint32             Percent               = 0;
   int               h;

   FILE * f;
   int minor;
   int size;
   int erasesize;
   int vMatchNb;
   char name[IPL_TB_INFOS_MAXCAR_PERLINE];
   char Mtdname[IPL_TB_INFOS_MAXCAR_PERLINE];
   char strLine[IPL_TB_INFOS_MAXCAR_PERLINE];
   char Devname[IPL_TB_INFOS_MAXCAR_PERLINE];

   /* No filesystems are implemented. NAND partitions are seen as a succession of blocks. */
   
   /* When a bad block is detected, it is marked as bad in Bad Block Table (done in NAND driver), 
      and we re-write the whole content of the block to the next good block in the partition. */

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
      IPL_PRT_DEBUG_PRINTF("ERROR IPL_MTD_F_WriteNand (MTD BUSY)\n");
      return (INTLDR_ERROR_MTD_WRITE_FLASH);
   }

   /* Get NAND geometry informations */
   vRet = ioctl(h, MEMGETINFO, &mtd_info);
   if (vRet != 0)
   {
      IPL_PRT_DEBUG_PRINTF("ERROR IPL_MTD_F_WriteNand IOCTLMEMGETINFO vRet= 0x%08x\r\n", vRet);
      Cr = INTLDR_ERROR_MTD_WRITE_FLASH;
   }

   /* Check size */
   if((i_StartAddress + i_Size) > mtd_info.size)
   {   
      IPL_PRT_DEBUG_PRINTF("ERROR IPL_MTD_F_ReadNand Override NAND partition area last_dest 0x%x > total_size 0x%x\n",(i_StartAddress + i_Size), mtd_info.size);
      Cr = INTLDR_ERROR_MTD_WRITE_FLASH;
   }

   if (Cr == MCM_OK)
   {
      NbPagesPerBlock       = mtd_info.erasesize / mtd_info.writesize;
      CurrentSrc            = (uint32)i_pData;
      CurrentDst            = i_StartAddress;
      LastDst               = i_StartAddress + mtd_info.size;
      Size                  = i_Size;

      while ((Cr == MCM_OK) && (Size > 0) && (CurrentDst < LastDst))
      {
         /* Check block status in BBT */
         vRet = ioctl(h, MEMGETBADBLOCK, &CurrentDst);
         switch (vRet)
         {
            case 0:
               {
                  /* Block is okay, continue with programming */
                  break;
               }
            case 1:
               {
                  /* Block is marked as bad, continue with next block */
                  IPL_PRT_DEBUG_PRINTF("ERROR IPL_MTD_F_WriteNand (Bad block) @0x%08x \r\n",(unsigned)CurrentDst);
                  CurrentDst += mtd_info.erasesize;
                  continue;
               }
            default:
               {
                  /* Should never happen */
                  IPL_PRT_DEBUG_PRINTF("ERROR IPL_MTD_F_WriteNand (Unknown): %x\r\n", vRet);
                  Cr        = INTLDR_ERROR_MTD_WRITE_FLASH;
               }
         }

         if (Cr == MCM_OK)
         {
            /* Write all pages in block */
            for (page_index = 0;
                 (Cr == MCM_OK) && (Size >= mtd_info.writesize) && (page_index < NbPagesPerBlock);
                 page_index++)
            {
               vRet = lseek(h, (__off_t)CurrentDst, SEEK_SET);
               if(vRet<0)
               {
                  IPL_PRT_DEBUG_PRINTF("[IPL_TB_NAND_Write]MMC seek error\n");
                  Cr        =INTLDR_ERROR_MTD_WRITE_FLASH;
                  break;
               }
               else
               {
                  vRet = write(h, (uint32 *)CurrentSrc, mtd_info.writesize);
                  if(vRet<0) 
                  {
                     /* Block is now marked as bad Abort writing of this page and try again with the next block */
                     IPL_PRT_DEBUG_PRINTF("ERROR write: %d, errno = %d\r\n", vRet, errno);
                     IPL_PRT_DEBUG_PRINTF(" > Src  = %x (%d)\r\n", CurrentSrc, CurrentSrc);
                     IPL_PRT_DEBUG_PRINTF(" > Writesize  = %x (%d)\r\n",  mtd_info.writesize,  mtd_info.writesize);
                           
                     CurrentSrc  -= (page_index * mtd_info.writesize);
                     CurrentDst  -= (page_index * mtd_info.writesize);
                     CurrentDst  += mtd_info.erasesize;
                     Size        += (page_index * mtd_info.writesize);

                     /* Just to stop the inner loop */
                     page_index   = NbPagesPerBlock;

                     vRet = lseek(h, (__off_t) CurrentDst, SEEK_SET);
                     if(vRet<0)
                     {
                        IPL_PRT_DEBUG_PRINTF("ERROR IPL_MTD_F_WriteNand (SEEK)\n");
                        Cr        = INTLDR_ERROR_MTD_WRITE_FLASH;
                     }
                  }
                  else
                  {
                     if (vRet != mtd_info.writesize)
                     {
                        IPL_PRT_DEBUG_PRINTF("ERROR write: vRet != WriteSize\r\n");
                        IPL_PRT_DEBUG_PRINTF(" > CurrentSrc = %x (%d)\r\n", CurrentSrc, CurrentSrc);
                        IPL_PRT_DEBUG_PRINTF(" > vRet       = %x (%d)\r\n", vRet, vRet);
                        IPL_PRT_DEBUG_PRINTF(" > WriteSize  = %x (%d)\r\n", mtd_info.writesize, mtd_info.writesize);

                        Cr        = INTLDR_ERROR_MTD_WRITE_FLASH;
                     }
                     else
                     {
                        lastblockSize =  Size + (page_index * mtd_info.writesize);
                        CurrentSrc += vRet;
                        CurrentDst += vRet;
                        Size       -= vRet;

                        Percent =(uint32) ((uint64)(i_Size - Size) * (uint64)100 / (uint64)i_Size);
                        IPL_PRT_DEBUG_PRINTF("\b\b\b%.2ld%%",Percent);

                     }
                  }
               }
            }
             
            /* Copy the remaining data from RAM to NAND (last page) */
            if ((Cr == MCM_OK) && (Size > 0) && (Size < mtd_info.writesize) && (page_index < NbPagesPerBlock))
            {
               IPL_PRT_DEBUG_PRINTF("[IPL_TB_NAND_Write]Write last page size: %d, page number: %d \n",Size,page_index+1);
               pBuffer = malloc(mtd_info.writesize);
               if(pBuffer)
               {
                  memset(pBuffer, 0xFF, mtd_info.writesize);
                  memcpy(pBuffer, (void *)CurrentSrc, Size);
               }
               else
               {
                  IPL_PRT_DEBUG_PRINTF("ERROR write: Allocation error %d\r\n");
                  Cr = INTLDR_ERROR_MTD_WRITE_FLASH;               
               }

               if (Cr == MCM_OK)
               {
                  vRet = write(h, (uint32 *)pBuffer, mtd_info.writesize);
                  if(vRet != mtd_info.writesize) 
                  {
                     /* Block is now marked as bad Abort writing of this page and try again with the next block */
                     IPL_PRT_DEBUG_PRINTF("ERROR write: %d, errno = %d\r\n", vRet, errno);
                     IPL_PRT_DEBUG_PRINTF(" > Src  = %x (%d)\r\n", CurrentSrc, CurrentSrc);
                     IPL_PRT_DEBUG_PRINTF(" > Writesize  = %x (%d)\r\n",  mtd_info.writesize,  mtd_info.writesize);

                     CurrentSrc  -= (page_index * mtd_info.writesize);
                     CurrentDst  -= (page_index * mtd_info.writesize);
                     CurrentDst  += mtd_info.erasesize;
                     Size        += (page_index * mtd_info.writesize);

                     vRet = lseek(h, (__off_t)CurrentDst, SEEK_SET);
                     if(vRet<0)
                     {
                        IPL_PRT_DEBUG_PRINTF("ERROR IPL_MTD_F_WriteNand (SEEK)\n");
                        Cr        = INTLDR_ERROR_MTD_WRITE_FLASH;
                     }
                  }
                  else
                  {
                     lastblockSize =  Size + (page_index * mtd_info.writesize);
                     Size       = 0;
                     CurrentDst += vRet;
                     page_index++;
                     Percent =(uint32) ((uint64)(i_Size - Size) * (uint64)100 / (uint64)i_Size);
                     IPL_PRT_DEBUG_PRINTF("\b\b\b%.2ld%%",Percent);
                  }
               }
             
               if(pBuffer)
               {   
                  free(pBuffer); pBuffer = NULL;
               }
            }
         }
      }

      /*write last block*/
      if ((Cr == MCM_OK) && (Size == 0) && (page_index < NbPagesPerBlock) && (CurrentDst < LastDst))
      {

         for (;
              (Cr == MCM_OK) && (page_index < NbPagesPerBlock);
              page_index++)
         {
            IPL_PRT_DEBUG_PRINTF("[IPL_TB_NAND_Write]Write last block page %d/%d \n",page_index+1,NbPagesPerBlock);
            pBuffer = malloc(mtd_info.writesize);
            if(pBuffer)
            {
               memset(pBuffer, 0xFF, mtd_info.writesize);
            }
            else
            {
               IPL_PRT_DEBUG_PRINTF("ERROR write: Allocation error %d\r\n");
               Cr = INTLDR_ERROR_MTD_WRITE_FLASH;               
            }
            if (Cr == MCM_OK)
            {
               vRet = lseek(h, (__off_t)CurrentDst, SEEK_SET);
               if(vRet<0)
               {
                  IPL_PRT_DEBUG_PRINTF("[IPL_TB_NAND_Write]NAND seek error\n");
                  free(pBuffer); 
                  Cr        =INTLDR_ERROR_MTD_WRITE_FLASH;
                  break;
               }
               else
               {
                  vRet = write(h, (uint32 *)pBuffer, mtd_info.writesize);
                  if(vRet<0) 
                  {
                     /* Block is now marked as bad Abort writing of this page and try again with the next block */
                     IPL_PRT_DEBUG_PRINTF("ERROR write: %d, errno = %d\r\n", vRet, errno);
                     IPL_PRT_DEBUG_PRINTF(" > Src  = %x (%d)\r\n", CurrentSrc, CurrentSrc);
                     IPL_PRT_DEBUG_PRINTF(" > Writesize  = %x (%d)\r\n",  mtd_info.writesize,  mtd_info.writesize);
                           
                     CurrentSrc  -= (page_index * mtd_info.writesize);
                     CurrentDst  -= (page_index * mtd_info.writesize);
                     CurrentDst  += mtd_info.erasesize;
                     Size        = lastblockSize;

                     /* Just to stop the inner loop */
                     page_index   = NbPagesPerBlock;
                  }
                  else
                  {
                     if (vRet != mtd_info.writesize)
                     {
                        IPL_PRT_DEBUG_PRINTF("ERROR write: vRet != WriteSize\r\n");
                        IPL_PRT_DEBUG_PRINTF(" > vRet       = %x (%d)\r\n", vRet, vRet);
                        IPL_PRT_DEBUG_PRINTF(" > WriteSize  = %x (%d)\r\n", mtd_info.writesize, mtd_info.writesize);

                        Cr        = INTLDR_ERROR_MTD_WRITE_FLASH;
                     }
                     else
                     {
                        CurrentDst += vRet;
                     }
                  }
               }   
            }
            if(pBuffer)
            {   
               free(pBuffer); pBuffer = NULL;
            }
         }
      }/*END: write last block*/   
                   
      if (Size != 0)
      {
         /* We fall in this case when the NAND partition has too many bad blocks.
          * (so we don't have enough space left to flash the datas.  */
         Cr = INTLDR_ERROR_MTD_WRITE_FLASH;
      }
   }

   /* Close the mtd device */
   if(MCM_OK != close(h))
   {
      IPL_PRT_DEBUG_PRINTF("ERROR IPL_MTD_F_WriteNand MTD Close\r\n");
      Cr = INTLDR_ERROR_MTD_WRITE_FLASH;
   }

   return Cr;

}
#else

/**
 * @param   i_pData        Data buffer array.
 * @param   i_StartAddress Start address of the area to write to
 * @param   i_Size         Number of the datas to be write
 *
 * @return  MCM_OK                               if no error occured
 * @return  INTLDR_ERROR_MTD_TOO_BIG_FLASH       if datas to write don't fit in area
   @return  INTLDR_ERROR_MTD_TOO_MANY_BAD_BLOCKS if datas can't fit in partition because of bad blocks
 * @return  INTLDR_ERROR_MTD_WRITE_FLASH         otherwise
 *
 * @brief   Writes data to NAND flash.
 */
int32 IPL_TB_NAND_Write(IPL_TB_Device_t    i_DeviceType,
                        const uint8* const          i_pData,
                        const uint32                i_StartAddress,
                        const uint32                i_Size)
{
   int32                 vRet = MCM_OK;
   uint32                SectorSize  = -1;
   uint32                CurrentSrc  = -1;
   uint32                CurrentDst  = -1;
   uint32                Size         = -1;
   uint32                WriteSize   = -1;
   uint32                Percent     = -1;
   char Devpath[IPL_TB_INFOS_MAXCAR_PERLINE];

   CurrentSrc  = (uint32)i_pData;
   CurrentDst  = i_StartAddress;
   Size        = i_Size;
   SectorSize  = CONFIG_MMC_BLOCK_SIZE;

   int h;

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
         IPL_PRT_DEBUG_PRINTF("Write in partition %s , start @ 0x%x size: 0x%x \n",Devpath,i_StartAddress,i_Size);

   for(;;)
   {
         h = open(Devpath,O_RDWR);     
         if( -1 == h )   
         {
            vRet=MCM_ERROR;
            IPL_PRT_DEBUG_PRINTF("%s , MMC Busy\n",Devpath);
            break;
         }
         else
         {
            IPL_PRT_DEBUG_PRINTF("Writing NAND flash: 00%%");
         }
         while (Size > 0) /** breack if write operation is not successful*/
         {
            WriteSize = (Size > SectorSize) ? SectorSize : Size;

            vRet = lseek(h, CurrentDst, SEEK_SET);
            if(vRet<0)
            {
               IPL_PRT_DEBUG_PRINTF("MMC seek error\n");
               vRet=INTLDR_ERROR_MTD_WRITE_FLASH;
               break;
            }
            vRet = write(h, (uint8 *)CurrentSrc, WriteSize);
            if(vRet<0) 
            {
               IPL_PRT_DEBUG_PRINTF("ERROR write: %d, errno = %d\r\n", vRet, errno);
               IPL_PRT_DEBUG_PRINTF(" > Src  = %x (%d)\r\n", CurrentSrc, CurrentSrc);
               IPL_PRT_DEBUG_PRINTF(" > Dst  = %x (%d)\r\n", CurrentDst, CurrentDst);
               IPL_PRT_DEBUG_PRINTF(" > Size = %x (%d)\r\n", WriteSize, WriteSize);

               vRet=INTLDR_ERROR_MTD_WRITE_FLASH  ;
               break;
            }
            else
            {
               if (vRet == 0)
               {
                  IPL_PRT_DEBUG_PRINTF("ERROR write: vRet == 0\r\n");
                  vRet=INTLDR_ERROR_MTD_WRITE_FLASH;
                  break;
               }

               if (vRet != WriteSize)
               {
                  IPL_PRT_DEBUG_PRINTF("ERROR write: vRet != WriteSize\r\n");
                  IPL_PRT_DEBUG_PRINTF(" > CurrentSrc = %x (%d)\r\n", CurrentSrc, CurrentSrc);
                  IPL_PRT_DEBUG_PRINTF(" > CurrentDst = %x (%d)\r\n", CurrentDst, CurrentDst);
                  IPL_PRT_DEBUG_PRINTF(" > vRet       = %x (%d)\r\n", vRet, vRet);
                  IPL_PRT_DEBUG_PRINTF(" > WriteSize  = %x (%d)\r\n", WriteSize, WriteSize);
               }

               CurrentSrc += vRet;
               CurrentDst += vRet;
               Size       -= vRet;

               Percent =(uint32) ((uint64)(i_Size - Size) * (uint64)100 / (uint64)i_Size);
               IPL_PRT_DEBUG_PRINTF("\b\b\b%.2ld%%",Percent);
            }
         }

         IPL_PRT_DEBUG_PRINTF("\r\n");
         close(h);

         break;
    }

   return (vRet >= 0) ? MCM_OK : INTLDR_ERROR_MTD_WRITE_FLASH;

}
#endif
