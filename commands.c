/* commands.c - Defines subroutines to translate LaTeX commands to RTF

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
    1995-1997 Ralf Schlatterbeck
    1998-2000 Georg Lehner
    2001-2002 Scott Prahl
*/

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "convert.h"
#include "funct1.h"
#include "equations.h"
#include "commands.h"
//#include "chars.h"
#include "utils.h"
//#include "parser.h"
#include "vertical.h"


static int iEnvCount = 0;               /* number of current environments */
static CommandArray *Environments[100]; /* call chain for current environments */
// static int g_par_indent_array[100];
// static int g_left_indent_array[100];
// static int g_right_indent_array[100];
// static char g_align_array[100];

static CommandArray commands[] = {
    {"begin", CmdBeginEnd, CMD_BEGIN},
    {"end", CmdBeginEnd, CMD_END},
    // {"today", CmdToday, 0},
    // {"footnote", CmdFootNote, FOOTNOTE},
    // {"endnote", CmdFootNote, ENDNOTE},

    // {"rmfamily", CmdFontFamily, F_FAMILY_ROMAN  },
    // {"rm", CmdFontFamily, F_FAMILY_ROMAN_1},
    // {"mathrm", CmdFontFamily, F_FAMILY_ROMAN_2},
    // {"textrm", CmdFontFamily, F_FAMILY_ROMAN_2},

    // {"sffamily", CmdFontFamily, F_FAMILY_SANSSERIF},
    // {"sf", CmdFontFamily, F_FAMILY_SANSSERIF_1},
    // {"mathsf", CmdFontFamily, F_FAMILY_SANSSERIF_2},
    // {"textsf", CmdFontFamily, F_FAMILY_SANSSERIF_2},

    // {"ttfamily", CmdFontFamily, F_FAMILY_TYPEWRITER},
    // {"tt", CmdFontFamily, F_FAMILY_TYPEWRITER_1},
    // {"mathtt", CmdFontFamily, F_FAMILY_TYPEWRITER_2},
    // {"texttt", CmdFontFamily, F_FAMILY_TYPEWRITER_2},

    // {"cal", CmdFontFamily, F_FAMILY_CALLIGRAPHIC_1},
    // {"mathcal", CmdFontFamily, F_FAMILY_CALLIGRAPHIC_2},

    // {"bfseries", CmdFontSeries, F_SERIES_BOLD},
    // {"bf", CmdFontSeries, F_SERIES_BOLD_1},
    // {"textbf", CmdFontSeries, F_SERIES_BOLD_2},
    // {"mathbf", CmdFontSeries, F_SERIES_BOLD_2},

    // {"mdseries", CmdFontSeries, F_SERIES_MEDIUM},
    // {"textmd", CmdFontSeries, F_SERIES_MEDIUM_2},
    // {"mathmd", CmdFontSeries, F_SERIES_MEDIUM_2},

    // {"itshape", CmdFontShape, F_SHAPE_ITALIC},
    // {"it", CmdFontShape, F_SHAPE_ITALIC_1},
    // {"mit", CmdFontShape, F_SHAPE_ITALIC_1},
    // {"textit", CmdFontShape, F_SHAPE_ITALIC_2},
    // {"mathit", CmdFontShape, F_SHAPE_ITALIC_2},

    // {"upshape", CmdFontShape, F_SHAPE_UPRIGHT},
    // {"textup", CmdFontShape, F_SHAPE_UPRIGHT_2},
    // {"mathup", CmdFontShape, F_SHAPE_UPRIGHT_2},

    // {"scfamily", CmdFontShape, F_SHAPE_CAPS},
    // {"scshape", CmdFontShape, F_SHAPE_CAPS},
    // {"sc", CmdFontShape, F_SHAPE_CAPS_1},
    // {"textsc", CmdFontShape, F_SHAPE_CAPS_2},
    // {"mathsc", CmdFontShape, F_SHAPE_CAPS_2},

    // {"slshape", CmdFontShape, F_SHAPE_SLANTED},
    // {"sl", CmdFontShape, F_SHAPE_SLANTED_1},
    // {"textsl", CmdFontShape, F_SHAPE_SLANTED_2},
    // {"mathsl", CmdFontShape, F_SHAPE_SLANTED_2},

    // {"tiny", CmdFontSize, 10},
    // {"scriptsize", CmdFontSize, 14},
    // {"footnotesize", CmdFontSize, 16},
    // {"enotesize", CmdFontSize, 16},
    // {"small", CmdFontSize, 18},
    // {"normalsize", CmdFontSize, 20},
    // {"large", CmdFontSize, 24},
    // {"Large", CmdFontSize, 28},
    // {"LARGE", CmdFontSize, 34},
    // {"huge", CmdFontSize, 40},
    // {"Huge", CmdFontSize, 50},
    // {"HUGE", CmdFontSize, 60},  /* from moresize.sty */

    /* ---------- OTHER FONT STUFF ------------------- */
    // {"em", CmdEmphasize, F_EMPHASIZE_1},
    // {"emph", CmdEmphasize, F_EMPHASIZE_2},
    // {"underline", CmdUnderline, 0},
    // {"underbar", CmdUnderline, 0},
    // {"textnormal", CmdTextNormal, F_TEXT_NORMAL_2},
    // {"normalfont", CmdTextNormal, F_TEXT_NORMAL_2},
    // {"mathnormal", CmdTextNormal, F_TEXT_NORMAL_3},
    // {"textfont", CmdTextFont, 0},
    // {"the", CmdThe, 0},

    // {"raggedright", CmdAlign, PAR_RAGGEDRIGHT},
    // {"centerline", CmdAlign, PAR_CENTERLINE},
    // {"vcenter", CmdAlign, PAR_VCENTER},

    /* ---------- LOGOS ------------------- */
    // {"latex", CmdLogo, CMD_LATEX},
    // {"LaTeX", CmdLogo, CMD_LATEX},
    // {"LaTeXe", CmdLogo, CMD_LATEXE},
    // {"TeX", CmdLogo, CMD_TEX},
    // {"SLiTeX", CmdLogo, CMD_SLITEX},
    // {"BibTeX", CmdLogo, CMD_BIBTEX},
    // {"AmSTeX", CmdLogo, CMD_AMSTEX},
    // {"AmSLaTeX", CmdLogo, CMD_AMSLATEX},
    // {"LyX", CmdLogo, CMD_LYX},
    // {"lower",CmdSubscript,2},
    // {"kern",CmdKern,0},

    /* ---------- SPECIAL CHARACTERS ------------------- */
    // {"hat", CmdHatChar, 0},
    // {"check", CmdCaronChar, 0},
    // {"breve", CmdBreveChar, 0},
    // {"acute", CmdAcuteChar, 0},
    // {"grave", CmdGraveChar, 0},
    // {"tilde", CmdTildeChar, 0},
    // {"bar", CmdMacronChar, 0},
    // {"vec", CmdVecChar, 0},
    // {"overrightarrow", CmdVecChar, 0},
    // {"dot", CmdDotChar, 0},
    // {"ddot", CmdUmlauteChar, 0},
    // {"\"", CmdUmlauteChar, 0},
    // {"u", CmdBreveChar, 0},
    // {"d", CmdUnderdotChar, 0},
    // {"v", CmdCaronChar, 0},
    // {"r", CmdRingChar, 0},
    // {"accent", CmdRingChar, 1},
    // {"b", CmdUnderbarChar, 0},
    // {"c", CmdCedillaChar, 0},
    // {"i", CmdDotlessChar, 0},
    // {"j", CmdDotlessChar, 1},
    // {"H", CmdDoubleAcuteChar, 0},
    // {"l", CmdUnicodeChar, 323},
    // {"L", CmdUnicodeChar, 322},
    
/* sectioning commands */
    // {"part", CmdSection, SECT_PART},
    // {"part*", CmdSection, SECT_PART_STAR},
    // {"chapter", CmdSection, SECT_CHAPTER},
    // {"chapter*", CmdSection, SECT_CHAPTER_STAR},
    // {"section", CmdSection, SECT_NORM},
    // {"section*", CmdSection, SECT_NORM_STAR},
    // {"subsection", CmdSection, SECT_SUB},
    // {"subsection*", CmdSection, SECT_SUB_STAR},
    // {"subsubsection", CmdSection, SECT_SUBSUB},
    // {"subsubsection*", CmdSection, SECT_SUBSUB_STAR},
    // {"paragraph", CmdSection, SECT_SUBSUBSUB},
    // {"paragraph*", CmdSection, SECT_SUBSUBSUB_STAR},
    // {"subparagraph", CmdSection, SECT_SUBSUBSUBSUB},
    // {"subparagraph*", CmdSection, SECT_SUBSUBSUBSUB_STAR},

    // {"ldots", CmdUnicodeChar, 8230},
    // {"dots", CmdUnicodeChar, 8230},
    // {"dotfill", CmdUnicodeChar, 8230},
    // {"textellipsis", CmdUnicodeChar, 8230},

    // {"maketitle", CmdMakeTitle, 0},
    // {"par", CmdEndParagraph, 0},
    // {"noindent", CmdIndent, INDENT_NONE},
    // {"indent", CmdIndent, INDENT_USUAL},
    // {"caption", CmdCaption, 0},
    // {"appendix", CmdAppendix, 0},
    // {"protect", CmdIgnore, 0},
    // {"clearpage", CmdNewPage, NewPage},
    // {"efloatseparator", CmdNewPage, NewPage},
    // {"cleardoublepage", CmdNewPage, NewPage},
    // {"newpage", CmdNewPage, NewColumn},
    // {"pagebreak", CmdNewPage, NewPage},
    // {"mbox", CmdBox, BOX_MBOX},
    // {"hbox", CmdBox, BOX_HBOX},
    // {"vbox", CmdBox, BOX_VBOX},
    // {"fbox", CmdBox, BOX_FBOX},
    // {"parbox", CmdBox, BOX_PARBOX},
    // {"makebox", CmdBox, BOX_MAKEBOX},
    // {"framebox", CmdBox, BOX_FRAMEBOX},
    // {"frenchspacing", CmdIgnore, 0},
    // {"nonfrenchspacing", CmdIgnore, 0},
    // {"include", CmdIgnoreParameter, No_Opt_One_NormParam},  /* should not happen */
    // {"input", CmdIgnoreParameter, No_Opt_One_NormParam},    /* should not happen */
    // {"verb", CmdVerb, VERB_VERB},
    // {"verb*", CmdVerb, VERB_STAR},
    // {"onecolumn", CmdColumn, One_Column},
    // {"twocolumn", CmdColumn, Two_Column},
    // {"includegraphics", CmdGraphics, FIGURE_INCLUDEGRAPHICS},
    // {"epsffile", CmdGraphics, FIGURE_EPSFFILE},
    // {"epsfbox", CmdGraphics, FIGURE_EPSFBOX},
    // {"BoxedEPSF", CmdGraphics, FIGURE_BOXEDEPSF},
    // {"psfig", CmdGraphics, FIGURE_PSFIG},
    // {"includegraphics*", CmdGraphics, FIGURE_INCLUDEGRAPHICS},
    // {"moveleft", CmdLength, 0},
    // {"moveright", CmdLength, 0},
    // {"hsize", CmdLength, 0},
    // {"letterspace", CmdLength, 0},
    // {"footnotemark", CmdIgnoreParameter, One_Opt_No_NormParam},
    // {"endnotemark", CmdIgnoreParameter, One_Opt_No_NormParam},
    // {"label", CmdLabel, LABEL_LABEL},
    // {"ref", CmdLabel, LABEL_REF},
    // {"vref", CmdLabel, LABEL_VREF},
    // {"eqref", CmdLabel, LABEL_EQREF},
    // {"pageref", CmdLabel, LABEL_PAGEREF},
    // {"nameref", CmdLabel, LABEL_NAMEREF},
    // {"cite", CmdCite, CITE_CITE},
    // {"onlinecite", CmdCite, CITE_CITE},
    // {"citeonline", CmdCite, CITE_CITE},
    // {"nobibliography", CmdIgnoreParameter, No_Opt_One_NormParam},  
    // {"bibliography", CmdBibliography, 0},
    // {"bibitem", CmdBibitem, 0},
    // {"bibentry", CmdBibEntry, 0},
    // {"newblock", CmdNewblock, 0},
    // {"newsavebox", CmdIgnoreParameter, No_Opt_One_NormParam},
    // {"usebox", CmdIgnoreParameter, No_Opt_One_NormParam},
    // {"subfigure", CmdSubFigure, 0},

    // {"quad", CmdQuad, 1},
    // {"qquad", CmdQuad, 2},
    {"textsuperscript", CmdSuperscript, 1},
    {"textsubscript", CmdSubscript, 1},
    // {"hspace", CmdIgnoreParameter, No_Opt_One_NormParam},
    // {"hspace*", CmdIgnoreParameter, No_Opt_One_NormParam},
    // {"vspace", CmdVspace, VSPACE_VSPACE},
    // {"vspace*", CmdVspace, VSPACE_VSPACE},
    // {"vskip", CmdVspace, VSPACE_VSKIP},
    // {"smallskip", CmdVspace, VSPACE_SMALL_SKIP},
    // {"medskip", CmdVspace, VSPACE_MEDIUM_SKIP},
    // {"bigskip", CmdVspace, VSPACE_BIG_SKIP},
    // {"addvspace", CmdIgnoreParameter, No_Opt_One_NormParam},
    // {"addcontentsline", CmdIgnoreParameter, No_Opt_Three_NormParam},
    // {"addcontents", CmdIgnoreParameter, No_Opt_Two_NormParam},
    // {"stretch", CmdIgnoreParameter, No_Opt_One_NormParam},
    // {"typeaout", CmdIgnoreParameter, No_Opt_One_NormParam},
    // {"index", CmdIndex, 0},
    // {"printindex", CmdPrintIndex, 0},
    // {"indexentry", CmdIgnoreParameter, No_Opt_Two_NormParam},
    // {"glossary", CmdIgnoreParameter, No_Opt_One_NormParam},
    // {"glossaryentry", CmdIgnoreParameter, No_Opt_Two_NormParam},
    // {"typeout", CmdIgnoreParameter, No_Opt_One_NormParam},
    // {"Typein", CmdIgnoreParameter, One_Opt_One_NormParam},
    // {"includeonly", CmdIgnoreParameter, No_Opt_One_NormParam},
    // {"nocite", CmdNoCite, No_Opt_One_NormParam},
    // {"stepcounter", CmdIgnoreParameter, No_Opt_One_NormParam},
    // {"refstepcounter", CmdIgnoreParameter, No_Opt_One_NormParam},
    // {"fnsymbol", CmdIgnoreParameter, No_Opt_One_NormParam},
    // {"Alph", CmdIgnoreParameter, No_Opt_One_NormParam},
    // {"alph", CmdIgnoreParameter, No_Opt_One_NormParam},
    // {"Roman", CmdIgnoreParameter, No_Opt_One_NormParam},
    // {"roman", CmdIgnoreParameter, No_Opt_One_NormParam},
    // {"arabic", CmdIgnoreParameter, No_Opt_One_NormParam},
    // {"newcount", CmdIgnoreDef, 0},
    // {"output", CmdIgnoreDef, 0},
    // {"value", CmdCounter, COUNTER_VALUE},
    // {"sbox", CmdIgnoreParameter, No_Opt_Two_NormParam},
    // {"savebox", CmdIgnoreParameter, Two_Opt_Two_NormParam},
    // {"rule", CmdRule,0},
    // {"raisebox", CmdIgnoreParameter, Two_Opt_Two_NormParam},
    // {"newfont", CmdIgnoreParameter, No_Opt_Two_NormParam},
    // {"settowidth", CmdIgnoreParameter, No_Opt_Two_NormParam},
    // {"nopagebreak", CmdIgnoreParameter, One_Opt_No_NormParam},
    // {"samepage", CmdIgnore, 0},
    // {"expandafter", CmdIgnore, 0},
    // {"long", CmdIgnore, 0},
    // {"nobreak", CmdIgnore, 0},
    // {"linebreak", CmdIgnoreParameter, One_Opt_No_NormParam},
    // {"nolinebreak", CmdIgnoreParameter, One_Opt_No_NormParam},
    // {"typein", CmdIgnoreParameter, One_Opt_One_NormParam},
    // {"marginpar", CmdIgnoreParameter, One_Opt_One_NormParam},
    // {"baselineskip", Cmd_OptParam_Without_braces, 0},
    // {"psfrag", CmdIgnoreParameter, No_Opt_Two_NormParam},
    // {"lineskip", Cmd_OptParam_Without_braces, 0},
    // {"vsize", Cmd_OptParam_Without_braces, 0},
    // {"setbox", Cmd_OptParam_Without_braces, 0},
    // {"thanks", CmdFootNote, FOOTNOTE_THANKS},
    // {"bibliographystyle", CmdIgnoreParameter, No_Opt_One_NormParam},
    {"let", CmdIgnoreLet, 0},
    // {"multicolumn", CmdMultiCol, 0},
    {"ensuremath", CmdEnsuremath, 0},
    {"frac", CmdFraction, 0},
    {"dfrac", CmdFraction, 0},
    {"Frac", CmdFraction, 0},
    {"sqrt", CmdRoot, 0},
    {"lim", CmdLim, 0},
    {"limsup", CmdLim, 1},
    {"liminf", CmdLim, 2},
    {"int", CmdIntegral, 0},
    {"iint", CmdIntegral, 3},
    {"iiint", CmdIntegral, 4},
    {"sum", CmdIntegral, 1},
    {"prod", CmdIntegral, 2},
    {"left", CmdLeftRight, 0},
    {"right", CmdLeftRight, 1},
    {"stackrel", CmdStackrel, 0},
    {"matrix", CmdMatrix, 0},
    {"overline",CmdOverLine,0},
    {"rightleftharpoons", CmdArrows, RIGHT_LEFT_HARPOONS},
    // {"psset", CmdPsset, 0},
    // {"newpsstyle", CmdNewPsStyle, 0},

    {"nonumber", CmdNonumber, EQN_NO_NUMBER},
    {"notag", CmdNonumber, EQN_NO_NUMBER},
    // {"char", CmdSymbol, 0},
    // {"symbol", CmdSymbol, 1},
    {"rlap", CmdLap, 0},
    {"llap", CmdLap, 1},
    // {"not", CmdNot, 0},

    // {"url",               CmdHtml, LABEL_URL},
    // {"urlstyle",          CmdHtml, LABEL_URLSTYLE},
    // {"htmladdnormallink", CmdHtml, LABEL_HTMLADDNORMALREF},
    // {"htmlref",           CmdHtml, LABEL_HTMLREF},
    
    // {"nobreakspace", CmdNonBreakSpace, 100},
    // {"thinspace", CmdNonBreakSpace, 50},
    // {"abstract", CmdAbstract, ABSTRACT_SIMPLE},
    // {"keywords", CmdKeywords, 0},
    // {"endinput", CmdEndInput, 0},
    // {"color", CmdTextColor, 0},
    // {"textcolor", CmdTextColor, 1},
    // {"tableofcontents", CmdTableOfContents, 0},
    // {"listoffigures", CmdListOf, LIST_OF_FIGURES},
    // {"listoftables", CmdListOf, LIST_OF_TABLES},
    // {"numberline", CmdNumberLine, 0},
    // {"contentsline", CmdContentsLine, 0},
    // {"centering", CmdAlign, PAR_CENTERING},
    
    // {"halign", CmdHAlign, 0},
        
    // {"efloatseparator", CmdIgnoreParameter,0},
    // {"pagestyle", CmdIgnoreParameter, No_Opt_One_NormParam},
    // {"pagenumbering", CmdIgnoreParameter, No_Opt_One_NormParam},
    // {"markboth", CmdIgnoreParameter, No_Opt_Two_NormParam},
    // {"markright", CmdIgnoreParameter, No_Opt_One_NormParam},

    /************ commands for auxfile.c *******************/
    // { "newlabel",      CmdNewLabel,       0 },
    // { "newacro",       CmdAcrodef,        ACRONYM_NEWACRO },
    // { "newacroplural", CmdAcrodef,        ACRONYM_NEWACROPLURAL },
    // { "harvardcite",   CmdAuxHarvardCite, 0 },
    // { "bibcite",       CmdBibCite,        0 },
    
    // {"usetikzlibrary",CmdTikzlib, 0},
    
    {"", NULL, 0}
};

