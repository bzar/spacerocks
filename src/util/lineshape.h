#ifndef LINESHAPE_HH
#define LINESHAPE_HH

#include "shape.h"
#include "vec2d.h"

class LineShape : public Shape
{
public:
  LineShape(Vec2D const& p1, Vec2D const& p2, float const radius);
  virtual bool collidesWith(CircleShape const* circle) const;
  virtual bool collidesWith(LineShape const* line) const;

  Vec2D p1;
  Vec2D p2;
  float radius;
};
#endif
