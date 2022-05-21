#ifndef INTLDR_H
#define INTLDR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define INTLDR_FILE_NAME_SIZE           0x410
#define INTLDR_MAX_WIFI_SSID            0x20
#define INTLDR_MAX_WIFI_KEY             0x40
#define INTLDR_FIX_IP_ADDRESS_SIZE      0x14
#define RAMBOX_MAX_ID                   0x40  
#define INTLDR_MAX_MODULE_NAME          0x22
#define  MAX_BUILD_TIME                 0x0B
#define HARDWARE_DESCRIPTOR_MAX_NB      0x0A


#define INTLDR_MAX_LNB_DESC             0x01

#define INTLDR_MAX_NUMBER_OF_LNBS       0x04
#define INTLDR_MAX_OPT_BANDS            0x04

#define MAX_MODULE_NAME                 0x22

typedef enum INTLDR_bool_t 
{
   INTLDR_FALSE          = 0x00 ,   
   INTLDR_TRUE           = 0x01      
}INTLDR_bool_t;

typedef enum INTLDR_erro_t 
{
   INTLDR_SUCESS          = 0x00 ,   
   INTLDR_INTERNAL_FAIL   = 0x01 , 
   INTLDR_INTERNAL_LAST   = 0x02      
}INTLDR_erro_t;

typedef enum INTLDR_GenericError_t
{
   INTLDR_ERROR_NONE                                              = 0x00,
   INTLDR_ERROR_RESERVED_00                                       = 0x01,
   INTLDR_ERROR_FLASH_ACCESS_ERROR                                = 0x02,
   INTLDR_ERROR_LOADER_INITIALIZATION_ERROR                       = 0x03,
   INTLDR_ERROR_INTERNAL_ERROR                                    = 0x04,
   INTLDR_ERROR_DOWNLOADABLE_MODULE_OLDER_THAN_CURRENT_FLASHED    = 0x05,
   INTLDR_ERROR_DOWNLOADED_MODULE_CONSISTENCY_ERROR               = 0x06,
   INTLDR_ERROR_RESERVED_01                                       = 0x07,
   INTLDR_ERROR_DOWNLOADED_MODULE_STORAGE_ERROR                   = 0x08,
   INTLDR_ERROR_RESERVED_03                                       = 0x09,
   INTLDR_ERROR_HEADER_CONSISTENCY_ERROR                          = 0x0A,
   INTLDR_ERROR_RESERVED_04                                       = 0x0B,
   INTLDR_ERROR_EEPROM_MEMORY_ACCESS_FAILED                       = 0x0C,
   INTLDR_ERROR_RESERVED_05                                       = 0x0D,
   INTLDR_ERROR_FRONTEND_TUNING_ERROR                             = 0x0E,
   INTLDR_ERROR_TIMEOUT_ERROR_DURING_ACQUISITION                  = 0x0F,
   INTLDR_ERROR_IO_ERROR_DURING_ACQUISITION                       = 0x10,
   INTLDR_ERROR_RESERVED_06                                       = 0x11,
   INTLDR_ERROR_SIGNATURE_INVALID                                 = 0x12,
   INTLDR_ERROR_RESERVED_07                                       = 0x13,
   INTLDR_ERROR_DOWNLOAD_ABORT                                    = 0x14,
   INTLDR_ERROR_LAST                                              = 0x15
}INTLDR_GenericError_t;


