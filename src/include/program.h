#pragma once

#include "lists.h"
#include "var.h"
#include "module.h"
#include "tok.h"

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
list_lui16 typeArg;

list_ls varBuf;
lookupT varTable;
list_lui16 varMut;
list_lui16 varDef;
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
struct module_data modules;

#define CHECK_MODULE(modID)                             \
    for (size_t i = 0; i < modules.moduleID.size; i++)  \
    {                                                   \
        if (get_lui16(&(modules.moduleID), i) == modID) \
        {                                               \
            ERROR_HERE                                  \
        }                                               \
    }

size_t stringToType(char *str)
{
    if (!strcmp(str, "state"))
    {
        return 1;
    }
    else if (!strcmp(str, "nat") || !strcmp(str, "nat16"))
    {
        return 2;
    }
    else if (!strcmp(str, "nat64"))
    {
        return 3;
    }
    else if (!strcmp(str, "int") || !strcmp(str, "int32"))
    {
        return 4;
    }
    else if (!strcmp(str, "int64"))
    {
        return 5;
    }
    else if (!strcmp(str, "frac") || !strcmp(str, "frac32"))
    {
        return 6;
    }
    else if (!strcmp(str, "frac64"))
    {
        return 7;
    }
    else if (!strcmp(str, "text"))
    {
        return 8;
    }
    else
    {
        return 0;
    }
}

size_t autoType(size_t i)
{
    char *str = get_ls(fline.ftoken->toks, i);

    if (get_lui16(&(fline.ftoken->isStringL), i))
    {
        return 8;
    }
    else if (strcmp(str, "on") && strcmp(str, "off"))
    {
        return autoNumericalType(str);
    }
    else
    {
        return 1;
    }
}

#define NEW_VALUE(id, name, value, endChar)                      \
    if (id > 1 && id < 8 && getLineSize(&fline) < 7)             \
    {                                                            \
        unsigned long long ull = strtoull(value, &endChar, 10);  \
        switch (*endChar)                                        \
        {                                                        \
        case '-':                                                \
            long long ll = strtoll(value, &endChar, 10);         \
            if (*endChar != '\0')                                \
            {                                                    \
                ERROR_HERE                                       \
            }                                                    \
            break;                                               \
        case '.':                                                \
            float dbl = strtof(value, &endChar);                 \
            if (*endChar != '\0')                                \
            {                                                    \
                ERROR_HERE                                       \
            }                                                    \
            break;                                               \
        default:                                                 \
            if (*endChar != '\0')                                \
            {                                                    \
                ERROR_HERE                                       \
            }                                                    \
        }                                                        \
    }                                                            \
                                                                 \
    switch (id)                                                  \
    {                                                            \
    case 0:                                                      \
        ERROR_HERE                                               \
        break;                                                   \
    case 1:                                                      \
        pushTable(&varTable, 1, stateVal.size);                  \
        push_ls(&varBuf, name);                                  \
        if (!strcmp(value, "on"))                                \
        {                                                        \
            push_lui16(&stateVal, TRUE);                         \
        }                                                        \
        else if (!strcmp(value, "off"))                          \
        {                                                        \
            push_lui16(&stateVal, FALSE);                        \
        }                                                        \
        break;                                                   \
    case 2:                                                      \
        unsigned long long ui16 = strtoull(value, &endChar, 10); \
                                                                 \
        pushTable(&varTable, 2, natVal.size);                    \
        push_ls(&varBuf, name);                                  \
        push_lui16(&natVal, (uint16_t)ui16);                     \
        break;                                                   \
    case 3:                                                      \
        unsigned long long ui64 = strtoull(value, &endChar, 10); \
                                                                 \
        pushTable(&varTable, 3, nat64Val.size);                  \
        push_ls(&varBuf, name);                                  \
        push_lui64(&nat64Val, ui64);                             \
        break;                                                   \
    case 4:                                                      \
        long long i32 = atoll(value);                            \
                                                                 \
        pushTable(&varTable, 4, intVal.size);                    \
        push_ls(&varBuf, name);                                  \
        push_lsi32(&intVal, (int32_t)i32);                       \
        break;                                                   \
    case 5:                                                      \
        long long i64 = atoll(value);                            \
                                                                 \
        pushTable(&varTable, 5, int64Val.size);                  \
        push_ls(&varBuf, name);                                  \
        push_lsi64(&int64Val, i64);                              \
        break;                                                   \
    case 6:                                                      \
        double f = atof(value);                                  \
                                                                 \
        pushTable(&varTable, 6, fracVal.size);                   \
        push_ls(&varBuf, name);                                  \
        push_lf32(&fracVal, (float)f);                           \
        break;                                                   \
    case 7:                                                      \
        double d = atof(value);                                  \
                                                                 \
        pushTable(&varTable, 7, frac64Val.size);                 \
        push_ls(&varBuf, name);                                  \
        push_ld64(&frac64Val, d);                                \
        break;                                                   \
    case 8:                                                      \
        pushTable(&varTable, 8, textVal.size);                   \
        push_ls(&varBuf, name);                                  \
        push_ls(&textVal, value);                                \
        break;                                                   \
    }

