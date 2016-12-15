#pragma once

#include "States.h"
#include "Button.h"
#include "Shapes.h"
#include "ShapeDraw.h"
#include "Vec2.h"
#include "sfwdraw.h"
#include "Collision.h"
#include "LAZERZCANNON.h"
#include "LAZERZTARGET.h"
#include <vector>
using std::vector;

#include <iostream>
using std::cout;

struct reflectionData
{
	Vec2 lazerDirection, lazerEndPoint, objectLineOriginPoint, objectLineEndPoint;
	bool isReflective;
	float closenessOfCollision;
};

class LAZERZLEVEL
{
	AABB m_aabbArr[20];
	Box m_mirrors[20];
	Transform m_mirrors_transforms[20];
	bool m_mirror_bounced[20], secondBounce = false;
	vector<Vec2>  m_bounceOriginAndDirections;
	LAZERZCANNON m_lazerz_cannon;
	LAZERZTARGET m_lazerz_target;
	Button fire_lazerz_button;
	unsigned m_aabb_arr_size, m_mirrors_size, m_target_size;
	unsigned m_font = sfw::loadTextureMap("./res/tonc_font.png", 16, 6);
	unsigned int mouse_image = sfw::loadTextureMap("./Images/LAZERZ_Button.png", 16, 6);

public:
	LAZERZLEVEL(); 
	reflectionData lazerzCollisionDetectionTarget(Vec2 lineDirection, Vec2 linePosition);
	reflectionData lazerzCollisionDetection(Vec2 lineDirection, Vec2 linePosition);
	reflectionData lazerzCollisionDetectionBox(Vec2 lineDirection, Vec2 linePosition);
	int MirrorMouseCollisionDetection();
	void init();
	void draw();
	void step();
};

LAZERZLEVEL::LAZERZLEVEL()
{
	fire_lazerz_button.m_pos = Vec2(850, 900);
	fire_lazerz_button.m_dim = Vec2(100, 50);
	fire_lazerz_button.m_texture = ("./Images/LAZERZ_Button.png", 16, 6);

	m_aabbArr[0] = AABB(500,  50, 500,  50);   // Ground
	m_aabbArr[1] = AABB(  0, 550, 150, 450);   // Left
	m_aabbArr[2] = AABB(950, 450,  50, 350);  // Right
	m_aabbArr[3] = AABB(575, 900, 425, 100); // Roof
	m_aabbArr[4] = AABB(400, 600,  50, 200);  // Blocker
	m_aabb_arr_size = 5;

	m_mirrors[0] = Box(0, 0, 20, 200);
	m_mirrors_transforms[0].m_facing = -30;
	m_mirrors_transforms[0].translateByMat3 = translate(880, 110);

	m_mirrors[1] = Box(0, 0, 20, 200);
	m_mirrors_transforms[1].translateByMat3 = translate(800, 500);
	m_mirrors_size = 2;

	m_lazerz_target.init();
}

reflectionData LAZERZLEVEL::lazerzCollisionDetectionTarget(Vec2 lineDirection, Vec2 linePosition)
{
	linePosition += normal(lineDirection);
	float line_dot_product = dot(lineDirection, linePosition),
		perp_line_dot_product = dot(perp(lineDirection), linePosition),
		closestCollPointCloseness = INFINITY;

	reflectionData reflectData;
	reflectData.closenessOfCollision = closestCollPointCloseness;
	reflectData.lazerEndPoint = Vec2(-INFINITY, -INFINITY);
	Vec2 closestCollPoint, closestLineOriginPoint, closestLineEndPoint;

	float aMin, aMax;

	for (int jj = 0; jj < 4; ++jj)
	{
		float dotProduct_0 = dot(perp(lineDirection), m_lazerz_target.m_body.pointsArr[jj]);
		float dotProduct_1 = dot(perp(lineDirection), m_lazerz_target.m_body.pointsArr[(jj + 1) % 4]);
		Vec2 lineBegin = m_lazerz_target.m_body.pointsArr[jj],
			lineEnd = m_lazerz_target.m_body.pointsArr[(jj + 1) % 4];

		aMin = fminf(dotProduct_0, dotProduct_1);
		aMax = fmaxf(dotProduct_0, dotProduct_1);


		if (aMin <= perp_line_dot_product && aMax >= perp_line_dot_product)
		{
			Vec2 velLine = slopeAndConstOfVectorAndVelocity(m_lazerz_target.m_body.pointsArr[jj], m_lazerz_target.m_body.pointsArr[(jj + 1) % 4] - m_lazerz_target.m_body.pointsArr[jj]),
				planeLine = slopeAndConstOfVectorAndVelocity(linePosition, lineDirection);

			// Find the collision point on the target
			Vec2 collPoint = pointOfCollisionBetweenLines(velLine, planeLine, m_lazerz_target.m_body.pointsArr[jj].x, linePosition.x);

			// Check if the collision point is closest to the laser without going past it
			float dotProduct_1 = dot(lineDirection, collPoint);

			if (dotProduct_1 >= line_dot_product && dotProduct_1 < closestCollPointCloseness)
			{
				closestCollPointCloseness = dotProduct_1;
				closestCollPoint = collPoint;

				reflectData.lazerEndPoint = collPoint;
				reflectData.closenessOfCollision = closestCollPointCloseness;
			}
		}
	}

	return reflectData;
}

