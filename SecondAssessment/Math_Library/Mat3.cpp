#include "Mat3.h"

Mat3::Mat3()
{
	x1 = 0;
	y1 = 0;
	z1 = 0;
	x2 = 0;
	y2 = 0;
	z2 = 0;
	x3 = 0;
	y3 = 0;
	z3 = 0;
}

Mat3::Mat3(float new_v[])
{
	for (int ii = 0; ii < 9; ++ii)
	{
		v[ii] = new_v[ii];
	}
}
Mat3::Mat3(Vec3 v1, Vec3 v2, Vec3 v3)
{
	c[0] = v1;
	c[1] = v2;
	c[2] = v3;
}

Mat3::Mat3(float newX1, float newY1, float newZ1,
	float newX2, float newY2, float newZ2,
	float newX3, float newY3, float newZ3)
{
	x1 = newX1; y1 = newY1; z1 = newZ1;
	x2 = newX2; y2 = newY2; z2 = newZ2;
	x3 = newX3; y3 = newY3; z3 = newZ3;
}

Mat3 mat3Identity()
{
	return Mat3(1, 0, 0,
				0, 1, 0,
				0, 0, 1);
}

bool operator== (const Mat3 &lhs, const Mat3 &rhs)
{
	if (lhs.x1 == rhs.x1 && lhs.y1 == rhs.y1 && lhs.z1 == rhs.z1 &&
		lhs.x2 == rhs.x2 && lhs.y2 == rhs.y2 && lhs.z2 == rhs.z2 &&
		lhs.x3 == rhs.x3 && lhs.y3 == rhs.y3 && lhs.z3 == rhs.z3)
	{
		return true;
	}

	return false;
}
bool operator!= (const Mat3 &lhs, const Mat3 &rhs)
{
	return !(lhs == rhs);
}

Mat3 operator+ (const Mat3 &lhs, const Mat3 &rhs)
{
	return Mat3(lhs.c[0] + rhs.c[0], 
				lhs.c[1] + rhs.c[1], 
				lhs.c[2] + rhs.c[2]);
}
Mat3 operator+= (Mat3 &lhs, const Mat3 &rhs)
{
	return lhs = lhs + rhs;
}

Mat3 operator- (const Mat3 &lhs, const Mat3 &rhs)
{
	return Mat3(lhs.c[0] - rhs.c[0],
				lhs.c[1] - rhs.c[1],
				lhs.c[2] - rhs.c[2]);
}
Mat3 operator-= (Mat3 &lhs, const Mat3 &rhs)
{
	return lhs = lhs - rhs;
}

// Mat3  * Mat3	  : Matrix multiplied by matrix
Mat3 operator* (const Mat3 &lhs, const Mat3 &rhs)
{
	std::vector<float> matrixVector = matrixMultiplier(lhs.v, 3, 3,
													   rhs.v, 3, 9);

	float matrixArray[9];

	for (int ii = 0; ii < 9; ++ii)
	{
		if (abs(matrixVector[ii]) < 0.0000007)
		{
			matrixArray[ii] = 0;
		}
		else
		{
			matrixArray[ii] = matrixVector[ii];
		}
	}	

	return Mat3(matrixArray);
}
Mat3 operator*= (Mat3 &lhs, const Mat3 &rhs)
{
	return lhs = lhs * rhs;
}

// Mat3  * float  : matrix by scalar
Mat3 operator* (const Mat3 &lhs, const float &rhs)
{
	float v[9];

	for (int ii = 0; ii < 9; ++ii)
	{
		v[ii] = lhs.v[ii] * rhs;
	}

	return Mat3(v);
}
Mat3 operator*= (Mat3 &lhs, const float &rhs)
{
	return lhs = lhs * rhs;
}

// float * Mat3   : scalr by matrix
Mat3 operator* (const float &lhs, const Mat3 &rhs)
{
	return rhs * lhs;
}

// Mat3  * vec2   : Matrix by vector
Vec3 operator* (const Mat3 &lhs, const Vec3 &rhs)
{
	return Vec3(lhs.x1 * rhs.x + lhs.y1 * rhs.y + lhs.z1 * rhs.z,
				lhs.x2 * rhs.x + lhs.y2 * rhs.y + lhs.z2 * rhs.z,
				lhs.x3 * rhs.x + lhs.y3 * rhs.y + lhs.z3 * rhs.z);
}

float Mat3::operator[](unsigned idx) const
{
	return v[idx];
}

float & Mat3::operator[](unsigned idx)
{
	return v[idx];
}

float determinant(const Mat3 &lhs)
{
	return ((lhs.mm[0][0] * (lhs.mm[1][1] * lhs.mm[2][2] - lhs.mm[1][2] * lhs.mm[2][1])) -
			(lhs.mm[0][1] * (lhs.mm[1][0] * lhs.mm[2][2] - lhs.mm[1][2] * lhs.mm[2][0])) +
			(lhs.mm[0][2] * (lhs.mm[1][0] * lhs.mm[2][1] - lhs.mm[1][1] * lhs.mm[2][0])));
}

Mat3 transverse(const Mat3 &lhs)
{
	float v[9];

	for (int ii = 0; ii < 3; ++ii)
	{
		for (int jj = 0; jj < 3; ++jj)
		{
			v[ii*3 + jj] = lhs.v[jj*3 + ii];
		}
	}

	return Mat3(v);
}

Mat3 inverse(const Mat3 &lhs)
{
	Mat3 tr = transverse(lhs);

	return (Mat3(determinant(Mat2(tr.v[4], tr.v[5], tr.v[7], tr.v[8])),
			   -determinant(Mat2(tr.v[3], tr.v[5], tr.v[6], tr.v[8])),
				determinant(Mat2(tr.v[3], tr.v[4], tr.v[6], tr.v[7])),

			   -determinant(Mat2(tr.v[1], tr.v[2], tr.v[7], tr.v[8])),
			    determinant(Mat2(tr.v[0], tr.v[2], tr.v[6], tr.v[8])),
			   -determinant(Mat2(tr.v[0], tr.v[1], tr.v[6], tr.v[7])),

				determinant(Mat2(tr.v[1], tr.v[2], tr.v[4], tr.v[5])),
			   -determinant(Mat2(tr.v[0], tr.v[2], tr.v[3], tr.v[5])),
				determinant(Mat2(tr.v[0], tr.v[1], tr.v[3], tr.v[4]))) * (1/determinant(lhs)));
}

Mat3 scale(float w, float h)
{
	return Mat3(w, 0, 0,
				0, h, 0,
				0, 0, 1);
}
Mat3 translate(float x, float y)
{
	return Mat3(1, 0, x,
				0, 1, y,
				0, 0, 1);
}
Mat3 rotateByDegrees(float a)
{

	return Mat3(cos(degreesToRadians(a)), -sin(degreesToRadians(a)), 0,
			    sin(degreesToRadians(a)),  cos(degreesToRadians(a)), 0,
				0,						   0,						1);
}
Mat3 rotateByRadians(float a)
{
	return Mat3(cos(a), -sin(a), 0,
			    sin(a),  cos(a), 0,
				0,		0,		1);
}