typedef enum {
    INTLDR_EXTENDED_OK,
    INTLDR_EXTENDED_ERROR_LOADER_INTEGRITY                               = 0x0101,
    INTLDR_EXTENDED_ERROR_MTD_ERASE_RELEASE                              = 0x0201,
    INTLDR_EXTENDED_ERROR_MTD_WRITE_RELEASE                              = 0x0202,
    INTLDR_EXTENDED_ERROR_MTD_COPY                                       = 0x0203,
    INTLDR_EXTENDED_ERROR_MTD_LOCK_FLASH                                 = 0x0204,
    INTLDR_EXTENDED_ERROR_MTD_UNLOCK_FLASH                               = 0x0205,
    INTLDR_EXTENDED_ERROR_MTD_READ_FLASH                                 = 0x0206,
    INTLDR_EXTENDED_ERROR_MTD_WRITE_FLASH                                = 0x0207,
    INTLDR_EXTENDED_ERROR_MTD_ERASE_FLASH                                = 0x0208,
    INTLDR_EXTENDED_ERROR_MTD_ERASE                                      = 0x0209,
    INTLDR_EXTENDED_ERROR_MTD_COMPARE                                    = 0x020A,
    INTLDR_EXTENDED_ERROR_MTD_OUT_OF_BAD_BLOCK                           = 0x020B,
    INTLDR_EXTENDED_ERROR_MTD_SET_BADBLOCK                               = 0x020C,
    INTLDR_EXTENDED_ERROR_MTD_GET_BADBLOCK                               = 0x020D,
    INTLDR_EXTENDED_ERROR_MTD_UNKNOWN_TYPE                               = 0x020E,
    INTLDR_EXTENDED_ERROR_FLA_INIT                                       = 0x0301,
    INTLDR_EXTENDED_ERROR_FE_INIT                                        = 0x0302,
    INTLDR_EXTENDED_ERROR_ETH_INIT                                       = 0x0303,
    INTLDR_EXTENDED_ERROR_DMX_INIT                                       = 0x0304,
    INTLDR_EXTENDED_ERROR_FUSION_INIT                                    = 0x0305,
    INTLDR_EXTENDED_ERROR_DHCP_INIT                                      = 0x0306,
    INTLDR_EXTENDED_ERROR_CRYPT_INIT                                     = 0x0307,
    INTLDR_EXTENDED_ERROR_SGN_INIT                                       = 0x0308,
    INTLDR_EXTENDED_ERROR_GFX_INIT                                       = 0x0309,
    INTLDR_EXTENDED_ERROR_STB_INIT                                       = 0x030A,
    INTLDR_EXTENDED_ERROR_ETH_NO_LINK                                    = 0x030B,
    INTLDR_EXTENDED_ERROR_WIFI_NO_LINK                                   = 0x030C,
    INTLDR_EXTENDED_ERROR_TLS_INIT                                       = 0x030D,
    INTLDR_EXTENDED_ERROR_MTD_INIT                                       = 0x030E,
    INTLDR_EXTENDED_ERROR_BCK_BSL                                        = 0x030F,
    INTLDR_EXTENDED_ERROR_SSU_IP_INIT                                    = 0x0310,
    INTLDR_EXTENDED_ETH_LINK                                             = 0x0311,
    INTLDR_EXTENDED_ERROR_INTERNAL_ERROR                                 = 0x0401,
    INTLDR_EXTENDED_ERROR_SAME_RELEASE_VERSION_ID                        = 0x0501,
    INTLDR_EXTENDED_ERROR_OLDER_RELEASE_VERSION_ID                       = 0x0502,
    INTLDR_EXTENDED_ERROR_NOT_URL_VERSION_ID                             = 0x0503,
    INTLDR_EXTENDED_ERROR_MTD_READ_RELEASE                               = 0x0601,
    INTLDR_EXTENDED_ERROR_RELEASE_INTEGRITY                              = 0x0602,
    INTLDR_EXTENDED_ERROR_MODULES_NOT_ALL_COMPLETE                       = 0x0701,
    INTLDR_EXTENDED_ERROR_TFTP_EOF                                       = 0x0702,
    INTLDR_EXTENDED_ERROR_NO_RAM_SPACE_MEMORY                            = 0x0801,
    INTLDR_EXTENDED_ERROR_NO_FLASH_SPACE_MEMORY                          = 0x0802,
    INTLDR_EXTENDED_ERROR_DATA_BLOCK_INVALID                             = 0x0A01,
    INTLDR_EXTENDED_ERROR_DATA_BLOCK_ADDR_INVALID                        = 0x0A02,
    INTLDR_EXTENDED_ERROR_STREAM_CHECKSUM_INVALID                        = 0x0A03,
    INTLDR_EXTENDED_ERROR_MODULE_SIZE_INVALID                            = 0x0A04,
    INTLDR_EXTENDED_ERROR_INCORRECT_DL_VERSION                           = 0x0A05,
    INTLDR_EXTENDED_ERROR_INCORRECT_COMM_VERSION                         = 0x0A06,
    INTLDR_EXTENDED_ERROR_INCORRECT_MODULE_TYPE                          = 0x0A07,
    INTLDR_EXTENDED_ERROR_HEADER_INVALID                                 = 0x0A08,
    INTLDR_EXTENDED_ERROR_E2P_INIT                                       = 0x0C01,
    INTLDR_EXTENDED_ERROR_MTD_READ_INTLDR                                = 0x0C02,
    INTLDR_EXTENDED_ERROR_MTD_WRITE_INTLDR                               = 0x0C03,
    INTLDR_EXTENDED_ERROR_MTD_READ_BACKUP                                = 0x0C04,
    INTLDR_EXTENDED_ERROR_MTD_WRITE_BACKUP                               = 0x0C05,
    INTLDR_EXTENDED_ERROR_MTD_DWLD_PARAMS                                = 0x0C06,
    INTLDR_EXTENDED_ERROR_MTD_VERIFY_AFTER_WRITE                         = 0x0D01,
    INTLDR_EXTENDED_ERROR_FE_BAD_PARAMS                                  = 0x0E01,
    INTLDR_EXTENDED_ERROR_FE_NOT_LOCKED                                  = 0x0E02,
    INTLDR_EXTENDED_ERROR_FE_SCAN_SETUP                                  = 0x0E03,
    INTLDR_EXTENDED_ERROR_FE_SCAN_ERROR                                  = 0x0E04,
    INTLDR_EXTENDED_ERROR_FE_SCAN_END                                    = 0x0E05,
    INTLDR_EXTENDED_ERROR_DHCP_BOUND_FAIL                                = 0x0E06,
    INTLDR_EXTENDED_ERROR_DSI_INVALID                                    = 0x0E07,
    INTLDR_EXTENDED_ERROR_DSI_TIMEOUT                                    = 0x0E08,
    INTLDR_EXTENDED_ERROR_TARGET_ID_NOT_COMPATIBLE                       = 0x0E09,
    INTLDR_EXTENDED_ERROR_FILE_NAME_NOT_COMPATIBLE                       = 0x0E0A,
    INTLDR_EXTENDED_ERROR_TFTP_ENETUNREACH                               = 0x0E0B,
    INTLDR_EXTENDED_ERROR_TFTP_ENOENT                                    = 0x0E0C,
    INTLDR_EXTENDED_ERROR_TFTP_DOWNLOAD_FAILURE                          = 0x0E0D,
    INTLDR_EXTENDED_ERROR_FTP_ENETUNREACH                                = 0x0E0E,
    INTLDR_EXTENDED_ERROR_FTP_ENOENT                                     = 0x0E0F,
    INTLDR_EXTENDED_ERROR_FTP_AUTH_FAILURE                               = 0x0E10,
    INTLDR_EXTENDED_ERROR_FTP_DOWNLOAD_FAILURE                           = 0x0E11,
    INTLDR_EXTENDED_ERROR_HTTP_ENETUNREACH                               = 0x0E12,
    INTLDR_EXTENDED_ERROR_HTTP_ENOENT                                    = 0x0E13,
    INTLDR_EXTENDED_ERROR_HTTP_AUTH_FAILURE                              = 0x0E14,
    INTLDR_EXTENDED_ERROR_HTTP_DOWNLOAD_FAILURE                          = 0x0E15,
    INTLDR_EXTENDED_ERROR_URL_NOT_DEFINED                                = 0x0E16,
    INTLDR_EXTENDED_ERROR_URL_INVALID                                    = 0x0E17,
    INTLDR_EXTENDED_ERROR_UNSUPPORTED_PROTOCOL                           = 0x0E18,
    INTLDR_EXTENDED_ERROR_TARGET_ID_INVALID                              = 0x0E19,
    INTLDR_EXTENDED_ERROR_WIFI_BAD_PARAMETRS                             = 0x0E1A,
    INTLDR_EXTENDED_ERROR_IP_CURL_ABORT                                  = 0x0E1B,
    INTLDR_EXTENDED_ERROR_USB_READ_FAILURE                               = 0x0E1C,
    INTLDR_EXTENDED_ERROR_USB_VALIDITY_DATE                              = 0x0E1D,
    INTLDR_EXTENDED_ERROR_NTP_NOT_AVAILABLE                              = 0x0E1E,
    INTLDR_EXTENDED_ERROR_INVALID_MODULES_NUMBER                         = 0x0E1F,
    INTLDR_EXTENDED_ERROR_DII_TIMEOUT                                    = 0x0F01,
    INTLDR_EXTENDED_ERROR_TFTP_ETIMEDOUT                                 = 0x0F02,
    INTLDR_EXTENDED_ERROR_DDB_TIMEOUT                                    = 0x0F03,
    INTLDR_EXTENDED_ERROR_HTTP_ETIMEDOUT                                 = 0x0F04,
    INTLDR_EXTENDED_ERROR_NIT_TIMEOUT                                    = 0x0F05,
    INTLDR_EXTENDED_ERROR_IMB_TIMEOUT                                    = 0x0F06,
    INTLDR_EXTENDED_ERROR_IMB_DSMCCINBAND_INVALID                        = 0x0F07,
    INTLDR_EXTENDED_ERROR_IMB_FREQUENCY_UNDEFINED                        = 0x0F08,
    INTLDR_EXTENDED_ERROR_IMB_PID_UNDEFINED                              = 0x0F09,
    INTLDR_EXTENDED_ERROR_IMB_BAD_PLANTID                                = 0x0F0A,
    INTLDR_EXTENDED_ERROR_IMB_BAD_PLATFORMID                             = 0x0F0B,
    INTLDR_EXTENDED_ERROR_IMB_BAD_MODELID                                = 0x0F0C,
    INTLDR_EXTENDED_ERROR_IMB_DESTINATION_MESSAGE_INVALID                = 0x0F0D,
    INTLDR_EXTENDED_ERROR_IMB_LOCATOR_INVALID                            = 0x0F0E,
    INTLDR_EXTENDED_ERROR_PMT_TIMEOUT                                    = 0x0F0F,
    INTLDR_EXTENDED_ERROR_PAT_TIMEOUT                                    = 0x0F10,
    INTLDR_EXTENDED_ERROR_GLOBAL_TIMEOUT                                 = 0x0F11,
    INTLDR_EXTENDED_ERROR_DMX_SECTION                                    = 0x1001,
    INTLDR_EXTENDED_ERROR_TFTP_EIO                                       = 0x1001,
    INTLDR_EXTENDED_ERROR_SSU_EIO                                        = 0x1001,
    INTLDR_EXTENDED_ERROR_DII_INVALID                                    = 0x1001,
    INTLDR_EXTENDED_ERROR_DDB_INVALID                                    = 0x1001,
    INTLDR_EXTENDED_ERROR_HTTP_EIO                                       = 0x1001,
    INTLDR_EXTENDED_ERROR_NIT_INVALID                                    = 0x1002,
    INTLDR_EXTENDED_ERROR_IMB_INVALID                                    = 0x1003,
    INTLDR_EXTENDED_ERROR_PMT_INVALID                                    = 0x1004,
    INTLDR_EXTENDED_ERROR_PAT_INVALID                                    = 0x1005,
    INTLDR_EXTENDED_ERROR_DSMCCD_CANCEL_IS_RECEIVED                      = 0x1101,
    INTLDR_EXTENDED_ERROR_SIGNATURE_INVALID                              = 0x1201,
    INTLDR_EXTENDED_ERROR_KEY_INDEX_INVALID                              = 0x1301,
    INTLDR_EXTENDED_ERROR_GET_KEY                                        = 0x1302,
    INTLDR_EXTENDED_ERROR_GET_RSA_KEY                                    = 0x1303,
    INTLDR_EXTENDED_ERROR_DOWNLOAD_ABORTED                               = 0x1401,
    INTLDR_EXTENDED_ERROR_POWER_FAILURE                                  = 0x1402,
    INTLDR_EXTENDED_ERROR_UNDEFINED                                      = 0x1601,
}INTLDR_ExtendedError_t;

