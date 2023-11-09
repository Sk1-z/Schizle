#pragma once

#include "program.h"

long long pow_ll(long long base, long long exp)
{
    long long r = base;
    for (size_t i = 1; i < exp; i++)
    {
        r *= base;
    }
    return r;
}

list_ls *shrinkExp(list_ls *expr_ls)
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
        else if (expr[i] == '$')
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

    expr_ls = shrinkExp(expr_ls);

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

    expr_ls = shrinkExp(expr_ls);

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

                expr_ls = shrinkExp(expr_ls);
            }
        }
    }

    while (product)
    {
        product = 0;

        for (size_t i = 0; i < expr_ls->size; i++)
        {
            if (!strcmp(get_ls(expr_ls, i), "KW_PROD") || !strcmp(get_ls(expr_ls, i), "KW_DIV") ||
                !strcmp(get_ls(expr_ls, i), "KW_MOD"))
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

                expr_ls = shrinkExp(expr_ls);
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

                expr_ls = shrinkExp(expr_ls);
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

    expr_ls = shrinkExp(expr_ls);

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
                    sprintf(doubleAsString, "%f", (double)GET_FRAC(loc[1]));
                    break;
                case 7:
                    sprintf(doubleAsString, "%lf", (double)GET_FRAC64(loc[1]));
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

    expr_ls = shrinkExp(expr_ls);

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

                expr_ls = shrinkExp(expr_ls);
            }
        }
    }

    while (product)
    {
        product = 0;

        for (size_t i = 0; i < expr_ls->size; i++)
        {
            if (!strcmp(get_ls(expr_ls, i), "KW_PROD") || !strcmp(get_ls(expr_ls, i), "KW_DIV") ||
                !strcmp(get_ls(expr_ls, i), "KW_MOD"))
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
                    double dividend = strtod(get_ls(expr_ls, i - 1), &end);
                    double divisor = strtod(get_ls(expr_ls, i + 1), &end);

                    if (*end != '\0')
                    {
                        *exitCode = 1;
                        return 0;
                    }

                    char doubleAsString[310];
                    sprintf(doubleAsString, "%lf", fmod(dividend, divisor));

                    set_ls(expr_ls, i - 1, EMPTY);
                    set_ls(expr_ls, i + 1, EMPTY);
                    set_ls(expr_ls, i, doubleAsString);
                }

                expr_ls = shrinkExp(expr_ls);
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

                expr_ls = shrinkExp(expr_ls);
            }
        }
    }

    val = atof(get_ls(expr_ls, 0));
    free_ls(expr_ls);
    *exitCode = 0;
    return val;
}

