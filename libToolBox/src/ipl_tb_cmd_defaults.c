
/** @brief no comment */
/*-----------------------------------------------------------------------------
 FILE DOCUMENTATION
-----------------------------------------------------------------------------*/
/**
 * @note    Copyright 2000-2009 Sagem Communications SAS All rights reserved
 *
 * @file    ipl_tb_cmd_defaults.c
 * @brief   m_IPL_TB_CmdDefaults
 * @version Revision of last commit : $Rev: 100 $
 * @date    Date of last commit     : $Date: 2013-01-13 10:45:30 +0100 (Thu, 13 Jan 2013) $
 * @author  Author of last commit   : $Author: g600860 $
 * @addtogroup IPL_FE
 * @{
 */

/*-----------------------------------------------------------------------------
 CURRENT COMPONENT               INCLUSIONS in functional/alphabetical order
-----------------------------------------------------------------------------*/
#include "ipl_tb_l.h"
#include "ipl_tb_internal.h"
typedef enum {
    OPAL_FE_INSTALL_INDIVIDUAL,          /**< Installation on individual dish. */
    OPAL_FE_INSTALL_INDIVIDUAL_DISEQC,   /**< Installation on individual dish, piloted by DiSEqC commands. */
    OPAL_FE_INSTALL_COLLECTIVE,          /**< Collective installation : no need for LNB,
                                           connection is made directly into BIS frequency band. */
    OPAL_FE_INSTALL_INDIVIDUAL_UNICABLE,                  /**< Unicable installation MANUAL settings. */
    OPAL_FE_INSTALL_INDIVIDUAL_UNICABLE_AUTO_DISCOVER,    /**< Unicable installation AUTO No disturb others STB = Enhancements to EN 50494 V1.0. */
    OPAL_FE_INSTALL_INDIVIDUAL_UNICABLE_AUTO_RECOVERY,    /**< Unicable installation AUTO If an UB slot allocate to another STB, disturb it. */
    OPAL_FE_INSTALL_INDIVIDUAL_UNICABLE_AUTO_DISCOVER_DISTURB,     /**< Unicable installation AUTO Disturb method because RF ON/OFF on each UB slots. */
    OPAL_FE_INSTALL_INDIVIDUAL_UNICABLE_UB_GET_LIST_DISTURB,       /**< Unicable installation AUTO Disturb method because RF ON/OFF on each UB slots. */
    OPAL_FE_INSTALL_INDIVIDUAL_UNICABLE_UB_GET_NEXT_USER_BAND_DISTURB,       /**< Unicable installation AUTO Disturb method because RF ON/OFF on each UB slots. Gets the next user band. */
    OPAL_FE_INSTALL_MODE_MDU,            /**< Set this parameter to configure LNB in MDU mode. */
    OPAL_FE_INSTALL_MODE_MDU_DISEQC,     /**< Set this parameter to configure LNB in MDU mode piloted by DiSEqC commands. */
    OPAL_FE_INSTALL_TYPE_LAST,
} opal_fe_install_type_t;
/** LNB power setting.
 */
typedef enum {
    OPAL_FE_LNB_ALIM_STB,               /**< LNB is powered by the STB. */
    OPAL_FE_LNB_ALIM_EXT,               /**< LNB is powered by an external source, thus
                                            the STB won't activate its own LNB. */

    OPAL_FE_LNB_ALIM_STB_HIGH,          /**< LNB is powered by the STB, 14V/19V is used
                                            instead of 13V/18V to compensate for power
                                            loss in cable. */

    OPAL_FE_LNB_ALIM_LAST               /**< Last enumerator value. */
} opal_fe_lnb_alim_t;

