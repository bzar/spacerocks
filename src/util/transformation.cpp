#include "transformation.h"
#include <cmath>
float const TAU = 2 * 3.14159265;

float const Transformation::INITIAL_VALUES[] = {1, 0, 0, 0, 1, 0, 0, 0, 1};
float const Transformation::ZERO_VALUES[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

namespace
{
  void copyArray(float const* from, float* to, int size)
  {
    for(int i = 0; i < size; ++i) 
    {
      to[i] = from[i];
    }
  }
}

Transformation::Transformation() 
{
  copyArray(INITIAL_VALUES, values, NUM_VALUES);
}

Transformation::Transformation(Transformation const& other)
{
  copyArray(other.values, values, NUM_VALUES);
}

Transformation& Transformation::reset()
{
  copyArray(INITIAL_VALUES, values, NUM_VALUES);
  return *this;
}

Transformation& Transformation::move(Vec2D const& v)
{
  float const matrix[] = { 1.0,  0.0, v.x, 
                           0.0,  1.0, v.y, 
                           0.0,  0.0, 1.0 };
  return apply(matrix);  
}

Transformation& Transformation::move(float const& x, float const& y)
{
  float const matrix[] = { 1.0,  0.0,   x, 
                           0.0,  1.0,   y, 
                           0.0,  0.0, 1.0 };
  return apply(matrix);  
}

Transformation& Transformation::rotate(float const& r)
{
  float const cs = cos(r * TAU);
  float const sn = sin(r * TAU);
  float const matrix[] = {  cs, -sn, 0.0, 
                            sn,  cs, 0.0, 
                           0.0, 0.0, 1.0 };
  return apply(matrix);
}

Transformation& Transformation::scale(float const& k)
{
  float const matrix[] = {   k, 0.0, 0.0, 
                           0.0,   k, 0.0, 
                           0.0, 0.0, 1.0 };
  return apply(matrix);
}

Transformation& Transformation::scale(float const& x, float const& y)
{
  float const matrix[] = {   x, 0.0, 0.0, 
                           0.0,   y, 0.0, 
                           0.0, 0.0, 1.0 };
  return apply(matrix);
}

Transformation& Transformation::apply(Transformation const& other)
{
  return apply(other.values);
}

Transformation& Transformation::apply(float const matrix[])
{
  float old[NUM_VALUES];
  copyArray(values, old, NUM_VALUES);
  copyArray(ZERO_VALUES, values, NUM_VALUES);
  
  for( unsigned int m = 0; m < NUM_ROWS; ++m )
  {
    for( unsigned int n = 0; n < NUM_COLS; ++n )
    {
      for( unsigned int k = 0; k < NUM_ROWS; ++k )
      {
        values[m * NUM_ROWS + n] += matrix[m * NUM_ROWS + k] * old[k * NUM_ROWS + n];
      }
    }
  }

  return *this;
}

Vec2D Transformation::transform(Vec2D const& v) const
{
  return Vec2D(v.x * values[0] + v.y * values[1] + values[2], v.x * values[3] + v.y * values[4] + values[5]);
}
