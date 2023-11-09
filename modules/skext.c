#pragma once

#include "../src/include/program.h"

#include <string.h>

#define GET_SKEXT(name)                                                                                                \
    push_ls(&moduleNames, name);                                                                                       \
    push_lui16(&moduleID, 4);                                                                                          \
                                                                                                                       \
    struct module skext;                                                                                               \
    init_lsig(&(skext.functionSignatures));                                                                            \
    init_ls(&(skext.functionNames));                                                                                   \
                                                                                                                       \
    struct functionSig replace_sig;                                                                                    \
    init_sig(&replace_sig);                                                                                            \
    replace_sig.numArgs = 2;                                                                                           \
    push_lui16(&(replace_sig.argID), 8);                                                                               \
    push_lui16(&(replace_sig.argID), 9);                                                                               \
    replace_sig.call = replace;                                                                                        \
                                                                                                                       \
    push_sig(&(skext.functionSignatures), &replace_sig);                                                               \
    push_ls(&(skext.functionNames), "replace");                                                                        \
                                                                                                                       \
    struct functionSig add_sig;                                                                                        \
    init_sig(&add_sig);                                                                                                \
    add_sig.numArgs = 2;                                                                                               \
    push_lui16(&(add_sig.argID), 8);                                                                                   \
    push_lui16(&(add_sig.argID), 9);                                                                                   \
    add_sig.call = add;                                                                                                \
                                                                                                                       \
    push_sig(&(skext.functionSignatures), &add_sig);                                                                   \
    push_ls(&(skext.functionNames), "add");                                                                            \
                                                                                                                       \
    struct functionSig length_sig;                                                                                     \
    init_sig(&length_sig);                                                                                             \
    length_sig.numArgs = 2;                                                                                            \
    push_lui16(&(length_sig.argID), 2);                                                                                \
    push_lui16(&(length_sig.argID), 9);                                                                                \
    length_sig.call = length;                                                                                          \
                                                                                                                       \
    push_sig(&(skext.functionSignatures), &length_sig);                                                                \
    push_ls(&(skext.functionNames), "length");                                                                         \
                                                                                                                       \
    struct functionSig equal_sig;                                                                                      \
    init_sig(&equal_sig);                                                                                              \
    equal_sig.numArgs = 3;                                                                                             \
    push_lui16(&(equal_sig.argID), 1);                                                                                 \
    push_lui16(&(equal_sig.argID), 8);                                                                                 \
    push_lui16(&(equal_sig.argID), 9);                                                                                 \
    equal_sig.call = equal;                                                                                            \
                                                                                                                       \
    push_sig(&(skext.functionSignatures), &equal_sig);                                                                 \
    push_ls(&(skext.functionNames), "equal");                                                                          \
                                                                                                                       \
    modules[modCount] = skext;                                                                                         \
    modCount++;

