#pragma once

#include "../src/include/program.h"

#define GET_SYSTEM(name)                                                                                               \
    push_ls(&varBuf, "_SYSTEM_");                                                                                      \
    pushTable(&varTable, 8, textVal.size);                                                                             \
    push_ls(&textVal, SYSTEM);                                                                                         \
    pushBool_lui16(&(varMut), FALSE);                                                                                  \
    pushBool_lui16(&(varDef), TRUE);                                                                                   \
                                                                                                                       \
    push_ls(&varBuf, "_CLOCKS_");                                                                                      \
    pushTable(&varTable, 7, frac64Val.size);                                                                           \
    push_ld64(&frac64Val, CLOCKS_PER_SEC);                                                                             \
    pushBool_lui16(&(varMut), FALSE);                                                                                  \
    pushBool_lui16(&(varDef), TRUE);                                                                                   \
                                                                                                                       \
    push_ls(&moduleNames, name);                                                                                       \
    push_lui16(&moduleID, 2);                                                                                          \
                                                                                                                       \
    struct module sys;                                                                                                 \
    init_lsig(&(sys.functionSignatures));                                                                              \
    init_ls(&(sys.functionNames));                                                                                     \
                                                                                                                       \
    struct functionSig cmd_sig;                                                                                        \
    init_sig(&cmd_sig);                                                                                                \
    cmd_sig.numArgs = 1;                                                                                               \
    push_lui16(&(cmd_sig.argID), 9);                                                                                   \
    cmd_sig.call = cmd;                                                                                                \
                                                                                                                       \
    push_sig(&(sys.functionSignatures), &cmd_sig);                                                                     \
    push_ls(&(sys.functionNames), "cmd");                                                                              \
                                                                                                                       \
    struct functionSig clock_sig;                                                                                      \
    init_sig(&clock_sig);                                                                                              \
    clock_sig.numArgs = 1;                                                                                             \
    push_lui16(&(clock_sig.argID), 7);                                                                                 \
    clock_sig.call = _clock;                                                                                           \
                                                                                                                       \
    push_sig(&(sys.functionSignatures), &clock_sig);                                                                   \
    push_ls(&(sys.functionNames), "clock");                                                                            \
                                                                                                                       \
    struct functionSig time_epoch_sig;                                                                                 \
    init_sig(&time_epoch_sig);                                                                                         \
    time_epoch_sig.numArgs = 1;                                                                                        \
    push_lui16(&(time_epoch_sig.argID), 3);                                                                            \
    time_epoch_sig.call = time_epoch;                                                                                  \
                                                                                                                       \
    push_sig(&(sys.functionSignatures), &time_epoch_sig);                                                              \
    push_ls(&(sys.functionNames), "time_epoch");                                                                       \
                                                                                                                       \
    struct functionSig time_sig;                                                                                       \
    init_sig(&time_sig);                                                                                               \
    time_sig.numArgs = 1;                                                                                              \
    push_lui16(&(time_sig.argID), 8);                                                                                  \
    time_sig.call = _time;                                                                                             \
                                                                                                                       \
    push_sig(&(sys.functionSignatures), &time_sig);                                                                    \
    push_ls(&(sys.functionNames), "time");                                                                             \
                                                                                                                       \
    struct functionSig timef_sig;                                                                                      \
    init_sig(&timef_sig);                                                                                              \
    timef_sig.numArgs = 1;                                                                                             \
    push_lui16(&(timef_sig.argID), 8);                                                                                 \
    timef_sig.call = timef;                                                                                            \
                                                                                                                       \
    push_sig(&(sys.functionSignatures), &timef_sig);                                                                   \
    push_ls(&(sys.functionNames), "timef");                                                                            \
                                                                                                                       \
    struct functionSig timef_ymd_sig;                                                                                  \
    init_sig(&timef_ymd_sig);                                                                                          \
    timef_ymd_sig.numArgs = 1;                                                                                         \
    push_lui16(&(timef_ymd_sig.argID), 8);                                                                             \
    timef_ymd_sig.call = timef_ymd;                                                                                    \
                                                                                                                       \
    push_sig(&(sys.functionSignatures), &timef_ymd_sig);                                                               \
    push_ls(&(sys.functionNames), "timef_ymd");                                                                        \
                                                                                                                       \
    struct functionSig timef_dmy_sig;                                                                                  \
    init_sig(&timef_dmy_sig);                                                                                          \
    timef_dmy_sig.numArgs = 1;                                                                                         \
    push_lui16(&(timef_dmy_sig.argID), 8);                                                                             \
    timef_dmy_sig.call = timef_dmy;                                                                                    \
                                                                                                                       \
    push_sig(&(sys.functionSignatures), &timef_dmy_sig);                                                               \
    push_ls(&(sys.functionNames), "timef_dmy");                                                                        \
                                                                                                                       \
    struct functionSig timef_hms_sig;                                                                                  \
    init_sig(&timef_hms_sig);                                                                                          \
    timef_hms_sig.numArgs = 1;                                                                                         \
    push_lui16(&(timef_hms_sig.argID), 8);                                                                             \
    timef_hms_sig.call = timef_hms;                                                                                    \
                                                                                                                       \
    push_sig(&(sys.functionSignatures), &timef_hms_sig);                                                               \
    push_ls(&(sys.functionNames), "timef_hms");                                                                        \
                                                                                                                       \
    struct functionSig year_sig;                                                                                       \
    init_sig(&year_sig);                                                                                               \
    year_sig.numArgs = 1;                                                                                              \
    push_lui16(&(year_sig.argID), 3);                                                                                  \
    year_sig.call = year;                                                                                              \
                                                                                                                       \
    push_sig(&(sys.functionSignatures), &year_sig);                                                                    \
    push_ls(&(sys.functionNames), "year");                                                                             \
                                                                                                                       \
    struct functionSig month_sig;                                                                                      \
    init_sig(&month_sig);                                                                                              \
    month_sig.numArgs = 1;                                                                                             \
    push_lui16(&(month_sig.argID), 3);                                                                                 \
    month_sig.call = month;                                                                                            \
                                                                                                                       \
    push_sig(&(sys.functionSignatures), &month_sig);                                                                   \
    push_ls(&(sys.functionNames), "month");                                                                            \
                                                                                                                       \
    struct functionSig day_sig;                                                                                        \
    init_sig(&day_sig);                                                                                                \
    day_sig.numArgs = 1;                                                                                               \
    push_lui16(&(day_sig.argID), 3);                                                                                   \
    day_sig.call = day;                                                                                                \
                                                                                                                       \
    push_sig(&(sys.functionSignatures), &day_sig);                                                                     \
    push_ls(&(sys.functionNames), "day");                                                                              \
                                                                                                                       \
    struct functionSig hour_sig;                                                                                       \
    init_sig(&hour_sig);                                                                                               \
    hour_sig.numArgs = 1;                                                                                              \
    push_lui16(&(hour_sig.argID), 3);                                                                                  \
    hour_sig.call = hour;                                                                                              \
                                                                                                                       \
    push_sig(&(sys.functionSignatures), &hour_sig);                                                                    \
    push_ls(&(sys.functionNames), "hour");                                                                             \
                                                                                                                       \
    struct functionSig minute_sig;                                                                                     \
    init_sig(&minute_sig);                                                                                             \
    minute_sig.numArgs = 1;                                                                                            \
    push_lui16(&(minute_sig.argID), 3);                                                                                \
    minute_sig.call = minute;                                                                                          \
                                                                                                                       \
    push_sig(&(sys.functionSignatures), &minute_sig);                                                                  \
    push_ls(&(sys.functionNames), "minute");                                                                           \
                                                                                                                       \
    struct functionSig second_sig;                                                                                     \
    init_sig(&second_sig);                                                                                             \
    second_sig.numArgs = 1;                                                                                            \
    push_lui16(&(second_sig.argID), 3);                                                                                \
    second_sig.call = second;                                                                                          \
                                                                                                                       \
    push_sig(&(sys.functionSignatures), &second_sig);                                                                  \
    push_ls(&(sys.functionNames), "second");                                                                           \
                                                                                                                       \
    modules[modCount] = sys;                                                                                           \
    modCount++;