list_ls *tokenizeCond(char *expr)
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
        else if (expr[i] == '(')
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

            push_ls(expr_ls, "KW_COND_BLOCK");
            i++;
            start = expr + i;

            size_t nest = 1;
            while (nest)
            {
                i++;
                tokLen++;

                if (expr[i] == '\0')
                {
                    break;
                }
                else if (expr[i] == '(')
                {
                    nest++;
                }
                else if (expr[i] == ')')
                {
                    nest--;
                }
            }

            char *block = (char *)malloc((tokLen + 1) * sizeof(char));
            strncpy(block, start, tokLen);
            block[tokLen] = '\0';
            push_ls(expr_ls, block);
            tokLen = 0;
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
            }

            char *block = (char *)malloc((tokLen + 1) * sizeof(char));
            strncpy(block, start, tokLen);
            block[tokLen] = '\0';
            push_ls(expr_ls, block);
            tokLen = 0;
        }
        else if (expr[i] == '$')
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
        else if (expr[i] == '~')
        {
            // if (expr[i + 1] == '=')
            // {
            //     if (tokLen > 0)
            //     {
            //         char *val = (char *)malloc((tokLen + 1) * sizeof(char));
            //         strncpy(val, start, tokLen);
            //         val[tokLen] = '\0';
            //         push_ls(expr_ls, val);
            //         tokLen = 0;
            //     }
            //     push_ls(expr_ls, "KW_NEGEQ");
            //     start = expr + i + 2;
            //     i++;
            // }
            // else
            // {
            if (tokLen > 0)
            {
                char *val = (char *)malloc((tokLen + 1) * sizeof(char));
                strncpy(val, start, tokLen);
                val[tokLen] = '\0';
                push_ls(expr_ls, val);
                tokLen = 0;
            }
            push_ls(expr_ls, "KW_NEG");
            start = expr + i + 1;
            // }
        }
        else if (expr[i] == '/' && expr[i + 1] == '\\')
        {
            if (tokLen > 0)
            {
                char *val = (char *)malloc((tokLen + 1) * sizeof(char));
                strncpy(val, start, tokLen);
                val[tokLen] = '\0';
                push_ls(expr_ls, val);
                tokLen = 0;
            }
            push_ls(expr_ls, "KW_CONJ");
            start = expr + i + 2;
            i++;
        }
        else if (expr[i] == '\\' && expr[i + 1] == '/')
        {
            if (tokLen > 0)
            {
                char *val = (char *)malloc((tokLen + 1) * sizeof(char));
                strncpy(val, start, tokLen);
                val[tokLen] = '\0';
                push_ls(expr_ls, val);
                tokLen = 0;
            }
            push_ls(expr_ls, "KW_DCONJ");
            start = expr + i + 2;
            i++;
        }
        else if (expr[i] == '=')
        {
            if (tokLen > 0)
            {
                char *val = (char *)malloc((tokLen + 1) * sizeof(char));
                strncpy(val, start, tokLen);
                val[tokLen] = '\0';
                push_ls(expr_ls, val);
                tokLen = 0;
            }
            push_ls(expr_ls, "KW_EQ");
            start = expr + i + 1;
        }
        else if (expr[i] == '>')
        {
            if (expr[i + 1] == '=')
            {
                if (tokLen > 0)
                {
                    char *val = (char *)malloc((tokLen + 1) * sizeof(char));
                    strncpy(val, start, tokLen);
                    val[tokLen] = '\0';
                    push_ls(expr_ls, val);
                    tokLen = 0;
                }
                push_ls(expr_ls, "KW_GE");
                start = expr + i + 2;
                i++;
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
                push_ls(expr_ls, "KW_G");
                start = expr + i + 1;
            }
        }
        else if (expr[i] == '<')
        {
            if (expr[i + 1] == '=')
            {
                if (tokLen > 0)
                {
                    char *val = (char *)malloc((tokLen + 1) * sizeof(char));
                    strncpy(val, start, tokLen);
                    val[tokLen] = '\0';
                    push_ls(expr_ls, val);
                    tokLen = 0;
                }
                push_ls(expr_ls, "KW_LE");
                start = expr + i + 2;
                i++;
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
                push_ls(expr_ls, "KW_L");
                start = expr + i + 1;
            }
        }
        else
        {
            tokLen++;
        }
    }

    for (size_t i = 0; i < expr_ls->size; i++)
    {
        if (!strcmp(get_ls(expr_ls, i), "and"))
        {
            set_ls(expr_ls, i, "KW_CONJ");
        }
        else if (!strcmp(get_ls(expr_ls, i), "or"))
        {
            set_ls(expr_ls, i, "KW_DCONJ");
        }
        else if (!strcmp(get_ls(expr_ls, i), "not"))
        {
            set_ls(expr_ls, i, "KW_NEG");
        }
        else if (!strcmp(get_ls(expr_ls, i), "on"))
        {
            set_ls(expr_ls, i, "KW_ON");
        }
        else if (!strcmp(get_ls(expr_ls, i), "off"))
        {
            set_ls(expr_ls, i, "KW_OFF");
        }
    }

    return expr_ls;
}

