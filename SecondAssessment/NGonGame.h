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

class NGonGame
{
	Hull m_hull, m_static_hull;
	unsigned m_color, m_font = sfw::loadTextureMap("./res/tonc_font.png", 16, 6),
		m_hullCollIndicator = sfw::loadTextureMap("./Images/OVERLAPPING_FALSE.png"),
		m_velCollIndicator =  sfw::loadTextureMap("./Images/VELOCITY_FALSE.png");
	bool usingVelocity, hullIsColliding, velIsColliding;
	void moveHull(Hull &hull, Vec2 move_vec);
	void drawOutputTextForHull();
	void drawCollisionLines();

	Button backButton, usingVelButton;
	MENU_STATE state = STAY;
	float currentTime;

	unsigned velocityToggle_button = sfw::loadTextureMap("./Images/VELOCITY_OFF_Button.png");
	unsigned exit_button = sfw::loadTextureMap("./Images/EXIT_Button.png");
	unsigned mouse = sfw::loadTextureMap("./Images/PLAY_Button.png", 10, 10);
	unsigned WASD = sfw::loadTextureMap("./Images/WASD.png");
	unsigned IJKL = sfw::loadTextureMap("./Images/IJKL.png");

	void buttonPressed_Exit();

public:
	NGonGame();
	void init();
	void draw();
	void step();
	MENU_STATE next();
};

NGonGame::NGonGame()
{
	hullIsColliding = velIsColliding = false;

	m_hull.m_vertArray[0] = Vec2(480, 120);
	m_hull.m_vertArray[1] = Vec2(490, 100);
	m_hull.m_vertArray[2] = Vec2(510, 100);
	m_hull.m_vertArray[3] = Vec2(520, 120);
	m_hull.m_vertArray[4] = Vec2(500, 130);
	m_hull.m_size = 5;
	m_hull.updateNormals();
	m_hull.m_velocity = Vec2(0, 0);

	m_static_hull.m_vertArray[0] = Vec2(480, 420);
	m_static_hull.m_vertArray[1] = Vec2(490, 400);
	m_static_hull.m_vertArray[2] = Vec2(510, 400);
	m_static_hull.m_vertArray[3] = Vec2(520, 420);
	m_static_hull.m_vertArray[4] = Vec2(500, 430);
	m_static_hull.m_size = 5;
	m_static_hull.updateNormals();
	m_static_hull.m_velocity = Vec2(0, 0);
}

void NGonGame::buttonPressed_Exit()
{
	usingVelocity = true;

	state = EXIT_MENU;
}

void NGonGame::init()
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

void NGonGame::draw()
{
	drawHull(m_hull, GREEN);
	drawHull(m_static_hull, YELLOW);

	// Main Hull draw
	sfw::drawString(m_font, "Main Hull", 425, 980, 20, 20, 0, ' ');
	sfw::drawTexture(WASD, 500, 900, 200, 100);
	sfw::drawTexture(m_hullCollIndicator, 500, 800, 200, 50);

	// Main Hull velocity draw
	if (usingVelocity)
	{
		Hull  hull_vel(m_hull);
		moveHull(hull_vel, m_hull.m_velocity);
		drawHull(hull_vel, 0xAAFFAA);
		sfw::drawString(m_font, "Main Velocity", 675, 980, 20, 20, 0, ' ');
		sfw::drawTexture(IJKL, 800, 900, 200, 100);
		sfw::drawTexture(m_velCollIndicator, 800, 800, 200, 50);
		drawCollisionLines();
	}
	
	backButton.draw();
	usingVelButton.draw();
	sfw::drawTexture(exit_button, sfw::getMouseX(), sfw::getMouseY(), 10, 10);
	drawOutputTextForHull();
}

void NGonGame::step()
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
		if(usingVelocity)
			usingVelButton.m_texture = sfw::loadTextureMap("./Images/VELOCITY_OFF_Button.png");
		else
			usingVelButton.m_texture = sfw::loadTextureMap("./Images/VELOCITY_ON_Button.png");

		usingVelocity = !usingVelocity;
	}
	usingVelButton.mouseDown();

	// Hull Controls
	if (sfw::getKey('W'))
	{
		moveHull(m_hull, Vec2(0, 10));
		m_hull.m_velocity.y -= 10;
	}
	else if (sfw::getKey('S'))
	{
		moveHull(m_hull, Vec2(0, -10));
		m_hull.m_velocity.y += 10;
	}
	if (sfw::getKey('A'))
	{
		moveHull(m_hull, Vec2(-10, 0));
		m_hull.m_velocity.x += 10;
	}
	else if (sfw::getKey('D'))
	{
		moveHull(m_hull, Vec2(10, 0));
		m_hull.m_velocity.x -= 10;
	}

	if (usingVelocity)
	{
		// Hull Velocity Controls
		if (sfw::getKey('I'))
		{
			m_hull.m_velocity.y += 10;
		}
		else if (sfw::getKey('K'))
		{
			m_hull.m_velocity.y -= 10;
		}
		if (sfw::getKey('J'))
		{
			m_hull.m_velocity.x -= 10;
		}
		else if (sfw::getKey('L'))
		{
			m_hull.m_velocity.x += 10;
		}
	}
}

MENU_STATE NGonGame::next()
{
	return state;
}

void NGonGame::moveHull(Hull &h, Vec2 v)
{
	for (int ii = 0; ii < h.m_size; ++ii)
	{
		h.m_vertArray[ii] += v;
	}
}

void NGonGame::drawOutputTextForHull()
{
	CollisionData coll2D = HullCollision(m_static_hull, m_hull);

	if (coll2D.resultIsCollision() != hullIsColliding)
	{
		hullIsColliding = !hullIsColliding;
		if (coll2D.resultIsCollision())
		{
			m_hullCollIndicator = sfw::loadTextureMap("./Images/OVERLAPPING_TRUE.png");
		}
		else
		{
			m_hullCollIndicator = sfw::loadTextureMap("./Images/OVERLAPPING_FALSE.png");
		}
	}

	if (usingVelocity)
	{
		CollisionDataSwept collSwept = HullCollisionSwept(m_hull, m_hull.m_velocity, m_static_hull, m_static_hull.m_velocity);
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

void NGonGame::drawCollisionLines()
{
	Vec2 indeces = maxAndMinIndices(m_hull, m_hull.m_velocity);

	sfw::drawLine(m_hull.m_vertArray[int(indeces.x)].x, m_hull.m_vertArray[int(indeces.x)].y,
		m_hull.m_vertArray[int(indeces.x)].x + m_hull.m_velocity.x,
		m_hull.m_vertArray[int(indeces.x)].y + m_hull.m_velocity.y, GREEN);

	sfw::drawLine(m_hull.m_vertArray[int(indeces.y)].x, m_hull.m_vertArray[int(indeces.y)].y,
		m_hull.m_vertArray[int(indeces.y)].x + m_hull.m_velocity.x,
		m_hull.m_vertArray[int(indeces.y)].y + m_hull.m_velocity.y, GREEN);
}