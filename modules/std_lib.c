#pragma once

#include "../src/include/program.h"

// module get statement
#define GET_STD(name)                                                                                                  \
    push_ls(&varBuf, "_VERSION_");                                                                                     \
    pushTable(&varTable, 8, textVal.size);                                                                             \
    push_ls(&textVal, "Schizle Script v0.4.0-alpha");                                                                  \
    pushBool_lui16(&(varMut), FALSE);                                                                                  \
    pushBool_lui16(&(varDef), TRUE);                                                                                   \
                                                                                                                       \
    push_ls(&varBuf, "_on_");                                                                                          \
    pushTable(&varTable, 1, stateVal.size);                                                                            \
    pushBool_lui16(&stateVal, TRUE);                                                                                   \
    pushBool_lui16(&(varMut), FALSE);                                                                                  \
    pushBool_lui16(&(varDef), TRUE);                                                                                   \
                                                                                                                       \
    push_ls(&varBuf, "_off_");                                                                                         \
    pushTable(&varTable, 1, stateVal.size);                                                                            \
    pushBool_lui16(&stateVal, FALSE);                                                                                  \
    pushBool_lui16(&(varMut), FALSE);                                                                                  \
    pushBool_lui16(&(varDef), TRUE);                                                                                   \
                                                                                                                       \
    push_ls(&varBuf, "_N16_MAX_");                                                                                     \
    pushTable(&varTable, 2, natVal.size);                                                                              \
    push_lui16(&natVal, _NAT16_LIMIT_);                                                                                \
    pushBool_lui16(&(varMut), FALSE);                                                                                  \
    pushBool_lui16(&(varDef), TRUE);                                                                                   \
                                                                                                                       \
    push_ls(&varBuf, "_N64_MAX_");                                                                                     \
    pushTable(&varTable, 3, nat64Val.size);                                                                            \
    push_lui64(&nat64Val, _NAT64_LIMIT_);                                                                              \
    pushBool_lui16(&(varMut), FALSE);                                                                                  \
    pushBool_lui16(&(varDef), TRUE);                                                                                   \
                                                                                                                       \
    push_ls(&varBuf, "_I32_MIN_");                                                                                     \
    pushTable(&varTable, 4, intVal.size);                                                                              \
    push_lsi32(&intVal, -_INT32_LIMIT_);                                                                               \
    pushBool_lui16(&(varMut), FALSE);                                                                                  \
    pushBool_lui16(&(varDef), TRUE);                                                                                   \
                                                                                                                       \
    push_ls(&varBuf, "_I32_MAX_");                                                                                     \
    pushTable(&varTable, 4, intVal.size);                                                                              \
    push_lsi32(&intVal, _INT32_LIMIT_);                                                                                \
    pushBool_lui16(&(varMut), FALSE);                                                                                  \
    pushBool_lui16(&(varDef), TRUE);                                                                                   \
                                                                                                                       \
    push_ls(&varBuf, "_I64_MIN_");                                                                                     \
    pushTable(&varTable, 5, int64Val.size);                                                                            \
    push_lsi64(&int64Val, -_INT64_LIMIT_);                                                                             \
    pushBool_lui16(&(varMut), FALSE);                                                                                  \
    pushBool_lui16(&(varDef), TRUE);                                                                                   \
                                                                                                                       \
    push_ls(&varBuf, "_I64_MAX_");                                                                                     \
    pushTable(&varTable, 5, int64Val.size);                                                                            \
    push_lsi64(&int64Val, _INT64_LIMIT_);                                                                              \
    pushBool_lui16(&(varMut), FALSE);                                                                                  \
    pushBool_lui16(&(varDef), TRUE);                                                                                   \
                                                                                                                       \
    push_ls(&moduleNames, name);                                                                                       \
    push_lui16(&moduleID, 1);                                                                                          \
                                                                                                                       \
    struct module std;                                                                                                 \
    init_lsig(&(std.functionSignatures));                                                                              \
    init_ls(&(std.functionNames));                                                                                     \
                                                                                                                       \
    struct functionSig sout_sig;                                                                                       \
    init_sig(&sout_sig);                                                                                               \
    sout_sig.numArgs = 1;                                                                                              \
    push_lui16(&(sout_sig.argID), 0);                                                                                  \
    sout_sig.call = sout;                                                                                              \
                                                                                                                       \
    push_sig(&(std.functionSignatures), &sout_sig);                                                                    \
    push_ls(&(std.functionNames), "sout");                                                                             \
                                                                                                                       \
    struct functionSig soutln_sig;                                                                                     \
    init_sig(&soutln_sig);                                                                                             \
    soutln_sig.numArgs = 1;                                                                                            \
    push_lui16(&(soutln_sig.argID), 0);                                                                                \
    soutln_sig.call = soutln;                                                                                          \
                                                                                                                       \
    push_sig(&(std.functionSignatures), &soutln_sig);                                                                  \
    push_ls(&(std.functionNames), "soutln");                                                                           \
                                                                                                                       \
    struct functionSig sinln_sig;                                                                                      \
    init_sig(&sinln_sig);                                                                                              \
    sinln_sig.numArgs = 1;                                                                                             \
    push_lui16(&(sinln_sig.argID), 8);                                                                                 \
    sinln_sig.call = sinln;                                                                                            \
                                                                                                                       \
    push_sig(&(std.functionSignatures), &sinln_sig);                                                                   \
    push_ls(&(std.functionNames), "sinln");                                                                            \
                                                                                                                       \
    struct functionSig text_frac_sig;                                                                                  \
    init_sig(&text_frac_sig);                                                                                          \
    text_frac_sig.numArgs = 2;                                                                                         \
    push_lui16(&(text_frac_sig.argID), 7);                                                                             \
    push_lui16(&(text_frac_sig.argID), 9);                                                                             \
    text_frac_sig.call = text_frac;                                                                                    \
                                                                                                                       \
    push_sig(&(std.functionSignatures), &text_frac_sig);                                                               \
    push_ls(&(std.functionNames), "text>frac");                                                                        \
                                                                                                                       \
    struct functionSig text_int_sig;                                                                                   \
    init_sig(&text_int_sig);                                                                                           \
    text_int_sig.numArgs = 2;                                                                                          \
    push_lui16(&(text_int_sig.argID), 5);                                                                              \
    push_lui16(&(text_int_sig.argID), 9);                                                                              \
    text_int_sig.call = text_int;                                                                                      \
                                                                                                                       \
    push_sig(&(std.functionSignatures), &text_int_sig);                                                                \
    push_ls(&(std.functionNames), "text>int");                                                                         \
                                                                                                                       \
    struct functionSig int_frac_sig;                                                                                   \
    init_sig(&int_frac_sig);                                                                                           \
    int_frac_sig.numArgs = 2;                                                                                          \
    push_lui16(&(int_frac_sig.argID), 7);                                                                              \
    push_lui16(&(int_frac_sig.argID), 5);                                                                              \
    int_frac_sig.call = int_frac;                                                                                      \
                                                                                                                       \
    push_sig(&(std.functionSignatures), &int_frac_sig);                                                                \
    push_ls(&(std.functionNames), "int>frac");                                                                         \
                                                                                                                       \
    struct functionSig frac_int_sig;                                                                                   \
    init_sig(&frac_int_sig);                                                                                           \
    frac_int_sig.numArgs = 2;                                                                                          \
    push_lui16(&(frac_int_sig.argID), 5);                                                                              \
    push_lui16(&(frac_int_sig.argID), 7);                                                                              \
    frac_int_sig.call = frac_int;                                                                                      \
                                                                                                                       \
    push_sig(&(std.functionSignatures), &frac_int_sig);                                                                \
    push_ls(&(std.functionNames), "frac>int");                                                                         \
                                                                                                                       \
    struct functionSig type_sig;                                                                                       \
    init_sig(&type_sig);                                                                                               \
    type_sig.numArgs = 2;                                                                                              \
    push_lui16(&(type_sig.argID), 8);                                                                                  \
    push_lui16(&(type_sig.argID), 0);                                                                                  \
    type_sig.call = type;                                                                                              \
                                                                                                                       \
    push_sig(&(std.functionSignatures), &type_sig);                                                                    \
    push_ls(&(std.functionNames), "type");                                                                             \
                                                                                                                       \
    struct functionSig do_script_sig;                                                                                  \
    init_sig(&do_script_sig);                                                                                          \
    do_script_sig.numArgs = 1;                                                                                         \
    push_lui16(&(do_script_sig.argID), 9);                                                                             \
    do_script_sig.call = do_script;                                                                                    \
                                                                                                                       \
    push_sig(&(std.functionSignatures), &do_script_sig);                                                               \
    push_ls(&(std.functionNames), "do_script");                                                                        \
                                                                                                                       \
    struct functionSig timeout_sig;                                                                                    \
    init_sig(&timeout_sig);                                                                                            \
    timeout_sig.numArgs = 1;                                                                                           \
    push_lui16(&(timeout_sig.argID), 2);                                                                               \
    timeout_sig.call = timeout;                                                                                        \
                                                                                                                       \
    push_sig(&(std.functionSignatures), &timeout_sig);                                                                 \
    push_ls(&(std.functionNames), "timeout");                                                                          \
                                                                                                                       \
    modules[modCount] = std;                                                                                           \
    modCount++;

