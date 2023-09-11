#include "lists.h"
#include "var.h"
#include "module.h"
#include "throwError.h"
#include "throwWarning.h"

#define CHARACTER_LIMIT 256

#define TRUE 1
#define FALSE 0

#define ERROR_HERE            \
    printf("\nERROR_HERE\n"); \
    goto exit;
#define WARNING_HERE            \
    printf("\nWARNING_HERE\n"); \
    continue;

#define THROW_ERROR(arg)              \
    exitCode = arg;                   \
    throwError(arg, argv, fline.num); \
    goto exit;

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
int intepreterState = 1;
// Runtime booleans
int program = 0;
int get = 1;

int expState = 1;
int falseExp = 0;
int loopExp = 0;
int paramInit = 0;
// Program list
list_ls argBuf;
list_lui16 isStringArg;

list_ls varBuf;
lookupT varTable;
// Variable list
list_lui16 stateVal;
list_lui16 natVal;
list_lui64 nat64Val;
list_lsi32 intVal;
list_lsi64 int64Val;
list_lf32 fracVal;
list_ld64 frac64Val;
list_ls textVal;
// Member list
struct module_list modules;
size_t moduleNum;

#define GET_STATE(index) get_lui16(&stateVal, index)
#define GET_NAT(index) get_lui16(&natVal, index)
#define GET_NAT64(index) get_lui64(&nat64Val, index)
#define GET_INT(index) get_lsi32(&intVal, index)
#define GET_INT64(index) get_lsi64(&int64Val, index)
#define GET_FRAC(index) get_lf32(&fracVal, index)
#define GET_FRAC64(index) get_ld64(&frac64Val, index)
#define GET_TEXT(index) get_ls(&textVal, index)

#define NEW_VALUE(id, name, value)               \
    switch (id)                                  \
    {                                            \
    case 0:                                      \
        pushTable(&varTable, 0, NULL);           \
        push_ls(&varBuf, name);                  \
        break;                                   \
    case 1:                                      \
        pushTable(&varTable, 1, stateVal.size);  \
        push_ls(&varBuf, name);                  \
        push_lui16(&stateVal, value);            \
        break;                                   \
    case 2:                                      \
        pushTable(&varTable, 2, natVal.size);    \
        push_ls(&varBuf, name);                  \
        push_lui16(&natVal, value);              \
        break;                                   \
    case 3:                                      \
        pushTable(&varTable, 3, nat64Val.size);  \
        push_ls(&varBuf, name);                  \
        push_lui64(&nat64Val, value);            \
        break;                                   \
    case 4:                                      \
        pushTable(&varTable, 4, intVal.size);    \
        push_ls(&varBuf, name);                  \
        push_lsi32(&intVal, value);              \
        break;                                   \
    case 5:                                      \
        pushTable(&varTable, 5, int64Val.size);  \
        push_ls(&varBuf, name);                  \
        push_lsi64(&int64Val, value);            \
        break;                                   \
    case 6:                                      \
        pushTable(&varTable, 6, fracVal.size);   \
        push_ls(&varBuf, name);                  \
        push_lf32(&fracVal, value);              \
        break;                                   \
    case 7:                                      \
        pushTable(&varTable, 3, frac64Val.size); \
        push_ls(&varBuf, name);                  \
        push_ld64(&frac64Val, value);            \
        break;                                   \
    case 8:                                      \
        pushTable(&varTable, 8, textVal.size);   \
        push_ls(&varBuf, name);                  \
        push_ls(&textVal, value);                \
        break;                                   \
    }

#define INIT_PROGRAM(PNAME)                  \
    init_ls(&varBuf);                        \
    initTable(&varTable);                    \
    init_ls(&argBuf);                        \
    init_lui16(&isStringArg);                \
    initOffsets(&fline);                     \
                                             \
    init_lui16(&stateVal);                   \
    init_lui16(&natVal);                     \
    init_lui64(&nat64Val);                   \
    init_lsi32(&intVal);                     \
    init_lsi64(&int64Val);                   \
    init_lf32(&fracVal);                     \
    init_ld64(&frac64Val);                   \
    init_ls(&textVal);                       \
    init_ls(&textVal);                       \
                                             \
    push_ls(&varBuf, "#NAME");               \
    pushTable(&varTable, 8, textVal.size);   \
    push_ls(&textVal, PNAME);                \
                                             \
    init_ls(&(modules.moduleNames));         \
    push_ls(&(modules.moduleNames), "main"); \
    push_lui16(&(modules.moduleID), 0);

#define FREE_PROGRAM               \
    free_ls(&varBuf);              \
    freeTable(&varTable);          \
    free_ls(&argBuf);              \
    free_lui16(&isStringArg);      \
    freeOffsets(&(fline.offsets)); \
                                   \
    free_lui16(&stateVal);         \
    free_lui16(&natVal);           \
    free_lui64(&nat64Val);         \
    free_lsi32(&intVal);           \
    free_lsi64(&int64Val);         \
    free_lf32(&fracVal);           \
    free_ld64(&frac64Val);         \
    free_ls(&textVal);             \
                                   \
    freeModules(&modules);         \
                                   \
    printf("\n\nsuccessfully freed memory");

#define GET_STD(name)                                                      \
    push_ls(&(modules.moduleNames), name);                                 \
    push_lui16(&(modules.moduleID), 0);                                    \
    push_ls(&(modules.modules_list[moduleNum].functionNames), "cmdl_out"); \
    init_sig(&(modules.modules_list->functionSignatures));                 \
    struct functionSig cmdl_out = {1, (size_t *)malloc(sizeof(size_t))};   \
    push_sig(&(modules.modules_list->functionSignatures), cmdl_out);