reflectionData LAZERZLEVEL::lazerzCollisionDetection(Vec2 lineDirection, Vec2 linePosition)
{
	reflectionData reflectData;

	float line_dot_product = dot(lineDirection, linePosition),
		perp_line_dot_product = dot(perp(lineDirection), linePosition),
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

			if (aMin <= perp_line_dot_product && aMax >= perp_line_dot_product)
			{
				Vec2 velLine = slopeAndConstOfVectorAndVelocity(vertArr[jj], vertArr[(jj + 1) % 4] - vertArr[jj]),
					planeLine = slopeAndConstOfVectorAndVelocity(linePosition, lineDirection);

				// Find the collision point on the AABB
				Vec2 collPoint = pointOfCollisionBetweenLines(velLine, planeLine, vertArr[jj].x, linePosition.x);

				// Check if the collision point is closest to the laser without going past it
				float dotProduct_1 = dot(lineDirection, collPoint);

				if (dotProduct_1 >= line_dot_product && dotProduct_1 < closestCollPointCloseness)
				{
					closestCollPointCloseness = dotProduct_1;
					closestCollPoint = collPoint;

					reflectData.lazerEndPoint = collPoint;
					reflectData.closenessOfCollision = closestCollPointCloseness;
				}
			}
		}
	}

	return reflectData;
}

reflectionData LAZERZLEVEL::lazerzCollisionDetectionBox(Vec2 lineDirection, Vec2 linePosition)
{
	linePosition += normal(lineDirection);
	float line_dot_product = dot(lineDirection, linePosition),
		perp_line_dot_product = dot(perp(lineDirection), linePosition),
		closestCollPointCloseness = INFINITY;

	reflectionData reflectData;
	reflectData.lazerEndPoint = Vec2(-INFINITY, -INFINITY);
	Vec2 closestCollPoint, closestLineOriginPoint, closestLineEndPoint;
	Box currMirror;

	for (int ii = 0; ii < m_mirrors_size; ++ii)
	{
		float aMin, aMax;
		currMirror = m_mirrors_transforms[ii].getGlobalTransform() * m_mirrors[ii];

		for (int jj = 0; jj < 4; ++jj)
		{
			float dotProduct_0 = dot(perp(lineDirection), currMirror.pointsArr[jj]);
			float dotProduct_1 = dot(perp(lineDirection), currMirror.pointsArr[(jj + 1) % 4]);
			Vec2 lineBegin = currMirror.pointsArr[jj],
				 lineEnd   = currMirror.pointsArr[(jj + 1) % 4];

			aMin = fminf(dotProduct_0, dotProduct_1);
			aMax = fmaxf(dotProduct_0, dotProduct_1);

			if (aMin <= perp_line_dot_product && aMax >= perp_line_dot_product)
			{
				Vec2 velLine = slopeAndConstOfVectorAndVelocity(currMirror.pointsArr[jj], currMirror.pointsArr[(jj + 1) % 4] - currMirror.pointsArr[jj]),
					planeLine = slopeAndConstOfVectorAndVelocity(linePosition, lineDirection);

				// Find the collision point on the Mirror
				Vec2 collPoint = pointOfCollisionBetweenLines(velLine, planeLine, currMirror.pointsArr[jj].x, linePosition.x);

				// Check if the collision point is closest to the laser without going past it
				float dotProduct_1 = dot(lineDirection, collPoint);

				if (dotProduct_1 >= line_dot_product && dotProduct_1 < closestCollPointCloseness)
				{
					closestCollPointCloseness = dotProduct_1;
					closestCollPoint = collPoint;

					reflectData.lazerEndPoint         = collPoint;
					reflectData.objectLineOriginPoint = lineBegin;
					reflectData.objectLineEndPoint    = lineEnd;
					reflectData.lazerDirection = lineDirection;
					reflectData.closenessOfCollision = closestCollPointCloseness;
				}
			}
		}
	}

	return reflectData;
}

void LAZERZLEVEL::init()
{
	m_lazerz_cannon.positionCannon(Vec2(200, 120));
}

