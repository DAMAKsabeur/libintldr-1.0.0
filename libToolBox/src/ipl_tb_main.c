
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
X File          : ipl_tb_main.c
X Author        : Anthony CAMMAS
X Creation date : 12/02/2009
X Product       : ST                                                        
X Language      : C                               
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
*@@@eF
*/

/* ########################################################################## */
/* ### #include                                                               */
#include "ipl_tb.h"
#include "ipl_tb_l.h"
#include "ipl_tb_internal.h"

#include "linenoise.h"

/* ########################################################################## */
/* ### Variables                                                              */

const char          *pcg_IPL_TB_prompt  = IPL_TB_PROMPT;
const char          *pcg_IPL_TB_display = IPL_TB_DISPLAY;

uint8                bm_IPL_TB_quit_flag;
uint32       * const loader_version;

static char         tbf_IPL_TB_rx_buf[MAX_INBUF_LEN];
static TS_IPL_TB_cmd tsf_IPL_TB_cmd[] =
{
   {"------------------------------NOR/NAND flash access----------------------------", NULL, 0, ""},
   {"read"                 , m_IPL_TB_CmdRead        , 0, "<boot|ldr1|ldr2|kernel|fw2|rootfs|da2|da2R|e2p|fd|bf|nomen>  \t: read designated area"},
   {"write"                , m_IPL_TB_CmdWrite       , 0, "<boot|ldr1|ldr2|kernel|fw2|rootfs|da2|da2R|e2p|fd|bf|nomen>  \t: write designated area"},
   {"nand"                 , m_IPL_TB_NandTest       , 0, " read/write offset size file | erase offset size | mark offset \t: Nand operation"},
#if defined (CONFIG_NAGRA_NASC_3x) && defined (CONFIG_IPL_MTD_NOR)
   {"DB3"                  , m_IPL_TB_DB3            , 0, "<read/write> <file_name> | <erase>  \t: read/write 'file_name' from/to DB3 areas"},
#endif
#if defined CONFIG_IPL_BKGD_APP
   {"background"           , m_IPL_TB_background      , 0, "<read/write> <file_name> | <erase>  \t: read/write 'file_name' from/to background area"},
#endif
   {"------------------------------EEPROM fields access----------------------------", NULL, 0, ""},
   {"-----------------------------------IMPORTANT-----------------------------------", NULL, 0, ""},
   {"--------------ALWAYS USE THE QUIT COMMAND TO UPDATE EEPROM---------------------", NULL, 0, ""},
   {"-----------------------------------IMPORTANT-----------------------------------", NULL, 0, ""},
#if defined CONFIG_IPL_FT_IMPLEMENTATION
   {"boot_mode"            , m_IPL_TB_CmdMbField     , INTLDR_BOOT_MODE,       "\t[boot_mode]                   : read/write boot_mode"},
   {"last_dhcp_address"    , m_IPL_TB_CmdMbField     , INTLDR_LAST_DHCP_ADDRESS, "[dhcp_address]                : read/write last_dhcp_address"},
   {"dhcp77"               , m_IPL_TB_CmdMbField     , INTLDR_DHCP_OPTION_77,"\t\t[dhcp77]                      : read/write dhcp userclass id"},
   {"ntpserver"            , m_IPL_TB_CmdMbField     , INTLDR_NTPSERVER,       "\t[ntpserver]                   : read/write ntpserver"},
   {"timeoffset"           , m_IPL_TB_CmdMbField     , INTLDR_TIMEOFFSET,      "\t[timeoffset]                  : read/write timeoffset"},
   {"dl_firmware_url"      , m_IPL_TB_CmdMbField     , INTLDR_DL_FIRMWARE_URL,   "[url]                         : read/write dl_firmware_url"},
   {"dl_firmware_ssu"      , m_IPL_TB_CmdMbField     , INTLDR_DL_FIRMWARE_SSU,   "[ssu]                         : read/write dl_firmware_ssu"},
   {"dl_firmware_ret"      , m_IPL_TB_CmdMbField     , INTLDR_DL_FIRMWARE_RET,   "[ret]                         : read/write dl_firmware_ret"},
   {"dl_firmware_idx"      , m_IPL_TB_CmdMbField     , INTLDR_DL_FIRMWARE_IDX,   "[ret]                         : read/write dl_firmware_idx"},
   {"rescue_firmware_url"  , m_IPL_TB_CmdMbField     , INTLDR_RESCUE_FIRMWARE_URL,"[url]                        : read/write rescue_firmware_url"},
   {"dl_loader_url"        , m_IPL_TB_CmdMbField     , INTLDR_DL_LOADER_URL,   "\t[url]                         : read/write dl_loader_url"},
   {"dl_loader_ssu"        , m_IPL_TB_CmdMbField     , INTLDR_DL_LOADER_SSU,   "\t[ssu]                         : read/write dl_loader_ssu"},
   {"dl_loader_ret"        , m_IPL_TB_CmdMbField     , INTLDR_DL_LOADER_RET,   "\t[ret]                         : read/write dl_loader_ret"},
   {"dl_loader_idx"        , m_IPL_TB_CmdMbField     , INTLDR_DL_LOADER_IDX,   "\t[ret]                         : read/write dl_loader_idx"},
   {"rescue_loader_url"    , m_IPL_TB_CmdMbField     , INTLDR_RESCUE_LOADER_URL, "[url]                         : read/write rescue_loader_url"},
   {"dl_ra_url"            , m_IPL_TB_CmdMbField     , INTLDR_DL_RA_URL,       "\t[url]                         : read/write dl_ra_url"},
   {"dl_ra_ssu"            , m_IPL_TB_CmdMbField     , INTLDR_DL_RA_SSU,       "\t[ssu]                         : read/write dl_ra_ssu"},
   {"dl_ra_ret"            , m_IPL_TB_CmdMbField     , INTLDR_DL_RA_RET,       "\t[ret]                         : read/write dl_ra_ret"},
   {"dl_ra_idx"            , m_IPL_TB_CmdMbField     , INTLDR_DL_RA_IDX,       "\t[ret]                         : read/write dl_ra_idx"},
   {"rescue_ra_url"        , m_IPL_TB_CmdMbField     , INTLDR_RESCUE_RA_URL,  "\t[url]                          : read/write rescue_ra_url"},
   {"country"              , m_IPL_TB_CmdMbField     , INTLDR_COUNTRY,        "\t[country]                      : read/write country"},
#if defined (CONFIG_IPL_NET_WIFI)
   {"wifi_ssid"            , m_IPL_TB_CmdMbField     , STREAM_INFO_ELEMENT_WIFI_SSID,      "\t[new_ssid]                       : show/set wifi_ssid field"},
   {"wifi_key"             , m_IPL_TB_CmdMbField     , STREAM_INFO_ELEMENT_WIFI_KEY,        "\t[new_key]                       : show/set wifi_key field"},
#if defined (CONFIG_IPL_NET_WIFI_STATE)
   {"wifi_state"           , m_IPL_TB_CmdMbField     , STREAM_INFO_ELEMENT_WIFI_STATE,      "\t[new_wifi_sate]                  : show/set wifi_state field"},
#endif //CONFIG_IPL_NET_WIFI_STATE

#endif // CONFIG_IPL_NET_WIFI
   {"defaults"             , m_IPL_TB_CmdDefaults    , 0,                     "\t                               : restore default MailBox values"},
#else /* Generic product (-> not CONFIG_IPL_FT_IMPLEMENTATION) */
   {"dl_request"           , m_IPL_TB_CmdMbField     , STREAM_INFO_ELEMENT_DL_REQUEST,          "\t[request]    : read/write download request"},
   {"ldr_disabled"         , m_IPL_TB_CmdMbField     , STREAM_INFO_ELEMENT_LDR_KO,              "\t[flag]       : read/write loader disabled flag"},
   {"dl_interface"         , m_IPL_TB_CmdMbField     , STREAM_INFO_ELEMENT_INTERFACE,           "\t[type]       : read/write DL interface"},
   {"dl_protocol"          , m_IPL_TB_CmdMbField     , STREAM_INFO_ELEMENT_PROTOCOL,            "\t[type]       : read/write DL protocol"},
   {"frequency"            , m_IPL_TB_CmdMbField     , STREAM_INFO_ELEMENT_FREQUENCY,           "\t[frequency]  : read/write frequency"},
   {"symbol_rate"          , m_IPL_TB_CmdMbField     , STREAM_INFO_ELEMENT_SYMBOLRATE,          "\t[rate]       : read/write Symbol Rate"},
   {"modulation"           , m_IPL_TB_CmdMbField     , STREAM_INFO_ELEMENT_MODULATION,          "\t[modulation] : read/write modulation"},
   {"lnb_type"             , m_IPL_TB_CmdMbField     , STREAM_INFO_ELEMENT_LNB_TYPE,            "\t[type]       : read/write LNB parameters"},
   {"module_id"            , m_IPL_TB_CmdMbField     , STREAM_INFO_ELEMENT_MODULE_ID,           "\t[ID]         : read/write module id"},
   {"timeout"              , m_IPL_TB_CmdMbField     , STREAM_INFO_ELEMENT_TIMEOUT,             "\t[timeout]    : read/write timeout"},
   {"PID"                  , m_IPL_TB_CmdMbField     , STREAM_INFO_ELEMENT_PID,                 "\t\t[PID]      : read/write PID"},
   {"fe_type"              , m_IPL_TB_CmdMbField     , STREAM_INFO_ELEMENT_FE_TYPE,             "\t[type]       : read/write fe_type"},
   {"module_type"          , m_IPL_TB_CmdMbField     , STREAM_INFO_ELEMENT_MODULE_TYPE,         "\t[type]       : read/write appli/loader type"},
   {"diseqc_version"       , m_IPL_TB_CmdMbField     , STREAM_INFO_ELEMENT_DISEQC_VER,          "\t[type]       : read/write diseqc version"},
   {"module_22khz"         , m_IPL_TB_CmdMbField     , STREAM_INFO_ELEMENT_22KHZ ,              "\t[type]       : read/write module 22kHz"},
   {"switch_entry"         , m_IPL_TB_CmdMbField     , STREAM_INFO_ELEMENT_SWITCH_ENTRY,        "\t[type]       : read/write lnb_nr"},
#ifdef CONFIG_DYNAMIC_USAGE_ID
   {"usage_id"             , m_IPL_TB_CmdMbField     , STREAM_INFO_ELEMENT_USAGE_ID,            "\t[ID]         : read/write usage id"},
#endif
#ifdef CONFIG_NET_FE
   {"polarization"         , m_IPL_TB_CmdMbField     , STREAM_INFO_ELEMENT_POLARIZATION,        "\t[polarization]: read/write polarization"},
   {"lnb_power"            , m_IPL_TB_CmdMbField     , STREAM_INFO_ELEMENT_LNB_POWER,           "\t[type]       : read/write LNB power parameters"},
   {"lnb_osc"              , m_IPL_TB_CmdMbField     , STREAM_INFO_ELEMENT_LNB_OSC,             "\t[type]       : read/write LNB osc parameters"},
   {"lnb_bands"            , m_IPL_TB_CmdMbField     , STREAM_INFO_ELEMENT_LNB_BANDS,           "\t[type]       : read/write LNB band parameters"},
   {"ub_nr"                , m_IPL_TB_CmdMbField     , STREAM_INFO_ELEMENT_UBNR,                "\t[number]     : read/write UB number"},
   {"ub_freq"              , m_IPL_TB_CmdMbField     , STREAM_INFO_ELEMENT_UBFREQ,              "\t[frequency]  : read/write UB frequency"},
#endif // CONFIG_NET_FE
#ifdef CONFIG_IPL_NET_IP
   {"url"                  , m_IPL_TB_CmdMbField     , STREAM_INFO_ELEMENT_URL,                 "\t\t[url]        : read/write downloader URL"},
   {"use_dhcp"             , m_IPL_TB_CmdMbField     , STREAM_INFO_ELEMENT_USE_DHCP,            "\t[Bool]      : Use dhcp to get ip address?"},
   {"ip"                   , m_IPL_TB_CmdMbField     , STREAM_INFO_ELEMENT_IP_ADD,              "\t\t[IP]        : STB IP Address"},
   {"netmask"              , m_IPL_TB_CmdMbField     , STREAM_INFO_ELEMENT_NETMASK,             "\t[IP]        : Netmask"},
   {"gateway"              , m_IPL_TB_CmdMbField     , STREAM_INFO_ELEMENT_DEFAULT_GW,          "\t[IP]        : Gateway address"},
   {"dns_1"                , m_IPL_TB_CmdMbField     , STREAM_INFO_ELEMENT_DNS_ADD_1,           "\t\t[IP]        : DNS 1 address"},
   {"dns_2"                , m_IPL_TB_CmdMbField     , STREAM_INFO_ELEMENT_DNS_ADD_2,           "\t\t[IP]        : DNS 2 address"}, 
#if defined (CONFIG_IPL_NET_WIFI)
   {"wifi_ssid"            , m_IPL_TB_CmdMbField     , STREAM_INFO_ELEMENT_WIFI_SSID,      "\t[new_ssid]                       : show/set wifi_ssid field"},
   {"wifi_key"             , m_IPL_TB_CmdMbField     , STREAM_INFO_ELEMENT_WIFI_KEY,        "\t[new_key]                       : show/set wifi_key field"},
   {"wifi_protocol"             , m_IPL_TB_CmdMbField     , STREAM_INFO_ELEMENT_WIFI_PROTOCOL,   "\t[wifi_protocol]                 : show/set wifi_protocol field"},
#endif // CONFIG_IPL_NET_WIFI
#endif // CONFIG_IPL_NET_IP
   
#if defined CONFIG_NET_FE_CAB || defined CONFIG_NET_FE
   {"fec_Rate"         , m_IPL_TB_CmdMbField     , STREAM_INFO_ELEMENT_FEC_RATE,        		"\t[fec_rate]	: read/write fec rate"},
#endif // CONFIG_NET_FE_CAB || CONFIG_NET_FE
   {"defaults"             , m_IPL_TB_CmdDefaults    , 0,                                       "\t             : restore default MailBox values"},
#if defined (CONFIG_IPL_FE_SELECT_ENTRY_BY_E2P)
   {"fe_entry"            , m_IPL_TB_CmdMbField     , STREAM_INFO_ELEMENT_FRONT_END_ENTRY,  "\t[type]        : read/write Front End Entry parameters"},
#endif // CONFIG_IPL_FE_SELECT_ENTRY_BY_E2P
#if defined (CONFIG_IPL_OSD_CONFIGURATION_BY_APP)
   {"video_port"          , m_IPL_TB_CmdMbField     , STREAM_INFO_ELEMENT_VIDEO_OUTPUT_PORT,  "\t[type]     : read/write Video_Output_port parameters"},
   {"video_standard"      , m_IPL_TB_CmdMbField     , STREAM_INFO_ELEMENT_VIDEO_STANDARD,  "\t[type]     : read/write Video_Standard parameters"},
   {"video_resolution"    , m_IPL_TB_CmdMbField     , STREAM_INFO_ELEMENT_VIDEO_RESOLUTION,"\t[type]  : read/write Video_Output_resolution parameters"},
#endif // CONFIG_IPL_OSD_CONFIGURATION_BY_APP
#if defined (CONFIG_SILENT_UPGRADE_MB)
   {"boot_screen"          , m_IPL_TB_CmdMbField     , STREAM_INFO_ELEMENT_BOOT_SCREEN_DISPLAY,   "\t[type] : read/write Boot_screen_display parameters"},
#endif // CONFIG_SILENT_UPGRADE_MB
   {"new_version_available", m_IPL_TB_CmdMbField     , STREAM_INFO_ELEMENT_NEW_VERSION_AVAILABLE, "\t[type] : read/write New_version_available"},
   {"group_id"             , m_IPL_TB_CmdMbField     , STREAM_INFO_ELEMENT_GROUP_ID, "\t[GroupId] : read/write Group_Id field"},
#if defined (CONFIG_IPL_SECU_VERIFIED_FLAG)
   {"verified_flag"        , m_IPL_TB_CmdMbField     , STREAM_INFO_ELEMENT_VERIFIED_FLAG, "\t[Verified flag] : read/write Verified flag"},
#endif // CONFIG_IPL_SECU_VERIFIED_FLAG
#endif  // Generic product (-> not CONFIG_IPL_FT_IMPLEMENTATION)
   {"-----------------------------------Others-----------------------------------------", NULL, 0, ""},
   {"corrupt"              , m_IPL_TB_CmdCorrupt           , 0,               "\t<area>                           : corrupt area"},
  // {"e2p_corrupt"          , m_IPL_TB_CmdE2pCorrupt      , 0, "        <page_num>                               : corrupt e2p page"},
   {"versions"             , m_IPL_TB_CmdVersions          , 0,               "\t                                 : show current modules versions"},
   {"system"               , m_IPL_TB_CmdSystem            , 0,               "\t\t<cmd>                            : execute system command"},
   {"set_path"             , m_IPL_TB_CmdSetPath           , 0,               "\tdefault|<path>              : set path to read/write memory dumps"},
   {"quit"                 , m_IPL_TB_CmdQuit              , 0,               "\t\t                                 : save data and quit program"},
   {"reboot"               , m_IPL_TB_CmdReboot            , 0,               "\t\t                                 : save data and reboot STB"},
   {NULL                   , NULL                          , 0, NULL}
};


