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

typedef enum {
   IPL_TB_NAND_READ,
   IPL_TB_NAND_WRITE,
   IPL_TB_NAND_ERASE,
   IPL_TB_NAND_MARK,
   IPL_TB_NAND_LAST,
} IPL_TB_NAND_operation_t;

static void f_IPL_TB_NandTest_print_usage(void)
{
   IPL_PRT_DEBUG_PRINTF("\n NAND cmd argument invalid\n");
   IPL_PRT_DEBUG_PRINTF("------------------------------NAND flash access----------------------------\n");
   IPL_PRT_DEBUG_PRINTF(" Write operation: nand write 0x100000 0x100000 /tmp/toto.bin\n");
   IPL_PRT_DEBUG_PRINTF(" Read operation: nand read 0x100000 0x100000 /tmp/dump_toto.bin\n");
   IPL_PRT_DEBUG_PRINTF(" Erase operation: nand erase 0x100000 0x100000 \n");
   IPL_PRT_DEBUG_PRINTF(" Mark bad block: nand mark 0x100000 \n"); 
   IPL_PRT_DEBUG_PRINTF("------------------------------NAND flash access----------------------------\n");
}

static void f_IPL_TB_NandTest_read(const uint32     i_StartAddress,
                                   const uint32     i_Size,
                                   char            *filename)
{
   int32        Cr=MCM_ERROR;
   uint8    	*p_DataRead=NULL;
   FILE        *output_file=NULL;

   IPL_PRT_DEBUG_PRINTF("NAND read operation... \n");

   p_DataRead = malloc(i_Size);
   if(p_DataRead != NULL)
   { 
      Cr=IPL_TB_ReadBlock(IPL_TB_NAND_DEVICE, p_DataRead, i_StartAddress, i_Size);
   }
   else
      IPL_PRT_DEBUG_PRINTF("\r\nmalloc failed : %d\r\n",i_Size);

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
         if ( 1 == fwrite(p_DataRead, i_Size, 1, output_file) )
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
      IPL_PRT_DEBUG_PRINTF("Read NAND device failed !\n");
   }

   free(p_DataRead);
}

static void f_IPL_TB_NandTest_write(const uint32     i_StartAddress,
                                   const uint32     i_Size,
                                   char            *filename)
{
   FILE			*input_file = NULL;
   uint32 		i_DataReadSize = 0 ;
   uint32 		WriteSize = 0 ;
   uint8  		*p_DataRead=NULL;
   IPL_PRT_DEBUG_PRINTF("NAND write operation... \n");

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
            IPL_PRT_DEBUG_PRINTF("[f_IPL_TB_NandTest_write] Impossible to read all module \r\n");}

         WriteSize = (i_DataReadSize > i_Size) ? i_Size : i_DataReadSize;
         IPL_TB_WriteBlock(p_DataRead, WriteSize, IPL_TB_NAND_DEVICE, i_StartAddress, i_Size);
         free(p_DataRead);
      }
      fclose(input_file);
   }
}