void LAZERZLEVEL::draw()
{
	fire_lazerz_button.draw();

	for (int ii = 0; ii < m_aabb_arr_size; ++ii)
	{
		drawAABB(m_aabbArr[ii], GREEN);
	}

	for (int ii = 0; ii < m_mirrors_size; ++ii)
	{
		drawBox(m_mirrors_transforms[ii].getGlobalTransform() * m_mirrors[ii], WHITE);
	}

	m_lazerz_target.draw();

	reflectionData hardCollisionData = lazerzCollisionDetection(m_lazerz_cannon.lazerzDirection(), m_lazerz_cannon.lazerzOrigin());
	reflectionData targetCollisionData = lazerzCollisionDetectionTarget(m_lazerz_cannon.lazerzDirection(), m_lazerz_cannon.lazerzOrigin());
	reflectionData p = lazerzCollisionDetectionBox(m_lazerz_cannon.lazerzDirection(), m_lazerz_cannon.lazerzOrigin());
	Vec2 lazerOrigin = m_lazerz_cannon.lazerzOrigin();
	Vec2 reflectVec = reflection(perp(p.objectLineOriginPoint - p.objectLineEndPoint), p.lazerDirection * 2000);
	if(hardCollisionData.closenessOfCollision < p.closenessOfCollision && hardCollisionData.closenessOfCollision < targetCollisionData.closenessOfCollision)
		sfw::drawLine(lazerOrigin.x, lazerOrigin.y, hardCollisionData.lazerEndPoint.x, hardCollisionData.lazerEndPoint.y, BLUE);
	else if (targetCollisionData.closenessOfCollision < p.closenessOfCollision)
	{
		sfw::drawLine(lazerOrigin.x, lazerOrigin.y, targetCollisionData.lazerEndPoint.x, targetCollisionData.lazerEndPoint.y, BLUE);
	}
	else
	{
		int limit = 20, ii = 0;
		while (p.lazerEndPoint.x != -INFINITY && ii < limit)
		{
			sfw::drawLine(lazerOrigin.x, lazerOrigin.y, p.lazerEndPoint.x, p.lazerEndPoint.y, YELLOW);

			lazerOrigin = p.lazerEndPoint;
			reflectVec = reflection(perp(p.objectLineOriginPoint - p.objectLineEndPoint), p.lazerDirection * 2000);
			p = lazerzCollisionDetectionBox(reflectVec, lazerOrigin);

			hardCollisionData = lazerzCollisionDetection(reflectVec, lazerOrigin);
			targetCollisionData = lazerzCollisionDetectionTarget(reflectVec, lazerOrigin);
			if (hardCollisionData.closenessOfCollision < p.closenessOfCollision || targetCollisionData.closenessOfCollision < p.closenessOfCollision)
				break;

			++ii;
		}
	}

	if (!reflectVec.x)
		reflectVec = m_lazerz_cannon.lazerzDirection();

	if (hardCollisionData.closenessOfCollision < targetCollisionData.closenessOfCollision)
		sfw::drawLine(lazerOrigin.x, lazerOrigin.y, hardCollisionData.lazerEndPoint.x, hardCollisionData.lazerEndPoint.y, BLUE);
	else
		sfw::drawLine(lazerOrigin.x, lazerOrigin.y, targetCollisionData.lazerEndPoint.x, targetCollisionData.lazerEndPoint.y, BLUE);

	m_lazerz_cannon.draw();

	m_bounceOriginAndDirections.clear();

	sfw::drawTexture(mouse_image, sfw::getMouseX(), sfw::getMouseY(), 10, 10);
}

int LAZERZLEVEL::MirrorMouseCollisionDetection()
{
	float closestCollPointCloseness = INFINITY;
	Box currMirror;
	int hitMirrorIndex = -1;

	for (int ii = 0; ii < m_mirrors_size; ++ii)
	{
		float aMin, aMax;
		currMirror = m_mirrors_transforms[ii].getGlobalTransform() * m_mirrors[ii];

		for (int jj = 0; jj < 4; ++jj)
		{
			Vec2 lineDirection = currMirror.pointsArr[jj] - currMirror.pointsArr[(jj + 1) % 4];
			float dotProduct_0 = dot(perp(lineDirection), Vec2(sfw::getMouseX(), sfw::getMouseY())),
				  dotProduct_1 = dot(perp(lineDirection), currMirror.pointsArr[jj]);

			if (dotProduct_0 <= dotProduct_1)
			{

				// Check if the collision point is closest to the laser without going past it
				float dotProduct_1 = dot(lineDirection, collPoint);

				if (dotProduct_1 >= line_dot_product && dotProduct_1 < closestCollPointCloseness)
				{
					closestCollPointCloseness = dotProduct_1;
					closestCollPoint = collPoint;

					reflectData.lazerEndPoint = collPoint;
					reflectData.objectLineOriginPoint = lineBegin;
					reflectData.objectLineEndPoint = lineEnd;
					reflectData.lazerDirection = lineDirection;
					reflectData.closenessOfCollision = closestCollPointCloseness;
				}
			}
		}
	}

	return 0; // hitMirrorIndex;
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
	if (sfw::getKey('W'))
	{
		m_mirrors_transforms[1].rotateLocalTransform(0.5);
	}
	else if (sfw::getKey('S'))
	{
		m_mirrors_transforms[1].rotateLocalTransform(-0.5);
	}
	
	m_lazerz_target.step();
	if (fire_lazerz_button.mouseUp())
	{
		m_lazerz_target.m_deathSequenceIsActive = true;
	}
	fire_lazerz_button.mouseDown();
}

//LAZERZMENU LAZERZLEVEL::next()
//{
//	return m_state;
//}

