#pragma once

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>

#include "debug.h"

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

typedef struct Settings {
    int width;
    int height;
    const char* name;
    bool fullscreen;
    bool vsync;
} Settings;