size_t cmd()
{
    if (get_lui16(&isStringArg, 0))
    {
        system(get_ls(&argBuf, 0));
    }
    else
    {
        system(GET_TEXT(getVarLookUp(&varTable, getIndex_ls(&varBuf, get_ls(&argBuf, 0)))[1]));
    }

    return 0;
}

size_t _clock()
{
    size_t *loc = getVarLookUp(&varTable, getIndex_ls(&varBuf, get_ls(&argBuf, 0)));
    switch (loc[0])
    {
    case 6:
        set_lf32(&fracVal, loc[1], (float)clock());
        break;
    case 7:
        set_ld64(&frac64Val, loc[1], (double)clock());
        break;
    }

    return 0;
}

size_t time_epoch()
{
    time_t cTime;
    time(&cTime);

    size_t *loc = getVarLookUp(&varTable, getIndex_ls(&varBuf, get_ls(&argBuf, 0)));

    switch (loc[0])
    {
    case 2:
        set_lui16(&natVal, loc[1], (uint16_t)cTime);
        break;
    case 3:
        set_lui64(&nat64Val, loc[1], (uint64_t)cTime);
        break;
    case 4:
        set_lsi32(&intVal, loc[1], (int32_t)cTime);
        break;
    case 5:
        set_lsi64(&int64Val, loc[1], (int64_t)cTime);
        break;
    }

    free(loc);

    return 0;
}

