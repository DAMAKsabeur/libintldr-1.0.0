
#ifndef IPL_TB_INTERNAL_H
#define IPL_TB_INTERNAL_H

/*
*@@@bF
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
X             Copyright (c) 2000 2009 SAGEM SA.
X                                                                           X
X  SAGEM S.A. confidential - Disclosure under NDA only                      X
X  Copyright (c) SAGEM S.A. 2003-2008. All Rights Reserved                  X
X  The copyright to the computer program(s) herein is the property of       X
X  SAGEM S.A., France. The program(s) may be used and/or copied only with   X
X  the written permission of SAGEM S.A. or in accordance with the terms and X 
X  condition stipulated in the agreement/contract under which the program(s)X
X  have been supplied. This copyright notice must not be removed            X
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
X File          : ipl_tb.h
X Author        : Anthony CAMMAS
X Creation date : 12/02/2009
X Product       : ST                                                        
X Language      : C                               
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
*@@@eF
*/

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <syscall.h>
#include <stdlib.h>
#include <mtd/mtd-user.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <termios.h>
#include <malloc.h>

#include "intldr.h"
#include "ipl_tb.h"
#ifndef NAND_ERROR_BASE
#define NAND_ERROR_BASE       ((int32)(0xFFF1F000))

enum
{
   NAND_ERROR_NOT_INITIALIZED = (NAND_ERROR_BASE + 1),  /*!< Driver is not initialized, you have to call NAND_Init before calling     any other function */
   NAND_ERROR_BAD_PARAMETER, /*!< Bad input paramters */
   NAND_ERROR_DEVICE_BUSY, /*!< The NAND is BUSY, try later to perform this operation */
   NAND_ERROR_ERASE, /*!< An error occured during the erase operation, possibly a block becoming bad */
   NAND_ERROR_PROGRAM, /*!< An error occured during the write operation, possibly a block becoming bad */
   NAND_ERROR_TIMEOUT, /*!< An error occured while accessing NAND (could occur in WRITE and READ operation) */
   NAND_ERROR_CORRECTED, /*!< Read operation returning corrupted data, the data are now corrected thanks to ecc computation. (howe    ver the block don't seem te be very healthy) */
   NAND_ERROR_UNCORRECTABLE,/*!< Read operation returning corrupted data. */
   NAND_ERROR_BAD_BLOCK,/*!< Read or Write operation Failed cause of performing on a BadBlock */
   NAND_ERROR_RESERVED,/*!< Read or Write operation Failed cause of performing on a reserved Block (used for BBT) */
   NAND_ERROR_MEMALLOC,/*!< Unable to allocate memory */
   NAND_ERROR_NOT_FOUND,/*!< No BBT fund in NAND */
   NAND_ERROR_NODEV,/*!< NAND Device not fund */
   NAND_ERROR_NOT_SUPPORTED,/*!< operation not supported */
};
#endif

#define IPL_TB_INFOS_NB 4
#define IPL_TB_INFOS_MAXCAR_PERLINE 63

/* ########################################################################## */
/* ### #define                                                                */

/* ########################################################################## */
/* ### typedef                                                                */


typedef struct
{
   uint8 custom_path;
   char usb_path[256]; 
   char mtd_strDevPath[256];
   char mtd_nand_strDevPath[256];
   uint8  mtd_available;
   uint8  FullNand_available;
} IPL_TB_M_Ctx_t;

typedef enum
{
   IPL_TB_NOR_DEVICE    = 0,  /**< NOR flash device */
   IPL_TB_NAND_DEVICE,        /**< NAND flash device */
   IPL_TB_EEPROM_DEVICE,       /**< EEPROM flash device */
   IPL_TB_NOMEN_HW_DEVICE,     /**< 3 Hardware nomenclature partition */
   IPL_TB_FACTORY_DATA_DEVICE, /**< 4 Factory Data partition */
   IPL_TB_NOE2P_DEVICE,        /**< 5 NOE2P partition without E2P emulation*/
   IPL_TB_DA2_DEVICE,          /**< 6 Da2 partition */
   IPL_TB_SEC_DATA_DEVICE,     /**< 7 SEC data partition */
   IPL_TB_RSA_KEYS_DEVICE,     /**< 8 RSA_KEY partition */
   IPL_TB_DOWNLOAD_MODULE1,    /**< 9 DOWNLOADER 1 partition */
   IPL_TB_RA_MODULE,           /**< 10 RESIDANT APPLICATION partition */
   IPL_TB_KERNEL_MODULE,       /**< 11 APPLICATION partition */
   IPL_TB_DOWNLOAD_MODULE2,    /**< 12 DOWNLOADER 1 partition */
   IPL_TB_BG_MODULE,           /**< 13 Backgroung area partition */
   IPL_TB_KERNEL_2_MODULE =17, /**< 17 APPLICATION 2 partition */
   IPL_TB_MAX_NB               /**< Max nb of MTD devices */
} IPL_TB_Device_t;

extern IPL_TB_M_Ctx_t    IPL_TB_M_Ctx;

#endif /* IPL_TB_H */
/** @} */
