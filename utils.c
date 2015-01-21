

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>

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

void appendToOutputStr(char c)
{
    int len = strlen(g_output_str_ptr);
    //append
    *(g_output_str_ptr+len) = c;
    *(g_output_str_ptr+len+1) = '\0';

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
