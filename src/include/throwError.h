#include "../../modules/errors.h"

void throwError(int errorNum, int argc, char **argv, int lineNum)
{
    char *fileName;
    switch (argc)
    {
    case 2:
        fileName = argv[1];
        break;
    case 3:
        fileName = argv[2];
        break;
    }

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
        invalidPath(argc, argv);
        break;
    case 6:
        nonMatchingOption(argv);
        break;
    case 7:
        invalidOption(argv);
        break;
    case 8:
        incompleteGet(fileName, lineNum);
        break;
    case 9:
        duplicateModule(fileName, lineNum);
        break;
    case 10:
        missingAlias(fileName, lineNum);
        break;
    case 11:
        invalidModule(fileName, lineNum);
        break;
    case 12:
        unnestedEnd(fileName, lineNum);
        break;
    case 13:
        nullExpression(fileName, lineNum);
        break;
    case 14:
        unnestedElifElse(fileName, lineNum);
        break;
    case 15:
        unnestedBreak(fileName, lineNum);
        break;
    case 16:
        invalidVarName(fileName, lineNum);
        break;
    case 17:
        invalidNumberConversion(fileName, lineNum);
        break;
    case 18:
        invalidExpression(fileName, lineNum);
        break;
    case 19:
        invalidNumber(fileName, lineNum);
        break;
    case 20:
        invalidConversion(fileName, lineNum);
        break;
    case 21:
        varDuplication(fileName, lineNum);
        break;
    case 22:
        undefinedVar(fileName, lineNum);
        break;
    case 23:
        constRedeclaration(fileName, lineNum);
        break;
    case 24:
        constInitialization(fileName, lineNum);
        break;
    case 25:
        blockOpen(fileName, lineNum);
        break;
    case 26:
        generalSyntax(fileName, lineNum);
        break;
    case 27:
        undefinedFunction(fileName, lineNum);
        break;
    case 28:
        moreArgs(fileName, lineNum);
        break;
    }
}