#pragma once

#include <float.h> // for DBL_MAX

#include <windows.h>

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

// Colors
#define CLR_WHITE   RGB(255, 255, 255)
#define CLR_BLACK   RGB(0, 0, 0)
#define CLR_RED     RGB(255, 0, 0)
#define CLR_GREEN   RGB(0, 255, 0)
#define CLR_BLUE    RGB(0, 0, 255)

#define RADIAN_2_DEGREE(x) ((x) * 180.0f / LS_PI)
#define DEGREE_2_RADIAN(x) ((x) * LS_PI / 180.0f)
