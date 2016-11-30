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
	unsigned m_size;

	unsigned m_font = sfw::loadTextureMap("./res/tonc_font.png", 16, 6);

public:
	LAZERZLEVEL();
	void init();
	void draw();
	void step();
};

LAZERZLEVEL::LAZERZLEVEL()
{
	m_aabbArr[0] = AABB(0, 0, 1000, 20);
	m_size = 1;
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
}

void LAZERZLEVEL::step()
{
}

//LAZERZMENU LAZERZLEVEL::next()
//{
//	return m_state;
//}

