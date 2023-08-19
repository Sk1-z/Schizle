#pragma once

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// char[] list
typedef struct
{
    char **data;
    size_t size;
    size_t capacity;
} list_s;

void initList_s(list_s *list)
{
    list->data = (char **)malloc(0);
    list->size = 0;
    list->capacity = 0;
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

    list->size = 0;
}

void listPush_s(list_s *list, char *val)
{
    if (list->size == list->capacity)
    {
        list->capacity = (size_t)((double)list->capacity * 1.5 + 1);
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
    for (size_t i = 0; i < list->size; ++i)
    {
        if (!strcmp(string, list->data[i]))
        {
            return i;
        }
    }
    return 0;
    // Name of the program
}

// signed i32 list
typedef struct
{
    int32_t *data;
    size_t size;
    size_t capacity;
} list_si32;

void initList_si32(list_si32 *list)
{
    list->data = (int32_t *)malloc(0);
    list->size = 0;
    list->capacity = 0;
}

void free_si32(list_si32 *list)
{
    free(list->data);
}

void clear_si32(list_si32 *list)
{
    for (size_t i = 0; i < list->size; ++i)
    {
        list->data[i] = 0;
    }

    list->size = 0;
}

void listPush_si32(list_si32 *list, int32_t val)
{
    if (list->size == list->capacity)
    {
        list->capacity = (size_t)((double)list->capacity * 1.5 + 1);
        list->data = (int32_t *)realloc(list->data, list->capacity * sizeof(int32_t));
    }
    list->data[list->size] = val;
    list->size++;
}