/********************************************************************/

/* commands for begin-end environments */

/* only strings used in the form \begin{text} or \end{text} */

/********************************************************************/
static CommandArray params[] = {
    // {"center", CmdAlign, PAR_CENTER},
    // {"flushright", CmdAlign, PAR_RIGHT},
    // {"flushleft", CmdAlign, PAR_LEFT},
    {"document", Environment, DOCUMENT_MODE},

    // {"quote", CmdQuote, QUOTE_MODE},
    // {"quotation", CmdQuote, QUOTATION_MODE},
    // {"enumerate", CmdList, ENUMERATE_MODE},
    // {"list", CmdList, ITEMIZE_MODE},
    // {"itemize", CmdList, ITEMIZE_MODE},
    // {"compactitem", CmdList, ITEMIZE_MODE},
    // {"description", CmdList, DESCRIPTION_MODE},
    
    // {"enumerate*", CmdList, ENUMERATE_MODE},
    // {"itemize*", CmdList, ITEMIZE_MODE},
    // {"description*", CmdList, DESCRIPTION_MODE},
    // {"basedescipt", CmdList, DESCRIPTION_MODE},
    
    // {"acronym", CmdBeginAcronym, 0},
    
    // {"asparaenum", CmdList, ENUMERATE_MODE},
    // {"inparaenum", CmdList, INPARAENUM_MODE},
    // {"compactenum", CmdList, ENUMERATE_MODE},
    // {"compactdesc", CmdList, DESCRIPTION_MODE},
    // {"compactitem", CmdList, ITEMIZE_MODE},

    // {"verbatim", CmdVerbatim, VERBATIM_1},
    // {"comment", CmdVerbatim, VERBATIM_4},
    // {"verse", CmdVerse, 0},
    // {"tabular", CmdTabular, TABULAR},
    // {"tabular*", CmdTabular, TABULAR_STAR},
    // {"longtable", CmdTabular, TABULAR_LONG},
    // {"longtable*", CmdTabular, TABULAR_LONG_STAR},
    {"array", CmdArray, 1},

    {"displaymath", CmdEquation, EQN_DISPLAYMATH},
    {"equation", CmdEquation, EQN_EQUATION},
    {"equation*", CmdEquation, EQN_EQUATION_STAR},
    {"eqnarray*", CmdEquation, EQN_ARRAY_STAR},
    {"eqnarray", CmdEquation, EQN_ARRAY},
    {"align*", CmdEquation, EQN_ALIGN_STAR},
    {"align", CmdEquation, EQN_ALIGN},
    {"math", CmdEquation, EQN_MATH},
    // {"tikzpicture",CmdTikzPicture, 0},
	
	{"", NULL, 0}
};                              /* end of list */


