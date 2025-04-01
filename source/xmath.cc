#pragma once

#include <math.h>
#include <assert.h>

#include "xmath.h"

namespace xm {

    // FUNCTIONS //////////////////////////////////////////////////////////////////////////////////////////////////////////

    float radians(float angle_deg)
    {
        return angle_deg * DEG_TO_RAD_FACTOR;
    }

    // VECTOR 3 FLOAT TYPE ////////////////////////////////////////////////////////////////////////////////////////////////

    vector3::vector3(float f) : x(f), y(f), z(f) {}
    vector3::vector3(float x, float y, float z) : x(x), y(y), z(z) {}
    vector3::vector3(const vector3& vec) : x(vec.x), y(vec.y), z(vec.z) {}

    vector3& vector3::operator= (const vector3& vec)
    {   
        x = vec.x;
        y = vec.y;
        z = vec.z;

        return *this;
    }

    vector3 operator- (vector3 vec)
    {
        return vector3(-vec.x, -vec.y, -vec.z);
    }

    vector3 operator+ (vector3 a, vector3 b)
    {
        return vector3(a.x + b.x, a.y + b.y, a.z + b.z);
    }

    vector3 operator- (vector3 a, vector3 b)
    {
        return vector3(a.x - b.x, a.y - b.y, a.z - b.z);
    }

    vector3 operator* (vector3 a, vector3 b)
    {
        return vector3(a.x * b.x, a.y * b.y, a.z * b.z);
    }

    vector3 operator/ (vector3 a, vector3 b)
    {
        assert(b.x != 0.f && b.y != 0.f && b.z != 0.f);
        return vector3(a.x / b.x, a.y / b.y, a.z / b.z);
    }

    void operator+= (vector3& a, vector3 b)
    {
        a.x += b.x;
        a.y += b.y;
        a.z += b.z;
    }

    void operator-= (vector3& a, vector3 b)
    {
        a.x -= b.x;
        a.y -= b.y;
        a.z -= b.z;
    }

    void operator*= (vector3& a, vector3 b)
    {
        a.x *= b.x;
        a.y *= b.y;
        a.z *= b.z;
    }

    void operator/= (vector3& a, vector3 b)
    {
        assert(b.x != 0.f && b.y != 0.f && b.z != 0.f);
        a.x /= b.x;
        a.y /= b.y;
        a.z /= b.z;
    }

    vector3 operator* (vector3 a, float f)
    {
        return vector3(a.x * f, a.y * f, a.z * f);
    }

    vector3 operator/ (vector3 a, float f)
    {
        assert(f != 0.0f);
        return vector3(a.x / f, a.y / f, a.z / f);
    }

    vector3 operator* (float f, vector3 vector)
    {
        return vector3(f * vector.x, f * vector.y, f * vector.z);
    }

    void operator*= (vector3& a, float f)
    {
        a.x *= f;
        a.y *= f;
        a.z *= f;
    }

    void operator/= (vector3& a, float f)
    {
        assert(f != 0.0f);
        a.x /= f;
        a.y /= f;
        a.z /= f;
    }

