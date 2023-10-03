#pragma once

#include "math.h"
#include "program.h"

#define EMPTY "KW_EMPTY"

long long pow_ll(long long base, long long exp)
{
    long long r = base;
    for (size_t i = 1; i < exp; i++)
    {
        r *= base;
    }
    return r;
}

list_ls *tokenizeEval(char *expr)
{
    list_ls *expr_ls = malloc(sizeof(list_ls));
    init_ls(expr_ls);

    size_t len = strlen(expr);
    size_t tokLen = 0;

    char *start = expr;

    for (size_t i = 0; i <= len; i++)
    {
        if (expr[i] == ' ' || expr[i] == '\0')
        {
            if (tokLen > 0)
            {
                char *val = (char *)malloc((tokLen + 1) * sizeof(char));
                strncpy(val, start, tokLen);
                val[tokLen] = '\0';
                push_ls(expr_ls, val);
                tokLen = 0;
            }

            start = expr + i + 1;
        }
        else if (expr[i] == '{')
        {
            if (tokLen > 0)
            {
                char *val = (char *)malloc((tokLen + 1) * sizeof(char));
                strncpy(val, start, tokLen);
                val[tokLen] = '\0';
                push_ls(expr_ls, val);
                tokLen = 0;
            }
            start = expr + i;

            push_ls(expr_ls, "KW_EVAL_BLOCK");
            i++;
            start = expr + i;

            // printf("%zu %zu %c\n", i, tokLen, expr[i]);
            size_t nest = 1;
            while (nest)
            {
                i++;
                tokLen++;

                if (expr[i] == '\0')
                {
                    break;
                }
                else if (expr[i] == '{')
                {
                    nest++;
                }
                else if (expr[i] == '}')
                {
                    nest--;
                }

                // printf("%zu %zu %c\n", i, tokLen, expr[i]);
            }

            char *block = (char *)malloc((tokLen + 1) * sizeof(char));
            strncpy(block, start, tokLen);
            block[tokLen] = '\0';
            push_ls(expr_ls, block);
            tokLen = 0;
        }
        else if (expr[i] == '!')
        {
            if (tokLen > 0)
            {
                char *val = (char *)malloc((tokLen + 1) * sizeof(char));
                strncpy(val, start, tokLen);
                val[tokLen] = '\0';
                push_ls(expr_ls, val);
                tokLen = 0;
            }
            push_ls(expr_ls, "KW_INTERP");
            start = expr + i + 1;
        }
        else if (expr[i] == '^')
        {
            if (tokLen > 0)
            {
                char *val = (char *)malloc((tokLen + 1) * sizeof(char));
                strncpy(val, start, tokLen);
                val[tokLen] = '\0';
                push_ls(expr_ls, val);
                tokLen = 0;
            }
            push_ls(expr_ls, "KW_EXP");
            start = expr + i + 1;
        }
        else if (expr[i] == '*')
        {
            if (tokLen > 0)
            {
                char *val = (char *)malloc((tokLen + 1) * sizeof(char));
                strncpy(val, start, tokLen);
                val[tokLen] = '\0';
                push_ls(expr_ls, val);
                tokLen = 0;
            }
            push_ls(expr_ls, "KW_PROD");
            start = expr + i + 1;
        }
        else if (expr[i] == '/')
        {
            if (tokLen > 0)
            {
                char *val = (char *)malloc((tokLen + 1) * sizeof(char));
                strncpy(val, start, tokLen);
                val[tokLen] = '\0';
                push_ls(expr_ls, val);
                tokLen = 0;
            }
            push_ls(expr_ls, "KW_DIV");
            start = expr + i + 1;
        }
        else if (expr[i] == '%')
        {
            if (tokLen > 0)
            {
                char *val = (char *)malloc((tokLen + 1) * sizeof(char));
                strncpy(val, start, tokLen);
                val[tokLen] = '\0';
                push_ls(expr_ls, val);
                tokLen = 0;
            }
            push_ls(expr_ls, "KW_MOD");
            start = expr + i + 1;
        }
        else if (expr[i] == '+')
        {
            if (tokLen > 0)
            {
                char *val = (char *)malloc((tokLen + 1) * sizeof(char));
                strncpy(val, start, tokLen);
                val[tokLen] = '\0';
                push_ls(expr_ls, val);
                tokLen = 0;
            }
            push_ls(expr_ls, "KW_ADD");
            start = expr + i + 1;
        }
        else if (expr[i] == '-')
        {
            if (expr[i - 1] == '-')
            {
                tokLen++;
            }
            else
            {
                if (tokLen > 0)
                {
                    char *val = (char *)malloc((tokLen + 1) * sizeof(char));
                    strncpy(val, start, tokLen);
                    val[tokLen] = '\0';
                    push_ls(expr_ls, val);
                    tokLen = 0;
                }
                push_ls(expr_ls, "KW_SUB");
                start = expr + i + 1;
            }
        }
        else
        {
            tokLen++;
        }

        // printf("%zu %zu %c\n", i, tokLen, expr[i]);
    }
    return expr_ls;
}

