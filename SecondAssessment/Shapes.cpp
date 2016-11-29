#include "Shapes.h"

#include <iostream>
using std::cout;

Circle::Circle(Circle &circle)
{
	m_radius = circle.m_radius;
	m_pos = circle.m_pos;
}

Circle::Circle(Vec2 new_pos, float new_radius)
{
	m_radius = new_radius;
	m_pos = new_pos;
}

Circle operator* (const Mat3 &MAT3, const Circle &rhs)
{
	Vec3 vec3_scale = MAT3 * Vec3(rhs.m_radius, rhs.m_radius, 1);
	Vec3 vec3_pos   = MAT3 * Vec3(rhs.m_pos.x, rhs.m_pos.y, 1);
	Circle return_circle(Vec2(vec3_pos.x, vec3_pos.y), rhs.m_radius);

	if (vec3_scale.x > vec3_scale.y)
	{
		return_circle.m_radius = vec3_scale.x;
	}
	else
	{
		return_circle.m_radius = vec3_scale.y;
	}

	return return_circle;
}

bool operator== (const Circle &lhs, const Circle &rhs)
{
	return ((floatsAlmostEqual(lhs.m_radius, rhs.m_radius)) && (lhs.m_pos == rhs.m_pos));
}

AABB::AABB()
{
	m_pos = Vec2(0, 0);
	m_he =  Vec2(1, 1);
	m_vel = Vec2(0, 0);
}

AABB::AABB(float pos_x, float pos_y, float he_x, float he_y)
{
	m_pos = Vec2(pos_x, pos_y);
	m_he = Vec2(he_x, he_y);
}

AABB::AABB(Vec2 new_pos, Vec2 new_he)
{
	m_pos = new_pos;
	m_he = new_he;
}

Vec2 AABB::min() const
{
	return m_pos - m_he;
}

Vec2 AABB::max() const
{
	return m_pos + m_he;
}

AABB operator*(const Mat3 &MAT3, const AABB &aabb)
{
	Vec3 vec3_pos = MAT3 * Vec3(aabb.m_pos.x, aabb.m_pos.y, 1);
	Vec3 vec3_trans = MAT3 * Vec3(aabb.m_he.x, aabb.m_he.y, 1);

	Vec3 vecArr[4];

	vecArr[0] = MAT3 * Vec3(aabb.m_pos.x - aabb.m_he.x, aabb.m_pos.y - aabb.m_he.y, 1); // br
	vecArr[1] = MAT3 * Vec3(aabb.m_pos.x - aabb.m_he.x, aabb.m_pos.y + aabb.m_he.y, 1); // tr
	vecArr[2] = MAT3 * Vec3(aabb.m_pos.x + aabb.m_he.x, aabb.m_pos.y + aabb.m_he.y, 1); // tl
	vecArr[3] = MAT3 * Vec3(aabb.m_pos.x + aabb.m_he.x, aabb.m_pos.y - aabb.m_he.y, 1); // bl

	for (int ii = 0; ii < 4; ++ii)
	{
		vecArr[ii] = MAT3 * vecArr[ii];
	}

	float max_x = -INFINITY, max_y = -INFINITY, min_x = INFINITY, min_y = INFINITY;

	for (int ii = 0; ii < 4; ++ii)
	{
		max_x < vecArr[ii].x ? max_x = vecArr[ii].x : max_x;
		min_x > vecArr[ii].x ? min_x = vecArr[ii].x : min_x;
		max_y < vecArr[ii].y ? max_y = vecArr[ii].y : max_y;
		min_y > vecArr[ii].y ? min_y = vecArr[ii].y : min_y;
	}

	return AABB(vec3_pos.x, vec3_pos.y, (max_x - min_x) * 0.5, (max_y - min_y) * 0.5);
}

Plane::Plane()
{
	m_he = Vec2(100, 0);
	m_position = Vec2(0, 0);
	m_direction = Vec2(0, 1);
}

Plane::Plane(float posX, float posY, float heX, float heY)
{
	m_position.x = posX;
	m_position.y = posY;
	m_he.x = heX;
	m_he.y = heY;
	m_direction = perp(normal(Vec2(heX, heY)));
}

Plane::Plane(Vec2 he, Vec2 pos, Vec2 dir)
{
	m_he =		  he;
	m_position =  pos;
	m_direction = normal(dir);
}

