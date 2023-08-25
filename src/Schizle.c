#include <stdio.h>
#include <string.h>

#include "../std/std.h"

#include "include/throwError.h"
#include "include/throwWarning.h"
#include "include/utils.h"
#include "include/tok.h"
#include "include/program.h"

#define THROW_ERROR(arg) throwE(arg, argv)

int main(int argc, char *argv[])
{
    // Command line parsing
    if (argc == 1)
    {
        exitCode = 2;
        THROW_ERROR(exitCode);
        goto exit;
    }
    else if (argc == 2)
    {
        exitCode = 4;
        THROW_ERROR(exitCode);
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
                    INIT_PROGRAM(argv[3])
                }
                else
                {
                    printf("%s out:\n\n", argv[2]);
                    INIT_PROGRAM(argv[2])
                }

                // parse file

                initOffsets(&fline);

                while (fgets(lineBuf, sizeof(lineBuf), ski) != NULL)
                {
                    fline.ftoken = tokenize(lineBuf);
                    if (!looping)
                    {
                        pushOffset(&(fline.offsets), ftell(ski));
                    }

                    fline.num++;

                    if (!strcmp(get_ls(fline.ftoken->toks, 0), "\n"))
                    {
                        PRINTTYPEDB("newline");
                    }
                    else if (!strcmp(get_ls(fline.ftoken->toks, 0), "_"))
                    {
                        PRINTTYPEDB("comment");
                    }
                    else if (!strcmp(get_ls(fline.ftoken->toks, 0), "end"))
                    {
                        PRINTTYPEDB("end");
                    }
                    else if (!strcmp(get_ls(fline.ftoken->toks, 0), "copy"))
                    {
                        PRINTTYPEDB("import");
                    }
                    else if (!strcmp(get_ls(fline.ftoken->toks, 0), "new"))
                    {
                        PRINTTYPEDB("declaration");
                    }
                    else if (!strcmp(get_ls(fline.ftoken->toks, 0), "new[]"))
                    {
                        PRINTTYPEDB("group declaration");
                    }
                    else if (!strcmp(get_ls(fline.ftoken->toks, 0), "if"))
                    {
                        PRINTTYPEDB("if");
                    }
                    else if (!strcmp(get_ls(fline.ftoken->toks, 0), "elseif"))
                    {
                        PRINTTYPEDB("elseif");
                    }
                    else if (!strcmp(get_ls(fline.ftoken->toks, 0), "else"))
                    {
                        PRINTTYPEDB("else");
                    }
                    else if (!strcmp(get_ls(fline.ftoken->toks, 0), "dur"))
                    {
                        PRINTTYPEDB("loop");
                    }
                    else if (!strcmp(get_ls(fline.ftoken->toks, 0), "params"))
                    {
                        PRINTTYPEDB("parameter declaration");

                        if (paramInit)
                        {
                            WARNING_HERE
                        }

                        clear_ls(&argBuf);
                        clear_lsi32(&argIsStringL);
                        for (size_t i = 1; i < fline.ftoken->toks->size; i++)
                        {
                            if (fline.ftoken->isStringL.data[i])
                            {
                                pushBool_lsi32(&argIsStringL, TRUE);
                            }
                            else
                            {
                                pushBool_lsi32(&argIsStringL, FALSE);
                            }

                            push_ls(&argBuf, get_ls(fline.ftoken->toks, i));
                        }

                        paramInit = 1;
                    }
                    else if (!strcmp(get_ls(fline.ftoken->toks, 0), "call"))
                    {
                        PRINTTYPEDB("function call");

                        if (!paramInit)
                        {
                            ERROR_HERE
                        }

                        paramInit = 0;

                        if (!strcmp(get_ls(fline.ftoken->toks, 1), "cmdl_out"))
                        {
                            size_t index = getIndex_ls(&varBuf, get_ls(&argBuf, 0));
                            if (get_lsi32(&argIsStringL, 0))
                            {
                                printf(get_ls(&argBuf, 0));
                            }
                            else if (index != 0 || !strcmp(get_ls(&argBuf, 0), "_name"))
                            {
                                size_t *loc = getVarLookUp(&varTable, index);
                                switch (loc[0])
                                {
                                case 0:
                                    // warning or error?
                                    WARNING_HERE
                                    break;
                                case 1:
                                    if (1)
                                    {
                                        printf("on");
                                    }
                                    else
                                    {
                                        printf("off");
                                    }
                                    break;
                                case 2:
                                case 3:
                                    printf("%lu", 0);
                                    break;
                                case 4:
                                case 5:
                                    printf("%ld", 0);
                                    break;
                                case 6:
                                case 7:
                                    printf("%LF", 0.0);
                                    break;
                                case 8:
                                    printf(get_ls(&textVal, loc[1]));
                                    break;
                                }
                            }
                            else
                            {
                                ERROR_HERE
                            }
                        }
                        else
                        {
                            ERROR_HERE
                        }
                    }
                    else
                    {
                        ERROR_HERE
                    }

                    freeToken(fline.ftoken);
                }

                // printf("%d", fline.num);
                // clean up

                fclose(ski);

                goto exit;
            }
            else
            {
                exitCode = 5;
                THROW_ERROR(exitCode);
                goto exit;
            }
        }
        else
        {
            exitCode = 3;
            THROW_ERROR(exitCode);
            goto exit;
        }
    }
    else if (argc > 4)
    {
        exitCode = 1;
        THROW_ERROR(exitCode);
        goto exit;
    }

exit:

    if (program == 1)
    {
        free_ls(&textVal);

        free_ls(&argBuf);
        free_ls(&varBuf);
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