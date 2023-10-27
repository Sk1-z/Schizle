#pragma once

#include <process.h>
#include <windows.h>

#include "../src/include/program.h"
// module get statement
#define GET_STD(name)                                      \
    push_ls(&varBuf, "_VERSION_");                         \
    pushTable(&varTable, 8, textVal.size);                 \
    push_ls(&textVal, "Schizle Script v0.4.0-alpha");      \
    pushBool_lui16(&(varMut), FALSE);                      \
    pushBool_lui16(&(varDef), TRUE);                       \
                                                           \
    push_ls(&(modules.moduleNames), name);                 \
    push_lui16(&(modules.moduleID), 1);                    \
                                                           \
    struct module ssl;                                     \
    init_lsig(&(ssl.functionSignatures));                  \
    init_ls(&(ssl.functionNames));                         \
                                                           \
    struct functionSig sout_sig;                           \
    init_sig(&sout_sig);                                   \
    sout_sig.numArgs = 1;                                  \
    push_lui16(&(sout_sig.argID), 0);                      \
    sout_sig.call = sout;                                  \
                                                           \
    push_sig(&(ssl.functionSignatures), sout_sig);         \
    push_ls(&(ssl.functionNames), "sout");                 \
                                                           \
    struct functionSig soutln_sig;                         \
    init_sig(&soutln_sig);                                 \
    soutln_sig.numArgs = 1;                                \
    push_lui16(&(soutln_sig.argID), 0);                    \
    soutln_sig.call = soutln;                              \
                                                           \
    push_sig(&(ssl.functionSignatures), soutln_sig);       \
    push_ls(&(ssl.functionNames), "soutln");               \
                                                           \
    struct functionSig sinln_sig;                          \
    init_sig(&sinln_sig);                                  \
    sinln_sig.numArgs = 1;                                 \
    push_lui16(&(sinln_sig.argID), 8);                     \
    sinln_sig.call = sinln;                                \
                                                           \
    push_sig(&(ssl.functionSignatures), sinln_sig);        \
    push_ls(&(ssl.functionNames), "sinln");                \
                                                           \
    struct functionSig text_to_frac_sig;                   \
    init_sig(&text_to_frac_sig);                           \
    text_to_frac_sig.numArgs = 2;                          \
    push_lui16(&(text_to_frac_sig.argID), 7);              \
    push_lui16(&(text_to_frac_sig.argID), 9);              \
    text_to_frac_sig.call = text_to_frac;                  \
                                                           \
    push_sig(&(ssl.functionSignatures), text_to_frac_sig); \
    push_ls(&(ssl.functionNames), "text_to_frac");         \
                                                           \
    struct functionSig text_to_int_sig;                    \
    init_sig(&text_to_int_sig);                            \
    text_to_int_sig.numArgs = 2;                           \
    push_lui16(&(text_to_int_sig.argID), 5);               \
    push_lui16(&(text_to_int_sig.argID), 9);               \
    text_to_int_sig.call = text_to_int;                    \
                                                           \
    push_sig(&(ssl.functionSignatures), text_to_int_sig);  \
    push_ls(&(ssl.functionNames), "text_to_int");          \
                                                           \
    struct functionSig type_sig;                           \
    init_sig(&type_sig);                                   \
    type_sig.numArgs = 2;                                  \
    push_lui16(&(type_sig.argID), 8);                      \
    push_lui16(&(type_sig.argID), 0);                      \
    type_sig.call = type;                                  \
                                                           \
    push_sig(&(ssl.functionSignatures), type_sig);         \
    push_ls(&(ssl.functionNames), "type");                 \
                                                           \
    struct functionSig do_script_sig;                      \
    init_sig(&do_script_sig);                              \
    do_script_sig.numArgs = 1;                             \
    push_lui16(&(do_script_sig.argID), 9);                 \
    do_script_sig.call = do_script;                        \
                                                           \
    push_sig(&(ssl.functionSignatures), do_script_sig);    \
    push_ls(&(ssl.functionNames), "do_script");            \
                                                           \
    struct functionSig timeout_sig;                        \
    init_sig(&timeout_sig);                                \
    timeout_sig.numArgs = 1;                               \
    push_lui16(&(timeout_sig.argID), 2);                   \
    timeout_sig.call = timeout;                            \
                                                           \
    push_sig(&(ssl.functionSignatures), timeout_sig);      \
    push_ls(&(ssl.functionNames), "timeout");              \
                                                           \
    push_module(&modules, &ssl);

