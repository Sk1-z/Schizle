#include <stdio.h>
#include <string.h>

#include "include/tok.h"
#include "include/program.h"

#include "../std/ssl.h"

int main(int argc, char *argv[])
{
    // Command line parsing
    if (argc == 1)
    {
        THROW_ERROR(2);
    }
    else if (argc == 2)
    {
        THROW_ERROR(4);
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

                            if (!strcmp(get_ls(fline.ftoken->toks, 1), "schizle_standard_library") || !strcmp(get_ls(fline.ftoken->toks, 1), "ssl") || !strcmp(get_ls(fline.ftoken->toks, 1), "std_lib") || !strcmp(get_ls(fline.ftoken->toks, 1), "std"))
                            {
                                if (getLineSize(&fline) <= 3)
                                {
                                    GET_SSL(get_ls(fline.ftoken->toks, 1))
                                }
                                else
                                {
                                    GET_SSL(get_ls(fline.ftoken->toks, 3))
                                }
                            }
                            else
                            {
                                ERROR_HERE
                            }
                        }
                    }
                    else if (!strcmp(get_ls(fline.ftoken->toks, 0), "_"))
                    {
                        continue;
                    }
                    else if (!strcmp(get_ls(fline.ftoken->toks, 0), "\n"))
                    {
                        continue;
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
                        clear_lui16(&typeArg);

                        size_t i = 1;
                        while (strcmp(get_ls(fline.ftoken->toks, i), "end"))
                        {
                            if (strcmp(get_ls(fline.ftoken->toks, i), "\n"))
                            {
                                push_ls(&argBuf, get_ls(fline.ftoken->toks, i));

                                if (get_lui16(&(fline.ftoken->isStringL), i))
                                {
                                    pushBool_lui16(&isStringArg, TRUE);
                                    push_lui16(&typeArg, 8);
                                }
                                else
                                {
                                    pushBool_lui16(&isStringArg, FALSE);

                                    size_t Index = getIndex_ls(&varBuf, get_ls(fline.ftoken->toks, i));

                                    if (Index)
                                    {
                                        size_t *loc = getVarLookUp(&varTable, Index);
                                        push_lui16(&typeArg, loc[0]);
                                        free(loc);
                                    }
                                    else
                                    {
                                        PRINT_POINT
                                        ERROR_HERE
                                    }
                                }
                            }

                            i++;
                            if (i == getLineSize(&fline))
                            {
                                ERROR_HERE
                            }
                        }
                        // for (size_t i = 1; i < getLineSize(&fline); i++)
                        // {
                        //     if (strcmp(get_ls(fline.ftoken->toks, i), "\n"))
                        //     {
                        //         push_ls(&argBuf, get_ls(fline.ftoken->toks, i));

                        //         if (get_lui16(&(fline.ftoken->isStringL), i))
                        //         {
                        //             pushBool_lui16(&isStringArg, TRUE);
                        //             push_lui16(&typeArg, 8);
                        //         }
                        //         else
                        //         {
                        //             pushBool_lui16(&isStringArg, FALSE);

                        //             size_t Index = getIndex_ls(&varBuf, get_ls(fline.ftoken->toks, i));

                        //             if (Index)
                        //             {
                        //                 size_t *loc = getVarLookUp(&varTable, Index);
                        //                 push_lui16(&typeArg, loc[0]);
                        //                 free(loc);
                        //             }
                        //             else
                        //             {
                        //                 ERROR_HERE
                        //             }
                        //         }
                        //     }
                        // }

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
                            if (getLineSize(&fline) <= 4)
                            {
                                ERROR_HERE
                            }

                            if (strcmp(get_ls(fline.ftoken->toks, 3), "mFunc"))
                            {
                                ERROR_HERE
                            }

                            modIndex = getIndex_ls(&(modules.moduleNames), get_ls(fline.ftoken->toks, 2));

                            if (modIndex)
                            {
                                struct module *module = get_module(&modules, modIndex);

                                size_t funcI = getIndex_ls(&(module->functionNames), get_ls(fline.ftoken->toks, 4));
                                if (funcI)
                                {
                                }
                                else
                                {
                                    if (strcmp(get_ls(fline.ftoken->toks, 4), get_ls(&(module->functionNames), 0)))
                                    {
                                        struct functionSig sig = get_sig(&(module->functionSignatures), 0);

                                        if (getSize_ls(&argBuf) < sig.numArgs)
                                        {
                                            ERROR_HERE
                                        }

                                        for (size_t i = 0; i < sig.argID.size; i++)
                                        {
                                            if (get_lui16(&typeArg, i) != get_lui16(&(sig.argID), i) && get_lui16(&(sig.argID), i) != 0)
                                            {
                                                ERROR_HERE
                                            }
                                        }

                                        sig.call();
                                    }
                                    else
                                    {
                                    }
                                }
                            }
                            else
                            {
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

                fclose(ski);

                goto exit;
            }
            else
            {
                THROW_ERROR(5);
            }
        }
        else
        {
            THROW_ERROR(3);
        }
    }
    else if (argc > 4)
    {
        THROW_ERROR(1);
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