/********************************************************************
purpose: commands for ignored commands (placeholder) 
********************************************************************/
// static CommandArray ignoreCommands[] = {
//     {"", NULL, 0}
// };

int CurrentEnvironmentCount(void)

/****************************************************************************
purpose: to eliminate the iEnvCount global variable 
****************************************************************************/
{
    return iEnvCount;
}

/****************************************************************************
purpose: returns a name for the current environment
 ****************************************************************************/
static char *EnvironmentName(CommandArray *code)
{
    // if (code == PreambleCommands)
    //     return strdup("preamble");
    if (code == commands)
        return strdup("document");
    // if (code == ItemizeCommands)
    //     return strdup("itemize");
    // if (code == EnumerateCommands)
    //     return strdup("enumerate");
    // if (code == DescriptionCommands)
    //     return strdup("description");
    // if (code == InparaenumCommands)
    //     return strdup("inparaenum");
    // if (code == LetterCommands)
    //     return strdup("letter");
    // if (code == GermanModeCommands)
    //     return strdup("german");
    // if (code == FrenchModeCommands)
    //     return strdup("french");
    // if (code == RussianModeCommands)
    //     return strdup("russian");
    // if (code == CzechModeCommands)
    //     return strdup("czech");
    // if (code == FigureCommands)
    //     return strdup("figure or wrapfigure");
    // if (code == ignoreCommands)
    //     return strdup("unknown environment");
    // if (code == hyperlatexCommands)
    //     return strdup("hyperlatex");
    // if (code == apaciteCommands)
    //     return strdup("apacite");
    // if (code == natbibCommands)
    //     return strdup("natbib");
    // if (code == harvardCommands)
    //     return strdup("harvard");
    // if (code == authordateCommands)
    //     return strdup("authordate");
    // if (code == verbatimCommands)
    //     return strdup("verbatim");
    // if (code == quoteCommands)
    //     return strdup("quote");
    // if (code == quotationCommands)
    //     return strdup("quotation");
    // if (code == bibliographyCommands)
    //     return strdup("bibliography");
    // if (code == verseCommands)
    //     return strdup("verse");
    // if (code == genericCommands)
    //     return strdup("generic");
    // if (code == acronymCommands)
    //     return strdup("acronym");
    // if (code == spacingCommands)
    //     return strdup("setpace");
    return strdup("unknown");
}

