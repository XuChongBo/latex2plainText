#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include "convert.h"
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
    {"2*4 + 5", ""},

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

    {"\\sqrt[3]{645}", ""},
    {"\\sqrt[n]{X}", ""},
    {"\\sqrt{\\frac{   134}{\\sqrt{400}}}", ""},

    /* power */
    {"x^3", ""},
    {"x^3+ 123 - y ^ n ", ""},
    {"\\frac{x^4}{x^n}", "1/2"},

    /* integal */
    {"\\int_{4}^{7}", ""},

    /* sum */
    {"\\sum_{x}^{5}",""},

    /* compound */
    {"\\frac{\\sqrt{ 100 + \\sqrt{  \\frac{x+300}{400} } +5 } } {x^2+y}", "gou20 + gou x + 3 / 2 + 5 / x 2 + y"},


    
    {"\\begin{equation} \\frac{1}{2} \\end{equation}", ""},
    {"$\\frac{\\sqrt{ 100 + \\sqrt{  \\frac{x+300}{400} } +5 } } {x^2+y}$", "gou20 + gou x + 3 / 2 + 5 / x 2 + y"},
    {"\\]\\frac{\\sqrt{ 100 + \\sqrt{  \\frac{x+300}{400} } +5 } } {x^2+y}\\]", "gou20 + gou x + 3 / 2 + 5 / x 2 + y"},
    {"\\frac{1}{2}\\end{equation}", ""},

    /* bad cases which can raise errors.  */
    {"\\sqrt{  \\frac{3 }{4 }  }{2}", ""},

    /* has unicode */
    {"\\sqrt{  \\frac{3 }{4 你}  }{2}", ""},
    {"\\frac{\\sqrt{ 100 + \\sqrt{  \\frac{x+一个量}{分母} } +5 } } {x^次方 \\frac{ok}{le}+偏移}", "gou20 + gou x + 3 / 2 + 5 / x 2 + y"},

    {NULL, NULL}
};   



int main(int argc, char **argv)
{

    /* the global initial */
    FILE *log_file = fopen("log.txt", "w");  
    setLogLevel(4);
    setLogFile(log_file);
    //setLogFile(stderr);

    InitializeStack();
    //OpenRtfFile(g_rtf_name, &fRtf);

    PushEnvironment(DOCUMENT_MODE);  /* because we use ConvertString in preamble.c */
    /*
    PushEnvironment(PREAMBLE_MODE);
    setTexMode(MODE_VERTICAL);
    
    ConvertLatexPreamble();    //handle the Preamble
    setPackageBabel(g_package_babel);
    */ 


    diagnostics(2, "initial ok.");
    //ConvertString("\\begin{equation}");
    char output_str[10000];

    if (0){
        char t[]="x^3+123";
        int isOK = ConvertTheEquationString(t,output_str);
        if (isOK){
            fprintf(stdout,"%s\n",output_str);
        }else{
            fprintf(stdout,"error happens during convert.");
        }
        
    }else {
            /* do batch test */
        CaseStruct *p=cases;
        while(p->latexEq ){
            int isOK = ConvertTheEquationString(p->latexEq,output_str);
            if (isOK){
                fprintf(stdout, "src: %s\n", p->latexEq);
                fprintf(stdout, "dst: %s\n\n", output_str);

            }else{
                fprintf(stdout, "src: %s\n", p->latexEq);
                fprintf(stdout,"error happens during convert.\n\n");
            }


            //fprintf(stdout,"%s\n",output_str);
            p++;
        }

    }

    //ConvertString("\\end{equation}");
    diagnostics(2, "end ok.\n");
    fclose(log_file);
    return 0;
}


