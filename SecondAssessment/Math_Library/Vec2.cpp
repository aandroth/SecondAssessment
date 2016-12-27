#include "Vec2.h"
#include <iostream>
using std::cout;

Vec2::Vec2()
{
	x = 0;
	y = 0;
}

Vec2::Vec2(float newX, float newY)
{
	x = newX;
	y = newY;
}

bool operator== (const Vec2 &lhs, const Vec2 &rhs)
{
	if (lhs.x == rhs.x && lhs.y == rhs.y)
	{
		return true;
	}

	return false;
}
bool operator!= (const Vec2 &lhs, const Vec2 &rhs)
{
	if (lhs.x != rhs.x || lhs.y != rhs.y)
	{
		return true;
	}

	return false;
}

Vec2 operator+ (const Vec2 &lhs, const Vec2 &rhs)
{
	return Vec2(lhs.x + rhs.x, lhs.y + rhs.y);
}
Vec2 operator+= (Vec2 &lhs, const Vec2 &rhs)
{
	return lhs = lhs + rhs;
}

Vec2 operator- (const Vec2 &lhs, const Vec2 &rhs)
{
	return Vec2(lhs.x - rhs.x, lhs.y - rhs.y);
}
Vec2 operator-= (Vec2 &lhs, const Vec2 &rhs)
{
	return lhs = lhs - rhs;
}

Vec2 operator* (const Vec2 &lhs, const float &rhs)
{
	return Vec2(lhs.x * rhs, lhs.y * rhs);
}
Vec2 operator* (const float &lhs, const Vec2 &rhs)
{
	return Vec2(lhs * rhs.x, lhs * rhs.y);
}
Vec2 operator*= (Vec2 &lhs, const float &rhs)
{
	return lhs = lhs * rhs;
}

Vec2 operator/ (const Vec2 &lhs, const float &rhs)
{
	float returnX, returnY;

	if (rhs == 0) { returnX = 0; returnY = 0; }
	else { returnX = lhs.x / rhs;  returnY = lhs.y / rhs; }

	return Vec2(returnX, returnY);
}
Vec2 operator/= (Vec2 &lhs, const float &rhs)
{
	return lhs = lhs / rhs;
}

float Vec2::operator[] (unsigned idx) const
{
	return v[idx];
}

float & Vec2::operator[] (unsigned idx)
{
	return v[idx];
}

float magnitude(const Vec2 &v)
{
	return sqrt(v.x*v.x + v.y*v.y);
}

// Reduce vector's length to 1
// v/magnitude(v)
Vec2 normal(const Vec2 &v)
{
	return Vec2((v.x / magnitude(v)), (v.y / magnitude(v)));
}

// Scalar Projection : width of the right angle between two vectors
// x1*x2 + y1*y2
float dot(const Vec2 &lhs, const Vec2 &rhs)
{
	return (lhs.x * rhs.x + lhs.y * rhs.y);
}

// Find the angle between two vectors
// acos(dot(normal(v1), normal(v2)))
float angleBetween(const Vec2 &lhs, const Vec2 &rhs)
{
	return radiansToDegrees(acos(dot(normal(lhs), normal(rhs))));
}

// Find a right angle perpendicular vector (2D only!)
Vec2 perp(const Vec2 &v)
{
	return Vec2(v.y, -v.x);
}

// Find the angle of the vector (2D only!)
float angle(const Vec2 &v)
{
	return radiansToDegrees(atan2f(v.y, v.x));
}

// vec2{cos(a), sin(a)};
Vec2 fromAngle(float a)
{
	Vec2 v = Vec2(cos(degreesToRadians(a)), sin(degreesToRadians(a)));

	if (abs(v.x) < 0.0000001) { v.x = 0; }
	if (abs(v.y) < 0.0000001) { v.y = 0; }

	return v;
}

Vec2 projection(const Vec2 &norm, const Vec2 &v)
{
	return dot(v, normal(norm)) * normal(norm);
}

Vec2 reflection(const Vec2 &norm, const Vec2 &v)
{
	return v - 2 * projection(norm, v);
}

Vec2 lerp(const Vec2 start, const Vec2 end, const float alpha)
{
	return Vec2((start.x + alpha *(end.x - start.x)),
		(start.y + alpha * (end.y - start.y)));
}

Vec2 quadBezier(const Vec2 start, const Vec2 mid, const Vec2 end, const float alpha)
{
	Vec2 mid1 = lerp(start, mid, alpha);

	Vec2 mid2 = lerp(mid, end, alpha);

	return lerp(mid1, mid2, alpha);
}

Vec2 cubicBezier(const Vec2 start, const Vec2 mid1, const Vec2 mid2, const Vec2 end, const float alpha)
{
	Vec2 cube_mid1 = lerp(start, mid1, alpha);

	Vec2 cube_mid2 = lerp(mid1, mid2, alpha);

	Vec2 cube_mid3 = lerp(mid2, end, alpha);

	return quadBezier(cube_mid1, cube_mid2, cube_mid3, alpha);
}

Vec2 hermiteSpline(const Vec2 start, const Vec2 end, const Vec2 tangent0, const Vec2 tangent1, float alpha)
{
	float a_squr = alpha*alpha;
	float a_cube = alpha*a_squr;

	float h00 = 2 * a_cube - 3 * a_squr + 1;
	float h01 = -2 * a_cube + 3 * a_squr;
	float h10 = a_cube - 2 * a_squr + alpha;
	float h11 = a_cube - a_squr;

	return start * h00 + tangent0 * h10 + end * h01 + tangent1 * h11;
}


Vec2 cardinalSpline(const Vec2 start, const Vec2 mid, const Vec2 end, float tightness, float alpha)
{
	Vec2 tangent0 = (mid - start) * tightness;
	Vec2 tangent1 = (end - mid) * tightness;

	float a_squr = alpha*alpha;
	float a_cube = alpha*a_squr;

	float h00 = 2 * a_cube - 3 * a_squr + 1;
	float h01 = -2 * a_cube + 3 * a_squr;
	float h10 = a_cube - 2 * a_squr + alpha;
	float h11 = a_cube - a_squr;

	return start * h00 + tangent0 * h10 + end * h01 + tangent1 * h11;
}
