#include "../../std/errors.h"

void throwError(int errorNum, char **argv, int lineNum)
{
    switch (errorNum)
    {
    case 1:
        tooManyArgs();
        break;
    case 2:
        NULLMode();
        break;
    case 3:
        invalidMode(argv);
        break;
    case 4:
        NULLPath();
        break;
    case 5:
        invalidPath(argv);
        break;
    }
}