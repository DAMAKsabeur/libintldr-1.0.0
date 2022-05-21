#ifndef INTLDR_INTERNAL_H
#define INTLDR_INTERNAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "intldr.h"

#define RVN_DEV            "/dev/rvn"
#define E2P_DEV            "/dev/mapper/e2p"
typedef struct Intldr_data_t
{
   INTLDR_bool_t                IsInitDone;                  
   TS_NVRAM_BSL_Area_t          TS_NVRAM_BSL_Area
} Intldr_data_t;


INTLDR_erro_t INTLDR_Internal_E2P_Read  (uint8_t * ptr , size_t size);
INTLDR_erro_t INTLDR_Internal_E2P_Write (uint8_t * ptr , size_t size);
Intldr_data_t* Intldr_data;
#ifdef __cplusplus
}
#endif
#endif /* INTLDR_INTERNAL_H */
