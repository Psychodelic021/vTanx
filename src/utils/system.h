#pragma once

#include "common.h"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <Windowsx.h>
#include <cstdint>
#include <string>
#include <vector>
#include <memory>

#pragma comment(lib, "user32.lib")

// C++ namespace for system utilities
namespace System {
    void SetDpiAwareness();
    float GetTime();
    void Delay(DWORD ms);
    void CheckLastError();
    std::string LoadTextFile(const char* path);
    void SetConsoleColor(WORD color);
}

// Keep C-style functions for backward compatibility
inline void SetDpiAwareness() { System::SetDpiAwareness(); }
inline float GetTime() { return System::GetTime(); }
inline void Delay(DWORD ms) { System::Delay(ms); }
inline void CheckLastError() { System::CheckLastError(); }
inline char* LoadTextFile(const char* path) { 
    std::string s = System::LoadTextFile(path);
    char* cstr = new char[s.length() + 1];
    strcpy_s(cstr, s.length() + 1, s.c_str());
    return cstr;
}
inline void SetConsoleColor(WORD color) { System::SetConsoleColor(color); }