static void f_IPL_TB_NandTest_erase(const uint32     i_StartAddress,
                                    const uint32     i_Size)
{
   IPL_PRT_DEBUG_PRINTF("NAND erase operation... \n");
   if (MCM_OK ==  IPL_TB_NAND_Erase(IPL_TB_NAND_DEVICE,i_StartAddress,i_Size)){
   IPL_PRT_DEBUG_PRINTF("NAND erase operation OK \n");
   }else
   IPL_PRT_DEBUG_PRINTF("NAND erase operation ERROR\n");
}
#if defined(CONFIG_IPL_MTD_NAND_SC)
static int f_IPL_TB_NandTest_mark(const uint32     i_StartAddress)
{
   struct mtd_info_user meminfo;
   IPL_TB_M_Ctx_t        *pCtx    = &IPL_TB_M_Ctx;
   int h;
   int nb_bloc=0;

   IPL_PRT_DEBUG_PRINTF("NAND mark bad block operation... \n");

   /* open mtd-nand device */
   IPL_PRT_DEBUG_PRINTF("IPL_MTD_F_ReadNand OPEN %s\n",pCtx->mtd_nand_strDevPath);
   h = open(pCtx->mtd_nand_strDevPath,O_RDWR);
   if(h<0)
   {
      IPL_PRT_DEBUG_PRINTF("ERROR IPL_MTD_F_ReadNand (MTD BUSY)\n");
      return (INTLDR_ERROR_MTD_WRITE_FLASH);
   }

	if (ioctl(h, MEMGETINFO, &meminfo)) {
      IPL_PRT_DEBUG_PRINTF("[f_IPL_TB_NandTest_mark]MEMGETINFO failled \n");
		close(h);
		return 0;
	}

   nb_bloc = i_StartAddress / meminfo.erasesize;

   IPL_PRT_DEBUG_PRINTF("Try to mark block number %d @0x%x \n",nb_bloc,i_StartAddress);

	loff_t test_ofs = nb_bloc*meminfo.erasesize;
	
	if (ioctl(h, MEMGETBADBLOCK, &test_ofs)) {
		IPL_PRT_DEBUG_PRINTF("\rBad block at 0x%08x\n", (unsigned)test_ofs);
		close(h);
		return 0;
	}
	else
	{
		IPL_PRT_DEBUG_PRINTF("\rNo Bad block at 0x%08x\n", (unsigned)test_ofs);
	}

	test_ofs = nb_bloc*meminfo.erasesize;
	ioctl(h, MEMSETBADBLOCK, &test_ofs);


	test_ofs = nb_bloc*meminfo.erasesize;
	if (ioctl(h, MEMGETBADBLOCK, &test_ofs)) {
		IPL_PRT_DEBUG_PRINTF("\rBad block have been marked at 0x%08x\n", (unsigned)test_ofs);
	}
	else
	{
		IPL_PRT_DEBUG_PRINTF("\rNo Bad block have been marked at 0x%08x\n", (unsigned)test_ofs);
	}

   close(h);
	return 0;
}
#else
static void f_IPL_TB_NandTest_mark(const uint32     i_StartAddress)
{
   IPL_PRT_DEBUG_PRINTF("NAND mark bad block operation not implemented with EMMC \n");
}
#endif

