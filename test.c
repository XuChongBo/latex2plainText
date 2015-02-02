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

    /* special symbol*/
    {"\\alph+123", ""},
    {"\\beta+123", ""},
    {"\\pi+123", ""},
    {"\\gamma+123", ""},
    {"\\delta", ""},
    {"\\epsilon", ""},
    {"\\mu", ""},
    {"\\nu", ""},
    {"\\rho", ""},
    {"\\sigma", ""},
    {"\\phi", ""},
    {"\\psi", ""},
    {"\\omega", ""},
    {"\\Omega", ""},

    /* function */
    {"f(x)+g(y)+h(z) = 100", ""},
    {"f(x)+g(y)+h(z) >= 100", ""},
    {"a\\neq b",""},
    {"a\\neq{b}",""},
    {"\\alph>\\beta",""},
    {"\\alph<\\beta",""},
    {"\\alph\\leq\\beta",""},
    {"\\alph\\geq\\beta",""},


    /* normal */
    {"1+2+y+x+2y", ""},
    {"2 \\times 4 + 5", ""},
    {"a\\div b", ""},
    {"a\\div (b -c) ", ""},
    {"(\\alph + \\beta) \\div (b -c) ", ""},
    {"a\\div\\\\  (b \\\\-c) ", ""},

    /* vector */
    {"\\vec\\alph", ""},
    {"\\vec{a+b}", ""},

    /* matrix */
    {"\\begin{pmatrix} a& 1 & 2\\\\ b&3  &4 \\\\ c&f  &f \\end{pmatrix}", ""},
    {"\\begin{pmatrix} x+y>0 \\\\ x-y<9 \\\\ y+z>0 \\end{pmatrix}",""},
    {"\\begin{pmatrix} \\frac{x}{\\beta}+y>0 \\\\ x-y<9 \\\\ \\gamma+z>0 \\end{pmatrix}",""},
    {"\\begin{cases} \\frac{x}{\\beta}+y>0 \\\\ x-y<9 \\\\ \\gamma+z>0 \\end{cases}",""},
    {"\\begin{cases} x+y=0 \\\\ a+b \\\\a>sc\\end{cases}",""},


    /* superscript, subscript*/ 
    {"x_i", ""},
    {"x_\\alph", ""},
    {"\\alph_\\beta", ""},
    {"\\alph_i", ""},
    {"\\frac{x}{y_i}", ""},
    {"x^i", ""},
    {"\\alph^i", ""},

    /* arrow, overline, bracket, brace*/
    {"\\sum_{i=1}^{n}{(X_i - \\overline{X})^2}", ""},
    {"{454}",""},
    {"{454}+{xx}",""},
    {"\\frac{{454}}{xx}",""},

    /* frac */
    {"\\frac{1}{2}", "1/2"},
    {"\\frac{x}{y}", "x/y"},
    {"x+\\frac{2}{3}", "x+2/3"},
    {"\\frac{\\frac{3}{4}}{2}", "3/4/2"},
    {"\\frac{  \\frac{3 }{4 }  }{2}", ""},
    {"x = a_0 + \\frac{1}{a_1 + \\frac{1}{a_2 + \\frac{1}{a_3 + a_4}}}",""},
    {"\\frac{n!}{r!(n-r)!} ",""},
    {"\\frac{x}{\\beta}",""},
   
    /* sqrt */
    {"\\sqrt{  \\frac{3 }{4 }  }{2}", ""},
    {"\\sqrt{x+1}", "gou x+1"},
    {"\\sqrt{a^2+b^2}",""},
    {"\\sqrt{\\sqrt{x+1}}", "gou gou x+1"},

    {"\\sqrt[3]{645}", ""},
    {"\\sqrt[n]{X}", ""},
    {"\\sqrt{\\frac{   134}{\\sqrt{400}}}", ""},

    /* power */
    {"x^3", ""},
    {"x^3+ 123 - y ^ n ", ""},
    {"\\frac{x^4}{x^n}", "1/2"},
    {"\\alph^4", ""},

    /* log  ln*/
    {"\\ln x", ""},
    {"\\ln 3x", ""},
    {"\\ln 3x", ""},
    {"\\log \\log_{10}90", ""},
    {"\\log_{e}45 + \\log_{\\alph}\\beta", ""},
    {"\\frac{\\log_{e}45 } { \\log_{\\alph^\\beta}}", ""},

    /* integal */
    {"\\int_{4}^{7}", ""},
    {"\\int_0^{\\infty} \\frac{1}^{x} dx", ""},

    /* sum */
    {"\\sum_{x}^{5}",""},

    /* compound */
    {"\\frac{\\sqrt{ 100 + \\sqrt{  \\frac{x+300}{400} } +5 } } {x^2+y}", "gou20 + gou x + 3 / 2 + 5 / x 2 + y"},

    /* has unicode */
    {"\\sqrt{  \\frac{3 }{4 你}  }{2}", ""},
    {"\\frac{\\sqrt{ 100 + \\sqrt{  \\frac{x+一个量}{分母} } +5 } } {x^次方 \\frac{ok}{le}+偏移}", "gou20 + gou x + 3 / 2 + 5 / x 2 + y"},
    /* inline math or xx math*/
    {"\\begin{equation} \\frac{1}{2} \\end{equation}", ""},
    {"$\\frac{\\sqrt{ 100 + \\sqrt{  \\frac{x+300}{400} } +5 } } {x^2+y}$", "gou20 + gou x + 3 / 2 + 5 / x 2 + y"},
    {"\\]\\frac{\\sqrt{ 100 + \\sqrt{  \\frac{x+300}{400} } +5 } } {x^2+y}\\]", "gou20 + gou x + 3 / 2 + 5 / x 2 + y"},
    {"\\frac{1}{2}\\end{equation}", ""},

    /* bad cases which can raise errors.  */
    {"\\sqrt{  \\frac{3 }{4 }  }{2}", ""},
    {"(1,f(x)", ""},
    {"0≤y≤a",""},

    {"$\\sqrt{  \\frac{3 }{4 你}  }{2}$这是两个公式 $\\sqrt{  \\frac{3 }{4 你}  }{2}$", ""},
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


    
    //
    if(argc>=2){
         FILE* caseFile = fopen(argv[1], "r");
         if(caseFile==NULL) {
            printf("read file error!\n");
            return -1;
         }
        char strLine[1024];  //the buffer for text line.
        int i=0;
        while(!feof(caseFile)){
            fgets(strLine, 1024,caseFile);
            if(strlen(strLine)<2)
                continue;
            //printf("1-%s",strLine);
            i++;
            int isOK = ConvertTheEquationString(strLine,output_str);
            fprintf(stdout, "case %d\n", i);
            if (isOK){
                fprintf(stdout, "src: %s\n", strLine);
                fprintf(stdout, "dst: %s\n\n", output_str);

            }else{
                fprintf(stdout, "src: %s\n", strLine);
                fprintf(stdout,"error happens during convert.\n\n");
            }



        }
        fclose(caseFile);
        return 0;
    }

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
        int i = 0;
        while(p->latexEq ){
            i++;
            int isOK = ConvertTheEquationString(p->latexEq,output_str);
            fprintf(stdout, "case %d\n", i);
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