// Standard out
size_t sout()
{
    if (get_lui16(&isStringArg, 0))
    {
        printf(get_ls(&argBuf, 0));
    }
    else
    {
        size_t *loc = getVarLookUp(&varTable, getIndex_ls(&varBuf, get_ls(&argBuf, 0)));
        switch (loc[0])
        {
        case 1:
            if (get_lui16(&stateVal, loc[1]) == 1)
            {
                printf("on");
            }
            else if (get_lui16(&stateVal, loc[1]) == 0)
            {
                printf("off");
            }
            break;
        case 2:
            printf("%hu", get_lui16(&natVal, loc[1]));
            break;
        case 3:
            printf("%llu", get_lui64(&nat64Val, loc[1]));
            break;
        case 4:
            printf("%d", get_lsi32(&intVal, loc[1]));
            break;
        case 5:
            printf("%lld", get_lsi64(&int64Val, loc[1]));
            break;
        case 6:
            printf("%f", get_lf32(&fracVal, loc[1]));
            break;
        case 7:
            printf("%lf", get_ld64(&frac64Val, loc[1]));
            break;
        case 8:
            printf(get_ls(&textVal, loc[1]));
            break;
        }
        free(loc);
    }

    return 0;
}

size_t soutln()
{
    size_t r = sout();
    printf("\n");
    return r;
}

