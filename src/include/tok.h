#include <stdlib.h>
#include <string.h>

#include "lists.h"

// Tokenize string by whitespace and quotes

list_s *tokenize(const char *string)
{
    list_s *brokenString = (list_s *)malloc(sizeof(list_s));
    initList_s(brokenString);

    size_t strLen = strlen(string);
    size_t tokLen = 0;
    char *wordStart = (char *)string;

    for (size_t i = 0; i <= strLen; i++)
    {
        if (string[i] == ' ' || string[i] == '\t' || string[i] == '\n' || string[i] == '\0')
        {
            if (tokLen > 0)
            {
                char *word = (char *)malloc((tokLen + 1) * sizeof(char));
                strncpy(word, wordStart, tokLen);
                word[tokLen] = '\0';
                listPush_s(brokenString, word);
                tokLen = 0;
            }
            wordStart = (char *)string + i + 1;
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
            if (string[i] == '\'')
            {
                char *word = (char *)malloc((tokLen + 1) * sizeof(char));
                strncpy(word, wordStart, tokLen);
                word[tokLen] = '\0';
                listPush_s(brokenString, word);
                tokLen = 0;
            }
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
            if (string[i] == '\"')
            {
                char *word = (char *)malloc((tokLen + 1) * sizeof(char));
                strncpy(word, wordStart, tokLen);
                word[tokLen] = '\0';
                listPush_s(brokenString, word);
                tokLen = 0;
            }
        }
        else
        {
            tokLen++;
        }
    }

    return brokenString;
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
    list_s *toks;
    list_byteOffset offsets;
    int num;
    long loopStart;
} line;

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