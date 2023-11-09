#pragma once

// char[] list
typedef struct
{
    char **data;
    size_t size;
    size_t capacity;
} list_ls;

void init_ls(list_ls *list)
{
    list->data = (char **)malloc(0);
    list->size = 0;
    list->capacity = 0;
}

void free_ls(list_ls *list)
{
    for (size_t i = 0; i < list->size; ++i)
    {
        free(list->data[i]);
    }
    free(list->data);
}

void clear_ls(list_ls *list)
{
    for (size_t i = 0; i < list->size; ++i)
    {
        free(list->data[i]);
    }

    list->size = 0;
}

void push_ls(list_ls *list, char *val)
{
    if (list->size == list->capacity)
    {
        list->capacity = (size_t)((double)list->capacity * 1.5 + 1);
        list->data = (char **)realloc(list->data, list->capacity * sizeof(char *));
    }
    list->data[list->size] = (char *)malloc((strlen(val) + 1) * sizeof(char *));
    memset(list->data[list->size], '\0', strlen(val) + 1);
    strcpy(list->data[list->size], val);
    list->size++;
}

void cat_ls(list_ls *list, size_t i, char *string)
{
    list->data[i] = (char *)realloc(list->data[i], (strlen(list->data[i]) + strlen(string) + 2) * sizeof(char));
    strcat(list->data[i], string);
}

char *get_ls(list_ls *list, size_t i)
{
    return list->data[i];
}

void set_ls(list_ls *list, size_t i, char *val)
{
    list->data[i] = (char *)realloc(list->data[i], ((strlen(val) + 1) * sizeof(char)));
    strcpy(list->data[i], val);
}

size_t getSize_ls(list_ls *list)
{
    return list->size;
}

size_t getIndex_ls(list_ls *list, char *string)
{
    // printf("\n");
    for (size_t i = 0; i < list->size; ++i)
    {
        // printf("compare: %s to: %s\n", string, list->data[i]);
        if (!strcmp(string, list->data[i]))
        {
            return i;
        }
    }
    return 0;
    // Name of the program
}

// unsigned i16 list
typedef struct
{
    uint16_t *data;
    size_t size;
    size_t capacity;
} list_lui16;

void init_lui16(list_lui16 *list)
{
    list->data = (uint16_t *)malloc(0);
    list->size = 0;
    list->capacity = 0;
}

void free_lui16(list_lui16 *list)
{
    free(list->data);
}

void clear_lui16(list_lui16 *list)
{
    for (size_t i = 0; i < list->size; ++i)
    {
        list->data[i] = 0;
    }

    list->size = 0;
}

void push_lui16(list_lui16 *list, uint16_t val)
{
    if (list->size == list->capacity)
    {
        list->capacity = (size_t)((double)list->capacity * 1.5 + 1);
        list->data = (uint16_t *)realloc(list->data, list->capacity * sizeof(uint16_t));
    }
    list->data[list->size] = val;
    list->size++;
}