int cond(size_t *exitCode, char *expr)
{
    list_ls *expr_ls = tokenizeCond(expr);

    int eq = 1;
    int negEq = 1;
    int greatEq = 1;
    int great = 1;
    int lessEq = 1;
    int less = 1;
    int negation = 1;
    int conjuction = 1;
    int disconjuction = 1;

    for (size_t i = 0; i < expr_ls->size; i++)
    {
        if (!strcmp(get_ls(expr_ls, i), "KW_COND_BLOCK"))
        {
            if (i + 1 == expr_ls->size)
            {
                *exitCode = 1;
                return 0;
            }

            set_ls(expr_ls, i, EMPTY);

            if (cond(exitCode, get_ls(expr_ls, i + 1)))
            {
                set_ls(expr_ls, i + 1, "KW_ON");
            }
            else
            {
                set_ls(expr_ls, i + 1, "KW_OFF");
            }

            if (*exitCode)
            {
                return 0;
            }
        }
    }

    expr_ls = shrinkExp(expr_ls);

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
            char *eval = get_ls(expr_ls, i + 1);
            int dbl = 0;

            for (size_t j = 0; j < strlen(eval); j++)
            {
                if (eval[j] == '.')
                {
                    dbl = 1;
                    break;
                }
            }

            if (dbl)
            {
                char doubleAsString[310];
                sprintf(doubleAsString, "%lf", eval_dbl(exitCode, get_ls(expr_ls, i + 1)));
                if (*exitCode)
                {
                    return 0;
                }
                set_ls(expr_ls, i + 1, doubleAsString);
            }
            else
            {
                char longAsString[19];
                sprintf(longAsString, "%lld", eval_ll(exitCode, get_ls(expr_ls, i + 1)));
                if (*exitCode)
                {
                    return 0;
                }
                set_ls(expr_ls, i + 1, longAsString);
            }
        }
    }

    expr_ls = shrinkExp(expr_ls);

    for (size_t i = 0; i < expr_ls->size; i++)
    {
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

                set_ls(expr_ls, i, EMPTY);
                char numStr[19];
                long long num;

                switch (loc[0])
                {
                case 1:
                    if (GET_STATE(loc[1]))
                    {
                        num = 1;
                    }
                    else
                    {
                        num = 0;
                    }
                    break;
                case 2:
                    num = (long long)GET_NAT(loc[1]);
                    break;
                case 3:
                    num = (long long)GET_NAT64(loc[1]);
                    break;
                case 4:
                    num = (long long)GET_INT(loc[1]);
                    break;
                case 5:
                    num = (long long)GET_INT64(loc[1]);
                    break;
                default:
                    *exitCode = 1;
                    return 0;
                }

                sprintf(numStr, "%lld", num);
                set_ls(expr_ls, i + 1, numStr);

                free(loc);
            }
            else
            {
                *exitCode = 1;
                return 0;
            }
        }
    }

    expr_ls = shrinkExp(expr_ls);

    char *end;

    while (eq)
    {
        eq = 0;

        for (size_t i = 0; i < expr_ls->size; i++)
        {
            if (!strcmp(get_ls(expr_ls, i), "KW_EQ"))
            {
                eq = 1;

                if (i + 1 == expr_ls->size)
                {
                    *exitCode = 1;
                    return 0;
                }

                long long a = strtoll(get_ls(expr_ls, i - 1), &end, 10);
                long long b = strtoll(get_ls(expr_ls, i + 1), &end, 10);

                if (*end != '\0')
                {
                    *exitCode = 1;
                    return 0;
                }

                set_ls(expr_ls, i - 1, EMPTY);
                set_ls(expr_ls, i + 1, EMPTY);

                if (a == b)
                {
                    set_ls(expr_ls, i, "KW_ON");
                }
                else
                {
                    set_ls(expr_ls, i, "KW_OFF");
                }

                expr_ls = shrinkExp(expr_ls);
            }
        }
    }

    // while (negEq)
    // {
    //     negEq = 0;

    //     for (size_t i = 0; i < expr_ls->size; i++)
    //     {
    //         if (!strcmp(get_ls(expr_ls, i), "KW_NEGEQ"))
    //         {
    //             negEq = 1;

    //             if (i + 1 == expr_ls->size)
    //             {
    //                 *exitCode = 1;
    //                 return 0;
    //             }

    //             long long a = strtoll(get_ls(expr_ls, i - 1), &end, 10);
    //             long long b = strtoll(get_ls(expr_ls, i + 1), &end, 10);

    //             if (*end != '\0')
    //             {
    //                 *exitCode = 1;
    //                 return 0;
    //             }

    //             set_ls(expr_ls, i - 1, EMPTY);
    //             set_ls(expr_ls, i + 1, EMPTY);

    //             if (a != b)
    //             {
    //                 set_ls(expr_ls, i, "KW_ON");
    //             }
    //             else
    //             {
    //                 set_ls(expr_ls, i, "KW_OFF");
    //             }

    //             expr_ls = shrinkExp(expr_ls);
    //         }
    //     }
    // }

    while (greatEq)
    {
        greatEq = 0;

        for (size_t i = 0; i < expr_ls->size; i++)
        {
            if (!strcmp(get_ls(expr_ls, i), "KW_greatEq"))
            {
                greatEq = 1;

                if (i + 1 == expr_ls->size)
                {
                    *exitCode = 1;
                    return 0;
                }

                long long a = strtoll(get_ls(expr_ls, i - 1), &end, 10);
                long long b = strtoll(get_ls(expr_ls, i + 1), &end, 10);

                if (*end != '\0')
                {
                    *exitCode = 1;
                    return 0;
                }

                set_ls(expr_ls, i - 1, EMPTY);
                set_ls(expr_ls, i + 1, EMPTY);

                if (a >= b)
                {
                    set_ls(expr_ls, i, "KW_ON");
                }
                else
                {
                    set_ls(expr_ls, i, "KW_OFF");
                }

                expr_ls = shrinkExp(expr_ls);
            }
        }
    }

    while (great)
    {
        great = 0;

        for (size_t i = 0; i < expr_ls->size; i++)
        {
            if (!strcmp(get_ls(expr_ls, i), "KW_G"))
            {
                great = 1;

                if (i + 1 == expr_ls->size)
                {
                    *exitCode = 1;
                    return 0;
                }

                long long a = strtoll(get_ls(expr_ls, i - 1), &end, 10);
                long long b = strtoll(get_ls(expr_ls, i + 1), &end, 10);

                if (*end != '\0')
                {
                    *exitCode = 1;
                    return 0;
                }

                set_ls(expr_ls, i - 1, EMPTY);
                set_ls(expr_ls, i + 1, EMPTY);

                if (a > b)
                {
                    set_ls(expr_ls, i, "KW_ON");
                }
                else
                {
                    set_ls(expr_ls, i, "KW_OFF");
                }

                expr_ls = shrinkExp(expr_ls);
            }
        }
    }

    while (lessEq)
    {
        lessEq = 0;

        for (size_t i = 0; i < expr_ls->size; i++)
        {
            if (!strcmp(get_ls(expr_ls, i), "KW_LE"))
            {
                lessEq = 1;

                if (i + 1 == expr_ls->size)
                {
                    *exitCode = 1;
                    return 0;
                }

                long long a = strtoll(get_ls(expr_ls, i - 1), &end, 10);
                long long b = strtoll(get_ls(expr_ls, i + 1), &end, 10);

                if (*end != '\0')
                {
                    *exitCode = 1;
                    return 0;
                }

                set_ls(expr_ls, i - 1, EMPTY);
                set_ls(expr_ls, i + 1, EMPTY);

                if (a <= b)
                {
                    set_ls(expr_ls, i, "KW_ON");
                }
                else
                {
                    set_ls(expr_ls, i, "KW_OFF");
                }

                expr_ls = shrinkExp(expr_ls);
            }
        }
    }

    while (less)
    {
        less = 0;

        for (size_t i = 0; i < expr_ls->size; i++)
        {
            if (!strcmp(get_ls(expr_ls, i), "KW_L"))
            {
                less = 1;

                if (i + 1 == expr_ls->size)
                {
                    *exitCode = 1;
                    return 0;
                }

                long long a = strtoll(get_ls(expr_ls, i - 1), &end, 10);
                long long b = strtoll(get_ls(expr_ls, i + 1), &end, 10);

                if (*end != '\0')
                {
                    *exitCode = 1;
                    return 0;
                }

                set_ls(expr_ls, i - 1, EMPTY);
                set_ls(expr_ls, i + 1, EMPTY);

                if (a < b)
                {
                    set_ls(expr_ls, i, "KW_ON");
                }
                else
                {
                    set_ls(expr_ls, i, "KW_OFF");
                }

                expr_ls = shrinkExp(expr_ls);
            }
        }
    }

    while (negation)
    {
        negation = 0;

        for (size_t i = 0; i < expr_ls->size; i++)
        {
            if (!strcmp(get_ls(expr_ls, i), "KW_NEG"))
            {
                negation = 1;

                if (i + 1 == expr_ls->size)
                {
                    *exitCode = 1;
                    return 0;
                }

                set_ls(expr_ls, i, EMPTY);
                if (!strcmp(get_ls(expr_ls, i + 1), "KW_ON") || atoll(get_ls(expr_ls, i + 1)))
                {
                    set_ls(expr_ls, i + 1, "KW_OFF");
                }
                else
                {
                    set_ls(expr_ls, i + 1, "KW_ON");
                }

                expr_ls = shrinkExp(expr_ls);
            }
        }
    }

    while (disconjuction)
    {
        disconjuction = 0;

        for (size_t i = 0; i < expr_ls->size; i++)
        {
            if (!strcmp(get_ls(expr_ls, i), "KW_DCONJ"))
            {
                disconjuction = 1;

                if (i + 1 == expr_ls->size)
                {
                    *exitCode = 1;
                    return 0;
                }

                long long a;
                long long b;

                if (!strcmp(get_ls(expr_ls, i - 1), "KW_ON"))
                {
                    a = 1;
                }
                else
                {
                    a = 0;
                }

                if (!strcmp(get_ls(expr_ls, i + 1), "KW_ON"))
                {
                    b = 1;
                }
                else
                {
                    b = 0;
                }

                if (a || b)
                {
                    set_ls(expr_ls, i, "KW_ON");
                }
                else
                {
                    set_ls(expr_ls, i, "KW_OFF");
                }

                set_ls(expr_ls, i - 1, EMPTY);
                set_ls(expr_ls, i + 1, EMPTY);

                expr_ls = shrinkExp(expr_ls);
            }
        }
    }

    while (conjuction)
    {
        conjuction = 0;

        for (size_t i = 0; i < expr_ls->size; i++)
        {
            if (!strcmp(get_ls(expr_ls, i), "KW_CONJ"))
            {
                conjuction = 1;

                if (i + 1 == expr_ls->size)
                {
                    *exitCode = 1;
                    return 0;
                }

                long long a;
                long long b;

                if (!strcmp(get_ls(expr_ls, i - 1), "KW_ON"))
                {
                    a = 1;
                }
                else
                {
                    a = 0;
                }

                if (!strcmp(get_ls(expr_ls, i + 1), "KW_ON"))
                {
                    b = 1;
                }
                else
                {
                    b = 0;
                }

                if (a && b)
                {
                    set_ls(expr_ls, i, "KW_ON");
                }
                else
                {
                    set_ls(expr_ls, i, "KW_OFF");
                }

                set_ls(expr_ls, i - 1, EMPTY);
                set_ls(expr_ls, i + 1, EMPTY);

                expr_ls = shrinkExp(expr_ls);
            }
        }
    }

    // for (size_t i = 0; i < expr_ls->size; i++)
    // {
    //     printf("\n%s", get_ls(expr_ls, i));
    // }
    // printf("\n");

    *exitCode = 0;
    if (atoll(get_ls(expr_ls, 0)) || !strcmp(get_ls(expr_ls, 0), "KW_ON"))
    {
        free_ls(expr_ls);
        return 1;
    }
    else
    {
        free_ls(expr_ls);
        return 0;
    }
}