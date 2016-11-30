#include "Collision.h"
#include <iostream>
using std::cout;

bool CollisionData1D::resultIsCollision() const
{
	return (m_penetrationDepth >= 0);
}

float CollisionData1D::MTV() const
{
	return m_penetrationDepth * m_collisionNormal;
}


CollisionData1D collisionDetection1D(float Amin, float Amax, 
									 float Bmin, float Bmax)
{
	CollisionData1D collisionDataDetection;

	float tl = Bmax - Amin, 
		  tr = Amax - Bmin;

	collisionDataDetection.m_penetrationDepth = fmin(tl, tr);

	collisionDataDetection.m_collisionNormal = copysignf(1, tl - tr);

	return collisionDataDetection;
}

CollisionDataSwept1D collisionDetectionSwept1D(float Amin, float Amax,
											   float Bmin, float Bmax, 
											   float Avel, float Bvel)
{
	CollisionDataSwept1D collisionDataDetectionSwept;

	float denom_0 = Bvel - Avel,
		denom_1 = Avel - Bvel;

	float tl = (Amin - Bmax) / denom_0,
		  tr = (Bmin - Amax) /denom_1;

	collisionDataDetectionSwept.m_entryTime =  fmin(tl, tr);
	collisionDataDetectionSwept.m_exitTime  =  fmax(tl, tr);

	if (denom_0 == 0)
	{
		collisionDataDetectionSwept.m_entryTime = 0;
		collisionDataDetectionSwept.m_exitTime = INFINITY;
	}

	collisionDataDetectionSwept.m_collisionNormal = copysignf(1, tl - tr);

	return collisionDataDetectionSwept;
}

bool CollisionData::resultIsCollision() const
{
	return (m_penetrationDepth >= 0);
}

Vec2 CollisionData::MTV() const
{
	return m_collisionNormal * m_penetrationDepth;
}

bool CollisionDataSwept::resultIsCollision() const
{
	return m_entryTime >= 0 && m_entryTime <= 1;
}

CollisionData aabbCollision(const AABB &A,
	const AABB &B)
{
	CollisionData retVal;

	CollisionData1D collisionX = collisionDetection1D(A.min().x, A.max().x, B.min().x, B.max().x);
	CollisionData1D	collisionY = collisionDetection1D(A.min().y, A.max().y, B.min().y, B.max().y);

	if (collisionX.m_penetrationDepth < collisionY.m_penetrationDepth)
	{
		retVal.m_penetrationDepth = collisionX.m_penetrationDepth;
		retVal.m_collisionNormal = Vec2(1, 0) * collisionX.m_collisionNormal;
	}
	else// collisionX.m_penetrationDepth < collisionY.m_penetrationDepth
	{
		retVal.m_penetrationDepth = collisionY.m_penetrationDepth;
		retVal.m_collisionNormal = Vec2(0, 1) * collisionY.m_collisionNormal;
	}

	return retVal;
}

CollisionDataSwept aabbCollisionSwept(const AABB &A, Vec2 A_vel,
									  const AABB &B, Vec2 B_vel)
{
	CollisionDataSwept retVal;

	// In case the objects have no velocity
	CollisionData1D collisionX = collisionDetection1D(A.min().x, A.max().x, B.min().x, B.max().x),
					collisionY = collisionDetection1D(A.min().y, A.max().y, B.min().y, B.max().y);

	// Swept results
	CollisionDataSwept1D collisionX_swept = collisionDetectionSwept1D(A.min().x, A.max().x, B.min().x, B.max().x, A_vel.x, B_vel.x),
						 collisionY_swept = collisionDetectionSwept1D(A.min().y, A.max().y, B.min().y, B.max().y, A_vel.y, B_vel.y);

	if (collisionX_swept.m_entryTime == 0 && !collisionX.resultIsCollision())
		collisionX_swept.m_entryTime = INFINITY;
	if (collisionY_swept.m_entryTime == 0 && !collisionY.resultIsCollision())
		collisionY_swept.m_entryTime = INFINITY;


	if (collisionY_swept.m_entryTime >= collisionX_swept.m_entryTime)
	{
		retVal.m_collisionNormal = Vec2(0, 1) * collisionY_swept.m_collisionNormal;

		retVal.m_entryTime = collisionY_swept.m_entryTime;
	}
	else // collisionY_swept.m_entryTime < collisionX_swept.m_entryTime
	{
		retVal.m_collisionNormal = Vec2(1, 0) * collisionX_swept.m_collisionNormal;

		retVal.m_entryTime = collisionX_swept.m_entryTime;
	}

	if (collisionY_swept.m_exitTime < collisionX_swept.m_exitTime)
	{
		retVal.m_exitTime = collisionY_swept.m_exitTime;
	}
	else // collisionY_swept.m_exitTime >= collisionX_swept.m_exitTime
	{
		retVal.m_exitTime = collisionX_swept.m_exitTime;
	}

	//cout << "retVal.m_entryTime: " << retVal.m_entryTime << "\n";
	//cout << "retVal.m_exitTime: " << retVal.m_exitTime << "\n\n";

	if (retVal.m_entryTime <= retVal.m_exitTime && retVal.m_entryTime >= 0 && retVal.m_entryTime <= 1)
		retVal.collides = true;
	else if (collisionX.resultIsCollision() && collisionY.resultIsCollision())
		retVal.collides = true;
	else
		retVal.collides = false;

	return retVal;
}


