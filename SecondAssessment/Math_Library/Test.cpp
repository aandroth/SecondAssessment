#include "Test.h"

int doNothing(int x)
{
	return x;
}

int orderTwo_Formula(int x)
{
	return x*x + 2 * x + 7;
}

Vec2 quadratic(int a, int b, int c)
{
	float d = (b*b - 4 * a*c);

	if (d < 0)
	{
		return Vec2(NULL, NULL);
	}

	float leftResult = ((b - d) / (2 * a));
	float rightResult = ((b + d) / (2 * a));
	return Vec2(leftResult, rightResult);
}

float linear_blend(int s, int e, int t)
{
	return s + t*(e - s);
}

float vector_difference(Vec2 a, Vec2 b)
{
	return sqrt((b.x - a.x)*(b.x - a.x) + (b.y - a.y)*(b.y - a.y));
}

float inner_product(Vec3 a, Vec3 b)
{
	return (b.x * a.x) + (b.y * a.y) + (b.z * a.z);
}

float point_line_distance(Vec3 point, Line Line)
{
	float numerator = (Line.a * point.x + Line.b*point.y + Line.c*point.z + Line.d);
	float denom = sqrt(Line.a*Line.a + Line.b*Line.b + Line.c*Line.c);

	return numerator / denom;
}

float bezier_curve(float t, Line Line)
{
	return pow((1 - t), 3)*Line.a + 3*pow((1 - t), 2)*Line.b + 3*(1 - t)*t*t*Line.c + pow(t, 3)*Line.d;
}