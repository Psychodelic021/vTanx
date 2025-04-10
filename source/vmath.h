#pragma once

#include <math.h>
#include <assert.h>

// DEFINES ////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define PI 3.1415926535897932384626433f

// FUNCTIONS //////////////////////////////////////////////////////////////////////////////////////////////////////////

float to_radians(float angle_deg);

// VECTOR 3 TYPE ////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct vector2 {
    float x, y;
} vec2;

vec2 v2_neg(const vec2 a);
vec2 v2_add(const vec2 a, const vec2 b);
vec2 v2_sub(const vec2 a, const vec2 b);
vec2 v2_mulf(const vec2 a, float f);
vec2 v2_pairwise(const vec2 a, const vec2 b);
float v2_dot(const vec2 a, const vec2 b);
float v2_len(const vec2 in);
vec2 v2_normalize(const vec2 in);

// VECTOR 3 TYPE ////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct vector3 {
    float x, y, z;
} vec3;

vec3 v3_neg(const vec3 a);
vec3 v3_add(const vec3 a, const vec3 b);
vec3 v3_sub(const vec3 a, const vec3 b);
vec3 v3_mulf(const vec3 a, float f);
vec3 v3_pairwise(const vec3 a, const vec3 b);
float v3_dot(const vec3 a, const vec3 b);
vec3 cross(const vec3 a, const vec3 b);
float v3_len(const vec3 in);
vec3 v3_normalize(const vec3 in);

// MATRIX 4 TYPE ////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct matrix4 {
    float idx[4][4];
} mat4;

mat4 identity();
mat4 m4_mul(const mat4 a, const mat4 b);
mat4 m4_mulf(const mat4 in, float f);
mat4 transpose(const mat4 in);
mat4 scale_uni(const mat4 in, float f);
mat4 scale(const mat4 in, float x, float y, float z);
mat4 scale_vec(const mat4 in, const vec3 v);
mat4 translate(const mat4 in, float x, float y, float z);
mat4 translate_vec(const mat4 in, const vec3 v);
mat4 rotate(const mat4 in, const vec3 axis, float angle_rad);
mat4 rotateX(const mat4 in, float angle_rad);
mat4 rotateY(const mat4 in, float angle_rad);
mat4 rotateZ(const mat4 in, float angle_rad);
mat4 lookAt(const vec3 eye, const vec3 target, const vec3 up);
mat4 ortho1(float width, float height, float zNear, float zFar);
mat4 ortho2(float left, float right, float bottom, float top, float zNear, float zFar);
mat4 perspective(float fov, float aspect, float zNear, float zFar);
mat4 m4_inverse();

// QUATERNION TYPE ////////////////////////////////////////////////////////////////////////////////////////////////
