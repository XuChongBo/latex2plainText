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

typedef unsigned char   uint8_t;     //无符号8位数

typedef signed   char   int8_t;      //有符号8位数

typedef unsigned int    uint32_t;    //无符号16位数


extern char *g_output_str_ptr;
extern void printRTF(char *pstr);

void setLogFile(FILE *log_file);
void setLogLevel(int level);
char *  my_strndup(const char *s, size_t n);
size_t my_strlcat(char *dst, const char *src, size_t siz);
void diagnostics(int level, char *format, ...);
char *  strdup_noendblanks(const char *s);
char *  my_strcpy(char *dest, const char *src);
int     even(long n);
void fprintRTF(char *format, ...);

int unicode_to_utf8(uint32_t unic, unsigned char *pOut);

static inline void safe_free(char *s)
{
    if (s) free(s); 
}
#endif
