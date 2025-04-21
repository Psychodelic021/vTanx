#pragma once

#include <iostream>
#include <format>
#include "system.h"

// C++ style logger using iostream
namespace Debug {
    // Color enum for better type safety
    enum class Color {
        White = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
        Red = FOREGROUND_RED,
        Green = FOREGROUND_GREEN,
        Blue = FOREGROUND_BLUE,
        Yellow = FOREGROUND_RED | FOREGROUND_GREEN,
        Magenta = FOREGROUND_RED | FOREGROUND_BLUE,
        Cyan = FOREGROUND_GREEN | FOREGROUND_BLUE
    };
    
    // Template function to print with color
    template<typename... Args>
    inline void PrintColored(Color color, const char* format, Args... args) {
        System::SetConsoleColor(static_cast<WORD>(color));
        printf(format, args...);
        System::SetConsoleColor(static_cast<WORD>(Color::White));
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