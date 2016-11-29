#pragma once
#include "RigidBody.h"

class PlanetMotor
{
public:
	float m_orbitSpeed;

	void updatePlanetMotor(RigidBody & rig);
};

