#pragma once

namespace xm
{
    // DEFINES ////////////////////////////////////////////////////////////////////////////////////////////////////////////

    #define PI      3.1415926535897932384626433
    #define PI_F    3.1415926535897932384626433f

    constexpr float DEG_TO_RAD_FACTOR = PI_F / 180.f;

    // FUNCTIONS //////////////////////////////////////////////////////////////////////////////////////////////////////////

    float radians(float);

    // VECTOR 3 FLOAT TYPE ////////////////////////////////////////////////////////////////////////////////////////////////

    struct vector3 { 

        float x, y, z;

        vector3(float);
        vector3(float, float, float);
        vector3(const vector3&);

        void operator= (vector3);
    };

    vector3 operator- (vector3);

    vector3 operator+ (vector3, vector3);
    vector3 operator- (vector3, vector3);
    vector3 operator* (vector3, vector3);
    vector3 operator/ (vector3, vector3);

    void operator+= (vector3&, vector3);
    void operator-= (vector3&, vector3);
    void operator*= (vector3&, vector3);
    void operator/= (vector3&, vector3);

    vector3 operator* (vector3, float);
    vector3 operator* (float, vector3);
    vector3 operator/ (vector3, float);

    void operator*= (vector3&, float);
    void operator/= (vector3&, float);

    float dot(vector3, vector3);
    vector3 cross(vector3, vector3);
    vector3 normalize(vector3);

    // MATRIX 4 FLOAT TYPE ////////////////////////////////////////////////////////////////////////////////////////////////

    struct matrix4 {
        float idx[4][4];
        
        matrix4(float);
        matrix4(const matrix4&);
        void operator= (matrix4);
    };

    matrix4 operator* (matrix4, matrix4);
    void operator*= (matrix4&, matrix4);
    matrix4 operator* (matrix4, float);
    matrix4 operator* (float, matrix4);
    void operator*= (matrix4&, float);

    matrix4 transpose(matrix4);
    matrix4 scale(matrix4, float);
    matrix4 scale(matrix4, float, float, float);
    matrix4 scale(matrix4, vector3);
    matrix4 translate(matrix4, float, float, float);
    matrix4 translate(matrix4, vector3);
    matrix4 rotate(matrix4, vector3, float);
    matrix4 rotateZ(matrix4, float);
    matrix4 rotateY(matrix4, float);
    matrix4 rotateX(matrix4, float);
    matrix4 lookAt(vector3, vector3, vector3);
    matrix4 ortho(float, float, float, float);
    matrix4 ortho(float, float, float, float, float, float);
    matrix4 perspective(float, float, float, float);

} // namespace xm


