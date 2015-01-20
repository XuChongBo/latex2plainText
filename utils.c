#if defined(NOSTDERR)
#define ERROUT stdout
#else
#define ERROUT stderr
#endif 

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

//int OUTPUT_ENABLE = FALSE; //TRUE;
int OUTPUT_ENABLE = TRUE; 

extern int RecursionLevel;
extern int BraceLevel;

char output_str[1000];
int  output_str_len = 0;

void fprintRTF(char *format, ...)
{
    // TODO
    int n;
    va_list apf;
    va_start(apf, format);
    output_str[output_str_len]='\0';
    char *p=output_str;
    n=vsprintf(p+output_str_len,format,apf);
    output_str_len+=n;
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

void diagnostics(int level, char *format, ...)

/****************************************************************************
purpose: Writes the message to stderr depending on debugging level
 ****************************************************************************/
{

    if(!OUTPUT_ENABLE){
        return;
    }
    static int first = TRUE;
    int g_verbosity_level = 2;
    char buffer[512], *buff_ptr;
    va_list apf;
    int i;

    buff_ptr = buffer;

    va_start(apf, format);

    if (level <= g_verbosity_level) {

        //CurrentEnvironmentCount();

        if (!first) fprintf(ERROUT,"\n");
        

        switch (level) {
            case 0:
                fprintf(ERROUT, "[Error]  ");
                break;
            case 1:
                fprintf(ERROUT, "[Warning] ");
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
            case 5:
            case 6:
            case 2:
            case 3:
            case 4:
                fprintf(ERROUT, "[INFO] RecursionLevel=%d ", RecursionLevel);
                for (i = 0; i < BraceLevel; i++)
                    fprintf(ERROUT, "{");
                for (i = 8; i > BraceLevel; i--)
                    fprintf(ERROUT, " ");

                for (i = 0; i < RecursionLevel; i++)
                    fprintf(ERROUT, "  ");
                break;
            default:
                break;
        }
        vfprintf(ERROUT, format, apf);
        first = FALSE;
    }
    va_end(apf);

    if (level == 0) {
        fprintf(ERROUT, "\n");
        fflush(ERROUT);
            
        exit(EXIT_FAILURE);
    }
}
