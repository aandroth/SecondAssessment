#pragma once

#include <time.h>
#include "sfwdraw.h"
#include "States.h"
#include "Vec2.h"
#include <string>
using std::string;

class Button
{
public:
	Vec2 m_pos, m_dim;
	string m_text;
	unsigned m_texture, m_highlight, m_depressed, m_font;
	bool mouseDepressedOverButton;
	APP_STATE m_state;

	void draw();
	bool mouseHover();
	bool mouseDown();
	bool mouseUp();
	APP_STATE buttonPressed();
};

bool Button::mouseHover()
{
	if (sfw::getMouseX() >= m_pos.x - m_dim.x*.5 && sfw::getMouseX() <= m_pos.x + m_dim.x*.5 &&
		sfw::getMouseY() >= m_pos.y - m_dim.y*.5 && sfw::getMouseY() <= m_pos.y + m_dim.y*.5)
	{
		return true;
	}

	return false;
}

bool Button::mouseDown()
{
	if (mouseHover() && sfw::getMouseButton(0))
	{
		mouseDepressedOverButton = true;
		return true;
	}

	mouseDepressedOverButton = false;
	return false;
}

bool Button::mouseUp()
{
	if (mouseHover() && !sfw::getMouseButton(0) && mouseDepressedOverButton)
	{
		return true;
	}

	return false;
}

APP_STATE Button::buttonPressed()
{
	return m_state;
}

void Button::draw()
{
	sfw::drawTexture(m_texture, m_pos.x, m_pos.y, m_dim.x, m_dim.y);
	//sfw::drawString(m_font, m_text.c_str(), 0, 600, 35, 35, 0, ' ');
}