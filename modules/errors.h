#include <stdio.h>

void tooManyArgs()
{
    printf("command error: Too many arguments\nUsage: Schizle <mode> <file-path>");
}

void NULLMode()
{
    printf("command error: expected argument <mode> got NULL\nUsage: Schizle <mode> <file-path> <program_name?>\n");
}

void invalidMode(char **argv)
{
    printf("command error: expected valid value for <mode> got '%s'\nUsage: argv[1] mode : 'run'", argv[1]);
}

void NULLPath()
{
    printf("command error: expected argument <file-path> got NULL\nUsage: Schizle <mode> <file-path> <program_name?>\n");
}

void invalidPath(char **argv)
{
    printf("command error: File not found at %s", argv[2]);
}