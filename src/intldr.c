#include "intldr.h"
#include "intldr_internal.h"

INTLDR_erro_t INTLDR_Init(void)
{
	INTLDR_erro_t result = INTLDR_SUCESS;

    if (Intldr_data == NULL)
    {
		Intldr_data = (Intldr_data_t*) calloc(1, sizeof(Intldr_data_t));
	}

	if (INTLDR_TRUE == Intldr_data->IsInitDone)
	{
		printf(" INTLDR_Init is already initialized !! \n");
		goto exit;
	}
	
	/* read TS_NVRAM_BSL_Area from shared flash memory to RAM */
    result = INTLDR_Internal_E2P_Read(&Intldr_data->TS_NVRAM_BSL_Area, sizeof(TS_NVRAM_BSL_Area_t));
	if (INTLDR_SUCESS != result)
	{
		free(Intldr_data);
		Intldr_data = NULL;
		goto exit;
	}
	
	/* get rvn data and put them in the context */
	
	/* get factory data and put them in the context */
	
	/* at this time all data are loaded into the RAM */ 
	
	Intldr_data->IsInitDone = INTLDR_TRUE;

exit:
	return (result);
}
INTLDR_erro_t INTLDR_Term(void)
{
	INTLDR_erro_t result = INTLDR_SUCESS;
	if (INTLDR_FALSE == Intldr_data->IsInitDone)
	{
		printf("INTLDR is not initialized yet nothing to be done !! \n");
		goto exit;
	}
	/* write TS_NVRAM_BSL_Area from ram to shared flash memory */
exit:
	return (result);	
}

