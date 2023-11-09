#pragma once

#include "../src/include/program.h"

#define GET_MATH(name)                                                                                                 \
    push_ls(&varBuf, "_PI_");                                                                                          \
    pushTable(&varTable, 7, frac64Val.size);                                                                           \
    push_ld64(&frac64Val, M_PI);                                                                                       \
    pushBool_lui16(&(varMut), FALSE);                                                                                  \
    pushBool_lui16(&(varDef), TRUE);                                                                                   \
                                                                                                                       \
    push_ls(&varBuf, "_E_");                                                                                           \
    pushTable(&varTable, 7, frac64Val.size);                                                                           \
    push_ld64(&frac64Val, M_E);                                                                                        \
    pushBool_lui16(&(varMut), FALSE);                                                                                  \
    pushBool_lui16(&(varDef), TRUE);                                                                                   \
                                                                                                                       \
    push_ls(&varBuf, "_R2_");                                                                                          \
    pushTable(&varTable, 7, frac64Val.size);                                                                           \
    push_ld64(&frac64Val, M_SQRT2);                                                                                    \
    pushBool_lui16(&(varMut), FALSE);                                                                                  \
    pushBool_lui16(&(varDef), TRUE);                                                                                   \
                                                                                                                       \
    push_ls(&varBuf, "_LN2_");                                                                                         \
    pushTable(&varTable, 7, frac64Val.size);                                                                           \
    push_ld64(&frac64Val, M_LN2);                                                                                      \
    pushBool_lui16(&(varMut), FALSE);                                                                                  \
    pushBool_lui16(&(varDef), TRUE);                                                                                   \
                                                                                                                       \
    push_ls(&varBuf, "_GR_");                                                                                          \
    pushTable(&varTable, 7, frac64Val.size);                                                                           \
    push_ld64(&frac64Val, (1 + sqrt(5.0)) / 2);                                                                        \
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
    struct functionSig floor_sig;                                                                                      \
    init_sig(&floor_sig);                                                                                              \
    floor_sig.numArgs = 1;                                                                                             \
    push_lui16(&(floor_sig.argID), 7);                                                                                 \
    floor_sig.call = _floor;                                                                                           \
                                                                                                                       \
    push_sig(&(std.functionSignatures), &floor_sig);                                                                   \
    push_ls(&(std.functionNames), "floor");                                                                            \
                                                                                                                       \
    struct functionSig ceil_sig;                                                                                       \
    init_sig(&ceil_sig);                                                                                               \
    ceil_sig.numArgs = 1;                                                                                              \
    push_lui16(&(ceil_sig.argID), 7);                                                                                  \
    ceil_sig.call = _ceil;                                                                                             \
                                                                                                                       \
    push_sig(&(std.functionSignatures), &ceil_sig);                                                                    \
    push_ls(&(std.functionNames), "ceil");                                                                             \
                                                                                                                       \
    struct functionSig ln_sig;                                                                                         \
    init_sig(&ln_sig);                                                                                                 \
    ln_sig.numArgs = 1;                                                                                                \
    push_lui16(&(ln_sig.argID), 7);                                                                                    \
    ln_sig.call = ln;                                                                                                  \
                                                                                                                       \
    push_sig(&(std.functionSignatures), &ln_sig);                                                                      \
    push_ls(&(std.functionNames), "ln");                                                                               \
                                                                                                                       \
    struct functionSig log10_sig;                                                                                      \
    init_sig(&log10_sig);                                                                                              \
    log10_sig.numArgs = 1;                                                                                             \
    push_lui16(&(log10_sig.argID), 7);                                                                                 \
    log10_sig.call = _log10;                                                                                           \
                                                                                                                       \
    push_sig(&(std.functionSignatures), &log10_sig);                                                                   \
    push_ls(&(std.functionNames), "log10");                                                                            \
                                                                                                                       \
    struct functionSig sin_sig;                                                                                        \
    init_sig(&sin_sig);                                                                                                \
    sin_sig.numArgs = 1;                                                                                               \
    push_lui16(&(sin_sig.argID), 7);                                                                                   \
    sin_sig.call = _sin;                                                                                               \
                                                                                                                       \
    push_sig(&(std.functionSignatures), &sin_sig);                                                                     \
    push_ls(&(std.functionNames), "sin");                                                                              \
                                                                                                                       \
    struct functionSig cos_sig;                                                                                        \
    init_sig(&cos_sig);                                                                                                \
    cos_sig.numArgs = 1;                                                                                               \
    push_lui16(&(cos_sig.argID), 7);                                                                                   \
    cos_sig.call = _cos;                                                                                               \
                                                                                                                       \
    push_sig(&(std.functionSignatures), &cos_sig);                                                                     \
    push_ls(&(std.functionNames), "cos");                                                                              \
                                                                                                                       \
    modules[modCount] = std;                                                                                           \
    modCount++;