size_t _time()
{
    time_t cTime;
    time(&cTime);
    set_ls(&textVal, getVarLookUp(&varTable, getIndex_ls(&varBuf, get_ls(&argBuf, 0)))[1], ctime(&cTime));

    return 0;
}

size_t timef()
{
    time_t cTime;
    time(&cTime);
    struct tm *localTime = localtime(&cTime);

    char timeFormat[CHARACTER_LIMIT];
    strftime(timeFormat, sizeof(timeFormat), "%Y-%m-%d %H:%M:%S", localTime);

    set_ls(&textVal, getVarLookUp(&varTable, getIndex_ls(&varBuf, get_ls(&argBuf, 0)))[1], timeFormat);

    return 0;
}

size_t timef_ymd()
{
    time_t cTime;
    time(&cTime);
    struct tm *localTime = localtime(&cTime);

    char timeFormat[CHARACTER_LIMIT];
    strftime(timeFormat, sizeof(timeFormat), "%Y-%m-%d", localTime);

    set_ls(&textVal, getVarLookUp(&varTable, getIndex_ls(&varBuf, get_ls(&argBuf, 0)))[1], timeFormat);

    return 0;
}

size_t timef_dmy()
{
    time_t cTime;
    time(&cTime);
    struct tm *localTime = localtime(&cTime);

    char timeFormat[CHARACTER_LIMIT];
    strftime(timeFormat, sizeof(timeFormat), "%d-%m-%Y", localTime);

    set_ls(&textVal, getVarLookUp(&varTable, getIndex_ls(&varBuf, get_ls(&argBuf, 0)))[1], timeFormat);

    return 0;
}

size_t timef_hms()
{
    time_t cTime;
    time(&cTime);
    struct tm *localTime = localtime(&cTime);

    char timeFormat[CHARACTER_LIMIT];
    strftime(timeFormat, sizeof(timeFormat), "%H:%M:%S", localTime);

    set_ls(&textVal, getVarLookUp(&varTable, getIndex_ls(&varBuf, get_ls(&argBuf, 0)))[1], timeFormat);

    return 0;
}

size_t year()
{
    time_t cTime;
    time(&cTime);
    struct tm *localTime = localtime(&cTime);

    size_t *loc = getVarLookUp(&varTable, getIndex_ls(&varBuf, get_ls(&argBuf, 0)));

    switch (loc[0])
    {
    case 2:
        set_lui16(&natVal, loc[1], (uint16_t)(localTime->tm_year + 1900));
        break;
    case 3:
        set_lui64(&nat64Val, loc[1], (uint64_t)(localTime->tm_year + 1900));
        break;
    case 4:
        set_lsi32(&intVal, loc[1], (int32_t)(localTime->tm_year + 1900));
        break;
    case 5:
        set_lsi64(&int64Val, loc[1], (int64_t)(localTime->tm_year + 1900));
        break;
    }

    free(loc);

    return 0;
}