typedef enum INTLDR_ModuleType_t
{
   INTLDR_MODULE_TYPE_UNKNOWN   = 0xFF ,
   INTLDR_MODULE_TYPE_DA2       = 0x01 ,
   INTLDR_MODULE_TYPE_LOADER    = 0x02 ,
   INTLDR_MODULE_TYPE_KERNEL    = 0x03 ,
   INTLDR_MODULE_TYPE_ROOT_FS   = 0x04 ,
   INTLDR_MODULE_TYPE_RA        = 0x05 ,
   INTLDR_MODULE_TYPE_WRAP      = 0x06 ,
   INTLDR_MODULE_TYPE_KERNEL2   = 0x07 ,
   INTLDR_MODULE_TYPE_LAST      = 0x08    
} INTLDR_ModuleType_t;




typedef enum INTLDR_Iface_t
{
   INTLDR_IFACE_NAND     = 0x00 ,   
   INTLDR_IFACE_USB      = 0x01 ,    
   INTLDR_IFACE_FRONTEND = 0x02 ,
   INTLDR_IFACE_ETH      = 0x03 ,     
   INTLDR_IFACE_WIFI     = 0x04 ,    
   INTLDR_IFACE_LAST     = 0x05      
}INTLDR_Iface_t;




typedef enum INTLDR_Proto_t
{
   INTLDR_PROTO_SSU_SAT           = 0x00,
   INTLDR_PROTO_SSU_SAT_SCAN      = 0x01,
   INTLDR_PROTO_SSU_CAB           = 0x02,
   INTLDR_PROTO_SSU_CAB_SCAN      = 0x03,
   INTLDR_PROTO_SSU_CAB_SCAN_IMB  = 0x04,
   INTLDR_PROTO_SSU_TER           = 0x05, 
   INTLDR_PROTO_SSU_IP            = 0x06,
   INTLDR_PROTO_TFTP              = 0x07,
   INTLDR_PROTO_FTP               = 0x08,
   INTLDR_PROTO_SFTP              = 0x09,
   INTLDR_PROTO_HTTP              = 0x0A,
   INTLDR_PROTO_HTTPS             = 0x0B,
   INTLDR_PROTO_FILESYSTEM        = 0x0C,
   INTLDR_PROTO_DEVICE            = 0x0D,
   INTLDR_PROTO_BKGD_APP          = 0x0E,
   INTLDR_PROTO_ANY               = 0x0F,
   INTLDR_PROTO_LAST              = 0x10
} INTLDR_Proto_t;