/* ########################################################################## */
/* ### Functions                                                              */

static void f_IPL_TB_Completion(const char *cmd, linenoiseCompletions *lc);
static int32 f_IPL_TB_ExecuteCommand(char *pb_buffer, uint32 w_size);
#if defined (CONFIG_IPL_MTD_NOR) || defined (CONFIG_IPL_MTD_NAND_SC)
static int32 f_IPL_TB_get_mtd_dev_path(void);
#endif

/** 
 * @brief Runs the toolbox.
 */
int32 IPL_TB_Main
(
   void
)
{
   uint32   wz_cmd_size;
   char     tcz_display[IPL_TB_DISPLAY_SIZE];
   
   char*    cmd = tbf_IPL_TB_rx_buf;

   strncpy(tcz_display, pcg_IPL_TB_display, IPL_TB_DISPLAY_SIZE-1);

   IPL_PRT_DEBUG_PRINTF("\r\n\n####################\r\n");
   IPL_PRT_DEBUG_PRINTF("# %s\r\n", IPL_TB_IDENTITY);
   IPL_PRT_DEBUG_PRINTF("####################\r\n\n");


#if defined (CONFIG_IPL_MTD_NOR) || defined (CONFIG_IPL_MTD_NAND_SC)
   if (f_IPL_TB_get_mtd_dev_path() != MCM_OK)
   {
      return MCM_ERROR;
   }
#elif !(defined CONFIG_LOADER_NO_NOR)

 !!!! ERROR NO FLASH device define !!!!
 
#endif

   if(INTLDR_Init() != MCM_OK)
   {
      IPL_PRT_DEBUG_PRINTF("Cannot read E2P data\r\n");
      return MCM_ERROR;
   }

   bm_IPL_TB_quit_flag = MCM_FALSE;

#ifdef CONFIG_IPL_TB_MIGRATION
if (0){
#endif
   
   linenoiseSetCompletionCallback (f_IPL_TB_Completion);

   /** Main loop **/
   /***************/
   /*Klocwork issue: infinite loop due to static code analysis*/
   while (bm_IPL_TB_quit_flag == MCM_FALSE)
   {
      /** Set to max size, so that the get command function knows the buffer size **/
      /*****************************************************************************/
      wz_cmd_size = MAX_INBUF_LEN;

      /** Get command from uart **/
      /***************************/
      cmd = linenoise (pcg_IPL_TB_prompt);
      linenoiseHistoryAdd (cmd);
      wz_cmd_size = strlen (cmd)+1;

      /** Execute the retrieved command **/
      /***********************************/
      f_IPL_TB_ExecuteCommand(cmd, wz_cmd_size);
      
      free (cmd);
   }
#ifdef CONFIG_IPL_TB_MIGRATION
}
   m_IPL_TB_CmdWrite(0,"ldr1");
   m_IPL_TB_CmdWrite(0,"ldr2");
   m_IPL_TB_CmdWrite(0,"boot");
   m_IPL_TB_CmdCorrupt(0,"kernel");
#endif
  
   if( INTLDR_Term() != OPAL_NO_ERROR )
   {
      /*Klocwork : Code is unreachable. Keep a trace when function return an error*/
      IPL_PRT_DEBUG_PRINTF("Cannot read E2P data\r\n");
      return MCM_ERROR; 
   }
   else
   {
      IPL_PRT_DEBUG_PRINTF("mailBox has been updated\r\n");
   }

   return MCM_OK;

} /* End of function 'IPL_TB_Main()' */

