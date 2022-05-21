
#ifndef IPL_TB_H
#define IPL_TB_H

/*-----------------------------------------------------------------------------
 FILE DOCUMENTATION
-----------------------------------------------------------------------------*/
/**
 * @note    Copyright 2000-2012 SagemCom SAS All rights reserved
 *
 * @file    ipl_tb.h
 * @brief   IPL_TB implementation
 * @version Revision of last commit : 14
 * @date    Date of last commit     : 2012-02-06 16:15:34 +0100 (Mon, 06 Feb 2012)
 * @author  Author of last commit   : Jean-Louis BONNAFFE 
 * @addtogroup IPL_TB
 * @{
 */

/*-----------------------------------------------------------------------------
 SYSTEM                          INCLUSIONS in functional/alphabetical order
-----------------------------------------------------------------------------*/
#include "intldr.h"


/*-----------------------------------------------------------------------------
 FUNCTIONAL                      INCLUSIONS in functional/alphabetical order
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 CURRENT COMPONENT               INCLUSIONS in functional/alphabetical order
-----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
 LOCAL CONSTANTS                 DEFINITIONS in functional/alphabetical order
-----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
 LOCAL MACROS                    DEFINITIONS in functional/alphabetical order
-----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
 LOCAL ENUMERATIONS              DEFINITIONS in functional/alphabetical order
-----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
 LOCAL TYPES                     DEFINITIONS in functional/alphabetical order
-----------------------------------------------------------------------------*/
/** 
 * @brief This variable selects the build mode : Embedded (linked to INTLDR)
 *  or Stand-Alone (not using INTLDR) 
 * @{
 */
typedef enum
{
   IPL_TB_MODE_EMBEDDED       = 0x0,
   IPL_TB_MODE_STAND_ALONE
} TE_IPL_TB_mode;
/** @} */

/*-----------------------------------------------------------------------------
 STATIC CONST VARIABLES          DECLARATIONS in functional/alphabetical order
-----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
 CONST VARIABLES                 DECLARATIONS in functional/alphabetical order
-----------------------------------------------------------------------------*/
extern const char *pcg_IPL_TB_prompt;
extern const char *pcg_IPL_TB_display;

/*-----------------------------------------------------------------------------
 STATIC VARIABLES                DECLARATIONS in functional/alphabetical order
-----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
 INTERNAL VARIABLES              DECLARATIONS in functional/alphabetical order
-----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
 STATIC FUNCTIONS                DEFINITIONS in functional/alphabetical order
-----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
 INTERNAL FUNCTIONS              DEFINITIONS in functional/alphabetical order
-----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
 PROTECTED FUNCTIONS              DEFINITIONS in functional/alphabetical order
-----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
 PUBLIC FUNCTIONS                DEFINITIONS in functional/alphabetical order
-----------------------------------------------------------------------------*/
/** 
 * @brief     Starts the toolbox.
 */
extern int32 IPL_TB_Main(void);

/**
 * @brief   Prints debug traces on the serial line.
 * @param   i_pFormat   printf-like string format
 */
void IPL_PRT_DEBUG_Printf(char *i_pFormat, ...);

#endif /* IPL_TB_H */

/** @} */

