#pragma once

#include "RigidBody.h"
#include "Transform.h"
#include "sfwdraw.h"
#include <iostream>

class SpaceshipLocomotion
{
	float vertThrust, horizThrust;
	float speed, maxThrust;

	float turn, turnSpeed, maxTurnSpeed;

public:
	SpaceshipLocomotion();

	void doThrust();
	void doTurn();
	void doBreakThrust(RigidBody & r, float deltaTime);
	void doBreakRotate(RigidBody & r, float deltaTime);

	void update(Transform & trans, RigidBody & rig, float deltaTime);
};