#include "include/expressions.h"
#include "include/module.h"
#include "include/program.h"

int interpret(char *fileName, size_t *eLine, size_t pargc, char *pargv[])
{
    // File entry point
    FILE *ski = fopen(fileName, "r");
    if (ski != NULL)
    {
        program = 1;
        // pre processing
        INIT_PROGRAM(fileName)
        if (pargc)
        {
            char name[6];
            for (size_t i = 0; i < pargc; i++)
            {
                sprintf(name, "PARG%zu", i + 1);
                push_ls(&varBuf, name);
                pushTable(&varTable, 8, textVal.size);
                push_ls(&textVal, pargv[i]);
                pushBool_lui16(&(varMut), FALSE);
                pushBool_lui16(&(varDef), TRUE);
            }
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
            *eLine = fline.num;
            offset = fline.num;

            if (!strcmp(get_ls(fline.ftoken->toks, 0), "KW_GET"))
            {
                if (getLineSize(&fline) < 2)
                {
                    I_ERROR(8)
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

                    if (!strcmp(get_ls(fline.ftoken->toks, 1), "std_lib") ||
                        !strcmp(get_ls(fline.ftoken->toks, 1), "std"))
                    {
                        CHECK_MODULE(1)

                        if (getLineSize(&fline) < 4)
                        {
                            GET_STD(get_ls(fline.ftoken->toks, 1))
                        }
                        else
                        {
                            if (strcmp(get_ls(fline.ftoken->toks, 2), "KW_ALIAS"))
                            {
                                I_ERROR(10)
                            }
                            GET_STD(get_ls(fline.ftoken->toks, 3))
                        }
                    }
                    else if (!strcmp(get_ls(fline.ftoken->toks, 1), "system"))
                    {
                        CHECK_MODULE(2)

                        if (getLineSize(&fline) < 4)
                        {
                            GET_SYSTEM(get_ls(fline.ftoken->toks, 1))
                        }
                        else
                        {
                            if (strcmp(get_ls(fline.ftoken->toks, 2), "KW_ALIAS"))
                            {
                                I_ERROR(10)
                            }
                            GET_SYSTEM(get_ls(fline.ftoken->toks, 3))
                        }
                    }
                    else if (!strcmp(get_ls(fline.ftoken->toks, 1), "file") ||
                             !strcmp(get_ls(fline.ftoken->toks, 1), "file_stream"))
                    {
                        CHECK_MODULE(3)

                        if (getLineSize(&fline) < 4)
                        {
                            GET_FILE(get_ls(fline.ftoken->toks, 1))
                        }
                        else
                        {
                            if (strcmp(get_ls(fline.ftoken->toks, 2), "KW_ALIAS"))
                            {
                                I_ERROR(10)
                            }
                            GET_FILE(get_ls(fline.ftoken->toks, 3));
                        }
                    }
                    else if (!strcmp(get_ls(fline.ftoken->toks, 1), "skext"))
                    {
                        CHECK_MODULE(4)

                        if (getLineSize(&fline) < 4)
                        {
                            GET_SKEXT(get_ls(fline.ftoken->toks, 1))
                        }
                        else
                        {
                            if (strcmp(get_ls(fline.ftoken->toks, 2), "KW_ALIAS"))
                            {
                                I_ERROR(10)
                            }
                            GET_SKEXT(get_ls(fline.ftoken->toks, 3))
                        }
                    }
                    else if (!strcmp(get_ls(fline.ftoken->toks, 1), "math"))
                    {
                        CHECK_MODULE(5)

                        if (getLineSize(&fline) < 4)
                        {
                            GET_MATH(get_ls(fline.ftoken->toks, 1))
                        }
                        else
                        {
                            if (strcmp(get_ls(fline.ftoken->toks, 2), "KW_ALIAS"))
                            {
                                I_ERROR(10)
                            }
                            GET_MATH(get_ls(fline.ftoken->toks, 3))
                        }
                    }
                    else
                    {
                        I_ERROR(11)
                    }
                }
            }
            else if (!strcmp(get_ls(fline.ftoken->toks, 0), "\n"))
            {
                freeToken(fline.ftoken);
                continue;
            }
            else if (!strcmp(get_ls(fline.ftoken->toks, 0), "KW_END"))
            {
                get = 0;
            }
            else
            {
                get = 0;
                goto get_skip;
            }

            freeToken(fline.ftoken);
        }

        while (intepreterState)
        {
            if (fgets(lineBuf, sizeof(lineBuf), ski) == NULL)
            {
                intepreterState = 0;
                continue;
            }

            fline.ftoken = tokenize(lineBuf);
            if (offset == fline.num)
            {
                pushOffset(&(fline.offsets), ftell(ski));
                offset++;
                // printf("\nOFFSET %d", offset);
            }
            fline.num++;
            *eLine = fline.num;

        get_skip:

            // printf("\nline %d:", fline.num);
            // for (size_t i = 0; i < getLineSize(&fline); i++)
            // {
            //     printf("%s ", get_ls(fline.ftoken->toks, i));
            // }

            if (!strcmp(get_ls(fline.ftoken->toks, 0), "exit"))
            {
                if (falseExp)
                {
                    goto skip;
                }
                else
                {
                    freeToken(fline.ftoken);
                    goto exit;
                }
            }
            else if (!strcmp(get_ls(fline.ftoken->toks, 0), "KW_END"))
            {
                if (nest)
                {
                    // printf("\nec: %d loop %d nest: %d lnest: %d f: %d\n", expC, loopNum, nest, loopNest, falseExp);
                    if (nest == 1)
                    {
                        if (get_lui16(&isLoop, nest - 1))
                        {
                            if (!falseLoop && !falseExp)
                            {
                                fseek(ski, get_lsi32(&loopOffsets, loopNest - 1), SEEK_SET);
                                fline.num = get_lsi32(&loopLineNum, loopNest - 1) - 1;
                                loopNum++;
                                fLoop = 1;
                            }
                            else
                            {
                                loopNum = 0;
                                fLoop = 0;
                                falseExp = 0;
                                falseLoop = 0;
                                loopNest = 0;
                                currentLoopNest = 0;

                                clear_lsi32(&loopOffsets);
                                clear_lsi32(&loopLineNum);
                                clear_lui16(&isLoop);

                                expC = 0;
                            }
                        }
                        else
                        {
                            if (falseIfExp)
                            {
                                falseExp = 0;
                                falseIf = 0;
                            }
                            controlDone = 0;

                            clear_lsi32(&loopOffsets);
                            clear_lsi32(&loopLineNum);
                            clear_lui16(&isLoop);

                            expC = 0;

                            if (currentLoopNest)
                            {
                                currentLoopNest = 0;
                            }
                        }
                    }
                    else if (get_lui16(&isLoop, nest - 1))
                    {
                        // printf("END WHILE\n");
                        if (!falseLoop && !falseExp)
                        {
                            fseek(ski, get_lsi32(&loopOffsets, loopNest - 1), SEEK_SET);
                            fline.num = get_lsi32(&loopLineNum, loopNest - 1) - 1;
                            loopNum++;
                        }
                        else
                        {
                            if (falseLoopExp == nest)
                            {
                                falseExp = 0;
                                falseLoop = 0;
                                loopNum = 0;
                                loopNest--;
                            }
                            else if (brokeLoop == loopNest && brokeLoop)
                            {
                                falseExp = 0;
                                falseLoop = 0;
                                brokeLoop = 0;
                                loopNum = 0;
                                loopNest--;
                            }
                        }
                    }
                    else if (!get_lui16(&isLoop, nest - 1))
                    {
                        // printf("END IF\n");
                        if (falseIfExp == nest)
                        {
                            falseExp = 0;
                            falseIf = 0;
                            controlDone = 0;
                        }
                    }

                    nest--;
                }
                else
                {
                    I_ERROR(12)
                }

                // if (falseExp && controlNest == 1)
                // {
                //     falseExp = 0;
                // }

                // if (controlNest)
                // {
                //     controlNest--;
                // }
                // else
                // {
                //     WARNING_HERE
                // }

                // if (falseExp && controlNest)
                // {
                //     PRINTTYPEDB("end case 1");
                //     controlNest--;
                //     if (!brokeLoop && !falseLoop)
                //     {
                //         falseExp = 0;
                //     }
                //     falseIf = 0;
                // }
                // else if (!falseIf && controlDone && !loopNest)
                // {
                //     PRINTTYPEDB("end case 1.5");
                //     controlDone = 0;
                //     controlNest--;
                // }
                // else if (falseExpNest == controlNest)
                // {
                //     PRINTTYPEDB("end case 2");
                //     controlNest--;
                // }
                // else if (!falseExp && loopNest)
                // {
                //     PRINTTYPEDB("end case 3");
                //     fseek(ski, get_lsi32(&loopOffsets, loopNest - 1), SEEK_SET);
                //     fline.num = get_lsi32(&loopLineNum, loopNest - 1) - 1;
                //     loopNum++;
                // }
                // else if (falseExp && loopNest)
                // {
                //     PRINTTYPEDB("end case 4");
                //     if (loopNest == 1)
                //     {
                //         loopNum = 0;
                //         clear_lsi32(&loopOffsets);
                //         clear_lsi32(&loopLineNum);
                //     }

                //     if (falseLoopNest == nest)
                //     {
                //         falseExp = 0;
                //         falseLoop = 0;
                //         falseLoopNest = 0;
                //     }
                //     loopNest--;

                //     if (brokeLoop)
                //     {
                //         brokeLoop = 0;
                //         falseExp = 0;
                //     }
                // }
                // else if (!falseExp)
                // {
                //     WARNING_HERE
                // }
            }
            else if (!strcmp(get_ls(fline.ftoken->toks, 0), "KW_IF"))
            {
                nest++;
                if (isLoop.size == nest - 1)
                {
                    expC++;
                    pushBool_lui16(&isLoop, FALSE);
                }
                else
                {
                    set_lui16(&isLoop, nest - 1, FALSE);
                }
                ifInit++;

                if (falseExp)
                {
                    goto skip;
                }

                size_t condCode;

                if (getLineSize(&fline) == 3)
                {
                    if (strcmp(get_ls(fline.ftoken->toks, 1), "KW_COND_BLOCK"))
                    {
                        I_ERROR(13)
                    }

                    if (cond(&condCode, get_ls(fline.ftoken->toks, 2)))
                    {
                        controlDone = 1;
                    }
                    else
                    {
                        falseExp = 1;
                        falseIf = 1;
                        falseIfExp = nest;
                    }

                    if (condCode)
                    {
                        I_ERROR(18)
                    }
                }
                else if (getLineSize(&fline) == 2)
                {
                    if (cond(&condCode, get_ls(fline.ftoken->toks, 1)))
                    {
                        controlDone = 1;
                    }
                    else
                    {
                        falseExp = 1;
                        falseIf = 1;
                        falseIfExp = nest;
                    }

                    if (condCode)
                    {
                        I_ERROR(18)
                    }
                }
                else
                {
                    I_ERROR(13)
                }

                PRINTTYPEDB("control if");
            }
            else if (!strcmp(get_ls(fline.ftoken->toks, 0), "KW_ELIF"))
            {
                if (!ifInit || get_lui16(&isLoop, nest - 1))
                {
                    I_ERROR(14)
                }
                else if (falseExp && falseIfExp != nest && !controlDone)
                {
                    goto skip;
                }
                else if (falseExp && falseIfExp == nest && !controlDone)
                {
                    size_t condCode;

                    if (getLineSize(&fline) == 3)
                    {
                        if (strcmp(get_ls(fline.ftoken->toks, 1), "KW_COND_BLOCK"))
                        {
                            I_ERROR(13)
                        }

                        if (cond(&condCode, get_ls(fline.ftoken->toks, 2)))
                        {
                            controlDone = 1;
                            falseExp = 0;
                            falseIf = 0;
                        }
                        else
                        {
                            falseExp = 1;
                            falseIf = 1;
                        }

                        if (condCode)
                        {
                            I_ERROR(18)
                        }
                    }
                    else if (getLineSize(&fline) == 2)
                    {
                        if (cond(&condCode, get_ls(fline.ftoken->toks, 1)))
                        {
                            controlDone = 1;
                        }
                        else
                        {
                            falseExp = 1;
                            falseIf = 1;
                        }

                        if (condCode)
                        {
                            I_ERROR(18)
                        }
                    }
                    else
                    {
                        I_ERROR(13)
                    }
                }
                else if (controlDone)
                {
                    falseExp = 1;
                    falseIf = 1;
                    falseIfExp = nest;
                }

                PRINTTYPEDB("control elseif");
            }
            else if (!strcmp(get_ls(fline.ftoken->toks, 0), "KW_ELSE"))
            {
                if (!ifInit || get_lui16(&isLoop, nest - 1))
                {
                    I_ERROR(14)
                }
                else if (falseExp && falseIfExp != nest && !controlDone)
                {
                    goto skip;
                }
                else if (falseExp && falseIfExp == nest && !controlDone)
                {
                    falseExp = 0;
                    falseIf = 0;
                }
                else if (controlDone)
                {
                    falseExp = 1;
                    falseIf = 1;
                    falseIfExp = nest;
                }

                PRINTTYPEDB("control else");
            }
            else if (!strcmp(get_ls(fline.ftoken->toks, 0), "KW_LOOP"))
            {
                // printf("nest: %d lnest: %d clnest: %d loop#: %d oSize: %zu\n", nest, loopNest, currentLoopNest,
                // loopNum,
                //        loopOffsets.size);
                nest++;

                if (isLoop.size == nest - 1)
                {
                    expC++;
                    pushBool_lui16(&isLoop, TRUE);
                }
                else
                {
                    set_lui16(&isLoop, nest - 1, TRUE);
                }

                if (falseExp)
                {
                    goto skip;
                }

                size_t condCode;

                if (getLineSize(&fline) == 3)
                {
                    if (strcmp(get_ls(fline.ftoken->toks, 1), "KW_COND_BLOCK"))
                    {
                        I_ERROR(13)
                    }

                    if (cond(&condCode, get_ls(fline.ftoken->toks, 2)))
                    {
                        if (!loopNum && !fLoop && currentLoopNest < nest)
                        {
                            // printf("\nshould only happen once case 1\n");
                            loopNest++;
                            push_lsi32(&loopOffsets, get_offset(&fline, fline.num));
                            push_lsi32(&loopLineNum, fline.num);
                        }
                        else if (loopOffsets.size < loopNest)
                        {
                            // printf("\nshould only happen once case 2\n");
                            loopNest++;
                            push_lsi32(&loopOffsets, get_offset(&fline, fline.num));
                            push_lsi32(&loopLineNum, fline.num);
                        }
                        else if (get_lsi32(&loopLineNum, loopNest - 1) != fline.num)
                        {
                            // printf("\nshould only happen once case 3\n");
                            loopNest++;
                            set_lsi32(&loopOffsets, loopNest - 1, get_offset(&fline, fline.num));
                            set_lsi32(&loopLineNum, loopNest - 1, fline.num);
                        }
                        // else if (get_lsi32(&loopLineNum, loopNest - 1) == fline.num)
                        // {
                        //     printf("\nshould only happen once case 4\n");
                        //     loopNest++;
                        // }
                        currentLoopNest = nest;
                    }
                    else
                    {
                        if (get_lsi32(&loopLineNum, loopNest - 1) != fline.num)
                        {
                            // printf("\nshould only happen once case 4\n");
                            loopNest++;
                            set_lsi32(&loopOffsets, loopNest - 1, get_offset(&fline, fline.num));
                            set_lsi32(&loopLineNum, loopNest - 1, fline.num);
                        }
                        falseExp = 1;
                        falseLoop = 1;
                        falseLoopExp = nest;
                    }

                    if (condCode)
                    {
                        I_ERROR(18)
                    }
                }
                else if (getLineSize(&fline) == 2)
                {
                    if (cond(&condCode, get_ls(fline.ftoken->toks, 1)))
                    {
                        if (!loopNum && !fLoop && currentLoopNest < nest)
                        {
                            // printf("\nshould only happen once case 1\n");
                            loopNest++;
                            push_lsi32(&loopOffsets, get_offset(&fline, fline.num));
                            push_lsi32(&loopLineNum, fline.num);
                        }
                        else if (loopOffsets.size < loopNest)
                        {
                            // printf("\nshould only happen once case 2\n");
                            loopNest++;
                            push_lsi32(&loopOffsets, get_offset(&fline, fline.num));
                            push_lsi32(&loopLineNum, fline.num);
                        }
                        else if (get_lsi32(&loopLineNum, loopNest - 1) != fline.num)
                        {
                            // printf("\nshould only happen once case 3\n");
                            loopNest++;
                            set_lsi32(&loopOffsets, loopNest - 1, get_offset(&fline, fline.num));
                            set_lsi32(&loopLineNum, loopNest - 1, fline.num);
                        }
                        // else if (get_lsi32(&loopLineNum, loopNest - 1) == fline.num)
                        // {
                        //     printf("\nshould only happen once case 4\n");
                        //     loopNest++;
                        // }
                        currentLoopNest = nest;
                    }
                    else
                    {
                        if (get_lsi32(&loopLineNum, loopNest - 1) != fline.num)
                        {
                            // printf("\nshould only happen once case 4\n");
                            loopNest++;
                            set_lsi32(&loopOffsets, loopNest - 1, get_offset(&fline, fline.num));
                            set_lsi32(&loopLineNum, loopNest - 1, fline.num);
                        }
                        falseExp = 1;
                        falseLoop = 1;
                        falseLoopExp = nest;
                    }

                    if (condCode)
                    {
                        I_ERROR(18)
                    }
                }
                else
                {
                    I_ERROR(13)
                }

                PRINTTYPEDB("loop");
            }
            else if (!strcmp(get_ls(fline.ftoken->toks, 0), "KW_BREAK"))
            {
                if (falseExp)
                {
                    goto skip;
                }
                else if (loopNest)
                {
                    falseExp = 1;
                    falseLoop = 1;
                    brokeLoop = loopNest;
                }
                else
                {
                    I_ERROR(15)
                }

                PRINTTYPEDB("break loop");
            }
            else if (!strcmp(get_ls(fline.ftoken->toks, 0), "\n"))
            {
            }
            else if (!strcmp(get_ls(fline.ftoken->toks, 0), "KW_MUT") ||
                     !strcmp(get_ls(fline.ftoken->toks, 0), "KW_IMMUT"))
            {
                if (falseExp)
                {
                    goto skip;
                }

                PRINTTYPEDB("declaration");

                if (getIndex_ls(&varBuf, get_ls(fline.ftoken->toks, 1)))
                {
                    I_ERROR(21)
                }

                if (getLineSize(&fline) == 6 || getLineSize(&fline) == 7)
                {
                    if (autoType(1))
                    {
                        I_ERROR(16)
                    }

                    if (!strcmp(get_ls(fline.ftoken->toks, 4), "KW_EQ"))
                    {
                        if (!strcmp(get_ls(fline.ftoken->toks, 5), "KW_EVAL_BLOCK"))
                        {
                            if (getLineSize(&fline) != 7)
                            {
                                I_ERROR(13)
                            }

                            size_t evalCode;

                            size_t type = stringToType(get_ls(fline.ftoken->toks, 3));

                            char *c;
                            char fmt[310];
                            switch (type)
                            {
                            case 1:
                            case 8:
                                I_ERROR(17)
                                break;
                            case 2:
                                sprintf(fmt, "%hu", (uint16_t)eval_ll(&evalCode, get_ls(fline.ftoken->toks, 6)));
                                NEW_VALUE(type, get_ls(fline.ftoken->toks, 1), fmt, c);
                                break;
                            case 3:
                                sprintf(fmt, "%llu", (uint64_t)eval_ll(&evalCode, get_ls(fline.ftoken->toks, 6)));
                                NEW_VALUE(type, get_ls(fline.ftoken->toks, 1), fmt, c);
                                break;
                            case 4:
                                sprintf(fmt, "%d", (int32_t)eval_ll(&evalCode, get_ls(fline.ftoken->toks, 6)));
                                NEW_VALUE(type, get_ls(fline.ftoken->toks, 1), fmt, c);
                                break;
                            case 5:
                                sprintf(fmt, "%lld", (int64_t)eval_ll(&evalCode, get_ls(fline.ftoken->toks, 6)));
                                NEW_VALUE(type, get_ls(fline.ftoken->toks, 1), fmt, c);
                                break;
                            case 6:
                                sprintf(fmt, "%f", (float)eval_dbl(&evalCode, get_ls(fline.ftoken->toks, 6)));
                                NEW_VALUE(type, get_ls(fline.ftoken->toks, 1), fmt, c);
                                break;
                            case 7:
                                sprintf(fmt, "%lf", (double)eval_dbl(&evalCode, get_ls(fline.ftoken->toks, 6)));
                                NEW_VALUE(type, get_ls(fline.ftoken->toks, 1), fmt, c);
                                break;
                            }

                            if (evalCode)
                            {
                                // printf("t: %zu ", type);
                                // printf("c: %zu expr: ", evalCode);
                                // printf(get_ls(fline.ftoken->toks, 6));
                                I_ERROR(18)
                            }

                            if (!strcmp(get_ls(fline.ftoken->toks, 0), "KW_MUT"))
                            {
                                pushBool_lui16(&varMut, TRUE);
                            }
                            else
                            {
                                pushBool_lui16(&varMut, FALSE);
                            }
                            pushBool_lui16(&varDef, TRUE);
                        }
                        else
                        {
                            if (getLineSize(&fline) != 6)
                            {
                                I_ERROR(13)
                            }

                            size_t foundType = autoType(5);
                            if (foundType)
                            {
                                size_t declaredType = stringToType(get_ls(fline.ftoken->toks, 3));

                                char *c;
                                switch (foundType)
                                {
                                case 1:
                                    if (declaredType == foundType)
                                    {
                                        NEW_VALUE(declaredType, get_ls(fline.ftoken->toks, 1),
                                                  get_ls(fline.ftoken->toks, 5), c)
                                    }
                                    else
                                    {
                                        I_ERROR(20)
                                    }
                                    break;
                                case 6:
                                case 7:
                                    if (declaredType == foundType)
                                    {
                                        NEW_VALUE(declaredType, get_ls(fline.ftoken->toks, 1),
                                                  get_ls(fline.ftoken->toks, 5), c)
                                    }
                                    else
                                    {
                                        I_ERROR(20)
                                    }
                                    break;
                                case 8:
                                    if (declaredType == foundType)
                                    {
                                        NEW_VALUE(declaredType, get_ls(fline.ftoken->toks, 1),
                                                  get_ls(fline.ftoken->toks, 5), c)
                                    }
                                    else
                                    {
                                        I_ERROR(20)
                                    }
                                    break;
                                default:
                                    if (declaredType < 6 && declaredType > 1)
                                    {
                                        NEW_VALUE(declaredType, get_ls(fline.ftoken->toks, 1),
                                                  get_ls(fline.ftoken->toks, 5), c)
                                    }
                                    else
                                    {
                                        I_ERROR(20)
                                    }
                                    break;
                                }
                            }
                            else
                            {
                                size_t index = getIndex_ls(&varBuf, get_ls(fline.ftoken->toks, 3));

                                if (index)
                                {
                                    size_t *loc = getVarLookUp(&varTable, index);
                                    NEW_VALUE_FROM_OLD(loc, get_ls(fline.ftoken->toks, 1));
                                    free(loc);
                                }
                                else
                                {
                                    I_ERROR(13)
                                }
                            }

                            if (!strcmp(get_ls(fline.ftoken->toks, 0), "KW_MUT"))
                            {
                                pushBool_lui16(&varMut, TRUE);
                            }
                            else
                            {
                                pushBool_lui16(&varMut, FALSE);
                            }
                            pushBool_lui16(&varDef, TRUE);
                        }
                    }
                    else
                    {
                        I_ERROR(13)
                    }
                }
                else if (getLineSize(&fline) == 4 || getLineSize(&fline) == 5)
                {
                    if (autoType(1))
                    {
                        I_ERROR(16)
                    }

                    if (getIndex_ls(&varBuf, get_ls(fline.ftoken->toks, 1)))
                    {
                        I_ERROR(21)
                    }

                    if (!strcmp(get_ls(fline.ftoken->toks, 2), "KW_EQ"))
                    {

                        if (getLineSize(&fline) == 5)
                        {
                            if (!strcmp(get_ls(fline.ftoken->toks, 3), "KW_EVAL_BLOCK"))
                            {
                                int dbl = 0;
                                for (size_t i = 0; i < strlen(get_ls(fline.ftoken->toks, 4)); i++)
                                {
                                    if (get_ls(fline.ftoken->toks, 4)[i] == '.')
                                    {
                                        dbl = 1;
                                        break;
                                    }
                                }

                                size_t evalCode;
                                char *c;

                                if (dbl)
                                {
                                    char fmt[310];
                                    sprintf(fmt, "%lf", eval_dbl(&evalCode, get_ls(fline.ftoken->toks, 4)));
                                    NEW_VALUE(7, get_ls(fline.ftoken->toks, 1), fmt, c);
                                }
                                else
                                {
                                    char fmt[19];
                                    sprintf(fmt, "%lld", eval_ll(&evalCode, get_ls(fline.ftoken->toks, 4)));
                                    NEW_VALUE(5, get_ls(fline.ftoken->toks, 1), fmt, c);
                                }
                            }
                            else
                            {
                                I_ERROR(13)
                            }
                        }
                        else
                        {
                            if (strcmp(get_ls(fline.ftoken->toks, 3), "KW_EVAL_BLOCK"))
                            {
                                size_t t = autoType(3);
                                if (t)
                                {
                                    char *c;
                                    NEW_VALUE(t, get_ls(fline.ftoken->toks, 1), get_ls(fline.ftoken->toks, 3), c)
                                }
                                else
                                {
                                    size_t index = getIndex_ls(&varBuf, get_ls(fline.ftoken->toks, 3));

                                    if (index)
                                    {
                                        size_t *loc = getVarLookUp(&varTable, index);
                                        NEW_VALUE_FROM_OLD(loc, get_ls(fline.ftoken->toks, 1));
                                        free(loc);
                                    }
                                    else
                                    {
                                        I_ERROR(22)
                                    }
                                }
                            }
                            else
                            {
                                I_ERROR(13)
                            }
                        }

                        if (!strcmp(get_ls(fline.ftoken->toks, 0), "KW_MUT"))
                        {
                            pushBool_lui16(&varMut, TRUE);
                        }
                        else
                        {
                            pushBool_lui16(&varMut, FALSE);
                        }
                        pushBool_lui16(&varDef, TRUE);
                    }
                    else if (!strcmp(get_ls(fline.ftoken->toks, 2), "KW_TYPE"))
                    {
                        if (getLineSize(&fline) == 5)
                        {
                            I_ERROR(13)
                        }

                        if (!strcmp(get_ls(fline.ftoken->toks, 0), "KW_IMMUT"))
                        {
                            I_ERROR(24)
                        }

                        char *c;
                        size_t t = stringToType(get_ls(fline.ftoken->toks, 3));

                        switch (t)
                        {
                        case 0:
                            I_ERROR(20)
                            break;
                        case 1:
                            NEW_VALUE(t, get_ls(fline.ftoken->toks, 1), "off", c)
                            break;
                        case 8:
                            NEW_VALUE(t, get_ls(fline.ftoken->toks, 1), "\0", c)
                            break;
                        default:
                            NEW_VALUE(t, get_ls(fline.ftoken->toks, 1), "0", c)
                            break;
                        }

                        pushBool_lui16(&varMut, TRUE);
                        pushBool_lui16(&varDef, FALSE);
                    }
                    else
                    {
                        I_ERROR(13)
                    }
                }
                else
                {
                    I_ERROR(13)
                }
            }
            else if (!strcmp(get_ls(fline.ftoken->toks, 0), "KW_PARAMS"))
            {
                if (falseExp)
                {
                    goto skip;
                }

                if (getLineSize(&fline) < 3)
                {
                    I_ERROR(13)
                }

                PRINTTYPEDB("parameter declaration");

                clear_ls(&argBuf);
                clear_lui16(&isStringArg);
                clear_lui16(&typeArg);

                size_t i = 1;
                while (strcmp(get_ls(fline.ftoken->toks, i), "KW_END"))
                {
                    if (strcmp(get_ls(fline.ftoken->toks, i), "\n"))
                    {
                        if (strcmp(get_ls(fline.ftoken->toks, i), "KW_LIST"))
                        {
                            push_ls(&argBuf, get_ls(fline.ftoken->toks, i));

                            if (get_lui16(&(fline.ftoken->isStringL), i))
                            {
                                pushBool_lui16(&isStringArg, TRUE);
                                push_lui16(&typeArg, 9);
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
                                    I_ERROR(22)
                                }
                            }
                        }
                    }
                    else
                    {
                        I_ERROR(26)
                    }

                    i++;
                    if (i == getLineSize(&fline))
                    {
                        I_ERROR(26)
                    }
                }

                paramInit = 1;

                i++;
                if (i < getLineSize(&fline))
                {
                    // printf("\n%s\n", get_ls(fline.ftoken->toks, i));
                    if (!strcmp(get_ls(fline.ftoken->toks, i), "KW_INS"))
                    {
                        size_t modIndex;
                        if (!strcmp(get_ls(fline.ftoken->toks, i + 1), "KW_MOD"))
                        {
                            if (getLineSize(&fline) < i + 5)
                            {
                                I_ERROR(13)
                            }

                            if (strcmp(get_ls(fline.ftoken->toks, i + 3), "KW_MEMBER"))
                            {
                                I_ERROR(26)
                            }

                            modIndex = getIndex_ls(&moduleNames, get_ls(fline.ftoken->toks, i + 2));

                            if (modIndex || !strcmp(get_ls(fline.ftoken->toks, i + 2), get_ls(&moduleNames, 0)))
                            {
                                struct module *module = &modules[modIndex];
                                // printf("\n%d %s\nindex: %s i: %zu\n", modules[1].functionNames.size,
                                //        get_ls(fline.ftoken->toks, 4), module->functionNames.data[0], modIndex);
                                size_t funcI = getIndex_ls(&(module->functionNames), get_ls(fline.ftoken->toks, i + 4));
                                // printf("\n%d %s\nindex: %s i: %zu\n", modules.data[0]->functionNames.size,
                                //        get_ls(fline.ftoken->toks, 4), module->functionNames.data[1], funcI);

                                if (funcI ||
                                    !strcmp(get_ls(fline.ftoken->toks, i + 4), get_ls(&(module->functionNames), 0)))
                                {
                                    struct functionSig *sig = malloc(sizeof(struct functionSig));
                                    sig = get_sig(&(module->functionSignatures), funcI);

                                    if (getSize_ls(&argBuf) != sig->numArgs)
                                    {
                                        I_ERROR(28)
                                    }

                                    for (size_t j = 0; j < sig->argID.size; j++)
                                    {
                                        if (get_lui16(&typeArg, j) != get_lui16(&(sig->argID), j) &&
                                            get_lui16(&(sig->argID), j) != 0)
                                        {
                                            switch (get_lui16(&(sig->argID), j))
                                            {
                                            case 1:
                                                if (get_lui16(&typeArg, j) != 1)
                                                {
                                                    I_ERROR(20)
                                                }
                                                break;
                                            case 2:
                                            case 3:
                                            case 4:
                                            case 5:
                                                if (!(get_lui16(&typeArg, j) > 1 && get_lui16(&typeArg, j) < 6))
                                                {
                                                    I_ERROR(20)
                                                }
                                                break;
                                            case 6:
                                            case 7:
                                                if (!(get_lui16(&typeArg, j) > 5 && get_lui16(&typeArg, j) < 8))
                                                {
                                                    I_ERROR(20)
                                                }
                                                break;
                                            case 8:
                                                if (get_lui16(&typeArg, j) != 8)
                                                {
                                                    I_ERROR(20)
                                                }
                                            case 9:
                                                if (get_lui16(&typeArg, j) != 8 && get_lui16(&typeArg, j) != 9)
                                                {
                                                    I_ERROR(20)
                                                }
                                                break;
                                            }
                                        }
                                    }

                                    size_t r = sig->call();

                                    clear_ls(&argBuf);
                                    clear_lui16(&isStringArg);
                                    clear_lui16(&typeArg);

                                    if (r)
                                    {
                                        I_ERROR(r)
                                    }
                                }
                                else
                                {
                                    I_ERROR(27)
                                }
                            }
                            else
                            {
                                I_ERROR(11)
                            }
                        }
                        else
                        {
                            I_ERROR(26)
                        }

                        paramInit = 0;
                    }
                    else
                    {
                        I_ERROR(26)
                    }
                }
            }
            else if (!strcmp(get_ls(fline.ftoken->toks, 0), "KW_MOD"))
            {
                if (falseExp)
                {
                    goto skip;
                }

                // printf("\nfunction call apparently print doesnt work i hate c\n");

                PRINTTYPEDB("function call");

                if (getLineSize(&fline) < 4)
                {
                    I_ERROR(13)
                }

                size_t modIndex;

                if (strcmp(get_ls(fline.ftoken->toks, 2), "KW_MEMBER"))
                {
                    I_ERROR(26)
                }

                modIndex = getIndex_ls(&moduleNames, get_ls(fline.ftoken->toks, 1));

                if (modIndex || !strcmp(get_ls(fline.ftoken->toks, 1), get_ls(&moduleNames, 0)))
                {
                    struct module *module = &modules[modIndex];
                    // printf("\n%d %s\nindex: %s i: %zu\n", modules[1].functionNames.size,
                    //        get_ls(fline.ftoken->toks, 4), module->functionNames.data[0], modIndex);
                    size_t funcI = getIndex_ls(&(module->functionNames), get_ls(fline.ftoken->toks, 3));
                    // printf("\n%d %s\nindex: %s i: %zu\n", modules.data[0]->functionNames.size,
                    //        get_ls(fline.ftoken->toks, 4), module->functionNames.data[1], funcI);

                    if (funcI || !strcmp(get_ls(fline.ftoken->toks, 3), get_ls(&(module->functionNames), 0)))
                    {
                        struct functionSig *sig = malloc(sizeof(struct functionSig));
                        sig = get_sig(&(module->functionSignatures), funcI);

                        if (getSize_ls(&argBuf) != sig->numArgs)
                        {
                            I_ERROR(28)
                        }

                        for (size_t i = 0; i < sig->argID.size; i++)
                        {
                            if (get_lui16(&typeArg, i) != get_lui16(&(sig->argID), i) &&
                                get_lui16(&(sig->argID), i) != 0)
                            {
                                switch (get_lui16(&(sig->argID), i))
                                {
                                case 1:
                                    if (get_lui16(&typeArg, i) != 1)
                                    {
                                        I_ERROR(20)
                                    }
                                    break;
                                case 2:
                                case 3:
                                case 4:
                                case 5:
                                    if (!(get_lui16(&typeArg, i) > 1 && get_lui16(&typeArg, i) < 6))
                                    {
                                        I_ERROR(20)
                                    }
                                    break;
                                case 6:
                                case 7:
                                    if (!(get_lui16(&typeArg, i) > 5 && get_lui16(&typeArg, i) < 8))
                                    {
                                        I_ERROR(20)
                                    }
                                    break;
                                case 8:
                                    if (get_lui16(&typeArg, i) != 8)
                                    {
                                        I_ERROR(20)
                                    }
                                case 9:
                                    if (get_lui16(&typeArg, i) != 8 && get_lui16(&typeArg, i) != 9)
                                    {
                                        I_ERROR(20)
                                    }
                                    break;
                                }
                            }
                        }

                        size_t r = sig->call();

                        clear_ls(&argBuf);
                        clear_lui16(&isStringArg);
                        clear_lui16(&typeArg);

                        if (r)
                        {
                            I_ERROR(r)
                        }
                    }
                    else
                    {
                        I_ERROR(27)
                    }
                    // else
                    // {
                    //     if (!strcmp(get_ls(fline.ftoken->toks, 4), get_ls(&(module->functionNames), 0)))
                    //     {
                    //         struct functionSig *sig = malloc(sizeof(struct functionSig));
                    //         sig = get_sig(&(module->functionSignatures), funcI);

                    //         if (getSize_ls(&argBuf) != sig->numArgs)
                    //         {
                    //             I_ERROR(28)
                    //         }

                    //         for (size_t i = 0; i < sig->argID.size; i++)
                    //         {
                    //             switch (get_lui16(&(sig->argID), i))
                    //             {
                    //             case 1:
                    //                 if (get_lui16(&typeArg, i) != 1)
                    //                 {
                    //                     I_ERROR(20)
                    //                 }
                    //                 break;
                    //             case 2:
                    //             case 3:
                    //             case 4:
                    //             case 5:
                    //                 if (!(get_lui16(&typeArg, i) > 1 && get_lui16(&typeArg, i) < 6))
                    //                 {
                    //                     I_ERROR(20)
                    //                 }
                    //                 break;
                    //             case 6:
                    //             case 7:
                    //                 if (!(get_lui16(&typeArg, i) > 5 && get_lui16(&typeArg, i) < 8))
                    //                 {
                    //                     I_ERROR(20)
                    //                 }
                    //                 break;
                    //             case 8:
                    //                 if (get_lui16(&typeArg, i) != 8)
                    //                 {
                    //                     I_ERROR(20)
                    //                 }
                    //             case 9:
                    //                 if (get_lui16(&typeArg, i) != 8 && get_lui16(&typeArg, i) != 9)
                    //                 {
                    //                     I_ERROR(20)
                    //                 }
                    //                 break;
                    //             }
                    //         }

                    //         size_t r = sig->call();

                    //         if (r)
                    //         {
                    //             I_ERROR(r)
                    //         }
                    //     }
                    //     else
                    //     {
                    //         I_ERROR(27)
                    //     }
                    // }
                }
                else
                {
                    I_ERROR(11)
                }

                paramInit = 0;
            }
            else
            {
                int index = getIndex_ls(&varBuf, get_ls(fline.ftoken->toks, 0));
                if (index)
                {
                    if (falseExp)
                    {
                        goto skip;
                    }

                    PRINTTYPEDB("redeclaration");

                    if (!strcmp(get_ls(fline.ftoken->toks, 1), "KW_EQ"))
                    {
                        if (get_lui16(&varMut, index))
                        {
                            size_t *loc = getVarLookUp(&varTable, index);

                            if (!strcmp(get_ls(fline.ftoken->toks, 2), "KW_EVAL_BLOCK"))
                            {
                                if (getLineSize(&fline) == 4)
                                {
                                    size_t evalCode;

                                    switch (loc[0])
                                    {
                                    case 1:
                                        I_ERROR(20)
                                        break;
                                    case 2:
                                        set_lui16(&natVal, loc[1],
                                                  (uint16_t)eval_ll(&evalCode, get_ls(fline.ftoken->toks, 3)));
                                        break;
                                    case 3:
                                        set_lui64(&nat64Val, loc[1],
                                                  (uint64_t)eval_ll(&evalCode, get_ls(fline.ftoken->toks, 3)));
                                        break;
                                    case 4:
                                        set_lsi32(&intVal, loc[1],
                                                  (int32_t)eval_ll(&evalCode, get_ls(fline.ftoken->toks, 3)));
                                        break;
                                    case 5:
                                        set_lsi64(&int64Val, loc[1],
                                                  (int64_t)eval_ll(&evalCode, get_ls(fline.ftoken->toks, 3)));
                                        break;
                                    case 6:
                                        set_lf32(&fracVal, loc[1],
                                                 (float)eval_dbl(&evalCode, get_ls(fline.ftoken->toks, 3)));
                                        break;
                                    case 7:
                                        set_ld64(&frac64Val, loc[1],
                                                 (double)eval_dbl(&evalCode, get_ls(fline.ftoken->toks, 3)));
                                        break;
                                    case 8:
                                        I_ERROR(20)
                                        break;
                                    }

                                    if (evalCode)
                                    {
                                        // printf("t: %zu ", type);
                                        // printf("c: %zu expr: ", evalCode);
                                        // printf(get_ls(fline.ftoken->toks, 6));
                                        I_ERROR(18)
                                    }
                                }
                                else
                                {
                                    I_ERROR(13)
                                }
                            }
                            else if (getLineSize(&fline) == 3)
                            {
                                size_t t = autoType(2);
                                char *endChar;

                                if (t > 1 && t < 8)
                                {
                                    unsigned long long ull = strtoull(get_ls(fline.ftoken->toks, 2), &endChar, 10);
                                    switch (*endChar)
                                    {
                                    case '-':
                                        long long ll = strtoll(get_ls(fline.ftoken->toks, 2), &endChar, 10);
                                        if (*endChar != '\0')
                                        {
                                            I_ERROR(19)
                                        }
                                        break;
                                    case '.':
                                        float dbl = strtof(get_ls(fline.ftoken->toks, 2), &endChar);
                                        if (*endChar != '\0')
                                        {
                                            I_ERROR(19)
                                        }
                                        break;
                                    default:
                                        if (*endChar != '\0')
                                        {
                                            I_ERROR(19)
                                        }
                                        break;
                                    }
                                }

                                switch (t)
                                {
                                case 0:
                                    size_t index = getIndex_ls(&varBuf, get_ls(fline.ftoken->toks, 2));

                                    if (index)
                                    {
                                        size_t *locSecondVar = getVarLookUp(&varTable, index);
                                        switch (locSecondVar[0])
                                        {
                                        case 1:
                                            if (loc[0] == 1)
                                            {
                                                if (get_lui16(&stateVal, locSecondVar[1]))
                                                {
                                                    set_lui16(&stateVal, loc[1], TRUE);
                                                }
                                                else
                                                {
                                                    set_lui16(&stateVal, loc[1], FALSE);
                                                }
                                            }
                                            else
                                            {
                                                I_ERROR(20)
                                            }
                                            break;
                                        case 2:
                                            if (loc[0] > 1 && loc[0] < 6)
                                            {
                                                switch (loc[0])
                                                {
                                                case 2:
                                                    set_lui16(&natVal, loc[1], get_lui16(&natVal, locSecondVar[1]));
                                                    break;
                                                case 3:
                                                    set_lui64(&nat64Val, loc[1],
                                                              (uint64_t)get_lui16(&natVal, locSecondVar[1]));
                                                    break;
                                                case 4:
                                                    set_lsi32(&intVal, loc[1],
                                                              (int32_t)get_lui16(&natVal, locSecondVar[1]));
                                                    break;
                                                case 5:
                                                    set_lsi64(&int64Val, loc[1],
                                                              (int64_t)get_lui16(&natVal, locSecondVar[1]));
                                                    break;
                                                }
                                            }
                                            else
                                            {
                                                I_ERROR(20)
                                            }
                                            break;
                                        case 3:
                                            if (loc[0] > 1 && loc[0] < 6)
                                            {
                                                switch (loc[0])
                                                {
                                                case 2:
                                                    set_lui16(&natVal, loc[1],
                                                              (uint16_t)get_lui64(&nat64Val, locSecondVar[1]));
                                                    break;
                                                case 3:
                                                    set_lui64(&nat64Val, loc[1], get_lui64(&nat64Val, locSecondVar[1]));
                                                    break;
                                                case 4:
                                                    set_lsi32(&intVal, loc[1],
                                                              (int32_t)get_lui64(&nat64Val, locSecondVar[1]));
                                                    break;
                                                case 5:
                                                    set_lsi64(&int64Val, loc[1],
                                                              (int64_t)get_lui64(&nat64Val, locSecondVar[1]));
                                                    break;
                                                }
                                            }
                                            else
                                            {
                                                I_ERROR(20)
                                            }
                                            break;
                                        case 4:
                                            if (loc[0] > 1 && loc[0] < 6)
                                            {
                                                switch (loc[0])
                                                {
                                                case 2:
                                                    set_lui16(&natVal, loc[1],
                                                              (uint16_t)get_lsi32(&intVal, locSecondVar[1]));
                                                    break;
                                                case 3:
                                                    set_lui64(&nat64Val, loc[1],
                                                              (uint64_t)get_lsi32(&intVal, locSecondVar[1]));
                                                    break;
                                                case 4:
                                                    set_lsi32(&intVal, loc[1], get_lsi32(&intVal, locSecondVar[1]));
                                                    break;
                                                case 5:
                                                    set_lsi64(&int64Val, loc[1],
                                                              (int64_t)get_lsi32(&intVal, locSecondVar[1]));
                                                    break;
                                                }
                                            }
                                            else
                                            {
                                                I_ERROR(20)
                                            }
                                            break;
                                        case 5:
                                            if (loc[0] > 1 && loc[0] < 6)
                                            {
                                                switch (loc[0])
                                                {
                                                case 2:
                                                    set_lui16(&natVal, loc[1],
                                                              (uint16_t)get_lsi64(&int64Val, locSecondVar[1]));
                                                    break;
                                                case 3:
                                                    set_lui64(&nat64Val, loc[1],
                                                              (uint64_t)get_lsi64(&int64Val, locSecondVar[1]));
                                                    break;
                                                case 4:
                                                    set_lsi32(&intVal, loc[1],
                                                              (int32_t)get_lsi64(&int64Val, locSecondVar[1]));
                                                    break;
                                                case 5:
                                                    set_lsi64(&int64Val, loc[1], get_lsi64(&int64Val, locSecondVar[1]));
                                                    break;
                                                }
                                            }
                                            else
                                            {
                                                I_ERROR(20)
                                            }
                                            break;
                                        case 6:
                                            if (loc[0] > 5 && loc[0] < 8)
                                            {
                                                switch (loc[0])
                                                {
                                                case 6:
                                                    set_lf32(&fracVal, loc[1], get_lf32(&fracVal, locSecondVar[1]));
                                                    break;
                                                case 7:
                                                    set_ld64(&frac64Val, loc[1],
                                                             (double)get_lf32(&fracVal, locSecondVar[1]));
                                                    break;
                                                }
                                            }
                                            else
                                            {
                                                I_ERROR(20)
                                            }
                                            break;
                                        case 7:
                                            if (loc[0] > 5 && loc[0] < 8)
                                            {
                                                switch (loc[0])
                                                {
                                                case 6:
                                                    set_lf32(&fracVal, loc[1],
                                                             (float)get_ld64(&frac64Val, locSecondVar[1]));
                                                    break;
                                                case 7:
                                                    set_ld64(&frac64Val, loc[1], get_ld64(&frac64Val, locSecondVar[1]));
                                                    break;
                                                }
                                            }
                                            else
                                            {
                                                I_ERROR(20)
                                            }
                                            break;
                                        case 8:
                                            if (loc[0] == 8)
                                            {
                                                set_ls(&textVal, loc[1], get_ls(&textVal, locSecondVar[1]));
                                            }
                                            else
                                            {
                                                I_ERROR(20)
                                            }
                                            break;
                                        }

                                        free(locSecondVar);
                                    }
                                    else
                                    {
                                        I_ERROR(22)
                                    }
                                    break;
                                case 1:
                                    if (loc[0] == 1)
                                    {
                                        if (strcmp(get_ls(fline.ftoken->toks, 2), "KW_OFF"))
                                        {
                                            set_lui16(&stateVal, loc[1], TRUE);
                                        }
                                        else
                                        {
                                            set_lui16(&stateVal, loc[1], TRUE);
                                        }
                                    }
                                    else
                                    {
                                        I_ERROR(20)
                                    }
                                    break;
                                case 2:
                                case 3:
                                case 4:
                                case 5:
                                    if (loc[0] > 1 && loc[0] < 6)
                                    {
                                        switch (loc[0])
                                        {
                                        case 2:
                                            set_lui16(&natVal, loc[1],
                                                      (uint16_t)strtoull(get_ls(fline.ftoken->toks, 2), &endChar, 10));
                                            break;
                                        case 3:
                                            set_lui64(&nat64Val, loc[1],
                                                      (uint64_t)strtoull(get_ls(fline.ftoken->toks, 2), &endChar, 10));
                                            break;
                                        case 4:
                                            set_lsi32(&intVal, loc[1],
                                                      (int32_t)strtoll(get_ls(fline.ftoken->toks, 2), &endChar, 10));
                                            break;
                                        case 5:
                                            set_lsi64(&int64Val, loc[1],
                                                      (int64_t)strtoll(get_ls(fline.ftoken->toks, 2), &endChar, 10));
                                            break;
                                        }
                                    }
                                    else
                                    {
                                        I_ERROR(20)
                                    }
                                    break;
                                case 6:
                                case 7:
                                    if (loc[0] > 5 && loc[0] < 8)
                                    {
                                        switch (loc[0])
                                        {
                                        case 6:
                                            set_lf32(&fracVal, loc[1],
                                                     (float)strtold(get_ls(fline.ftoken->toks, 2), &endChar));
                                            break;
                                        case 7:
                                            set_ld64(&frac64Val, loc[1],
                                                     (double)strtold(get_ls(fline.ftoken->toks, 2), &endChar));
                                            break;
                                        }
                                    }
                                    else
                                    {
                                        I_ERROR(20)
                                    }
                                    break;
                                case 8:
                                    if (loc[0] == 8)
                                    {
                                        set_ls(&textVal, loc[1], get_ls(fline.ftoken->toks, 2));
                                    }
                                    else
                                    {
                                        I_ERROR(20)
                                    }
                                    break;
                                }
                            }
                            else
                            {
                                I_ERROR(13)
                            }

                            if (!get_lui16(&varDef, index))
                            {
                                set_lui16(&varDef, index, TRUE);
                            }

                            free(loc);
                        }
                        else
                        {
                            I_ERROR(23)
                        }
                    }
                    else
                    {
                        I_ERROR(18)
                    }
                }
                else
                {
                    I_ERROR(22)
                }
            }
        skip:
            freeToken(fline.ftoken);
        }

        if (nest)
        {
            // printf("\nnest over: %d", nest);
            I_ERROR(25)
        }
        // printf("%d", fline.num);

        fclose(ski);

        goto exit;
    }
    else
    {
        return 5;
    }

exit:

    if (program == 1)
    {
        FREE_PROGRAM
    }

    return 0;
}
