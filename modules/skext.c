#pragma once

#include "../src/include/program.h"

#include <string.h>

#define GET_SKEXT(name)                                                                                                \
    push_ls(&moduleNames, name);                                                                                       \
    push_lui16(&moduleID, 2);                                                                                          \
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
    modules[modCount] = skext;                                                                                         \
    modCount++;

size_t replace()
{
    size_t index = getIndex_ls(&varBuf, get_ls(&argBuf, 0));
    if (index)
    {
        char *str;
        if (get_lui16(&isStringArg, 1))
        {
            str = get_ls(&argBuf, 1);
        }
        else
        {
            size_t indexStr = getIndex_ls(&varBuf, get_ls(&argBuf, 1));
            if (indexStr)
            {
                size_t *loc = getVarLookUp(&varTable, indexStr);
                str = GET_TEXT(loc[1]);
                free(loc);
            }
            else
            {
                return 22;
            }
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
                size_t index = getIndex_ls(&varBuf, get_ls(&tokens, i + 1));

                if (index)
                {
                    size_t *loc = malloc(2 * sizeof(size_t));
                    loc = getVarLookUp(&varTable, index);

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
                else
                {
                    return 22;
                }
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

        size_t *loc = getVarLookUp(&varTable, index);
        set_ls(&textVal, loc[1], interpStr);
        free(loc);
    }
    else
    {
        return 22;
    }
    return 0;
}