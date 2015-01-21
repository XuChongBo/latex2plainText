#ifndef _CONVERT_H_FILE_INCLUDE
#define _CONVERT_H_FILE_INCLUDE 1

extern int RecursionLevel;
extern int g_processing_eqnarray ;
extern int      g_processing_arrays;
extern int isConvertOK;
void ConvertString(const char *string);
void Convert(void);

int ConvertTheEquationString(const char *src, char *dst);

#endif
