#include "ShapeDraw.h"

#include <iostream>
using std::cout;

void drawCircle(const Circle &circle, unsigned color)
{
	sfw::drawCircle(circle.m_pos.x, circle.m_pos.y, circle.m_radius, 12, color);
}

void drawAABB(const AABB &aabb, unsigned color)
{
	sfw::drawLine(aabb.min().x, aabb.min().y, aabb.min().x, aabb.max().y, color); // bl -> tl
	sfw::drawLine(aabb.min().x, aabb.max().y, aabb.max().x, aabb.max().y, color); // tl -> tr
	sfw::drawLine(aabb.max().x, aabb.max().y, aabb.max().x, aabb.min().y, color); // tr -> br
	sfw::drawLine(aabb.max().x, aabb.min().y, aabb.min().x, aabb.min().y, color); // br -> bl
}

void drawBox(const Box &BOX, unsigned color)
{
	sfw::drawLine(BOX.pointsArr[0].x, BOX.pointsArr[0].y, BOX.pointsArr[1].x, BOX.pointsArr[1].y, color); // bl -> tl
	sfw::drawLine(BOX.pointsArr[1].x, BOX.pointsArr[1].y, BOX.pointsArr[2].x, BOX.pointsArr[2].y, color); // tl -> tr
	sfw::drawLine(BOX.pointsArr[2].x, BOX.pointsArr[2].y, BOX.pointsArr[3].x, BOX.pointsArr[3].y, color); // tr -> br
	sfw::drawLine(BOX.pointsArr[3].x, BOX.pointsArr[3].y, BOX.pointsArr[0].x, BOX.pointsArr[0].y, color); // br -> bl
}

void drawPlane(const Plane &plane, unsigned color)
{
	Vec2 point0 = plane.m_position + plane.m_he;
	Vec2 point1 = plane.m_position - plane.m_he;

	sfw::drawLine(point0.x, point0.y, point1.x, point1.y, color);
}

void drawHull(const Hull &hull, unsigned color)
{
	for (int ii = 0; ii < hull.m_size - 1; ++ii)
	{
		Vec2 currPoint = hull.m_vertArray[ii];
		Vec2 nextPoint = hull.m_vertArray[ii + 1];
		sfw::drawLine(currPoint.x, currPoint.y, nextPoint.x, nextPoint.y, color);
	}

	Vec2 lastPoint = hull.m_vertArray[(hull.m_size - 1)];
	Vec2 origPoint = hull.m_vertArray[0];
	sfw::drawLine(lastPoint.x, lastPoint.y, origPoint.x, origPoint.y, color);
}