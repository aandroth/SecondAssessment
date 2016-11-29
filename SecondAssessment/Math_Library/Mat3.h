#pragma once
#pragma once
#include "Mat3.h"
#include "Vec3.h"
#include "Mat2.h"
#include "Flops.h"
#include <cmath>
#include <vector>
#include <iostream>

#ifndef MAT3_H
#define MAT3_H

union Mat3
{
	struct
	{
		float x1, y1, z1,
				x2, y2, z2,
				x3, y3, z3;
	};
	float v[9];
	float mm[3][3];
	Vec3 c[3];

	Mat3();
	Mat3(float v[]);
	Mat3(Vec3, Vec3, Vec3);
	Mat3(float x1, float y1, float z1,
		float x2, float y2, float z2, 
		float x3, float y3, float z3);

	float operator[](unsigned idx) const;
	float & operator[](unsigned idx);
};

Mat3 mat3Identity();

bool operator== (const Mat3 &lhs, const Mat3 &rhs);
bool operator!= (const Mat3 &lhs, const Mat3 &rhs);

Mat3 operator+ (const Mat3 &lhs, const Mat3 &rhs);
Mat3 operator+= (Mat3 &lhs, const Mat3 &rhs);

Mat3 operator- (const Mat3 &lhs, const Mat3 &rhs);
Mat3 operator-= (Mat3 &lhs, const Mat3 &rhs);

// Mat3  * Mat3	  : Matrix multiplied by matrix
Mat3 operator* (const Mat3 &lhs, const Mat3 &rhs);
Mat3 operator*= (Mat3 &lhs, const Mat3 &rhs);

// Mat3  * float  : matrix by scalar
Mat3 operator* (const Mat3 &lhs, const float &rhs);

// float * Mat3   : scaler by matrix
Mat3 operator* (const float &lhs, const Mat3 &rhs);

// Mat3  * vec3   : Matrix by vector
Vec3 operator* (const Mat3 &lhs, const Vec3 &rhs);

// Determinant: width of the right angle between two vectors
float determinant(const Mat3 &lhs);

// Tranverse: width of the right angle between two vectors
Mat3 transverse(const Mat3 &lhs);

// Inverse: width of the right angle between two vectors
Mat3 inverse(const Mat3 &lhs);

Mat3 scale(float w, float h);
Mat3 translate(float x, float y);
Mat3 rotateByDegrees(float a);
Mat3 rotateByRadians(float a);

#endif