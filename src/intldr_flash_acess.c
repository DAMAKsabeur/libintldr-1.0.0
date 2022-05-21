#include "intldr.h"
#include "intldr_internal.h"
#include <errno.h>

INTLDR_erro_t INTLDR_Internal_E2P_Read(uint8_t * ptr , size_t size)
{
	
   INTLDR_erro_t  result = INTLDR_SUCESS;
   int ret = 0;
   FILE * handler = NULL;

   handler = fopen(E2P_DEV ,"r");
   if(NULL == handler)
   {
	  printf ("[%s] Error opening device %s (err:%s)\n", E2P_DEV , strerror(errno)); 
	  result = INTLDR_INTERNAL_FAIL;
	  goto exit;
   }
   
   if (fseek(handler, 0x00, SEEK_SET) == -1)
   {
	  printf("[%s]Error seeking in device %s (err:%s)\n", E2P_DEV, strerror(errno)); 
	  result = INTLDR_INTERNAL_FAIL;
	  goto exit;
   }
   
   ret = fread((uint8_t *)ptr, size ,1, handler); 
   if(ret < 0)
   {
	  printf("[%s]Error reading in device %s (err:%s)\n", E2P_DEV, strerror(errno)); 
	  result = INTLDR_INTERNAL_FAIL;
	  goto exit;
   }
exit:
   if(NULL != handler)
   {
	  fclose(handler);
   }
   return (result);
}


INTLDR_erro_t INTLDR_Internal_E2P_Write(uint8_t * ptr , size_t size)
{
	
   INTLDR_erro_t  result = INTLDR_SUCESS;
   int ret = 0;
   FILE * handler = NULL;

   handler = fopen(E2P_DEV ,"r");
   if(NULL == handler)
   {
	  printf ("[%s] Error opening device %s (err:%s)\n", E2P_DEV , strerror(errno)); 
	  result = INTLDR_INTERNAL_FAIL;
	  goto exit;
   }
   
   if (fseek(handler, 0x00, SEEK_SET) == -1)
   {
	  printf("[%s]Error seeking in device %s (err:%s)\n", E2P_DEV, strerror(errno)); 
	  result = INTLDR_INTERNAL_FAIL;
	  goto exit;
   }
   
   ret = fwrite((uint8_t *)ptr, size ,1, handler); 
   if(ret < 0)
   {
	  printf("[%s]Error reading in device %s (err:%s)\n", E2P_DEV, strerror(errno)); 
	  result = INTLDR_INTERNAL_FAIL;
	  goto exit;
   }
exit:
   if(NULL != handler)
   {
	  fclose(handler);
   }
   return (result);
}
