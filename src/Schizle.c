#include <stdio.h>
#include <string.h>

#include "include/utils.h"
#include "include/lists.h"

int main(int argc, char *argv[])
{
    // Initialize reading variables
    int noWarning = 1;
    char charBuf[512];
    size_t lineLen = 0;
    int lineNum = 1;

    // Enter file
    if (argc == 1)
    {
        printf("error: expected argument <mode> got NULL\nUsage: Schizle <mode> <file-path>\n");
        return 1;
    }
    else if (argc == 2)
    {
        printf("error: expected argument <file-path> got NULL\nUsage: Schizle <mode> <file-path>\n");
        if (strcmp(argv[1], "run") != 0)
        {
            printf("error: expected valid value for <mode> got '%s'\nUsage: argv[1] mode = 'run'", argv[1]);
        }
        return 1;
    }
    else if (argc == 3)
    {
        if (!strcmp(argv[1], "run"))
        {
            FILE *ski = fopen(argv[2], "r");
            if (ski != NULL)
            {
                printf("interpreter process started\n");
                // parse file'
                while (fgets(charBuf, sizeof(charBuf), ski) != NULL)
                {
                    printf("%s", charBuf);
                    lineNum++;
                }

                // finish parsing
                if (noWarning)
                {
                    printf("interpreter exited with code: 0\n");
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
            printf("error: expected valid value for <mode> got '%s'\nUsage: argv[1] mode = 'run'", argv[1]);
            return 1;
        }
    }
    else if (argc > 3)
    {
        printf("error: Too many arguments\nUsage: Schizle <mode> <file-path>");
        return 1;
    }
}