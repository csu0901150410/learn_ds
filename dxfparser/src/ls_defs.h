#pragma once

#include <float.h> // for DBL_MAX

#ifdef _USE_DOUBLE
    #define lsReal double
    
    #ifndef LS_HUGE
        #define LS_HUGE DBL_MAX
    #endif
#else
    #define lsReal float

    #ifndef LS_HUGE
        #define LS_HUGE FLT_MAX
    #endif
#endif

#define MAX_BUF_SIZE 1024

#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define LS_PI 3.1415926535897932384626433832795
