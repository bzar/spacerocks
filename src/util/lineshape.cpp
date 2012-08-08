#include "lineshape.h"
#include "circleshape.h"
#include "util.h"

LineShape::LineShape(Vec2D const& p1, Vec2D const& p2, float const radius) :
  Shape(), p1(p1), p2(p2), radius(radius)
{
}
bool LineShape::collidesWith(CircleShape const* circle) const
{
  return circleLineIntersect(circle->center, circle->radius, p1, p2, radius);
}

bool LineShape::collidesWith(LineShape const* line) const
{
  return lineLineIntersect(p1, p2, radius, line->p1, line->p2, line->radius);
}
