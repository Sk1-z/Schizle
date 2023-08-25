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

void init_ls(list_s *list)
{
    list->data = (char **)malloc(0);
    list->size = 0;
    list->capacity = 0;
}

void free_ls(list_s *list)
{
    for (size_t i = 0; i < list->size; ++i)
    {
        free(list->data[i]);
    }
    free(list->data);
}

void clear_ls(list_s *list)
{
    for (size_t i = 0; i < list->size; ++i)
    {
        free(list->data[i]);
    }

    list->size = 0;
}

void push_ls(list_s *list, char *val)
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

void cat_ls(list_s *list, size_t i, char *string)
{
    list->data[i] = (char *)realloc(list->data[i], (strlen(list->data[i]) + strlen(string) + 2) * sizeof(char));
    strcat(list->data[i], string);
}

char *get_ls(list_s *list, size_t i)
{
    return list->data[i];
}

size_t getIndex_ls(list_s *list, char *string)
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

void init_lsi32(list_si32 *list)
{
    list->data = (int32_t *)malloc(0);
    list->size = 0;
    list->capacity = 0;
}

void free_lsi32(list_si32 *list)
{
    free(list->data);
}

void clear_lsi32(list_si32 *list)
{
    for (size_t i = 0; i < list->size; ++i)
    {
        list->data[i] = 0;
    }

    list->size = 0;
}

void push_lsi32(list_si32 *list, int32_t val)
{
    if (list->size == list->capacity)
    {
        list->capacity = (size_t)((double)list->capacity * 1.5 + 1);
        list->data = (int32_t *)realloc(list->data, list->capacity * sizeof(int32_t));
    }
    list->data[list->size] = val;
    list->size++;
}

void pushBool_lsi32(list_si32 *list, int val)
{
    if (list->size == list->capacity)
    {
        list->capacity = (size_t)((double)list->capacity * 1.5 + 1);
        list->data = (int32_t *)realloc(list->data, list->capacity * sizeof(int32_t));
    }
    if (val == 1)
    {
        list->data[list->size] = 1;
        list->size++;
    }
    else if (val == 0)
    {
        list->data[list->size] = 0;
        list->size++;
    }
}

int32_t get_lsi32(list_si32 *list, size_t i)
{
    return list->data[i];
}

void set_lsi32(list_si32 *list, size_t i, int32_t val)
{
    list->data[i] = val;
}