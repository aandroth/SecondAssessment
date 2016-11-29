#pragma once

#include "States.h"
#include <time.h>
#include "sfwdraw.h"

class ApplicationEndPage
{
	bool stayOnScreen = true;
	int splashFont;

	unsigned mouse = sfw::loadTextureMap("./Images/PLAY_Button.png", 10, 10);
	unsigned font = sfw::loadTextureMap("./res/tonc_font.png", 16, 6);

	void buttonPressed_Exit();

public:
	void init();
	void draw();
	void step();
	bool next();
};

void ApplicationEndPage::buttonPressed_Exit()
{
	stayOnScreen = false;
}

void ApplicationEndPage::init()
{

	splashFont = font;
}

void ApplicationEndPage::draw()
{
	sfw::drawString(font, "Programming", 100, 550, 50, 50, 0, ' ');
}

void ApplicationEndPage::step()
{
	if (sfw::getMouseButton(0) &&
		(sfw::getMouseX() > 250 && sfw::getMouseX() < 450) &&
		(sfw::getMouseY() > 100 && sfw::getMouseY() < 200))
	{
		buttonPressed_Exit();
	}
}

bool ApplicationEndPage::next()
{
	return stayOnScreen;
}
#pragma once
