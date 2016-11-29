#include "Mat2.h"

Mat2::Mat2()
{
	x1 = 0;
	y1 = 0;
	x2 = 0;
	y2 = 0;
}

Mat2::Mat2(float newX1, float newY1, float newX2, float newY2)
{
	x1 = newX1;
	y1 = newY1;
	x2 = newX2;
	y2 = newY2;
}

Mat2::Mat2(Vec2 newV0, Vec2 newV1)
{
	c[0] = newV0;
	c[1] = newV1;
}

bool operator== (const Mat2 &lhs, const Mat2 &rhs)
{
	if (lhs.x1 == rhs.x1 && lhs.y1 == rhs.y1 &&
		lhs.x2 == rhs.x2 && lhs.y2 == rhs.y2)
	{
		return true;
	}

	return false;
}
bool operator!= (const Mat2 &lhs, const Mat2 &rhs)
{
	if (lhs.x1 != rhs.x1 || lhs.y1 != rhs.y1 ||
		lhs.x2 != rhs.x2 || lhs.y2 != rhs.y2)
	{
		return true;
	}

	return false;
}

Mat2 operator+ (const Mat2 &lhs, const Mat2 &rhs)
{
	return Mat2(lhs.x1 + rhs.x1, lhs.y1 + rhs.y1,
		lhs.x2 + rhs.x2, lhs.y2 + rhs.y2);
}
Mat2 operator+= (Mat2 &lhs, const Mat2 &rhs)
{
	return lhs = lhs + rhs;
}

Mat2 operator- (const Mat2 &lhs, const Mat2 &rhs)
{
	return Mat2(lhs.x1 - rhs.x1, lhs.y1 - rhs.y1,
		lhs.x2 - rhs.x2, lhs.y2 - rhs.y2);
}
Mat2 operator-= (Mat2 &lhs, const Mat2 &rhs)
{
	return lhs = lhs - rhs;
}

// mat2  * mat2	  : Matrix multiplied by matrix
Mat2 operator* (const Mat2 &lhs, const Mat2 &rhs)
{
	return Mat2(lhs.x1 * rhs.x1 + lhs.y1 * rhs.x2, lhs.x1 * rhs.y1 + lhs.y1 * rhs.y2, 
				lhs.x2 * rhs.x1 + lhs.y2 * rhs.x2, lhs.x2 * rhs.y1 + lhs.y2 * rhs.y2);
}
Mat2 operator*= (Mat2 &lhs, const Mat2 &rhs)
{
	return lhs = lhs * rhs;
}

// mat2  * float  : matrix by scalar
Mat2 operator* (const Mat2 &lhs, const float &rhs)
{
	return Mat2(lhs.x1 * rhs, lhs.y1 * rhs, lhs.x2 * rhs, lhs.y2 * rhs);
}
Mat2 operator*= (Mat2 &lhs, const float &rhs)
{
	return lhs = lhs * rhs;
}

// float * mat2   : scalr by matrix
Mat2 operator* (const float &lhs, const Mat2 &rhs)
{
	return rhs * lhs;
}

// mat2  * vec2   : Matrix by vector
Vec2 operator* (const Mat2 &lhs, const Vec2 &rhs)
{
	return Vec2(lhs.x1 * rhs.x + lhs.y1 * rhs.y, 
				lhs.x2 * rhs.x + lhs.y2 * rhs.y);
}

float Mat2::operator[](unsigned idx) const
{
	return v[idx];
}

float & Mat2::operator[](unsigned idx)
{
	return v[idx];
}

float determinant(const Mat2 &lhs)
{
	return lhs.x1*lhs.y2 - lhs.x2*lhs.y1;
}

Mat2 transverse(const Mat2 &lhs)
{
	return Mat2(lhs.x1, lhs.x2, lhs.y1, lhs.y2);
}

Mat2 inverse(const Mat2 &lhs)
{
	return Mat2(lhs.y2, -lhs.y1, -lhs.x2, lhs.x1) * (1/determinant(lhs));
}