list_ls *shrinkEval(list_ls *expr_ls)
{
    list_ls *shrink_ls = malloc(sizeof(list_ls));
    init_ls(shrink_ls);

    for (size_t i = 0; i < expr_ls->size; i++)
    {
        if (strcmp(get_ls(expr_ls, i), EMPTY))
        {
            push_ls(shrink_ls, get_ls(expr_ls, i));
        }
    }

    free_ls(expr_ls);
    return shrink_ls;
}

long long eval_ll(size_t *exitCode, char *expr)
{
    long long val;

    list_ls *expr_ls = tokenizeEval(expr);

    int power = 1;
    int product = 1;
    int sum = 1;

    for (size_t i = 0; i < expr_ls->size; i++)
    {
        if (!strcmp(get_ls(expr_ls, i), "KW_EVAL_BLOCK"))
        {
            if (i + 1 == expr_ls->size)
            {
                *exitCode = 1;
                return 0;
            }

            set_ls(expr_ls, i, EMPTY);
            char longAsString[19];
            sprintf(longAsString, "%lld", eval_ll(exitCode, get_ls(expr_ls, i + 1)));

            if (*exitCode)
            {
                return 0;
            }

            set_ls(expr_ls, i + 1, longAsString);
        }
    }

    // printf("\n");

    expr_ls = shrinkEval(expr_ls);

    for (size_t i = 0; i < expr_ls->size; i++)
    {
        // printf("%s\n", get_ls(expr_ls, i));

        if (!strcmp(get_ls(expr_ls, i), "KW_INTERP"))
        {
            if (i + 1 == expr_ls->size)
            {
                *exitCode = 1;
                return 0;
            }

            size_t index = getIndex_ls(&varBuf, get_ls(expr_ls, i + 1));

            if (index)
            {
                size_t *loc = malloc(2 * sizeof(size_t));
                loc = getVarLookUp(&varTable, index);

                char longAsString[19];

                switch (loc[0])
                {
                case 2:
                    sprintf(longAsString, "%lld", (long long)GET_NAT(loc[1]));
                    break;
                case 3:
                    sprintf(longAsString, "%lld", (long long)GET_NAT64(loc[1]));
                    break;
                case 4:
                    sprintf(longAsString, "%lld", (long long)GET_INT(loc[1]));
                    break;
                case 5:
                    sprintf(longAsString, "%lld", (long long)GET_INT64(loc[1]));
                    break;
                default:
                    *exitCode = 1;
                    return 0;
                }

                set_ls(expr_ls, i, EMPTY);
                set_ls(expr_ls, i + 1, longAsString);

                free(loc);
            }
            else
            {
                *exitCode = 1;
                return 0;
            }
        }
    }

    expr_ls = shrinkEval(expr_ls);

    char *end;

    while (power)
    {
        power = 0;

        for (size_t i = 0; i < expr_ls->size; i++)
        {
            if (!strcmp(get_ls(expr_ls, i), "KW_EXP"))
            {
                power = 1;

                if (i + 1 == expr_ls->size)
                {
                    *exitCode = 1;
                    return 0;
                }

                long long base = strtoll(get_ls(expr_ls, i - 1), &end, 10);
                long long exp = strtoll(get_ls(expr_ls, i + 1), &end, 10);

                if (*end != '\0')
                {
                    *exitCode = 1;
                    return 0;
                }

                char longAsString[19];
                sprintf(longAsString, "%lld", pow_ll(base, exp));

                set_ls(expr_ls, i - 1, EMPTY);
                set_ls(expr_ls, i + 1, EMPTY);
                set_ls(expr_ls, i, longAsString);

                expr_ls = shrinkEval(expr_ls);
            }
        }
    }

    while (product)
    {
        product = 0;

        for (size_t i = 0; i < expr_ls->size; i++)
        {
            if (!strcmp(get_ls(expr_ls, i), "KW_PROD") || !strcmp(get_ls(expr_ls, i), "KW_DIV") || !strcmp(get_ls(expr_ls, i), "KW_MOD"))
            {
                power = 1;

                if (i + 1 == expr_ls->size)
                {
                    *exitCode = 1;
                    return 0;
                }

                if (!strcmp(get_ls(expr_ls, i), "KW_PROD"))
                {
                    long long factor1 = strtoll(get_ls(expr_ls, i - 1), &end, 10);
                    long long factor2 = strtoll(get_ls(expr_ls, i + 1), &end, 10);

                    if (*end != '\0')
                    {
                        *exitCode = 1;
                        return 0;
                    }

                    char longAsString[19];
                    sprintf(longAsString, "%lld", factor1 * factor2);

                    set_ls(expr_ls, i - 1, EMPTY);
                    set_ls(expr_ls, i + 1, EMPTY);
                    set_ls(expr_ls, i, longAsString);
                }
                else if (!strcmp(get_ls(expr_ls, i), "KW_DIV"))
                {
                    long long dividend = strtoll(get_ls(expr_ls, i - 1), &end, 10);
                    long long divisor = strtoll(get_ls(expr_ls, i + 1), &end, 10);

                    if (*end != '\0')
                    {
                        *exitCode = 1;
                        return 0;
                    }

                    char longAsString[19];
                    sprintf(longAsString, "%lld", (long long)(dividend / divisor));

                    set_ls(expr_ls, i - 1, EMPTY);
                    set_ls(expr_ls, i + 1, EMPTY);
                    set_ls(expr_ls, i, longAsString);
                }
                else if (!strcmp(get_ls(expr_ls, i), "KW_MOD"))
                {
                    long long dividend = strtoll(get_ls(expr_ls, i - 1), &end, 10);
                    long long divisor = strtoll(get_ls(expr_ls, i + 1), &end, 10);

                    if (*end != '\0')
                    {
                        *exitCode = 1;
                        return 0;
                    }

                    char longAsString[19];
                    sprintf(longAsString, "%lld", dividend % divisor);

                    set_ls(expr_ls, i - 1, EMPTY);
                    set_ls(expr_ls, i + 1, EMPTY);
                    set_ls(expr_ls, i, longAsString);
                }

                expr_ls = shrinkEval(expr_ls);
            }
        }
    }

    while (sum)
    {
        sum = 0;

        for (size_t i = 0; i < expr_ls->size; i++)
        {
            if (!strcmp(get_ls(expr_ls, i), "KW_ADD") || !strcmp(get_ls(expr_ls, i), "KW_SUB"))
            {
                sum = 1;

                if (i + 1 == expr_ls->size)
                {
                    *exitCode = 1;
                    return 0;
                }

                if (!strcmp(get_ls(expr_ls, i), "KW_ADD"))
                {
                    long long addend1 = strtoll(get_ls(expr_ls, i - 1), &end, 10);
                    long long addend2 = strtoll(get_ls(expr_ls, i + 1), &end, 10);

                    if (*end != '\0')
                    {
                        *exitCode = 1;
                        return 0;
                    }

                    char longAsString[19];
                    sprintf(longAsString, "%lld", addend1 + addend2);

                    set_ls(expr_ls, i - 1, EMPTY);
                    set_ls(expr_ls, i + 1, EMPTY);
                    set_ls(expr_ls, i, longAsString);
                }
                else if (!strcmp(get_ls(expr_ls, i), "KW_SUB"))
                {
                    long long minuend = strtoll(get_ls(expr_ls, i - 1), &end, 10);
                    long long subtrahend = strtoll(get_ls(expr_ls, i + 1), &end, 10);

                    if (*end != '\0')
                    {
                        *exitCode = 1;
                        return 0;
                    }

                    char longAsString[19];
                    sprintf(longAsString, "%lld", minuend - subtrahend);

                    set_ls(expr_ls, i - 1, EMPTY);
                    set_ls(expr_ls, i + 1, EMPTY);
                    set_ls(expr_ls, i, longAsString);
                }

                expr_ls = shrinkEval(expr_ls);
            }
        }
    }

    val = atoll(get_ls(expr_ls, 0));
    free_ls(expr_ls);
    *exitCode = 0;
    return val;
}

