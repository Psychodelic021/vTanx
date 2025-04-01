#pragma once

#include <iostream>
#include <windows.h>

static void DebugOutput(const char* szFormat, ...)
{
    char szBuff[1024];
    va_list arg;
    va_start(arg, szFormat);
    _vsnprintf_s(szBuff, sizeof(szBuff), szFormat, arg);
    va_end(arg);

    OutputDebugStringA(szBuff);
}

static void DebugOutputW(const wchar_t* szFormat, ...)
{
    wchar_t szBuff[1024];
    va_list arg;
    va_start(arg, szFormat);
    _vsnwprintf_s(szBuff, sizeof(szBuff), szFormat, arg);
    va_end(arg);

    OutputDebugStringW(szBuff);
}
