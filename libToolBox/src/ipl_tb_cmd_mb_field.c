
/*-----------------------------------------------------------------------------
 FILE DOCUMENTATION
-----------------------------------------------------------------------------*/
/**
 * @note    Copyright 2000-2012 SagemCom SAS All rights reserved
 *
 * @file    ipl_tb_cmd_mb_field.c
 * @brief   IPL_TB implementation
 * @version Revision of last commit : 14
 * @date    Date of last commit     : 2012-03-25 19:15:34 +0100 (Mon, 06 Feb 2012)
 * @author  Author of last commit   : Jean-Louis BONNAFFE 
 * @addtogroup IPL_TB
 * @{
 */

/*-----------------------------------------------------------------------------
 FUNCTIONAL                      INCLUSIONS in functional/alphabetical order
-----------------------------------------------------------------------------*/
#include <stdint.h>
#include <stdlib.h> 
#include "intldr.h" 
/* ########################################################################## */
/* ### Variables                                                              */
# define FIELD_DESC_SIZE	128
# define FIELD_INFO_SIZE	512

/*-----------------------------------------------------------------------------
 INTERNAL FUNCTIONS              DEFINITIONS in functional/alphabetical order
-----------------------------------------------------------------------------*/
/** 
 * @brief    Get or set any mailbox field.
 */