void pushBool_lui16(list_lui16 *list, int val)
{
    if (list->size == list->capacity)
    {
        list->capacity = (size_t)((double)list->capacity * 1.5 + 1);
        list->data = (uint16_t *)realloc(list->data, list->capacity * sizeof(uint16_t));
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

uint16_t get_lui16(list_lui16 *list, size_t i)
{
    return list->data[i];
}

void set_lui16(list_lui16 *list, size_t i, uint16_t val)
{
    list->data[i] = val;
}

// unsigned i64 list
typedef struct
{
    uint64_t *data;
    size_t size;
    size_t capacity;
} list_lui64;

void init_lui64(list_lui64 *list)
{
    list->data = (uint64_t *)malloc(0);
    list->size = 0;
    list->capacity = 0;
}

void free_lui64(list_lui64 *list)
{
    free(list->data);
}

void clear_lui64(list_lui64 *list)
{
    for (size_t i = 0; i < list->size; ++i)
    {
        list->data[i] = 0;
    }

    list->size = 0;
}

void push_lui64(list_lui64 *list, uint64_t val)
{
    if (list->size == list->capacity)
    {
        list->capacity = (size_t)((double)list->capacity * 1.5 + 1);
        list->data = (uint64_t *)realloc(list->data, list->capacity * sizeof(uint64_t));
    }
    list->data[list->size] = val;
    list->size++;
}

uint64_t get_lui64(list_lui64 *list, size_t i)
{
    return list->data[i];
}

void set_lui64(list_lui64 *list, size_t i, uint64_t val)
{
    list->data[i] = val;
}

// signed i32 list
typedef struct
{
    int32_t *data;
    size_t size;
    size_t capacity;
} list_lsi32;

void init_lsi32(list_lsi32 *list)
{
    list->data = (int32_t *)malloc(0);
    list->size = 0;
    list->capacity = 0;
}

void free_lsi32(list_lsi32 *list)
{
    free(list->data);
}

void clear_lsi32(list_lsi32 *list)
{
    for (size_t i = 0; i < list->size; ++i)
    {
        list->data[i] = 0;
    }

    list->size = 0;
}

void push_lsi32(list_lsi32 *list, int32_t val)
{
    if (list->size == list->capacity)
    {
        list->capacity = (size_t)((double)list->capacity * 1.5 + 1);
        list->data = (int32_t *)realloc(list->data, list->capacity * sizeof(int32_t));
    }
    list->data[list->size] = val;
    list->size++;
}

int32_t get_lsi32(list_lsi32 *list, size_t i)
{
    return list->data[i];
}

void set_lsi32(list_lsi32 *list, size_t i, int32_t val)
{
    list->data[i] = val;
}

// signed i64 list
typedef struct
{
    int64_t *data;
    size_t size;
    size_t capacity;
} list_lsi64;

void init_lsi64(list_lsi64 *list)
{
    list->data = (int64_t *)malloc(0);
    list->size = 0;
    list->capacity = 0;
}

void free_lsi64(list_lsi64 *list)
{
    free(list->data);
}

void clear_lsi64(list_lsi64 *list)
{
    for (size_t i = 0; i < list->size; ++i)
    {
        list->data[i] = 0;
    }

    list->size = 0;
}

void push_lsi64(list_lsi64 *list, int64_t val)
{
    if (list->size == list->capacity)
    {
        list->capacity = (size_t)((double)list->capacity * 1.5 + 1);
        list->data = (int64_t *)realloc(list->data, list->capacity * sizeof(int64_t));
    }
    list->data[list->size] = val;
    list->size++;
}

int64_t get_lsi64(list_lsi64 *list, size_t i)
{
    return list->data[i];
}

void set_lsi64(list_lsi64 *list, size_t i, int64_t val)
{
    list->data[i] = val;
}

// float
typedef struct
{
    float *data;
    size_t size;
    size_t capacity;
} list_lf32;

void init_lf32(list_lf32 *list)
{
    list->data = (float *)malloc(0);
    list->size = 0;
    list->capacity = 0;
}

void free_lf32(list_lf32 *list)
{
    free(list->data);
}

void clear_lf32(list_lf32 *list)
{
    for (size_t i = 0; i < list->size; ++i)
    {
        list->data[i] = 0;
    }

    list->size = 0;
}

void push_lf32(list_lf32 *list, float val)
{
    if (list->size == list->capacity)
    {
        list->capacity = (size_t)((double)list->capacity * 1.5 + 1);
        list->data = (float *)realloc(list->data, list->capacity * sizeof(float));
    }
    list->data[list->size] = val;
    list->size++;
}

float get_lf32(list_lf32 *list, size_t i)
{
    return list->data[i];
}

void set_lf32(list_lf32 *list, size_t i, float val)
{
    list->data[i] = val;
}

// double
typedef struct
{
    double *data;
    size_t size;
    size_t capacity;
} list_ld64;

void init_ld64(list_ld64 *list)
{
    list->data = (double *)malloc(0);
    list->size = 0;
    list->capacity = 0;
}

void free_ld64(list_ld64 *list)
{
    free(list->data);
}

void clear_ld64(list_ld64 *list)
{
    for (size_t i = 0; i < list->size; ++i)
    {
        list->data[i] = 0;
    }

    list->size = 0;
}

void push_ld64(list_ld64 *list, double val)
{
    if (list->size == list->capacity)
    {
        list->capacity = (size_t)((double)list->capacity * 1.5 + 1);
        list->data = (double *)realloc(list->data, list->capacity * sizeof(double));
    }
    list->data[list->size] = val;
    list->size++;
}

double get_ld64(list_ld64 *list, size_t i)
{
    return list->data[i];
}

void set_ld64(list_ld64 *list, size_t i, double val)
{
    list->data[i] = val;
}
