#pragma once

#include <math.h>
#include <assert.h>

// DEFINES ////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define PI 3.1415926535897932384626433f

// FUNCTIONS //////////////////////////////////////////////////////////////////////////////////////////////////////////

float radians(float angle_deg);

// VECTOR 2 FLOAT TYPE ////////////////////////////////////////////////////////////////////////////////////////////////

struct vec2 {
    
    float x, y;

    vec2(float f);
    vec2(float x, float y);
    vec2(const vec2& src);
    
    void operator= (const vec2 src);
};

vec2 operator- (const vec2& a);
vec2 operator+ (const vec2& a, const vec2& b);
vec2 operator- (const vec2& a, const vec2& b);
vec2 operator* (const vec2& a, const vec2& b);
vec2 operator/ (const vec2& a, const vec2& b);
vec2 operator- (const vec2& a);
void operator/= (vec2& a, const vec2& b);
vec2 operator* (const vec2& a, float f);
vec2 operator/ (const vec2& a, float f);
vec2 operator* (const float f, const vec2& vec);
void operator*= (vec2& a, float f);
void operator/= (vec2& a, const float f);

float dot(const vec2& a, const vec2& b);
vec2 normalize(const vec2& a);

// VECTOR 3 FLOAT TYPE ////////////////////////////////////////////////////////////////////////////////////////////////

struct vec3 { 

    float x, y, z;

    vec3(float f);
    vec3(float x, float y, float z);
    vec3(const vec3& src);

    void operator= (const vec3 src);
};

vec3 operator- (const vec3 a);
vec3 operator+ (const vec3 a, const vec3 b);
vec3 operator- (const vec3 a, const vec3 b);
vec3 operator* (const vec3 a, const vec3 b);
vec3 operator/ (const vec3 a, const vec3 b);
void operator+= (vec3& a, const vec3 b);
void operator-= (vec3& a, const vec3 b);
void operator*= (vec3& a, const vec3 b);
void operator/= (vec3& a, const vec3 b);
vec3 operator* (const vec3 a, float f);
vec3 operator/ (const vec3 a, float f);
vec3 operator* (const float f, const vec3 vec);
void operator*= (vec3 a, float f);
void operator/= (vec3 a, const float f);

float dot(const vec3 a, const vec3 b);
vec3 cross(const vec3 a, const vec3 b);
vec3 normalize(const vec3& a);

// VECTOR 4 FLOAT TYPE ////////////////////////////////////////////////////////////////////////////////////////////////

struct vec4 { 
    float x, y, z, w;

    vec4(float f);
    vec4(float x, float y, float z, float w);
    vec4(const vec4& src);
    
    void operator= (const vec4 src);
};

// MATRIX 4 FLOAT TYPE ////////////////////////////////////////////////////////////////////////////////////////////////

struct mat4 {
    float idx[4][4];
    
    mat4(float f);
    mat4(const mat4& src);
    void operator= (const mat4 src);
};

mat4 operator* (const mat4 a, const mat4 b);
void operator*= (mat4& out, const mat4 in);
mat4 operator* (const mat4 in, float f);
mat4 operator* (float f, const mat4 in);
void operator*= (mat4& out, float f);

mat4 transpose(const mat4 a);
mat4 scale(const mat4 a, float f);
mat4 scale(const mat4 a, float x, float y, float z);
mat4 scale(const mat4 a, const vec3& b);
mat4 translate(const mat4 a, float x, float y, float z);
mat4 translate(const mat4 a, const vec3 b);
mat4 rotate(const mat4 a, const vec3 axis, float angle_rad);
mat4 rotateZ(const mat4& a, float angle_rad);
mat4 rotateY(const mat4& a, float angle_rad);
mat4 rotateX(const mat4 a, float angle_rad);
mat4 lookAt(const vec3 eye, const vec3 target, const vec3 up);
mat4 ortho(float width, float height, float zNear, float zFar);
mat4 ortho(float left, float right, float bottom, float top, float zNear, float zFar);
mat4 perspective(float fov, float aspect, float zNear, float zFar);
mat4 inverse();

// QUATERNION TYPE ////////////////////////////////////////////////////////////////////////////////////////////////////
