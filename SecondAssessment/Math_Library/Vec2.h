#pragma once

#include "Flops.h"
#include <cmath>

#ifndef VEC2_H
#define VEC2_H

union Vec2
{
	float v[2];
	struct
	{
		float x, y;
	};
	Vec2();
	Vec2(float, float);

	float operator[](unsigned idx) const;
	float & operator[](unsigned idx);
};

bool operator== (const Vec2 &lhs, const Vec2 &rhs);
bool operator!= (const Vec2 &lhs, const Vec2 &rhs);

Vec2 operator+ (const Vec2 &lhs, const Vec2 &rhs);
Vec2 operator+= (Vec2 &lhs, const Vec2 &rhs);

Vec2 operator- (const Vec2 &lhs, const Vec2 &rhs);
Vec2 operator-= (Vec2 &lhs, const Vec2 &rhs);

Vec2 operator* (const Vec2 &lhs, const float &rhs);
Vec2 operator*= (Vec2 &lhs, const float &rhs);

Vec2 operator/ (const Vec2 &lhs, const float &rhs);
Vec2 operator/= (Vec2 &lhs, const float &rhs);

float magnitude(const Vec2 &v);

// Reduce vector's length to 1
// v/magnitude(v)
Vec2 normal(const Vec2 &v);

// Scalar Projection : width of the right angle between two vectors
// x1*x2 + y1*y2
float dot(const Vec2 &lhs, const Vec2 &rhs);

// Find the angle between two vectors
// acos(dot(normal(v1), normal(v2)))
float angleBetween(const Vec2 &lhs, const Vec2 &rhs);

// Find a right angle perpendicular vector (2D only!)
// (y,-x)
Vec2 perp(const Vec2 &v);

// Find the angle of the vector (2D only!)
// atan2f(y,x)
float angle(const Vec2 &v);

// vec2{cos(a), sin(a)};
Vec2 fromAngle(float a);

Vec2 projection(const Vec2 &normal, const Vec2 &v);

Vec2 reflection(const Vec2 &normal, const Vec2 &v);

Vec2 lerp(const Vec2 start, const Vec2 end, const float alpha);

Vec2 quadBezier(const Vec2 start, const Vec2 mid, const Vec2 end, const float alpha);

Vec2 cubicBezier(const Vec2 start, const Vec2 mid1, const Vec2 mid2, const Vec2 end, const float alpha);

Vec2 hermiteSpline(const Vec2 start, const Vec2 end, const Vec2 tangent0, const Vec2 tangent1, float alpha);

Vec2 cardinalSpline(const Vec2 start, const Vec2 mid, const Vec2 end, float tightness, float alpha);

#endif