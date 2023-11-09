#pragma once

#define _NAT16_LIMIT_ 0xffff
#define _NAT64_LIMIT_ 0xffffffffffffffff
#define _INT32_LIMIT_ 0x0fffffff
#define _INT64_LIMIT_ 0x0fffffffffffffff
#define _FRAC32_LIMIT_ __FLT_MAX__
#define _FRAC64_LIMIT_ __DBL_MAX__

// 0 = UNDEF
// 1 = state
// 2 = nat
// 3 = nat64
// 4 = int
// 5 = int64
// 6 = frac
// 7 = frac64
// 8 = text

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

// [0] = id [1] = index
size_t *getVarLookUp(lookupT *table, size_t i)
{
    // get variable's id and index by index in table of identifiers
    size_t *returnT = (size_t *)malloc(2 * sizeof(size_t));
    returnT[0] = table->tableData[i][0];
    returnT[1] = table->tableData[i][1];
    return returnT;
}

size_t autoNumericalType(char *val)
{
    size_t len = strlen(val);
    char *end;

    if (isdigit(val[0]))
    {
        for (int i = 0; i < len; i++)
        {
            if (val[i] == '.')
            {
                double d = strtod(val, &end);

                if (*end == '\0')
                {
                    if (d <= _FRAC32_LIMIT_ && d >= -_FRAC32_LIMIT_)
                    {
                        return 6;
                    }
                    else if (d < _FRAC64_LIMIT_ && d > -_FRAC64_LIMIT_)
                    {
                        return 7;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    return 0;
                }
            }
        }

        unsigned long long ull = strtoull(val, &end, 10);
        if (*end == '\0')
        {
            if (ull <= _NAT16_LIMIT_)
            {
                return 2;
            }
            else if (ull < _NAT64_LIMIT_)
            {
                return 3;
            }
            else
            {
                return 0;
            }
        }
        else
        {
            return 0;
        }
    }
    else if (val[0] == '-')
    {
        long long ll = strtoll(val, &end, 10);

        if (*end == '\0')
        {
            if (ll >= -_INT32_LIMIT_)
            {
                return 4;
            }
            else if (ll > -_INT64_LIMIT_)
            {
                return 5;
            }
            else
            {
                return 0;
            }
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}
