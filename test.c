#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include "commands.h"
#include "convert.h"
#include "stack.h"
#include "utils.h"

typedef struct CaseStructTag {
    char *latexEq; 
    char *outEq;
} CaseStruct;

static CaseStruct cases[] = {
    /* normal */
    {"1+2+y+x+2y", ""},
    {"2*4+5", ""},

    /* frac */
    {"\\frac{1}{2}", "1/2"},
    {"\\frac{x}{y}", "x/y"},
    {"x+\\frac{2}{3}", "x+2/3"},
    {"\\frac{\\frac{3}{4}}{2}", "3/4/2"},
    {"\\frac{  \\frac{3 }{4 }  }{2}", ""},

    /* sqrt */
    {"\\sqrt{  \\frac{3 }{4 }  }{2}", ""},
    {"\\sqrt{x+1}", "gou x+1"},
    {"\\sqrt{\\sqrt{x+1}}", "gou gou x+1"},

    {"\\sqrt{\\frac{   134}{\\sqrt{400}}}", ""},

    /* power */
    {"x^3", ""},
    {"x^3+123", ""},

    /* compound */
    {"\\frac{\\sqrt{ 100 + \\sqrt{  \\frac{x+300}{400} } +5 } } {x^2+y}", "gou20 + gou x + 3 / 2 + 5 / x 2 + y"},


    /* bad cases */
    {"\\frac{1}{2}\\end{equation}", ""},
    {NULL, NULL}
};   



int main(int argc, char **argv)
{
    InitializeStack();
    //OpenRtfFile(g_rtf_name, &fRtf);

    PushEnvironment(DOCUMENT_MODE);  /* because we use ConvertString in preamble.c */
    /*
    PushEnvironment(PREAMBLE_MODE);
    setTexMode(MODE_VERTICAL);
    
    ConvertLatexPreamble();    //handle the Preamble
    setPackageBabel(g_package_babel);
    */ 

    //char t[]=;
    //char t[]=;
    //char t[]=;
    //char t[]=;
    //char t[]=;
    //char t[]="\\sqrt{100}";

    //char t[]="2+3";


    diagnostics(2, "begin");
    //ConvertString("\\begin{equation}");
    


    /* do batch test */
    CaseStruct *p=cases;

    while(p->latexEq)
    {

        

        ConvertString(p->latexEq);
        fprintf(stderr, "\ntest convert   %s  out: %s \n", p->latexEq, output_str);

        //fprintf(stdout,"%s\n",output_str);
        output_str_len=0;

        p++;
    }


    //ConvertString("\\end{equation}");
    diagnostics(2, "end\n");

    

    return 0;
}