int32 m_IPL_TB_NandTest
(
   int32 wz_mbfield,
   char *pb_argv
)
{
   IPL_TB_M_Ctx_t *pCtx    = &IPL_TB_M_Ctx;
   IPL_TB_NAND_operation_t action = IPL_TB_NAND_LAST;
   char  *p_extractarg=NULL;
   uint32 address=0;
   uint32 size=0;
   char  *p_extractfilename=NULL;

   if (pb_argv == NULL)
   {
      return -1;
   }

   if(pCtx->FullNand_available==0)
   {
      IPL_PRT_DEBUG_PRINTF("\n Full NAND partion not found!\n");
      return -1;
   }

   /* Get NAND operation type*/
   p_extractarg = strtok_r(pb_argv, " ", &pb_argv);
   if ((p_extractarg == NULL) || (pb_argv == NULL))
   {
      f_IPL_TB_NandTest_print_usage();
      return MCM_ERROR;
   }
   IPL_PRT_DEBUG_PRINTF("NAND operation is    :%s \n",p_extractarg);

   if(!strcmp(p_extractarg,"read"))
      action = IPL_TB_NAND_READ;
   else if(!strcmp(p_extractarg,"write"))
      action = IPL_TB_NAND_WRITE;
   else if(!strcmp(p_extractarg,"erase"))
      action = IPL_TB_NAND_ERASE;
   else if(!strcmp(p_extractarg,"mark"))
      action = IPL_TB_NAND_MARK;
   else
   {
      f_IPL_TB_NandTest_print_usage();
      return MCM_ERROR;
   }

   p_extractarg = strtok_r(NULL, " ", &pb_argv);
   if ((p_extractarg == NULL) || (pb_argv == NULL))
   {
      IPL_PRT_DEBUG_PRINTF("\n Invalid NAND address\n");
      f_IPL_TB_NandTest_print_usage();
      return -1;
   }
   address = (uint32)strtol(p_extractarg, NULL, 0);
   IPL_PRT_DEBUG_PRINTF("NAND offset is       :0x%x \n",address);


   switch (action)
   {
      case IPL_TB_NAND_READ:
        {
           p_extractarg = strtok_r(NULL, " ", &pb_argv);
           if ((p_extractarg == NULL) || (pb_argv == NULL))
           {
              IPL_PRT_DEBUG_PRINTF("\n Invalid size \n");
              f_IPL_TB_NandTest_print_usage();
              return MCM_ERROR;
           }
           size = (uint32)strtol(p_extractarg, NULL, 0);
           IPL_PRT_DEBUG_PRINTF("Size is :0x%x \n",size);

           p_extractfilename = strtok_r(NULL, " ", &pb_argv);
           if ((p_extractfilename == NULL) || (pb_argv == NULL))
           {
              IPL_PRT_DEBUG_PRINTF("\n Invalid filename \n");
              f_IPL_TB_NandTest_print_usage();
              return MCM_ERROR;
           }
           IPL_PRT_DEBUG_PRINTF(" File Name: %s\n",p_extractfilename);
           if (size < 0x40000000)
           {
              f_IPL_TB_NandTest_read(address,size,p_extractfilename);
           }else {IPL_PRT_DEBUG_PRINTF("Can not allocate size > 1 Go (0x%x) \n",size);}
        }
       break;
      case IPL_TB_NAND_WRITE:
       {
           p_extractarg = strtok_r(NULL, " ", &pb_argv);
           if ((p_extractarg == NULL) || (pb_argv == NULL))
           {
              IPL_PRT_DEBUG_PRINTF("\n Invalid size \n");
              f_IPL_TB_NandTest_print_usage();
              return MCM_ERROR;
           }
           size = (uint32)strtol(p_extractarg, NULL, 0);
           IPL_PRT_DEBUG_PRINTF("Max Size to write is :0x%x \n",size);

           p_extractfilename = strtok_r(NULL, " ", &pb_argv);
           if ((p_extractfilename == NULL) || (pb_argv == NULL))
           {
              IPL_PRT_DEBUG_PRINTF("\n Invalid filename \n");
              f_IPL_TB_NandTest_print_usage();
              return MCM_ERROR;
           }
           IPL_PRT_DEBUG_PRINTF(" File Name: %s\n",p_extractfilename);
           if ((size < 0x40000000) && (address < 0xFFFFFFFF))
           {
              f_IPL_TB_NandTest_write(address,size,p_extractfilename);
           }else {IPL_PRT_DEBUG_PRINTF("Can not allocate size > 1 Go (0x%x) or access requested address (0x%x) \n",size,address);}
        }
       break;
      case IPL_TB_NAND_ERASE:
       {
           p_extractarg = strtok_r(NULL, " ", &pb_argv);
           if ((p_extractarg == NULL) || (pb_argv == NULL))
           {
              IPL_PRT_DEBUG_PRINTF("\n Invalid size \n");
              f_IPL_TB_NandTest_print_usage();
              return MCM_ERROR;
           }
           size = (uint32)strtol(p_extractarg, NULL, 0);
           IPL_PRT_DEBUG_PRINTF("Size is :0x%x \n",size);
           f_IPL_TB_NandTest_erase(address,size);
        }
       break;
      case IPL_TB_NAND_MARK:
           f_IPL_TB_NandTest_mark(address);
       break;
      default :
       break;
   }

   return MCM_OK;
} /* End of function 'm_IPL_TB_NandTest()' */

