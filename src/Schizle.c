#include <stdio.h>
#include <string.h>

#include "../std/std.h"

#include "include/throwE.h"
#include "include/throwW.h"
#include "include/utils.h"
#include "include/lists.h"
#include "include/tok.h"
#include "include/var.h"

#define CHARACTER_LIMIT 256

#define ERROR_HERE printf("ERROR_HERE\n");
#define WARNING_HERE printf("WARNING_HERE\n");
#define PRINTTYPEDB(str) // printf("%s\n", str)

int main(int argc, char *argv[])
{
    // Initialize reading variables
    int noWarning = 1;
    char charBuf[CHARACTER_LIMIT];

    line fline;

    int exitCode = 0;
    // Runtime booleans
    int program = 0;
    int falseExp = 0;
    int looping = 0;
    int paramInit = 0;
    // Program list
    list_s *argBuf;

    list_s varBuf;
    lookupT varTable;
    // Variable list
    list_s chars;
    // Command line parsing
    if (argc == 1)
    {
        exitCode = 2;
        throwE(exitCode, argv);
        goto exit;
    }
    else if (argc == 2)
    {
        exitCode = 4;
        throwE(exitCode, argv);
        goto exit;
    }
    else if (argc == 3 || argc == 4)
    {
        // File entry point
        if (!strcmp(argv[1], "run"))
        {
            FILE *ski = fopen(argv[2], "r");
            if (ski != NULL)
            {
                program = 1;
                // pre processing
                if (argc == 4)
                {
                    printf("%s out:\n\n", argv[3]);
                    initProgram(&chars, &varBuf, argBuf, &varTable, argv[3]);
                }
                else
                {
                    printf("%s out:\n\n", argv[2]);
                    initProgram(&chars, &varBuf, argBuf, &varTable, argv[2]);
                }

                // parse file

                initOffsets(&fline);

                while (fgets(charBuf, sizeof(charBuf), ski) != NULL)
                {
                    fline.toks = tokenize(charBuf);
                    if (!looping)
                    {
                        pushOffset(&(fline.offsets), ftell(ski));
                    }

                    fline.num++;

                    if (!strcmp(listGet_s(fline.toks, 0), "\n"))
                    {
                        PRINTTYPEDB("newline");
                    }
                    else if (!strcmp(listGet_s(fline.toks, 0), "_"))
                    {
                        PRINTTYPEDB("comment");
                    }
                    else if (mstrcmp(listGet_s(fline.toks, 0), (char *[]){"end", "end\n", NULL}))
                    {
                        PRINTTYPEDB("end");
                    }
                    else if (!strcmp(listGet_s(fline.toks, 0), "copy"))
                    {
                        PRINTTYPEDB("import");
                    }
                    else if (!strcmp(listGet_s(fline.toks, 0), "new"))
                    {
                        PRINTTYPEDB("declaration");
                    }
                    else if (!strcmp(listGet_s(fline.toks, 0), "new[]"))
                    {
                        PRINTTYPEDB("group declaration");
                    }
                    else if (!strcmp(listGet_s(fline.toks, 0), "if"))
                    {
                        PRINTTYPEDB("if");
                    }
                    else if (!strcmp(listGet_s(fline.toks, 0), "elseif"))
                    {
                        PRINTTYPEDB("elseif");
                    }
                    else if (!strcmp(listGet_s(fline.toks, 0), "else"))
                    {
                        PRINTTYPEDB("else");
                    }
                    else if (!strcmp(listGet_s(fline.toks, 0), "dur"))
                    {
                        PRINTTYPEDB("loop");
                    }
                    else if (!strcmp(listGet_s(fline.toks, 0), "params"))
                    {
                        PRINTTYPEDB("parameter declaration");

                        if (paramInit)
                        {
                            WARNING_HERE
                        }

                        clear_s(argBuf);
                        for (size_t i = 1; i < fline.toks->size; i++)
                        {
                            listPush_s(argBuf, listGet_s(fline.toks, i));
                        }

                        paramInit = 1;
                    }
                    else if (!strcmp(listGet_s(fline.toks, 0), "call"))
                    {
                        PRINTTYPEDB("function call");

                        if (!paramInit)
                        {
                            ERROR_HERE
                        }

                        paramInit = 0;

                        if (mstrcmp(listGet_s(fline.toks, 1), (char *[]){"cmdl_out", "cmdl_out\n", NULL}))
                        {
                            if (mstrcmp(listGet_s(argBuf, 0), (char *[]){"_name", "_name\n", NULL}))
                            {
                                cmdl_out(listGet_s(&chars, 0));
                            }
                            else
                            {
                                size_t index = listGetIndex_s(&varBuf, listGet_s(argBuf, 0));
                                if (index == 0)
                                {
                                    cmdl_out(listGet_s(argBuf, 0));
                                }
                                else
                                {
                                    size_t *loc = getVarLookUp(&varTable, index);
                                    if (loc[0] != 5)
                                    {
                                        ERROR_HERE
                                    }

                                    char *string = listGet_s(&chars, loc[1]);
                                    cmdl_out(string);
                                }
                            }
                        }
                        else
                        {
                            ERROR_HERE
                        }
                    }
                    else
                    {
                        printf("expected valid line signature got %s in line %d\n", listGet_s(fline.toks, 0), fline.num);
                        // free_s(fline.toks);
                        // break;
                    }

                    free_s(fline.toks);
                }

                // printf("%d", fline.num);
                // clean up

                fclose(ski);

                goto exit;
            }
            else
            {
                exitCode = 5;
                throwE(exitCode, argv);
                goto exit;
            }
        }
        else
        {
            exitCode = 3;
            throwE(exitCode, argv);
            goto exit;
        }
    }
    else if (argc > 4)
    {
        exitCode = 1;
        throwE(exitCode, argv);
        goto exit;
    }
exit:

    if (program == 1)
    {
        free_s(&chars);

        free_s(argBuf);
        free_s(&varBuf);
        freeTable(&varTable);

        freeOffsets(&(fline.offsets));
        printf("\n\nsuccessfully freed memory");
    }

    if (argc == 4)
    {
        printf("\ninterpreter exited %s with code: %d", argv[3], exitCode);
    }
    else if (argc == 3)
    {

        printf("\ninterpreter exited %s with code: %d", argv[2], exitCode);
    }
    else
    {
        printf("\ninterpreter exited with code: %d", exitCode);
    }
}