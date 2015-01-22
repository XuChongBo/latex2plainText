

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <assert.h>
#ifdef UNIX
# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>
#else
# include <stdio.h>
#endif



#include "utils.h"



extern int RecursionLevel;
extern int BraceLevel;

char *g_output_str_ptr = NULL;  // assignment by the user

static int g_log_level = 6;  //0--ERROR 1--WARNING 2-INFO  6-DEBUG 

static FILE *g_log_file = NULL;



void printRTF(char *pstr)
{

    char *p_out_str = g_output_str_ptr + strlen(g_output_str_ptr);
    while(*pstr){
        *p_out_str++ = *pstr++;

    }
    *p_out_str = '\0';
}


void fprintRTF(char *format, ...)
{
    int n;
    va_list apf;
    if(!g_output_str_ptr)
        return;

    va_start(apf, format);
    int len = strlen(g_output_str_ptr);
    n=vsprintf(g_output_str_ptr+len,format,apf);

    //check if ok
    if(n<0){
       diagnostics(ERROR, "in fprintRTF");
       strcpy(g_output_str_ptr,"ERROR");
       //TODO raise an exception
    }
    diagnostics(2, "output <%s>",g_output_str_ptr+len);
    // fprintf(stdout, "output: ");
    // vfprintf(stdout, format, apf);
    //fprintf(stdout,"%s",output_str);
    va_end(apf);
}



/*
 * Appends src to string dst of size siz (unlike strncat, siz is the
 * full size of dst, not space left).  At most siz-1 characters
 * will be copied.  Always NUL terminates (unless siz <= strlen(dst)).
 * Returns strlen(src) + MIN(siz, strlen(initial dst)).
 * If retval >= siz, truncation occurred.
 */
size_t
my_strlcat(char *dst, const char *src, size_t siz)
{
        char *d = dst;
        const char *s = src;
        size_t n = siz;
        size_t dlen;

        /* Find the end of dst and adjust bytes left but don't go past end */
        while (n-- != 0 && *d != '\0')
                d++;
        dlen = d - dst;
        n = siz - dlen;

        if (n == 0)
                return(dlen + strlen(s));
        while (*s != '\0') {
                if (n != 1) {
                        *d++ = *s;
                        n--;
                }
                s++;
        }
        *d = '\0';

        return(dlen + (s - src));        /* count does not include NUL */
}
/******************************************************************************
 purpose:  returns true if n is even
******************************************************************************/
int even(long n)
{
    return (int) (!(n & 1));
}

/******************************************************************************
 purpose: this works with overlapping pointers ... the C standard says that
   strcpy can do anything it likes for this case (which it happens to
   do under Fedora 
******************************************************************************/
char *my_strcpy(char *dest, const char *src)
{
   char *save = dest;
   while ( (*dest++ = *src++) );
   return save;
} 

/******************************************************************************
 purpose:  returns a new string having n characters from src terminated with
           '\0' at the end.  (so the length is n+1)
******************************************************************************/
char *my_strndup(const char *src, size_t n)
{
    char *dst;

    dst = (char *) calloc(n + 1, sizeof(char));
    if (dst == NULL)
        return NULL;

    strncpy(dst, src, n);

    return dst;
}


/******************************************************************************
 purpose:  duplicates a string without spaces or newlines at front or end
******************************************************************************/
char *strdup_noendblanks(const char *s)
{
    char *p, *t;

    if (s == NULL)
        return NULL;
    if (*s == '\0')
        return strdup("");

    /* find pointer to first non-space character in string */
    t = (char *) s;
    while (*t == ' ' || *t == '\n')
        t++;                    /* first non blank char */

    /* find pointer to last non-space character in string */
    p = (char *) s + strlen(s) - 1;
    while (p >= t && (*p == ' ' || *p == '\n'))
        p--;                    /* last non blank char */

    if (t > p)
        return strdup("");
    return my_strndup(t, (size_t) (p - t + 1));
}



//0--ERROR 1--WARNING 2-INFO  6-DEBUG        the default value is 6.
void setLogLevel(int level)
{
    g_log_level = level;
}

void setLogFile(FILE *log_file)
{
    g_log_file = log_file;
}
void diagnostics(int level, char *format, ...)

