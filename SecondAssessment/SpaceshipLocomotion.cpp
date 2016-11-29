#pragma once
#include "SpaceshipLocomotion.h"

SpaceshipLocomotion::SpaceshipLocomotion()
{
	vertThrust = 0.0;
	horizThrust = 0.0;
	speed = 500.0;
	maxThrust = 100.0;

	turn = 0;
	turnSpeed = 50;
	maxTurnSpeed = 30.0;
}

void SpaceshipLocomotion::doThrust()
{
	if (sfw::getKey('W') && vertThrust < maxThrust)
		vertThrust += speed;
	else if (sfw::getKey('S') && vertThrust > -maxThrust)
		vertThrust -= speed;
}

void SpaceshipLocomotion::doTurn()
{
	if (sfw::getKey('A') && horizThrust < maxTurnSpeed)
		horizThrust += turnSpeed;
	else if (sfw::getKey('D') && horizThrust > -maxTurnSpeed)
		horizThrust -= turnSpeed;
}

void SpaceshipLocomotion::doBreakThrust(RigidBody & rig, float deltaTime)
{
	rig.velocity -= rig.velocity*1.5*deltaTime;
}

void SpaceshipLocomotion::doBreakRotate(RigidBody & rig, float deltaTime)
{
	rig.angularVelocity -= rig.angularVelocity*1.5*deltaTime;
}

void SpaceshipLocomotion::update(Transform & trans, RigidBody & rig, float deltaTime)
{
	doThrust();
	doTurn();

	if (sfw::getKey('Q'))
	{
		doBreakThrust(rig, deltaTime);
	}
	if (sfw::getKey('E'))
	{
		doBreakRotate(rig, deltaTime);
	}

	rig.addForce(Vec2(vertThrust*trans.getDirection().x,
		vertThrust*trans.getDirection().y));

	rig.addTorque(horizThrust);

	vertThrust = 0;
	horizThrust = 0;
}