CollisionData HullCollision(const Hull &Hull_0, const Hull &Hull_1)
{
	CollisionData bestColl;
	bestColl.m_penetrationDepth = INFINITY;

	Vec2 allNormalsArr[32];

	// Put all normals from both hulls into one array
	for (int ii = 0; ii < Hull_0.m_size; ++ii)
	{
		allNormalsArr[ii] = Hull_0.m_normals[ii];
	}	
	for (int ii = Hull_0.m_size, jj = 0; jj < Hull_1.m_size; ++ii, ++jj)
	{
		allNormalsArr[ii] = Hull_1.m_normals[jj];
	}

	// Go through each normal, to find the axis of collision
	for (int ii = 0; ii < Hull_0.m_size + Hull_1.m_size; ++ii)
	{
		float aMin = INFINITY, aMax = -INFINITY, 
			  bMin = INFINITY, bMax = -INFINITY; 

		for (int jj = 0; jj < Hull_0.m_size; ++jj)
		{
			float dotProduct_0 = dot(allNormalsArr[ii], Hull_0.m_vertArray[jj]);
			aMin > dotProduct_0 ? aMin = dotProduct_0 : aMin;
			aMax < dotProduct_0 ? aMax = dotProduct_0 : aMax;
		}
		for (int jj = 0; jj < Hull_1.m_size; ++jj)
		{
			float dotProduct_1 = dot(allNormalsArr[ii], Hull_1.m_vertArray[jj]);
			bMin > dotProduct_1 ? bMin = dotProduct_1 : bMin;
			bMax < dotProduct_1 ? bMax = dotProduct_1 : bMax;
		}

		float pDr, pDl, pD, Dir;
		pDr = aMax - bMin;
		pDl = bMax - aMin;

		pD = fminf(pDr, pDl);

		Dir = copysignf(1, pDl - pDr);

		if (pD < bestColl.m_penetrationDepth)
		{
			bestColl.m_penetrationDepth = pD;
			bestColl.m_collisionNormal = allNormalsArr[ii] * Dir;
		}
	}

	return bestColl;
}

CollisionDataSwept HullCollisionSwept(const Hull &Hull_0, const Vec2 Vel_0, const Hull &Hull_1, const Vec2 Vel_1)
{
	CollisionDataSwept bestColl;
	bestColl.m_entryTime = -INFINITY;
	bestColl.m_exitTime = INFINITY;

	Vec2 allNormalsArr[32];

	for (int ii = 0; ii < Hull_0.m_size; ++ii)
	{
		allNormalsArr[ii] = Hull_0.m_normals[ii];
	}
	for (int ii = Hull_0.m_size, jj = 0; jj < Hull_1.m_size; ++ii, ++jj)
	{
		allNormalsArr[ii] = Hull_1.m_normals[jj];
	}

	for (int ii = 0; ii < Hull_0.m_size + Hull_1.m_size; ++ii)
	{
		float aMin = INFINITY, aMax = -INFINITY,
			  bMin = INFINITY, bMax = -INFINITY;

		for (int jj = 0; jj < Hull_0.m_size; ++jj)
		{
			float dotProduct_0 = dot(allNormalsArr[ii], Hull_0.m_vertArray[jj]);
			aMin > dotProduct_0 ? aMin = dotProduct_0 : aMin;
			aMax < dotProduct_0 ? aMax = dotProduct_0 : aMax;
		}
		for (int jj = 0; jj < Hull_1.m_size; ++jj)
		{
			float dotProduct_1 = dot(allNormalsArr[ii], Hull_1.m_vertArray[jj]);
			bMin > dotProduct_1 ? bMin = dotProduct_1 : bMin;
			bMax < dotProduct_1 ? bMax = dotProduct_1 : bMax;
		}
		
		// In case the objects have no velocity
		CollisionData1D collDis = collisionDetection1D(aMin, aMax, bMin, bMax);

		float aVel = dot(allNormalsArr[ii], Vel_0);
		float bVel = dot(allNormalsArr[ii], Vel_1);

		// Swept results
		CollisionDataSwept1D collSwept = collisionDetectionSwept1D(aMin, aMax, bMin, bMax, aVel, bVel);


		float pDr, pDl, pEntry, pExit, Dir;
		pDl = (bMax - aMin) / (bVel - aVel);
		pDr = (aMax - bMin) / (aVel - bVel);

		pEntry = fmin(pDr, pDl);
		pExit  = fmax(pDr, pDl);

		Dir = copysignf(1, pDl - pDr);

		//cout << "pEntry: " << pEntry << "\n";
		//cout << "pExit: " << pExit << "\n\n";

		if (collSwept.m_entryTime == 0 && !collDis.resultIsCollision())
			collSwept.m_entryTime = INFINITY;

		if (bestColl.m_entryTime < collSwept.m_entryTime)
		{
			bestColl.m_collisionNormal =  allNormalsArr[ii] * collSwept.m_collisionNormal;
			bestColl.m_entryTime = collSwept.m_entryTime;
		}
		if (bestColl.m_exitTime > collSwept.m_exitTime)
		{
			bestColl.m_exitTime = collSwept.m_exitTime;
		}
	}

	//cout << bestColl.m_entryTime << ", " << bestColl.m_exitTime << "\n\n";
	if (bestColl.m_entryTime <= bestColl.m_exitTime && bestColl.m_entryTime >= 0 && bestColl.m_entryTime <= 1)
		bestColl.collides = true;
	else
		bestColl.collides = false;

	return bestColl;
}

