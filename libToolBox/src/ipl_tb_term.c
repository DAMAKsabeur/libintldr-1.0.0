
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
X File          : ipl_tb_term.c
X Author        : Anthony CAMMAS
X Creation date : 08/03/2009
X Product       : ST                                                        
X Language      : C                               
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
*@@@eF
*/

/* ########################################################################## */
/* ### #include                                                               */
#include "ipl_tb_internal.h"
#include "ipl_tb.h"
#include "ipl_tb_l.h"

/* ########################################################################## */
/* ### Variables                                                              */

/* ########################################################################## */
/* ### Functions                                                              */

/*
*@@@bf
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
X                     Copyright (c) 2000 2009 SAGEM SA.
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
X Function      : IPL_TB_Term()
X Author        : Anthony CAMMAS
X Creation date : 08/03/2009
XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
*@@@ef
*/
int32 IPL_TB_Term
(
   TE_IPL_TB_mode e_ipl_tb_mode
)
{
   switch (e_ipl_tb_mode)
   {
      case IPL_TB_MODE_EMBEDDED:
         break;

      case IPL_TB_MODE_STAND_ALONE:
         break;

      default:
         return -1;
   }
   
   return MCM_OK;

} /* End of function 'IPL_TB_Term()' */

