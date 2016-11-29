#pragma once

#include "Shapes.h"
#include "sfwdraw.h"

void drawCircle(const Circle &C, unsigned color = YELLOW);

void drawAABB(const AABB &aabb, unsigned color = YELLOW);

void drawBox(const Box &BOX, unsigned color = RED);

void drawPlane(const Plane &plane, unsigned color = GREEN);

void drawHull(const Hull &hull, unsigned color = GREEN);