    float dot(vector3 a, vector3 b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    vector3 cross(vector3 a, vector3 b)
    {
        return vector3 {
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x
        };
    }

    vector3 normalize(vector3 vec)
    {
        float inverse_len = 1.0f / sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
        return vector3(vec.x * inverse_len, vec.y * inverse_len, vec.z * inverse_len);
    }

    // MATRIX 4 FLOAT TYPE ////////////////////////////////////////////////////////////////////////////////////////////////

    matrix4::matrix4(float f)
    {
        idx[0][0] = f;
        idx[1][1] = f;
        idx[2][2] = f;
        idx[3][3] = f;
    }

    matrix4::matrix4(const matrix4& M)
    {
        idx[0][0] = M.idx[0][0]; idx[0][1] = M.idx[0][1]; idx[0][2] = M.idx[0][2]; idx[0][3] = M.idx[0][3];
        idx[1][0] = M.idx[1][0]; idx[1][1] = M.idx[1][1]; idx[1][2] = M.idx[1][2]; idx[1][3] = M.idx[1][3];
        idx[2][0] = M.idx[2][0]; idx[2][1] = M.idx[2][1]; idx[2][2] = M.idx[2][2]; idx[2][3] = M.idx[2][3];
        idx[3][0] = M.idx[3][0]; idx[3][1] = M.idx[3][1]; idx[3][2] = M.idx[3][2]; idx[3][3] = M.idx[3][3];
    }

    matrix4& matrix4::operator= (const matrix4& M)
    {
        idx[0][0] = M.idx[0][0]; idx[0][1] = M.idx[0][1]; idx[0][2] = M.idx[0][2]; idx[0][3] = M.idx[0][3];
        idx[1][0] = M.idx[1][0]; idx[1][1] = M.idx[1][1]; idx[1][2] = M.idx[1][2]; idx[1][3] = M.idx[1][3];
        idx[2][0] = M.idx[2][0]; idx[2][1] = M.idx[2][1]; idx[2][2] = M.idx[2][2]; idx[2][3] = M.idx[2][3];
        idx[3][0] = M.idx[3][0]; idx[3][1] = M.idx[3][1]; idx[3][2] = M.idx[3][2]; idx[3][3] = M.idx[3][3];

        return *this;
    }

    matrix4 operator* (matrix4 A, matrix4 B)
    {
        matrix4 out(0);

        out.idx[0][0] = A.idx[0][0] * B.idx[0][0] + A.idx[0][1] * B.idx[1][0] + A.idx[0][2] * B.idx[2][0] + A.idx[0][3] * B.idx[3][0];
        out.idx[0][1] = A.idx[0][0] * B.idx[0][1] + A.idx[0][1] * B.idx[1][1] + A.idx[0][2] * B.idx[2][1] + A.idx[0][3] * B.idx[3][1];
        out.idx[0][2] = A.idx[0][0] * B.idx[0][2] + A.idx[0][1] * B.idx[1][2] + A.idx[0][2] * B.idx[2][2] + A.idx[0][3] * B.idx[3][2];
        out.idx[0][3] = A.idx[0][0] * B.idx[0][3] + A.idx[0][1] * B.idx[1][3] + A.idx[0][2] * B.idx[2][3] + A.idx[0][3] * B.idx[3][3];

        out.idx[1][0] = A.idx[1][0] * B.idx[0][0] + A.idx[1][1] * B.idx[1][0] + A.idx[1][2] * B.idx[2][0] + A.idx[1][3] * B.idx[3][0];
        out.idx[1][1] = A.idx[1][0] * B.idx[0][1] + A.idx[1][1] * B.idx[1][1] + A.idx[1][2] * B.idx[2][1] + A.idx[1][3] * B.idx[3][1];
        out.idx[1][2] = A.idx[1][0] * B.idx[0][2] + A.idx[1][1] * B.idx[1][2] + A.idx[1][2] * B.idx[2][2] + A.idx[1][3] * B.idx[3][2];
        out.idx[1][3] = A.idx[1][0] * B.idx[0][3] + A.idx[1][1] * B.idx[1][3] + A.idx[1][2] * B.idx[2][3] + A.idx[1][3] * B.idx[3][3];

        out.idx[2][0] = A.idx[2][0] * B.idx[0][0] + A.idx[2][1] * B.idx[1][0] + A.idx[2][2] * B.idx[2][0] + A.idx[2][3] * B.idx[3][0];
        out.idx[2][1] = A.idx[2][0] * B.idx[0][1] + A.idx[2][1] * B.idx[1][1] + A.idx[2][2] * B.idx[2][1] + A.idx[2][3] * B.idx[3][1];
        out.idx[2][2] = A.idx[2][0] * B.idx[0][2] + A.idx[2][1] * B.idx[1][2] + A.idx[2][2] * B.idx[2][2] + A.idx[2][3] * B.idx[3][2];
        out.idx[2][3] = A.idx[2][0] * B.idx[0][3] + A.idx[2][1] * B.idx[1][3] + A.idx[2][2] * B.idx[2][3] + A.idx[2][3] * B.idx[3][3];

        out.idx[3][0] = A.idx[3][0] * B.idx[0][0] + A.idx[3][1] * B.idx[1][0] + A.idx[3][2] * B.idx[2][0] + A.idx[3][3] * B.idx[3][0];
        out.idx[3][1] = A.idx[3][0] * B.idx[0][1] + A.idx[3][1] * B.idx[1][1] + A.idx[3][2] * B.idx[2][1] + A.idx[3][3] * B.idx[3][1];
        out.idx[3][2] = A.idx[3][0] * B.idx[0][2] + A.idx[3][1] * B.idx[1][2] + A.idx[3][2] * B.idx[2][2] + A.idx[3][3] * B.idx[3][2];
        out.idx[3][3] = A.idx[3][0] * B.idx[0][3] + A.idx[3][1] * B.idx[1][3] + A.idx[3][2] * B.idx[2][3] + A.idx[3][3] * B.idx[3][3];

        return out;
    }

    void operator*= (matrix4& out, matrix4 in)
    {
        matrix4 t = out;

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

    matrix4 operator* (matrix4 M, float f)
    {
        matrix4 out(0);

        out.idx[0][0] = M.idx[0][0] * f; out.idx[0][1] = M.idx[0][1] * f; out.idx[0][2] = M.idx[0][2] * f; out.idx[0][3] = M.idx[0][3] * f;
        out.idx[1][0] = M.idx[1][0] * f; out.idx[1][1] = M.idx[1][1] * f; out.idx[1][2] = M.idx[1][2] * f; out.idx[1][3] = M.idx[1][3] * f;
        out.idx[2][0] = M.idx[2][0] * f; out.idx[2][1] = M.idx[2][1] * f; out.idx[2][2] = M.idx[2][2] * f; out.idx[2][3] = M.idx[2][3] * f;
        out.idx[3][0] = M.idx[3][0] * f; out.idx[3][1] = M.idx[3][1] * f; out.idx[3][2] = M.idx[3][2] * f; out.idx[3][3] = M.idx[3][3] * f;

        return out;
    }

    matrix4 operator* (float f, matrix4 M)
    {
        matrix4 out(0);

        out.idx[0][0] = M.idx[0][0] * f; out.idx[0][1] = M.idx[0][1] * f; out.idx[0][2] = M.idx[0][2] * f; out.idx[0][3] = M.idx[0][3] * f;
        out.idx[1][0] = M.idx[1][0] * f; out.idx[1][1] = M.idx[1][1] * f; out.idx[1][2] = M.idx[1][2] * f; out.idx[1][3] = M.idx[1][3] * f;
        out.idx[2][0] = M.idx[2][0] * f; out.idx[2][1] = M.idx[2][1] * f; out.idx[2][2] = M.idx[2][2] * f; out.idx[2][3] = M.idx[2][3] * f;
        out.idx[3][0] = M.idx[3][0] * f; out.idx[3][1] = M.idx[3][1] * f; out.idx[3][2] = M.idx[3][2] * f; out.idx[3][3] = M.idx[3][3] * f;

        return out;
    }

    void operator*= (matrix4& M, float f)
    {
        M.idx[0][0] = M.idx[0][0] * f; M.idx[0][1] = M.idx[0][1] * f; M.idx[0][2] = M.idx[0][2] * f; M.idx[0][3] = M.idx[0][3] * f;
        M.idx[1][0] = M.idx[1][0] * f; M.idx[1][1] = M.idx[1][1] * f; M.idx[1][2] = M.idx[1][2] * f; M.idx[1][3] = M.idx[1][3] * f;
        M.idx[2][0] = M.idx[2][0] * f; M.idx[2][1] = M.idx[2][1] * f; M.idx[2][2] = M.idx[2][2] * f; M.idx[2][3] = M.idx[2][3] * f;
        M.idx[3][0] = M.idx[3][0] * f; M.idx[3][1] = M.idx[3][1] * f; M.idx[3][2] = M.idx[3][2] * f; M.idx[3][3] = M.idx[3][3] * f;
    }

    matrix4 transpose(matrix4 M)
    {
        matrix4 out(0);

        M.idx[0][0] = M.idx[0][0]; M.idx[0][1] = M.idx[1][0]; M.idx[0][2] = M.idx[2][0]; M.idx[0][3] = M.idx[3][0];
        M.idx[1][0] = M.idx[0][1]; M.idx[1][1] = M.idx[1][1]; M.idx[1][2] = M.idx[2][1]; M.idx[1][3] = M.idx[3][1];
        M.idx[2][0] = M.idx[0][2]; M.idx[2][1] = M.idx[1][2]; M.idx[2][2] = M.idx[2][2]; M.idx[2][3] = M.idx[3][2];
        M.idx[3][0] = M.idx[0][3]; M.idx[3][1] = M.idx[1][3]; M.idx[3][2] = M.idx[2][3]; M.idx[3][3] = M.idx[3][3];

        return out;
    }

    matrix4 scale(matrix4 M, float f)
    {
        matrix4 scaling(0);

        scaling.idx[0][0] = f;
        scaling.idx[1][1] = f;
        scaling.idx[2][2] = f;
        scaling.idx[3][3] = 1.0f;

        return M * scaling;
    }

    matrix4 scale(matrix4 M, float x, float y, float z)
    {
        matrix4 scaling(0);

        scaling.idx[0][0] = x;
        scaling.idx[1][1] = y;
        scaling.idx[2][2] = z;
        scaling.idx[3][3] = 1.f;

        return M * scaling;
    }

    matrix4 scale(matrix4 M, vector3 vec)
    {
        matrix4 scaling(0);

        scaling.idx[0][0] = vec.x;
        scaling.idx[1][1] = vec.y;
        scaling.idx[2][2] = vec.z;
        scaling.idx[3][3] = 1.f;

        return M * scaling;
    }

    matrix4 translate(matrix4 M, float x, float y, float z)
    {
        matrix4 trans(1);

        trans.idx[0][3] = x;
        trans.idx[1][3] = y;
        trans.idx[2][3] = z;

        return M * trans;
    }

    matrix4 translate(matrix4 A, vector3 B)
    {
        matrix4 trans(1);

        trans.idx[0][3] = B.x;
        trans.idx[1][3] = B.y;
        trans.idx[2][3] = B.z;

        return A * trans;
    }

    matrix4 rotate(matrix4 M, vector3 axis, float angle_rad)
    {
        float s = sinf(angle_rad);
        float c = cosf(angle_rad);
        float nc = 1.f - c;

        vector3 n_axis = normalize(axis);

        matrix4 rot(0);

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

        return M * rot;
    }

    matrix4 rotateZ(matrix4 M, float angle_rad)
    {
        float s = sinf(angle_rad);
        float c = cosf(angle_rad);

        matrix4 rot(0);

        rot.idx[0][0] = c;
        rot.idx[0][1] = -s;
        rot.idx[1][0] = s;
        rot.idx[1][1] = c;
        rot.idx[2][2] = 1.0f;
        rot.idx[3][3] = 1.0f;

        return M * rot;
    }

    matrix4 rotateY(matrix4 M, float angle_rad)
    {
        float s = sinf(angle_rad);
        float c = cosf(angle_rad);

        matrix4 rot(0);

        rot.idx[0][0] = c;
        rot.idx[0][2] = s;
        rot.idx[1][1] = 1.0f;
        rot.idx[2][0] = -s;
        rot.idx[2][2] = c;
        rot.idx[3][3] = 1.0f;

        return M * rot;
    }

    matrix4 rotateX(matrix4 M, float angle_rad)
    {
        float s = sinf(angle_rad);
        float c = cosf(angle_rad);

        matrix4 rot(0);

        rot.idx[0][0] = 1.0f;
        rot.idx[1][1] = c;
        rot.idx[1][2] = -s;
        rot.idx[2][1] = s;
        rot.idx[2][2] = c;
        rot.idx[3][3] = 1.0f;

        return M * rot;
    }

    matrix4 lookAt(vector3 eye, vector3 target, vector3 up)
    {
        vector3 view_z = normalize(target - eye);
        vector3 view_x = normalize(cross(view_z, up));
        vector3 view_y = cross(view_x, view_z);
        view_z = -view_z;

        matrix4 out(0);

        out.idx[0][0] = view_x.x; out.idx[0][1] = view_x.y; out.idx[0][2] = view_x.z; out.idx[0][3] = -dot(view_x, eye);
        out.idx[1][0] = view_y.x; out.idx[1][1] = view_y.y; out.idx[1][2] = view_y.z; out.idx[1][3] = -dot(view_y, eye);
        out.idx[2][0] = view_z.x; out.idx[2][1] = view_z.y; out.idx[2][2] = view_z.z; out.idx[2][3] = -dot(view_z, eye);
        out.idx[3][0] = 0.f;      out.idx[3][1] = 0.f;      out.idx[3][2] = 0.f;      out.idx[3][3] = 1.f;

        return out;
    }

    matrix4 ortho(float width, float height, float zNear, float zFar)
    {
        matrix4 out(0);

        out.idx[0][0] = 2.0f / width;
        out.idx[1][1] = 2.0f / height;
        out.idx[2][2] = -2.0f / (zFar - zNear);
        out.idx[0][3] = -1.0f;
        out.idx[1][3] = -1.0f;
        out.idx[2][3] = -(zFar + zNear) / (zFar - zNear);
        out.idx[3][3] = 1.0f;

        return out;
    }

    matrix4 ortho(float left, float right, float bottom, float top, float zNear, float zFar)
    {
        matrix4 out(0);

        out.idx[0][0] = 2.0f / (right - left);
        out.idx[1][1] = 2.0f / (top - bottom);
        out.idx[2][2] = -2.0f / (zFar - zNear);
        out.idx[0][3] = -(right + left) / (right - left);
        out.idx[1][3] = -(top + bottom) / (top - bottom);
        out.idx[2][3] = -(zFar + zNear) / (zFar - zNear);
        out.idx[3][3] = 1.0f;

        return out;
    }

    matrix4 perspective(float fov, float aspect, float zNear, float zFar)
    {
        float scale = 1.0f / tanf(fov * 0.5f);

        matrix4 out(0);

        out.idx[0][0] = scale;
        out.idx[1][1] = scale * aspect;
        out.idx[2][2] = (zFar + zNear) / (zNear - zFar);
        out.idx[2][3] = 2.0f * zFar * zNear / (zNear - zFar);
        out.idx[3][2] = -1.0f;

        return out;
    }

} // namespace xm
