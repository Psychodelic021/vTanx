#pragma once

#include <math.h>
#include <assert.h>

#include "vmatx.h"

// DEFINES ////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define PI 3.1415926535897932384626433f

// FUNCTIONS //////////////////////////////////////////////////////////////////////////////////////////////////////////

float radians(float angle_deg)
{
    return angle_deg * PI / 180.f;
}

// VECTOR 2 FLOAT TYPE ////////////////////////////////////////////////////////////////////////////////////////////////

vec2::vec2(float f) : x(f), y(f) {}
vec2::vec2(float x, float y) : x(x), y(y) {}
vec2::vec2(const vec2& src) : x(src.x), y(src.y) {};

void vec2::operator= (const vec2 src) { x = src.x; y = src.y; }

vec2 operator- (const vec2& a)
{
    return vec2(-a.x, -a.y);
}

vec2 operator+ (const vec2& a, const vec2& b)
{
    return vec2(a.x + b.x, a.y + b.y);
}

vec2 operator- (const vec2& a, const vec2& b)
{
    return vec2(a.x - b.x, a.y - b.y);
}

vec2 operator* (const vec2& a, const vec2& b)
{
    return vec2(a.x * b.x, a.y * b.y);
}

 vec2 operator/ (const vec2& a, const vec2& b)
{
    assert(b.x != 0.f && b.y != 0.f);
    return vec2(a.x / b.x, a.y / b.y);
}

void operator+= (vec2& a, const vec2& b)
{
    a.x += b.x;
    a.y += b.y;
}

void operator-= (vec2& a, const vec2& b)
{
    a.x -= b.x;
    a.y -= b.y;
}

void operator*= (vec2& a, const vec2& b)
{
    a.x *= b.x;
    a.y *= b.y;
}

void operator/= (vec2& a, const vec2& b)
{
    assert(b.x != 0.f && b.y != 0.f);
    a.x /= b.x;
    a.y /= b.y;
}

vec2 operator* (const vec2& a, float f)
{
    return vec2(a.x * f, a.y * f);
}

vec2 operator/ (const vec2& a, float f)
{
    assert(f != 0.0f);
    return vec2(a.x / f, a.y / f);
}

vec2 operator* (const float f, const vec2& vec)
{
    return vec2(f * vec.x, f * vec.y);
}

void operator*= (vec2& a, float f)
{
    a.x *= f;
    a.y *= f;
}

void operator/= (vec2& a, const float f)
{
    assert(f != 0.0f);
    a.x /= f;
    a.y /= f;
}

float dot(const vec2& a, const vec2& b)
{
    return a.x * b.x + a.y * b.y;
}

vec2 normalize(const vec2& a)
{
    float inverse_len = 1.0f / sqrtf(a.x * a.x + a.y * a.y);
    return vec2(a.x * inverse_len, a.y * inverse_len);
}

vec2 reflect(vec2& out, const vec2& a, const vec2& b)
{
    float k = dot(a, b) * 2.f;
    out.x = a.x - b.x * k;
    out.y = a.y - b.y * k;
}

// VECTOR 3 FLOAT TYPE ////////////////////////////////////////////////////////////////////////////////////////////////

vec3::vec3(float f) : x(f), y(f), z(f) {}
vec3::vec3(float x, float y, float z) : x(x), y(y), z(z) {}
vec3::vec3(const vec3& src) : x(src.x), y(src.y), z(src.z) {}

void vec3::operator= (const vec3 src)
{   
    x = src.x;
    y = src.y;
    z = src.z;
}

vec3 operator- (const vec3 a)
{
    return vec3(-a.x, -a.y, -a.z);
}

