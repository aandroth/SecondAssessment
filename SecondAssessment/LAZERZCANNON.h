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
	Vec2 m_pos;
	Transform m_transform, m_cannon_barrel_transform, m_cannon_pivot_transform;

	Box m_body, m_cannon;
	Circle m_head, m_cannon_pivot;

	Button fireButton;

	LAZERZCANNON();
	void positionCannon(Vec2 newPos);
	void LAZERZCANNON::buttonPressed_Fire();
	Vec2 lazerzOrigin();
	Vec2 lazerzDirection();
	void init();
	void step();
	void draw();
};

LAZERZCANNON::LAZERZCANNON()
{
	m_pos = Vec2(0, 0);

	m_transform.m_position = m_pos;
	m_transform.m_facing = 0;
	m_transform.m_scale = Vec2(1, 1);

	m_body = Box(m_pos.x, m_pos.y, 40, 40);
	m_body.m_pos = m_pos;

	m_head.m_pos = m_pos + Vec2(0, 20);
	m_head.m_radius = 20;

	m_cannon_pivot.m_pos = m_head.m_pos;
	m_cannon_pivot.m_radius = 5;
	m_cannon_pivot_transform.m_position = m_pos;

	m_cannon_barrel_transform = m_cannon_pivot_transform;
	m_cannon_barrel_transform.m_position = Vec2(20, 0);
	m_cannon_barrel_transform.m_parent = &m_cannon_pivot_transform;

	m_cannon = Box(0, 0, 40, 20);
}

Vec2 LAZERZCANNON::lazerzOrigin()
{
	return m_head.m_pos;
}

Vec2 LAZERZCANNON::lazerzDirection()
{
	Vec2 lazerzEnd(2000, 0);
	lazerzEnd = (m_cannon_barrel_transform.getGlobalTransform() * Vec3(lazerzEnd, 1)).xy();
	return normal(lazerzEnd - m_head.m_pos);
}

void LAZERZCANNON::positionCannon(Vec2 newPos)
{
	m_pos = newPos;

	m_transform.m_position = m_pos;

	m_body = Box(m_pos.x, m_pos.y, 40, 40);
	m_body.m_pos = m_pos;

	m_head.m_pos = m_pos + Vec2(0, 20);
	m_head.m_radius = 20;

	m_cannon_pivot.m_pos = m_head.m_pos;
	m_cannon_pivot.m_radius = 5;
	m_cannon_pivot_transform.m_position = m_head.m_pos;

	m_cannon_barrel_transform = m_cannon_pivot_transform;
	m_cannon_barrel_transform.m_position = Vec2(20, 0);
	m_cannon_barrel_transform.m_parent = &m_cannon_pivot_transform;

	m_cannon = Box(0, 0, 40, 20);
}

void LAZERZCANNON::buttonPressed_Fire()
{

}

void LAZERZCANNON::init()
{
	//state = STAY;

	// EXIT Button
	fireButton.mouseDepressedOverButton = false;
	fireButton.m_pos = Vec2(50, 900);
	fireButton.m_dim = Vec2(100, 50);
	fireButton.m_text = "EXIT";
	fireButton.m_texture = sfw::loadTextureMap("./Images/BACK_Button.png");
}

void LAZERZCANNON::draw()
{
	drawBox(m_body, WHITE);
	drawBox(m_cannon_barrel_transform.getGlobalTransform() * m_cannon, WHITE);

	drawCircle(m_head, WHITE);
	drawCircle(m_cannon_pivot, WHITE);
	
	m_cannon_barrel_transform.debugDraw();
	m_cannon_pivot_transform.debugDraw();

	Vec2 lazerzEnd(2000, 0);
	lazerzEnd = (m_cannon_barrel_transform.getGlobalTransform() * Vec3(lazerzEnd, 1)).xy();
	sfw::drawLine(m_head.m_pos.x, m_head.m_pos.y, lazerzEnd.x, lazerzEnd.y, YELLOW);
}

void LAZERZCANNON::step()
{
	//// Back Button
	//if ( .mouseUp())
	//{
	//	state = EXIT_MENU;
	//}
	//backButton.mouseDown();

	// AABB Controls
	if (sfw::getKey('A'))
	{
		m_cannon_pivot_transform.rotateLocalTransform(1);
	}
	else if (sfw::getKey('D'))
	{
		m_cannon_pivot_transform.rotateLocalTransform(-1);
	}
}

//MENU_STATE LAZERZCANNON::next()
//{
//	return state;
//}
