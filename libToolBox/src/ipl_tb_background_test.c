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
#include "ipl_tb_l.h"
#ifdef CONFIG_VIACCESS_AUTHENTICATION
#include "ipl_sgn.h"
#endif

#ifdef CONFIG_IPL_SGN_NAGRA
#include "ipl_sgn_nagra_protected.h"
#endif

typedef enum {
   IPL_TB_BACKGROUND_READ,
   IPL_TB_BACKGROUND_WRITE,
   IPL_TB_BACKGROUND_ERASE,
   IPL_TB_BACKGROUND_MARK,
   IPL_TB_BACKGROUND_LAST,
} IPL_TB_BACKGROUND_operation_t;

static void f_IPL_TB_BACKGROUNDTest_print_usage(void)
{
   IPL_PRT_DEBUG_PRINTF("\n BACKGROUND cmd argument invalid\n");
   IPL_PRT_DEBUG_PRINTF("----------------------------------------------------------------\n");
   IPL_PRT_DEBUG_PRINTF(" Write operation: background write <filename> \n");
   IPL_PRT_DEBUG_PRINTF(" Read operation: background read  <filename_dump>\n");
   IPL_PRT_DEBUG_PRINTF(" Erase operation: background erase  \n");
   IPL_PRT_DEBUG_PRINTF("----------------------------------------------------------------\n");
}

