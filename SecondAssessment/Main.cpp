#include "Collision.h"
#include "Transform.h"
#include "RigidBody.h"
#include "SpaceshipLocomotion.h"
#include "Camera.h"
#include "sfwdraw.h"
#include "Shapes.h"
#include <cmath>
#include <string>
#include <time.h>
#include "Mat3.h"
#include <vector>
#include "Collider.h"
#include "Menu.h"
#include "SplashPage.h"
#include "ApplicationEndPage.h"
#include "NGonGame.h"
#include "AABBGame.h"
#include "AABBPlaneGame.h"
#include "LAZERZLEVEL.h"
#include <iostream>
using std::cout;
unsigned font;
enum Tank_State { ROTATE, MOVE };

int main()
{
	// Create the window
	sfw::initContext(1000, 1000, "Testbed");

	float t = 0, spacer = 0;
	float b = 0.5, m = 0.5, e = 1.0f, tightness = 0.1;
	float m1 = 100, m2 = 100, n1 = 500, n2 = 500;
	bool onV1 = true;

	float grav = 0;
	int timeStep = 0;
	Transform cameraTransform;

	APP_STATE state = MENU;
	SplashPage splash;
	splash.init(font);
	Menu menu;
	menu.init();
	LAZERZLEVEL *lazerz_level_ptr = nullptr;
	AABBPlaneGame *aabb_plane_game_ptr = nullptr;
	AABBGame *aabb_game_ptr = nullptr;
	NGonGame *ngon_game_ptr = nullptr;
	ApplicationEndPage appEnd;

	while (sfw::stepContext())
	{
		switch (state)
		{
		case MENU:
			menu.step();
			menu.draw();
			if (menu.next() == STAY) {}
			else if (menu.next() == EXIT_MENU) { state = EXIT; }
			//else if (menu.next() == PLAY_LAZERZ) { state = ENTER_LAZERZ_GAMEPLAY; }
			else if (menu.next() == PLAY_AABB_PLANE) { state = ENTER_AABB_PLANE_GAMEPLAY; }
			else if (menu.next() == PLAY_AABB) { state = ENTER_AABB_GAMEPLAY; }
			else if (menu.next() == PLAY_NGON) { state = ENTER_NGON_GAMEPLAY; }
			else { return 0; }
			break;
		/*case SHOW_CREDITS:
			appEnd.draw();
			credits.step();
			if (credits.next()) {}
			else { menu.init(); state = MENU; }
			break;
		case ENTER_SPLASH:
			splash.play();
			state = SPLASH;
		case SPLASH:
			splash.step();
			splash.draw();
			state = splash.next(state);
			break;*/
		//case ENTER_LAZERZ_GAMEPLAY:
		//	//ngon_game_ptr = new NGonGame;
		//	lazerz_level_ptr = new LAZERZLEVEL;
		//	lazerz_level_ptr->init();
		//	state = LAZERZ_GAMEPLAY;
		//	break;
		//case LAZERZ_GAMEPLAY:
		//	lazerz_level_ptr->step();
		//	lazerz_level_ptr->draw();
		//	//if (lazerz_level_ptr->next() == STAY) {}
		//	//else { state = MENU; delete aabb_plane_game_ptr; aabb_plane_game_ptr = nullptr; menu.init(); }
		//	break;
		case ENTER_AABB_PLANE_GAMEPLAY:
			//ngon_game_ptr = new NGonGame;
			aabb_plane_game_ptr = new AABBPlaneGame;
			aabb_plane_game_ptr->init();
			state = AABB_PLANE_GAMEPLAY;
			break;
		case AABB_PLANE_GAMEPLAY:
			aabb_plane_game_ptr->step();
			aabb_plane_game_ptr->draw();
			if (aabb_plane_game_ptr->next() == STAY) {}
			else { state = MENU; delete aabb_plane_game_ptr; aabb_plane_game_ptr = nullptr; menu.init(); }
			break;
		case ENTER_AABB_GAMEPLAY:
			aabb_game_ptr = new AABBGame;
			aabb_game_ptr->init();
			state = AABB_GAMEPLAY;
			break;
		case AABB_GAMEPLAY:
			aabb_game_ptr->step();
			aabb_game_ptr->draw();
			if (aabb_game_ptr->next() == STAY) {}
			else { state = MENU; delete aabb_game_ptr; aabb_game_ptr = nullptr; menu.init(); }
			break;
		case ENTER_NGON_GAMEPLAY:
			ngon_game_ptr = new NGonGame;
			ngon_game_ptr->init();
			state = NGON_GAMEPLAY;
			break;
		case NGON_GAMEPLAY:
			ngon_game_ptr->step();
			ngon_game_ptr->draw();
			if (ngon_game_ptr->next() == STAY) {}
			else { state = MENU; delete ngon_game_ptr; ngon_game_ptr = nullptr; menu.init(); }
			break;
		//case GAMEOVER:
		//	appEnd.step();
		//	appEnd.draw();
		//	if (appEnd.next()) {}
		//	else { state = EXIT; }
		//	break;
		case EXIT:
			return 0;
		}
	}
	sfw::termContext();

	return 0;
}