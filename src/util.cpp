#include "util.h"

bool circlesIntersect(Vec2D const& p1, float const r1, Vec2D const& p2, float const r2)
{
  return (p1 - p2).lengthSquared() < (r1 + r2) * (r1 + r2);
}
