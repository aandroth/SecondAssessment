#pragma once
#include <cmath>
#include "Shapes.h"
#include "Transform.h"
#include "RigidBody.h"


struct CollisionData1D
{
	float m_penetrationDepth;
	float m_collisionNormal; // Will be 1 or -1

	bool resultIsCollision() const;
	float MTV() const;
};

CollisionData1D collisionDetection1D(float Amin, float Amax,
									 float Bmin, float Bmax);


struct CollisionDataSwept1D
{
	float m_entryTime, m_exitTime;
	float m_collisionNormal; // Will be 1 or -1
};

CollisionDataSwept1D collisionDetectionSwept1D(float Amin, float Amax,
											   float Bmin, float Bmax, 
											   float Avel, float Bvel);

struct CollisionData
{
	float m_penetrationDepth;
	Vec2 m_collisionNormal; // Will be 1 or -1

	bool resultIsCollision() const;
	Vec2 MTV() const;
};

struct CollisionDataSwept
{
	float m_entryTime, m_exitTime;
	Vec2 m_collisionNormal;
	bool collides;

	bool resultIsCollision() const;
};

CollisionData aabbCollision(const AABB &A,
							const AABB &B);

CollisionDataSwept aabbCollisionSwept(const AABB &A, Vec2 A_vel,
									  const AABB &B, Vec2 B_vel);

CollisionData planeAABBCollision(const Plane &P,
								  const AABB &B);

CollisionDataSwept planeAABBCollisionSwept(const Plane &P, const Vec2  &pVel,
										   const AABB  &aabb, const Vec2  &bVel);

CollisionData HullCollision(const Hull &Hull_0, const Hull &Hull_1);

CollisionDataSwept HullCollisionSwept(const Hull &Hull_0, const Vec2, const Hull &Hull_1, const Vec2);

Vec2 maxAndMinIndices(const Hull &Hull, const Vec2 velocity);


Vec2 slopeAndConstOfVectorAndVelocity(Vec2 vec, Vec2 vel);

Vec2 pointOfCollisionBetweenLines(Vec2 vec0, Vec2 vec1);