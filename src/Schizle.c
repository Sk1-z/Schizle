#include <stdio.h>
#include <string.h>

#include "../std/ssl.h"

#include "include/tok.h"
#include "include/program.h"

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
                while (get)
                {
                    if (fgets(lineBuf, sizeof(lineBuf), ski) == NULL)
                    {
                        get = 0;
                        goto exit;
                    }

                    fline.ftoken = tokenize(lineBuf);
                    pushOffset(&(fline.offsets), ftell(ski));
                    fline.num++;

                    if (!strcmp(get_ls(fline.ftoken->toks, 0), "get"))
                    {
                        if (getLineSize(&fline) <= 1)
                        {
                            WARNING_HERE
                        }
                        else
                        {
                            for (int i = 0; i < strlen(get_ls(fline.ftoken->toks, 1)); i++)
                            {
                                if (get_ls(fline.ftoken->toks, 1)[i] == ' ')
                                {
                                    fline.ftoken->toks->data[1][i] = '_';
                                }
                            }

                            if (!strcmp(get_ls(fline.ftoken->toks, 1), "schizle_standard_library") || !strcmp(get_ls(fline.ftoken->toks, 1), "std_lib"))
                            {
                                if (getLineSize(&fline) <= 3)
                                {
                                    push_ls(&(modules.moduleNames), get_ls(fline.ftoken->toks, 1));
                                    GET_STD(get_ls(fline.ftoken->toks, 1))
                                }
                                else
                                {
                                    push_ls(&(modules.moduleNames), get_ls(fline.ftoken->toks, 3));
                                    GET_STD(get_ls(fline.ftoken->toks, 3))
                                }
                            }
                            else
                            {
                                ERROR_HERE
                            }
                        }
                    }
                    else if (!strcmp(get_ls(fline.ftoken->toks, 0), "end"))
                    {
                        get = 0;
                    }
                    else
                    {
                        get = 0;
                        goto get_skip;
                    }
                }

                while (intepreterState)
                {
                    if (fgets(lineBuf, sizeof(lineBuf), ski) == NULL)
                    {
                        intepreterState = 0;
                        continue;
                    }

                    fline.ftoken = tokenize(lineBuf);
                    if (!loopExp)
                    {
                        pushOffset(&(fline.offsets), ftell(ski));
                    }

                    fline.num++;
                get_skip:

                    if (!strcmp(get_ls(fline.ftoken->toks, 0), "end"))
                    {
                        if (expState != 0)
                        {
                        }
                        else
                        {
                            WARNING_HERE
                        }
                        expState--;
                    }
                    else if (!strcmp(get_ls(fline.ftoken->toks, 0), "\n"))
                    {
                        PRINTTYPEDB("newline");
                    }
                    else if (!strcmp(get_ls(fline.ftoken->toks, 0), "_"))
                    {
                        PRINTTYPEDB("comment");
                    }
                    else if (!strcmp(get_ls(fline.ftoken->toks, 0), "new"))
                    {
                        if (getLineSize(&fline) <= 1)
                        {
                            WARNING_HERE
                        }
                        PRINTTYPEDB("declaration");
                    }
                    else if (!strcmp(get_ls(fline.ftoken->toks, 0), "new[]"))
                    {
                        if (getLineSize(&fline) <= 1)
                        {
                            WARNING_HERE
                        }
                        PRINTTYPEDB("group declaration");
                    }
                    else if (!strcmp(get_ls(fline.ftoken->toks, 0), "if"))
                    {
                        if (getLineSize(&fline) <= 1)
                        {
                            WARNING_HERE
                        }
                        PRINTTYPEDB("if");
                    }
                    else if (!strcmp(get_ls(fline.ftoken->toks, 0), "elseif"))
                    {
                        if (getLineSize(&fline) <= 1)
                        {
                            WARNING_HERE
                        }
                        PRINTTYPEDB("elseif");
                    }
                    else if (!strcmp(get_ls(fline.ftoken->toks, 0), "else"))
                    {
                        if (getLineSize(&fline) <= 1)
                        {
                            WARNING_HERE
                        }
                        PRINTTYPEDB("else");
                    }
                    else if (!strcmp(get_ls(fline.ftoken->toks, 0), "dur"))
                    {
                        if (getLineSize(&fline) <= 1)
                        {
                            WARNING_HERE
                        }
                        PRINTTYPEDB("loop");
                    }
                    else if (!strcmp(get_ls(fline.ftoken->toks, 0), "params"))
                    {
                        if (paramInit)
                        {
                            WARNING_HERE
                        }

                        if (getLineSize(&fline) <= 1)
                        {
                            WARNING_HERE
                        }
                        PRINTTYPEDB("parameter declaration");

                        clear_ls(&argBuf);
                        clear_lui16(&isStringArg);

                        for (size_t i = 1; i < getLineSize(&fline); i++)
                        {
                            if (get_lui16(&(fline.ftoken->isStringL), i))
                            {
                                pushBool_lui16(&isStringArg, TRUE);
                            }
                            else
                            {
                                pushBool_lui16(&isStringArg, FALSE);
                            }

                            push_ls(&argBuf, get_ls(fline.ftoken->toks, i));
                        }

                        paramInit = 1;
                    }
                    else if (!strcmp(get_ls(fline.ftoken->toks, 0), "call"))
                    {
                        if (getLineSize(&fline) <= 1)
                        {
                            WARNING_HERE
                        }

                        PRINTTYPEDB("function call");

                        size_t modIndex;
                        if (!strcmp(get_ls(fline.ftoken->toks, 1), "@"))
                        {
                            if (getLineSize(&fline) <= 3)
                            {
                                ERROR_HERE
                            }
                            modIndex = getIndex_ls(&(modules.moduleNames), get_ls(fline.ftoken->toks, 2));
                            if (!modIndex)
                            {
                                ERROR_HERE
                            }
                            switch (get_lui16(&(modules.moduleID), modIndex))
                            {
                            case 0:
                                if (!strcmp(get_ls(fline.ftoken->toks, 3), "cmdl_out"))
                                {
                                    if (get_lui16(&isStringArg, 0))
                                    {
                                        size_t loc[2] = {0, 0};
                                        CMDL_OUT(loc, get_ls(&argBuf, 0))
                                    }
                                    else
                                    {
                                        size_t index = getIndex_ls(&varBuf, get_ls(&argBuf, 0));
                                        if (!index)
                                        {
                                            ERROR_HERE
                                        }
                                        else
                                        {
                                            size_t *loc = getVarLookUp(&varTable, index);
                                            CMDL_OUT(loc, NULL)
                                        }
                                    }
                                }
                                else
                                {
                                    ERROR_HERE
                                }
                                break;
                            default:
                                ERROR_HERE
                            }
                        }
                        else
                        {
                        }

                        paramInit = 0;
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
        FREE_PROGRAM
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