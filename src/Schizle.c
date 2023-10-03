#ifdef DEBUG
#define PRINTTYPEDB(str) printf("\n%s\n", str)
#define PRINT_POINT                   \
    printf("\nPoint %d\n", pointNum); \
    pointNum++;
#define _CRT_SECURE_NO_WARNINGS
#else
#define PRINTTYPEDB(str)
#define PRINT_POINT
#endif

#include <stdio.h>
#include <string.h>

#include "include/tok.h"
#include "include/expressions.h"
#include "include/program.h"

#include "../modules/import.h"

int main(int argc, char *argv[])
{
    // Command line parsing
    if (argc == 1)
    {
        THROW_ERROR(2);
    }
    else if (argc == 2)
    {
        THROW_ERROR(4);
    }
    else if (argc == 3 || argc == 4)
    {
        // File entry point
        if (!strcmp(argv[1], "run"))
        {
            FILE *ski = fopen(argv[2], "r");
            if (ski != NULL)
            {
                program = 1;
                // pre processing
                if (argc == 4)
                {
                    printf("%s out:\n\n", argv[3]);
                    INIT_PROGRAM(argv[3])
                }
                else
                {
                    printf("%s out:\n\n", argv[2]);
                    INIT_PROGRAM(argv[2])
                }

                // parse file
                while (get)
                {
                    if (fgets(lineBuf, sizeof(lineBuf), ski) == NULL)
                    {
                        printf("oopshie empty");
                        get = 0;
                        goto exit;
                    }

                    fline.ftoken = tokenize(lineBuf);
                    pushOffset(&(fline.offsets), ftell(ski));
                    fline.num++;

                    if (!strcmp(get_ls(fline.ftoken->toks, 0), "get"))
                    {
                        if (getLineSize(&fline) < 2)
                        {
                            ERROR_HERE
                        }
                        else
                        {
                            for (int i = 0; i < strlen(get_ls(fline.ftoken->toks, 1)); i++)
                            {
                                if (get_ls(fline.ftoken->toks, 1)[i] == ' ')
                                {
                                    fline.ftoken->toks->data[1][i] = '_';
                                }
                            }

                            if (!strcmp(get_ls(fline.ftoken->toks, 1), "schizle_standard_library") || !strcmp(get_ls(fline.ftoken->toks, 1), "ssl") || !strcmp(get_ls(fline.ftoken->toks, 1), "std"))
                            {
                                CHECK_MODULE(1)

                                if (getLineSize(&fline) < 4)
                                {
                                    GET_SSL(get_ls(fline.ftoken->toks, 1))
                                }
                                else
                                {
                                    if (strcmp(get_ls(fline.ftoken->toks, 2), "as"))
                                    {
                                        ERROR_HERE
                                    }
                                    GET_SSL(get_ls(fline.ftoken->toks, 3))
                                }
                                PRINTTYPEDB("got ssl");
                            }
                            else
                            {
                                ERROR_HERE
                            }
                        }
                    }
                    else if (!strcmp(get_ls(fline.ftoken->toks, 0), "\n"))
                    {
                        continue;
                    }
                    else if (!strcmp(get_ls(fline.ftoken->toks, 0), "end"))
                    {
                        get = 0;
                    }
                    else
                    {
                        get = 0;
                        goto get_skip;
                    }
                }

                while (intepreterState)
                {
                    if (fgets(lineBuf, sizeof(lineBuf), ski) == NULL)
                    {
                        intepreterState = 0;
                        continue;
                    }

                    fline.ftoken = tokenize(lineBuf);
                    if (!loopExp)
                    {
                        pushOffset(&(fline.offsets), ftell(ski));
                    }

                    fline.num++;
                get_skip:

                    if (!strcmp(get_ls(fline.ftoken->toks, 0), "end"))
                    {
                        if (expState)
                        {
                            expState--;
                        }
                        else
                        {
                            WARNING_HERE
                        }
                    }
                    else if (!strcmp(get_ls(fline.ftoken->toks, 0), "\n"))
                    {
                    }
                    else if (!strcmp(get_ls(fline.ftoken->toks, 0), "KW_MUT") || !strcmp(get_ls(fline.ftoken->toks, 0), "KW_IMMUT"))
                    {

                        PRINTTYPEDB("declaration");

                        if (getLineSize(&fline) == 6 || getLineSize(&fline) == 7)
                        {
                            if (autoType(1))
                            {
                                ERROR_HERE
                            }

                            if (!strcmp(get_ls(fline.ftoken->toks, 4), "KW_EQ"))
                            {
                                if (!strcmp(get_ls(fline.ftoken->toks, 5), "KW_EVAL_BLOCK"))
                                {
                                    if (getLineSize(&fline) != 7)
                                    {
                                        ERROR_HERE
                                    }

                                    size_t evalCode;

                                    size_t type = stringToType(get_ls(fline.ftoken->toks, 3));

                                    char *c;
                                    char fmt[310];
                                    switch (type)
                                    {
                                    case 1:
                                        ERROR_HERE
                                        break;
                                    case 2:
                                        sprintf(fmt, "%hu", (uint16_t)eval_ll(&evalCode, get_ls(fline.ftoken->toks, 6)));
                                        NEW_VALUE(type, get_ls(fline.ftoken->toks, 1), fmt, c);
                                        break;
                                    case 3:
                                        sprintf(fmt, "%llu", (uint64_t)eval_ll(&evalCode, get_ls(fline.ftoken->toks, 6)));
                                        NEW_VALUE(type, get_ls(fline.ftoken->toks, 1), fmt, c);
                                        break;
                                    case 4:
                                        sprintf(fmt, "%d", (int32_t)eval_ll(&evalCode, get_ls(fline.ftoken->toks, 6)));
                                        NEW_VALUE(type, get_ls(fline.ftoken->toks, 1), fmt, c);
                                        break;
                                    case 5:
                                        sprintf(fmt, "%lld", (int64_t)eval_ll(&evalCode, get_ls(fline.ftoken->toks, 6)));
                                        NEW_VALUE(type, get_ls(fline.ftoken->toks, 1), fmt, c);
                                        break;
                                    case 6:
                                        sprintf(fmt, "%f", (float)eval_dbl(&evalCode, get_ls(fline.ftoken->toks, 6)));
                                        NEW_VALUE(type, get_ls(fline.ftoken->toks, 1), fmt, c);
                                        break;
                                    case 7:
                                        sprintf(fmt, "%lf", (double)eval_dbl(&evalCode, get_ls(fline.ftoken->toks, 6)));
                                        NEW_VALUE(type, get_ls(fline.ftoken->toks, 1), fmt, c);
                                        break;
                                    case 8:
                                        ERROR_HERE
                                        break;
                                    }

                                    if (evalCode)
                                    {
                                        // printf("t: %zu ", type);
                                        // printf("c: %zu expr: ", evalCode);
                                        // printf(get_ls(fline.ftoken->toks, 6));
                                        ERROR_HERE;
                                    }

                                    if (!strcmp(get_ls(fline.ftoken->toks, 0), "KW_MUT"))
                                    {
                                        pushBool_lui16(&varMut, TRUE);
                                    }
                                    else
                                    {
                                        pushBool_lui16(&varMut, FALSE);
                                    }
                                    pushBool_lui16(&varDef, TRUE);
                                }
                                else
                                {
                                    if (getLineSize(&fline) != 6)
                                    {
                                        ERROR_HERE
                                    }

                                    size_t index = getIndex_ls(&varBuf, get_ls(fline.ftoken->toks, 1));

                                    if (index)
                                    {
                                        ERROR_HERE
                                    }
                                    else
                                    {
                                        size_t foundType = autoType(5);
                                        if (foundType)
                                        {
                                            size_t declaredType = stringToType(get_ls(fline.ftoken->toks, 3));

                                            char *c;
                                            switch (foundType)
                                            {
                                            case 1:
                                                if (declaredType == foundType)
                                                {
                                                    NEW_VALUE(declaredType, get_ls(fline.ftoken->toks, 1), get_ls(fline.ftoken->toks, 5), c)
                                                }
                                                else
                                                {
                                                    ERROR_HERE
                                                }
                                                break;
                                            case 6:
                                            case 7:
                                                if (declaredType == foundType)
                                                {
                                                    NEW_VALUE(declaredType, get_ls(fline.ftoken->toks, 1), get_ls(fline.ftoken->toks, 5), c)
                                                }
                                                else
                                                {
                                                    ERROR_HERE
                                                }
                                                break;
                                            case 8:
                                                if (declaredType == foundType)
                                                {
                                                    NEW_VALUE(declaredType, get_ls(fline.ftoken->toks, 1), get_ls(fline.ftoken->toks, 5), c)
                                                }
                                                else
                                                {
                                                    ERROR_HERE
                                                }
                                                break;
                                            default:
                                                if (declaredType < 6 && declaredType > 1)
                                                {
                                                    NEW_VALUE(declaredType, get_ls(fline.ftoken->toks, 1), get_ls(fline.ftoken->toks, 5), c)
                                                }
                                                else
                                                {
                                                    ERROR_HERE
                                                }
                                                break;
                                            }
                                        }
                                        else
                                        {
                                            size_t index = getIndex_ls(&varBuf, get_ls(fline.ftoken->toks, 3));

                                            if (index)
                                            {
                                                size_t *loc = getVarLookUp(&varTable, index);
                                                NEW_VALUE_FROM_OLD(loc, get_ls(fline.ftoken->toks, 1));
                                                free(loc);
                                            }
                                            else
                                            {
                                                ERROR_HERE
                                            }
                                        }

                                        if (!strcmp(get_ls(fline.ftoken->toks, 0), "KW_MUT"))
                                        {
                                            pushBool_lui16(&varMut, TRUE);
                                        }
                                        else
                                        {
                                            pushBool_lui16(&varMut, FALSE);
                                        }
                                        pushBool_lui16(&varDef, TRUE);
                                    }
                                }
                            }
                        }
                        else if (getLineSize(&fline) == 4)
                        {
                            if (autoType(1))
                            {
                                ERROR_HERE
                            }

                            if (!strcmp(get_ls(fline.ftoken->toks, 2), "KW_EQ"))
                            {
                                size_t index = getIndex_ls(&varBuf, get_ls(fline.ftoken->toks, 1));

                                if (index)
                                {
                                    ERROR_HERE
                                }
                                else
                                {
                                    size_t t = autoType(3);
                                    if (t)
                                    {
                                        char *c;
                                        NEW_VALUE(t, get_ls(fline.ftoken->toks, 1), get_ls(fline.ftoken->toks, 3), c)
                                    }
                                    else
                                    {
                                        size_t index = getIndex_ls(&varBuf, get_ls(fline.ftoken->toks, 3));

                                        if (index)
                                        {
                                            size_t *loc = getVarLookUp(&varTable, index);
                                            NEW_VALUE_FROM_OLD(loc, get_ls(fline.ftoken->toks, 1));
                                            free(loc);
                                        }
                                        else
                                        {
                                            ERROR_HERE
                                        }
                                    }
                                }

                                if (!strcmp(get_ls(fline.ftoken->toks, 0), "KW_MUT"))
                                {
                                    pushBool_lui16(&varMut, TRUE);
                                }
                                else
                                {
                                    pushBool_lui16(&varMut, FALSE);
                                }
                                pushBool_lui16(&varDef, TRUE);
                            }
                            else if (!strcmp(get_ls(fline.ftoken->toks, 2), "KW_TYPE"))
                            {
                                if (getLineSize(&fline) == 5)
                                {
                                    ERROR_HERE
                                }

                                if (!strcmp(get_ls(fline.ftoken->toks, 0), "KW_IMMUT"))
                                {
                                    ERROR_HERE
                                }

                                char *c;
                                size_t t = stringToType(get_ls(fline.ftoken->toks, 3));

                                switch (t)
                                {
                                case 0:
                                    ERROR_HERE
                                    break;
                                case 1:
                                    NEW_VALUE(t, get_ls(fline.ftoken->toks, 1), "off", c)
                                    break;
                                case 8:
                                    NEW_VALUE(t, get_ls(fline.ftoken->toks, 1), "0", c)
                                    break;
                                default:
                                    NEW_VALUE(t, get_ls(fline.ftoken->toks, 1), "0", c)
                                    break;
                                }

                                pushBool_lui16(&varMut, TRUE);
                                pushBool_lui16(&varDef, FALSE);
                            }
                            else
                            {
                                ERROR_HERE
                            }
                        }
                        else
                        {
                            ERROR_HERE
                        }
                    }
                    else if (!strcmp(get_ls(fline.ftoken->toks, 0), "if"))
                    {
                        PRINTTYPEDB("if");
                    }
                    else if (!strcmp(get_ls(fline.ftoken->toks, 0), "elseif"))
                    {
                        PRINTTYPEDB("elseif");
                    }
                    else if (!strcmp(get_ls(fline.ftoken->toks, 0), "else"))
                    {
                        PRINTTYPEDB("else");
                    }
                    else if (!strcmp(get_ls(fline.ftoken->toks, 0), "dur"))
                    {
                        PRINTTYPEDB("loop");
                    }
                    else if (!strcmp(get_ls(fline.ftoken->toks, 0), "KW_PARAMS"))
                    {
                        if (paramInit)
                        {
                            WARNING_HERE
                        }

                        if (getLineSize(&fline) < 3)
                        {
                            ERROR_HERE
                        }

                        PRINTTYPEDB("parameter declaration");

                        clear_ls(&argBuf);
                        clear_lui16(&isStringArg);
                        clear_lui16(&typeArg);

                        size_t i = 1;
                        while (strcmp(get_ls(fline.ftoken->toks, i), "end"))
                        {
                            if (strcmp(get_ls(fline.ftoken->toks, i), "\n"))
                            {
                                push_ls(&argBuf, get_ls(fline.ftoken->toks, i));

                                if (get_lui16(&(fline.ftoken->isStringL), i))
                                {
                                    pushBool_lui16(&isStringArg, TRUE);
                                    push_lui16(&typeArg, 8);
                                }
                                else
                                {
                                    pushBool_lui16(&isStringArg, FALSE);

                                    size_t Index = getIndex_ls(&varBuf, get_ls(fline.ftoken->toks, i));

                                    if (Index)
                                    {
                                        size_t *loc = getVarLookUp(&varTable, Index);
                                        push_lui16(&typeArg, loc[0]);
                                        free(loc);
                                    }
                                    else
                                    {
                                        ERROR_HERE
                                    }
                                }
                            }

                            i++;
                            if (i == getLineSize(&fline))
                            {
                                ERROR_HERE
                            }
                        }

                        paramInit = 1;
                    }
                    else if (!strcmp(get_ls(fline.ftoken->toks, 0), "call"))
                    {
                        PRINTTYPEDB("function call");

                        if (getLineSize(&fline) < 2)
                        {
                            ERROR_HERE
                        }

                        size_t modIndex;
                        if (!strcmp(get_ls(fline.ftoken->toks, 1), "KW_MOD"))
                        {
                            if (getLineSize(&fline) < 5)
                            {
                                ERROR_HERE
                            }

                            if (strcmp(get_ls(fline.ftoken->toks, 3), "KW_MEMBER"))
                            {
                                ERROR_HERE
                            }

                            modIndex = getIndex_ls(&(modules.moduleNames), get_ls(fline.ftoken->toks, 2));

                            if (modIndex)
                            {
                                struct module *module = get_module(&modules, modIndex);

                                size_t funcI = getIndex_ls(&(module->functionNames), get_ls(fline.ftoken->toks, 4));
                                if (funcI)
                                {
                                }
                                else
                                {
                                    if (strcmp(get_ls(fline.ftoken->toks, 4), get_ls(&(module->functionNames), 0)))
                                    {
                                        struct functionSig sig = get_sig(&(module->functionSignatures), 0);

                                        if (getSize_ls(&argBuf) != sig.numArgs)
                                        {
                                            ERROR_HERE
                                        }

                                        for (size_t i = 0; i < sig.argID.size; i++)
                                        {
                                            if (get_lui16(&typeArg, i) != get_lui16(&(sig.argID), i) && get_lui16(&(sig.argID), i) != 0)
                                            {
                                                ERROR_HERE
                                            }
                                        }

                                        sig.call();
                                    }
                                    else
                                    {
                                    }
                                }
                            }
                            else
                            {
                            }
                        }
                        else
                        {
                        }

                        paramInit = 0;
                    }
                    else
                    {
                        int index = getIndex_ls(&varBuf, get_ls(fline.ftoken->toks, 0));
                        if (index)
                        {
                            PRINTTYPEDB("redeclaration");

                            if (!strcmp(get_ls(fline.ftoken->toks, 1), "KW_EQ"))
                            {
                                if (get_lui16(&varMut, index))
                                {
                                    size_t *loc = getVarLookUp(&varTable, index);

                                    if (!strcmp(get_ls(fline.ftoken->toks, 2), "KW_EVAL_BLOCK"))
                                    {
                                        if (getLineSize(&fline) == 4)
                                        {
                                            size_t evalCode;

                                            switch (loc[0])
                                            {
                                            case 1:
                                                ERROR_HERE
                                                break;
                                            case 2:
                                                set_lui16(&natVal, loc[1], (uint16_t)eval_ll(&evalCode, get_ls(fline.ftoken->toks, 3)));
                                                break;
                                            case 3:
                                                set_lui64(&nat64Val, loc[1], (uint64_t)eval_ll(&evalCode, get_ls(fline.ftoken->toks, 3)));
                                                break;
                                            case 4:
                                                set_lsi32(&intVal, loc[1], (int32_t)eval_ll(&evalCode, get_ls(fline.ftoken->toks, 3)));
                                                break;
                                            case 5:
                                                set_lsi64(&int64Val, loc[1], (int64_t)eval_ll(&evalCode, get_ls(fline.ftoken->toks, 3)));
                                                break;
                                            case 6:
                                                set_lf32(&fracVal, loc[1], (float)eval_dbl(&evalCode, get_ls(fline.ftoken->toks, 3)));
                                                break;
                                            case 7:
                                                set_ld64(&frac64Val, loc[1], (double)eval_dbl(&evalCode, get_ls(fline.ftoken->toks, 3)));
                                                break;
                                            case 8:
                                                ERROR_HERE
                                                break;
                                            }

                                            if (evalCode)
                                            {
                                                // printf("t: %zu ", type);
                                                // printf("c: %zu expr: ", evalCode);
                                                // printf(get_ls(fline.ftoken->toks, 6));
                                                ERROR_HERE;
                                            }
                                        }
                                        else
                                        {
                                            ERROR_HERE
                                        }
                                    }
                                    else if (getLineSize(&fline) == 3)
                                    {
                                        size_t t = autoType(2);
                                        char *endChar;

                                        if (t > 1 && t < 8 && getLineSize(&fline) < 3)
                                        {
                                            unsigned long long ull = strtoull(get_ls(fline.ftoken->toks, 2), &endChar, 10);
                                            switch (*endChar)
                                            {
                                            case '-':
                                                long long ll = strtoll(get_ls(fline.ftoken->toks, 2), &endChar, 10);
                                                if (*endChar != '\0')
                                                {
                                                    ERROR_HERE
                                                }
                                                break;
                                            case '.':
                                                float dbl = strtof(get_ls(fline.ftoken->toks, 2), &endChar);
                                                if (*endChar != '\0')
                                                {
                                                    ERROR_HERE
                                                }
                                                break;
                                            default:
                                                if (*endChar != '\0')
                                                {
                                                    ERROR_HERE
                                                }
                                            }
                                        }

                                        switch (t)
                                        {
                                        case 0:
                                            size_t index = getIndex_ls(&varBuf, get_ls(fline.ftoken->toks, 2));

                                            if (index)
                                            {
                                                size_t *locSecondVar = getVarLookUp(&varTable, index);
                                                switch (locSecondVar[0])
                                                {
                                                case 1:
                                                    if (loc[0] == 1)
                                                    {
                                                        if (get_lui16(&stateVal, locSecondVar[1]))
                                                        {
                                                            set_lui16(&stateVal, loc[1], TRUE);
                                                        }
                                                        else
                                                        {
                                                            set_lui16(&stateVal, loc[1], FALSE);
                                                        }
                                                    }
                                                    else
                                                    {
                                                        ERROR_HERE
                                                    }
                                                    break;
                                                case 2:
                                                    if (loc[0] > 1 && loc[0] < 6)
                                                    {
                                                        switch (loc[0])
                                                        {
                                                        case 2:
                                                            set_lui16(&natVal, loc[1], get_lui16(&natVal, locSecondVar[1]));
                                                            break;
                                                        case 3:
                                                            set_lui64(&nat64Val, loc[1], (uint64_t)get_lui16(&natVal, locSecondVar[1]));
                                                            break;
                                                        case 4:
                                                            set_lsi32(&intVal, loc[1], (int32_t)get_lui16(&natVal, locSecondVar[1]));
                                                            break;
                                                        case 5:
                                                            set_lsi64(&int64Val, loc[1], (int64_t)get_lui16(&natVal, locSecondVar[1]));
                                                            break;
                                                        }
                                                    }
                                                    else
                                                    {
                                                        ERROR_HERE
                                                    }
                                                    break;
                                                case 3:
                                                    if (loc[0] > 1 && loc[0] < 6)
                                                    {
                                                        switch (loc[0])
                                                        {
                                                        case 2:
                                                            set_lui16(&natVal, loc[1], (uint16_t)get_lui64(&nat64Val, locSecondVar[1]));
                                                            break;
                                                        case 3:
                                                            set_lui64(&nat64Val, loc[1], get_lui64(&nat64Val, locSecondVar[1]));
                                                            break;
                                                        case 4:
                                                            set_lsi32(&intVal, loc[1], (int32_t)get_lui64(&nat64Val, locSecondVar[1]));
                                                            break;
                                                        case 5:
                                                            set_lsi64(&int64Val, loc[1], (int64_t)get_lui64(&nat64Val, locSecondVar[1]));
                                                            break;
                                                        }
                                                    }
                                                    else
                                                    {
                                                        ERROR_HERE
                                                    }
                                                    break;
                                                case 4:
                                                    if (loc[0] > 1 && loc[0] < 6)
                                                    {
                                                        switch (loc[0])
                                                        {
                                                        case 2:
                                                            set_lui16(&natVal, loc[1], (uint16_t)get_lsi32(&intVal, locSecondVar[1]));
                                                            break;
                                                        case 3:
                                                            set_lui64(&nat64Val, loc[1], (uint64_t)get_lsi32(&intVal, locSecondVar[1]));
                                                            break;
                                                        case 4:
                                                            set_lsi32(&intVal, loc[1], get_lsi32(&intVal, locSecondVar[1]));
                                                            break;
                                                        case 5:
                                                            set_lsi64(&int64Val, loc[1], (int64_t)get_lsi32(&intVal, locSecondVar[1]));
                                                            break;
                                                        }
                                                    }
                                                    else
                                                    {
                                                        ERROR_HERE
                                                    }
                                                    break;
                                                case 5:
                                                    if (loc[0] > 1 && loc[0] < 6)
                                                    {
                                                        switch (loc[0])
                                                        {
                                                        case 2:
                                                            set_lui16(&natVal, loc[1], (uint16_t)get_lsi64(&int64Val, locSecondVar[1]));
                                                            break;
                                                        case 3:
                                                            set_lui64(&nat64Val, loc[1], (uint64_t)get_lsi64(&int64Val, locSecondVar[1]));
                                                            break;
                                                        case 4:
                                                            set_lsi32(&intVal, loc[1], (int32_t)get_lsi64(&int64Val, locSecondVar[1]));
                                                            break;
                                                        case 5:
                                                            set_lsi64(&int64Val, loc[1], get_lsi64(&int64Val, locSecondVar[1]));
                                                            break;
                                                        }
                                                    }
                                                    else
                                                    {
                                                        ERROR_HERE
                                                    }
                                                    break;
                                                case 6:
                                                    if (loc[0] > 5 && loc[0] < 8)
                                                    {
                                                        switch (loc[0])
                                                        {
                                                        case 6:
                                                            set_lf32(&fracVal, loc[1], get_lf32(&fracVal, locSecondVar[1]));
                                                            break;
                                                        case 7:
                                                            set_ld64(&frac64Val, loc[1], (double)get_lf32(&fracVal, locSecondVar[1]));
                                                            break;
                                                        }
                                                    }
                                                    else
                                                    {
                                                        ERROR_HERE
                                                    }
                                                    break;
                                                case 7:
                                                    if (loc[0] > 5 && loc[0] < 8)
                                                    {
                                                        switch (loc[0])
                                                        {
                                                        case 6:
                                                            set_lf32(&fracVal, loc[1], (float)get_ld64(&frac64Val, locSecondVar[1]));
                                                            break;
                                                        case 7:
                                                            set_ld64(&frac64Val, loc[1], get_ld64(&frac64Val, locSecondVar[1]));
                                                            break;
                                                        }
                                                    }
                                                    else
                                                    {
                                                        ERROR_HERE
                                                    }
                                                    break;
                                                case 8:
                                                    if (loc[0] == 8)
                                                    {
                                                        set_ls(&textVal, loc[1], get_ls(&textVal, locSecondVar[1]));
                                                    }
                                                    else
                                                    {
                                                        ERROR_HERE
                                                    }
                                                    break;
                                                }

                                                free(locSecondVar);
                                            }
                                            else
                                            {
                                                ERROR_HERE
                                            }
                                            break;
                                        case 1:
                                            if (loc[0] == 1)
                                            {
                                                if (strcmp(get_ls(fline.ftoken->toks, 2), "off"))
                                                {
                                                    set_lui16(&stateVal, loc[1], TRUE);
                                                }
                                                else
                                                {
                                                    set_lui16(&stateVal, loc[1], TRUE);
                                                }
                                            }
                                            else
                                            {
                                                ERROR_HERE
                                            }
                                            break;
                                        case 2:
                                            if (loc[0] == 2)
                                            {
                                                set_lui16(&natVal, loc[1], (uint16_t)strtoull((get_ls(fline.ftoken->toks, 2)), &endChar, 10));
                                            }
                                            else
                                            {
                                                ERROR_HERE
                                            }
                                            break;
                                        case 3:
                                            if (loc[0] == 3)
                                            {
                                                set_lui64(&nat64Val, loc[1], strtoull((get_ls(fline.ftoken->toks, 2)), &endChar, 10));
                                            }
                                            else
                                            {
                                                ERROR_HERE
                                            }
                                            break;
                                        case 4:
                                            if (loc[0] == 4)
                                            {
                                                set_lsi32(&intVal, loc[1], (int32_t)atoll((get_ls(fline.ftoken->toks, 2))));
                                            }
                                            else
                                            {
                                                ERROR_HERE
                                            }
                                            break;
                                        case 5:
                                            if (loc[0] == 4)
                                            {
                                                set_lsi64(&int64Val, loc[1], atoll((get_ls(fline.ftoken->toks, 2))));
                                            }
                                            else
                                            {
                                                ERROR_HERE
                                            }
                                            break;
                                        case 6:
                                            if (loc[0] == 6)
                                            {
                                                set_lf32(&fracVal, loc[1], (float)atof(get_ls(fline.ftoken->toks, 2)));
                                            }
                                            else
                                            {
                                                ERROR_HERE
                                            }
                                            break;
                                        case 7:
                                            if (loc[0] == 7)
                                            {
                                                set_ld64(&frac64Val, loc[1], atof(get_ls(fline.ftoken->toks, 2)));
                                            }
                                            else
                                            {
                                                ERROR_HERE
                                            }
                                            break;
                                        case 8:
                                            if (loc[0] == 8)
                                            {
                                                set_ls(&textVal, loc[1], get_ls(fline.ftoken->toks, 2));
                                            }
                                            else
                                            {
                                                ERROR_HERE
                                            }
                                            break;
                                        }
                                    }
                                    else
                                    {
                                        ERROR_HERE
                                    }

                                    if (!get_lui16(&varDef, index))
                                    {
                                        set_lui16(&varDef, index, TRUE);
                                    }

                                    free(loc);
                                }
                                else
                                {
                                    ERROR_HERE
                                }
                            }
                            else
                            {
                                ERROR_HERE
                            }
                        }
                        else
                        {
                            ERROR_HERE
                        }
                    }

                    freeToken(fline.ftoken);
                }

                // printf("%d", fline.num);

                fclose(ski);

                goto exit;
            }
            else
            {
                THROW_ERROR(5);
            }
        }
        else
        {
            THROW_ERROR(3);
        }
    }
    else if (argc > 4)
    {
        THROW_ERROR(1);
    }

exit:

    if (program == 1)
    {
        FREE_PROGRAM
    }

    if (argc == 4)
    {
        printf("\ninterpreter exited %s with code: %d", argv[3], exitCode);
    }
    else if (argc == 3)
    {

        printf("\ninterpreter exited %s with code: %d", argv[2], exitCode);
    }
    else
    {
        printf("\ninterpreter exited with code: %d", exitCode);
    }
}