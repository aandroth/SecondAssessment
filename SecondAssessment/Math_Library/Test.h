#pragma once

#include "Flops.h"
#include <cmath>
#include "Vec2.h"
#include "Vec3.h"

#ifndef TEST_H
#define TEST_H

struct Line
{
	float a, b, c, d;
	Line(float newA, float newB, float newC, float newD)
	{
		a = newA;
		b = newB;
		c = newC;
		d = newD;
	}
};

int doNothing(int);
int orderTwo_Formula(int);
Vec2 quadratic(int, int, int);
float linear_blend(int, int, int);
float vector_difference(Vec2, Vec2);
float inner_product(Vec3, Vec3);
float point_line_distance(Vec3, Line);
float bezier_curve(float, Line);

#endif