size_t sinln()
{
    char in[CHARACTER_LIMIT];
    fgets(in, sizeof(in), stdin);

    in[strcspn(in, "\n")] = '\0';
    set_ls(&textVal, getVarLookUp(&varTable, getIndex_ls(&varBuf, get_ls(&argBuf, 0)))[1], in);
    return 0;
}

size_t text_frac()
{
    size_t *loc = getVarLookUp(&varTable, getIndex_ls(&varBuf, get_ls(&argBuf, 0)));

    if (get_lui16(&isStringArg, 1))
    {
        switch (loc[0])
        {
        case 6:
            set_lf32(&fracVal, loc[1], (float)atof(get_ls(&argBuf, 1)));
            break;
        case 7:
            set_ld64(&frac64Val, loc[1], atof(get_ls(&argBuf, 1)));
            break;
        }
    }
    else
    {
        size_t *locStr = getVarLookUp(&varTable, getIndex_ls(&varBuf, get_ls(&argBuf, 1)));

        char *str = get_ls(&textVal, locStr[1]);

        switch (loc[0])
        {
        case 6:
            set_lf32(&fracVal, loc[1], (float)atof(str));
            break;
        case 7:
            set_ld64(&frac64Val, loc[1], atof(str));
            break;
        }

        free(locStr);
    }

    free(loc);

    return 0;
}

size_t text_int()
{
    size_t *loc = getVarLookUp(&varTable, getIndex_ls(&varBuf, get_ls(&argBuf, 0)));

    if (get_lui16(&isStringArg, 1))
    {
        switch (loc[0])
        {
        case 2:
            set_lui16(&natVal, loc[1], (uint16_t)atoll(get_ls(&argBuf, 1)));
            break;
        case 3:
            set_lui64(&nat64Val, loc[1], (uint64_t)atoll(get_ls(&argBuf, 1)));
            break;
        case 4:
            set_lsi32(&intVal, loc[1], (int32_t)atoll(get_ls(&argBuf, 1)));
            break;
        case 5:
            set_lsi64(&int64Val, loc[1], atoll(get_ls(&argBuf, 1)));
            break;
        }
    }
    else
    {
        char *str = get_ls(&textVal, getVarLookUp(&varTable, getIndex_ls(&varBuf, get_ls(&argBuf, 1)))[1]);

        switch (loc[0])
        {
        case 2:
            set_lui16(&natVal, loc[1], (uint16_t)atoll(str));
            break;
        case 3:
            set_lui64(&nat64Val, loc[1], (uint64_t)atoll(str));
            break;
        case 4:
            set_lsi32(&intVal, loc[1], (int)atoll(str));
            break;
        case 5:
            set_lsi64(&int64Val, loc[1], atoll(str));
            break;
        }
    }

    free(loc);

    return 0;
}