typedef enum INTLDR_RequestType_t
{

   INTLDR_DOWNLOAD_NO_REQUEST            = 0x00,

   INTLDR_DOWNLOAD_RELEASE_REQUEST       = 0x01,

   INTLDR_DOWNLOAD_USER_REQUEST          = 0x02,
   
   INTLDR_DOWNLOAD_RESERVED_0            = 0x03,

   INTLDR_DOWNLOAD_SSA_APP_REQUEST       = 0x04,

   INTLDR_DOWNLOAD_RECOVERY_REQUEST      = 0x05,
   
   INTLDR_DOWNLOAD_RELEASE_SSU_REQUEST   = 0x06,
   
   INTLDR_DOWNLOAD_RESERVED_1            = 0x07,
   
   INTLDR_DOWNLOAD_REQUEST_RESET_FACTORY = 0x08,
   
   INTLDR_DOWNLOAD_REQUEST_LAST          = 0x09,
   
} INTLDR_RequestType_t;


typedef enum INTLDR_Video_output_port_t
{
   OUTPUT_PORT_RGB    = 0x00,  /**< RGB */
   OUTPUT_PORT_YUV    = 0x01,  /**< YUV */
   OUTPUT_PORT_CVBS   = 0x02,  /**< CVBS */
   OUTPUT_PORT_YC     = 0x03,  /**< YC */
   OUTPUT_PORT_HDMI   = 0x04,  /**< HDMI */
   OUTPUT_PORT_LAST   = 0x05   /**< Number of enumerated values. */

} INTLDR_Video_output_port_t;