vec3 operator+ (const vec3 a, const vec3 b)
{
    return vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

vec3 operator- (const vec3 a, const vec3 b)
{
    return vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

vec3 operator* (const vec3 a, const vec3 b)
{
    return vec3(a.x * b.x, a.y * b.y, a.z * b.z);
}

vec3 operator/ (const vec3 a, const vec3 b)
{
    assert(b.x != 0.f && b.y != 0.f && b.z != 0.f);
    return vec3(a.x / b.x, a.y / b.y, a.z / b.z);
}

void operator+= (vec3& a, const vec3 b)
{
    a.x += b.x;
    a.y += b.y;
    a.z += b.z;
}

void operator-= (vec3& a, const vec3 b)
{
    a.x -= b.x;
    a.y -= b.y;
    a.z -= b.z;
}

void operator*= (vec3& a, const vec3 b)
{
    a.x *= b.x;
    a.y *= b.y;
    a.z *= b.z;
}

void operator/= (vec3& a, const vec3 b)
{
    assert(b.x != 0.f && b.y != 0.f && b.z != 0.f);
    a.x /= b.x;
    a.y /= b.y;
    a.z /= b.z;
}

vec3 operator* (const vec3 a, float f)
{
    return vec3(a.x * f, a.y * f, a.z * f);
}

vec3 operator/ (const vec3 a, float f)
{
    assert(f != 0.0f);
    return vec3(a.x / f, a.y / f, a.z / f);
}

 vec3 operator* (const float f, const vec3 vec)
{
    return vec3(f * vec.x, f * vec.y, f * vec.z);
}

void operator*= (vec3 a, float f)
{
    a.x *= f;
    a.y *= f;
    a.z *= f;
}

void operator/= (vec3 a, const float f)
{
    assert(f != 0.0f);
    a.x /= f;
    a.y /= f;
    a.z /= f;
}

float dot(const vec3 a, const vec3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

vec3 cross(const vec3 a, const vec3 b)
{
    return vec3 {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

vec3 normalize(const vec3& a)
{
    float inverse_len = 1.0f / sqrtf(a.x * a.x + a.y * a.y + a.z * a.z);
    return vec3(a.x * inverse_len, a.y * inverse_len, a.z * inverse_len);
}

// VECTOR 4 FLOAT TYPE ////////////////////////////////////////////////////////////////////////////////////////////////

vec4::vec4(float f) : x(f), y(f), z(f), w(f) {}
vec4::vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
vec4::vec4(const vec4& src) : x(src.x), y(src.y), z(src.z), w(src.w) {}

void vec4::operator= (const vec4 src)
{
    x = src.x;
    y = src.y;
    z = src.z;
    w = src.w;
}

// MATRIX 4 FLOAT TYPE ////////////////////////////////////////////////////////////////////////////////////////////////

mat4::mat4(float f)
{
    idx[0][0] = f;
    idx[1][1] = f;
    idx[2][2] = f;
    idx[3][3] = f;
}

mat4::mat4(const mat4& src)
{
    idx[0][0] = src.idx[0][0]; idx[0][1] = src.idx[0][1]; idx[0][2] = src.idx[0][2]; idx[0][3] = src.idx[0][3];
    idx[1][0] = src.idx[1][0]; idx[1][1] = src.idx[1][1]; idx[1][2] = src.idx[1][2]; idx[1][3] = src.idx[1][3];
    idx[2][0] = src.idx[2][0]; idx[2][1] = src.idx[2][1]; idx[2][2] = src.idx[2][2]; idx[2][3] = src.idx[2][3];
    idx[3][0] = src.idx[3][0]; idx[3][1] = src.idx[3][1]; idx[3][2] = src.idx[3][2]; idx[3][3] = src.idx[3][3];
}

void mat4::operator= (const mat4 src)
{
    idx[0][0] = src.idx[0][0]; idx[0][1] = src.idx[0][1]; idx[0][2] = src.idx[0][2]; idx[0][3] = src.idx[0][3];
    idx[1][0] = src.idx[1][0]; idx[1][1] = src.idx[1][1]; idx[1][2] = src.idx[1][2]; idx[1][3] = src.idx[1][3];
    idx[2][0] = src.idx[2][0]; idx[2][1] = src.idx[2][1]; idx[2][2] = src.idx[2][2]; idx[2][3] = src.idx[2][3];
    idx[3][0] = src.idx[3][0]; idx[3][1] = src.idx[3][1]; idx[3][2] = src.idx[3][2]; idx[3][3] = src.idx[3][3];
}

mat4 operator* (const mat4 a, const mat4 b)
{
    mat4 out(0);

    out.idx[0][0] = a.idx[0][0] * b.idx[0][0] + a.idx[0][1] * b.idx[1][0] + a.idx[0][2] * b.idx[2][0] + a.idx[0][3] * b.idx[3][0];
    out.idx[0][1] = a.idx[0][0] * b.idx[0][1] + a.idx[0][1] * b.idx[1][1] + a.idx[0][2] * b.idx[2][1] + a.idx[0][3] * b.idx[3][1];
    out.idx[0][2] = a.idx[0][0] * b.idx[0][2] + a.idx[0][1] * b.idx[1][2] + a.idx[0][2] * b.idx[2][2] + a.idx[0][3] * b.idx[3][2];
    out.idx[0][3] = a.idx[0][0] * b.idx[0][3] + a.idx[0][1] * b.idx[1][3] + a.idx[0][2] * b.idx[2][3] + a.idx[0][3] * b.idx[3][3];

    out.idx[1][0] = a.idx[1][0] * b.idx[0][0] + a.idx[1][1] * b.idx[1][0] + a.idx[1][2] * b.idx[2][0] + a.idx[1][3] * b.idx[3][0];
    out.idx[1][1] = a.idx[1][0] * b.idx[0][1] + a.idx[1][1] * b.idx[1][1] + a.idx[1][2] * b.idx[2][1] + a.idx[1][3] * b.idx[3][1];
    out.idx[1][2] = a.idx[1][0] * b.idx[0][2] + a.idx[1][1] * b.idx[1][2] + a.idx[1][2] * b.idx[2][2] + a.idx[1][3] * b.idx[3][2];
    out.idx[1][3] = a.idx[1][0] * b.idx[0][3] + a.idx[1][1] * b.idx[1][3] + a.idx[1][2] * b.idx[2][3] + a.idx[1][3] * b.idx[3][3];

    out.idx[2][0] = a.idx[2][0] * b.idx[0][0] + a.idx[2][1] * b.idx[1][0] + a.idx[2][2] * b.idx[2][0] + a.idx[2][3] * b.idx[3][0];
    out.idx[2][1] = a.idx[2][0] * b.idx[0][1] + a.idx[2][1] * b.idx[1][1] + a.idx[2][2] * b.idx[2][1] + a.idx[2][3] * b.idx[3][1];
    out.idx[2][2] = a.idx[2][0] * b.idx[0][2] + a.idx[2][1] * b.idx[1][2] + a.idx[2][2] * b.idx[2][2] + a.idx[2][3] * b.idx[3][2];
    out.idx[2][3] = a.idx[2][0] * b.idx[0][3] + a.idx[2][1] * b.idx[1][3] + a.idx[2][2] * b.idx[2][3] + a.idx[2][3] * b.idx[3][3];

    out.idx[3][0] = a.idx[3][0] * b.idx[0][0] + a.idx[3][1] * b.idx[1][0] + a.idx[3][2] * b.idx[2][0] + a.idx[3][3] * b.idx[3][0];
    out.idx[3][1] = a.idx[3][0] * b.idx[0][1] + a.idx[3][1] * b.idx[1][1] + a.idx[3][2] * b.idx[2][1] + a.idx[3][3] * b.idx[3][1];
    out.idx[3][2] = a.idx[3][0] * b.idx[0][2] + a.idx[3][1] * b.idx[1][2] + a.idx[3][2] * b.idx[2][2] + a.idx[3][3] * b.idx[3][2];
    out.idx[3][3] = a.idx[3][0] * b.idx[0][3] + a.idx[3][1] * b.idx[1][3] + a.idx[3][2] * b.idx[2][3] + a.idx[3][3] * b.idx[3][3];

    return out;
}

void operator*= (mat4& out, const mat4 in)
{
    mat4 t = out;

    out.idx[0][0] = t.idx[0][0] * in.idx[0][0] + t.idx[0][1] * in.idx[1][0] + t.idx[0][2] * in.idx[2][0] + t.idx[0][3] * in.idx[3][0];
    out.idx[0][1] = t.idx[0][0] * in.idx[0][1] + t.idx[0][1] * in.idx[1][1] + t.idx[0][2] * in.idx[2][1] + t.idx[0][3] * in.idx[3][1];
    out.idx[0][2] = t.idx[0][0] * in.idx[0][2] + t.idx[0][1] * in.idx[1][2] + t.idx[0][2] * in.idx[2][2] + t.idx[0][3] * in.idx[3][2];
    out.idx[0][3] = t.idx[0][0] * in.idx[0][3] + t.idx[0][1] * in.idx[1][3] + t.idx[0][2] * in.idx[2][3] + t.idx[0][3] * in.idx[3][3];

    out.idx[1][0] = t.idx[1][0] * in.idx[0][0] + t.idx[1][1] * in.idx[1][0] + t.idx[1][2] * in.idx[2][0] + t.idx[1][3] * in.idx[3][0];
    out.idx[1][1] = t.idx[1][0] * in.idx[0][1] + t.idx[1][1] * in.idx[1][1] + t.idx[1][2] * in.idx[2][1] + t.idx[1][3] * in.idx[3][1];
    out.idx[1][2] = t.idx[1][0] * in.idx[0][2] + t.idx[1][1] * in.idx[1][2] + t.idx[1][2] * in.idx[2][2] + t.idx[1][3] * in.idx[3][2];
    out.idx[1][3] = t.idx[1][0] * in.idx[0][3] + t.idx[1][1] * in.idx[1][3] + t.idx[1][2] * in.idx[2][3] + t.idx[1][3] * in.idx[3][3];

    out.idx[2][0] = t.idx[2][0] * in.idx[0][0] + t.idx[2][1] * in.idx[1][0] + t.idx[2][2] * in.idx[2][0] + t.idx[2][3] * in.idx[3][0];
    out.idx[2][1] = t.idx[2][0] * in.idx[0][1] + t.idx[2][1] * in.idx[1][1] + t.idx[2][2] * in.idx[2][1] + t.idx[2][3] * in.idx[3][1];
    out.idx[2][2] = t.idx[2][0] * in.idx[0][2] + t.idx[2][1] * in.idx[1][2] + t.idx[2][2] * in.idx[2][2] + t.idx[2][3] * in.idx[3][2];
    out.idx[2][3] = t.idx[2][0] * in.idx[0][3] + t.idx[2][1] * in.idx[1][3] + t.idx[2][2] * in.idx[2][3] + t.idx[2][3] * in.idx[3][3];

    out.idx[3][0] = t.idx[3][0] * in.idx[0][0] + t.idx[3][1] * in.idx[1][0] + t.idx[3][2] * in.idx[2][0] + t.idx[3][3] * in.idx[3][0];
    out.idx[3][1] = t.idx[3][0] * in.idx[0][1] + t.idx[3][1] * in.idx[1][1] + t.idx[3][2] * in.idx[2][1] + t.idx[3][3] * in.idx[3][1];
    out.idx[3][2] = t.idx[3][0] * in.idx[0][2] + t.idx[3][1] * in.idx[1][2] + t.idx[3][2] * in.idx[2][2] + t.idx[3][3] * in.idx[3][2];
    out.idx[3][3] = t.idx[3][0] * in.idx[0][3] + t.idx[3][1] * in.idx[1][3] + t.idx[3][2] * in.idx[2][3] + t.idx[3][3] * in.idx[3][3];
}

mat4 operator* (const mat4 in, float f)
{
    mat4 out(0);

    out.idx[0][0] = in.idx[0][0] * f; out.idx[0][1] = in.idx[0][1] * f; out.idx[0][2] = in.idx[0][2] * f; out.idx[0][3] = in.idx[0][3] * f;
    out.idx[1][0] = in.idx[1][0] * f; out.idx[1][1] = in.idx[1][1] * f; out.idx[1][2] = in.idx[1][2] * f; out.idx[1][3] = in.idx[1][3] * f;
    out.idx[2][0] = in.idx[2][0] * f; out.idx[2][1] = in.idx[2][1] * f; out.idx[2][2] = in.idx[2][2] * f; out.idx[2][3] = in.idx[2][3] * f;
    out.idx[3][0] = in.idx[3][0] * f; out.idx[3][1] = in.idx[3][1] * f; out.idx[3][2] = in.idx[3][2] * f; out.idx[3][3] = in.idx[3][3] * f;

    return out;
}

mat4 operator* (float f, const mat4 in)
{
    mat4 out(0);

    out.idx[0][0] = in.idx[0][0] * f; out.idx[0][1] = in.idx[0][1] * f; out.idx[0][2] = in.idx[0][2] * f; out.idx[0][3] = in.idx[0][3] * f;
    out.idx[1][0] = in.idx[1][0] * f; out.idx[1][1] = in.idx[1][1] * f; out.idx[1][2] = in.idx[1][2] * f; out.idx[1][3] = in.idx[1][3] * f;
    out.idx[2][0] = in.idx[2][0] * f; out.idx[2][1] = in.idx[2][1] * f; out.idx[2][2] = in.idx[2][2] * f; out.idx[2][3] = in.idx[2][3] * f;
    out.idx[3][0] = in.idx[3][0] * f; out.idx[3][1] = in.idx[3][1] * f; out.idx[3][2] = in.idx[3][2] * f; out.idx[3][3] = in.idx[3][3] * f;

    return out;
}

void operator*= (mat4& out, float f)
{
    out.idx[0][0] = out.idx[0][0] * f; out.idx[0][1] = out.idx[0][1] * f; out.idx[0][2] = out.idx[0][2] * f; out.idx[0][3] = out.idx[0][3] * f;
    out.idx[1][0] = out.idx[1][0] * f; out.idx[1][1] = out.idx[1][1] * f; out.idx[1][2] = out.idx[1][2] * f; out.idx[1][3] = out.idx[1][3] * f;
    out.idx[2][0] = out.idx[2][0] * f; out.idx[2][1] = out.idx[2][1] * f; out.idx[2][2] = out.idx[2][2] * f; out.idx[2][3] = out.idx[2][3] * f;
    out.idx[3][0] = out.idx[3][0] * f; out.idx[3][1] = out.idx[3][1] * f; out.idx[3][2] = out.idx[3][2] * f; out.idx[3][3] = out.idx[3][3] * f;
}

 mat4 transpose(const mat4 a)
{
    mat4 out(0);

    out.idx[0][0] = a.idx[0][0]; out.idx[0][1] = a.idx[1][0]; out.idx[0][2] = a.idx[2][0]; out.idx[0][3] = a.idx[3][0];
    out.idx[1][0] = a.idx[0][1]; out.idx[1][1] = a.idx[1][1]; out.idx[1][2] = a.idx[2][1]; out.idx[1][3] = a.idx[3][1];
    out.idx[2][0] = a.idx[0][2]; out.idx[2][1] = a.idx[1][2]; out.idx[2][2] = a.idx[2][2]; out.idx[2][3] = a.idx[3][2];
    out.idx[3][0] = a.idx[0][3]; out.idx[3][1] = a.idx[1][3]; out.idx[3][2] = a.idx[2][3]; out.idx[3][3] = a.idx[3][3];

    return out;
}

 mat4 scale(const mat4 a, float f)
{
    mat4 scaling(0);

    scaling.idx[0][0] = f;
    scaling.idx[1][1] = f;
    scaling.idx[2][2] = f;
    scaling.idx[3][3] = 1.0f;

    return a * scaling;
}

 mat4 scale(const mat4 a, float x, float y, float z)
{
    mat4 scaling(0);

    scaling.idx[0][0] = x;
    scaling.idx[1][1] = y;
    scaling.idx[2][2] = z;
    scaling.idx[3][3] = 1.f;

    return a * scaling;
}

 mat4 scale(const mat4 a, const vec3& b)
{
    mat4 scaling(0);

    scaling.idx[0][0] = b.x;
    scaling.idx[1][1] = b.y;
    scaling.idx[2][2] = b.z;
    scaling.idx[3][3] = 1.f;

    return a * scaling;
}

 mat4 translate(const mat4 a, float x, float y, float z)
{
    mat4 trans(1);

    trans.idx[0][3] = x;
    trans.idx[1][3] = y;
    trans.idx[2][3] = z;

    return a * trans;
}

 mat4 translate(const mat4 a, const vec3 b)
{
    mat4 trans(1);

    trans.idx[0][3] = b.x;
    trans.idx[1][3] = b.y;
    trans.idx[2][3] = b.z;

    return a * trans;
}

 mat4 rotate(const mat4 a, const vec3 axis, float angle_rad)
{
    float s = sinf(angle_rad);
    float c = cosf(angle_rad);
    float nc = 1.f - c;

    vec3 n_axis = normalize(axis);

    mat4 rot(0);

    rot.idx[0][0] = n_axis.x * n_axis.x * nc + c;
    rot.idx[0][1] = n_axis.x * n_axis.y * nc - s * n_axis.z;
    rot.idx[0][2] = n_axis.x * n_axis.z * nc + s * n_axis.y;
    rot.idx[1][0] = n_axis.y * n_axis.x * nc + s * n_axis.z;
    rot.idx[1][1] = n_axis.y * n_axis.y * nc + c;
    rot.idx[1][2] = n_axis.y * n_axis.z * nc - s * n_axis.x;
    rot.idx[2][0] = n_axis.z * n_axis.x * nc - s * n_axis.y;
    rot.idx[2][1] = n_axis.z * n_axis.y * nc + s * n_axis.x;
    rot.idx[2][2] = n_axis.z * n_axis.z * nc + c;
    rot.idx[3][3] = 1.0f;

    return a * rot;
}

 mat4 rotateZ(const mat4& a, float angle_rad)
{
    float s = sinf(angle_rad);
    float c = cosf(angle_rad);

    mat4 rot(0);

    rot.idx[0][0] = c;
    rot.idx[0][1] = -s;
    rot.idx[1][0] = s;
    rot.idx[1][1] = c;
    rot.idx[2][2] = 1.0f;
    rot.idx[3][3] = 1.0f;

    return a * rot;
}

 mat4 rotateY(const mat4& a, float angle_rad)
{
    float s = sinf(angle_rad);
    float c = cosf(angle_rad);

    mat4 rot(0);

    rot.idx[0][0] = c;
    rot.idx[0][2] = s;
    rot.idx[1][1] = 1.0f;
    rot.idx[2][0] = -s;
    rot.idx[2][2] = c;
    rot.idx[3][3] = 1.0f;

    return a * rot;
}

 mat4 rotateX(const mat4 a, float angle_rad)
{
    float s = sinf(angle_rad);
    float c = cosf(angle_rad);

    mat4 temp(0);

    temp.idx[0][0] = 1.0f;
    temp.idx[1][1] = c;
    temp.idx[1][2] = -s;
    temp.idx[2][1] = s;
    temp.idx[2][2] = c;
    temp.idx[3][3] = 1.0f;

    return a * temp;
}

 mat4 lookAt(const vec3 eye, const vec3 target, const vec3 up)
{
    vec3 view_z = normalize(target - eye);
    vec3 view_x = normalize(cross(view_z, up));
    vec3 view_y = cross(view_x, view_z);
    view_z = -view_z;

    mat4 out(0);

    out.idx[0][0] = view_x.x; out.idx[0][1] = view_x.y; out.idx[0][2] = view_x.z; out.idx[0][3] = -dot(view_x, eye);
    out.idx[1][0] = view_y.x; out.idx[1][1] = view_y.y; out.idx[1][2] = view_y.z; out.idx[1][3] = -dot(view_y, eye);
    out.idx[2][0] = view_z.x; out.idx[2][1] = view_z.y; out.idx[2][2] = view_z.z; out.idx[2][3] = -dot(view_z, eye);
    out.idx[3][0] = 0.f;      out.idx[3][1] = 0.f;      out.idx[3][2] = 0.f;      out.idx[3][3] = 1.f;

    return out;
}

 mat4 ortho(float width, float height, float zNear, float zFar)
{
    mat4 out(0);

    out.idx[0][0] = 2.0f / width;
    out.idx[1][1] = 2.0f / height;
    out.idx[2][2] = -2.0f / (zFar - zNear);
    out.idx[0][3] = -1.0f;
    out.idx[1][3] = -1.0f;
    out.idx[2][3] = -(zFar + zNear) / (zFar - zNear);
    out.idx[3][3] = 1.0f;

    return out;
}

 mat4 ortho(float left, float right, float bottom, float top, float zNear, float zFar)
{
    mat4 out(0);

    out.idx[0][0] = 2.0f / (right - left);
    out.idx[1][1] = 2.0f / (top - bottom);
    out.idx[2][2] = -2.0f / (zFar - zNear);
    out.idx[0][3] = -(right + left) / (right - left);
    out.idx[1][3] = -(top + bottom) / (top - bottom);
    out.idx[2][3] = -(zFar + zNear) / (zFar - zNear);
    out.idx[3][3] = 1.0f;

    return out;
}

 mat4 perspective(float fov, float aspect, float zNear, float zFar)
{
    float scale = 1.0f / tanf(fov * 0.5f);

    mat4 out(0);

    out.idx[0][0] = scale;
    out.idx[1][1] = scale * aspect;
    out.idx[2][2] = (zFar + zNear) / (zNear - zFar);
    out.idx[2][3] = 2.0f * zFar * zNear / (zNear - zFar);
    out.idx[3][2] = -1.0f;

    return out;
}

 mat4 inverse()
{
    return mat4(0);
}

// QUATERNION TYPE ////////////////////////////////////////////////////////////////////////////////////////////////
