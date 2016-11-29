#pragma once

//#include "Test.h"
#include "sfwdraw.h"
#include "Mat3.h"
#include "Shapes.h"
#include "ShapeDraw.h"

class Transform
{
public:
	Vec2 m_position;
	Vec2 m_scale;
	float m_facing, moveBy;
	Transform * m_parent;

	Mat3 rotateByMat3 = mat3Identity(), translateByMat3 = mat3Identity();

	Transform(float x = 0, float y = 0, float w = 1.0, float h = 1.0, float a = 0);

	Vec2 getUp() const;
	Vec2 getDirection() const;
	void setDirection(const Vec2 &dir);

	void Transform::rotateLocalTransform(float const &R = 0);
	void Transform::translateLocalTransform(float const &T = 0);
	Mat3 getLocalTransform() const;
	Mat3 getSunTransform() const;
	Mat3 getGlobalTransform() const;
	Vec2 getGlobalPosition() const;

	void debugDraw(const Mat3 &T = mat3Identity());
};