typedef enum INTLDR_Video_Standard_t
{
    VIDEO_STANDARD_NTSC          = 0x00,   /**< NTSC.     */
    VIDEO_STANDARD_NTSC_J        = 0x01,   /**< NTSC J.   */
    VIDEO_STANDARD_NTSC_443      = 0x02,   /**< NTSC 443. */
    VIDEO_STANDARD_PAL_BGDHI     = 0x03,   /**< PAL bgdhi.*/
    VIDEO_STANDARD_PAL_M         = 0x04,   /**< PAL M.    */
    VIDEO_STANDARD_PAL_N         = 0x05,   /**< PAL N.    */
    VIDEO_STANDARD_PAL_N_C       = 0x06,   /**< PAL N C.  */
    VIDEO_STANDARD_SECAM         = 0x07,   /**< SECAM.    */
    VIDEO_STANDARD_LAST          = 0x08    /**< Number of enumerated values. */
} INTLDR_Video_Standard_t;


typedef enum INTLDR_Video_output_resolution_t 
{
    VIDEO_OUTPUT_720_576I_50_4_3   = 0x00,  /**< 576i 50Hz with 4/3 aspect ratio. */
    VIDEO_OUTPUT_720_576P_50_4_3   = 0x01,  /**< 576p 50Hz with 4/3 aspect ratio. */
    VIDEO_OUTPUT_720_480I_59_4_3   = 0x02,  /**< 480i 59.94Hz with 4/3 aspect ratio. */
    VIDEO_OUTPUT_720_480P_59_4_3   = 0x03,  /**< 480p 59.94Hz with 4/3 aspect ratio. */
    VIDEO_OUTPUT_720_480I_50_4_3   = 0x04,  /**< 480i 50Hz with 4/3 aspect ratio. */
    VIDEO_OUTPUT_720_480P_50_4_3   = 0x05,  /**< 480p 50Hz with 4/3 aspect ratio. */
    VIDEO_OUTPUT_720_576I_50_16_9  = 0x06,  /**< 576i 50Hz with 16/9 aspect ratio. */
    VIDEO_OUTPUT_720_576P_50_16_9  = 0x07,  /**< 576p 50Hz with 16/9 aspect ratio. */
    VIDEO_OUTPUT_720_480I_59_16_9  = 0x08,  /**< 480i 59.94Hz with 16/9 aspect ratio. */
    VIDEO_OUTPUT_720_480P_59_16_9  = 0x09,  /**< 480p 59.94Hz with 16/9 aspect ratio. */
    VIDEO_OUTPUT_720_480I_50_16_9  = 0x0A,  /**< 480i 50Hz with 16/9 aspect ratio. */
    VIDEO_OUTPUT_720_480P_50_16_9  = 0x0B,  /**< 480p 50Hz with 16/9 aspect ratio. */
    VIDEO_OUTPUT_640_480P_60       = 0x0C,  /**< 640x480p 60Hz. */
    VIDEO_OUTPUT_1280_720P_50      = 0x0D,  /**< 720p 50Hz. */
    VIDEO_OUTPUT_1920_1080P_24     = 0x0E,  /**< 1080p 24Hz. */
    VIDEO_OUTPUT_1920_1080P_25     = 0x0F,  /**< 1080p 25Hz. */
    VIDEO_OUTPUT_1920_1080P_30     = 0x10,  /**< 1080p 30Hz. */
    VIDEO_OUTPUT_1920_1080I_50     = 0x11,  /**< 1080i 50Hz. */
    VIDEO_OUTPUT_1920_1080P_50     = 0x12,  /**< 1080p 50Hz. */
    VIDEO_OUTPUT_1280_720P_60      = 0x13,  /**< 720p 60Hz. */
    VIDEO_OUTPUT_1920_1080I_60     = 0x14,  /**< 1080i 60Hz. */
    VIDEO_OUTPUT_1920_1080P_60     = 0x15,  /**< 1080p 60Hz. */
    VIDEO_OUTPUT_LAST              = 0x16   /**< Number of enumerated values. */
} INTLDR_Video_output_resolution_t;


