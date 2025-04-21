#pragma once

#include <cmath>
#include <cassert>

namespace Math {

// Constants
constexpr float PI = 3.1415926535897932384626433f;

// Utility functions
inline float ToRadians(float angle_deg) {
    return angle_deg * (PI / 180.0f);
}

// Vector2 class
class Vector2 {
public:
    float x, y;

    // Constructors
    Vector2() : x(0.0f), y(0.0f) {}
    Vector2(float x, float y) : x(x), y(y) {}
    
    // Unary operators
    Vector2 operator-() const { return Vector2(-x, -y); }
    
    // Binary operators
    Vector2 operator+(const Vector2& other) const { return Vector2(x + other.x, y + other.y); }
    Vector2 operator-(const Vector2& other) const { return Vector2(x - other.x, y - other.y); }
    Vector2 operator*(float scalar) const { return Vector2(x * scalar, y * scalar); }
    Vector2 operator*(const Vector2& other) const { return Vector2(x * other.x, y * other.y); }
    
    // Compound assignment operators
    Vector2& operator+=(const Vector2& other) {
        x += other.x;
        y += other.y;
        return *this;
    }
    
    Vector2& operator-=(const Vector2& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }
    
    Vector2& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }
    
    // Vector operations
    float Dot(const Vector2& other) const { return x * other.x + y * other.y; }
    float Length() const { return std::sqrt(x * x + y * y); }
    Vector2 Normalized() const {
        float len = Length();
        if (len < 1e-6f) return Vector2();
        float invLen = 1.0f / len;
        return Vector2(x * invLen, y * invLen);
    }
    void Normalize() {
        float len = Length();
        if (len < 1e-6f) return;
        float invLen = 1.0f / len;
        x *= invLen;
        y *= invLen;
    }
};

// Vector3 class
class Vector3 {
public:
    float x, y, z;

    // Constructors
    Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
    
    // Unary operators
    Vector3 operator-() const { return Vector3(-x, -y, -z); }
    
    // Binary operators