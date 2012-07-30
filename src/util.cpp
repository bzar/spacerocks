#include "util.h"
#include <cstdlib>

bool circlesIntersect(Vec2D const& p1, float const r1, Vec2D const& p2, float const r2)
{
  return (p1 - p2).lengthSquared() < (r1 + r2) * (r1 + r2);
}

float randFloat(float const minValue, float const maxValue)
{
  int const M = 1024*1024;
  return (rand() % M) * (maxValue - minValue) / M + minValue;
}