typedef enum INTLDR_DownloadResult_t 
{
   DOWNLOAD_SUCCESSFUL               = 0x00,
   DOWNLOAD_ABORTED_GLOBAL_TIMEOUT   = 0x01,
   DOWNLOAD_ABORTED_ERROR            = 0x02,
   DOWNLOAD_ERROR_RESERVED_0         = 0x03,
   DOWNLOAD_ABORTED_POWER_FAIL       = 0x04,
   NO_DOWNLOAD_OCCURED               = 0xFF
} INTLDR_DownloadResult_t;

typedef enum INTLDR_Boot_screen_display_t 
{
   BOOT_SCREEN_DISPLAY_OFF = 0x00,     /**< No OSD display during download */
   BOOT_SCREEN_DISPLAY_ON  = 0x01      /**< Boot Screen is activated */
} INTLDR_Boot_screen_display_t;

typedef enum INTLDR_Polarization_t 
{
   INTLDR_POLARIZATION_V = 0x00,
   INTLDR_POLARIZATION_H = 0x01
} INTLDR_Polarization_t;

typedef enum {
    INTLDR_VOLTAGE_13   = 0x00,
    INTLDR_VOLTAGE_18   = 0x01,
    INTLDR_VOLTAGE_OFF  = 0x02,
    INTLDR_VOLTAGE_LAST = 0x03
} INTLDR_Voltage_t;

/** Frontend SEC tone modes.
 */
typedef enum {
    INTLDR_TONE_ON         = 0x00,
    INTLDR_TONE_OFF        = 0x01,
    INTLDR_TONE_MODE_LAST  = 0x02     /**< Number of enumerated values. */
} INTLDR_tone_mode_t;


typedef struct MB_INTLDR_LnbDesc_t
{
   uint32_t low_freq_in_kHz;         /**< local low freq. oscilator */
   uint32_t high_freq_in_kHz;        /**< local high freq. oscilator */
   uint32_t switch_freq_in_kHz;      /**< frequence de coupure */
} MB_INTLDR_LnbDesc_t;

typedef struct MB_INTLDR_LnbOptBand_t
{
   uint32_t min_freq_in_kHz;              /**< LNB minimum frequency (kHz) */
   uint32_t max_freq_in_kHz;              /**< LNB maximum frequency (kHz) */
   INTLDR_Polarization_t polarization;    /**< LNB polarization */
   uint32_t LO_freq_in_kHz;               /**< LNB low frequency (kHz) */
   INTLDR_Voltage_t voltage;              /**< LNB voltage */
   INTLDR_tone_mode_t tone_mode;          /**< LNB tone_mode */
} MB_INTLDR_LnbOptBand_t;


typedef  struct  MB_DownloadRequest_t
{
    INTLDR_RequestType_t DownloadRequest ;
    uint8_t    Reserved ;
    uint16_t   LDR_KO ;
    uint32_t   ModuleId ;
    uint32_t   Timeout ;
    uint32_t   Crc32 ;
}MB_DownloadRequest_t ; /*16 bytes */


typedef struct  TS_INTLDR_Dhcp 
{
    uint8_t Reserved[100];     /**< 100 bytes unused*/
}TS_INTLDR_Dhcp;

typedef struct  TS_INTLDR_StaticIP_params_t
 {
    char IP_address[INTLDR_FIX_IP_ADDRESS_SIZE];    /**< IP address of the interface */
    char netmask[INTLDR_FIX_IP_ADDRESS_SIZE];       /**< Netmask associated to the IP address above */
    char default_gw[INTLDR_FIX_IP_ADDRESS_SIZE];    /**< Default gateway IP address */
    char DNS_address1[INTLDR_FIX_IP_ADDRESS_SIZE];  /**< DNS server IP addresses */
    char DNS_address2[INTLDR_FIX_IP_ADDRESS_SIZE];  /**< DNS server IP addresses */
}TS_INTLDR_StaticIP_params_t; /* 100 */


typedef union {
   TS_INTLDR_Dhcp    Dhcp;                /**< DHCP on */
   TS_INTLDR_StaticIP_params_t Static_Ip;   /**< static on */
}TU_INTLDR_Http_Config;


typedef struct MB_DownloadSettingParameters_IP_t
{
   INTLDR_bool_t                Use_DHCP;                  
   TU_INTLDR_Http_Config        Http_Config;               
   char                         url[INTLDR_FILE_NAME_SIZE]; 
} MB_DownloadSettingParameters_IP_t;


typedef struct MB_DownloadSettingParameters_WIFI_t
{
   INTLDR_bool_t          Use_DHCP;
   TU_INTLDR_Http_Config  Http_Config;
   char                   url[INTLDR_FILE_NAME_SIZE];
   uint32_t               wifi_protocol;     
   char                   wifi_ssid[INTLDR_MAX_WIFI_SSID];
   char                   wifi_key[INTLDR_MAX_WIFI_KEY];
} MB_DownloadSettingParameters_WIFI_t;


