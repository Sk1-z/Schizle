#include <stdlib.h>
#include <string.h>

#include "lists.h"
#include "var.h"

void initProgram(list_s *chars, list_s *varNameList, list_s *argNameList, lookupT *varTable, char *pName)
{
    initList_s(varNameList);
    initList_s(chars);

    listPush_s(varNameList, "_name");
    listPush_s(chars, pName);

    initTable(varTable);
    pushTable(varTable, 5, 0);

    initList_s(argNameList);
}

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