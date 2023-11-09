#pragma once

#include "../src/include/program.h"
// File module
FILE *currentStream = NULL;
char *openPath;
int fileOpen = 0;
int canWrite = -1;

#define GET_FILE(name)                                                                                                 \
    push_ls(&moduleNames, name);                                                                                       \
    push_lui16(&moduleID, 3);                                                                                          \
                                                                                                                       \
    struct module fs;                                                                                                  \
    init_lsig(&(fs.functionSignatures));                                                                               \
    init_ls(&(fs.functionNames));                                                                                      \
                                                                                                                       \
    struct functionSig create_sig;                                                                                     \
    init_sig(&create_sig);                                                                                             \
    create_sig.numArgs = 1;                                                                                            \
    push_lui16(&(create_sig.argID), 9);                                                                                \
    create_sig.call = create;                                                                                          \
                                                                                                                       \
    push_sig(&(fs.functionSignatures), &create_sig);                                                                   \
    push_ls(&(fs.functionNames), "create");                                                                            \
                                                                                                                       \
    struct functionSig rename_sig;                                                                                     \
    init_sig(&rename_sig);                                                                                             \
    rename_sig.numArgs = 2;                                                                                            \
    push_lui16(&(rename_sig.argID), 9);                                                                                \
    push_lui16(&(rename_sig.argID), 9);                                                                                \
    rename_sig.call = _rename;                                                                                         \
                                                                                                                       \
    push_sig(&(fs.functionSignatures), &rename_sig);                                                                   \
    push_ls(&(fs.functionNames), "rename");                                                                            \
                                                                                                                       \
    struct functionSig delete_sig;                                                                                     \
    init_sig(&delete_sig);                                                                                             \
    delete_sig.numArgs = 1;                                                                                            \
    push_lui16(&(delete_sig.argID), 9);                                                                                \
    delete_sig.call = delete;                                                                                          \
                                                                                                                       \
    push_sig(&(fs.functionSignatures), &delete_sig);                                                                   \
    push_ls(&(fs.functionNames), "delete");                                                                            \
                                                                                                                       \
    struct functionSig open_sig;                                                                                       \
    init_sig(&open_sig);                                                                                               \
    open_sig.numArgs = 1;                                                                                              \
    push_lui16(&(open_sig.argID), 9);                                                                                  \
    open_sig.call = open;                                                                                              \
                                                                                                                       \
    push_sig(&(fs.functionSignatures), &open_sig);                                                                     \
    push_ls(&(fs.functionNames), "open");                                                                              \
                                                                                                                       \
    struct functionSig change_mode_sig;                                                                                \
    init_sig(&change_mode_sig);                                                                                        \
    change_mode_sig.numArgs = 0;                                                                                       \
    change_mode_sig.call = change_mode;                                                                                \
                                                                                                                       \
    push_sig(&(fs.functionSignatures), &change_mode_sig);                                                              \
    push_ls(&(fs.functionNames), "change_mode");                                                                       \
                                                                                                                       \
    struct functionSig reset_sig;                                                                                      \
    init_sig(&reset_sig);                                                                                              \
    reset_sig.numArgs = 0;                                                                                             \
    reset_sig.call = reset;                                                                                            \
                                                                                                                       \
    push_sig(&(fs.functionSignatures), &reset_sig);                                                                    \
    push_ls(&(fs.functionNames), "reset");                                                                             \
                                                                                                                       \
    struct functionSig write_sig;                                                                                      \
    init_sig(&write_sig);                                                                                              \
    write_sig.numArgs = 1;                                                                                             \
    push_lui16(&(write_sig.argID), 9);                                                                                 \
    write_sig.call = write;                                                                                            \
                                                                                                                       \
    push_sig(&(fs.functionSignatures), &write_sig);                                                                    \
    push_ls(&(fs.functionNames), "write");                                                                             \
                                                                                                                       \
    struct functionSig read_sig;                                                                                       \
    init_sig(&read_sig);                                                                                               \
    read_sig.numArgs = 1;                                                                                              \
    push_lui16(&(read_sig.argID), 8);                                                                                  \
    read_sig.call = read;                                                                                              \
                                                                                                                       \
    push_sig(&(fs.functionSignatures), &read_sig);                                                                     \
    push_ls(&(fs.functionNames), "read");                                                                              \
                                                                                                                       \
    struct functionSig readln_sig;                                                                                     \
    init_sig(&readln_sig);                                                                                             \
    readln_sig.numArgs = 1;                                                                                            \
    push_lui16(&(readln_sig.argID), 8);                                                                                \
    readln_sig.call = readln;                                                                                          \
                                                                                                                       \
    push_sig(&(fs.functionSignatures), &readln_sig);                                                                   \
    push_ls(&(fs.functionNames), "readln");                                                                            \
                                                                                                                       \
    struct functionSig close_sig;                                                                                      \
    init_sig(&close_sig);                                                                                              \
    close_sig.numArgs = 0;                                                                                             \
    close_sig.call = close;                                                                                            \
                                                                                                                       \
    push_sig(&(fs.functionSignatures), &close_sig);                                                                    \
    push_ls(&(fs.functionNames), "close");                                                                             \
                                                                                                                       \
    modules[modCount] = fs;                                                                                            \
    modCount++;

