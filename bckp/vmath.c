#pragma once

#include "vmath.h"

// DEFINES ////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define PI 3.1415926535897932384626433f

// FUNCTIONS //////////////////////////////////////////////////////////////////////////////////////////////////////////

static inline float to_radians(float angle_deg)
{
    return angle_deg * PI / 180.f;
}

// VECTOR 3 TYPE ////////////////////////////////////////////////////////////////////////////////////////////////

static inline vec2 v2_neg(const vec2 a)
{
    return (vec2) { -a.x, -a.y };
}

static inline vec2 v2_add(const vec2 a, const vec2 b)
{
    return (vec2) { a.x + b.x, a.y + b.y };
}

static inline vec2 v2_sub(const vec2 a, const vec2 b)
{
    return (vec2) { a.x - b.x, a.y - b.y };
}

static inline vec2 v2_mulf(const vec2 a, float f)
{
    return (vec2) { a.x * f, a.y * f };
}

static inline vec2 v2_pairwise(const vec2 a, const vec2 b)
{
    return (vec2) { a.x * b.x, a.y * b.y };
}

static inline float v2_dot(const vec2 a, const vec2 b)
{
    return a.x * b.x + a.y * b.y;
}

static inline float v2_len(const vec2 in)
{
    return sqrtf(in.x * in.x + in.y * in.y);
}

static inline vec2 v2_normalize(const vec2 in)
{
    if (in.x == 0 && in.y == 0) return in;
    float inv_len = 1.0f / v2_len(in);
    return (vec2) { in.x * inv_len, in.y * inv_len};
}

// VECTOR 3 TYPE ////////////////////////////////////////////////////////////////////////////////////////////////


static inline vec3 v3_neg(const vec3 a)
{
    return (vec3) { -a.x, -a.y, -a.z };
}

static inline vec3 v3_add(const vec3 a, const vec3 b)
{
    return (vec3) { a.x + b.x, a.y + b.y, a.z + b.z };
}

static inline vec3 v3_sub(const vec3 a, const vec3 b)
{
    return (vec3) { a.x - b.x, a.y - b.y, a.z - b.z };
}

static inline vec3 v3_mulf(const vec3 a, float f)
{
    return (vec3) { a.x * f, a.y * f, a.z * f };
}

static inline vec3 v3_pairwise(const vec3 a, const vec3 b)
{
    return (vec3) { a.x * b.x, a.y * b.y, a.z * b.z };
}