/****************************************************************************
purpose: Writes the message to stderr depending on debugging level
 ****************************************************************************/
{
    if (!g_log_file){
        return;
    }


    static int first = TRUE;

    char buffer[512], *buff_ptr;
    va_list apf;
    int i;

    buff_ptr = buffer;

    va_start(apf, format);

    if (level <= g_log_level) {

        //CurrentEnvironmentCount();

        if (!first) fprintf(g_log_file,"\n");
        

        switch (level) {
            case 0: //ERROR
                fprintf(g_log_file, "[ERROR]   ");
                break;
            case 1: //WARNING
                fprintf(g_log_file, "[WARNING] ");
                // if (g_RTF_warnings) {
                //     vsnprintf(buffer, 512, format, apf);
                //     fprintRTF("{\\plain\\cf2 [latex2rtf:");
                //     while (*buff_ptr) {
                //         putRtfCharEscaped(*buff_ptr);
                //         buff_ptr++;
                //     }
                //     fprintRTF("]}");
                // }
                break;
            case 2:  //INFO
                fprintf(g_log_file, "[INFO]    ");
                break;
            case 3:
                fprintf(g_log_file, "[DEBUG]   ");
                break;

            case 4:
            case 5:
            case 6: //DEBUG
                fprintf(g_log_file, "[DEBUG]   Level=%d ", RecursionLevel);
                for (i = 0; i < BraceLevel; i++)
                    fprintf(g_log_file, "{");
                for (i = 8; i > BraceLevel; i--)
                    fprintf(g_log_file, " ");

                for (i = 0; i < RecursionLevel; i++)
                    fprintf(g_log_file, "  ");
                break;
            default:
                break;
        }
        vfprintf(g_log_file, format, apf);
        first = FALSE;
    }
    va_end(apf);

    if (level == 0) {
        fprintf(g_log_file, "\n");
        fflush(g_log_file);
        //exit(EXIT_FAILURE);   //use exception in other place.
    }
}



/***************************************************************************** 
 * translate  one Unicode(UCS-2和UCS-4) letter  to UTF-8 
 * 
 * params: 
 *    unic     Unicode value
 *    pOutput  point to the result.
 *    outsize  pOutput buffer size 
 * 
 * return: 
 *    the encode len.    0 stands for error. 
 * 
 * Note: 
 *     1. low bits piror  in unic.
 *     2. make sure pOutput has 6+1 bytes memory.
 *
 * from http://blog.csdn.net/tge7618291/article/details/7599902
 ****************************************************************************/  
int unicode_to_utf8(uint32_t unic, unsigned char *pOutput)  
{  
    assert(pOutput != NULL);  
    int nSize = 0;
    if ( unic <= 0x0000007F )  
    {  
        // * U-00000000 - U-0000007F:  0xxxxxxx  
        *pOutput     = (unic & 0x7F);  
        nSize = 1;

    }  
    else if ( unic >= 0x00000080 && unic <= 0x000007FF )  
    {  
        // * U-00000080 - U-000007FF:  110xxxxx 10xxxxxx  
        *(pOutput+1) = (unic & 0x3F) | 0x80;  
        *pOutput     = ((unic >> 6) & 0x1F) | 0xC0;  
        nSize = 2;

    }  
    else if ( unic >= 0x00000800 && unic <= 0x0000FFFF )  
    {  
        // * U-00000800 - U-0000FFFF:  1110xxxx 10xxxxxx 10xxxxxx  
        *(pOutput+2) = (unic & 0x3F) | 0x80;  
        *(pOutput+1) = ((unic >>  6) & 0x3F) | 0x80;  
        *pOutput     = ((unic >> 12) & 0x0F) | 0xE0;  
        nSize = 3;
 
    }  
    else if ( unic >= 0x00010000 && unic <= 0x001FFFFF )  
    {  
        // * U-00010000 - U-001FFFFF:  11110xxx 10xxxxxx 10xxxxxx 10xxxxxx  
        *(pOutput+3) = (unic & 0x3F) | 0x80;  
        *(pOutput+2) = ((unic >>  6) & 0x3F) | 0x80;  
        *(pOutput+1) = ((unic >> 12) & 0x3F) | 0x80;  
        *pOutput     = ((unic >> 18) & 0x07) | 0xF0;  
        nSize = 4;

    }  
    else if ( unic >= 0x00200000 && unic <= 0x03FFFFFF )  
    {  
        // * U-00200000 - U-03FFFFFF:  111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx  
        *(pOutput+4) = (unic & 0x3F) | 0x80;  
        *(pOutput+3) = ((unic >>  6) & 0x3F) | 0x80;  
        *(pOutput+2) = ((unic >> 12) & 0x3F) | 0x80;  
        *(pOutput+1) = ((unic >> 18) & 0x3F) | 0x80;  
        *pOutput     = ((unic >> 24) & 0x03) | 0xF8;  
        nSize = 5;

    }  
    else if ( unic >= 0x04000000 && unic <= 0x7FFFFFFF )  
    {  
        // * U-04000000 - U-7FFFFFFF:  1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx  
        *(pOutput+5) = (unic & 0x3F) | 0x80;  
        *(pOutput+4) = ((unic >>  6) & 0x3F) | 0x80;  
        *(pOutput+3) = ((unic >> 12) & 0x3F) | 0x80;  
        *(pOutput+2) = ((unic >> 18) & 0x3F) | 0x80;  
        *(pOutput+1) = ((unic >> 24) & 0x3F) | 0x80;  
        *pOutput     = ((unic >> 30) & 0x01) | 0xFC;  
        nSize = 6;

    }  
  
    *(pOutput+nSize) = '\0';
    return nSize;  
}  
// #c---end  
