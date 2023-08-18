#pragma once

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
