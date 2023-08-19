#include <stdio.h>
#include <string.h>

#include "include/utils.h"
#include "include/lists.h"
#include "include/tok.h"
#include "include/var.h"

#define CHARACTER_LIMIT 256

int main(int argc, char *argv[])
{
    if (argv[3])
    {
        printf("started %s\n", argv[3]);
    }
    else
    {

        printf("started %s\n", argv[2]);
    }
    // Initialize reading variables
    int noWarning = 1;
    char charBuf[CHARACTER_LIMIT];
    // Runtime booleans
    int falseExp = 0;
    int looping = 0;

    // Program list
    list_s argBuf;

    list_s varBuf;
    lookupT varTable;

    // Variable list
    list_s chars;
    // Command line parsing
    if (argc == 1)
    {
        printf("error: expected argument <mode> got NULL\nUsage: Schizle <mode> <file-path> <program_name?>\n");
        return 1;
    }
    else if (argc == 2)
    {
        printf("error: expected argument <file-path> got NULL\nUsage: Schizle <mode> <file-path> <program_name?>\n");
        if (strcmp(argv[1], "run") != 0)
        {
            printf("error: expected valid value for <mode> got '%s'\nUsage: argv[1] mode : 'run'", argv[1]);
        }
        return 1;
    }
    else if (argc == 3 || argc == 4)
    {
        // File entry point
        if (!strcmp(argv[1], "run"))
        {
            FILE *ski = fopen(argv[2], "r");
            if (ski != NULL)
            {
                lookupT test;
                initTable(&test);
                // pre processing
                if (argv[3])
                {
                    printf("%s out:\n\n", argv[3]);
                    initProgram(&varBuf, &chars, &argBuf, &varTable, argv[3]);
                }
                else
                {
                    printf("%s out:\n\n", argv[2]);
                    initProgram(&varBuf, &chars, &argBuf, &varTable, argv[2]);
                }

                // parse file
                line fline;
                initOffsets(&fline);

                while (fgets(charBuf, sizeof(charBuf), ski) != NULL)
                {
                    fline.toks = tokenize(charBuf);
                    if (!looping)
                    {
                        pushOffset(&(fline.offsets), ftell(ski));
                    }

                    fline.num++;
                    free_s(fline.toks);
                }

                // finish parsing
                if (noWarning)
                {
                    printf("\ninterpreter exited with SS code: 0\n");
                }
                else
                {
                    printf("\ninterpreter exited with SS warning:%i", noWarning);
                }
                // clean up
                free_s(&chars);

                free_s(&argBuf);
                free_s(&varBuf);
                freeTable(&varTable);

                freeOffsets(&(fline.offsets));

                fclose(ski);

                if (argv[3])
                {
                    printf("exited %s with code: 0", argv[3]);
                }
                else
                {

                    printf("exited %s with code: 0", argv[2]);
                }
                return 0;
            }
            else
            {
                printf("error: File not found at %s", argv[2]);
                return 1;
            }
        }
        else
        {
            printf("error: expected valid value for <mode> got '%s'\nUsage: argv[1] mode : 'run'", argv[1]);
            return 1;
        }
    }
    else if (argc > 4)
    {
        printf("error: Too many arguments\nUsage: Schizle <mode> <file-path>");
        return 1;
    }
}