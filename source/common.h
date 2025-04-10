#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <cmath>
#include <cassert>
#include <cstdint>

typedef int8_t int8, i8;
typedef int16_t int16, i16;
typedef int32_t int32, i32;
typedef int64_t int64, i64;

typedef uint8_t uint8, u8;
typedef uint16_t uint16, u16;
typedef uint32_t uint32, u32;
typedef uint64_t uint64, u64;

typedef unsigned int uint;

typedef float f32;
typedef double f64;

using std::string;

struct Settings {
    int width;
    int height;
    const char* name;
    bool fullscreen;
    bool vsync;
};

#define PRINT_INFO(msg) { \
    System::SetConsoleColor(FOREGROUND_GREEN); \
    printf(msg); \
    System::SetConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); \
}

#define PRINT_DEBUG(msg) { \
    System::SetConsoleColor(FOREGROUND_BLUE); \
    printf(msg); \
    System::SetConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); \
}

#define PRINT_ERROR(msg) { \
    System::SetConsoleColor(FOREGROUND_RED); \
    printf(msg); \
    System::SetConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); \
}
