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
};

struct Algebra
{
    static Vec2 centroid(const std::vector<Vec2>& vertices)
    {
        if (vertices.size() == 1) return vertices[0];
        if (vertices.size() == 2) return Algebra::interpolate(vertices[0], vertices[1], 0.5);

        float signedArea = 0.0f;
        float centroidX = 0.0f;
        float centroidY = 0.0f;

        for (size_t i = 0; i < vertices.size(); ++i) {
            size_t next = (i + 1) % vertices.size();
            float x0 = vertices[i].x;
            float y0 = vertices[i].y;
            float x1 = vertices[next].x;
            float y1 = vertices[next].y;

            float crossProduct = x0 * y1 - x1 * y0;
            signedArea += crossProduct;
            centroidX += (x0 + x1) * crossProduct;
            centroidY += (y0 + y1) * crossProduct;
        }

        signedArea *= 0.5f;
        if (signedArea == 0) {
            throw std::runtime_error("The polygon area is zero; ensure the vertices form a valid polygon.");
        }

        centroidX /= (6.0f * signedArea);
        centroidY /= (6.0f * signedArea);
        
        return {centroidX, centroidY};
    }

    static std::vector<Vec2> relativeVertices(const std::vector<Vec2>& vertices, const Vec2& position)
    {
        std::vector<Vec2> relVerts;
        relVerts.reserve(vertices.size());
        for (auto& vertex : vertices)
            relVerts.push_back(vertex - position);

        return relVerts;
    }

    static Vec2 interpolate(const Vec2 a, const Vec2 b, const float weight)
    {
        return Vec2(a + ((b - a) * weight));
    } 

    static Vec2 interpolateSet(const std::vector<Vec2>& values, float weight)
    {
        if (weight < 0) return Algebra::interpolate(values[0], values[1], weight);
        if (weight > 1) return Algebra::interpolate(values[values.size() - 2], values[values.size() - 1], weight);

        float scaledPos = weight * (values.size() - 1);
        int i = static_cast<int>(std::floorf(scaledPos));
        float frac = scaledPos - i;

        return Algebra::interpolate(values[i], values[i + 1], frac);
    }

    static std::vector<Vec2> getInterpolatedSet(const std::vector<Vec2>& values, float weight)
    {
    	int i = (int) std::floor(weight * (values.size() - 1));
    	std::vector<Vec2> interpolatedSet(values.begin(), values.begin() + i + 1);
    	interpolatedSet.push_back(Algebra::interpolateSet(values, weight));

    	return interpolatedSet;
    }

    static std::vector<Vec2> close(std::vector<Vec2> shape)
    {
    	shape.push_back(shape[0]);
    	return shape;
    }
};
