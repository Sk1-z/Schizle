#include "lists.h"

#include <stdlib.h>

struct functionSig
{
    size_t numArgs;
    size_t *argID;
};

// signature list
typedef struct
{
    struct functionSig *data;
    size_t size;
    size_t capacity;
} list_functionSig;

void init_sig(list_functionSig *list)
{
    list->data = (struct functionSig *)malloc(0);
    list->size = 0;
    list->capacity = 0;
}

void free_sig(list_functionSig *list)
{
    for (int i = 0; i < list->size; i++)
    {
        free(list->data[i].argID);
    }
    free(list->data);
}

void push_sig(list_functionSig *list, struct functionSig val)
{
    if (list->size == list->capacity)
    {
        list->capacity = (size_t)((double)list->capacity * 1.5 + 1);
        list->data = (struct functionSig *)realloc(list->data, list->capacity * sizeof(struct functionSig));
    }
    list->data[list->size] = val;
    list->size++;
}

struct functionSig get_sig(list_functionSig *list, size_t i)
{
    return list->data[i];
}

struct module
{
    list_ls functionNames;
    list_functionSig functionSignatures;
};

struct module_list
{
    list_ls moduleNames;
    list_lui16 moduleID;
    struct module modules_list[16];
};

void freeModules(struct module_list *list)
{
    free_ls(&(list->moduleNames));
    for (int i = 0; i < 16; i++)
    {
        free_ls(&(list->modules_list[i].functionNames));
        free_sig(&(list->modules_list[i].functionSignatures));
    }
}