#pragma once

#include "States.h"
#include "Button.h"
#include "Shapes.h"
#include "ShapeDraw.h"
#include "Vec2.h"
#include "sfwdraw.h"
#include "Collision.h"

#include <iostream>
using std::cout;

class AABBPlaneGame
{
	AABB m_aabb;
	Plane m_static_plane;
	unsigned m_color, m_font = sfw::loadTextureMap("./res/tonc_font.png", 16, 6),
		m_aabbCollIndicator = sfw::loadTextureMap("./Images/OVERLAPPING_FALSE.png"),
		m_velCollIndicator = sfw::loadTextureMap("./Images/VELOCITY_FALSE.png");
	bool usingVelocity, aabbIsColliding, velIsColliding;

	void rotatePlaneLine(float degrees);
	void drawOutputTextForAABBPlane();
	void drawCollisionLines();
	void drawReflectionLines();

	Button backButton, usingVelButton;
	MENU_STATE state = STAY;
	float currentTime;

	unsigned velocityToggle_button = sfw::loadTextureMap("./Images/VELOCITY_OFF_Button.png");
	unsigned exit_button = sfw::loadTextureMap("./Images/EXIT_Button.png");
	unsigned mouse = sfw::loadTextureMap("./Images/PLAY_Button.png", 10, 10);
	unsigned WASD = sfw::loadTextureMap("./Images/WASD.png");
	unsigned QE = sfw::loadTextureMap("./Images/QE.png");
	unsigned IJKL = sfw::loadTextureMap("./Images/IJKL.png");

	void buttonPressed_Exit();

public:
	AABBPlaneGame();
	void init();
	void draw();
	void step();
	MENU_STATE next();
};

AABBPlaneGame::AABBPlaneGame()
{
	aabbIsColliding = velIsColliding = false;

	m_aabb.m_pos = Vec2(500, 100);
	m_aabb.m_he = Vec2(50, 50);
	m_aabb.m_vel = Vec2(0, 0);

	m_static_plane.m_position = Vec2(500, 420);
	m_static_plane.m_he = Vec2(1000, 1000);
	m_static_plane.m_direction = perp(m_static_plane.m_he);
}

void AABBPlaneGame::buttonPressed_Exit()
{
	usingVelocity = true;

	state = EXIT_MENU;
}

void AABBPlaneGame::init()
{
	state = STAY;
	usingVelocity = false;

	// EXIT Button
	backButton.mouseDepressedOverButton = false;
	backButton.m_pos = Vec2(50, 900);
	backButton.m_dim = Vec2(100, 50);
	backButton.m_text = "EXIT";
	backButton.m_texture = sfw::loadTextureMap("./Images/BACK_Button.png");

	// Using Velocity Button
	usingVelButton.mouseDepressedOverButton = false;
	usingVelButton.m_pos = Vec2(50, 800);
	usingVelButton.m_dim = Vec2(100, 50);
	usingVelButton.m_text = "Velocity";
	usingVelButton.m_texture = sfw::loadTextureMap("./Images/VELOCITY_OFF_Button.png");
}

void AABBPlaneGame::draw()
{
	drawAABB(m_aabb, GREEN);
	drawPlane(m_static_plane, YELLOW);

	// Main AABB draw
	sfw::drawString(m_font, "Main AABB", 425, 980, 20, 20, 0, ' ');
	sfw::drawTexture(WASD, 500, 900, 200, 100);
	sfw::drawTexture(m_aabbCollIndicator, 500, 800, 200, 50);

	sfw::drawString(m_font, "Plane Rotation", 380, 750, 20, 20, 0, ' ');
	sfw::drawTexture(QE, 500, 700, 200, 50);

	// Main AABB velocity draw
	if (usingVelocity)
	{
		AABB  aabb_vel(m_aabb);
		aabb_vel.m_pos += m_aabb.m_vel;
		drawAABB(aabb_vel, 0xAAFFAA);
		sfw::drawString(m_font, "Main Velocity", 675, 980, 20, 20, 0, ' ');
		sfw::drawTexture(IJKL, 800, 900, 200, 100);
		sfw::drawTexture(m_velCollIndicator, 800, 800, 200, 50);
		drawCollisionLines();
		if(!aabbIsColliding)
			drawReflectionLines();
	}

	backButton.draw();
	usingVelButton.draw();
	sfw::drawTexture(exit_button, sfw::getMouseX(), sfw::getMouseY(), 10, 10);
	drawOutputTextForAABBPlane();
}