typedef enum {
    OPAL_FE_POLARIZATION_V,         ///< Vertical Polarization.
    OPAL_FE_POLARIZATION_H,         ///< Horizontal Polarization.
    OPAL_FE_POLARIZATION_R,         ///< Right Circular Polarization.
    OPAL_FE_POLARIZATION_L,         ///< Left Circular Polarization.
    OPAL_FE_POLARIZATION_LAST       ///< last enumerator value.
} opal_fe_dvb_s_polarization_t;
typedef enum {
    OPAL_FE_SEC_VOLTAGE_13,         ///< SEC voltage 13V.
    OPAL_FE_SEC_VOLTAGE_18,         ///< SEC voltage 18V.
    OPAL_FE_SEC_VOLTAGE_OFF,        ///< SEC voltage OFF.
    OPAL_FE_SEC_VOLTAGE_LAST        ///< Last enumerator value.
} opal_fe_voltage_t;
typedef enum {
    OPAL_FE_SEC_TONE_ON,            ///< Enable SEC tone.
    OPAL_FE_SEC_TONE_OFF,           ///< Disable SEC tone.
    OPAL_FE_SEC_TONE_MODE_LAST      ///< Last enumerator value.
} opal_fe_sec_tone_mode_t;

int32 m_IPL_TB_CmdDefaults
(
   int32 wz_mbfield,
   char  *pb_arg
)
{
   int32 Cr = MCM_OK;
#if defined CONFIG_IPL_FT_IMPLEMENTATION
   INTLDR_RestoreDefaults();
#else
   uint8                     i;
   char                      lnb_type[3];
   char                      lnb_power[3];
   TS_INTLDR_LnbDesc         lnb_desc        = {9750000, 10600000, 11700000 };
   TS_INTLDR_LnbOptBand      lnb_bands[INTLDR_MAX_OPT_BANDS];
   char                      arg[STREAM_INFO_TOTAL_ELEMENT_NB -13][100] = 
   {
                             "1",         /* STREAM_INFO_ELEMENT_DL_REQUEST */
                             "0",         /* STREAM_INFO_ELEMENT_LDR_KO */
                             "2",         /* STREAM_INFO_ELEMENT_INTERFACE */
                             "0",         /* STREAM_INFO_ELEMENT_PROTOCOL */
                             "2",         /* STREAM_INFO_ELEMENT_MODULE_ID    */   
                             "1000",      /* STREAM_INFO_ELEMENT_TIMEOUT      */     
                             "100",       /* STREAM_INFO_ELEMENT_PID          */
#if defined (CONFIG_IPL_FE_SELECT_ENTRY_BY_E2P)
                             "0",         /* STREAM_INFO_ELEMENT_FRONT_END_ENTRY  */
#endif
                             "6",         /* STREAM_INFO_ELEMENT_MODULE_TYPE - deprecated: 6 is always wrap type */    
                             "10800000",  /* STREAM_INFO_ELEMENT_FREQUENCY    */
                             "27500",     /* STREAM_INFO_ELEMENT_SYMBOLRATE   */
                             "6",         /* STREAM_INFO_ELEMENT_MODULATION - INTLDR_MODULATION_AUTO */    
                             "0",         /* STREAM_INFO_ELEMENT_POLARIZATION */    
                             "0",         /* STREAM_INFO_ELEMENT_LNB_TYPE */
                             "0",         /* STREAM_INFO_ELEMENT_LNB_POWER */
                             "0",         /* STREAM_INFO_ELEMENT_LNB_OSC */
                             "0",         /* STREAM_INFO_ELEMENT_22KHZ */    
                             "0",         /* STREAM_INFO_ELEMENT_UBNR */
                             "0",         /* STREAM_INFO_ELEMENT_UBFREQ */
                             "0",         /* STREAM_INFO_ELEMENT_SWITCH_ENTRY */    
                             "0",         /* STREAM_INFO_ELEMENT_DISEQC_VER */    
                             "2",         /* STREAM_INFO_ELEMENT_FE_TYPE */
                             "0",         /* STREAM_INFO_ELEMENT_LNB_BANDS */
                             "0",         /* STREAM_INFO_ELEMENT_FEC_RATE */
   };                   

   /* Check parameter */
   if(NULL == pb_arg)
   {
      IPL_PRT_DEBUG_PRINTF("\nUSAGE: defaults LNB_CONFIG\n"
                           "  LNB_CONFIG can be universal, universal_diseqc, advansat, brasilsat, 3losat\n");
      return MCM_OK;
   }

   memset(lnb_bands, 0, INTLDR_MAX_OPT_BANDS * sizeof(TS_INTLDR_LnbOptBand));
   /* Optimized LNB bands */
   if(!strcmp(pb_arg, "universal") || !strcmp(pb_arg, "0") )
   {
      snprintf(lnb_type,  3, "%d", OPAL_FE_INSTALL_INDIVIDUAL);
      snprintf(lnb_power, 3, "%d", OPAL_FE_LNB_ALIM_STB);
   }
   else if(!strcmp(pb_arg, "universal_diseqc") )
   {
      snprintf(lnb_type,  3, "%d", OPAL_FE_INSTALL_INDIVIDUAL_DISEQC);
      snprintf(lnb_power, 3, "%d", OPAL_FE_LNB_ALIM_STB);
   }
   else if(!strcmp(pb_arg, "advansat") )
   {
      snprintf(lnb_type,  3, "%d", OPAL_FE_INSTALL_MODE_MDU);
      snprintf(lnb_power, 3, "%d", OPAL_FE_LNB_ALIM_STB);

      lnb_bands[0].min_freq_in_kHz = 11010500;
      lnb_bands[0].max_freq_in_kHz = 11067500;
      lnb_bands[0].polarization    = OPAL_FE_POLARIZATION_V;
      lnb_bands[0].LO_freq_in_kHz  =  9225000;
      lnb_bands[0].voltage         = OPAL_FE_SEC_VOLTAGE_18;
      lnb_bands[0].tone_mode       = OPAL_FE_SEC_TONE_OFF;

      lnb_bands[1].min_freq_in_kHz = 10962500;
      lnb_bands[1].max_freq_in_kHz = 11199500;
      lnb_bands[1].polarization    = OPAL_FE_POLARIZATION_H;
      lnb_bands[1].LO_freq_in_kHz  = 10012000;
      lnb_bands[1].voltage         = OPAL_FE_SEC_VOLTAGE_18;
      lnb_bands[1].tone_mode       = OPAL_FE_SEC_TONE_OFF;

      lnb_bands[2].min_freq_in_kHz = 11700000;
      lnb_bands[2].max_freq_in_kHz = 11950000;
      lnb_bands[2].polarization    = OPAL_FE_POLARIZATION_V;
      lnb_bands[2].LO_freq_in_kHz  =  9800000;
      lnb_bands[2].voltage         = OPAL_FE_SEC_VOLTAGE_18;
      lnb_bands[2].tone_mode       = OPAL_FE_SEC_TONE_OFF;

      lnb_bands[3].min_freq_in_kHz = 11704000;
      lnb_bands[3].max_freq_in_kHz = 12188000;
      lnb_bands[3].polarization    = OPAL_FE_POLARIZATION_H;
      lnb_bands[3].LO_freq_in_kHz  = 13432000;
      lnb_bands[3].voltage         = OPAL_FE_SEC_VOLTAGE_18;
      lnb_bands[3].tone_mode       = OPAL_FE_SEC_TONE_OFF;
   }
   else if(!strcmp(pb_arg, "brasilsat") )
   {
      snprintf(lnb_type,  3, "%d", OPAL_FE_INSTALL_MODE_MDU);
      snprintf(lnb_power, 3, "%d", OPAL_FE_LNB_ALIM_STB);

      lnb_bands[0].min_freq_in_kHz = 11010500;
      lnb_bands[0].max_freq_in_kHz = 11067500;
      lnb_bands[0].polarization    = OPAL_FE_POLARIZATION_V;
      lnb_bands[0].LO_freq_in_kHz  = 12860000;
      lnb_bands[0].voltage         = OPAL_FE_SEC_VOLTAGE_18;
      lnb_bands[0].tone_mode       = OPAL_FE_SEC_TONE_OFF;

      lnb_bands[1].min_freq_in_kHz = 11704000;
      lnb_bands[1].max_freq_in_kHz = 11941000;
      lnb_bands[1].polarization    = OPAL_FE_POLARIZATION_V;
      lnb_bands[1].LO_freq_in_kHz  = 13435000;
      lnb_bands[1].voltage         = OPAL_FE_SEC_VOLTAGE_18;
      lnb_bands[1].tone_mode       = OPAL_FE_SEC_TONE_OFF;

      lnb_bands[2].min_freq_in_kHz = 10962500;
      lnb_bands[2].max_freq_in_kHz = 11199500;
      lnb_bands[2].polarization    = OPAL_FE_POLARIZATION_H;
      lnb_bands[2].LO_freq_in_kHz  = 13112000;
      lnb_bands[2].voltage         = OPAL_FE_SEC_VOLTAGE_18;
      lnb_bands[2].tone_mode       = OPAL_FE_SEC_TONE_OFF;

      lnb_bands[3].min_freq_in_kHz = 11704000;
      lnb_bands[3].max_freq_in_kHz = 12188000;;
      lnb_bands[3].polarization    = OPAL_FE_POLARIZATION_H;
      lnb_bands[3].LO_freq_in_kHz  = 13138000;
      lnb_bands[3].voltage         = OPAL_FE_SEC_VOLTAGE_18;
      lnb_bands[3].tone_mode       = OPAL_FE_SEC_TONE_OFF;
   }
   else if(!strcmp(pb_arg, "3losat") )
   {
      snprintf(lnb_type, 3, "%d", OPAL_FE_INSTALL_MODE_MDU);
      snprintf(lnb_power, 3, "%d", OPAL_FE_LNB_ALIM_STB);
 
      lnb_bands[0].min_freq_in_kHz = 11010500;
      lnb_bands[0].max_freq_in_kHz = 11067500;
      lnb_bands[0].polarization = OPAL_FE_POLARIZATION_V;
      lnb_bands[0].LO_freq_in_kHz = 12860000;
      lnb_bands[0].voltage = OPAL_FE_SEC_VOLTAGE_18;
      lnb_bands[0].tone_mode = OPAL_FE_SEC_TONE_OFF;
 
      lnb_bands[1].min_freq_in_kHz = 11704000;
      lnb_bands[1].max_freq_in_kHz = 11941000;
      lnb_bands[1].polarization    = OPAL_FE_POLARIZATION_V;
      lnb_bands[1].LO_freq_in_kHz  = 13435000;
      lnb_bands[1].voltage         = OPAL_FE_SEC_VOLTAGE_18;
      lnb_bands[1].tone_mode       = OPAL_FE_SEC_TONE_OFF;
 
      lnb_bands[2].min_freq_in_kHz = 10962500;
      lnb_bands[2].max_freq_in_kHz = 11199500;
      lnb_bands[2].polarization = OPAL_FE_POLARIZATION_H;
      lnb_bands[2].LO_freq_in_kHz = 13112000;
      lnb_bands[2].voltage = OPAL_FE_SEC_VOLTAGE_18;
      lnb_bands[2].tone_mode = OPAL_FE_SEC_TONE_OFF; 
   }
   else
   {
      IPL_PRT_DEBUG_PRINTF("\nUSAGE: defaults LNB_CONFIG\n"
                           "  LNB_CONFIG can be universal, universal_diseqc, advansat, brasilsat\n");
      return MCM_OK;
   }

   /* Execute toolbox commands to set common fields */
   for( i=0 ; (MCM_OK == Cr) && (i< STREAM_INFO_TOTAL_ELEMENT_NB -19) ; i++ )
      Cr = m_IPL_TB_CmdMbField(i, arg[i]);

   /* Fill INTLDR context LNB-dependent fields */
   INTLDR_SetField(STREAM_INFO_ELEMENT_LNB_TYPE,  (const uint8*)&lnb_type);
   INTLDR_SetField(STREAM_INFO_ELEMENT_LNB_POWER, (const uint8*)&lnb_power);
   INTLDR_SetField(STREAM_INFO_ELEMENT_LNB_OSC,   (const uint8*)&lnb_desc);
   INTLDR_SetField(STREAM_INFO_ELEMENT_LNB_BANDS, (const uint8*)&lnb_bands);
#endif

   return Cr;

} /* End of function 'm_IPL_TB_CmdDefaults()' */

