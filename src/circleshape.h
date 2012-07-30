#ifndef CIRCLESHAPE_HH
#define CIRCLESHAPE_HH

#include "shape.h"
#include "vec2d.h"

class CircleShape : public Shape
{
public:
  CircleShape(Vec2D const& center, float const radius);
  virtual bool collidesWith(CircleShape const* circle) const;
  virtual bool collidesWith(LineShape const* line) const;

  Vec2D center;
  float radius;
};
#endif
