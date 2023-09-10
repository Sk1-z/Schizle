// prints to the command line
#define CMDL_OUT(loc, arg)                 \
    switch (loc[0])                        \
    {                                      \
    case 0:                                \
        printf(arg);                       \
    case 1:                                \
        if (GET_STATE(loc[1]) == 1)        \
        {                                  \
            printf("on");                  \
        }                                  \
        else if (GET_STATE(loc[1]) == 0)   \
        {                                  \
            printf("off");                 \
        }                                  \
        break;                             \
    case 2:                                \
        printf("%hu", GET_NAT(loc[1]));    \
        break;                             \
    case 3:                                \
        printf("%llu", GET_NAT64(loc[1])); \
        break;                             \
    case 4:                                \
        printf("%d", GET_INT(loc[1]));     \
        break;                             \
    case 5:                                \
        printf("%lld", GET_INT64(loc[1])); \
        break;                             \
    case 6:                                \
        printf("%f", GET_FRAC(loc[1]));    \
        break;                             \
    case 7:                                \
        printf("%lf", GET_FRAC64(loc[1])); \
        break;                             \
    case 8:                                \
        printf(GET_TEXT(loc[1]));          \
        break;                             \
    }