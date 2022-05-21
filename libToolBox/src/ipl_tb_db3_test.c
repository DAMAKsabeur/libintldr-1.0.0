#ifdef CONFIG_IPL_MTD_NOR 

#include "ipl_tb_internal.h"
#include "ipl_tb_l.h"

typedef enum {
   IPL_TB_DB3_READ,
   IPL_TB_DB3_WRITE,
   IPL_TB_DB3_ERASE,
   IPL_TB_DB3_LAST,
} IPL_TB_DB3_operation_t;

static void f_IPL_TB_DB3Test_print_usage(void)
{
   IPL_PRT_DEBUG_PRINTF("\n DB3 cmd argument invalid\n");
   IPL_PRT_DEBUG_PRINTF("----------------------------------------------------------------\n");
   IPL_PRT_DEBUG_PRINTF(" Write operation: DB3 write <filename> \n");
   IPL_PRT_DEBUG_PRINTF(" Read operation: DB3 read  <filename_dump>\n");
   IPL_PRT_DEBUG_PRINTF(" Erase operation: DB3 erase  \n");
   IPL_PRT_DEBUG_PRINTF("----------------------------------------------------------------\n");
}

static int f_IPL_TB_DB3_read(char *filename)
{
   int32        Cr=MCM_ERROR;
   uint8    	*p_DataRead=NULL;
   FILE        *output_file=NULL;

   IPL_PRT_DEBUG_PRINTF("DB3 Primary read operation... \n");

   p_DataRead = malloc(CONFIG_MAPPING_DB3_SIZE);
   if(p_DataRead != NULL)
   { 
      Cr=IPL_TB_ReadBlock(IPL_TB_NOR_DEVICE, p_DataRead, CONFIG_MAPPING_DB3_PRIMARY, CONFIG_MAPPING_DB3_SIZE);
   }
   else
   {
      IPL_PRT_DEBUG_PRINTF("\r\nmalloc failed : %d\r\n",CONFIG_MAPPING_DB3_SIZE);
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
         if ( 1 == fwrite(p_DataRead, CONFIG_MAPPING_DB3_SIZE, 1, output_file) )
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
      IPL_PRT_DEBUG_PRINTF("Read DB3 area failed !\n");
   }
   free(p_DataRead);
   return MCM_OK;
}

static void f_IPL_TB_DB3_write(char *filename)
{
   FILE			*input_file = NULL;
   uint32 		i_DataReadSize = 0 ;
   uint32 		WriteSize = 0 ;
   uint8  		*p_DataRead=NULL;
   IPL_PRT_DEBUG_PRINTF("DB3 write operation... \n");
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
         if(1 != fread (p_DataRead,i_DataReadSize,1, input_file))
         {
            IPL_PRT_DEBUG_PRINTF("[f_IPL_TB_DB3_write] Impossible to read all module \r\n");
         }
         else
         {
            WriteSize = (i_DataReadSize > CONFIG_MAPPING_DB3_SIZE) ? CONFIG_MAPPING_DB3_SIZE : i_DataReadSize;

            IPL_PRT_DEBUG_PRINTF("DB3 Primary area write operation... \n");
            if (MCM_OK != IPL_TB_WriteBlock(p_DataRead, WriteSize, IPL_TB_NOR_DEVICE, CONFIG_MAPPING_DB3_PRIMARY, CONFIG_MAPPING_DB3_SIZE))
            {
               IPL_PRT_DEBUG_PRINTF("DB3 Primary area write operation failed ! \n");
            } 
            else{IPL_PRT_DEBUG_PRINTF("DB3 Primary area write operation OK \n");}

            IPL_PRT_DEBUG_PRINTF("DB3 Redundant area write operation... \n");
            if (MCM_OK != IPL_TB_WriteBlock(p_DataRead, WriteSize, IPL_TB_NOR_DEVICE, CONFIG_MAPPING_DB3_REDUNDANT, CONFIG_MAPPING_DB3_SIZE))
            {
               IPL_PRT_DEBUG_PRINTF("DB3 Redundant area write operation failed ! \n");
            }
            else{ IPL_PRT_DEBUG_PRINTF("DB3 Redundant area write operation OK \n");}
         }
         free(p_DataRead);
      }
      fclose(input_file);
   }
}