size_t month()
{
    time_t cTime;
    time(&cTime);
    struct tm *localTime = localtime(&cTime);

    size_t *loc = getVarLookUp(&varTable, getIndex_ls(&varBuf, get_ls(&argBuf, 0)));

    switch (loc[0])
    {
    case 2:
        set_lui16(&natVal, loc[1], (uint16_t)(localTime->tm_mon + 1));
        break;
    case 3:
        set_lui64(&nat64Val, loc[1], (uint64_t)(localTime->tm_mon + 1));
        break;
    case 4:
        set_lsi32(&intVal, loc[1], (int32_t)(localTime->tm_mon + 1));
        break;
    case 5:
        set_lsi64(&int64Val, loc[1], (int64_t)(localTime->tm_mon + 1));
        break;
    }

    free(loc);

    return 0;
}

size_t day()
{
    time_t cTime;
    time(&cTime);
    struct tm *localTime = localtime(&cTime);

    size_t *loc = getVarLookUp(&varTable, getIndex_ls(&varBuf, get_ls(&argBuf, 0)));

    switch (loc[0])
    {
    case 2:
        set_lui16(&natVal, loc[1], (uint16_t)localTime->tm_mday);
        break;
    case 3:
        set_lui64(&nat64Val, loc[1], (uint64_t)localTime->tm_mday);
        break;
    case 4:
        set_lsi32(&intVal, loc[1], (int32_t)localTime->tm_mday);
        break;
    case 5:
        set_lsi64(&int64Val, loc[1], (int64_t)localTime->tm_mday);
        break;
    }

    free(loc);

    return 0;
}

size_t hour()
{
    time_t cTime;
    time(&cTime);
    struct tm *localTime = localtime(&cTime);

    size_t *loc = getVarLookUp(&varTable, getIndex_ls(&varBuf, get_ls(&argBuf, 0)));

    switch (loc[0])
    {
    case 2:
        set_lui16(&natVal, loc[1], (uint16_t)localTime->tm_hour);
        break;
    case 3:
        set_lui64(&nat64Val, loc[1], (uint64_t)localTime->tm_hour);
        break;
    case 4:
        set_lsi32(&intVal, loc[1], (int32_t)localTime->tm_hour);
        break;
    case 5:
        set_lsi64(&int64Val, loc[1], (int64_t)localTime->tm_hour);
        break;
    }

    free(loc);

    return 0;
}

size_t minute()
{
    time_t cTime;
    time(&cTime);
    struct tm *localTime = localtime(&cTime);

    size_t *loc = getVarLookUp(&varTable, getIndex_ls(&varBuf, get_ls(&argBuf, 0)));

    switch (loc[0])
    {
    case 2:
        set_lui16(&natVal, loc[1], (uint16_t)localTime->tm_min);
        break;
    case 3:
        set_lui64(&nat64Val, loc[1], (uint64_t)localTime->tm_min);
        break;
    case 4:
        set_lsi32(&intVal, loc[1], (int32_t)localTime->tm_min);
        break;
    case 5:
        set_lsi64(&int64Val, loc[1], (int64_t)localTime->tm_min);
        break;
    }

    free(loc);

    return 0;
}

size_t second()
{
    time_t cTime;
    time(&cTime);
    struct tm *localTime = localtime(&cTime);

    size_t *loc = getVarLookUp(&varTable, getIndex_ls(&varBuf, get_ls(&argBuf, 0)));

    switch (loc[0])
    {
    case 2:
        set_lui16(&natVal, loc[1], (uint16_t)localTime->tm_sec);
        break;
    case 3:
        set_lui64(&nat64Val, loc[1], (uint64_t)localTime->tm_sec);
        break;
    case 4:
        set_lsi32(&intVal, loc[1], (int32_t)localTime->tm_sec);
        break;
    case 5:
        set_lsi64(&int64Val, loc[1], (int64_t)localTime->tm_sec);
        break;
    }

    free(loc);

    return 0;
}