Plane operator*(const Mat3 &MAT3, const Plane &PLANE)
{
	Plane retVal;

	retVal.m_position  = (MAT3 * Vec3(PLANE.m_position.x,  PLANE.m_position.y, 1)).xy();

	retVal.m_direction = normal((MAT3 * Vec3(PLANE.m_direction.x, PLANE.m_direction.y, 0)).xy());

	return retVal;
}

Box::Box(float centerPos_x, float centerPos_y, float width, float height)
{
	pointsArr[0] = Vec2(centerPos_x + (width * 0.5), centerPos_y - (height * 0.5)); // br
	pointsArr[1] = Vec2(centerPos_x + (width * 0.5), centerPos_y + (height * 0.5)); // tr
	pointsArr[2] = Vec2(centerPos_x - (width * 0.5), centerPos_y + (height * 0.5)); // tl
	pointsArr[3] = Vec2(centerPos_x - (width * 0.5), centerPos_y - (height * 0.5)); // bl
}

Box::Box(const Box &Box)
{
	pointsArr[0] = Box.pointsArr[0];
	pointsArr[1] = Box.pointsArr[1];
	pointsArr[2] = Box.pointsArr[2];
	pointsArr[3] = Box.pointsArr[3];
}

Box::Box(Vec2 pos0, Vec2 pos1, Vec2 pos2, Vec2 pos3)
{
	pointsArr[0] = pos0;
	pointsArr[1] = pos1;
	pointsArr[2] = pos2;
	pointsArr[3] = pos3;
}

Box operator*(const Mat3 &MAT3, const Box &BOX)
{
	Vec3 vecArr[4];

	vecArr[0] = MAT3 * Vec3(BOX.pointsArr[0].x, BOX.pointsArr[0].y, 1); // br
	vecArr[1] = MAT3 * Vec3(BOX.pointsArr[1].x, BOX.pointsArr[1].y, 1); // tr
	vecArr[2] = MAT3 * Vec3(BOX.pointsArr[2].x, BOX.pointsArr[2].y, 1); // tl
	vecArr[3] = MAT3 * Vec3(BOX.pointsArr[3].x, BOX.pointsArr[3].y, 1); // bl

	return Box(vecArr[0].xy(), vecArr[1].xy(), vecArr[2].xy(), vecArr[3].xy());
}

Ray::Ray(float pos_x0, float pos_y0, float pos_x1, float pos_y1)
{
	m_pos0 = Vec2(pos_x0, pos_y0);
	m_pos1 = Vec2(pos_x1, pos_y1);
}
Ray::Ray(Vec2 pos0, Vec2 pos1)
{
	m_pos0 = pos0;
	m_pos1 = pos1;
}

Ray operator*(const Mat3 &MAT3, const Ray &RAY)
{
	Vec3 vec3_trans0 = MAT3 * Vec3(RAY.m_pos0.x, RAY.m_pos0.y, 1);
	Vec3 vec3_trans1 = MAT3 * Vec3(RAY.m_pos1.x, RAY.m_pos1.y, 1);

	return Ray(vec3_trans0.x, vec3_trans0.y, vec3_trans1.x, vec3_trans1.y);
}

Hull::Hull()
{

}

Hull::Hull(const Vec2 *a_verts, unsigned a_size, const Vec2 a_velocity)
{
	for (int ii = 0; ii < a_size; ++ii)
	{
		m_vertArray[ii] = a_verts[ii];
	}
	m_size = a_size;
	m_velocity = a_velocity;
	updateNormals();
}

void Hull::updateNormals()
{
	for (int ii = 0; ii < m_size-1; ++ii)
	{
		m_normals[ii] = perp(normal(m_vertArray[ii + 1] - m_vertArray[ii]));
	}

	m_normals[m_size] = perp(normal(m_vertArray[0] - m_vertArray[m_size]));
}

bool operator==(const Hull &lhs, const Hull &rhs)
{
	if (lhs.m_size != rhs.m_size)
	{
		return false;
	}

	for (int ii = 0; ii < lhs.m_size; ++ii)
	{
		if (lhs.m_vertArray[ii] != rhs.m_vertArray[ii])
		{
			return false;
		}
	}

	return true;
}

Hull operator*(const Mat3 &MAT3, const Hull &HULL)
{
	Hull retHull;
	retHull.m_size = HULL.m_size;

	for (int ii = 0; ii < HULL.m_size; ++ii)
	{
		retHull.m_vertArray[ii] = (MAT3 * Vec3(HULL.m_vertArray[ii].x, HULL.m_vertArray[ii].y, 0)).xy();
	}

	retHull.updateNormals();

	return retHull;
}