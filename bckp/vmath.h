#pragma once

#include <math.h>

// DEFINES ////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define PI 3.1415926535897932384626433f

// FUNCTIONS //////////////////////////////////////////////////////////////////////////////////////////////////////////

static inline float to_radians(float angle_deg);

// VECTOR 3 TYPE //////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct vector2 {
    union {
        struct { float x, y; };
        struct { float u, v; };
    };
} vec2;

static inline vec2 v2_neg(const vec2 a);
static inline vec2 v2_add(const vec2 a, const vec2 b);
static inline vec2 v2_sub(const vec2 a, const vec2 b);
static inline vec2 v2_mulf(const vec2 a, float f);
static inline vec2 v2_pairwise(const vec2 a, const vec2 b);
static inline float v2_dot(const vec2 a, const vec2 b);
static inline float v2_len(const vec2 in);
static inline vec2 v2_normalize(const vec2 in);

// VECTOR 3 TYPE //////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct vector3 {
    union {
        struct { float x, y, z; };
        struct { float r, g, b; };
    };
} vec3;

static inline vec3 v3_neg(const vec3 a);
static inline vec3 v3_add(const vec3 a, const vec3 b);
static inline vec3 v3_sub(const vec3 a, const vec3 b);
static inline vec3 v3_mulf(const vec3 a, float f);
static inline vec3 v3_pairwise(const vec3 a, const vec3 b);
static inline float v3_dot(const vec3 a, const vec3 b);
static inline vec3 v3_cross(const vec3 a, const vec3 b);
static inline float v3_len(const vec3 in);
static inline vec3 v3_normalize(const vec3 in);

// MATRIX 4 TYPE //////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct matrix4 {
    union {
        struct {
            float m00, m01, m02, m03;
            float m10, m11, m12, m13;
            float m20, m21, m22, m23;
            float m30, m31, m32, m33;
        };
        float idx[4][4];
        float data[16];
    };
} mat4;

static inline mat4 m4_identity();
static inline mat4 m4_mul(const mat4 a, const mat4 b);
static inline mat4 m4_mulf(const mat4 in, float f);
static inline mat4 m4_transpose(const mat4 in);
static inline mat4 m4_scale_uni(const mat4 in, float f);
static inline mat4 m4_scale(const mat4 in, float x, float y, float z);
static inline mat4 m4_scale_vec(const mat4 in, const vec3 v);
static inline mat4 m4_translate(const mat4 in, float x, float y, float z);
static inline mat4 m4_translate_vec(const mat4 in, const vec3 v);
static inline mat4 m4_rotate(const mat4 in, const vec3 axis, float angle_rad);
static inline mat4 m4_rotateX(const mat4 in, float angle_rad);
static inline mat4 m4_rotateY(const mat4 in, float angle_rad);
static inline mat4 m4_rotateZ(const mat4 in, float angle_rad);
static inline mat4 m4_lookat(const vec3 eye, const vec3 target, const vec3 up);
static inline mat4 m4_ortho1(float width, float height, float zNear, float zFar);
static inline mat4 m4_ortho2(float left, float right, float bottom, float top, float zNear, float zFar);
static inline mat4 m4_perspective(float fov, float aspect, float zNear, float zFar);
static inline mat4 m4_inverse();

// QUATERNION TYPE ////////////////////////////////////////////////////////////////////////////////////////////////////
