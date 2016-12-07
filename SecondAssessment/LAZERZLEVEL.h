#pragma once

#include "States.h"
#include "Button.h"
#include "Shapes.h"
#include "ShapeDraw.h"
#include "Vec2.h"
#include "sfwdraw.h"
#include "Collision.h"
#include "LAZERZCANNON.h"
#include <vector>
using std::vector;

#include <iostream>
using std::cout;

class LAZERZLEVEL
{
	AABB m_aabbArr[20];
	AABB m_target[5];
	Box m_mirrors[20];
	Transform m_mirrors_transforms[20];
	bool m_mirror_bounced[20], secondBounce = false;
	vector<Vec2>  m_bouncePointsAndDestinations;
	LAZERZCANNON m_lazerz_cannon;
	unsigned m_aabb_arr_size, m_mirrors_size, m_target_size;

	unsigned m_font = sfw::loadTextureMap("./res/tonc_font.png", 16, 6);

public:
	LAZERZLEVEL();
	Vec2 lazerzCollisionDetection(Vec2 lineDirection, Vec2 linePosition);
	Vec2 lazerzCollisionReflection(Vec2 lineDirection, Vec2 linePosition);
	void init();
	void draw();
	void step();
};

LAZERZLEVEL::LAZERZLEVEL()
{
	m_aabbArr[0] = AABB(500, 50, 500, 50);   // Ground
	m_aabbArr[1] = AABB(0, 550, 150, 450);   // Left
	m_aabbArr[2] = AABB(950, 450, 50, 350);  // Right
	m_aabbArr[3] = AABB(575, 900, 425, 100); // Roof
	m_aabbArr[4] = AABB(400, 600, 50, 200);  // Blocker
	m_aabb_arr_size = 5;

	m_mirrors[0] = Box(0, 0, 5, 200);
	m_mirrors_transforms[0].m_facing = -45;
	m_mirrors_transforms[0].translateByMat3 = translate(880, 110);

	m_mirrors[1] = Box(0, 0, 5, 200);
	m_mirrors_transforms[1].translateByMat3 = translate(530, 500);
	m_mirrors_size = 2;

	m_target[0] = AABB(525, 525, 20, 20);
	m_target_size = 1;
}

Vec2 LAZERZLEVEL::lazerzCollisionDetection(Vec2 lineDirection, Vec2 linePosition)
{
	Vec2 perpDirection = perp(lineDirection);
	float perp_line_dot_product = dot(lineDirection, linePosition),
		line_dot_product = dot(perp(lineDirection), linePosition),
		closestCollPointCloseness = INFINITY;

	Vec2 closestCollPoint;

	for (int ii = 0; ii < m_aabb_arr_size; ++ii)
	{
		float aMin, aMax;

		Vec2 vertArr[4];
		vertArr[0] = Vec2(m_aabbArr[ii].min().x, m_aabbArr[ii].max().y); // TL
		vertArr[1] = m_aabbArr[ii].min();								 // BL
		vertArr[2] = Vec2(m_aabbArr[ii].max().x, m_aabbArr[ii].min().y); // BR
		vertArr[3] = m_aabbArr[ii].max();								 // TR

		// Find the distance from the AABB to the lazer
		for (int jj = 0; jj < 4; ++jj)
		{
			float dotProduct_0 = dot(perp(lineDirection), vertArr[jj]);
			float dotProduct_1 = dot(perp(lineDirection), vertArr[(jj + 1) % 4]);
			aMin = fminf(dotProduct_0, dotProduct_1);
			aMax = fmaxf(dotProduct_0, dotProduct_1);

			if (aMin <= line_dot_product && aMax >= line_dot_product)
			{

				Vec2 velLine = slopeAndConstOfVectorAndVelocity(vertArr[jj], vertArr[(jj + 1) % 4] - vertArr[jj]),
					planeLine = slopeAndConstOfVectorAndVelocity(linePosition, lineDirection);

				// Find the collision point on the AABB
				Vec2 collPoint = pointOfCollisionBetweenLines(velLine, planeLine, vertArr[jj].x, linePosition.x);

				// Check if the collision point is closest to the laser without going past it
				float dotProduct_1 = dot(lineDirection, collPoint);
				if (dotProduct_1 >= perp_line_dot_product && dotProduct_1 < closestCollPointCloseness)
				{
					closestCollPointCloseness = dotProduct_1;
					closestCollPoint = collPoint;
				}
			}
		}
	}

	return closestCollPoint;
}

