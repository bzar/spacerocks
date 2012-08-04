#include "util.h"
#include <cstdlib>
#include <stdexcept>
#include <iostream>

bool circlesIntersect(Vec2D const& p1, float const r1, Vec2D const& p2, float const r2)
{
  return (p1 - p2).lengthSquared() <= (r1 + r2) * (r1 + r2);
}

bool circleLineIntersect(Vec2D const& p, float const r, Vec2D const& l1, Vec2D const& l2, float const lr)
{
  Vec2D ld = l2 - l1;
  Vec2D ln = ld.normal();
  Vec2D l1p = p - l1;
  Vec2D l2p = p - l2;
  if(ln.cross(l1p) * ln.cross(l2p) < 0)
  {
    return l1p.projectioni(ln).lengthSquared() <= (r + lr) * (r + lr);
  }
  else if(l1p.lengthSquared() < l2p.lengthSquared())
  {
    return l1p.lengthSquared() <= (r + lr) * (r + lr);
  }
  else
  {
    return l2p.lengthSquared() <= (r + lr) * (r + lr);
  }
}

bool lineLineIntersect(Vec2D const& l11, Vec2D const& l12, float const l1r,
                       Vec2D const& l21, Vec2D const& l22, float const l2r)
{
  throw std::runtime_error("Not implemented!");
}

Vec2D circleLineIntersectionPoint(Vec2D const& p, float const r, Vec2D const& l1, Vec2D const& l2, float const lr)
{
  Vec2D l1q = (p - l1).projectioni(l2 - l1);
  Vec2D q = l1q - l1;
  float s2 = (lr + r) * (lr + r) - (p - q).lengthSquared();
  float t = 1 - s2 / l1q.lengthSquared();
  t = t < 0 ? 0 : t > 1 ? 1 : t;
  Vec2D x = l1 + l1q.scale(t + 0.01);
  return x;
}

float randInt(int const minValue, int const maxValue)
{
  return rand() % (maxValue - minValue + 1) + minValue;
}

float randFloat(float const minValue, float const maxValue)
{
  int const M = 1024*1024;
  return (rand() % (M + 1)) * (maxValue - minValue) / M + minValue;
}

float lerp(float a, float b, float x)
{
  return (x > 1.0f ? 1.0f : x < 0.0f ? 0.0f : x) * (b - a) + a;
}

