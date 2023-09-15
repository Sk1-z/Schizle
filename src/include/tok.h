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
    size_t tokNum = 0;
    char *wordStart = (char *)string;

    int j = 0;
    while (string[j] != '\0')
    {
        if (string[j] == '\\' && string[j + 1] == 'n')
        {
            string[j] = ' ';
            string[j + 1] = '\n';
        }
        else if (string[j] == '\\' && string[j + 1] == 't')
        {
            string[j] = ' ';
            string[j + 1] = '\t';
        }
        else if (string[j] == '\\' && string[j + 1] == 'b')
        {
            string[j] = ' ';
            string[j + 1] = '\b';
        }
        j++;
    }

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
            push_ls(tok->toks, "\n");
            push_lui16(&(tok->isStringL), 0);
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
                push_ls(tok->toks, "mFunc");
                push_lui16(&(tok->isStringL), 0);
                wordStart = (char *)string + i + 1;
            }
            else if (string[i + 1] != ':' && string[i - 1] != ':')
            {
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
                push_ls(tok->toks, "params");
                push_lui16(&(tok->isStringL), 0);
                wordStart = (char *)string + i + 1;
            }
            else if (string[i + 1] == '<')
            {
            }
        }
        else if (string[i] == '[' || string[i] == '<')
        {
        }
        else if (string[i] == ']' || string[i] == '>')
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
            push_ls(tok->toks, "@");
            push_lui16(&(tok->isStringL), 0);
            wordStart = (char *)string + i + 1;
        }
        else if (string[i] == '{')
        {
            push_ls(tok->toks, "eval");
            push_lui16(&(tok->isStringL), 0);
            i++;
            wordStart = (char *)string + i;
            while (string[i] != '}' && string[i] != '\0')
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
        else if (string[i] == '(')
        {
            push_ls(tok->toks, "cond");
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