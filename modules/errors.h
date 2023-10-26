#include <stdio.h>

// 1
void tooManyArgs()
{
    printf("command | ERROR: Too many arguments\nUse 'schizle -h' for help\n");
}

// 2
void NULLMode()
{
    printf("command | ERROR: expected argument <mode> got NULL\nUse 'schizle -h' for help\n");
}

// 3
void invalidMode(char **argv)
{
    printf("command | ERROR: expected valid value for <mode> got '%s'\nUse 'schizle -h' for help\n", argv[1]);
}

// 4
void NULLPath()
{
    printf("command | ERROR: expected argument <file-path> got NULL\nUse 'schizle -h' for help\n");
}

// 5
void invalidPath(int argc, char **argv)
{
    if (argc == 2)
    {
        printf("command | ERROR: File not found at %s\n", argv[1]);
    }
    else
    {
        printf("command | ERROR: File not found at %s\n", argv[2]);
    }
}

// 6
void nonMatchingOption(char **argv)
{
    printf("command | ERROR: expected matching [option] got %s\nUse 'schizle -h' for help\n", argv[3]);
}

// 7
void invalidOption(char **argv)
{
    printf("command | ERROR: expected valid value for [option] got %s\nUse 'schizle -h' for help\n", argv[3]);
}

// 8
void incompleteGet(char *file, int line)
{
    printf("%s %d | ERROR: expected module name after 'get' got NULL\n", file, line);
}

// 9
void duplicateModule(char *file, int line)
{
    printf("%s %d | ERROR: cannot get module twice\n", file, line);
}

// 10
void missingAlias(char *file, int line)
{
    printf("%s %d | ERROR: expected 'as' to declare module alias\n", file, line);
}

// 11
void invalidModule(char *file, int line)
{
    printf("%s %d | ERROR: unknown module\n", file, line);
}

// 12
void unnestedEnd(char *file, int line)
{
    printf("%s %d | ERROR: cannot use 'end' outside of block\n", file, line);
}

// 13
void nullExpression(char *file, int line)
{
    printf("%s %d | ERROR: expected valid expression\n", file, line);
}

// 14
void unnestedElifElse(char *file, int line)
{
    printf("%s %d | ERROR: cannot use 'elseif' or 'else' outside of conditional\n", file, line);
}

// 15
void unnestedBreak(char *file, int line)
{
    printf("%s %d | ERROR: cannot use 'break' outside of loop\n", file, line);
}

// 16
void invalidVarName(char *file, int line)
{
    printf("%s %d | ERROR: cannot start variable name with number\n", file, line);
}

// 17
void invalidNumberConversion(char *file, int line)
{
    printf("%s %d | ERROR: cannot convert non-numerical type to a numerical one\n", file, line);
}

// 18
void invalidExpression(char *file, int line)
{
    printf("%s %d | ERROR: invalid expression block\n", file, line);
}

// 19
void invalidNumber(char *file, int line)
{
    printf("%s %d | ERROR: numerical token must consist of solely digits", file, line);
}

// 20
void invalidConversion(char *file, int line)
{
    printf("%s %d | ERROR: invalid cast", file, line);
}

// 21
void varDuplication(char *file, int line)
{
    printf("%s %d | ERROR: cannot duplicate variable", file, line);
}

// 22
void undefinedVar(char *file, int line)
{
    printf("%s %d | ERROR: undefined variable", file, line);
}

// 23
void constRedeclaration(char *file, int line)
{
    printf("%s %d | ERROR: cannot redeclare a constant value", file, line);
}

// 24
void constInitialization(char *file, int line)
{
    printf("%s %d | ERROR: constant must be initialized with a value", file, line);
}

// 25
void blockOpen(char *file, int line)
{
    printf("%s %d | ERROR: block not ended", file, line);
}

// 26
void generalSyntax(char *file, int line)
{
    printf("%s %d | ERROR: syntax error", file, line);
}

// 27
void undefinedFunction(char *file, int line)
{
    printf("%s %d | ERROR: undefined function", file, line);
}

// 28
void wrongNumberOfArgs(char *file, int line)
{
    printf("%s %d | ERROR: incorrect number of args", file, line);
}