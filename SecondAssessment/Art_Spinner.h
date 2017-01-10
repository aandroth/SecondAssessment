#pragma once

#include "States.h"
#include "Button.h"
#include "Shapes.h"
#include "ShapeDraw.h"
#include "Vec2.h"
#include "sfwdraw.h"
#include <list>
#include <math.h>
using std::iterator;

#include <iostream>
using std::cout;

struct line
{
	Vec2 m_beginPoint, m_direction;
	unsigned int m_color, m_length;
};

class ArtSpiral
{
public:
	std::list<line> m_line_list;
	unsigned int m_color, m_length, m_angle;
	float m_time_delay_count, m_time_delay_limit;
	Vec2 m_pos;
	
	ArtSpiral();
	void init();
	void step();
	void draw();
};

ArtSpiral::ArtSpiral()
{
	m_color = BLUE;
	m_length = 200;
	m_pos = Vec2(500, 500);
	m_time_delay_count = 0;
	m_time_delay_limit = 0.01;
}

void ArtSpiral::init()
{
	//state = STAY;

	line new_line;
	new_line.m_beginPoint = m_pos;
	new_line.m_direction = (rotateByDegrees(m_angle) * Vec3(Vec2(0, 1), 1)).xy();
	new_line.m_color = m_color;
	new_line.m_length = m_length;

	m_line_list.push_back(new_line);
}

void ArtSpiral::draw()
{
	for (auto itr : m_line_list)
	{
		sfw::drawLine(itr.m_beginPoint.x, itr.m_beginPoint.y, 
			itr.m_beginPoint.x + itr.m_direction.x * itr.m_length, 
			itr.m_beginPoint.y + itr.m_direction.y * itr.m_length, itr.m_color);
	}
}

void ArtSpiral::step()
{
	auto itr = m_line_list.begin();
	float time_change = sfw::getDeltaTime();
	while (itr != m_line_list.end())
	{
		itr->m_color -= itr->m_color * time_change * 0.001;
		itr++;
	}

	if (m_line_list.begin()->m_color <= 65500)
	{
		m_line_list.pop_front();
	}
	m_time_delay_count += time_change;
	//if (m_time_delay_count >= m_time_delay_limit)
	//{
	//	m_angle += time_change * 1000;
	//	m_time_delay_count = 0;
	//	line new_line;
	//	new_line.m_beginPoint = m_pos;
	//	new_line.m_direction = (rotateByDegrees(m_angle) * Vec3(Vec2(0,1), 1)).xy();
	//	new_line.m_color = m_color;
	//	new_line.m_length = (sin(m_angle + 1)*40 + 50);
	//	m_line_list.push_back(new_line);
	//}
}