/****************************************************************************
purpose: prints the names of all the current environments
 ****************************************************************************/
#if 0
static void WriteEnvironmentStack(void)
{
    int i;
    char *s;
        
    for (i=0; i<iEnvCount; i++) {
        s=EnvironmentName(Environments[i]);
        diagnostics(6, "Environments[%2d]=\"%12s\"", i, s);
        free(s);
    }
}
#endif

int CallCommandFunc(char *cCommand)

/****************************************************************************
purpose: Tries to call the command-function for the commandname
params:  string with command name
returns: success or failure
globals: command-functions have side effects or recursive calls
 ****************************************************************************/
{
    int iCommand, iEnv; //,user_def_index;
    //char *macro_string;

    diagnostics(4, "CallCommandFunc seeking <%s> (%d environments to look through)", cCommand, iEnvCount);

    // user_def_index = existsDefinition(cCommand);
    // if (user_def_index > -1) {
    //     macro_string = expandDefinition(user_def_index);
    //     diagnostics(5, "CallCommandFunc <%s> expanded to <%s>", cCommand, macro_string);
    //     ConvertString(macro_string);
    //     free(macro_string);
    //     return TRUE;
    // }

    /* search backwards through chain of environments*/
    for (iEnv = iEnvCount - 1; iEnv >= 0; iEnv--) {
    
        /* test every command in the current enviroment */
        iCommand = 0;
        while (strcmp(Environments[iEnv][iCommand].cmd_name, "") != 0) {

            /*
            if (iCommand<3)
                diagnostics(1,"CallCommandFunc (%d,%3d) Trying %s",iEnv,iCommand,Environments[iEnv][iCommand].cmd_name);
            */
        
            if (strcmp(Environments[iEnv][iCommand].cmd_name, cCommand) == 0) {
                if (Environments[iEnv][iCommand].func == NULL)
                    return FALSE;
                // if (*Environments[iEnv][iCommand].func == CmdIgnoreParameter) {
                //     diagnostics(2, "Unknown command '\\%s'", cCommand);
                // }

                diagnostics(5, "CallCommandFunc Found '%s' iEnvCommand=%d number=%d", Environments[iEnv][iCommand].cmd_name, iEnv, iCommand);
                (*Environments[iEnv][iCommand].func) ((Environments[iEnv][iCommand].param));
                return TRUE;    /* Command Function found */
            }
            ++iCommand;
        }
    }
    return FALSE;
}


