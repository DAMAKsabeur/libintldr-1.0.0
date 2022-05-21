
#ifndef IPL_TB_L_H
#define IPL_TB_L_H

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
X File          : ipl_tb_l.h
X Author        : Anthony CAMMAS
X Creation date : 12/02/2009
X Product       : ST                                                        
X Language      : C                               
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
*@@@eF
*/

#include "ipl_tb_internal.h"

/* ########################################################################## */
/* ### #define                                                                */

#define  MAX_INBUF_LEN        512
#define  MAX_E2P_LEN          MAX_INBUF_LEN

#define  KEY_NL               '\n'
#define  KEY_CR               '\r'
#define  KEY_BS               '\b'

#define  ACTIVATE_ECHO        0

#ifdef CONFIG_IPL_TB_MIGRATION
#define  IPL_TB_IDENTITY      "Migration to final configuration"
#else
#define  IPL_TB_IDENTITY      "ToolBox v0.2"
#endif
#define  IPL_TB_DISPLAY_SIZE  4
#define  IPL_TB_DISPLAY       "  tb"
#define  IPL_TB_PROMPT        " > "

#define  IPL_TB_BOOT_DISPLAY  "boot"

/**
 *  MTD proc file path
 */
#define IPL_TB_MTD_PROC_PATH "/proc/mtd"

/* ########################################################################## */
/* ### typedef                                                                */

typedef struct
{
   const char  *pc_cmd;
   int32       (*pf_cmd_function)(int32 wz_mbfield, char *pb_argv);
   int32       wz_field;
   const char  *pc_help;
} TS_IPL_TB_cmd;

/* ########################################################################## */
/* ### struct                                                                 */

/* ########################################################################## */
/* ### macro                                                                  */

/* ########################################################################## */
/* ### variables                                                              */

extern uint8                  bm_IPL_TB_quit_flag;

/* ########################################################################## */
/* ### functions                                                              */

extern int32 m_IPL_TB_CmdMbField      (int32 wz_field_arg, char *pb_argv);
extern int32 m_IPL_TB_CmdCorrupt      (int32 wz_mbfield,   char *pb_argv);
extern int32 m_IPL_TB_CmdDefaults     (int32 wz_mbfield,   char  *pb_arg);
extern int32 m_IPL_TB_CmdRead         (int32 wz_mbfield,   char *pb_argv);
extern int32 m_IPL_TB_CmdWrite        (int32 wz_mbfield,   char *pb_argv);
extern int32 m_IPL_TB_CmdVersions     (int32 wz_mbfield,   char *pb_argv);
extern int32 m_IPL_TB_CmdSetPath      (int32 data,   char *pb_argv);
extern int32 m_IPL_TB_CmdSystem       (int32 wz_mbfield,   char *pb_argv);
extern int32 m_IPL_TB_CmdQuit         (int32 wz_mbfield,   char *pb_argv);
extern int32 m_IPL_TB_CmdReboot       (int32 wz_mbfield,   char *pb_argv);
extern int32 m_IPL_TB_NandTest        (int32 wz_mbfield,   char *pb_argv);
#if defined CONFIG_IPL_MTD_NOR 
extern int32 IPL_TB_NOR_Read          (uint8* const o_pData, const uint32 i_StartAddress, const uint32  i_Size);
extern int32 IPL_TB_NOR_Erase         (const uint32 i_StartAddress, const uint32 i_Size); 
extern int32 IPL_TB_NOR_Write         (const uint8* const i_pData, const uint32 i_StartAddress, const uint32 i_Size);
#endif
extern int32 IPL_TB_NAND_Read         (IPL_TB_Device_t i_DeviceType, uint8* const o_pData, const uint32 i_StartAddress, const uint32  i_Size);
extern int32 IPL_TB_NAND_Erase        (IPL_TB_Device_t i_DeviceType, const uint32 i_StartAddress, const uint32 i_Size); 
extern int32 IPL_TB_NAND_Write        (IPL_TB_Device_t i_DeviceType, const uint8* const i_pData, const uint32 i_StartAddress, const uint32 i_Size);
extern int32 IPL_TB_WriteBlock        (uint8 *pDataRead, uint32 i_DataReadSize, IPL_TB_Device_t Mtd_Device, uint32 Block_Mapping, uint32 Block_Size);
extern int32 IPL_TB_ReadBlock         (IPL_TB_Device_t i_DeviceType, uint8* const o_pData, const uint32 i_StartAddress, const uint32 i_Size);
#if defined (CONFIG_IPL_BKGD_APP)
extern int32 m_IPL_TB_background(int32 wz_mbfield,char *pb_argv);
#endif
#if defined (CONFIG_NAGRA_NASC_3x) && defined CONFIG_IPL_MTD_NOR
extern int32 m_IPL_TB_DB3(int32 wz_mbfield,char *pb_argv);
#endif

#endif /* IPL_TB_L_H */