void AABBPlaneGame::step()
{
	// Back Button
	if (backButton.mouseUp())
	{
		state = EXIT_MENU;
	}
	backButton.mouseDown();

	// Using velocity toggle button
	if (usingVelButton.mouseUp())
	{
		if (usingVelocity)
			usingVelButton.m_texture = sfw::loadTextureMap("./Images/VELOCITY_OFF_Button.png");
		else
			usingVelButton.m_texture = sfw::loadTextureMap("./Images/VELOCITY_ON_Button.png");

		usingVelocity = !usingVelocity;
	}
	usingVelButton.mouseDown();

	// AABB Controls
	if (sfw::getKey('W'))
	{
		m_aabb.m_pos += Vec2(0, 10);
		m_aabb.m_vel.y -= 10;
	}
	else if (sfw::getKey('S'))
	{
		m_aabb.m_pos += Vec2(0, -10);
		m_aabb.m_vel.y += 10;
	}
	if (sfw::getKey('A'))
	{
		m_aabb.m_pos += Vec2(-10, 0);
		m_aabb.m_vel.x += 10;
	}
	else if (sfw::getKey('D'))
	{
		m_aabb.m_pos += Vec2(10, 0);
		m_aabb.m_vel.x -= 10;
	}

	if (usingVelocity)
	{
		// AABB Velocity Controls
		if (sfw::getKey('I'))
		{
			m_aabb.m_vel.y += 10;
		}
		else if (sfw::getKey('K'))
		{
			m_aabb.m_vel.y -= 10;
		}
		if (sfw::getKey('J'))
		{
			m_aabb.m_vel.x -= 10;
		}
		else if (sfw::getKey('L'))
		{
			m_aabb.m_vel.x += 10;
		}
	}

	// AABB Velocity Controls
	if (sfw::getKey('Q'))
	{
		rotatePlaneLine(1);
	}
	else if (sfw::getKey('E'))
	{
		rotatePlaneLine(-1);
	}
}

MENU_STATE AABBPlaneGame::next()
{
	return state;
}

void AABBPlaneGame::rotatePlaneLine(float degrees)
{
	m_static_plane.m_he = (rotateByDegrees(degrees) * Vec3(m_static_plane.m_he.x, m_static_plane.m_he.y, 1)).xy();
	m_static_plane.m_direction = perp(m_static_plane.m_he);
}

void AABBPlaneGame::drawOutputTextForAABBPlane()
{
	CollisionData coll2D = planeAABBCollision(m_static_plane, m_aabb);

	if (coll2D.resultIsCollision() != aabbIsColliding)
	{
		aabbIsColliding = !aabbIsColliding;
		if (coll2D.resultIsCollision())
		{
			m_aabbCollIndicator = sfw::loadTextureMap("./Images/OVERLAPPING_TRUE.png");
		}
		else
		{
			m_aabbCollIndicator = sfw::loadTextureMap("./Images/OVERLAPPING_FALSE.png");
		}
	}

	if (usingVelocity)
	{
		CollisionDataSwept collSwept = planeAABBCollisionSwept(m_static_plane, Vec2(0, 0), m_aabb, m_aabb.m_vel);
		if (collSwept.collides != velIsColliding)
		{
			velIsColliding = !velIsColliding;
			if (collSwept.collides)
			{
				m_velCollIndicator = sfw::loadTextureMap("./Images/VELOCITY_TRUE.png");
			}
			else
			{
				m_velCollIndicator = sfw::loadTextureMap("./Images/VELOCITY_FALSE.png");
			}
		}
	}
}

void AABBPlaneGame::drawCollisionLines()
{
	float aMin = INFINITY, aMax = -INFINITY;

	Vec2 velPerp = perp(m_aabb.m_vel);

	Vec2 minAndMaxIndeces;
	Vec2 vertArr[4];
	vertArr[0] = m_aabb.min();
	vertArr[1] = m_aabb.max();
	vertArr[2] = Vec2(m_aabb.min().x, m_aabb.max().y);
	vertArr[3] = Vec2(m_aabb.max().x, m_aabb.min().y);

	for (int ii = 0; ii < 4; ++ii)
	{
		float dotProduct_0 = dot(velPerp, vertArr[ii]);
		if (aMin > dotProduct_0)
		{
			aMin = dotProduct_0;
			minAndMaxIndeces.x = ii;
		}
		if (aMax < dotProduct_0)
		{
			aMax = dotProduct_0;
			minAndMaxIndeces.y = ii;
		}
	}

	sfw::drawLine(vertArr[int(minAndMaxIndeces.x)].x, vertArr[int(minAndMaxIndeces.x)].y,
		vertArr[int(minAndMaxIndeces.x)].x + m_aabb.m_vel.x,
		vertArr[int(minAndMaxIndeces.x)].y + m_aabb.m_vel.y, GREEN);

	sfw::drawLine(vertArr[int(minAndMaxIndeces.y)].x, vertArr[int(minAndMaxIndeces.y)].y,
		vertArr[int(minAndMaxIndeces.y)].x + m_aabb.m_vel.x,
		vertArr[int(minAndMaxIndeces.y)].y + m_aabb.m_vel.y, GREEN);
}

