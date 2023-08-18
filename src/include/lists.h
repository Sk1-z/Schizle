#pragma once

#include <stdlib.h>
#include <string.h>

//  String List and methods
typedef struct
{
    char **data;
    size_t size;
    size_t capacity;
} list_s;

void initList_s(list_s *list)
{
    list->data = (char **)malloc(2 * sizeof(char *));
    list->size = 0;
    list->capacity = 2;
}

void free_s(list_s *list)
{
    for (size_t i = 0; i < list->size; ++i)
    {
        free(list->data[i]);
    }
    free(list->data);
}

void clear_s(list_s *list)
{
    for (size_t i = 0; i < list->size; ++i)
    {
        free(list->data[i]);
    }

    list->capacity = 2;
    list->data = (char **)realloc(list->data, list->capacity * sizeof(char *));

    list->size = 0;
}

void listPush_s(list_s *list, char *val)
{
    if (list->size == list->capacity)
    {
        list->capacity *= 1.5;
        list->data = (char **)realloc(list->data, list->capacity * sizeof(char *));
    }
    list->data[list->size] = (char *)malloc((strlen(val) + 1) * sizeof(char *));
    strcpy(list->data[list->size], val);
    list->size++;
}

void listCat_s(list_s *list, size_t i, char *string)
{
    list->data[i] = (char *)realloc(list->data[i], (strlen(list->data[i]) + strlen(string) + 2) * sizeof(char));
    strcat(list->data[i], string);
}

char *listGet_s(list_s *list, size_t i)
{
    return list->data[i];
}

size_t listGetIndex_s(list_s *list, char *string)
{
    return 0;
}