//#include "SpaceshipController.h"
//
//SpaceshipController::SpaceshipController(unsigned new_CTRL_LEFT, unsigned new_CTRL_RIGHT,
//	unsigned new_CTRL_FORWARD, unsigned new_CTRL_BACKWARD,
//	unsigned new_CTRL_MOVEMENT_BREAK, unsigned new_CTRL_ROTATION_BREAK)
//{
//	CTRL_LEFT = new_CTRL_LEFT; CTRL_RIGHT = new_CTRL_RIGHT;
//	CTRL_FORWARD = new_CTRL_FORWARD; CTRL_BACKWARD = new_CTRL_BACKWARD;
//	CTRL_MOVEMENT_BREAK = new_CTRL_MOVEMENT_BREAK, CTRL_ROTATION_BREAK = new_CTRL_ROTATION_BREAK;
//}
//
//void SpaceshipController::update(SpaceshipLocomotion &spaceshipLocomotion)
//{
//	float hInput = 0.0f;
//	hInput += sfw::getKey(CTRL_LEFT);
//	hInput -= sfw::getKey(CTRL_RIGHT);
//
//	float vInput = 0.0f;
//	vInput += sfw::getKey(CTRL_FORWARD);
//	vInput -= sfw::getKey(CTRL_BACKWARD);
//
//	float vBreak = 0.0f;
//	vBreak = sfw::getKey(CTRL_MOVEMENT_BREAK);
//
//	float hBreak = 0.0f;
//	hBreak = sfw::getKey(CTRL_ROTATION_BREAK);
//
//	spaceshipLocomotion.doTurn(hInput);
//	spaceshipLocomotion.doThrust(vInput);
//	spaceshipLocomotion.doBreakMovement(vBreak);
//	spaceshipLocomotion.doBreakRotate(hBreak);
//}