typedef struct MB_DownloadSettingParameters_SAT_t
{
   /* transponder information */
   uint32_t frequency;        /* kHz*/
   uint32_t symbol_rate;      /* kbaud */

   uint8_t modulation;       /* a value of type INTLDR_Modulation_t */
   uint8_t polarization;     /* a value of type INTLDR_Polarization_t */
   uint8_t FecRate;          /* a value of type INTLDR_FecRate_t */
   uint8_t FE_type;           /* a value of type opal_fe_type_t */

   /* LNB config */
   uint8_t lnb_type;          /* a value of type INTLDR_LnbType_t */
   uint8_t Modul22K;         /* a value of type INTLDR_22Khz_t*/
   uint8_t DiseqcVer;         /* a value of type INTLDR_DiseqcVer_t */
   uint8_t LnbNr;             /* Lnb Entry for Diseqc switch (not unicable) */

   MB_INTLDR_LnbDesc_t lnb_osc[INTLDR_MAX_LNB_DESC];  /* LNB oscillators */
   MB_INTLDR_LnbOptBand_t lnb_bands[INTLDR_MAX_OPT_BANDS];
   uint8_t reserved[4 * INTLDR_MAX_OPT_BANDS];      /* keep mailbox compatiblity with previous (MB_INTLDR_LnbOptBand_t definition) */

   uint32_t UbFreq;           /* UserBand BIS Frequency assigned to the STB */

   uint8_t UbNr;             /* 0 => no unicable, from 1 to 16: UserBand number assigned to the STB */
   uint8_t lnb_power;         /* a value of type INTLDR_LnbPower_t */
   uint8_t FrontEndEntry;     /* Hardware Front End entry selection. First entry is 0x0*/
} MB_DownloadSettingParameters_SAT_t; 

typedef struct MB_DownloadSettingParameters_CAB_t
{
   /* transponder information */
   uint32_t frequency;        /* kHz*/
   uint32_t symbol_rate;      /* kbaud */

   uint8_t modulation;       /* a value of type INTLDR_Modulation_t */
   uint8_t FecRate;          /* a value of type INTLDR_FecRate_t */
} MB_DownloadSettingParameters_CAB_t; 


typedef struct MB_DownloadSettingParameters_TER_t 
{
   uint32_t frequency;     /* in kHz */

   uint8_t fe_type;        /* a value of type opal_fe_type_t */
   uint8_t bandwidth;      /* a value of type INTLDR_TerBandwidth_t */
   uint8_t code_rate_hp;   /* a value of type INTLDR_FecRate_t */
   uint8_t code_rate_lp;   /* a value of type INTLDR_FecRate_t */

   uint8_t modulation;     /* a value of type INTLDR_Modulation_t */
   uint8_t guard_interval; /* a value of type INTLDR_TerGuardInterval_t */
   uint8_t hierarchy_info; /* a value of type INTLDR_TerHierarchyInfo_t */
   uint8_t tx_mode;        /* a value of type INTLDR_TerTransmissionMode_t */
} MB_DownloadSettingParameters_TER_t;


typedef struct MB_DownloadSettingParameters_USB_t
{
   char reserved;
} MB_DownloadSettingParameters_USB_t;


typedef  struct MB_DownloadSettingsParameters_t
{
    uint16_t PID;
    INTLDR_Iface_t  Interface;
    INTLDR_Proto_t Protocol;
    INTLDR_ModuleType_t ModuleType;
    uint8_t VerifiedFlag;
    uint8_t reserved [2];
    union
    {
        MB_DownloadSettingParameters_IP_t  ip;
        MB_DownloadSettingParameters_WIFI_t   wifi;
        MB_DownloadSettingParameters_SAT_t   sat;
        MB_DownloadSettingParameters_TER_t   ter;
        MB_DownloadSettingParameters_CAB_t  cab;
        MB_DownloadSettingParameters_USB_t  usb;
        char rawbytes [1992];
    };
uint32_t   reserved2 ;
uint32_t   Crc32;
} MB_DownloadSettingsParameters_t; /*2008 bytes*/


typedef struct
{
   INTLDR_Video_output_port_t       VideoOutputPort;         /**< video output port */
   INTLDR_Video_Standard_t          VideoStandard;           /**< video standard */
   INTLDR_Video_output_resolution_t VideoOutputResolution;   /**< video output resolution */
   uint8_t padding[3];                                       /**< padding */
   uint16_t Crc16;                                           /**< CRC */
} MB_VideoParameters_t;


typedef struct MB_DownloadReportParameters_t
{
   INTLDR_RequestType_t     DownloadType;
   INTLDR_DownloadResult_t  DownloadResult;
   INTLDR_GenericError_t    DownloadError;           /* enum type INTLDR_GenericError_t*/
   uint8_t  ExtendedDownloadError;  /* (enum type INTLDR_ExtendedError_t) % 256 */
   uint32_t Crc32;
} MB_DownloadReportParameters_t;

