int pointNum = 1;

#ifdef DEBUG
#define PRINTTYPEDB(str) // printf("\n%s\n", str)
#define PRINT_POINT                   \
    printf("\nPoint %d\n", pointNum); \
    pointNum++;
#define _CRT_SECURE_NO_WARNINGS
#else
#define PRINTTYPEDB(str)
#define PRINT_POINT
#endif

#include <stdio.h>
#include <string.h>

#include "include/program.h"
#include "../modules/export.h"
#include "interpret.c"

int main(int argc, char *argv[])
{
    size_t programArgc = 0;
    char *programArgv[64];
    size_t errorLine;
    int exitCode;

    if (argc == 1)
    {
        THROW_ERROR(4)
    }
    else if (!strcmp(argv[1], "help") || !strcmp(argv[1], "-h"))
    {
        printf("\nUsage: Schizle <mode default=run> <file-path> [options] \n\n");
        printf("Modes:\n");
        printf("  version, -v   version info\n");
        printf("  help, -h      help\n");
        printf("  run, -r       run script\n");
        printf("  build, -t     transpile script using clang by default\n\n");
        printf("Run options:\n");
        printf("  -a  run with args\n\n");
        printf("Build options:\n");
        printf("  -c  build with clang (default)\n");
        printf("  -g  build with gcc\n");
        printf("  -s  output source\n");
        printf("  -w  translate without building\n");
    }
    else if (!strcmp(argv[1], "version") || !strcmp(argv[1], "-v"))
    {
        printf("Schizle Script Executable version a0.4.0\n");
        printf("Repository: https://github.com/Sk1-z/Schizle \n\n\n\n");
        printf("  ______             __        __            __                   ______                       __             __\n");
        printf(" /      \\           |  \\      |  \\          |  \\                 /      \\                     |  \\           |  \\\n");
        printf("|  $$$$$$\\  _______ | $$____   \\$$ ________ | $$  ______        |  $$$$$$\\  _______   ______   \\$$  ______  _| $$_\n");
        printf("| $$___\\$$ /       \\| $$    \\ |  \\|        \\| $$ /      \\       | $$___\\$$ /       \\ /      \\ |  \\ /      \\|   $$ \\\n");
        printf(" \\$$    \\ |  $$$$$$$| $$$$$$$\\| $$ \\$$$$$$$$| $$|  $$$$$$\\       \\$$    \\ |  $$$$$$$|  $$$$$$\\| $$|  $$$$$$\\\\$$$$$$\n");
        printf("_\\$$$$$$\\| $$      | $$  | $$| $$  /    $$ | $$| $$    $$       _\\$$$$$$\\| $$      | $$   \\$$| $$| $$  | $$ | $$ _\n");
        printf("|  \\__| $$| $$_____ | $$  | $$| $$ /  $$$$_ | $$| $$$$$$$$      |  \\__| $$| $$_____ | $$      | $$| $$__/ $$ | $$|  \\\n");
        printf(" \\$$    $$ \\$$     \\| $$  | $$| $$|  $$    \\| $$ \\$$     \\       \\$$    $$ \\$$     \\| $$      | $$| $$    $$  \\$$  $$\n");
        printf("  \\$$$$$$   \\$$$$$$$ \\$$   \\$$ \\$$ \\$$$$$$$$ \\$$  \\$$$$$$$        \\$$$$$$   \\$$$$$$$ \\$$       \\$$| $$$$$$$    \\$$$$\n");
        printf("                                                                                                  | $$\n");
        printf("                                                                                                  | $$\n");
        printf("                                                                                                   \\$$\n");
    }
    else if (!strcmp(argv[1], "run") || !strcmp(argv[1], "-r"))
    {
        if (argc < 3)
        {
            THROW_ERROR(4)
        }
        else
        {
            if (argc > 4)
            {
                if (!strcmp(argv[3], "-a"))
                {
                    programArgc = argc - 4;
                    for (size_t i = 0; i < argc - 4; i++)
                    {
                        programArgv[i] = argv[i + 4];
                    }
                    exitCode = interpret(argv[2], &errorLine, programArgc, programArgv);
                }
                else
                {
                    THROW_ERROR(1)
                }
            }
            else if (argc == 4)
            {
                if (!strcmp(argv[3], "-a"))
                {
                    THROW_ERROR(6)
                }
                else if (!strcmp(argv[3], "-c") || !strcmp(argv[3], "-g") || !strcmp(argv[3], "-t") || !strcmp(argv[3], "-v"))
                {
                    THROW_ERROR(6)
                }
                else
                {
                    THROW_ERROR(7)
                }
            }
            else
            {
                exitCode = interpret(argv[2], &errorLine, 0, NULL);
            }
        }
    }
    else if (!strcmp(argv[1], "build") || !strcmp(argv[1], "-b"))
    {
        if (argc != 3)
        {
            THROW_ERROR(4)
        }
        else
        {
            if (argc == 4)
            {
                if (!strcmp(argv[3], "-c") || !strcmp(argv[3], "-g") || !strcmp(argv[3], "-t") || !strcmp(argv[3], "-v"))
                {
                    printf("imagine it got built");
                }
                else
                {
                    THROW_ERROR(7)
                }
            }
            else
            {
                printf("imagine it got built");
            }
        }
    }
    else
    {
        if (argc > 2)
        {
            programArgc = argc - 2;
            for (size_t i = 0; i < argc - 2; i++)
            {
                programArgv[i] = argv[i + 2];
            }
            exitCode = interpret(argv[1], &errorLine, programArgc, programArgv);
        }
        else
        {
            exitCode = interpret(argv[1], &errorLine, 0, NULL);
        }
    }

    if (exitCode)
    {
        THROW_ERROR(exitCode)
    }
    return 1;
}