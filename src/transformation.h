#ifndef TRANSFORMATION_HH
#define TRANSFORMATION_HH

#include "vec2d.h"
class Transformation
{
public:
  Transformation();
  Transformation(Transformation const& other);
  Transformation& reset();
  Transformation& move(Vec2D const& v);
  Transformation& move(float const& x, float const& y);
  Transformation& rotate(float const& r);
  Transformation& scale(float const& k);
  Transformation& scale(float const& x, float const& y);
  Transformation& apply(Transformation const& other);
  Transformation& apply(float const matrix[]);
  Vec2D transform(Vec2D const& v) const;
  
private:
  static unsigned int const NUM_ROWS = 3;
  static unsigned int const NUM_COLS = 3;
  static unsigned int const NUM_VALUES = NUM_ROWS * NUM_COLS;
  static float const INITIAL_VALUES[NUM_VALUES];
  static float const ZERO_VALUES[NUM_VALUES];

  float values[NUM_VALUES];

  
};
#endif