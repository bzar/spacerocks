#ifndef UTIL_HH
#define UTIL_HH

#include "vec2d.h"

bool circlesIntersect(Vec2D const& p1, float const r1, Vec2D const& p2, float const r2);
bool circleLineIntersect(Vec2D const& p, float const r, Vec2D const& l1, Vec2D const& l2, float const lr = 0);
float randFloat(float const minValue, float const maxValue);
#endif