static inline float v3_dot(const vec3 a, const vec3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

static inline vec3 v3_cross(const vec3 a, const vec3 b)
{
    return (vec3) {

        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

static inline float v3_len(const vec3 in)
{
    return sqrtf(in.x * in.x + in.y * in.y + in.z * in.z);
}

static inline vec3 v3_normalize(const vec3 in)
{
    if (in.x == 0 && in.y == 0 && in.z == 0) return in;
    float inv_len = 1.0f / v3_len(in);
    return (vec3) { in.x * inv_len, in.y * inv_len, in.z * inv_len };
}

// MATRIX 4 TYPE ////////////////////////////////////////////////////////////////////////////////////////////////

static inline mat4 m4_identity()
{
    return (mat4) {
        1.f, 0.f, 0.f, 0.f,
        0.f, 1.f, 0.f, 0.f,
        0.f, 0.f, 1.f, 0.f,
        0.f, 0.f, 0.f, 1.f
    };
}

static inline mat4 m4_mul(const mat4 a, const mat4 b)
{
    return (mat4) {

        a.idx[0][0] * b.idx[0][0] + a.idx[0][1] * b.idx[1][0] + a.idx[0][2] * b.idx[2][0] + a.idx[0][3] * b.idx[3][0],
        a.idx[0][0] * b.idx[0][1] + a.idx[0][1] * b.idx[1][1] + a.idx[0][2] * b.idx[2][1] + a.idx[0][3] * b.idx[3][1],
        a.idx[0][0] * b.idx[0][2] + a.idx[0][1] * b.idx[1][2] + a.idx[0][2] * b.idx[2][2] + a.idx[0][3] * b.idx[3][2],
        a.idx[0][0] * b.idx[0][3] + a.idx[0][1] * b.idx[1][3] + a.idx[0][2] * b.idx[2][3] + a.idx[0][3] * b.idx[3][3],

        a.idx[1][0] * b.idx[0][0] + a.idx[1][1] * b.idx[1][0] + a.idx[1][2] * b.idx[2][0] + a.idx[1][3] * b.idx[3][0],
        a.idx[1][0] * b.idx[0][1] + a.idx[1][1] * b.idx[1][1] + a.idx[1][2] * b.idx[2][1] + a.idx[1][3] * b.idx[3][1],
        a.idx[1][0] * b.idx[0][2] + a.idx[1][1] * b.idx[1][2] + a.idx[1][2] * b.idx[2][2] + a.idx[1][3] * b.idx[3][2],
        a.idx[1][0] * b.idx[0][3] + a.idx[1][1] * b.idx[1][3] + a.idx[1][2] * b.idx[2][3] + a.idx[1][3] * b.idx[3][3],

        a.idx[2][0] * b.idx[0][0] + a.idx[2][1] * b.idx[1][0] + a.idx[2][2] * b.idx[2][0] + a.idx[2][3] * b.idx[3][0],
        a.idx[2][0] * b.idx[0][1] + a.idx[2][1] * b.idx[1][1] + a.idx[2][2] * b.idx[2][1] + a.idx[2][3] * b.idx[3][1],
        a.idx[2][0] * b.idx[0][2] + a.idx[2][1] * b.idx[1][2] + a.idx[2][2] * b.idx[2][2] + a.idx[2][3] * b.idx[3][2],
        a.idx[2][0] * b.idx[0][3] + a.idx[2][1] * b.idx[1][3] + a.idx[2][2] * b.idx[2][3] + a.idx[2][3] * b.idx[3][3],

        a.idx[3][0] * b.idx[0][0] + a.idx[3][1] * b.idx[1][0] + a.idx[3][2] * b.idx[2][0] + a.idx[3][3] * b.idx[3][0],
        a.idx[3][0] * b.idx[0][1] + a.idx[3][1] * b.idx[1][1] + a.idx[3][2] * b.idx[2][1] + a.idx[3][3] * b.idx[3][1],
        a.idx[3][0] * b.idx[0][2] + a.idx[3][1] * b.idx[1][2] + a.idx[3][2] * b.idx[2][2] + a.idx[3][3] * b.idx[3][2],
        a.idx[3][0] * b.idx[0][3] + a.idx[3][1] * b.idx[1][3] + a.idx[3][2] * b.idx[2][3] + a.idx[3][3] * b.idx[3][3]
    };
}

static inline mat4 m4_mulf(const mat4 in, float f)
{
    return (mat4) {

        in.idx[0][0] * f, in.idx[0][1] * f, in.idx[0][2] * f, in.idx[0][3] * f,
        in.idx[1][0] * f, in.idx[1][1] * f, in.idx[1][2] * f, in.idx[1][3] * f,
        in.idx[2][0] * f, in.idx[2][1] * f, in.idx[2][2] * f, in.idx[2][3] * f,
        in.idx[3][0] * f, in.idx[3][1] * f, in.idx[3][2] * f, in.idx[3][3] * f
    };
}

static inline mat4 m4_transpose(const mat4 in)
{
    return (mat4) {

        in.idx[0][0], in.idx[1][0], in.idx[2][0], in.idx[3][0],
        in.idx[0][1], in.idx[1][1], in.idx[2][1], in.idx[3][1],
        in.idx[0][2], in.idx[1][2], in.idx[2][2], in.idx[3][2],
        in.idx[0][3], in.idx[1][3], in.idx[2][3], in.idx[3][3]
    };
}

static inline mat4 m4_scale_uni(const mat4 in, float f)
{
    mat4 scaling = {0};

    scaling.idx[0][0] = f;
    scaling.idx[1][1] = f;
    scaling.idx[2][2] = f;
    scaling.idx[3][3] = 1.0f;

    return m4_mul(in, scaling);
}

static inline mat4 m4_scale(const mat4 in, float x, float y, float z)
{
    mat4 scaling = {0};

    scaling.idx[0][0] = x;
    scaling.idx[1][1] = y;
    scaling.idx[2][2] = z;
    scaling.idx[3][3] = 1.f;

    return m4_mul(in, scaling);
}

static inline mat4 m4_scale_vec(const mat4 in, const vec3 v)
{
    mat4 scaling = {0};

    scaling.idx[0][0] = v.x;
    scaling.idx[1][1] = v.y;
    scaling.idx[2][2] = v.z;
    scaling.idx[3][3] = 1.f;

    return m4_mul(in, scaling);
}

static inline mat4 m4_translate(const mat4 in, float x, float y, float z)
{
    mat4 trans = m4_identity();

    trans.idx[0][3] = x;
    trans.idx[1][3] = y;
    trans.idx[2][3] = z;

    return m4_mul(in, trans);
}

static inline mat4 m4_translate_vec(const mat4 in, const vec3 v)
{
    mat4 trans = m4_identity();

    trans.idx[0][3] = v.x;
    trans.idx[1][3] = v.y;
    trans.idx[2][3] = v.z;

    return m4_mul(in, trans);
}

static inline mat4 m4_rotate(const mat4 in, const vec3 axis, float angle_rad)
{
    float s = sinf(angle_rad);
    float c = cosf(angle_rad);
    float nc = 1.f - c;

    vec3 n_axis = v3_normalize(axis);

    mat4 rot = {0};

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

    return m4_mul(in, rot);
}

static inline mat4 m4_rotateX(const mat4 in, float angle_rad)
{
    float s = sinf(angle_rad);
    float c = cosf(angle_rad);

    mat4 rot = {0};

    rot.idx[0][0] = 1.0f;
    rot.idx[1][1] = c;
    rot.idx[1][2] = -s;
    rot.idx[2][1] = s;
    rot.idx[2][2] = c;
    rot.idx[3][3] = 1.0f;

    return m4_mul(in, rot);
}

static inline mat4 m4_rotateY(const mat4 in, float angle_rad)
{
    float s = sinf(angle_rad);
    float c = cosf(angle_rad);

    mat4 rot = {0};

    rot.idx[0][0] = c;
    rot.idx[0][2] = s;
    rot.idx[1][1] = 1.0f;
    rot.idx[2][0] = -s;
    rot.idx[2][2] = c;
    rot.idx[3][3] = 1.0f;

    return m4_mul(in, rot);
}

static inline mat4 m4_rotateZ(const mat4 in, float angle_rad)
{
    float s = sinf(angle_rad);
    float c = cosf(angle_rad);

    mat4 rot = {0};

    rot.idx[0][0] = c;
    rot.idx[0][1] = -s;
    rot.idx[1][0] = s;
    rot.idx[1][1] = c;
    rot.idx[2][2] = 1.0f;
    rot.idx[3][3] = 1.0f;

    return m4_mul(in, rot);
}

static inline mat4 m4_lookat(const vec3 eye, const vec3 target, const vec3 up)
{
    vec3 view_z = v3_normalize(v3_sub(target, eye));
    vec3 view_x = v3_normalize(v3_cross(view_z, up));
    vec3 view_y = v3_cross(view_x, view_z);
    view_z = v3_neg(view_z);

    return (mat4) {

        view_x.x, view_x.y, view_x.z, -v3_dot(view_x, eye),
        view_y.x, view_y.y, view_y.z, -v3_dot(view_y, eye),
        view_z.x, view_z.y, view_z.z, -v3_dot(view_z, eye),
        0.f, 0.f, 0.f, 1.f
    };
}

static inline mat4 m4_ortho1(float width, float height, float zNear, float zFar)
{
    mat4 out = {0};

    out.idx[0][0] = 2.0f / width;
    out.idx[1][1] = 2.0f / height;
    out.idx[2][2] = -2.0f / (zFar - zNear);
    out.idx[0][3] = -1.0f;
    out.idx[1][3] = -1.0f;
    out.idx[2][3] = -(zFar + zNear) / (zFar - zNear);
    out.idx[3][3] = 1.0f;

    return out;
}

static inline mat4 m4_ortho2(float left, float right, float bottom, float top, float zNear, float zFar)
{
    mat4 out = {0};

    out.idx[0][0] = 2.0f / (right - left);
    out.idx[1][1] = 2.0f / (top - bottom);
    out.idx[2][2] = -2.0f / (zFar - zNear);
    out.idx[0][3] = -(right + left) / (right - left);
    out.idx[1][3] = -(top + bottom) / (top - bottom);
    out.idx[2][3] = -(zFar + zNear) / (zFar - zNear);
    out.idx[3][3] = 1.0f;

    return out;
}

static inline mat4 m4_perspective(float fov, float aspect, float zNear, float zFar)
{
    float scale = 1.0f / tanf(fov * 0.5f);

    mat4 out = {0};

    out.idx[0][0] = scale;
    out.idx[1][1] = scale * aspect;
    out.idx[2][2] = (zFar + zNear) / (zNear - zFar);
    out.idx[2][3] = 2.0f * zFar * zNear / (zNear - zFar);
    out.idx[3][2] = -1.0f;

    return out;
}

static inline mat4 m4_inverse()
{
    mat4 inverse = {0};
    return inverse;
}

// QUATERNION TYPE ////////////////////////////////////////////////////////////////////////////////////////////////
