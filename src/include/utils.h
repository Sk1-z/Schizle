#include <stdlib.h>
#include <string.h>

#include "lists.h"

int mstrcmp(char *compared, char **compareTo)
{
    for (size_t i = 0; compareTo[i] != NULL; i++)
    {
        if (!strcmp(compared, compareTo[i]))
        {
            return 1;
        }
    }

    return 0;
}
