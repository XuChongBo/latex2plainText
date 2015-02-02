#ifndef _CONVERT_H_FILE_INCLUDE
#define _CONVERT_H_FILE_INCLUDE 1

#define MODE_INTERNAL_VERTICAL     1
#define MODE_HORIZONTAL            2
#define MODE_RESTRICTED_HORIZONTAL 3
#define MODE_MATH                  4
#define MODE_DISPLAYMATH           5
#define MODE_VERTICAL              6


extern int RecursionLevel;
extern int g_processing_eqnarray ;
extern int      g_processing_arrays;
extern int isConvertOK;
void ConvertString(const char *string);
void Convert(void);

int ConvertTheEquationString(const char *src, char *dst);
int getTexMode(void);

#endif
