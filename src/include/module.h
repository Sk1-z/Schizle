#pragma once

#include "lists.h"

#include <stdio.h>
#include <stdlib.h>

struct functionSig
{
    size_t numArgs;
    list_lui16 argID;
    size_t (*call)();
};

void init_sig(struct functionSig *sig)
{
    init_lui16(&(sig->argID));
}

// signature list
typedef struct
{
    struct functionSig **data;
    size_t size;
    size_t capacity;
} list_functionSig;

void init_lsig(list_functionSig *list)
{
    list->data = (struct functionSig **)malloc(0);
    list->size = 0;
    list->capacity = 0;
}

void free_lsig(list_functionSig *list)
{
    for (size_t i = 0; i < list->size; i++)
    {
        free_lui16(&(list->data[i]->argID));
    }
    free(list->data);
}

void push_sig(list_functionSig *list, struct functionSig *val)
{
    if (list->size == list->capacity)
    {
        list->capacity = (size_t)((double)list->capacity * 1.5 + 1);
        list->data = (struct functionSig **)realloc(list->data, list->capacity * sizeof(struct functionSig *));
    }
    list->data[list->size] = val;
    list->size++;
}

struct functionSig *get_sig(list_functionSig *list, size_t i)
{
    return list->data[i];
}

struct module
{
    list_ls functionNames;
    list_functionSig functionSignatures;
};

struct module_data
{
    list_ls moduleNames;
    list_lui16 moduleID;
    struct module **data;
    size_t size;
    size_t capacity;
};

void init_lmod(struct module_data *list)
{
    init_ls(&(list->moduleNames));
    init_lui16(&(list->moduleID));
    list->data = (struct module **)malloc(16 * sizeof(struct module *));
    list->size = 0;
    list->capacity = 16;
}

struct module *get_module(struct module_data *list, size_t i)
{
    return list->data[i];
}

void push_module(struct module_data *list, struct module *val)
{
    if (list->size == list->capacity)
    {
        list->capacity = (size_t)((double)list->capacity * 1.5 + 1);
        list->data = (struct module **)realloc(list->data, list->capacity * sizeof(struct module *));
    }
    list->data[list->size] = val;
    list->size++;
}

void freeModules(struct module_data *list)
{
    free_ls(&(list->moduleNames));
    free_lui16(&(list->moduleID));
    // printf("\nList size: %zu\n", list->size);
    // for (int i = 0; i < list->size; i++)
    // {
    //     printf("\nLoop: %d %zu\n", i, list->data[0]->functionNames.size);
    //     // free_ls(&(list->data[i]->functionNames));
    //     printf("\nNames: %d\n", i);
    //     // free_lsig(&(list->data[i]->functionSignatures));
    //     printf("\nSigs: %d\n", i);
    // }
    // printf("\nSize: %zu\n", list->size);
}