double eval_dbl(size_t *exitCode, char *expr)
{
    double val;

    list_ls *expr_ls = tokenizeEval(expr);

    int power = 1;
    int product = 1;
    int sum = 1;

    for (size_t i = 0; i < expr_ls->size; i++)
    {
        if (!strcmp(get_ls(expr_ls, i), "KW_EVAL_BLOCK"))
        {
            if (i + 1 == expr_ls->size)
            {
                *exitCode = 1;
                return 0;
            }

            set_ls(expr_ls, i, EMPTY);
            char doubleAsString[310];
            sprintf(doubleAsString, "%lf", eval_dbl(exitCode, get_ls(expr_ls, i + 1)));

            if (*exitCode)
            {
                return 0;
            }

            set_ls(expr_ls, i + 1, doubleAsString);
        }
    }

    // printf("\n");

    expr_ls = shrinkEval(expr_ls);

    for (size_t i = 0; i < expr_ls->size; i++)
    {
        // printf("%s\n", get_ls(expr_ls, i));

        if (!strcmp(get_ls(expr_ls, i), "KW_INTERP"))
        {
            if (i + 1 == expr_ls->size)
            {
                *exitCode = 1;
                return 0;
            }

            size_t index = getIndex_ls(&varBuf, get_ls(expr_ls, i + 1));

            if (index)
            {
                size_t *loc = malloc(2 * sizeof(size_t));
                loc = getVarLookUp(&varTable, index);

                char doubleAsString[310];

                switch (loc[0])
                {
                case 6:
                    sprintf(doubleAsString, "%f", (double)GET_NAT(loc[1]));
                    break;
                case 7:
                    sprintf(doubleAsString, "%lf", (double)GET_NAT64(loc[1]));
                    break;
                default:
                    *exitCode = 1;
                    return 0;
                }

                set_ls(expr_ls, i, EMPTY);
                set_ls(expr_ls, i + 1, doubleAsString);

                free(loc);
            }
            else
            {
                *exitCode = 1;
                return 0;
            }
        }
    }

    expr_ls = shrinkEval(expr_ls);

    char *end;

    while (power)
    {
        power = 0;

        for (size_t i = 0; i < expr_ls->size; i++)
        {
            if (!strcmp(get_ls(expr_ls, i), "KW_EXP"))
            {
                power = 1;

                if (i + 1 == expr_ls->size)
                {
                    *exitCode = 1;
                    return 0;
                }

                double base = strtod(get_ls(expr_ls, i - 1), &end);
                double exp = strtod(get_ls(expr_ls, i + 1), &end);

                if (*end != '\0')
                {
                    *exitCode = 1;
                    return 0;
                }

                char doubleAsString[19];
                sprintf(doubleAsString, "%lf", pow(base, exp));

                set_ls(expr_ls, i - 1, EMPTY);
                set_ls(expr_ls, i + 1, EMPTY);
                set_ls(expr_ls, i, doubleAsString);

                expr_ls = shrinkEval(expr_ls);
            }
        }
    }

    while (product)
    {
        product = 0;

        for (size_t i = 0; i < expr_ls->size; i++)
        {
            if (!strcmp(get_ls(expr_ls, i), "KW_PROD") || !strcmp(get_ls(expr_ls, i), "KW_DIV") || !strcmp(get_ls(expr_ls, i), "KW_MOD"))
            {
                power = 1;

                if (i + 1 == expr_ls->size)
                {
                    *exitCode = 1;
                    return 0;
                }

                if (!strcmp(get_ls(expr_ls, i), "KW_PROD"))
                {
                    double factor1 = strtod(get_ls(expr_ls, i - 1), &end);
                    double factor2 = strtod(get_ls(expr_ls, i + 1), &end);

                    if (*end != '\0')
                    {
                        *exitCode = 1;
                        return 0;
                    }

                    char doubleAsString[19];
                    sprintf(doubleAsString, "%lf", factor1 * factor2);

                    set_ls(expr_ls, i - 1, EMPTY);
                    set_ls(expr_ls, i + 1, EMPTY);
                    set_ls(expr_ls, i, doubleAsString);
                }
                else if (!strcmp(get_ls(expr_ls, i), "KW_DIV"))
                {
                    double dividend = strtod(get_ls(expr_ls, i - 1), &end);
                    double divisor = strtod(get_ls(expr_ls, i + 1), &end);

                    if (*end != '\0')
                    {
                        *exitCode = 1;
                        return 0;
                    }

                    char doubleAsString[310];
                    sprintf(doubleAsString, "%lf", (double)(dividend / divisor));

                    set_ls(expr_ls, i - 1, EMPTY);
                    set_ls(expr_ls, i + 1, EMPTY);
                    set_ls(expr_ls, i, doubleAsString);
                }
                else if (!strcmp(get_ls(expr_ls, i), "KW_MOD"))
                {
                    *exitCode = 1;
                    return 0;
                }

                expr_ls = shrinkEval(expr_ls);
            }
        }
    }

    while (sum)
    {
        sum = 0;

        for (size_t i = 0; i < expr_ls->size; i++)
        {
            if (!strcmp(get_ls(expr_ls, i), "KW_ADD") || !strcmp(get_ls(expr_ls, i), "KW_SUB"))
            {
                sum = 1;

                if (i + 1 == expr_ls->size)
                {
                    *exitCode = 1;
                    return 0;
                }

                if (!strcmp(get_ls(expr_ls, i), "KW_ADD"))
                {
                    double addend1 = strtod(get_ls(expr_ls, i - 1), &end);
                    double addend2 = strtod(get_ls(expr_ls, i + 1), &end);

                    if (*end != '\0')
                    {
                        *exitCode = 1;
                        return 0;
                    }

                    char doubleAsString[310];
                    sprintf(doubleAsString, "%lf", addend1 + addend2);

                    set_ls(expr_ls, i - 1, EMPTY);
                    set_ls(expr_ls, i + 1, EMPTY);
                    set_ls(expr_ls, i, doubleAsString);
                }
                else if (!strcmp(get_ls(expr_ls, i), "KW_SUB"))
                {
                    double minuend = strtod(get_ls(expr_ls, i - 1), &end);
                    double subtrahend = strtod(get_ls(expr_ls, i + 1), &end);

                    if (*end != '\0')
                    {
                        *exitCode = 1;
                        return 0;
                    }

                    char doubleAsString[310];
                    sprintf(doubleAsString, "%lf", minuend - subtrahend);

                    set_ls(expr_ls, i - 1, EMPTY);
                    set_ls(expr_ls, i + 1, EMPTY);
                    set_ls(expr_ls, i, doubleAsString);
                }

                expr_ls = shrinkEval(expr_ls);
            }
        }
    }

    val = atof(get_ls(expr_ls, 0));
    free_ls(expr_ls);
    *exitCode = 0;
    return val;
}