size_t create()
{
    char *path;
    if (get_lui16(&isStringArg, 0))
    {
        path = get_ls(&argBuf, 0);
    }
    else
    {
        path = GET_TEXT(getVarLookUp(&varTable, getIndex_ls(&varBuf, get_ls(&argBuf, 0)))[1]);
    }

    FILE *temp = fopen(path, "r");
    if (temp == NULL)
    {
        temp = fopen(path, "w");
        if (temp == NULL)
        {
            return 30;
        }
        else
        {
            fclose(temp);
        }
    }
    else
    {
        return 31;
    }

    return 0;
}

size_t _rename()
{
    char *path;
    if (get_lui16(&isStringArg, 0))
    {
        path = get_ls(&argBuf, 0);
    }
    else
    {
        path = GET_TEXT(getVarLookUp(&varTable, getIndex_ls(&varBuf, get_ls(&argBuf, 0)))[1]);
    }

    char *name;
    if (get_lui16(&isStringArg, 1))
    {
        name = get_ls(&argBuf, 1);
    }
    else
    {
        name = GET_TEXT(getVarLookUp(&varTable, getIndex_ls(&varBuf, get_ls(&argBuf, 1)))[1]);
    }

    FILE *temp = fopen(path, "r");
    if (temp == NULL)
    {
        return 29;
    }
    fclose(temp);

    rename(path, name);

    return 0;
}

size_t delete()
{
    char *path;
    if (get_lui16(&isStringArg, 0))
    {
        path = get_ls(&argBuf, 0);
    }
    else
    {
        path = GET_TEXT(getVarLookUp(&varTable, getIndex_ls(&varBuf, get_ls(&argBuf, 0)))[1]);
    }

    FILE *temp = fopen(path, "r");
    if (temp == NULL)
    {
        return 29;
    }
    fclose(temp);

    remove(path);

    return 0;
}

size_t open()
{
    char *path;
    if (get_lui16(&isStringArg, 0))
    {
        path = get_ls(&argBuf, 0);
    }
    else
    {
        path = GET_TEXT(getVarLookUp(&varTable, getIndex_ls(&varBuf, get_ls(&argBuf, 0)))[1]);
    }

    currentStream = fopen(path, "a+");
    openPath = path;
    canWrite = 0;
    fileOpen = 1;

    if (currentStream == NULL)
    {
        return 30;
    }

    return 0;
}

size_t change_mode()
{
    if (!fileOpen)
    {
        return 30;
    }

    long pos = ftell(currentStream);
    fclose(currentStream);
    if (canWrite)
    {
        currentStream = fopen(openPath, "a+");
        canWrite = 0;
    }
    else
    {
        currentStream = fopen(openPath, "w+");
        canWrite = 1;
    }

    fseek(currentStream, pos, SEEK_SET);
    return 0;
}

size_t reset()
{
    if (!fileOpen)
    {
        return 30;
    }

    rewind(currentStream);

    return 0;
}

size_t write()
{
    if (!fileOpen)
    {
        return 30;
    }

    fputs(GET_TEXT(getVarLookUp(&varTable, getIndex_ls(&varBuf, get_ls(&argBuf, 0)))[1]), currentStream);

    return 0;
}

size_t read()
{
    if (!fileOpen)
    {
        return 30;
    }

    fseek(currentStream, 0, SEEK_END);
    long fSize = ftell(currentStream);
    rewind(currentStream);

    char *content = malloc(fSize * sizeof(char));

    fread(content, 1, fSize, currentStream);
    set_ls(&textVal, getVarLookUp(&varTable, getIndex_ls(&varBuf, get_ls(&argBuf, 0)))[1], content);

    return 0;
}

size_t readln()
{
    if (!fileOpen)
    {
        return 30;
    }

    char line[CHARACTER_LIMIT];
    if (fgets(line, sizeof(line), currentStream) != NULL)
    {
        set_ls(&textVal, getVarLookUp(&varTable, getIndex_ls(&varBuf, get_ls(&argBuf, 0)))[1], line);
    }
    else
    {
        set_ls(&textVal, getVarLookUp(&varTable, getIndex_ls(&varBuf, get_ls(&argBuf, 0)))[1], "\0");
    }

    return 0;
}

size_t close()
{
    if (!fileOpen)
    {
        return 30;
    }

    canWrite = -1;
    fileOpen = 0;
    fclose(currentStream);

    return 0;
}