CollisionData planeAABBCollision(const Plane &P,
	const AABB  &aabb)
{
	float pTL = dot(P.m_direction, Vec2(aabb.min().x, aabb.max().y));
	float pBR = dot(P.m_direction, Vec2(aabb.max().x, aabb.min().y));
	float pTR = dot(P.m_direction, aabb.min());
	float pBL = dot(P.m_direction, aabb.max());

	float pBmin = fminf(fminf(pTR, pTL), fminf(pBR, pBL));
	float pPmax = dot(P.m_direction, P.m_position);

	CollisionData retVal;

	retVal.m_penetrationDepth = pPmax - pBmin;
	retVal.m_collisionNormal = P.m_direction;

	return retVal;
}

CollisionDataSwept planeAABBCollisionSwept(const Plane &P, const Vec2  &pVel,
											const AABB  &aabb, const Vec2  &bVel)
{
	float pTL = dot(P.m_direction, Vec2(aabb.min().x, aabb.max().y));
	float pBR = dot(P.m_direction, Vec2(aabb.max().x, aabb.min().y));
	float pTR = dot(P.m_direction, aabb.min());
	float pBL = dot(P.m_direction, aabb.max());

	float pBmin = fminf(fminf(pTR, pTL), fminf(pBR, pBL));
	float pBmax = fmaxf(fmaxf(pTR, pTL), fmaxf(pBR, pBL));

	float pBvel = dot(P.m_direction, bVel);
	float pPvel = dot(P.m_direction, pVel);

	float pPmax = dot(P.m_direction, P.m_position);

	CollisionDataSwept retVal;

	retVal.m_entryTime = (pPmax - (pBmin + (pBvel - pPvel)));
	retVal.m_exitTime = (pPmax - pBmin);
	//retVal.m_entryTime = (pBmin - pPmax) / (pPvel - pBvel);
	//retVal.m_exitTime  = (pBmax - pPmax) / (pPvel - pBvel);
	retVal.m_collisionNormal = P.m_direction;

	if (retVal.m_entryTime >= 0 || retVal.m_exitTime >= 0)
		retVal.collides = true;
	else
		retVal.collides = false;

	return retVal;
}

Vec2 maxAndMinIndices(const Hull &Hull, const Vec2 velocity)
{
	Vec2 retVal;

	float aMin = INFINITY, aMax = -INFINITY;
	retVal.x = 0;
	retVal.y = 0;

	Vec2 velPerp = perp(velocity);

	for (int ii = 0; ii < Hull.m_size; ++ii)
	{
		float dotProduct_0 = dot(velPerp, Hull.m_vertArray[ii]);
		if (aMin > dotProduct_0)
		{
			aMin = dotProduct_0;
			retVal.x = ii;
		}
		if (aMax < dotProduct_0)
		{
			aMax = dotProduct_0;
			retVal.y = ii;
		}
	}

	return retVal;
}

Vec2 slopeAndConstOfVectorAndVelocity(Vec2 vec, Vec2 vel)
{
	Vec2 retVal;

	Vec2 diff = vel - vec;

	if (vel.x == 0)
	{
		retVal.x = -INFINITY;
	}
	else
	{
		retVal.x = vel.y / vel.x;
	}
	retVal.y = vec.y - retVal.x * vec.x;

	return retVal;
}

Vec2 pointOfCollisionBetweenLines(Vec2 vec0, Vec2 vec1, float xPosInCaseOfUndefinedSlope)
{
	Vec2 retVal;

	// The slope of the velocity is undefined
	if (vec0.x == -INFINITY)
	{
		retVal.x = xPosInCaseOfUndefinedSlope;
		retVal.y = (vec1.x * retVal.x) + vec1.y;
	}
	else
	{
		retVal.x = (vec1.y - vec0.y) / (vec0.x - vec1.x);
		retVal.y = (vec0.x * retVal.x) + vec0.y;
	}

	return retVal;
}