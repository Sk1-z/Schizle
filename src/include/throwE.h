#include "../../std/errors.h"

void throwE(int errorNum, char **argv)
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