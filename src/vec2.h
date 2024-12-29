#pragma once

#include <cmath>
#include <iostream>

struct Vec2
{
    float x, y;

    Vec2(float x = 0, float y = 0) : x(x), y(y) {}

    // Addition
    Vec2 operator+(const Vec2& other) const
    {
        return Vec2(x + other.x, y + other.y);
    }

    // Subtraction
    Vec2 operator-(const Vec2& other) const
    {
        return Vec2(x - other.x, y - other.y);
    }

    // Scalar Multiplication
    Vec2 operator*(float scalar) const
    {
        return Vec2(x * scalar, y * scalar);
    }

    // Scalar Division
    Vec2 operator/(float scalar) const
    {
        return Vec2(x / scalar, y / scalar);
    }

    // Hadamard Product
    Vec2 operator*(const Vec2& other) const
    {
        return Vec2(x * other.x, y * other.y);
    }

    // Negation
    Vec2 operator-() const
    {
        return Vec2(-x, -y);
    }

    // Magnitude
    float mag() const
    {
        return std::sqrt(x * x + y * y);
    }

    // Magnitude Squared
    float mag2() const
    {
        return x * x + y * y;
    }

    // Normalization
    Vec2 norm() const
    {
        float mag = std::sqrt(x * x + y * y);
        return mag == 0 ? Vec2(0, 0) : Vec2(x / mag, y / mag);
    }

    Vec2 rotate(float angle, Vec2 pivot)
    {
        Vec2 rotatedVec;

        const float dx = x - pivot.x;
        const float dy = y - pivot.y;

        float cosTheta = cos(angle);
        float sinTheta = sin(angle);

        rotatedVec.x = pivot.x + (dx * cosTheta - dy * sinTheta);
        rotatedVec.y = pivot.y + (dx * sinTheta + dy * cosTheta);

        return rotatedVec;
    }

    // Output Stream (for printing)
    friend std::ostream& operator<<(std::ostream& os, const Vec2& v) {
        os << "(" << v.x << ", " << v.y << ")";
        return os;
    }
};