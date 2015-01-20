
/* funct1.c - interpret various LaTeX commands and produce RTF

Copyright (C) 1995-2002 The Free Software Foundation

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.

This file is available from http://sourceforge.net/projects/latex2rtf/
 
Authors:
    1995      Fernando Dorner, Andreas Granzer, Freidrich Polzer, Gerhard Trisko
    1995-1997 Ralf Schlatterbeck
    1998-2000 Georg Lehner
    2001-2002 Scott Prahl
*/

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "convert.h"
#include "funct1.h"
#include "commands.h"
#include "stack.h"



#include "utils.h"

#include "parser.h"

#include "equations.h"


#define ARABIC_NUMBERING 0
#define ALPHA_NUMBERING  1
#define ROMAN_NUMBERING  2

char *roman_item(int n, int upper);

//static int g_chapter_numbering = ARABIC_NUMBERING;
//static int g_section_numbering = ARABIC_NUMBERING;
//static int g_appendix = 0;

int g_processing_list_environment = FALSE;

void CmdBeginEnd(int code)

/***************************************************************************
 purpose: reads the parameter after the \begin or \end-command; ( see also getBraceParam )
          after reading the parameter the CallParamFunc-function calls the
          handling-routine for that special environment
 parameter: code: CMD_BEGIN: start of environment
                  CMD_END:   end of environment
 ***************************************************************************/
{
    //int i;
    //char *str, *option;
    char *s = getBraceParam();



/* hack to avoid problems with multicols */
    if (strcmp(s, "multicols") == 0) {
        free(s);
        return;
    }





/* usual environments */
    if (code == CMD_BEGIN) {
        diagnostics(5, "in CmdBeginEnd handle \\begin{%s}", s);
        (void) CallParamFunc(s, ON);
    } else {
        diagnostics(5, "in CmdBeginEnd handle \\end{%s}", s);
        (void) CallParamFunc(s, OFF);

    }
    free(s);
}

void CmdIgnore(int code)

/******************************************************************************
 purpose: allows handling of constructs that do not require changes to RTF
 ******************************************************************************/
{
}

void Environment(int code)

/******************************************************************************
  purpose: pushes/pops the new environment-commands on/from the stack
parameter: code includes the type of the environment
 ******************************************************************************/
{
    if (code & ON) {
        code &= ~(ON);          /* mask MSB */
        diagnostics(4, "Entering Environment (%d)", code);
        PushEnvironment(GENERIC_MODE);
    } else {                    /* off switch */
       // CmdEndParagraph(0);
        diagnostics(4, "Exiting  Environment (%d)", code);
        PopEnvironment();
    }
}


void CmdIgnoreLet(int code)

/******************************************************************************
     purpose : ignore \let 
       Format: \let\XXXXX = \YYYYYY or \let\XXXXX\YYYYYY
 ******************************************************************************/
{
    char t[100];
    char cThis;
    int i;
    char *s;

    s = getSimpleCommand();
    
    cThis = getNonBlank();
    if (cThis == '=') 
        cThis = getNonBlank();
    
    t[0] = cThis;
    for (i=1; i<100; i++) {
        t[i] = getTexChar();
        if (t[i]==' ' || t[i]=='\n') break;
    }
    t[i]='\0';
    
   // newDefinition(s+1,NULL,t,0);
    free(s);    
}



/******************************************************************************
  purpose: parse \rlap{text} or \llap{text} , but ignore spacing changes
******************************************************************************/
void CmdLap(int code)
{
    char *s = getBraceParam();
    ConvertString(s);
    free(s);
}
