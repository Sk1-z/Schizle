#pragma once

#include <stdlib.h>
#include <string.h>

#include "lists.h"

// Tokenize string by whitespace and quotes
typedef struct
{
    list_lui16 isStringL;
    list_ls *toks;

} token;

void initToken(token *tok)
{
    init_ls(tok->toks);
    init_lui16(&(tok->isStringL));
}

void freeToken(token *tok)
{
    free_ls(tok->toks);
    free_lui16(&(tok->isStringL));
}

token *tokenize(char *string)
{
    token *tok;
    initToken(tok);

    size_t strLen = strlen(string);
    size_t tokLen = 0;

    char *wordStart = (char *)string;

    if (string[0] == '\n' || string[0] == '_')
    {
        push_ls(tok->toks, "\n");
        push_lui16(&(tok->isStringL), 0);
    }
    else
    {
        for (size_t i = 0; i <= strLen; i++)
        {
            if (string[i] == ' ' || string[i] == '\t' || string[i] == ',' || string[i] == '\0')
            {
                if (tokLen > 0)
                {
                    char *word = (char *)malloc((tokLen + 1) * sizeof(char));
                    strncpy(word, wordStart, tokLen);
                    word[tokLen] = '\0';
                    push_ls(tok->toks, word);
                    tokLen = 0;

                    push_lui16(&(tok->isStringL), 0);
                }
                wordStart = (char *)string + i + 1;
            }
            else if (string[i] == '\n')
            {
                if (tokLen > 0)
                {
                    char *word = (char *)malloc((tokLen + 1) * sizeof(char));
                    strncpy(word, wordStart, tokLen);
                    word[tokLen] = '\0';
                    push_ls(tok->toks, word);
                    tokLen = 0;

                    push_lui16(&(tok->isStringL), 0);
                }
                wordStart = (char *)string + i + 1;
            }
            else if (string[i] == ':')
            {
                if (string[i + 1] == ':' && string[i - 1] != ':')
                {
                    if (tokLen > 0)
                    {
                        char *word = (char *)malloc((tokLen + 1) * sizeof(char));
                        strncpy(word, wordStart, tokLen);
                        word[tokLen] = '\0';
                        push_ls(tok->toks, word);
                        tokLen = 0;

                        push_lui16(&(tok->isStringL), 0);
                    }
                    push_ls(tok->toks, "KW_MEMBER");
                    push_lui16(&(tok->isStringL), 0);
                    wordStart = (char *)string + i + 1;
                }
                else if (string[i + 1] != ':' && string[i - 1] != ':')
                {
                    if (tokLen > 0)
                    {
                        char *word = (char *)malloc((tokLen + 1) * sizeof(char));
                        strncpy(word, wordStart, tokLen);
                        word[tokLen] = '\0';
                        push_ls(tok->toks, word);
                        tokLen = 0;

                        push_lui16(&(tok->isStringL), 0);
                    }
                    push_ls(tok->toks, "KW_TYPE");
                    push_lui16(&(tok->isStringL), 0);
                    wordStart = (char *)string + i + 1;
                }
            }
            else if (string[i] == '!')
            {
                if (string[i + 1] == '[')
                {
                    if (tokLen > 0)
                    {
                        char *word = (char *)malloc((tokLen + 1) * sizeof(char));
                        strncpy(word, wordStart, tokLen);
                        word[tokLen] = '\0';
                        push_ls(tok->toks, word);
                        tokLen = 0;

                        push_lui16(&(tok->isStringL), 0);
                    }
                    push_ls(tok->toks, "KW_PARAMS");
                    push_lui16(&(tok->isStringL), 0);
                    wordStart = (char *)string + i + 1;
                }
                else if (string[i + 1] == '<')
                {
                }
                else if (string[i + 1] == '!')
                {
                    if (tokLen > 0)
                    {
                        char *word = (char *)malloc((tokLen + 1) * sizeof(char));
                        strncpy(word, wordStart, tokLen);
                        word[tokLen] = '\0';
                        push_ls(tok->toks, word);
                        tokLen = 0;

                        push_lui16(&(tok->isStringL), 0);
                    }
                    push_ls(tok->toks, "KW_IMMUT");
                    push_lui16(&(tok->isStringL), 0);
                    wordStart = (char *)string + i + 1;
                }
                else if (string[i - 1] != '!')
                {
                    if (tokLen > 0)
                    {
                        char *word = (char *)malloc((tokLen + 1) * sizeof(char));
                        strncpy(word, wordStart, tokLen);
                        word[tokLen] = '\0';
                        push_ls(tok->toks, word);
                        tokLen = 0;

                        push_lui16(&(tok->isStringL), 0);
                    }
                    push_ls(tok->toks, "KW_MUT");
                    push_lui16(&(tok->isStringL), 0);
                    wordStart = (char *)string + i + 1;
                }
            }
            else if (string[i] == '[' || string[i] == '<')
            {
                if (string[i - 1] == '!')
                {
                    wordStart = (char *)string + i + 1;
                }
            }
            else if (string[i] == ']' || string[i] == '>')
            {

                for (size_t j = 0; j < i; j++)
                {
                    if (string[j] == '!')
                    {
                        if (tokLen > 0)
                        {
                            char *word = (char *)malloc((tokLen + 1) * sizeof(char));
                            strncpy(word, wordStart, tokLen);
                            word[tokLen] = '\0';
                            push_ls(tok->toks, word);
                            tokLen = 0;

                            push_lui16(&(tok->isStringL), 0);
                        }
                        push_ls(tok->toks, "end");
                        push_lui16(&(tok->isStringL), 0);
                        wordStart = (char *)string + i + 1;
                        break;
                    }
                }
            }
            else if (string[i] == '@')
            {
                if (tokLen > 0)
                {
                    char *word = (char *)malloc((tokLen + 1) * sizeof(char));
                    strncpy(word, wordStart, tokLen);
                    word[tokLen] = '\0';
                    push_ls(tok->toks, word);
                    tokLen = 0;

                    push_lui16(&(tok->isStringL), 0);
                }
                push_ls(tok->toks, "KW_MOD");
                push_lui16(&(tok->isStringL), 0);
                wordStart = (char *)string + i + 1;
            }
            else if (string[i] == '=')
            {
                if (tokLen > 0)
                {
                    char *word = (char *)malloc((tokLen + 1) * sizeof(char));
                    strncpy(word, wordStart, tokLen);
                    word[tokLen] = '\0';
                    push_ls(tok->toks, word);
                    tokLen = 0;

                    push_lui16(&(tok->isStringL), 0);
                }
                push_ls(tok->toks, "KW_EQ");
                push_lui16(&(tok->isStringL), 0);
                wordStart = (char *)string + i + 1;
            }
            else if (string[i] == '{')
            {
                // if (tokLen > 0)
                // {
                //     char *val = (char *)malloc((tokLen + 1) * sizeof(char));
                //     strncpy(val, wordStart, tokLen);
                //     val[tokLen] = '\0';
                //     push_ls(tok->toks, val);
                //     tokLen = 0;
                // }
                // wordStart = (char *)string + i;

                // printf("\n");
                // size_t j = 0;
                // while (string[j] != '\0')
                // {
                //     printf("%c", *(wordStart + j));
                //     j++;
                // }

                push_ls(tok->toks, "KW_EVAL_BLOCK");
                push_lui16(&(tok->isStringL), 0);

                wordStart = (char *)string + i + 1;

                size_t nest = 1;
                tokLen--;

                while (nest)
                {
                    i++;
                    tokLen++;

                    if (string[i] == '\0')
                    {
                        break;
                    }
                    else if (string[i] == '{')
                    {
                        nest++;
                    }
                    else if (string[i] == '}')
                    {
                        nest--;
                    }
                }

                char *word = (char *)malloc((tokLen + 1) * sizeof(char));
                strncpy(word, wordStart, tokLen);
                word[tokLen] = '\0';
                push_ls(tok->toks, word);
                push_lui16(&(tok->isStringL), 0);
                tokLen = 0;
            }
            else if (string[i] == '(')
            {
                push_ls(tok->toks, "KW_COND_BLOCK");
                push_lui16(&(tok->isStringL), 0);
                i++;
                wordStart = (char *)string + i;
                while (string[i] != ')' && string[i] != '\0')
                {
                    i++;
                    tokLen++;
                }

                char *word = (char *)malloc((tokLen + 1) * sizeof(char));
                strncpy(word, wordStart, tokLen);
                word[tokLen] = '\0';
                push_ls(tok->toks, word);
                push_lui16(&(tok->isStringL), 0);
                tokLen = 0;
            }
            else if (string[i] == '\'')
            {
                i++;
                wordStart = (char *)string + i;
                while (string[i] != '\'' && string[i] != '\0')
                {
                    if (string[i] == '\\' && string[i + 1] == 'n')
                    {
                        string[i] = ' ';
                        string[i + 1] = '\n';
                    }
                    else if (string[i] == '\\' && string[i + 1] == 't')
                    {
                        string[i] = ' ';
                        string[i + 1] = '\t';
                    }
                    else if (string[i] == '\\' && string[i + 1] == 'b')
                    {
                        string[i] = ' ';
                        string[i + 1] = '\b';
                    }

                    i++;
                    tokLen++;
                }

                char *word = (char *)malloc((tokLen + 1) * sizeof(char));
                strncpy(word, wordStart, tokLen);
                word[tokLen] = '\0';
                push_ls(tok->toks, word);
                tokLen = 0;

                push_lui16(&(tok->isStringL), 1);
            }
            else if (string[i] == '\"')
            {
                i++;
                wordStart = (char *)string + i;
                while (string[i] != '\"' && string[i] != '\0')
                {
                    if (string[i] == '\\' && string[i + 1] == 'n')
                    {
                        string[i] = ' ';
                        string[i + 1] = '\n';
                    }
                    else if (string[i] == '\\' && string[i + 1] == 't')
                    {
                        string[i] = ' ';
                        string[i + 1] = '\t';
                    }
                    else if (string[i] == '\\' && string[i + 1] == 'b')
                    {
                        string[i] = ' ';
                        string[i + 1] = '\b';
                    }

                    i++;
                    tokLen++;
                }

                char *word = (char *)malloc((tokLen + 1) * sizeof(char));
                strncpy(word, wordStart, tokLen);
                word[tokLen] = '\0';
                push_ls(tok->toks, word);
                tokLen = 0;

                push_lui16(&(tok->isStringL), 1);
            }
            else
            {
                tokLen++;
            }
        }
    }

    return tok;
}

typedef struct
{
    long *offset;
    size_t size;
    size_t capacity;
} list_byteOffset;

void pushOffset(list_byteOffset *list, long val)
{
    if (list->size == list->capacity)
    {
        list->capacity *= 2;
        list->offset = (long *)realloc(list->offset, list->capacity * sizeof(long));
    }
    list->offset[list->size] = val;
    list->size++;
}

typedef struct
{
    token *ftoken;
    list_byteOffset offsets;
    int num;
    long loopStart;
} line;

size_t getLineSize(line *fline)
{
    return fline->ftoken->toks->size;
}

void initOffsets(line *line)
{
    list_byteOffset *list = &(line->offsets);
    list->size = 1;
    list->capacity = 1;
    list->offset = (long *)malloc(sizeof(long));
    pushOffset(list, 0);
}

void freeOffsets(list_byteOffset *list)
{
    free(list->offset);
}