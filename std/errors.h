#include <stdio.h>

void tooManyArgs()
{
    printf("error: Too many arguments\nUsage: Schizle <mode> <file-path>");
}

void NULLMode()
{
    printf("error: expected argument <mode> got NULL\nUsage: Schizle <mode> <file-path> <program_name?>\n");
}

void invalidMode(char **argv)
{
    printf("error: expected valid value for <mode> got '%s'\nUsage: argv[1] mode : 'run'", argv[1]);
}

void NULLPath()
{
    printf("error: expected argument <file-path> got NULL\nUsage: Schizle <mode> <file-path> <program_name?>\n");
}

void invalidPath(char **argv)
{
    printf("error: File not found at %s", argv[2]);
}