void CallParamFunc(char *cCommand, int AddParam)

/****************************************************************************
purpose: Try to call the environment-function for the commandname
params:  cCommand - string with command name
     AddParam - param "ORed"(||) to the int param of command-funct
returns: sucess or not
globals: command-functions have side effects or recursive calls
 ****************************************************************************/
{
    int i = 0;
   // char unknown_environment[100];
    diagnostics(3, "CallParamFunc[Beginning] cmd=%s", cCommand);
    while (strcmp(params[i].cmd_name, "") != 0) {
        if (strcmp(params[i].cmd_name, cCommand) == 0) {
            assert(params[i].func != NULL);
            (*params[i].func) ((params[i].param) | AddParam);
            return;             /* command function found */
        }
        ++i;
    }

    // /* unknown environment must be ignored */
    // if (AddParam == ON) {
    //     snprintf(unknown_environment, 100, "\\%s%s%s", "end{", cCommand, "}");
    //     Ignore_Environment(cCommand);
    //     diagnostics(WARNING, "Unknown environment \\begin{%s} ... \\end{%s}", cCommand, cCommand);
    // }
}

/****************************************************************************
purpose: returns a name for the current environment number
 ****************************************************************************/
static char *EnvironmentNameByNumber(int n)
{
    if (n<0) return "";
    return EnvironmentName(Environments[n]);
}