size_t replace()
{
    char *str;
    if (get_lui16(&isStringArg, 1))
    {
        str = get_ls(&argBuf, 1);
    }
    else
    {
        str = GET_TEXT(getVarLookUp(&varTable, getIndex_ls(&varBuf, get_ls(&argBuf, 1)))[1]);
    }

    list_ls tokens;
    init_ls(&tokens);

    size_t lenOriginal = strlen(str);
    size_t tokLen = 0;
    char *start = str;

    for (size_t i = 0; i <= lenOriginal; i++)
    {
        if (str[i] == ' ' || str[i] == '\t' || str[i] == '\0')
        {
            if (tokLen > 0)
            {
                char *word = (char *)malloc((tokLen + 1) * sizeof(char));
                memset(word, '\0', tokLen + 1);
                strncpy(word, start, tokLen);
                push_ls(&tokens, word);
                free(word);
                tokLen = 0;
            }
            start = str + i + 1;
        }
        else if (str[i] == '$')
        {
            if (tokLen > 0)
            {
                char *word = (char *)malloc((tokLen + 1) * sizeof(char));
                memset(word, '\0', tokLen + 1);
                strncpy(word, start, tokLen);
                push_ls(&tokens, word);
                free(word);
                tokLen = 0;
            }
            push_ls(&tokens, "KW_INTERP");
            start = str + i + 1;
        }
        else
        {
            tokLen++;
        }
    }

    for (size_t i = 0; i < tokens.size; i++)
    {
        // printf("%s ", get_ls(&tokens, i));
        if (!strcmp(get_ls(&tokens, i), "KW_INTERP"))
        {
            if (i + 1 == tokens.size)
            {
                break;
            }

            // printf("\nvar: %s\n", get_ls(&tokens, i + 1));
            size_t *loc = getVarLookUp(&varTable, getIndex_ls(&varBuf, get_ls(&tokens, i + 1)));

            char varStr[CHARACTER_LIMIT];

            switch (loc[0])
            {
            case 1:
                if (GET_STATE(loc[1]))
                {
                    sprintf(varStr, "on");
                }
                else
                {
                    sprintf(varStr, "off");
                }
                break;
            case 2:
                sprintf(varStr, "%hu", GET_NAT(loc[1]));
                break;
            case 3:
                sprintf(varStr, "%llu", GET_NAT64(loc[1]));
                break;
            case 4:
                sprintf(varStr, "%d", GET_INT(loc[1]));
                break;
            case 5:
                sprintf(varStr, "%lld", GET_INT64(loc[1]));
                break;
            case 6:
                sprintf(varStr, "%f", GET_FRAC(loc[1]));
                break;
            case 7:
                sprintf(varStr, "%lf", GET_FRAC64(loc[1]));
                break;
            case 8:
                sprintf(varStr, GET_TEXT(loc[1]));
                break;
            }

            set_ls(&tokens, i, EMPTY);
            set_ls(&tokens, i + 1, varStr);

            free(loc);
        }
    }

    // printf("\n");
    // for (size_t i = 0; i < tokens.size; i++)
    // {
    //     printf("%s ", get_ls(&tokens, i));
    // }

    size_t lenEnd = 0;
    list_ls interpolated;
    init_ls(&interpolated);

    for (size_t i = 0; i < tokens.size; i++)
    {
        if (strcmp(get_ls(&tokens, i), EMPTY))
        {
            char *gTok = get_ls(&tokens, i);
            lenEnd += strlen(gTok) + 1;
            push_ls(&interpolated, gTok);
        }
    }

    // printf("\n");
    // for (size_t i = 0; i < interpolated.size; i++)
    // {
    //     printf("%s ", get_ls(&interpolated, i));
    // }

    free_ls(&tokens);

    char *interpStr = malloc(lenEnd + 1 * sizeof(char));
    memset(interpStr, '\0', lenEnd + 1);

    char iTok[CHARACTER_LIMIT];

    for (size_t i = 0; i < interpolated.size; i++)
    {
        // printf(get_ls(&interpolated, i));
        sprintf(iTok, "%s ", get_ls(&interpolated, i));
        strcat(interpStr, iTok);
    }

    free_ls(&interpolated);
    interpStr[lenEnd] = '\0';

    set_ls(&textVal, getVarLookUp(&varTable, getIndex_ls(&varBuf, get_ls(&argBuf, 0)))[1], interpStr);
    return 0;
}

size_t add()
{
    size_t *loc = getVarLookUp(&varTable, getIndex_ls(&varBuf, get_ls(&argBuf, 0)));

    char *str1 = GET_TEXT(loc[1]);
    size_t len1 = strlen(str1);

    char *str2;
    if (get_lui16(&isStringArg, 1))
    {
        str2 = get_ls(&argBuf, 1);
    }
    else
    {
        str2 = GET_TEXT(getVarLookUp(&varTable, getIndex_ls(&varBuf, get_ls(&argBuf, 1)))[1]);
    }

    size_t len2 = strlen(str2);

    char *newStr = malloc((len1 + len2 + 1) * sizeof(char));
    strcpy(newStr, str1);
    strcat(newStr, str2);
    newStr[len1 + len2] = '\0';

    set_ls(&textVal, loc[1], newStr);

    free(loc);
    return 0;
}

size_t length()
{
    size_t *loc = getVarLookUp(&varTable, getIndex_ls(&varBuf, get_ls(&argBuf, 0)));

    char *str;
    if (get_lui16(&isStringArg, 1))
    {
        str = get_ls(&argBuf, 1);
    }
    else
    {
        str = GET_TEXT(getVarLookUp(&varTable, getIndex_ls(&varBuf, get_ls(&argBuf, 1)))[1]);
    }

    switch (loc[0])
    {
    case 2:
        set_lui16(&natVal, loc[1], (uint16_t)strlen(str));
        break;
    case 3:
        set_lui64(&nat64Val, loc[1], (uint64_t)strlen(str));
        break;
    case 4:
        set_lsi32(&intVal, loc[1], (int32_t)strlen(str));
        break;
    case 5:
        set_lsi64(&int64Val, loc[1], (int64_t)strlen(str));
        break;
    }

    free(loc);
    return 0;
}

size_t equal()
{

    char *str1, *str2;

    str1 = GET_TEXT(getVarLookUp(&varTable, getIndex_ls(&varBuf, get_ls(&argBuf, 1)))[1]);

    if (get_lui16(&isStringArg, 2))
    {
        str2 = get_ls(&argBuf, 2);
    }
    else
    {
        str2 = GET_TEXT(getVarLookUp(&varTable, getIndex_ls(&varBuf, get_ls(&argBuf, 2)))[1]);
    }

    size_t *loc = getVarLookUp(&varTable, getIndex_ls(&varBuf, get_ls(&argBuf, 0)));

    if (!strcmp(str1, str2))
    {
        set_lui16(&stateVal, loc[1], TRUE);
    }
    else
    {
        set_lui16(&stateVal, loc[1], FALSE);
    }

    free(loc);

    return 0;
}