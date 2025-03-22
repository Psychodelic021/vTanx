#pragma once

// DEFINES ////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define PI 3.1415926535897932384626433f

// FUNCTIONS //////////////////////////////////////////////////////////////////////////////////////////////////////////

float radians(float angle_deg);

// VECTOR 2 FLOAT TYPE ////////////////////////////////////////////////////////////////////////////////////////////////

struct vec2 {
    
    float x, y;

    vec2(float f);
    vec2(float x, float y);
    vec2(vec2& src);
    
    void operator= (vec2 src);
};

vec2 operator- (vec2 a);
vec2 operator+ (vec2 a, vec2 b);
vec2 operator- (vec2 a, vec2 b);
vec2 operator* (vec2 a, vec2 b);
vec2 operator/ (vec2 a, vec2 b);
vec2 operator- (vec2 a);
void operator/= (vec2& a, vec2 b);
vec2 operator* (vec2 a, float f);
vec2 operator/ (vec2 a, float f);
vec2 operator* (float f, vec2 vec);
void operator*= (vec2& a, float f);
void operator/= (vec2& a, float f);

float dot(vec2 a, vec2 b);
vec2 normalize(vec2 a);

// VECTOR 3 FLOAT TYPE ////////////////////////////////////////////////////////////////////////////////////////////////

struct vec3 { 

    float x, y, z;

    vec3(float f);
    vec3(float x, float y, float z);
    vec3(vec3& src);

    void operator= (vec3 src);
};

vec3 operator- (vec3 a);

vec3 operator+ (vec3 a, vec3 b);
vec3 operator- (vec3 a, vec3 b);
vec3 operator* (vec3 a, vec3 b);
vec3 operator/ (vec3 a, vec3 b);

void operator+= (vec3& a, vec3 b);
void operator-= (vec3& a, vec3 b);
void operator*= (vec3& a, vec3 b);
void operator/= (vec3& a, vec3 b);

vec3 operator* (vec3 a, float f);
vec3 operator* (float f, vec3 vec);
vec3 operator/ (vec3 a, float f);

void operator*= (vec3& a, float f);
void operator/= (vec3& a, float f);

float dot(vec3 a, vec3 b);
vec3 cross(vec3 a, vec3 b);
vec3 normalize(vec3 a);

// VECTOR 4 FLOAT TYPE ////////////////////////////////////////////////////////////////////////////////////////////////

struct vec4 { 
    float x, y, z, w;

    vec4(float f);
    vec4(float x, float y, float z, float w);
    vec4(vec4& src);
    
    void operator= (vec4 src);
};

// MATRIX 4 FLOAT TYPE ////////////////////////////////////////////////////////////////////////////////////////////////

struct mat4 {
    float idx[4][4];
    
    mat4(float f);
    mat4(mat4& src);
    void operator= (mat4 src);
};

mat4 operator* (mat4 a, mat4 b);
void operator*= (mat4& out, mat4 in);
mat4 operator* (mat4 in, float f);
mat4 operator* (float f, mat4 in);
void operator*= (mat4& out, float f);

mat4 transpose(mat4 a);
mat4 scale(mat4 a, float f);
mat4 scale(mat4 a, float x, float y, float z);
mat4 scale(mat4 a, vec3& b);
mat4 translate(mat4 a, float x, float y, float z);
mat4 translate(mat4 a, vec3 b);
mat4 rotate(mat4 a, vec3 axis, float angle_rad);
mat4 rotateZ(mat4& a, float angle_rad);
mat4 rotateY(mat4& a, float angle_rad);
mat4 rotateX(mat4 a, float angle_rad);
mat4 lookAt(vec3 eye, vec3 target, vec3 up);
mat4 ortho(float width, float height, float zNear, float zFar);
mat4 ortho(float left, float right, float bottom, float top, float zNear, float zFar);
mat4 perspective(float fov, float aspect, float zNear, float zFar);
mat4 inverse();

// QUATERNION TYPE ////////////////////////////////////////////////////////////////////////////////////////////////////