Vec2 LAZERZLEVEL::lazerzCollisionReflection(Vec2 lineDirection, Vec2 linePosition)
{
	Vec2 perpDirection = perp(lineDirection);
	float perp_line_dot_product = dot(lineDirection, linePosition),
		line_dot_product = dot(perp(lineDirection), linePosition),
		closestCollPointCloseness = INFINITY;

	Vec2 closestCollPoint, closestPointReflection = Vec2(-INFINITY, -INFINITY);

	Box currMirror;
	for (int ii = 0; ii < m_mirrors_size; ++ii)
	{
		currMirror = m_mirrors_transforms[ii].getGlobalTransform() * m_mirrors[ii];
		float aMin, aMax;
																		 // Find the distance from the AABB to the lazer
		for (int jj = 0; jj < 4; ++jj)
		{
			float dotProduct_0 = dot(perp(lineDirection), currMirror.pointsArr[jj]);
			float dotProduct_1 = dot(perp(lineDirection), currMirror.pointsArr[(jj + 1) % 4]);
			aMin = fminf(dotProduct_0, dotProduct_1);
			aMax = fmaxf(dotProduct_0, dotProduct_1);

			if (aMin <= line_dot_product && aMax >= line_dot_product)
			{
				Vec2 reflectVec = reflection(perp(currMirror.pointsArr[jj] - currMirror.pointsArr[(jj + 1) % 4]), lineDirection*2000);
				cout << "currMirror: " << perp(currMirror.pointsArr[jj] - currMirror.pointsArr[(jj + 1) % 4]).x << ", " << perp(currMirror.pointsArr[jj] - currMirror.pointsArr[(jj + 1) % 4]).y << "\n";
				cout << "reflectVec: " << reflectVec.x << ", " << reflectVec.y << "\n";
				cout << "lineDirection: " << lineDirection.x << ", " << lineDirection.y << "\n";

				Vec2 velLine = slopeAndConstOfVectorAndVelocity(currMirror.pointsArr[jj], currMirror.pointsArr[(jj + 1) % 4] - currMirror.pointsArr[jj]),
					planeLine = slopeAndConstOfVectorAndVelocity(linePosition, lineDirection);

				// Find the collision point on the AABB
				Vec2 collPoint = pointOfCollisionBetweenLines(velLine, planeLine, currMirror.pointsArr[jj].x);

				// Check if the collision point is closest to the laser without going past it
				float dotProduct_1 = dot(lineDirection, collPoint);
				if (dotProduct_1 >= perp_line_dot_product && dotProduct_1 < closestCollPointCloseness)
				{
					closestCollPointCloseness = dotProduct_1;
					closestCollPoint = collPoint;
					closestPointReflection = reflectVec;
				}
			}
		}
	}
	if (closestPointReflection.x != -INFINITY)
	{
		m_bouncePointsAndDestinations.push_back(closestCollPoint);
		m_bouncePointsAndDestinations.push_back(closestPointReflection + linePosition);
	}
	cout << "\n\n";
	return closestPointReflection;
}

void LAZERZLEVEL::init()
{
	m_lazerz_cannon.positionCannon(Vec2(200, 120));
}

