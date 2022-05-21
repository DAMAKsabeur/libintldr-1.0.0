/**
 * @param   o_pData                       Data buffer array.
 * @param   i_StartAddress                Start address of the area to read from
 * @param   i_Size                        Number of the datas to be read
 *
 * @return  MCM_OK                        if no error occured
 * @return  INTLDR_ERROR_MTD_READ_FLASH   if an error occured
 *
 * @brief   Reads data from NOR flash.
 */
#include "ipl_tb_internal.h"

#if defined CONFIG_IPL_MTD_NOR 
int32 IPL_TB_NOR_Read(	uint8* const                o_pData,
                      	const uint32                i_StartAddress,
                      	const uint32                i_Size)
{        
   int h;   
   int32                 vRet = MCM_OK;
   IPL_TB_M_Ctx_t        *pCtx    = &IPL_TB_M_Ctx;
   uint32                SectorSize  = -1;
   uint32                CurrentSrc  = (uint32)o_pData;
   uint32                CurrentDst  = i_StartAddress;
   uint32                Size         = i_Size;
   uint32                ReadSize   = -1;
   uint32                Percent    = -1;

   if (pCtx->mtd_available == 1)
   {            
      SectorSize = CONFIG_NOR_BLOCK_SIZE;

      for(;;)
      {
         h = open(pCtx->mtd_strDevPath,O_RDONLY);          if (h<0)  {break;}
         IPL_PRT_DEBUG_PRINTF("Reading NOR flash: 00%%");

         while (Size > 0) /** break if write operation is not successful*/
         {
            ReadSize = (Size > SectorSize) ? SectorSize : Size;
            vRet = lseek(h, CurrentDst, SEEK_SET);                   if(vRet<0) {break;}
            vRet = read  (h, (uint8 *)CurrentSrc, ReadSize);    
            if(vRet<0) 
            {
               break;
            }
            else
            {
               CurrentSrc += ReadSize;
               CurrentDst += ReadSize;
               Size       -= ReadSize;

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
   return (vRet == MCM_OK) ? MCM_OK : INTLDR_ERROR_MTD_READ_FLASH;
}
#endif

#if defined(CONFIG_IPL_MTD_NAND_SC)
/**
 * @param   o_pData                       Data buffer array.
 * @param   i_StartAddress                Start address of the area to read from
 * @param   i_Size                        Number of the datas to be read
 * @param   i_MaxSize                     Maximum size of area
 *
 * @return  MCM_OK                        if no error occured
 * @return  INTLDR_ERROR_MTD_READ_FLASH   if an error occured
 *
 * @brief   Reads data from NAND flash.
 */
int32 IPL_TB_NAND_Read(    IPL_TB_Device_t    i_DeviceType,
                           uint8* const 	    o_pData,
                           const uint32       i_StartAddress,
                           const uint32       i_Size)
{
   mtd_info_t mtd_info;
   uint32   NbPagesPerBlock 	= -1;
   uint64 	CurrentSrc 			= -1;
   uint32 	CurrentDst 			= -1;
   uint32 	Size 					= -1;
   uint32 	i 						= -1;
   int 	 	Cr 					= MCM_ERROR;
   int32    vRet              = -1;
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
      case IPL_TB_DOWNLOAD_MODULE2:
         strcpy(Devname, CONFIG_IPL_MTD_DOWNLOADER2_DEVICE_NAME);
        break;
      case IPL_TB_BG_MODULE:
         strcpy(Devname, CONFIG_IPL_MTD_BG_DEVICE_NAME);
        break;
      case IPL_TB_KERNEL_2_MODULE:
         strcpy(Devname, CONFIG_IPL_MTD_KERNEL_2_DEVICE_NAME);
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

   /* open mtd-nand device */
   h = open(Mtdname,O_RDONLY);
   if(h<0)
   {
      IPL_PRT_DEBUG_PRINTF("ERROR IPL_MTD_F_ReadNand (MTD BUSY)\n");
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
      NbPagesPerBlock 	= mtd_info.erasesize / mtd_info.writesize;
      CurrentSrc 		 	= i_StartAddress;
      CurrentDst 		 	= (uint32)o_pData;
      Size 			      = i_Size;

      /* Read datas from NAND */
      while ((Cr == MCM_OK) && (Size > 0))
      {

         /* Check block status in BBT */
         vRet = ioctl(h, MEMGETBADBLOCK, &CurrentSrc); 
         switch (vRet)
         {
            case 0:
               {
                  /* Block is okay, continue with reading */
                  break;
               }
            case 1:
               {
                  /* Block is marked as bad, continue with next block */
                  IPL_PRT_DEBUG_PRINTF("ERROR IPL_MTD_F_ReadNand (Bad block) @0x%08x \r\n",(unsigned)CurrentSrc);
                  CurrentSrc  += mtd_info.erasesize;
                  continue;
               }
            default:
               {
                  /* Should never happen */
                  IPL_PRT_DEBUG_PRINTF("ERROR IPL_MTD_F_ReadNand (Unknown) %x \r\n", vRet);
                  Cr        = INTLDR_ERROR_MTD_READ_FLASH;
               }
         }

         for (i = 0;
              (Cr == MCM_OK) && (i < NbPagesPerBlock);
              i++)
         {
            uint32_t size_l = (Size > mtd_info.writesize) ? mtd_info.writesize : Size;

            vRet = lseek (h, (__off_t)CurrentSrc, SEEK_SET);
            if(vRet<0)
            {
               IPL_PRT_DEBUG_PRINTF("MMC seek error: @ %0x\n",CurrentSrc);
               Cr        = INTLDR_ERROR_MTD_READ_FLASH;
            }
            else
            {

               vRet = read(h, (uint32 *)CurrentDst, size_l);
               if(vRet <0)
               {
                  /* Datas are corrupted, and unrecoverable Go out of function  */
                  IPL_PRT_DEBUG_PRINTF("ERROR IPL_MTD_F_ReadNand @0x%08x (Uncorrectable) %x\r\n",(unsigned)CurrentSrc, vRet);
                  Cr        = INTLDR_ERROR_MTD_READ_FLASH;
               }
               else if(vRet == size_l)
               {
                  /* Update pointers and Size */
                  CurrentSrc 	+= size_l;
                  Size 			-= size_l;
                  CurrentDst 	+= size_l;
                  continue;
               }
               else
               {
                  /* Datas are corrupted, and unrecoverable Go out of function  */
                  IPL_PRT_DEBUG_PRINTF("ERROR IPL_MTD_F_ReadNand @0x%08x (Uncorrectable) %x\r\n",(unsigned)CurrentSrc, vRet);
                  Cr        = INTLDR_ERROR_MTD_READ_FLASH;
               }
            }
         }
      }
   }

   Cr = close(h); /* Close the mtd device */
   if(MCM_OK != Cr)
   {
      IPL_PRT_DEBUG_PRINTF("ERROR IPL_MTD_F_EraseNand MTD Close\r\n");
   }

   return Cr;
}
#else

/**
 * @param   o_pData                       Data buffer array.
 * @param   i_StartAddress                Start address of the area to read from
 * @param   i_Size                        Number of the datas to be read
 * @param   i_MaxSize                     Maximum size of area
 *
 * @return  MCM_OK                        if no error occured
 * @return  INTLDR_ERROR_MTD_READ_FLASH   if an error occured
 *
 * @brief   Reads data from NAND flash.
 */
int32 IPL_TB_NAND_Read(     IPL_TB_Device_t     i_DeviceType,
                            uint8* const 	    	o_pData,
                            const uint32       	i_StartAddress,
                            const uint32       	i_Size)
{
   int                   h;
   int32                 vRet = MCM_OK;
   uint32                SectorSize  = -1;
   uint32                CurrentSrc  = (uint32)o_pData;
   uint32                CurrentDst  = i_StartAddress;
   uint32                Size         = i_Size;
   uint32                ReadSize   = -1;
   uint32                Percent    = -1;
   char Devpath[IPL_TB_INFOS_MAXCAR_PERLINE];

   SectorSize = CONFIG_MMC_BLOCK_SIZE;

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

   for(;;)
   {
      h    = open  (Devpath, O_RDONLY);
      if( -1 == h )   
      {
         vRet=MCM_ERROR;
         IPL_PRT_DEBUG_PRINTF("%s , MMC Busy\n",Devpath);
         break;
      }
      else
      {
         IPL_PRT_DEBUG_PRINTF("Reading NAND flash: 00%%");
      }

      while (Size > 0) /** break if write operation is not successful*/
      {
         ReadSize = (Size > SectorSize) ? SectorSize : Size;
         vRet = lseek(h, CurrentDst, SEEK_SET);                   if(vRet<0) {break;}
         vRet = read  (h, (uint8 *)CurrentSrc, ReadSize);    
         if(vRet<0) 
         {
            break;
         }
         else
         {
            CurrentSrc += ReadSize;
            CurrentDst += ReadSize;
            Size       -= ReadSize;

            Percent = (uint32)((uint64)(i_Size - Size) * (uint64)100 / (uint64)i_Size);
            IPL_PRT_DEBUG_PRINTF("\b\b\b%.2ld%%",Percent);
         }
      }

      IPL_PRT_DEBUG_PRINTF("\r\n");
      vRet = close (h); 
      break;
   }

   return (vRet >= 0) ? MCM_OK : INTLDR_ERROR_MTD_READ_FLASH;
}
#endif
