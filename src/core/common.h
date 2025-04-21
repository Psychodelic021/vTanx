#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <stdint.h>
#include <limits.h>

#include "debug.h"

#define true 1
#define false 0

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

#undef MAX
#undef MIN

#define MAX(x, y) ({typeof(x) _x = (x); typeof(y) _y = (y); _x > _y ? _x : _y;})
#define MIN(x, y) ({typeof(x) _x = (x); typeof(y) _y = (y); _x < _y ? _x : _y;})