void LAZERZLEVEL::draw()
{
	for (int ii = 0; ii < m_aabb_arr_size; ++ii)
	{
		drawAABB(m_aabbArr[ii], GREEN);
	}

	for (int ii = 0; ii < m_mirrors_size; ++ii)
	{
		drawBox(m_mirrors_transforms[ii].getGlobalTransform() * m_mirrors[ii], WHITE);
	}

	for (int ii = 0; ii < m_target_size; ++ii)
	{
		drawAABB(m_target[ii], RED);
	}

	Vec2 initEnd = lazerzCollisionDetection(m_lazerz_cannon.lazerzDirection(), m_lazerz_cannon.lazerzOrigin());
	sfw::drawLine(m_lazerz_cannon.m_head.m_pos.x, m_lazerz_cannon.m_head.m_pos.y, initEnd.x, initEnd.y, YELLOW);

	//if (m_bouncePointsAndDestinations.size() > 0)
	//{
	//	sfw::drawLine(m_lazerz_cannon.m_head.m_pos.x, m_lazerz_cannon.m_head.m_pos.y, m_bouncePointsAndDestinations[0].x, m_bouncePointsAndDestinations[0].y, BLUE);
	//	cout << "m_bouncePointsAndDestinations: " << m_bouncePointsAndDestinations[0].x << ", " << m_bouncePointsAndDestinations[0].y << "\n";

	//	m_bouncePointsAndDestinations.clear();
	//}

	//cout << "m_bouncePointsAndDestinations.size(): " << m_bouncePointsAndDestinations.size() << "\n";
	for (int ii = 0; ii < m_bouncePointsAndDestinations.size(); ii+=2)
	{
		cout << m_bouncePointsAndDestinations[ii].x << ", " << m_bouncePointsAndDestinations[ii].y << "\n";
		sfw::drawLine(m_bouncePointsAndDestinations[ii].x, m_bouncePointsAndDestinations[ii].y, m_bouncePointsAndDestinations[ii+1].x, m_bouncePointsAndDestinations[ii+1].y, YELLOW);
		drawCircle(Circle(Vec2(m_bouncePointsAndDestinations[ii].x, m_bouncePointsAndDestinations[ii].y), 16), YELLOW);
	}

	m_lazerz_cannon.draw();

	//Vec2 collPoint;
	//if (m_bouncePointsAndDestinations.size() == 0)
	//	collPoint = lazerzCollisionDetection(m_lazerz_cannon.lazerzDirection(), m_lazerz_cannon.lazerzOrigin());
	//else
	//	collPoint = lazerzCollisionDetection(m_bouncePointsAndDestinations[m_bouncePointsAndDestinations.size() - 2],
	//										  m_bouncePointsAndDestinations[m_bouncePointsAndDestinations.size() - 1]);
	//drawCircle(Circle(collPoint, 16), YELLOW);

	m_bouncePointsAndDestinations.clear();
}

void LAZERZLEVEL::step()
{
	m_lazerz_cannon.step();

	Vec2 reflectVal = Vec2(1, 1);

	if (sfw::getKey('Q'))
	{
		m_mirrors_transforms[0].rotateLocalTransform(0.5);
	}
	else if (sfw::getKey('E'))
	{
		m_mirrors_transforms[0].rotateLocalTransform(-0.5);
	}
	//while (reflectVal != Vec2(-INFINITY, -INFINITY))
	{
		reflectVal = lazerzCollisionReflection(m_lazerz_cannon.lazerzDirection(), m_lazerz_cannon.lazerzOrigin());

		if (m_bouncePointsAndDestinations.size() != 0)
			reflectVal = lazerzCollisionReflection(m_bouncePointsAndDestinations[m_bouncePointsAndDestinations.size() - 2],
													m_bouncePointsAndDestinations[m_bouncePointsAndDestinations.size() - 1]);
		//else
		//	reflectVal = lazerzCollisionReflection(m_lazerz_cannon.lazerzDirection(), m_lazerz_cannon.lazerzOrigin());
	}
}

//LAZERZMENU LAZERZLEVEL::next()
//{
//	return m_state;
//}

