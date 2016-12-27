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
	int lastMirrorIndex;
};

class LAZERZLEVEL
{
	AABB m_aabbArr[20];
	Box m_mirrors[20];
	Transform m_mirrors_transforms[20];
	bool m_mirror_bounced[20], targetIsInSight = false, lazerIsFired = false, lazerIsOnTargetLine = false, mirrorMoving = false, mouseIsDown = false;
	vector<Vec2>  m_bounceOriginAndDirections;
	LAZERZCANNON m_lazerz_cannon;
	LAZERZTARGET m_lazerz_target;
	Button fire_lazerz_button, add_mirror_button, del_mirror_button;
	size_t m_aabb_arr_size, m_mirrors_size, m_mirror_colors[20], indexOfSelectedMirror = -1;
	unsigned m_font = sfw::loadTextureMap("./res/tonc_font.png", 16, 6);
	unsigned int mouse_image = sfw::loadTextureMap("./Images/LAZERZ_Button.png", 16, 6);
	float timePassed;
	Vec2 currLazerOrigin, currLazerDir, currLazerEnd, progress, currMousePos;
	reflectionData nextLazer;

public:
	LAZERZLEVEL(); 
	reflectionData lazerzCollisionDetectionTarget(Vec2 lineDirection, Vec2 linePosition);
	reflectionData lazerzCollisionDetection(Vec2 lineDirection, Vec2 linePosition);
	reflectionData lazerzCollisionDetectionBox(Vec2 lineDirection, Vec2 linePosition, int lastMirroIndex);
	int MirrorMouseCollisionDetection();
	void init();
	void draw();
	void step();
};

LAZERZLEVEL::LAZERZLEVEL()
{
	fire_lazerz_button.m_pos = Vec2(600, 900);
	fire_lazerz_button.m_dim = Vec2(100, 50);
	fire_lazerz_button.m_texture = ("./Images/LAZERZ_Button.png", 16, 6);

	add_mirror_button.m_pos = Vec2(750, 900);
	add_mirror_button.m_dim = Vec2(100, 50);
	add_mirror_button.m_texture = ("./Images/LAZERZ_Button.png", 16, 6);

	del_mirror_button.m_pos = Vec2(900, 900);
	del_mirror_button.m_dim = Vec2(100, 50);
	del_mirror_button.m_texture = ("./Images/LAZERZ_Button.png", 16, 6);

	m_aabbArr[0] = AABB(500,  50, 500,  50);   // Ground
	m_aabbArr[1] = AABB(  0, 550, 150, 450);   // Left
	m_aabbArr[2] = AABB(950, 450,  50, 350);  // Right
	m_aabbArr[3] = AABB(575, 900, 425, 100); // Roof
	m_aabbArr[4] = AABB(400, 600,  50, 200);  // Blocker
	m_aabb_arr_size = 5;

	m_mirrors[0] = Box(0, 0, 20, 200);
	m_mirrors_transforms[0].m_facing = -30;
	m_mirrors_transforms[0].m_position = Vec2(880, 110);
	m_mirror_colors[0] = WHITE;

	m_mirrors[1] = Box(0, 0, 20, 200);
	m_mirrors_transforms[1].m_position = Vec2(800, 350);
	m_mirror_colors[1] = WHITE;

	m_mirrors[2] = Box(0, 0, 20, 200);
	m_mirrors_transforms[2].m_position = Vec2(840, 350);
	m_mirror_colors[2] = WHITE;
	m_mirrors_size = 3;

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

reflectionData LAZERZLEVEL::lazerzCollisionDetectionBox(Vec2 lineDirection, Vec2 linePosition, int lastMirrorIndex)
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
		if (ii == lastMirrorIndex)
			continue;

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

				//if ((ii == 1 && jj == 2) || (ii == 2 && jj == 0))
				//{
				//	//cout << "idx: " << ii << ", " << jj << "\n";
				//	//cout << "velLine: " << velLine.x << ", " << velLine.y << "\n";
				//	//cout << "planeLine: " << planeLine.x << ", " << planeLine.y << "\n";
				//	//cout << "collPoint: " << collPoint.x << ", " << collPoint.y << "\n";
				//	//cout << "lineDirection: " << lineDirection.x << ", " << lineDirection.y << "\n";
				//	//cout << "linePosition: " << linePosition.x << ", " << linePosition.y << "\n";
				//	//cout << "closestCollPointCloseness: " << closestCollPointCloseness << "\n";
				//	//cout << "dotProduct_1: " << dotProduct_1 << "\n";
				//	//cout << "line_dot_product: " << line_dot_product << "\n\n";
				//}

				if (dotProduct_1 >= line_dot_product && dotProduct_1 < closestCollPointCloseness)
				{
					closestCollPointCloseness = dotProduct_1;
					closestCollPoint = collPoint;

					reflectData.lazerEndPoint         = collPoint;
					reflectData.objectLineOriginPoint = lineBegin;
					reflectData.objectLineEndPoint    = lineEnd;
					reflectData.lazerDirection = lineDirection;
					reflectData.closenessOfCollision = closestCollPointCloseness;
					reflectData.lastMirrorIndex = ii;
				}
			}
		}
	}

	return reflectData;
}

