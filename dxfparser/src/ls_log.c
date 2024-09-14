#include "ls_log.h"

#include <stdio.h>// for sprintf_s
#include <stdarg.h>// for va_list va_start va_end

#include <Windows.h>// for MultiByteToWideChar

#define MAX_BUF_SIZE 4096

void ls_log_info(const char* fmt, ...)
{
    char buf[MAX_BUF_SIZE] = { 0 };

    va_list ap;
    va_start(ap, fmt);
    vsprintf_s(buf, sizeof(buf) - 1, fmt, ap);
    va_end(ap);

    int widesize = MultiByteToWideChar(CP_ACP, 0, buf, -1, NULL, 0);
    TCHAR* widebuf = (TCHAR*)malloc(widesize * sizeof(TCHAR));
    if (NULL == widebuf)
        return;

    MultiByteToWideChar(CP_ACP, 0, buf, -1, widebuf, widesize);
    OutputDebugString(widebuf);
    free(widebuf);
}