typedef struct
{
   INTLDR_Boot_screen_display_t BootScreenDisplay;    /**< display boot screen flag */
   uint8_t NewVersionAvailable;  /**< new version available flag */
   uint32_t Crc32;               /**< CRC32 */
} MB_SignallingBits_t;

typedef  struct INTLDR_MB_IntldrInfo_t
{
    MB_DownloadRequest_t Request ; /*16   bytes*/
    MB_DownloadSettingsParameters_t  Parameters ; /*2008 bytes*/
    MB_VideoParameters_t Video ; /*8 bytes*/
    MB_DownloadReportParameters_t Report ; /* 8 bytes */
    MB_SignallingBits_t SignalingBits /*8 bytes */
    
} INTLDR_MB_IntldrInfo_t ;

typedef struct
{
   uint16_t                           aes_key_index;
   uint16_t                           aes_key_index_C1;
   uint16_t                           rsa_key_index;
   uint16_t                           rsa_key_index_C1;
   MB_DownloadSettingsParameters_t BackupParametersAppli;
   MB_DownloadSettingsParameters_t BackupParametersLDR;
} INTLDR_MB_BackupInfo_t;

typedef struct version_header_t {
   uint32_t version;       /**< module version in big endian */
   uint8_t  reserved[16];  /**< reserved for future use */
   uint32_t src_version;   /**< source version - only for incremental update */
   uint32_t crc32_patch;   /**< crc32 computed on datas after patch - only for incremental update */
   uint32_t crc32;         /**< version header checksum */
} version_header_t;

/* Rom toolbox  */
typedef struct rom_bvn_t
{
    version_header_t fw ; /* 0x20 bytes */
    version_header_t dl ; /* 0x20 bytes */
} rom_bvn_t ;

typedef struct TS_NVRAM_BSL_Area_t
{
    INTLDR_MB_IntldrInfo_t Intldr_Info;  /*Offset : 0x0000−>0x07FF:2047  Bytes*/
    rom_bvn_t bvn ;                      /*Offset : 0x0800−>0x0840:64    Bytes*/
    INTLDR_MB_BackupInfo_t Backup_Info ; /*Offset : 0x1000−>0x1FFF :4096 Bytes*/
}TS_NVRAM_BSL_Area_t ;/*8192 Bytes*/

/* Rambox data and stucture */

typedef struct
{
   uint8_t HwId[2];     /**< Target Hardware Id */
   uint8_t UsageId[2];  /**< Target Usage Id */
} target_descriptors_t;




typedef struct module_header_hg03_s {
   uint32_t             size;                   /**< Data size */
   uint32_t             magic;                  /**< Magic number */
   uint8_t              module_type;            /**< Type of module */
   uint8_t              storage_device;         /**< ROM device for module storage */
   uint16_t             properties;             /**< Bit field to define module properties */
   uint8_t              oui[3];                 /**< IEEE manufacturer */
   uint8_t              hardware_descriptor_nb; /**< Number of descriptor (couple HwId + UsageId) */
   target_descriptors_t target_desc[8];         /**< hardware descriptors */
   uint32_t             load_address;           /**< Address in RAM to be copied to */
   uint32_t             store_address;          /**< Offset in ROM device (nor / nand / emmc) */
   uint8_t              aes_key_index;          /**< AES key index (deciphering) */
   uint8_t              rsa_key_index;          /**< RSA key index (signature) */
   uint8_t              name[MAX_MODULE_NAME];  /**< Module string identifier */
   uint32_t             crc32_header;           /**< crc32 on module header */
   uint32_t             version;                /**< module version */
   uint8_t              reserved_3[16];
   uint32_t             src_version;            /**< Source version (incremental patch) */
   uint32_t             crc32_patch;            /**< CRC32 of module after patch application (incremental patch) */
   uint32_t             crc32_version;          /**< crc32 on version header */
} module_header_hg03_t; /* 128 bytes */


typedef struct rambox_s {
   uint32_t              magic;  /**< magic number */
   module_header_hg03_t  dl;     /**< Downloader module header */
   module_header_hg03_t  dl_ver; /**< Downloader version header */
   module_header_hg03_t  fw;     /**< Firmware module header */
   module_header_hg03_t  fw_ver; /**< Firmware version  header */
   module_header_hg03_t  bs;     /**< Bootscreen module header */
   module_header_hg03_t  bs_ver; /**< Bootscreen version header */
   module_header_hg03_t  diag;     /**< Diagnostic module header */
   module_header_hg03_t  diag_ver; /**< Diagnostic version header */
   char                  s2_id[RAMBOX_MAX_ID]; /**< ssbl id string */
   char                  s3_id[RAMBOX_MAX_ID]; /**< bootloader id string */
   uint32_t              crc32;  /**< checksum on ram mailbox */
} rambox_t;


/* Inteldr interfaces API */

INTLDR_erro_t INTLDR_Init(void);
INTLDR_erro_t INTLDR_Term(void);



#ifdef __cplusplus
}
#endif
#endif /* INTLDR_H */
