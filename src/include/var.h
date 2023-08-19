#pragma once

#include <stdlib.h>

// 0 = CER
// 1 = state
// 2 = nat
// 3 = sint
// 4 = frac
// 5 = chars

// argv[0] = schizle; argc 1
// argv[1] = run; argc 2
// argv[2] = path; argc 3
// argv[3] = alias; argc 4

// lookup table
typedef struct
{
    size_t **tableData;
    size_t size;
    size_t capacity;
} lookupT;

void initTable(lookupT *table)
{
    table->tableData = (size_t **)malloc(0);
    table->size = 0;
    table->capacity = 0;
}

void freeTable(lookupT *table)
{
    for (size_t i = 0; i < table->size; ++i)
    {
        free(table->tableData[i]);
    }
    free(table->tableData);
}

void pushTable(lookupT *table, size_t typeID, size_t index)
{
    if (table->size == table->capacity)
    {
        table->capacity = (size_t)((double)table->capacity * 1.5 + 1);
        table->tableData = (size_t **)realloc(table->tableData, table->capacity * sizeof(size_t));
    }
    table->tableData[table->size] = (size_t *)malloc(2 * sizeof(size_t));
    table->tableData[table->size][0] = typeID;
    table->tableData[table->size][1] = index;
    table->size++;
}

size_t *getVarLookUp(lookupT *table, size_t i)
{
    // get variable's id and index by index in table of identifiers
    size_t *returnT = (size_t *)malloc(2 * sizeof(size_t));
    returnT[0] = table->tableData[i][0];
    returnT[1] = table->tableData[i][1];
    return returnT;
}