/*
*@@@bf
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
X                     Copyright (c) 2000 2009 SAGEM SA.
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
X Function      : f_IPL_TB_Completion()
X Author        : Anthony CAMMAS
X Creation date : 12/02/2009
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
*@@@ef
*/
static void f_IPL_TB_Completion(const char *cmd, linenoiseCompletions *lc)
{
   int          i;
   const size_t len = strlen (cmd);

   for (i = 0; tsf_IPL_TB_cmd[i].pc_cmd != NULL; ++i)
   {
      if ((tsf_IPL_TB_cmd[i].pc_cmd[0] != '-')
          && (strncasecmp (cmd, tsf_IPL_TB_cmd[i].pc_cmd, len) == 0))
      {
         linenoiseAddCompletion (lc, tsf_IPL_TB_cmd[i].pc_cmd);
      }
   }
}                               /* End of function 'f_IPL_TB_Completion()' */

/*
*@@@bf
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
X                     Copyright (c) 2000 2009 SAGEM SA.
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
X Function      : f_IPL_TB_ExecuteCommand()
X Author        : Anthony CAMMAS
X Creation date : 12/02/2009
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
*@@@ef
*/
static int32 f_IPL_TB_ExecuteCommand
(
   char    *pb_buffer,
   uint32   w_size
)
{
   char     *pcz_cmd;
   char     *pcz_arg;
   uint32   i = 0;
   char     *pcz_save;

   /** Extract command **/
   /*********************/
   pcz_cmd = strtok_r(pb_buffer, " ", &pcz_save);
   if (pcz_cmd == NULL)
   {
      /** No command found, then print usage **/
      /****************************************/
      IPL_PRT_DEBUG_PRINTF("\r\n\n####################\r\n");
      IPL_PRT_DEBUG_PRINTF("# List of commands\r\n");
      IPL_PRT_DEBUG_PRINTF("####################\r\n");
      
      while (tsf_IPL_TB_cmd[i].pc_cmd != NULL)
      {
         IPL_PRT_DEBUG_PRINTF("%s \t %s\r\n",
               tsf_IPL_TB_cmd[i].pc_cmd, tsf_IPL_TB_cmd[i].pc_help);
         i++;
      }
      IPL_PRT_DEBUG_PRINTF("####################\r\n");

      return 0;
   }
   
   /** Extract arguments **/
   /***********************/
   if((!strcmp(pcz_cmd,"system")) || (!strcmp(pcz_cmd,"nand")) || (!strcmp(pcz_cmd,"background"))|| (!strcmp(pcz_cmd,"DB3")))
   {
      pcz_arg   = pcz_save;
   }
   else
   {
      pcz_arg  = strtok_r(NULL, " ", &pcz_save);
   }

   while (tsf_IPL_TB_cmd[i].pc_cmd != NULL)
   {
      if (strcmp(pcz_cmd, tsf_IPL_TB_cmd[i].pc_cmd) == 0)
      {
         int32 z_cr;

         z_cr = tsf_IPL_TB_cmd[i].pf_cmd_function(
                tsf_IPL_TB_cmd[i].wz_field, 
                pcz_arg);
         if (z_cr != 0)
         {
            IPL_PRT_DEBUG_PRINTF("\r\n  ==> Command '%s' error = %d\r\n", pcz_cmd, (unsigned int)z_cr);
            return -1;
         }

         IPL_PRT_DEBUG_PRINTF("\r\n  ==> Command '%s' OK\r\n", pcz_cmd);
         return 0;
      }
      i++;
   }
   
   IPL_PRT_DEBUG_PRINTF("\r\n ==> Unknown command '%s'\r\n", pcz_cmd);
   
   return MCM_OK;

} /* End of function 'f_IPL_TB_ExecuteCommand()' */