size_t _floor()
{
    size_t *loc = getVarLookUp(&varTable, getIndex_ls(&varBuf, get_ls(&argBuf, 0)));
    switch (loc[0])
    {
    case 6:
        set_lf32(&fracVal, loc[1], floor(GET_FRAC(loc[1])));
        break;
    case 7:
        set_ld64(&frac64Val, loc[1], floor(GET_FRAC64(loc[1])));
        break;
    }

    free(loc);

    return 0;
}

size_t _ceil()
{
    size_t *loc = getVarLookUp(&varTable, getIndex_ls(&varBuf, get_ls(&argBuf, 0)));
    switch (loc[0])
    {
    case 6:
        set_lf32(&fracVal, loc[1], ceil(GET_FRAC(loc[1])));
        break;
    case 7:
        set_ld64(&frac64Val, loc[1], ceil(GET_FRAC64(loc[1])));
        break;
    }

    free(loc);

    return 0;
}

size_t ln()
{
    size_t *locArg = getVarLookUp(&varTable, getIndex_ls(&varBuf, get_ls(&argBuf, 1)));

    double d;
    switch (locArg[0])
    {
    case 6:
        d = GET_FRAC(locArg[1]);
        break;
    case 7:
        d = GET_FRAC64(locArg[1]);
        break;
    }

    free(locArg);

    size_t *loc = getVarLookUp(&varTable, getIndex_ls(&varBuf, get_ls(&argBuf, 0)));
    switch (loc[0])
    {
    case 6:
        set_lf32(&fracVal, loc[1], log(d));
        break;
    case 7:
        set_ld64(&frac64Val, loc[1], log(d));
        break;
    }

    free(loc);

    return 0;
}

size_t _log10()
{
    size_t *locArg = getVarLookUp(&varTable, getIndex_ls(&varBuf, get_ls(&argBuf, 1)));

    double d;
    switch (locArg[0])
    {
    case 6:
        d = GET_FRAC(locArg[1]);
        break;
    case 7:
        d = GET_FRAC64(locArg[1]);
        break;
    }

    free(locArg);

    size_t *loc = getVarLookUp(&varTable, getIndex_ls(&varBuf, get_ls(&argBuf, 0)));
    switch (loc[0])
    {
    case 6:
        set_lf32(&fracVal, loc[1], log10(d));
        break;
    case 7:
        set_ld64(&frac64Val, loc[1], log10(d));
        break;
    }

    free(loc);

    return 0;
}

size_t _sin()
{
    size_t *locArg = getVarLookUp(&varTable, getIndex_ls(&varBuf, get_ls(&argBuf, 1)));

    double d;
    switch (locArg[0])
    {
    case 6:
        d = GET_FRAC(locArg[1]);
        break;
    case 7:
        d = GET_FRAC64(locArg[1]);
        break;
    }

    free(locArg);

    size_t *loc = getVarLookUp(&varTable, getIndex_ls(&varBuf, get_ls(&argBuf, 0)));
    switch (loc[0])
    {
    case 6:
        set_lf32(&fracVal, loc[1], sin(d));
        break;
    case 7:
        set_ld64(&frac64Val, loc[1], sin(d));
        break;
    }

    free(loc);

    return 0;
}

size_t _cos()
{
    size_t *locArg = getVarLookUp(&varTable, getIndex_ls(&varBuf, get_ls(&argBuf, 1)));

    double d;
    switch (locArg[0])
    {
    case 6:
        d = GET_FRAC(locArg[1]);
        break;
    case 7:
        d = GET_FRAC64(locArg[1]);
        break;
    }

    free(locArg);

    size_t *loc = getVarLookUp(&varTable, getIndex_ls(&varBuf, get_ls(&argBuf, 0)));
    switch (loc[0])
    {
    case 6:
        set_lf32(&fracVal, loc[1], cos(d));
        break;
    case 7:
        set_ld64(&frac64Val, loc[1], cos(d));
        break;
    }

    free(loc);

    return 0;
}