void AABBPlaneGame::drawReflectionLines()
{
	float aMin = INFINITY, vMin;
	int iMin;

	Vec2 vertArr[4];
	vertArr[0] = m_aabb.min();
	vertArr[1] = m_aabb.max();
	vertArr[2] = Vec2(m_aabb.min().x, m_aabb.max().y);
	vertArr[3] = Vec2(m_aabb.max().x, m_aabb.min().y);

	// Find the distance from the AABB to the plane
	for (int ii = 0; ii < 4; ++ii)
	{
		float dotProduct_0 = dot(m_static_plane.m_direction, vertArr[ii]);
		if (aMin > dotProduct_0)
		{
			aMin = dotProduct_0;
			vMin = dot(m_static_plane.m_direction, vertArr[ii] + m_aabb.m_vel);
			iMin = ii;
		}
	}
	float pPmax = dot(m_static_plane.m_direction, m_static_plane.m_position);


	if (velIsColliding)
	{
		// Find the reflection vector of the velocity off of the Plane
		Vec2 reflectVec = reflection(m_static_plane.m_direction, m_aabb.m_vel);

		// Find the lines of the velocity and the plane
		Vec2 velLine = slopeAndConstOfVectorAndVelocity(vertArr[iMin], m_aabb.m_vel),
			planeLine = slopeAndConstOfVectorAndVelocity(m_static_plane.m_position, m_static_plane.m_he);

		// Find the collision point of the velocity and the plane
		Vec2 collPoint = pointOfCollisionBetweenLines(velLine, planeLine, vertArr[iMin].x);

		// Find the magnitude of the velocity to the collision point
		float magOfAabbToCollPoint = magnitude(collPoint - vertArr[iMin]);

		// Find the magnitude of the collision point to the end of the velocity
		float magOfCollPointToEnd = magnitude(m_aabb.m_vel) - magOfAabbToCollPoint;

		// Multiply the normalized velocity vector by the distance to the Plane
		Vec2 vecToPlane = normal(m_aabb.m_vel) * magOfAabbToCollPoint;

		// Multiply the normalized reflection vector by the remainder of the velocity vector and the distance to the Plane
		Vec2 vecOffOfPlane = normal(reflectVec) * magOfCollPointToEnd;

		cout << velLine.x << ", " << velLine.y << "\n";
		cout << planeLine.x << ", " << planeLine.y << "\n";
		cout << collPoint.x << ", " << collPoint.y << "\n\n";

		sfw::drawCircle(collPoint.x, collPoint.y, 10, 16, BLUE);

		sfw::drawLine(m_aabb.m_pos.x, m_aabb.m_pos.y,
			m_aabb.m_pos.x + vecToPlane.x,
			m_aabb.m_pos.y + vecToPlane.y, RED);

		sfw::drawLine(m_aabb.m_pos.x + vecToPlane.x, m_aabb.m_pos.y + vecToPlane.y,
			m_aabb.m_pos.x + vecToPlane.x + vecOffOfPlane.x,
			m_aabb.m_pos.y + vecToPlane.y + vecOffOfPlane.y, BLUE);

		drawAABB(AABB(m_aabb.m_pos.x + vecToPlane.x + vecOffOfPlane.x,
			m_aabb.m_pos.y + vecToPlane.y + vecOffOfPlane.y, m_aabb.m_he.x, m_aabb.m_he.y), BLUE);
	}
	else
	{
		sfw::drawLine(m_aabb.m_pos.x, m_aabb.m_pos.y,
			m_aabb.m_pos.x + m_aabb.m_vel.x,
			m_aabb.m_pos.y + m_aabb.m_vel.y, RED);
	}
}