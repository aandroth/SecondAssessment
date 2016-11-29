#pragma once
#include "Mat2.h"
#include "Vec2.h"

#include "Flops.h"
#include <cmath>

#ifndef MAT2_H
#define MAT2_H

union Mat2
{
	struct
	{
		float x1, y1, x2, y2;
	};
	float v[4];
	float mm[2][2];
	Vec2 c[2];

	Mat2();
	Mat2(float, float, float, float);
	Mat2(Vec2, Vec2);

	float operator[](unsigned idx) const;
	float & operator[](unsigned idx);
};

bool operator== (const Mat2 &lhs, const Mat2 &rhs);
bool operator!= (const Mat2 &lhs, const Mat2 &rhs);

Mat2 operator+ (const Mat2 &lhs, const Mat2 &rhs);
Mat2 operator+= (Mat2 &lhs, const Mat2 &rhs);

Mat2 operator- (const Mat2 &lhs, const Mat2 &rhs);
Mat2 operator-= (Mat2 &lhs, const Mat2 &rhs);

// mat2  * mat2	  : Matrix multiplied by matrix
Mat2 operator* (const Mat2 &lhs, const Mat2 &rhs);
Mat2 operator*= (Mat2 &lhs, const Mat2 &rhs);

// mat2  * float  : matrix by scalar
Mat2 operator* (const Mat2 &lhs, const float &rhs);

// float * mat2   : scalr by matrix
Mat2 operator* (const float &lhs, const Mat2 &rhs);

// mat2  * vec2   : Matrix by vector
Vec2 operator* (const Mat2 &lhs, const Vec2 &rhs);

// Determinant: width of the right angle between two vectors
float determinant(const Mat2 &lhs);

// Tranverse: width of the right angle between two vectors
Mat2 transverse(const Mat2 &lhs);

// Inverse: width of the right angle between two vectors
Mat2 inverse(const Mat2 &lhs);

#endif