size_t int_frac()
{
    size_t *loc = getVarLookUp(&varTable, getIndex_ls(&varBuf, get_ls(&argBuf, 0)));
    size_t *locInt = getVarLookUp(&varTable, getIndex_ls(&varBuf, get_ls(&argBuf, 1)));

    double d;
    switch (locInt[0])
    {
    case 2:
        d = (double)GET_NAT(locInt[1]);
        break;
    case 3:
        d = (double)GET_NAT64(locInt[1]);
        break;
    case 4:
        d = (double)GET_INT(locInt[1]);
        break;
    case 5:
        d = (double)GET_INT64(locInt[1]);
        break;
    }

    free(locInt);

    switch (loc[0])
    {
    case 6:
        set_lf32(&fracVal, loc[1], (float)d);
        break;
    case 7:
        set_ld64(&frac64Val, loc[1], d);
        break;
    }

    free(loc);

    return 0;
}

size_t frac_int()
{
    size_t *loc = getVarLookUp(&varTable, getIndex_ls(&varBuf, get_ls(&argBuf, 0)));
    size_t *locFrac = getVarLookUp(&varTable, getIndex_ls(&varBuf, get_ls(&argBuf, 1)));

    int64_t i;
    switch (locFrac[0])
    {
    case 6:
        i = (int64_t)GET_FRAC(locFrac[1]);
        break;
    case 7:
        i = (int64_t)GET_FRAC64(locFrac[0]);
        break;
    }

    free(locFrac);

    switch (loc[0])
    {
    case 2:
        set_lui16(&natVal, loc[1], (uint16_t)i);
        break;
    case 3:
        set_lui64(&nat64Val, loc[1], (uint64_t)i);
        break;
    case 4:
        set_lsi32(&intVal, loc[1], (int32_t)i);
        break;
    case 5:
        set_lsi64(&int64Val, loc[1], i);
        break;
    }

    free(loc);

    return 0;
}

size_t type()
{
    size_t *loc = getVarLookUp(&varTable, getIndex_ls(&varBuf, get_ls(&argBuf, 0)));

    size_t *locVar = getVarLookUp(&varTable, getIndex_ls(&varBuf, get_ls(&argBuf, 1)));

    switch (locVar[0])
    {
    case 1:
        set_ls(&textVal, loc[1], "state");
        break;
    case 2:
        set_ls(&textVal, loc[1], "nat16");
        break;
    case 3:
        set_ls(&textVal, loc[1], "nat64");
        break;
    case 4:
        set_ls(&textVal, loc[1], "int32");
        break;
    case 5:
        set_ls(&textVal, loc[1], "int64");
        break;
    case 6:
        set_ls(&textVal, loc[1], "frac32");
        break;
    case 7:
        set_ls(&textVal, loc[1], "frac64");
        break;
    case 8:
        set_ls(&textVal, loc[1], "text");
        break;
    }

    free(loc);
    free(locVar);

    return 0;
}

size_t do_script()
{
    // printf("the function 'do_script' is deprecated, there is currently no
    // alternative\n");

    // I cheated, thanks Chad.
    DOSCRIPT
    return 0;
}

size_t timeout()
{
    size_t *loc = getVarLookUp(&varTable, getIndex_ls(&varBuf, get_ls(&argBuf, 0)));

    switch (loc[0])
    {
    case 2:
        SLEEP(GET_NAT(loc[1]));
        break;
    case 3:
        SLEEP(GET_NAT64(loc[1]));
        break;
    case 4:
        SLEEP(GET_INT(loc[1]));
        break;
    case 5:
        SLEEP(GET_INT64(loc[1]));
        break;
    }

    free(loc);

    return 0;
}
