#pragma once

#include "Mat3.h"
#include "Vec2.h"

struct Circle 
{
	Circle();
	Circle(Vec2 new_pos, float new_radius);
	Circle(Circle &circle);

	Vec2 m_pos;
	float m_radius;
};

Circle operator*(const Mat3 &MAT3, const Circle &CIR);
bool operator==(const Circle & lhs, const Circle &rhs);

struct AABB
{
	Vec2 m_pos, m_vel, m_he;

	AABB();
	AABB(float pos_x, float pos_y, float dim_x, float dim_y);
	AABB(Vec2 new_pos, Vec2 new_dim);

	Vec2 min() const;
	Vec2 max() const;
};
AABB operator*(const Mat3 &MAT3, const AABB &aabb);

struct Plane
{
	Vec2 m_he, m_position, m_direction;

	Plane();
	Plane(float posX, float posY, float heX, float heY);
	Plane(Vec2 he, Vec2 pos, Vec2 dir);
};

Plane operator*(const Mat3 &MAT3, const Plane &PLANE);

struct Box
{
	Vec2 pointsArr[4];
	Vec2 m_pos;

	Box();
	Box(float centerPos_x, float centerPos_y, float width, float height);
	Box(Vec2 pos0, Vec2 pos1, Vec2 pos2, Vec2 pos3);
	Box(const Box &Box);
	void moveBoxToPoint(Vec2 newPos);
};

Box operator*(const Mat3 &MAT3, const Box &Box);

struct Ray
{
public:
	Ray(float pos_x0, float pos_y0, float x1, float y1);
	Ray(Vec2 pos0, Vec2 pos1);

	Vec2 m_pos0, m_pos1;
};

Ray operator*(const Mat3 &MAT3, const Ray &RAY);

struct Hull
{
	Vec2 m_vertArray[16];
	Vec2 m_normals[16];
	unsigned int m_size;
	Vec2 m_velocity;

	void updateNormals();
	Hull();
	Hull(const Vec2 *a_verts, unsigned a_size, const Vec2 a_velocity = Vec2(0, 0));
};
bool operator==(const Hull &lhs, const Hull &rhs);

Hull operator*(const Mat3 &MAT3, const Hull &HULL);
