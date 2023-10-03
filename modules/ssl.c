#include "common.h"

// module get statement
#define GET_SSL(name)                                 \
    push_ls(&(modules.moduleNames), name);            \
    push_lui16(&(modules.moduleID), 1);               \
                                                      \
    struct module ssl;                                \
    init_lsig(&(ssl.functionSignatures));             \
    init_ls(&(ssl.functionNames));                    \
                                                      \
    struct functionSig cli_out_sig;                   \
    init_sig(&cli_out_sig);                           \
    cli_out_sig.numArgs = 1;                          \
    push_lui16(&(cli_out_sig.argID), 0);              \
    cli_out_sig.call = cli_out;                       \
    push_sig(&(ssl.functionSignatures), cli_out_sig); \
    push_ls(&(ssl.functionNames), "cli_out");         \
                                                      \
    push_module(&modules, &ssl);

// prints to the command line
#define CLI_OUT(loc)                       \
    switch (loc[0])                        \
    {                                      \
    case 0:                                \
        printf(get_ls(&argBuf, 0));        \
    case 1:                                \
        if (GET_STATE(loc[1]) == 1)        \
        {                                  \
            printf("on");                  \
        }                                  \
        else if (GET_STATE(loc[1]) == 0)   \
        {                                  \
            printf("off");                 \
        }                                  \
        break;                             \
    case 2:                                \
        printf("%hu", GET_NAT(loc[1]));    \
        break;                             \
    case 3:                                \
        printf("%llu", GET_NAT64(loc[1])); \
        break;                             \
    case 4:                                \
        printf("%d", GET_INT(loc[1]));     \
        break;                             \
    case 5:                                \
        printf("%lld", GET_INT64(loc[1])); \
        break;                             \
    case 6:                                \
        printf("%f", GET_FRAC(loc[1]));    \
        break;                             \
    case 7:                                \
        printf("%lf", GET_FRAC64(loc[1])); \
        break;                             \
    case 8:                                \
        printf(GET_TEXT(loc[1]));          \
        break;                             \
    }

size_t cli_out()
{
    int warning = 0;
    if (get_lui16(&isStringArg, 0))
    {
        printf("string");
        size_t loc[2] = {0, 0};
        CLI_OUT(loc)
    }
    else
    {
        size_t index = getIndex_ls(&varBuf, get_ls(&argBuf, 0));
        if (!index)
        {
            return ERROR;
        }
        else
        {
            if (!get_lui16(&varDef, index))
            {
                warning = WARNING;
            }
            size_t *loc = getVarLookUp(&varTable, index);
            CLI_OUT(loc)
            free(loc);
        }
    }

    if (warning)
    {
        return warning;
    }
    else
    {
        return 0;
    }
}