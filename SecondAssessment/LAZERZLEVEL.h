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

class LAZERZLEVEL
{
	AABB m_aabbArr[20];
	AABB m_target[5];
	unsigned m_size, m_target_size;

	unsigned m_font = sfw::loadTextureMap("./res/tonc_font.png", 16, 6);

public:
	LAZERZLEVEL();
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
	m_size = 4;

	m_target[0] = AABB(500, 500, 20, 20);
	m_target_size = 1;
}

void LAZERZLEVEL::init()
{

}

void LAZERZLEVEL::draw()
{
	for (int ii = 0; ii < m_size; ++ii)
	{
		drawAABB(m_aabbArr[ii], GREEN);
	}

	for (int ii = 0; ii < m_target_size; ++ii)
	{
		drawAABB(m_target[ii], RED);
	}
}

void LAZERZLEVEL::step()
{
}

//LAZERZMENU LAZERZLEVEL::next()
//{
//	return m_state;
//}