void LAZERZLEVEL::init()
{
	m_lazerz_cannon.positionCannon(Vec2(200, 120));
	timePassed = 0.0;
	currLazerOrigin = m_lazerz_cannon.lazerzOrigin();
	currLazerDir = m_lazerz_cannon.lazerzDirection();
	nextLazer = lazerzCollisionDetectionBox(m_lazerz_cannon.lazerzDirection(), m_lazerz_cannon.lazerzOrigin(), -1);
	currLazerEnd = nextLazer.lazerEndPoint;
}

void LAZERZLEVEL::draw()
{
	fire_lazerz_button.draw();
	add_mirror_button.draw();
	del_mirror_button.draw();

	for (int ii = 0; ii < m_aabb_arr_size; ++ii)
	{
		drawAABB(m_aabbArr[ii], GREEN);
	}

	for (int ii = 0; ii < m_mirrors_size; ++ii)
	{
		drawBox(m_mirrors_transforms[ii].getGlobalTransform() * m_mirrors[ii], m_mirror_colors[ii]);
	}

	m_lazerz_target.draw();

	if (!lazerIsFired)
	{
		reflectionData hardCollisionData = lazerzCollisionDetection(m_lazerz_cannon.lazerzDirection(), m_lazerz_cannon.lazerzOrigin());
		reflectionData targetCollisionData = lazerzCollisionDetectionTarget(m_lazerz_cannon.lazerzDirection(), m_lazerz_cannon.lazerzOrigin());
		reflectionData p = lazerzCollisionDetectionBox(m_lazerz_cannon.lazerzDirection(), m_lazerz_cannon.lazerzOrigin(), -1);
		//cout << "p.lazerEndPoint: " << p.lazerEndPoint.x << ", " << p.lazerEndPoint.y << "\n";
		Vec2 lazerOrigin = m_lazerz_cannon.lazerzOrigin();
		Vec2 reflectVec = reflection(perp(p.objectLineOriginPoint - p.objectLineEndPoint), p.lazerDirection * 1000);
		int limit = 20, ii = 0;
		if (p.closenessOfCollision < hardCollisionData.closenessOfCollision && 
			p.closenessOfCollision < targetCollisionData.closenessOfCollision)
		{
			while (p.lazerEndPoint.x != -INFINITY && ii < limit)
			{
				sfw::drawLine(lazerOrigin.x, lazerOrigin.y, p.lazerEndPoint.x, p.lazerEndPoint.y, YELLOW);

				lazerOrigin = p.lazerEndPoint;
				reflectVec = reflection(perp(p.objectLineOriginPoint - p.objectLineEndPoint), p.lazerDirection * 1000);

				p = lazerzCollisionDetectionBox(normal(reflectVec), p.lazerEndPoint, p.lastMirrorIndex);

				hardCollisionData = lazerzCollisionDetection(normal(reflectVec), lazerOrigin);
				targetCollisionData = lazerzCollisionDetectionTarget(normal(reflectVec), lazerOrigin);
				if (hardCollisionData.closenessOfCollision < p.closenessOfCollision || targetCollisionData.closenessOfCollision < p.closenessOfCollision)
					break;
				++ii;
			}
		}
		else if (targetCollisionData.closenessOfCollision < hardCollisionData.closenessOfCollision)
		{
			sfw::drawLine(lazerOrigin.x, lazerOrigin.y, targetCollisionData.lazerEndPoint.x, targetCollisionData.lazerEndPoint.y, BLUE);
		}
		else
		{
			sfw::drawLine(lazerOrigin.x, lazerOrigin.y, hardCollisionData.lazerEndPoint.x, hardCollisionData.lazerEndPoint.y, BLUE);
			targetIsInSight = false;
		}

		if (ii != limit)
		{
			if (hardCollisionData.closenessOfCollision < targetCollisionData.closenessOfCollision)
			{
				sfw::drawLine(lazerOrigin.x, lazerOrigin.y, hardCollisionData.lazerEndPoint.x, hardCollisionData.lazerEndPoint.y, YELLOW);
			}
			else
			{
				sfw::drawLine(lazerOrigin.x, lazerOrigin.y, targetCollisionData.lazerEndPoint.x, targetCollisionData.lazerEndPoint.y, BLUE);
				targetIsInSight = true;
			}
		}
	}
	//////////////////////////////////////////////////////////////
	else //if (lazerIsFired)
	{
		Vec2 unit(normal(currLazerDir)), length(normal(currLazerDir) * 50.0);

		if (magnitude(unit * timePassed) < magnitude(currLazerEnd - currLazerOrigin))
		{
			timePassed += sfw::getDeltaTime() * 1000;
			progress = currLazerOrigin + (unit * timePassed);

			if (magnitude((progress + length) - currLazerOrigin) < magnitude(currLazerEnd - currLazerOrigin))
			{
				sfw::drawLine(progress.x, progress.y,
					progress.x + length.x, progress.y + length.y, RED);
			}
			else
			{
				sfw::drawLine(progress.x, progress.y,
					currLazerEnd.x, currLazerEnd.y, RED);

				Vec2 r = reflection(perp(nextLazer.objectLineOriginPoint - nextLazer.objectLineEndPoint), nextLazer.lazerDirection * 1000);
				Vec2 leftOverLineEnd(normal(r) * magnitude((progress + length) - currLazerEnd));

				sfw::drawLine(currLazerEnd.x, currLazerEnd.y,
					currLazerEnd.x + leftOverLineEnd.x, currLazerEnd.y + leftOverLineEnd.y, RED);

				if(lazerIsOnTargetLine)
					m_lazerz_target.m_deathSequenceIsActive = true;
			}
		}
		else
		{
			timePassed = 0.0;
			currLazerOrigin = currLazerEnd;
			currLazerDir = reflection(perp(nextLazer.objectLineOriginPoint - nextLazer.objectLineEndPoint), nextLazer.lazerDirection * 1000);
			nextLazer = lazerzCollisionDetectionBox(normal(currLazerDir), currLazerOrigin, nextLazer.lastMirrorIndex);
			if (nextLazer.lazerEndPoint.x == -INFINITY)
			{
				nextLazer = lazerzCollisionDetectionTarget(currLazerDir, currLazerOrigin);
				lazerIsOnTargetLine = true;
			}
			currLazerEnd = nextLazer.lazerEndPoint;
		}
	}

	m_lazerz_cannon.draw();

	m_bounceOriginAndDirections.clear();

	sfw::drawTexture(mouse_image, sfw::getMouseX(), sfw::getMouseY(), 10, 10);
}

