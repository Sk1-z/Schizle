#include "lists.h"
#include "var.h"

#define CHARACTER_LIMIT 256

#define TRUE 1
#define FALSE 0

#define ERROR_HERE printf("\nERROR_HERE\n");
#define WARNING_HERE printf("\nWARNING_HERE\n");

// --Debug--
int pointNum = 1;

#define DEBUG 0
#if DEBUG == 1
#define PRINTTYPEDB(str) printf("\n%s\n", str)
#define PRINT_POINT                   \
    printf("\nPoint %d\n", pointNum); \
    pointNum++;
#else
#define PRINTTYPEDB(str)
#define PRINT_POINT
#endif

// Initialize reading variables
int noWarning = 1;
char lineBuf[CHARACTER_LIMIT];

line fline;

int exitCode = 0;
// Runtime booleans
int program = 0;
int falseExp = 0;
int looping = 0;
int paramInit = 0;
// Program list
list_s argBuf;
list_si32 argIsStringL;

list_s varBuf;
lookupT varTable;
// Variable list
list_s textVal;

#define INIT_PROGRAM(PNAME)     \
    init_ls(&textVal);          \
    init_ls(&varBuf);           \
    push_ls(&varBuf, "_name");  \
    push_ls(&textVal, PNAME);   \
    initTable(&varTable);       \
    pushTable(&varTable, 8, 0); \
    init_ls(&argBuf);           \
    init_lsi32(&argIsStringL);
