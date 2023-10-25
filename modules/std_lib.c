#pragma once

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
                                                      \
    push_sig(&(ssl.functionSignatures), cli_out_sig); \
    push_ls(&(ssl.functionNames), "cli_out");         \
                                                      \
    push_module(&modules, &ssl);

// Standard out
size_t cli_out()
{
    if (get_lui16(&isStringArg, 0))
    {
        size_t loc[2] = {0, 0};
        printf(get_ls(&argBuf, 0));
    }
    else
    {
        size_t index = getIndex_ls(&varBuf, get_ls(&argBuf, 0));
        if (!index)
        {
            return 22;
        }
        else
        {
            size_t *loc = getVarLookUp(&varTable, index);
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
    }

    return 0;
}