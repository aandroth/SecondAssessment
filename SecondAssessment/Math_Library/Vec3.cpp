#include "Vec3.h"

Vec3::Vec3()
{
	x = 0;
	y = 0;
	z = 0;
}

Vec3::Vec3(float newX, float newY, float newZ)
{
	x = newX;
	y = newY;
	z = newZ;
}

Vec2 Vec3::xy()
{
	return Vec2(x, y);
}

bool operator== (const Vec3 &lhs, const Vec3 &rhs)
{
	if (lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z)
	{
		return true;
	}

	return false;
}
bool operator!= (const Vec3 &lhs, const Vec3 &rhs)
{
	if (!(lhs == rhs))
	{
		return true;
	}

	return false;
}

Vec3 operator+ (const Vec3 &lhs, const Vec3 &rhs)
{
	return Vec3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
}
Vec3 operator+= (Vec3 &lhs, const Vec3 &rhs)
{
	return lhs = lhs + rhs;
}

Vec3 operator- (const Vec3 &lhs, const Vec3 &rhs)
{
	return Vec3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
}
Vec3 operator-= (Vec3 &lhs, const Vec3 &rhs)
{
	return lhs = lhs - rhs;
}

Vec3 operator* (const Vec3 &lhs, const float &rhs)
{
	return Vec3(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs);
}
Vec3 operator*= (Vec3 &lhs, const float &rhs)
{
	return lhs = lhs * rhs;
}

Vec3 operator/ (const Vec3 &lhs, const float &rhs)
{
	float returnX, returnY, returnZ;

	if (rhs == 0)
	{ 
		return Vec3(0, 0, 0);
	}

	return Vec3(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs);
}
Vec3 operator/= (Vec3 &lhs, const float &rhs)
{
	return lhs = lhs / rhs;
}

float Vec3::operator[](unsigned idx) const
{
	return v[idx];
}

float &Vec3::operator[](unsigned idx)
{
	return v[idx];
}

float magnitude(const Vec3 &v)
{
	return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

// Scalar Projection : width of the right angle between two vectors
float dot(const Vec3 &lhs, const Vec3 &rhs)
{
	return (lhs.x*rhs.x + lhs.y*rhs.y + lhs.z*rhs.z);
}

// Reduce vector's length to 1
// v/magnitude(v)
Vec3 normal(const Vec3 &v)
{
	return v / magnitude(v);
}

// Find the angle between two vectors
float angleBetween(const Vec3 &lhs, const Vec3 &rhs)
{
	return radiansToDegrees(acos(dot(normal(lhs), normal(rhs))));
}

// Find a vector perpendicular to the plane created by two vectors
Vec3 cross(const Vec3 &lhs, const Vec3 &rhs)
{
	return Vec3((lhs.y*rhs.z - lhs.z*rhs.y),
		(lhs.z*rhs.x - lhs.x*rhs.z),
		(lhs.x*rhs.y - lhs.y*rhs.x));
}