int32_t m_IPL_TB_CmdMbField
(
   int32_t wz_field,
   char *pb_arg
)
{
   char arg_name[FIELD_DESC_SIZE] = "";
   char arg_info[FIELD_INFO_SIZE] = "";
   uint8_t* pBuffer = NULL;

   switch((TE_INTLDR_MailboxField)wz_field)
   {
#if defined CONFIG_IPL_FT_IMPLEMENTATION
      case INTLDR_BOOT_MODE:
         strncpy(arg_name, "Boot mode", (size_t)FIELD_DESC_SIZE);
         strncpy(arg_info, "Valid values are:\n"
                           "APPLICATION\n"
                           "DOWNLOAD\n"
                           "DOWNLOAD_SSU\n",FIELD_INFO_SIZE);
         break;
         break;
      case INTLDR_LAST_DHCP_ADDRESS:
         strncpy(arg_name, "Last DHCP address", (size_t)FIELD_DESC_SIZE);
         strncpy(arg_info, "Max string size is 32\n",
                            FIELD_INFO_SIZE);
         break;
      case INTLDR_NTPSERVER:
         strncpy(arg_name, "NTP server(s)", FIELD_DESC_SIZE);
         strncpy(arg_info, "Max string size is 4096\n",
                            FIELD_INFO_SIZE);
         break;
      case INTLDR_TIMEOFFSET:
         strncpy(arg_name, "Time zone", FIELD_DESC_SIZE);
         strncpy(arg_info, "Max string size is 4096\n",
                            FIELD_INFO_SIZE);
         break;
      case INTLDR_DL_FIRMWARE_URL:
         strncpy(arg_name, "firmware download URL", FIELD_DESC_SIZE);
         strncpy(arg_info, "Max string size is 4096\n",
                            FIELD_INFO_SIZE);
         break;
      case INTLDR_DL_FIRMWARE_SSU:
         strncpy(arg_name, "firmware SSU download URL", FIELD_DESC_SIZE);
         strncpy(arg_info, "Max string size is 4096 \n",
                            FIELD_INFO_SIZE);
         break;
      case INTLDR_DL_FIRMWARE_RET:
         strncpy(arg_name, "firmware download return code", FIELD_DESC_SIZE);
         strncpy(arg_info, "Download status\n",
                            FIELD_INFO_SIZE);
         break;
      case INTLDR_RESCUE_FIRMWARE_URL:
         strncpy(arg_name, "firmware rescue download URL", FIELD_DESC_SIZE);
         strncpy(arg_info, "Max string size is 4096\n",
                            FIELD_INFO_SIZE);
         break;
      case INTLDR_DL_LOADER_URL:
         strncpy(arg_name, "Loader download URL", FIELD_DESC_SIZE);
         strncpy(arg_info, "Max string size is 4096\n",
                            FIELD_INFO_SIZE);
         break;
      case INTLDR_DL_LOADER_SSU:
         strncpy(arg_name, "Loader SSU download URL", FIELD_DESC_SIZE);
         strncpy(arg_info, "Max string size is 4096\n",
                            FIELD_INFO_SIZE);
         break;
      case INTLDR_DL_LOADER_RET:
         strncpy(arg_name, "Loader download return code", FIELD_DESC_SIZE);
         strncpy(arg_info, "Download status\n",
                            FIELD_INFO_SIZE);
         break;
      case INTLDR_RESCUE_LOADER_URL:
         strncpy(arg_name, "Loader rescue download URL", FIELD_DESC_SIZE);
         strncpy(arg_info, "Max string size is 4096\n",
                            FIELD_INFO_SIZE);
         break;
      case INTLDR_DL_RA_URL:
         strncpy(arg_name, "RA download URL", FIELD_DESC_SIZE);
         strncpy(arg_info, "Max string size is 4096\n",
                            FIELD_INFO_SIZE);
         break;
      case INTLDR_DL_RA_SSU:
         strncpy(arg_name, "RA SSU download URL", FIELD_DESC_SIZE);
         strncpy(arg_info, "Max string size is 4096\n",
                            FIELD_INFO_SIZE);
         break;
      case INTLDR_DL_RA_RET:
         strncpy(arg_name, "RA download return code", FIELD_DESC_SIZE);
         strncpy(arg_info, "Download status\n",
                            FIELD_INFO_SIZE);
         break;
      case INTLDR_RESCUE_RA_URL:
         strncpy(arg_name, "RA rescue download URL", FIELD_DESC_SIZE);
         strncpy(arg_info, "Max string size is 4096\n",
                            FIELD_INFO_SIZE);
         break;
      case INTLDR_COUNTRY:
         strncpy(arg_name, "Country", FIELD_DESC_SIZE);
         strncpy(arg_info, "Max string size is 4\n",
                            FIELD_INFO_SIZE);
         break;
      case INTLDR_DL_FIRMWARE_IDX:
         strncpy(arg_name, "FW download index", FIELD_DESC_SIZE);
         strncpy(arg_info, "Last protocol used\n", FIELD_INFO_SIZE);
         break;
      case INTLDR_DL_LOADER_IDX:
         strncpy(arg_name, "Loader download index", FIELD_DESC_SIZE);
         strncpy(arg_info, "Last protocol used\n", FIELD_INFO_SIZE);
         break;
      case INTLDR_DL_RA_IDX:
         strncpy(arg_name, "RA download index", FIELD_DESC_SIZE);
         strncpy(arg_info, "Last protocol used\n", FIELD_INFO_SIZE);
         break;
      case INTLDR_DHCP_OPTION_77:
         strncpy(arg_name, "DHCP option 77", FIELD_DESC_SIZE);
         strncpy(arg_info, "Max string size is 128\n", FIELD_INFO_SIZE);
         break;
#if defined (CONFIG_IPL_NET_WIFI)
      case STREAM_INFO_ELEMENT_WIFI_SSID:
         strncpy(arg_name, "wifi ssid", FIELD_DESC_SIZE);
         strncpy(arg_info, "Max string size is 32\n", FIELD_INFO_SIZE);
         break;
      case STREAM_INFO_ELEMENT_WIFI_KEY:
         strncpy(arg_name, "wifi key", FIELD_DESC_SIZE);
         strncpy(arg_info, "Max string size is 64\n", FIELD_INFO_SIZE);
         break;
#if defined (CONFIG_IPL_NET_WIFI_STATE)
      case STREAM_INFO_ELEMENT_WIFI_STATE:
         strncpy(arg_name, "wifi state", FIELD_DESC_SIZE);
         strncpy(arg_info, "Max string size is 1\n", FIELD_INFO_SIZE);
         break;
#endif
#endif  
#else /*generic implementation*/
      case STREAM_INFO_ELEMENT_DL_REQUEST:
         strncpy(arg_name, "Download request", (size_t)FIELD_DESC_SIZE);
#if defined (CONFIG_IPL_ANDROID_N) || defined (CONFIG_IPL_ANDROID_O)
         strncpy(arg_info, "No download               --> 0 \n"
                           "Download on demand        --> 1 \n"
                           "Forced download           --> 2 \n"
        		 		   "Rescue download           --> 4 \n"
                           "Reset factory           --> 8 \n", (size_t)FIELD_INFO_SIZE);
#else
         strncpy(arg_info, "No download               --> 0 \n"
                           "Download on demand        --> 1 \n"
                           "Forced download           --> 2 \n"
        		 		   "Rescue download           --> 4 \n", (size_t)FIELD_INFO_SIZE);   
#endif /*CONFIG_IPL_ANDROID_X*/
         break;
      case STREAM_INFO_ELEMENT_LDR_KO:
         strncpy(arg_name, "Loader disabled", (size_t)FIELD_DESC_SIZE);
         strncpy(arg_info, "Enabled                   --> 0 \n"
                           "Disabled                  --> 1 \n", (size_t)FIELD_INFO_SIZE);
         break;
      case STREAM_INFO_ELEMENT_MODULE_ID:
         strncpy(arg_name, "Module ID (version)", (size_t)FIELD_DESC_SIZE);
         break;
      case STREAM_INFO_ELEMENT_INTERFACE:
         strncpy(arg_name, "Interface", (size_t)FIELD_DESC_SIZE);
         strncpy(arg_info, "INTLDR_IFACE_NAND     --> 0 \n"
                           "INTLDR_IFACE_USB      --> 1 \n"
                           "INTLDR_IFACE_FRONTEND --> 2 \n"
                           "INTLDR_IFACE_ETH      --> 3 \n"
                           "INTLDR_IFACE_WIFI     --> 4 \n", (size_t)FIELD_INFO_SIZE);
         break;
      case STREAM_INFO_ELEMENT_PROTOCOL:
         strncpy(arg_name, "Protocol", (size_t)FIELD_DESC_SIZE);
         strncpy(arg_info, "INTLDR_PROTO_SSU_SAT            --> 0 \n"
                           "INTLDR_PROTO_SSU_SAT_SCAN       --> 1 \n"
                           "INTLDR_PROTO_SSU_CAB            --> 2 \n"
                           "INTLDR_PROTO_SSU_CAB_SCAN       --> 3 \n"
                           "INTLDR_PROTO_SSU_CAB_SCAN_IMB   --> 4 \n"
               			   "INTLDR_PROTO_SSU_TER            --> 5 \n"
                           "INTLDR_PROTO_SSU_IP             --> 6 \n"
               			   "INTLDR_PROTO_TFTP               --> 7 \n"
                            "INTLDR_PROTO_FTP               --> 8 \n"
                            "INTLDR_PROTO_SFTP              --> 9 \n"
               			   "INTLDR_PROTO_HTTP               --> 10 \n"
               			   "INTLDR_PROTO_HTTPS              --> 11 \n"
               			   "INTLDR_PROTO_USB                --> 12 \n"
               				"INTLDR_PROTO_BKGD_APP          --> 13 \n",(size_t)FIELD_INFO_SIZE);
         break;
      case STREAM_INFO_ELEMENT_DISEQC_VER:
         strncpy(arg_name, "Diseqc version", (size_t)FIELD_DESC_SIZE);
         strncpy(arg_info, "INTLDR_DISEQC_NONE      --> 0 \n"
                           "INTLDR_DISEQC_TONEBURST --> 1 \n"
                           "INTLDR_DISEQC_1_0       --> 2 \n"
                           "INTLDR_DISEQC_VER_NR    --> 3 \n", (size_t)FIELD_INFO_SIZE);
         break;
      case STREAM_INFO_ELEMENT_22KHZ:
         strncpy(arg_name, "Module 22kHz", (size_t)FIELD_DESC_SIZE);
         strncpy(arg_info, " INTLDR_22KHZ_AUTO  --> 0 \n"
                           " INTLDR_22KHZ_ON    --> 1 \n"
                           " INTLDR_22KHZ_OFF   --> 2 \n", (size_t)FIELD_INFO_SIZE);
         break;
      case STREAM_INFO_ELEMENT_UBNR:
         strncpy(arg_name, "User Band number", (size_t)FIELD_DESC_SIZE);
         strncpy(arg_info, " Normal installation --> 0 \n"
                           " MDU installation    --> 1 to 8\n", (size_t)FIELD_INFO_SIZE);
         break;
      case STREAM_INFO_ELEMENT_UBFREQ:
         strncpy(arg_name, "User Band center frequency (kHz)", (size_t)FIELD_DESC_SIZE);
         break;
         
#ifdef CONFIG_DYNAMIC_USAGE_ID
      case STREAM_INFO_ELEMENT_USAGE_ID:
         strncpy(arg_name, "Usage ID", (size_t)FIELD_DESC_SIZE);
         strncpy(arg_info, "Disable overriding factory data --> -1 or 65535\n"
                           "Enable  overriding factory data --> 0 to 65534\n", (size_t)FIELD_INFO_SIZE);
         break;
#endif // CONFIG_DYNAMIC_USAGE_ID 
         
      case STREAM_INFO_ELEMENT_BANDWIDTH:
      case STREAM_INFO_ELEMENT_CODE_RATE_HP:
      case STREAM_INFO_ELEMENT_CODE_RATE_LP:
      case STREAM_INFO_ELEMENT_GUARD_INTERVAL:
      case STREAM_INFO_ELEMENT_HIERARCHY_INFO:
      case STREAM_INFO_ELEMENT_TRANSMISSION_MODE:
         strncpy(arg_name, "element to define", (size_t)FIELD_DESC_SIZE);
         strncpy(arg_info, "TBD \n"
                           "TBD \n", (size_t)FIELD_INFO_SIZE);
         break;
      case STREAM_INFO_ELEMENT_WIFI_SSID:
         strncpy(arg_name, "wifi ssid", FIELD_DESC_SIZE);
         strncpy(arg_info, "Max string size is 32\n", FIELD_INFO_SIZE);
         break;
      case STREAM_INFO_ELEMENT_WIFI_KEY:
         strncpy(arg_name, "wifi key", FIELD_DESC_SIZE);
         strncpy(arg_info, "Max string size is 64\n", FIELD_INFO_SIZE);
         break;
      case STREAM_INFO_ELEMENT_WIFI_PROTOCOL:
         strncpy(arg_name, "wifi protocol", FIELD_DESC_SIZE);
         strncpy(arg_info, "WIFI_NO_SECU      --> 0 \n"
                           "WIFI_WEP          --> 1 \n"
                           "WIFI_WPA_TKIP     --> 2 \n"
                           "WIFI_WPA2_PSKI    --> 3 \n"
                           "WIFI_WPS_PBC      --> 4 \n"
                           "WIFI_WPS_PIN      --> 5 \n", (size_t)FIELD_INFO_SIZE);
         break;
      case STREAM_INFO_ELEMENT_SWITCH_ENTRY:
         strncpy(arg_name, "Element switch / Lnb nr", (size_t)FIELD_DESC_SIZE);
         strncpy(arg_info, "TBD default      --> 0 \n"
                           "TBD other_value  --> other_value \n", (size_t)FIELD_INFO_SIZE);
         break;
      case STREAM_INFO_ELEMENT_TIMEOUT:
         strncpy(arg_name, "Timeout", (size_t)FIELD_DESC_SIZE);
         break;
      case STREAM_INFO_ELEMENT_PID:
         strncpy(arg_name, "PID", (size_t)FIELD_DESC_SIZE);
         break;
      case STREAM_INFO_ELEMENT_MODULE_TYPE:
         strncpy(arg_name, "Module type", (size_t)FIELD_DESC_SIZE);
         strncpy(arg_info, "Loader                   --> 2 \n"
                           "Firmware                 --> 3 \n"
							"Wrap                    --> 6 \n", (size_t)FIELD_INFO_SIZE);
         break;
      case STREAM_INFO_ELEMENT_FREQUENCY:
         strncpy(arg_name, "Satellite frequency (kHz)", (size_t)FIELD_DESC_SIZE);
         break;
      case STREAM_INFO_ELEMENT_SYMBOLRATE:
         strncpy(arg_name, "Symbol rate (kbaud)", (size_t)FIELD_DESC_SIZE);
         break;
      case STREAM_INFO_ELEMENT_MODULATION:
         strncpy(arg_name, "modulation", (size_t)FIELD_DESC_SIZE);
#ifdef CONFIG_NET_FE_CAB
		 strncpy(arg_info, "INTLDR_MODULATION_QAM16  -->  1\n"
				 "INTLDR_MODULATION_QAM32  -->  2\n"
				 "INTLDR_MODULATION_QAM64  -->  3\n"
				 "INTLDR_MODULATION_QAM128 -->  4\n"
				 "INTLDR_MODULATION_QAM256 --> 5\n"
				 "INTLDR_MODULATION_AUTO -->  6\n", (size_t)FIELD_INFO_SIZE);
#else
          strncpy(arg_info, "AUTO                     --> 6 \n", (size_t)FIELD_INFO_SIZE);
#endif
         break;
      case STREAM_INFO_ELEMENT_POLARIZATION:
         strncpy(arg_name, "Polarization", (size_t)FIELD_DESC_SIZE);
         strncpy(arg_info, "Vertical                  -->  0 \n"
                           "Horizontal                -->  1 \n", (size_t)FIELD_INFO_SIZE);
         break;
      case STREAM_INFO_ELEMENT_FE_TYPE:
         strncpy(arg_name, "Front End Type", (size_t)FIELD_DESC_SIZE);
         strncpy(arg_info, "DVBS                     -->  0 \n"
                           "DVBS2                    -->  1 \n"
                           "DVBS_AUTO                -->  2 \n", (size_t)FIELD_INFO_SIZE);
         break;
      case STREAM_INFO_ELEMENT_LNB_TYPE:
         strncpy(arg_name, "LNB type", (size_t)FIELD_DESC_SIZE);
         strncpy(arg_info, "Universal                --> 0 \n"
                           "Universal with DiSEqC    --> 1 \n"
                           "Optimized                --> 4 \n", (size_t)FIELD_INFO_SIZE);
         break;
      case STREAM_INFO_ELEMENT_LNB_POWER:
         strncpy(arg_name, "LNB power supply", (size_t)FIELD_DESC_SIZE);
         strncpy(arg_info, "STB 13V/18V              --> 0 \n"
                           "External                 --> 1 \n"
                           "STB 14V/19V              --> 2 \n", (size_t)FIELD_INFO_SIZE);
         break; 
      case STREAM_INFO_ELEMENT_LNB_OSC:
         if (pb_arg != NULL) {
            IPL_PRT_DEBUG_PRINTF("manually setting not implemented\n");
            return MCM_OK;
         } else {
            strncpy(arg_name, "LNB oscillators/cutoff frequency", (size_t)FIELD_DESC_SIZE);
            strncpy(arg_info, "[LO1 LO2 cutoff]\n", (size_t)FIELD_INFO_SIZE);
            break; 
         }
      case STREAM_INFO_ELEMENT_LNB_BANDS:
         if (pb_arg != NULL) {
            IPL_PRT_DEBUG_PRINTF("manually setting not implemented\n");
            return MCM_OK;
         } else {
            strncpy(arg_name, "Optimized LNB bands", (size_t)FIELD_DESC_SIZE);
            strncpy(arg_info, "[Fmin Fmax polarization LO]\n", (size_t)FIELD_INFO_SIZE);
            break; 
         }
      case STREAM_INFO_ELEMENT_URL:
         strncpy(arg_name, "URL", (size_t)FIELD_DESC_SIZE);
         strncpy(arg_info, "syntax: <protocol>://<IP_address>[:<port>]/<filename>\n", (size_t)FIELD_INFO_SIZE);
         break; 
      case STREAM_INFO_ELEMENT_IP_ADD:
         strncpy(arg_name, "IP", (size_t)FIELD_DESC_SIZE);
         strncpy(arg_info, "STB IP address\n", (size_t)FIELD_INFO_SIZE);
         break; 
      case STREAM_INFO_ELEMENT_NETMASK:
         strncpy(arg_name, "netmask", (size_t)FIELD_DESC_SIZE);
         strncpy(arg_info, "netmask value\n", (size_t)FIELD_INFO_SIZE);
         break;   
      case STREAM_INFO_ELEMENT_DEFAULT_GW:
         strncpy(arg_name, "gateway", (size_t)FIELD_DESC_SIZE);
         strncpy(arg_info, "Default Gateway address\n", (size_t)FIELD_INFO_SIZE);
         break;          
      case STREAM_INFO_ELEMENT_DNS_ADD_1:
         strncpy(arg_name, "DNS 1", (size_t)FIELD_DESC_SIZE);
         strncpy(arg_info, "DNS 1 address\n", (size_t)FIELD_INFO_SIZE);
         break;  
      case STREAM_INFO_ELEMENT_DNS_ADD_2:
         strncpy(arg_name, "DNS 2", (size_t)FIELD_DESC_SIZE);
         strncpy(arg_info, "DNS 2 address\n", (size_t)FIELD_INFO_SIZE);
         break;           
      case STREAM_INFO_ELEMENT_USE_DHCP:
         strncpy(arg_name, "Use of DHCP", (size_t)FIELD_DESC_SIZE);
         strncpy(arg_info, "Use Fix IP         --> 0 \n"
                            "Use DHCP params    --> 1 \n", (size_t)FIELD_INFO_SIZE);
         break;
	 case STREAM_INFO_ELEMENT_FEC_RATE:
         strncpy(arg_name, "fec_rate", (size_t)FIELD_DESC_SIZE);
         strncpy(arg_info, "INTLDR_FEC_1_2  -->  1\n"
				 "INTLDR_FEC_2_3  -->  2\n"
				 "INTLDR_FEC_3_4  -->  3\n"
				 "INTLDR_FEC_3_5  -->  4\n"
				 "INTLDR_FEC_4_5  -->  5\n"
				 "INTLDR_FEC_5_6  -->  6\n"
				 "INTLDR_FEC_6_7  -->  5\n"
				 "INTLDR_FEC_7_8  -->  8\n"
				 "INTLDR_FEC_8_9  -->  9\n"
				 "INTLDR_FEC_9_10 -->  19\n"
				 "INTLDR_FEC_AUTO -->  11\n", (size_t)FIELD_INFO_SIZE);
         break; 
     
#endif

#if defined (CONFIG_IPL_FE_SELECT_ENTRY_BY_E2P)
      case STREAM_INFO_ELEMENT_FRONT_END_ENTRY:
         strncpy(arg_name, "Front End Entry", (size_t)FIELD_DESC_SIZE);
         strncpy(arg_info, "Select HW Front End entry [0-Max?]\n", (size_t)FIELD_INFO_SIZE);
          break;
#endif
#if defined (CONFIG_IPL_OSD_CONFIGURATION_BY_APP)
      case STREAM_INFO_ELEMENT_VIDEO_OUTPUT_PORT:
         strncpy(arg_name, "Video ouput port", (size_t)FIELD_DESC_SIZE);
         strncpy(arg_info, "RGB                   -->  0 \n"
                           "YUV                   -->  1 \n"
                           "CVBS                  -->  2 \n"
                           "YC                    -->  3 \n"
                           "HDMI                  -->  4 \n", (size_t)FIELD_INFO_SIZE);
          break;
      case STREAM_INFO_ELEMENT_VIDEO_STANDARD:
         strncpy(arg_name, "Video Standard", (size_t)FIELD_DESC_SIZE);
         strncpy(arg_info, "NTSC                  -->  0 \n"
                           "NTSC_J                -->  1 \n"
                           "NTSC_443              -->  2 \n"
                           "PAL_BGDHI             -->  3 \n"
                           "PAL_M                 -->  4 \n"
                           "PAL_N                 -->  5 \n"
                           "PAL_N_C               -->  6 \n"
                           "SECAM                 -->  7 \n", (size_t)FIELD_INFO_SIZE);
          break;
      case STREAM_INFO_ELEMENT_VIDEO_RESOLUTION:
         strncpy(arg_name, "Video Resolution", (size_t)FIELD_DESC_SIZE);
         strncpy(arg_info, "720_576I_50_4_3       -->  0 \n"
                           "NTSC_J                -->  1 \n"
                           "720_576P_50_4_3       -->  2 \n"
                           "720_480I_59_4_3       -->  3 \n"
                           "720_480P_59_4_3       -->  4 \n"
                           "720_480I_50_4_3       -->  5 \n"
                           "720_480P_50_4_3       -->  6 \n"
                           "720_576I_50_16_9      -->  7 \n"
                           "720_576P_50_16_9      -->  8 \n"
                           "720_480I_59_16_9      -->  9 \n"
                           "720_480P_59_16_9      -->  10 \n"
                           "720_480I_50_16_9      -->  11 \n"
                           "720_480P_50_16_9      -->  12 \n"
                           "640_480P_60           -->  13 \n"
                           "1280_720P_50          -->  14 \n"
                           "1920_1080P_24         -->  15 \n"
                           "1920_1080P_25         -->  16 \n"
                           "1920_1080P_30         -->  17 \n"
                           "1920_1080I_50         -->  18 \n"
                           "1920_1080P_50         -->  19 \n"
                           "1280_720P_60          -->  20 \n"
                           "1920_1080I_60         -->  21 \n"
                           "1920_1080P_60         -->  22 \n", (size_t)FIELD_INFO_SIZE);
          break;
#endif
#if defined (CONFIG_SILENT_UPGRADE_MB)
      case STREAM_INFO_ELEMENT_BOOT_SCREEN_DISPLAY:
         strncpy(arg_name, "Boot Screen display", (size_t)FIELD_DESC_SIZE);
         strncpy(arg_info, "OFF                   -->  0 \n"
                           "ON                    -->  1 \n", (size_t)FIELD_INFO_SIZE);
          break;
#endif
      case STREAM_INFO_ELEMENT_GROUP_ID:
         strncpy(arg_name, "Group_Id", (size_t)FIELD_DESC_SIZE);
         strncpy(arg_info, "4 bytes field \n", (size_t)FIELD_INFO_SIZE);
          break;
      case STREAM_INFO_ELEMENT_VERIFIED_FLAG:
         strncpy(arg_name, "Verified Flag", (size_t)FIELD_DESC_SIZE);
         strncpy(arg_info, "Firmware backup parameters KO    -->  0 \n"
                           "Firmware backup parameters valid -->  1 \n", (size_t)FIELD_INFO_SIZE);
          break;
      default:
         strncpy(arg_name, "Out of Range Element", (size_t)FIELD_DESC_SIZE);
         IPL_PRT_DEBUG_PRINTF("%s", arg_name);
         return MCM_ERROR;
   }

   /* No argument : read the value */
   /********************************/
   if (pb_arg == NULL)
   {
      IPL_PRT_DEBUG_PRINTF("%s", arg_info);
      pBuffer = INTLDR_GetField((TE_INTLDR_MailboxField)wz_field);
      if(pBuffer)
      {
         switch(wz_field)
         {
#if defined CONFIG_IPL_FT_IMPLEMENTATION
          case INTLDR_DL_FIRMWARE_IDX:
          case INTLDR_DL_LOADER_IDX:
          case INTLDR_DL_RA_IDX:
            IPL_PRT_DEBUG_PRINTF("%s :  %d\r\n", arg_name, (uint32_t)*pBuffer);
            break;
#else  /* Default behavior to build a new project case*/
          case STREAM_INFO_ELEMENT_DL_REQUEST:
          case STREAM_INFO_ELEMENT_LDR_KO:
          case STREAM_INFO_ELEMENT_MODULE_TYPE:
          case STREAM_INFO_ELEMENT_FE_TYPE:
          case STREAM_INFO_ELEMENT_INTERFACE:
          case STREAM_INFO_ELEMENT_PROTOCOL:
          case STREAM_INFO_ELEMENT_22KHZ:
          case STREAM_INFO_ELEMENT_DISEQC_VER:
          case STREAM_INFO_ELEMENT_MODULATION:
          case STREAM_INFO_ELEMENT_SWITCH_ENTRY:
          case STREAM_INFO_ELEMENT_USE_DHCP:
          case STREAM_INFO_ELEMENT_WIFI_PROTOCOL:
		    case STREAM_INFO_ELEMENT_FEC_RATE:
          case STREAM_INFO_ELEMENT_POLARIZATION:
          case STREAM_INFO_ELEMENT_LNB_TYPE:
		    case STREAM_INFO_ELEMENT_UBNR:
               IPL_PRT_DEBUG_PRINTF("%s :  %d\n", arg_name, *pBuffer);
            break;
#ifdef CONFIG_DYNAMIC_USAGE_ID
          case STREAM_INFO_ELEMENT_USAGE_ID:
#endif //CONFIG_DYNAMIC_USAGE_ID
          case STREAM_INFO_ELEMENT_PID:
               IPL_PRT_DEBUG_PRINTF("%s :  %hu\n", arg_name, *((uint16_t*)pBuffer));
            break;
          case STREAM_INFO_ELEMENT_MODULE_ID:
          case STREAM_INFO_ELEMENT_TIMEOUT:
          case STREAM_INFO_ELEMENT_FREQUENCY:
          case STREAM_INFO_ELEMENT_SYMBOLRATE:
          case STREAM_INFO_ELEMENT_LNB_POWER:
          case STREAM_INFO_ELEMENT_UBFREQ:
            IPL_PRT_DEBUG_PRINTF("%s :  %lu\n", arg_name, *((uint32_t*)pBuffer));
            break;
          case STREAM_INFO_ELEMENT_LNB_OSC:
            IPL_PRT_DEBUG_PRINTF("%s :  [%lu %lu %lu]\n", arg_name,
                                                 ((TS_INTLDR_LnbDesc*)pBuffer)->low_freq_in_kHz,
                                                 ((TS_INTLDR_LnbDesc*)pBuffer)->high_freq_in_kHz,
                                                 ((TS_INTLDR_LnbDesc*)pBuffer)->switch_freq_in_kHz );
            break;
          case STREAM_INFO_ELEMENT_LNB_BANDS:
            { 
            uint8_t i=0;
            for(i=0 ; i<INTLDR_MAX_OPT_BANDS ; i++)
               IPL_PRT_DEBUG_PRINTF("%s :  [%lu %lu %lu %lu]\n", arg_name,
                                                 ((TS_INTLDR_LnbOptBand*)pBuffer)[i].min_freq_in_kHz,
                                                 ((TS_INTLDR_LnbOptBand*)pBuffer)[i].max_freq_in_kHz,
                                                 ((TS_INTLDR_LnbOptBand*)pBuffer)[i].polarization,
                                                 ((TS_INTLDR_LnbOptBand*)pBuffer)[i].LO_freq_in_kHz );
            }
            break;
          case STREAM_INFO_ELEMENT_URL:
          case STREAM_INFO_ELEMENT_IP_ADD:
          case STREAM_INFO_ELEMENT_NETMASK:
          case STREAM_INFO_ELEMENT_DEFAULT_GW:
          case STREAM_INFO_ELEMENT_DNS_ADD_1:
          case STREAM_INFO_ELEMENT_DNS_ADD_2:
             pBuffer[INTLDR_FILE_NAME_SIZE-1]='\0';
             IPL_PRT_DEBUG_PRINTF("%s :  %s\n", arg_name, pBuffer);
             break;
#endif
#if defined (CONFIG_IPL_FE_SELECT_ENTRY_BY_E2P)
      case STREAM_INFO_ELEMENT_FRONT_END_ENTRY:
            IPL_PRT_DEBUG_PRINTF("%s :  %d\n", arg_name, *pBuffer);
          break;
#endif
#if defined (CONFIG_IPL_OSD_CONFIGURATION_BY_APP)
      case STREAM_INFO_ELEMENT_VIDEO_OUTPUT_PORT:
      case STREAM_INFO_ELEMENT_VIDEO_STANDARD:
      case STREAM_INFO_ELEMENT_VIDEO_RESOLUTION:
            IPL_PRT_DEBUG_PRINTF("%s :  %d\n", arg_name, *pBuffer);
          break;
#endif
#if defined (CONFIG_SILENT_UPGRADE_MB)
      case STREAM_INFO_ELEMENT_BOOT_SCREEN_DISPLAY:
            IPL_PRT_DEBUG_PRINTF("%s :  %d\n", arg_name, *pBuffer);
          break;
#endif   
      case STREAM_INFO_ELEMENT_GROUP_ID:
            IPL_PRT_DEBUG_PRINTF("%s :  %.4s \n", arg_name, pBuffer);
          break; 
      case STREAM_INFO_ELEMENT_VERIFIED_FLAG  :
            IPL_PRT_DEBUG_PRINTF("%s :  %d\n", arg_name, *pBuffer);
          break;
      default:
            /* Default buffer should be a string*/
            IPL_PRT_DEBUG_PRINTF("%s :  %s\r\n", arg_name, pBuffer);
            break;
         }
      }
      else
      {
         return MCM_ERROR;
      }
   }
   /* With argument : write the value */
   /*************************************/
   else
   {
      if ( !strcmp(pb_arg,"\"\"") || !strcmp(pb_arg,"\'\'") )
      {
         *pb_arg = '\0';
      }

      if( ! INTLDR_SetField(wz_field, (uint8_t*)pb_arg) )
      {
         IPL_PRT_DEBUG_PRINTF("%s set as '%s'\r\n", arg_name, pb_arg);
      }
      else
      {
         IPL_PRT_DEBUG_PRINTF("set parameter failed\r\n");
         return MCM_ERROR;
      }
   }

   return MCM_OK;
}

/** @} */

