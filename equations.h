#define EQN_DOLLAR         2    /* ('$')  */
#define EQN_RND_OPEN       3    /* ('/(') */
#define EQN_BRACKET_OPEN   4    /* ('/[') */
#define EQN_RND_CLOSE      5    /* ('/)') */
#define EQN_BRACKET_CLOSE  6    /* ('/]') */
#define EQN_ARRAY          8    /* eqnarray environment */
#define EQN_ARRAY_STAR     9    /* eqnarray* environment */
#define EQN_EQUATION      10    /* equation environment */
#define EQN_EQUATION_STAR 11    /* equation* environment */
#define EQN_MATH          12    /* \begin{math} ... \end{math} */
#define EQN_DISPLAYMATH   13    /* \begin{displaymath} ... \end{displaymath} */
#define EQN_DOLLAR_DOLLAR 14    /* \begin{displaymath} ... \end{displaymath} */
#define EQN_NO_NUMBER     15    /* \nonumber */
#define EQN_ENSUREMATH    16    /* \ensuremath */
#define EQN_ALIGN         17    /* align environment */
#define EQN_ALIGN_STAR    18    /* align* environment */

#define LEFT_RIGHT 1
#define RIGHT_LEFT 2
#define LONG_LEFTRIGHT 3
#define LONG_RIGHTLEFT 4
#define LEFT_LEFT 5
#define RIGHT_RIGHT 6
#define LONG_LEFT 7
#define LONG_RIGHT 8
#define RIGHT_LEFT_HARPOONS 9

void CmdMatrixOrCases(int code);
void CmdVecChar(int code);
void CmdLog(int code);
void CmdSepcialLetterToUTF8(int code);
void CmdEquation(int code);
void CmdFraction(int code);
void CmdRoot(int code);
void CmdLim(int code);
void CmdIntegral(int code);
void CmdSuperscript(int code);
void CmdSubscript(int code);
void CmdNonumber(int code);
void CmdArray(int code);
void CmdLeftRight(int code);
void CmdMatrix(int code);
void CmdStackrel(int code);
void CmdArrows(int code);
int  script_shift(void);
int  script_size(void);
void CmdEnsuremath(int code);
void CmdOverLine(int code);
void CmdSlashSlash(int code);
void CmdArraySlashSlash(int height);
void CmdEqnArraySlashSlash(int height);