// Standard out
size_t sout()
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
    size_t index = getIndex_ls(&varBuf, get_ls(&argBuf, 0));
    if (!index)
    {
        return 22;
    }
    else
    {
        set_ls(&textVal, getVarLookUp(&varTable, index)[1], in);
    }
    return 0;
}

size_t text_to_frac()
{
    size_t index = getIndex_ls(&varBuf, get_ls(&argBuf, 0));
    if (index)
    {
        size_t *loc = getVarLookUp(&varTable, index);

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
            size_t indexStr = getIndex_ls(&varBuf, get_ls(&argBuf, 1));

            if (indexStr)
            {
                size_t *locStr = getVarLookUp(&varTable, indexStr);

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
            else
            {
                return 22;
            }
        }

        free(loc);
        return 0;
    }
    else
    {
        return 22;
    }

    return 0;
}

size_t text_to_int()
{
    size_t index = getIndex_ls(&varBuf, get_ls(&argBuf, 0));

    if (index)
    {
        size_t *loc = getVarLookUp(&varTable, index);

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
                set_lsi32(&intVal, loc[1], (int)atoll(get_ls(&argBuf, 1)));
                break;
            case 5:
                set_lsi64(&int64Val, loc[1], atoll(get_ls(&argBuf, 1)));
                break;
            }
        }
        else
        {
            size_t indexStr = getIndex_ls(&varBuf, get_ls(&argBuf, 1));

            if (indexStr)
            {
                size_t *locStr = getVarLookUp(&varTable, indexStr);

                char *str = get_ls(&textVal, locStr[1]);

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

                free(locStr);
            }
            else
            {
                return 22;
            }
        }

        free(loc);
        return 0;
    }
    else
    {
        return 22;
    }

    return 0;
}

size_t type()
{
    size_t index = getIndex_ls(&varBuf, get_ls(&argBuf, 0));

    if (index)
    {
        size_t *loc = getVarLookUp(&varTable, index);

        size_t indexVar = getIndex_ls(&varBuf, get_ls(&argBuf, 1));
        size_t *locVar = getVarLookUp(&varTable, indexVar);

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
    }
    else
    {
        return 22;
    }

    return 0;
}

size_t do_script()
{
    // printf("the function 'do_script' is deprecated, there is currently no alternative\n");

    // I cheated
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    char command[CHARACTER_LIMIT];

    // CreateProcess(NULL, command, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);

    if (get_lui16(&isStringArg, 0))
    {
        sprintf(command, "schizle %s", get_ls(&argBuf, 0));
        CreateProcess(NULL, command, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);

        // const char *args[] = {"schizle", get_ls(&argBuf, 0), NULL};
        // _execv("schizle", args);
    }
    else
    {
        size_t index = getIndex_ls(&varBuf, get_ls(&argBuf, 0));

        if (index)
        {
            size_t *loc = getVarLookUp(&varTable, index);

            sprintf(command, "schizle %s", get_ls(&textVal, loc[1]));
            CreateProcess(NULL, command, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);

            // const char *argv[] = {"schizle", get_ls(&textVal, loc[1]), NULL};
            // _execv("schizle", argv);

            free(loc);
        }
        else
        {
            return 22;
        }
    }

    WaitForSingleObject(pi.hProcess, INFINITE);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}

size_t timeout()
{
    size_t index = getIndex_ls(&varBuf, get_ls(&argBuf, 0));

    if (index)
    {
        size_t *loc = getVarLookUp(&varTable, index);

        switch (loc[0])
        {
        case 2:
            Sleep(GET_NAT(loc[1]));
            break;
        case 3:
            Sleep(GET_NAT64(loc[1]));
            break;
        case 4:
            Sleep(GET_INT(loc[1]));
            break;
        case 5:
            Sleep(GET_INT64(loc[1]));
            break;
        }

        free(loc);
    }
    else
    {
        return 22;
    }

    return 0;
}