static void f_IPL_TB_DB3_erase(void)
{	
   IPL_PRT_DEBUG_PRINTF("DB3 Primary area erase operation... \n");
#if defined (CONFIG_IPL_MTD_NOR)
   if (MCM_OK != IPL_TB_NOR_Erase(CONFIG_MAPPING_DB3_PRIMARY, CONFIG_MAPPING_DB3_SIZE))
#else
   if (MCM_OK != IPL_TB_NAND_Erase(IPL_TB_NAND_DEVICE, CONFIG_MAPPING_DB3_PRIMARY, CONFIG_MAPPING_DB3_SIZE))
#endif
   {
   IPL_PRT_DEBUG_PRINTF("DB3 Primary area erase operation failed ! \n");
   }else{
   IPL_PRT_DEBUG_PRINTF("DB3 Primary area erase operation OK \n");}

   IPL_PRT_DEBUG_PRINTF("DB3 Redundant area erase operation... \n");
#if defined (CONFIG_IPL_MTD_NOR)
   if (MCM_OK != IPL_TB_NOR_Erase(CONFIG_MAPPING_DB3_REDUNDANT, CONFIG_MAPPING_DB3_SIZE))
#else
   if (MCM_OK != IPL_TB_NAND_Erase(IPL_TB_NAND_DEVICE, CONFIG_MAPPING_DB3_REDUNDANT, CONFIG_MAPPING_DB3_SIZE))
#endif
   {
   IPL_PRT_DEBUG_PRINTF("DB3 Redundant area erase operation failed ! \n");
   }else{
   IPL_PRT_DEBUG_PRINTF("DB3 Redundant area erase operation OK \n");}
}

int32 m_IPL_TB_DB3
(
   int32 wz_mbfield,
   char *pb_argv
)
{
   IPL_TB_DB3_operation_t action = IPL_TB_DB3_LAST;
   char  *p_extractarg=NULL;
   char  *p_extractfilename=NULL;

   if (pb_argv == NULL)
   {
      return -1;
   }

   /* Get DB3 operation type*/
   p_extractarg = strtok_r(pb_argv, " ", &pb_argv);
   if ((p_extractarg == NULL) || (pb_argv == NULL))
   {
      f_IPL_TB_DB3Test_print_usage();
      return MCM_ERROR;
   }
   IPL_PRT_DEBUG_PRINTF("DB3 operation is    :%s \n",p_extractarg);

   if(!strcmp(p_extractarg,"read"))
      action = IPL_TB_DB3_READ;
   else if(!strcmp(p_extractarg,"write"))
      action = IPL_TB_DB3_WRITE;
   else if(!strcmp(p_extractarg,"erase"))
      action = IPL_TB_DB3_ERASE;
   else
   {
      f_IPL_TB_DB3Test_print_usage();
      return MCM_ERROR;
   }

   switch (action)
   {
      case IPL_TB_DB3_READ:
        {
           p_extractfilename = strtok_r(NULL, " ", &pb_argv);
           if ((p_extractfilename == NULL) || (pb_argv == NULL))
           {
              IPL_PRT_DEBUG_PRINTF("\n Invalid filename \n");
              f_IPL_TB_DB3Test_print_usage();
              return MCM_ERROR;
           }
           IPL_PRT_DEBUG_PRINTF(" File Name: %s\n",p_extractfilename);
           f_IPL_TB_DB3_read(p_extractfilename);
        }
       break;
      case IPL_TB_DB3_WRITE:
       {
           p_extractfilename = strtok_r(NULL, " ", &pb_argv);
           if ((p_extractfilename == NULL) || (pb_argv == NULL))
           {
              IPL_PRT_DEBUG_PRINTF("\n Invalid filename \n");
              f_IPL_TB_DB3Test_print_usage();
              return MCM_ERROR;
           }
           IPL_PRT_DEBUG_PRINTF(" File Name: %s\n",p_extractfilename);
           f_IPL_TB_DB3_write(p_extractfilename);
        }
       break;
      case IPL_TB_DB3_ERASE:
       {
          f_IPL_TB_DB3_erase();
       }
       break;
      default :
       break;
   }

   return MCM_OK;
}

#endif
