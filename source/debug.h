#pragma once

#include <iostream>
#include <cstdio>
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include "system.h"

// C++ style logger using iostream
namespace Debug {
    // Color enum for better type safety
    enum class Color {
        White = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
        Red = FOREGROUND_RED | FOREGROUND_INTENSITY,
        Green = FOREGROUND_GREEN | FOREGROUND_INTENSITY,
        Blue = FOREGROUND_BLUE | FOREGROUND_INTENSITY,
        Yellow = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
        Magenta = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
        Cyan = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY
    };
    
    // Handle to the console output
    static HANDLE GetConsoleHandle() {
        static HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
        return handle;
    }
    
    // Set console text color directly without relying on external function
    static void SetConsoleColor(WORD color) {
        SetConsoleTextAttribute(GetConsoleHandle(), color);
    }
    
    // Template function to print with color - fixed to address format security warnings
    template<typename... Args>
    inline void PrintColored(Color color, const char* format, Args... args) {
        // Use our own SetConsoleColor function
        SetConsoleColor(static_cast<WORD>(color));
        // Check if we have any arguments (handles both cases safely)
        if constexpr(sizeof...(args) > 0) {
            printf(format, args...);
        } else {
            printf("%s", format);
        }
        SetConsoleColor(static_cast<WORD>(Color::White));
    }
    
    // Specialized print functions
    template<typename... Args>
    inline void Error(const char* format, Args... args) {
        PrintColored(Color::Red, format, args...);
    }
    
    template<typename... Args>
    inline void Info(const char* format, Args... args) {
        PrintColored(Color::Green, format, args...);
    }
    
    template<typename... Args>
    inline void Debug(const char* format, Args... args) {
        PrintColored(Color::Blue, format, args...);
    }
    
    template<typename... Args>
    inline void Warning(const char* format, Args... args) {
        PrintColored(Color::Magenta, format, args...);
    }
    
    template<typename... Args>
    inline void Trace(const char* format, Args... args) {
        PrintColored(Color::Yellow, format, args...);
    }
    
    template<typename... Args>
    inline void Note(const char* format, Args... args) {
        PrintColored(Color::Cyan, format, args...);
    }
}

// Keep traditional C-style macros for backward compatibility
#define NEWLINE printf("\n");
#define ENDLINE { printf("\n"); fflush(stdout); }

#define PRINT(...) { \
    printf(__VA_ARGS__); \
}

#define PRINT_ERROR(...) { \
    Debug::Error(__VA_ARGS__); \
}

#define PRINT_INFO(...) { \
    Debug::Info(__VA_ARGS__); \
}

#define PRINT_DEBUG(...) { \
    Debug::Debug(__VA_ARGS__); \
}

#define PRINT_WARNING(...) { \
    Debug::Warning(__VA_ARGS__); \
}

#define PRINT_TRACE(...) { \
    Debug::Trace(__VA_ARGS__); \
}

#define PRINT_NOTE(...) { \
    Debug::Note(__VA_ARGS__); \
}