int LAZERZLEVEL::MirrorMouseCollisionDetection()
{
	Box currMirror;
	int hitMirrorIndex = -1;
	Vec2 mousePos = Vec2(sfw::getMouseX(), sfw::getMouseY());

	for (int ii = 0; ii < m_mirrors_size; ++ii)
	{
		currMirror = m_mirrors_transforms[ii].getGlobalTransform() * m_mirrors[ii];

		hitMirrorIndex = ii;
		for (int jj = 0; jj < 4; ++jj)
		{
			Vec2 lineDirection = currMirror.pointsArr[jj] - currMirror.pointsArr[(jj + 1) % 4];
			float dotProduct_0 = dot(perp(lineDirection), mousePos),
				dotProduct_1 = dot(perp(lineDirection), currMirror.pointsArr[jj]);

			if (!(dotProduct_0 >= dotProduct_1))
			{
				hitMirrorIndex = -1;
				break;
			}
		}

		if (hitMirrorIndex >= 0) 
		{
			cout << ii << "\n";
			return hitMirrorIndex;
		}
	}
	return hitMirrorIndex;
}


void LAZERZLEVEL::step()
{
	m_lazerz_cannon.step();

	Vec2 reflectVal = Vec2(1, 1);

	if (sfw::getKey('Q') && indexOfSelectedMirror >= 0)
	{
		m_mirrors_transforms[indexOfSelectedMirror].rotateLocalTransform(0.5);
	}
	else if (sfw::getKey('E') && indexOfSelectedMirror >= 0)
	{
		m_mirrors_transforms[indexOfSelectedMirror].rotateLocalTransform(-0.5);
	}
	
	m_lazerz_target.step();

	if (fire_lazerz_button.mouseUp() && targetIsInSight)
	{
		lazerIsFired = true;
		timePassed = 0.0;
		currLazerOrigin = m_lazerz_cannon.lazerzOrigin();
		currLazerDir = m_lazerz_cannon.lazerzDirection();
		nextLazer = lazerzCollisionDetectionBox(m_lazerz_cannon.lazerzDirection(), m_lazerz_cannon.lazerzOrigin(), -1);
		currLazerEnd = nextLazer.lazerEndPoint;
	}
	fire_lazerz_button.mouseDown();
	if (add_mirror_button.mouseUp())
	{
		if (m_mirrors_size < 10)
		{
			m_mirrors[m_mirrors_size] = Box(0, 0, 20, 200);
			m_mirrors_transforms[m_mirrors_size].m_facing = 0;
			m_mirrors_transforms[m_mirrors_size].m_position = Vec2(500, 500);
			m_mirror_colors[m_mirrors_size] = WHITE;
			++m_mirrors_size;
		}
	}
	add_mirror_button.mouseDown();
	if (del_mirror_button.mouseUp())
	{
		if (m_mirrors_size > 0 && indexOfSelectedMirror != -1)
		{
			for (int ii = indexOfSelectedMirror; ii < m_mirrors_size-1; ++ii)
			{
				cout << "here\n";
				m_mirrors_transforms[ii].m_facing = m_mirrors_transforms[ii + 1].m_facing;
				m_mirrors_transforms[ii].m_position = m_mirrors_transforms[ii + 1].m_position;
			}
			m_mirrors_transforms[m_mirrors_size - 1].m_facing = 0;
			m_mirrors_transforms[m_mirrors_size - 1].m_position = Vec2(0, 0);
			m_mirror_colors[indexOfSelectedMirror] = WHITE;
			--m_mirrors_size;
			indexOfSelectedMirror = -1;
		}
	}
	del_mirror_button.mouseDown();

	if (sfw::getMouseButton(0))
	{
		for (int ii = 0; ii < m_mirrors_size; ++ii)
		{
			if(ii != indexOfSelectedMirror)
				m_mirror_colors[ii] = WHITE;
		}

		if (indexOfSelectedMirror == MirrorMouseCollisionDetection())
			mirrorMoving = true;

		if (indexOfSelectedMirror != -1 && mirrorMoving)
		{
			m_mirrors_transforms[indexOfSelectedMirror].m_position += Vec2(sfw::getMouseX() - currMousePos.x, sfw::getMouseY() - currMousePos.y);
			currMousePos = Vec2(sfw::getMouseX(), sfw::getMouseY());
		}
		else if(!mouseIsDown && MirrorMouseCollisionDetection() != -1)
		{
			indexOfSelectedMirror = MirrorMouseCollisionDetection();
			m_mirror_colors[indexOfSelectedMirror] = YELLOW;
			currMousePos = Vec2(sfw::getMouseX(), sfw::getMouseY());
		}

		mouseIsDown = true;
	}
	else
	{
		if (mouseIsDown) // mouse just came up
		{
			mouseIsDown = false;
			if(MirrorMouseCollisionDetection() == -1 && indexOfSelectedMirror != -1)
			{
				m_mirror_colors[indexOfSelectedMirror] = WHITE;
				indexOfSelectedMirror = -1;
			}

		}
		currMousePos = Vec2(sfw::getMouseX(), sfw::getMouseY());
		mirrorMoving = false;
	}

}

//LAZERZMENU LAZERZLEVEL::next()
//{
//	return m_state;
//}

