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

class LAZERZCANNON
{
public:
	unsigned m_color;
	Transform m_transform, m_cannon_transform, m_cannon_pivot_transform;

	AABB m_body, m_cannon;
	Circle m_head, m_cannon_pivot;

	LAZERZCANNON();
	void LAZERZCANNON::buttonPressed_Fire();
	void init();
	void draw();
};

LAZERZCANNON::LAZERZCANNON()
{
	m_transform.m_position = Vec2(200, 200);
	m_transform.m_facing = 0;
	m_transform.m_scale = Vec2(1, 1);
	
	m_body.m_pos = m_transform.m_position;
	m_body.m_he  = Vec2(10, 10);

	m_cannon.m_pos = m_body.m_pos + Vec2(15, 10);
	m_cannon.m_he = Vec2(10, 5);

	m_head.m_pos = m_body.m_pos + Vec2(0, 10);
	m_head.m_radius = 10;

	m_cannon_pivot.m_pos = m_head.m_pos;
	m_cannon_pivot.m_radius = 5;
}

void LAZERZCANNON::buttonPressed_Fire()
{

}

void LAZERZCANNON::init()
{
	state = STAY;

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

void LAZERZCANNON::draw()
{
	drawAABB(m_aabb, GREEN);
	drawAABB(m_static_aabb, YELLOW);

	// Main AABB draw
	sfw::drawString(m_font, "Main AABB", 425, 980, 20, 20, 0, ' ');
	sfw::drawTexture(WASD, 500, 900, 200, 100);
	sfw::drawTexture(m_aabbCollIndicator, 500, 800, 200, 50);

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
	}

	backButton.draw();
	usingVelButton.draw();
	sfw::drawTexture(exit_button, sfw::getMouseX(), sfw::getMouseY(), 10, 10);
	drawOutputTextForAABB();
}

void LAZERZCANNON::step()
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
}

MENU_STATE LAZERZCANNON::next()
{
	return state;
}

void LAZERZCANNON::drawOutputTextForAABB()
{
	CollisionData coll2D = aabbCollision(m_static_aabb, m_aabb);

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
		CollisionDataSwept collSwept = aabbCollisionSwept(m_aabb, m_aabb.m_vel, m_static_aabb, m_static_aabb.m_vel);
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

void LAZERZCANNON::drawCollisionLines()
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