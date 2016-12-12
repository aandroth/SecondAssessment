#pragma once

#include <time.h>
#include "sfwdraw.h"
#include "States.h"
#include "Button.h"

class Menu
{
	Button lazersButton, aabbPlaneButton, aabbButton, nGonButton, exitButton;
	MENU_STATE state = STAY;
	int splashFont;
	float currentTime;

	unsigned play_button = sfw::loadTextureMap("./Images/PLAY_Button.png");
	unsigned exit_button = sfw::loadTextureMap("./Images/EXIT_Button.png");
	unsigned mouse = sfw::loadTextureMap("./Images/PLAY_Button.png", 10, 10);

	void buttonPressed_Credits();
	void buttonPressed_LAZERZ();
	void buttonPressed_AABB_PLANE();
	void buttonPressed_AABB();
	void buttonPressed_NGON();
	void buttonPressed_Exit();

public:
	void init();
	void play();
	void draw();
	void step();
	MENU_STATE next();
};

void Menu::buttonPressed_Credits()
{
	state = CREDITS;
}

void Menu::buttonPressed_LAZERZ()
{
	state = PLAY_LAZERZ;
}

void Menu::buttonPressed_AABB_PLANE()
{
	state = PLAY_AABB_PLANE;
}

void Menu::buttonPressed_AABB()
{
	state = PLAY_AABB;
}

void Menu::buttonPressed_NGON()
{
	state = PLAY_NGON;
}

void Menu::buttonPressed_Exit()
{
	state = EXIT_MENU;
}

void Menu::init()
{
	state = STAY;
	                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            
	// LAZERZ Button
	lazersButton.mouseDepressedOverButton = false;
	lazersButton.m_pos = Vec2(450, 875);
	lazersButton.m_dim = Vec2(100, 50);
	lazersButton.m_text = "EXIT";
	lazersButton.m_texture = sfw::loadTextureMap("./Images/LAZERZ_Button.png");

	// AABB-Plane Button
	aabbPlaneButton.mouseDepressedOverButton = false;
	aabbPlaneButton.m_pos = Vec2(450, 775);
	aabbPlaneButton.m_dim = Vec2(100, 50);
	aabbPlaneButton.m_text = "EXIT";
	aabbPlaneButton.m_texture = sfw::loadTextureMap("./Images/AABB_Plane_Button.png");
	//nGonButton.m_font = sfw::loadTextureMap("./res/tonc_font.png", 16, 6);

	// AABB Button
	aabbButton.mouseDepressedOverButton = false;
	aabbButton.m_pos = Vec2(450, 575);
	aabbButton.m_dim = Vec2(100, 50);
	aabbButton.m_text = "EXIT";
	aabbButton.m_texture = sfw::loadTextureMap("./Images/AABB_Button.png");
	//nGonButton.m_font = sfw::loadTextureMap("./res/tonc_font.png", 16, 6);

	// N-Gon Button
	nGonButton.mouseDepressedOverButton = false;
	nGonButton.m_pos = Vec2(450, 375);
	nGonButton.m_dim = Vec2(100, 50);
	nGonButton.m_text = "EXIT";
	nGonButton.m_texture = sfw::loadTextureMap("./Images/HULL_Button.png");
	//nGonButton.m_font = sfw::loadTextureMap("./res/tonc_font.png", 16, 6);

	// EXIT Button
	exitButton.mouseDepressedOverButton = false;
	exitButton.m_pos = Vec2(450, 175);
	exitButton.m_dim = Vec2(100, 50);
	exitButton.m_text = "EXIT";
	exitButton.m_texture = sfw::loadTextureMap("./Images/EXIT_Button.png");
	//exitButton.m_font = sfw::loadTextureMap("./res/tonc_font.png", 16, 6);

	state = STAY;
}

void Menu::play()
{
	currentTime = 2.f;
}

void Menu::draw()
{
	lazersButton.draw();
	aabbPlaneButton.draw();
	aabbButton.draw();
	nGonButton.draw();
	exitButton.draw();

	sfw::drawTexture(exit_button, sfw::getMouseX(), sfw::getMouseY(), 10, 10);
}
void Menu::step()
{
	if (exitButton.mouseUp())
	{
		state = EXIT_MENU;
	}
	exitButton.mouseDown();

	if (lazersButton.mouseUp())
	{
		state = PLAY_LAZERZ;
	}
	lazersButton.mouseDown();

	if (aabbPlaneButton.mouseUp())
	{
		state = PLAY_AABB_PLANE;
	}
	aabbPlaneButton.mouseDown();


	if (aabbButton.mouseUp())
	{
		state = PLAY_AABB;
	}
	aabbButton.mouseDown();


	if (nGonButton.mouseUp())
	{
		state = PLAY_NGON;
	}
	nGonButton.mouseDown();
}

MENU_STATE Menu::next()
{
	return state;
}
#pragma once
