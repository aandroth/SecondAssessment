#pragma once
#include "Shapes.h"
#include "ShapeDraw.h"
#include "Vec2.h"
#include "sfwdraw.h"
#include "Transform.h"

#include <iostream>
using std::cout;

class LAZERZTARGET
{
public:
	unsigned m_color;
	Vec2 m_pos, m_dim;
	bool m_deathSequenceIsActive;
	int deathFrameCount = 0, deathFrameCountLimit = 20;

	Box m_body;

	LAZERZTARGET();
	void init();
	void step();
	void draw();
};

LAZERZTARGET::LAZERZTARGET()
{
	m_body = Box(525, 525, 40, 40);
}

void LAZERZTARGET::init()
{
	m_deathSequenceIsActive = false;
	m_color = RED;
	m_pos = Vec2(525, 525);
	m_dim = Vec2(20, 20);
}

void LAZERZTARGET::draw()
{
	if(deathFrameCount < deathFrameCountLimit)
		drawBox(m_body, m_color);
}

void LAZERZTARGET::step()
{
	if (!m_deathSequenceIsActive) {}
	else if(m_deathSequenceIsActive && deathFrameCount < deathFrameCountLimit)
	{
		m_dim += Vec2(2, 2);
		m_body = Box(m_pos.x, m_pos.y, m_dim.x, m_dim.y);
		m_color = YELLOW;
		++deathFrameCount;
	}
}
