#pragma once

#include "Vec2.h"
#include <vector>
using std::vector;
#include "PlayerTransform.h"
#include "sfwdraw.h"

class SpaceshipRenderer
{
public:
	unsigned m_color;
	vector<Vec2> pointsVector;

	SpaceshipRenderer();
	void draw(const Mat3 & T, const Transform & ship);
};