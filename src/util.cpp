#include "util.h"
#include <cstdlib>

bool circlesIntersect(Vec2D const& p1, float const r1, Vec2D const& p2, float const r2)
{
  return (p1 - p2).lengthSquared() <= (r1 + r2) * (r1 + r2);
}

bool circleLineIntersect(Vec2D const& p, float const r, Vec2D const& l1, Vec2D const& l2)
{
  Vec2D ld = l2 - l1;
  Vec2D ln = ld.normal();
  Vec2D l1p = p - l1;
  Vec2D l2p = p - l2;
  if(ln.cross(l1p) * ln.cross(l2p) < 0)
  {
    return l1p.projectioni(ln).lengthSquared() <= r * r;
  }
  else if(l1p.lengthSquared() < l2p.lengthSquared())
  {
    return l1p.lengthSquared() <= r * r;
  }
  else
  {
    return l2p.lengthSquared() <= r * r;
  }
}

float randFloat(float const minValue, float const maxValue)
{
  int const M = 1024*1024;
  return (rand() % M) * (maxValue - minValue) / M + minValue;
}