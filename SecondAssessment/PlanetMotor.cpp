#include "PlanetMotor.h"

void PlanetMotor::updatePlanetMotor(RigidBody & planetBody)
{
	planetBody.addTorque(m_orbitSpeed);
}