void PushEnvironment(int code)

/****************************************************************************
purpose: adds the command list for a specific environment to the list
     of commands searched through.
params:  constant identifying the environment
globals: changes Environment - array of active environments
         iEnvCount   - counter of active environments
 ****************************************************************************/
{
    char *diag;
    
    // g_par_indent_array[iEnvCount] = getLength("parindent");
    // g_left_indent_array[iEnvCount] = getLeftMarginIndent();
    // g_right_indent_array[iEnvCount] = getRightMarginIndent();
    // g_align_array[iEnvCount] = getAlignment();

    // PushFontSettings();

    switch (code) {
        case DOCUMENT_MODE:
            Environments[iEnvCount] = commands;
            break;
        default:
            diagnostics(ERROR, "assertion failed at function PushEnvironment");
    }
     
    iEnvCount++;
    diag = EnvironmentNameByNumber(iEnvCount-1);
    diagnostics(4, "\\begin{%s} [%d]", diag, iEnvCount-1);
    free(diag);

/*    WriteEnvironmentStack();*/
}

/****************************************************************************
purpose: removes the environment-commands list added by last PushEnvironment;
globals: changes Environment - array of active environments
         iEnvCount - counter of active environments
 ****************************************************************************/
void PopEnvironment(void)
{
    char *this_env, *last_env;

    this_env = EnvironmentNameByNumber(iEnvCount-1);
    last_env = EnvironmentNameByNumber(iEnvCount-2);
    
    /* always pop the current environment */
    --iEnvCount;
    Environments[iEnvCount] = NULL;
    
    // setLength("parindent", g_par_indent_array[iEnvCount]);
    // setLeftMarginIndent(g_left_indent_array[iEnvCount]);
    // setRightMarginIndent(g_right_indent_array[iEnvCount]);
    // setAlignment(g_align_array[iEnvCount]);
    // PopFontSettings();

    diagnostics(3, "\\end{%s} [%d]", this_env, iEnvCount-1);
   
    free(this_env);
    free(last_env);
    
/*    WriteEnvironmentStack(); */
}