static int f_IPL_TB_Background_read(char *filename)
{
   int32        Cr=MCM_ERROR;
   uint8    	*p_DataRead=NULL;
   FILE        *output_file=NULL;

   IPL_PRT_DEBUG_PRINTF("Background read operation... \n");
	if(!strcmp(CONFIG_BKGD_APP_FS_TYPE, "raw"))
   {
		uint8 *pDataRead;
		size_t fwSize;
		/* Get the header and check the CRC */
		pDataRead=malloc(sizeof (INTLDR_ModuleHeader_t));
		if(pDataRead)
		{
			memset(pDataRead, 0, sizeof (INTLDR_ModuleHeader_t));
		}
		else
		{
			IPL_PRT_DEBUG_PRINTF("[%s] cannot allocate memory\n", __FUNCTION__);
			return MCM_ERROR;
		}  

		/* Read Header */ 
      Cr = IPL_TB_ReadBlock(IPL_TB_BG_MODULE, pDataRead, CONFIG_MAPPING_BG, sizeof (INTLDR_ModuleHeader_t));
		if(MCM_OK != Cr)
		{
			IPL_PRT_DEBUG_PRINTF ("[%s] NAND Read Failed !\n", __FUNCTION__);
			/* free buffer */
			free(pDataRead);			
			return MCM_ERROR;
		}
		else
		{
			/* checks the header consistency CRC32_Header */
			uint32 Crc32 = 0;
			uint32 ComputedCrc32 = 0;
			ComputedCrc32 = INTLDR_ComputeCheck32 ((uint8 *) pDataRead,CRC_32_HEADER_OFFSET);
			/* Read CRC from header */
			Crc32 = pDataRead[CRC_32_HEADER_OFFSET] << 24;
			Crc32 |= pDataRead[CRC_32_HEADER_OFFSET+1] << 16;
			Crc32 |= pDataRead[CRC_32_HEADER_OFFSET+2] << 8;
			Crc32 |= pDataRead[CRC_32_HEADER_OFFSET+3];

			if(ComputedCrc32 != Crc32)
			{
				IPL_PRT_DEBUG_PRINTF("\n[%s] CRC_Header is invalid = %x != %x\n", __FUNCTION__,ComputedCrc32, Crc32);
				/* free buffer */
				free(pDataRead);				
				return INTLDR_ERROR_HEADER_INVALID;
			}
			else 
			{
				IPL_PRT_DEBUG_PRINTF("\n[%s] CRC_Header is valid\n", __FUNCTION__);
			}
		}		
		/* Read firmware size */       
		fwSize = pDataRead[0] << 24;
		fwSize |= pDataRead[1] << 16;
		fwSize |= pDataRead[2] << 8;
		fwSize |= pDataRead[3];
		IPL_PRT_DEBUG_PRINTF ("[%s] Module size = 0x%08X\n", __FUNCTION__, fwSize);

		/*
		 *  checks the wrap header consistency Size. 
		 * Add the signature size
		 */
		#if defined CONFIG_VIACCESS_AUTHENTICATION
			fwSize = fwSize + IPL_SGN_VIACCESS_SIGNATURE_SIZE;
	    #elif defined CONFIG_NAGRA_NASC_1x
			fwSize = fwSize + NAGRA_NASC_1x_SSD_SIGNATURE_SIZE;
	    #elif defined CONFIG_NAGRA_NASC_3x
			fwSize = fwSize + NAGRA_NASC_3x_WRAP_SSV_SIGNATURE_SIZE;
		#else 
			fwSize = fwSize + SAGEMCOM_SIGNATURE_SIZE;
		#endif	

		if (fwSize > CONFIG_MAPPING_BG_SIZE)
		{
			IPL_PRT_DEBUG_PRINTF("[%s] Invalid wrap size: %x > %x\n",__FUNCTION__, fwSize ,CONFIG_MAPPING_BG_SIZE);
			//free buffer
			free(pDataRead);				
			return INTLDR_ERROR_CONSISTENCY_CHECK;
		}
		//free buffer
		free(pDataRead);

      p_DataRead = malloc(fwSize);
      if(p_DataRead != NULL)
      { 
         Cr=IPL_TB_ReadBlock(IPL_TB_BG_MODULE, p_DataRead, CONFIG_MAPPING_BG, fwSize);
      }
      else
      {
         IPL_PRT_DEBUG_PRINTF("\r\nmalloc failed : %d\r\n",fwSize);
			return MCM_ERROR;
      }

      if(MCM_OK == Cr)
      {
         /* Copy the download file to usb key */
         output_file = fopen(filename, "wb");
         if (output_file == NULL)
         {
            IPL_PRT_DEBUG_PRINTF("Impossible to write to %s\r\n",filename);
         }
         else
         {
            IPL_PRT_DEBUG_PRINTF("Writing dump file...");
            if ( 1 == fwrite(p_DataRead, fwSize, 1, output_file) )
            {
               IPL_PRT_DEBUG_PRINTF("done\n");
            }
            else
            {
               IPL_PRT_DEBUG_PRINTF("failed\n");
            }
            fclose(output_file);            
         }
      }
      else
      {
         IPL_PRT_DEBUG_PRINTF("Read Background area failed !\n");
      }
      free(p_DataRead);
      return MCM_OK;
   }
   else /* CONFIG_BKGD_APP_FS_TYPE !="raw" */
   {
      IPL_PRT_DEBUG_PRINTF("Copy file Name from file system not implemented\n");
      return MCM_ERROR;
   }
}

static void f_IPL_TB_Background_write(char            *filename)
{
   FILE			*input_file = NULL;
   uint32 		i_DataReadSize = 0 ;
   uint32 		WriteSize = 0 ;
   uint8  		*p_DataRead=NULL;
   IPL_PRT_DEBUG_PRINTF("Background write operation... \n");

	if(!strcmp(CONFIG_BKGD_APP_FS_TYPE, "raw"))
	{
      input_file = fopen(filename, "rb");
      if (input_file == NULL)
      {
         IPL_PRT_DEBUG_PRINTF ("Impossible to access to %s\r\n",filename);
      }
      else
      {
         fseek(input_file, 0, SEEK_END);
         i_DataReadSize += ftell(input_file);
         fseek(input_file, 0, SEEK_SET);
         p_DataRead = malloc(i_DataReadSize);
         if (p_DataRead != NULL)
         {
            if(1 != fread (p_DataRead,i_DataReadSize,1, input_file)){
               IPL_PRT_DEBUG_PRINTF("[f_IPL_TB_Background_write] Impossible to read all module \r\n");}

            WriteSize = (i_DataReadSize > CONFIG_MAPPING_BG_SIZE) ? CONFIG_MAPPING_BG_SIZE : i_DataReadSize;
            IPL_TB_WriteBlock(p_DataRead, WriteSize, IPL_TB_BG_MODULE, CONFIG_MAPPING_BG, CONFIG_MAPPING_BG_SIZE);
            free(p_DataRead);
         }
         fclose(input_file);
      }
   }
   else /* CONFIG_BKGD_APP_FS_TYPE !="raw" */
   {
      IPL_PRT_DEBUG_PRINTF("Write file into file system not implemented\n");
   }
}

