#ifndef _UTILS_H_DEFINED
#define _UTILS_H_DEFINED 1

#define ERROR 0
#define WARNING 1
#define MAXCOMMANDLEN 100

/*** handy boolean type definition ***/
#ifndef TRUE
typedef enum { FALSE = 0,
              TRUE } boolean;
#endif
extern char output_str[];
extern int  output_str_len;
char *  my_strndup(const char *s, size_t n);
size_t my_strlcat(char *dst, const char *src, size_t siz);
void diagnostics(int level, char *format, ...);
char *  strdup_noendblanks(const char *s);
char *  my_strcpy(char *dest, const char *src);
int     even(long n);
void fprintRTF(char *format, ...);
static inline void safe_free(char *s)
{
    if (s) free(s); 
}
#endif