#define MTD_DEVICES_NUMBER 2


#if defined (CONFIG_IPL_MTD_NOR) || defined (CONFIG_IPL_MTD_NAND_SC)
/**
 *  Sets the MTD device name according to /proc/mtd
 */
static int32 f_IPL_TB_get_mtd_dev_path(void)
{
    FILE * h;
    int vRet= MCM_ERROR;
    IPL_TB_M_Ctx_t *pCtx    = &IPL_TB_M_Ctx;
    int minor;
    int size;
    int erasesize;
    int vMatchNb;
    int vDeviceFound=0;
    char name[63];
    char strLine[63];

    pCtx->mtd_available = 0;
    pCtx->FullNand_available=0;
    /* Break on error loop */
    for(;;)
    {
        h=fopen("/proc/mtd","r");
        if(h== NULL) {vRet=MCM_ERROR; break;}

        while(0 != fgets (strLine,63,h))
        {
            /* First line is a dummy line : "dev:    size   erasesize  name\n" */
            /* (see .../kernel/drivers/mtd/mtdcore.c)                          */

            vMatchNb=sscanf (strLine,"mtd%d: %8x %8x %s",&minor,&size,&erasesize,name);

            /* Scanf pattern above should return 4 values (i.e. minor, size, erasesize and name) */
            if(4==vMatchNb)
            {
                if(!strcmp(CONFIG_IPL_MTD_NOR_DEVICE_NAME,name))
                {
                    vDeviceFound++;
			           IPL_PRT_DEBUG_PRINTF("Found NOR partition : /dev/mtd%d\n",minor);
                    sprintf(pCtx->mtd_strDevPath,"/dev/mtd%d",minor);
                }
                if(!strcmp(CONFIG_IPL_MTD_NOE2P_DEVICE_NAME,name))
                {
   			        IPL_PRT_DEBUG_PRINTF("Found NoE2P partition : /dev/mtd%d\n",minor);
                    vDeviceFound++;
                }
#if defined(CONFIG_IPL_TB_NAND_SC)
                if(!strcmp(CONFIG_IPL_TB_MMC_DEVICE_NAME,name))
                {
   			        IPL_PRT_DEBUG_PRINTF("Found NAND partition : /dev/mtd%d\n",minor);
                    sprintf(pCtx->mtd_nand_strDevPath,"/dev/mtd%d",minor);
                    vDeviceFound++;
                    pCtx->FullNand_available=1;
                }
#endif
                if(vDeviceFound == MTD_DEVICES_NUMBER)
                {
                  break;
                }
            }
        }

        vRet=fclose(h);
        if(vRet) {vRet=OPAL_ERROR_IO; break;}

        if(vDeviceFound == MTD_DEVICES_NUMBER)
        {
          pCtx->mtd_available = 1;
          vRet=MCM_OK;
        }
        else
        {
#if defined(CONFIG_IPL_TB_NAND_SC)
          IPL_PRT_DEBUG_PRINTF("\nERROR!! check Mtd partition definition: FullNor / Noe2pFS  and NAND modules partition are requested\n");
#else
          IPL_PRT_DEBUG_PRINTF("\nERROR!! check Mtd partition definition: FullNor and Noe2pFS are requested\n");
#endif
          IPL_PRT_DEBUG_PRINTF("\n!!!All Toolbox commands are not available!!! \n");
          vRet=MCM_OK;
        }

        break;
    }

    return vRet;
}
#endif
   
