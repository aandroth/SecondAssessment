#include "RigidBody.h"

RigidBody::RigidBody()
{
	mass = 1.0;
	torque = 0;
	force = Vec2(0, 0);
	impulse = Vec2(0, 0);
	velocity = Vec2(0, 0);
	accelleration = Vec2(0, 0);
	angularVelocity = 0;
	angularAccelleration = 0;
}

void RigidBody::addForce(const Vec2 & a_force)
{
	force += a_force;
}
void RigidBody::addImpulse(const Vec2 & an_impulse)
{
	impulse += an_impulse;
}

void RigidBody::addTorque(float a_torque)
{
	torque += a_torque;
}

void RigidBody::integrate(Transform &trans, const float deltaTime)
{
	accelleration = force / mass;
	velocity += accelleration * deltaTime + impulse/mass;
	trans.m_position += velocity * deltaTime;
	debugDraw(trans);
	force = impulse = { 0, 0 };

	angularAccelleration = torque / mass;
	angularVelocity += angularAccelleration * deltaTime;
	trans.m_facing += angularVelocity * deltaTime;
	torque = 0;
}

void RigidBody::setAccelleration(Vec2 newAcc)
{
	accelleration = newAcc;
}

Vec2 RigidBody::getAccelleration() const
{
	return accelleration;
}

void RigidBody::setVelocity(Vec2 newVel)
{
	velocity = newVel;
}

Vec2 RigidBody::getVelocity() const
{
	return velocity;
}

void RigidBody::debugDraw(Transform trans)
{
	sfw::drawLine(trans.m_position.x, trans.m_position.y,
		trans.m_position.x+velocity.x, trans.m_position.y+velocity.y, CYAN);

	sfw::drawLine(trans.m_position.x + velocity.x, trans.m_position.y + velocity.y,
		trans.m_position.x + velocity.x+force.x, trans.m_position.y + velocity.y+force.y, MAGENTA);
}