#define GET_STATE(index) get_lui16(&stateVal, index)
#define GET_NAT(index) get_lui16(&natVal, index)
#define GET_NAT64(index) get_lui64(&nat64Val, index)
#define GET_INT(index) get_lsi32(&intVal, index)
#define GET_INT64(index) get_lsi64(&int64Val, index)
#define GET_FRAC(index) get_lf32(&fracVal, index)
#define GET_FRAC64(index) get_ld64(&frac64Val, index)
#define GET_TEXT(index) get_ls(&textVal, index)

#define NEW_VALUE_FROM_OLD(loc, name)                 \
    char *value;                                      \
    char *c;                                          \
                                                      \
    switch (loc[0])                                   \
    {                                                 \
    case 1:                                           \
        if (GET_STATE(loc[1]))                        \
        {                                             \
            strcpy(value, "on");                      \
        }                                             \
        else                                          \
        {                                             \
            strcpy(value, "off");                     \
        }                                             \
        NEW_VALUE(loc[0], name, value, c)             \
        break;                                        \
    case 2:                                           \
        sprintf(value, "%hu", GET_NAT(loc[1]));       \
        NEW_VALUE(loc[0], name, value, c)             \
        break;                                        \
    case 3:                                           \
        sprintf(value, "%llu", GET_NAT64(loc[1]));    \
        NEW_VALUE(loc[0], name, value, c)             \
        break;                                        \
    case 4:                                           \
        sprintf(value, "%d", GET_INT(loc[1]));        \
        NEW_VALUE(loc[0], name, value, c)             \
        break;                                        \
    case 5:                                           \
        sprintf(value, "%lld", GET_INT64(loc[1]));    \
        NEW_VALUE(loc[0], name, value, c)             \
        break;                                        \
    case 6:                                           \
        sprintf(value, "%f", GET_FRAC(loc[1]));       \
        NEW_VALUE(loc[0], name, value, c)             \
        break;                                        \
    case 7:                                           \
        sprintf(value, "%lf", GET_FRAC64(loc[1]));    \
        NEW_VALUE(loc[0], name, value, c)             \
        break;                                        \
    case 8:                                           \
        NEW_VALUE(loc[0], name, GET_TEXT(loc[1]), c); \
        break;                                        \
    }

#define INIT_PROGRAM(PNAME)                  \
    init_ls(&varBuf);                        \
    initTable(&varTable);                    \
    init_lui16(&varMut);                     \
    init_lui16(&varDef);                     \
    init_ls(&argBuf);                        \
    init_lui16(&isStringArg);                \
    init_lui16(&typeArg);                    \
                                             \
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
    pushBool_lui16(&(varMut), FALSE);        \
    pushBool_lui16(&(varDef), TRUE);         \
                                             \
    init_ls(&(modules.moduleNames));         \
    init_lui16(&(modules.moduleID));         \
                                             \
    push_ls(&(modules.moduleNames), "main"); \
    push_lui16(&(modules.moduleID), 0);      \
    struct module main;                      \
    init_ls(&(main.functionNames));          \
    init_lsig(&(main.functionSignatures));   \
    push_module(&(modules), &main);

#define FREE_PROGRAM               \
    free_ls(&varBuf);              \
    freeTable(&varTable);          \
    free_lui16(&varMut);           \
    free_lui16(&varDef);           \
    free_ls(&argBuf);              \
    free_lui16(&isStringArg);      \
    free_lui16(&typeArg);          \
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