static void f_IPL_TB_Background_erase(void)
{
	if(!strcmp(CONFIG_BKGD_APP_FS_TYPE, "raw"))
	{
      IPL_PRT_DEBUG_PRINTF("Background area erase operation... \n");
      if (MCM_OK ==  IPL_TB_NAND_Erase(IPL_TB_BG_MODULE,CONFIG_MAPPING_BG,CONFIG_MAPPING_BG_SIZE)){
      IPL_PRT_DEBUG_PRINTF("Background area erase operation OK \n");
      }else
      IPL_PRT_DEBUG_PRINTF("Background erase operation ERROR\n");
   }
   else
   {
      IPL_PRT_DEBUG_PRINTF("Remove file Name from file system not implemented\n");
   }
}

int32 m_IPL_TB_background
(
   int32 wz_mbfield,
   char *pb_argv
)
{
   IPL_TB_BACKGROUND_operation_t action = IPL_TB_BACKGROUND_LAST;
   char  *p_extractarg=NULL;
   char  *p_extractfilename=NULL;

   if (pb_argv == NULL)
   {
      return -1;
   }

   /* Get BACKGROUND operation type*/
   p_extractarg = strtok_r(pb_argv, " ", &pb_argv);
   if ((p_extractarg == NULL) || (pb_argv == NULL))
   {
      f_IPL_TB_BACKGROUNDTest_print_usage();
      return MCM_ERROR;
   }
   IPL_PRT_DEBUG_PRINTF("BACKGROUND operation is    :%s \n",p_extractarg);

   if(!strcmp(p_extractarg,"read"))
      action = IPL_TB_BACKGROUND_READ;
   else if(!strcmp(p_extractarg,"write"))
      action = IPL_TB_BACKGROUND_WRITE;
   else if(!strcmp(p_extractarg,"erase"))
      action = IPL_TB_BACKGROUND_ERASE;
   else
   {
      f_IPL_TB_BACKGROUNDTest_print_usage();
      return MCM_ERROR;
   }

   switch (action)
   {
      case IPL_TB_BACKGROUND_READ:
        {
           p_extractfilename = strtok_r(NULL, " ", &pb_argv);
           if ((p_extractfilename == NULL) || (pb_argv == NULL))
           {
              IPL_PRT_DEBUG_PRINTF("\n Invalid filename \n");
              f_IPL_TB_BACKGROUNDTest_print_usage();
              return MCM_ERROR;
           }
           IPL_PRT_DEBUG_PRINTF(" File Name: %s\n",p_extractfilename);
           f_IPL_TB_Background_read(p_extractfilename);
        }
       break;
      case IPL_TB_BACKGROUND_WRITE:
       {
           p_extractfilename = strtok_r(NULL, " ", &pb_argv);
           if ((p_extractfilename == NULL) || (pb_argv == NULL))
           {
              IPL_PRT_DEBUG_PRINTF("\n Invalid filename \n");
              f_IPL_TB_BACKGROUNDTest_print_usage();
              return MCM_ERROR;
           }
           IPL_PRT_DEBUG_PRINTF(" File Name: %s\n",p_extractfilename);
           f_IPL_TB_Background_write(p_extractfilename);
        }
       break;
      case IPL_TB_BACKGROUND_ERASE:
       {
          f_IPL_TB_Background_erase();
       }
       break;
      default :
       break;
   }

   return MCM_OK;
}

