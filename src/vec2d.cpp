#include "vec2d.h"
#include <cmath>
float const TAU = 2 * 3.14159265;

Vec2D::Vec2D(float const& x, float const& y) : x(x), y(y)
{
}

Vec2D::Vec2D(Vec2D const& other) : x(other.x), y(other.y)
{
}
  
Vec2D Vec2D::add(Vec2D const& other) const
{
  return Vec2D(x + other.x, y + other.y);
}
Vec2D Vec2D::subtract(Vec2D const& other) const
{
  return Vec2D(x - other.x, y - other.y);
}
Vec2D Vec2D::scale(float const& scalar) const
{
  return Vec2D(x * scalar, y * scalar);
}
Vec2D Vec2D::neg() const
{
  return Vec2D(-x, -y);
}


Vec2D& Vec2D::addi(Vec2D const& other)
{
  x += other.x;
  y += other.y;
  return *this;
}
Vec2D& Vec2D::subtracti(Vec2D const& other)
{
  x -= other.x;
  y -= other.y;  
  return *this;
}
Vec2D& Vec2D::scalei(float const& scalar)
{
  x *= scalar;
  y *= scalar;  
  return *this;  
}
Vec2D& Vec2D::negi()
{
  x = -x;
  y = -y;  
  return *this;  
}

Vec2D Vec2D::unit() const
{
  float l = length();
  if(l > 0)
  {
    return Vec2D(x / l, y / l);
  }
  else
  {
    return Vec2D(0, 0);
  }
}

Vec2D Vec2D::normal() const
{
  return Vec2D(y, -x);
}

Vec2D Vec2D::projection(Vec2D const& other) const
{
  Vec2D direction = other.unit();
  return direction.scalei(dot(direction));
}
Vec2D Vec2D::rotate(float const& amount) const
{
  float sn = sin(amount * TAU);
  float cs = cos(amount * TAU);
  return Vec2D(x * cs - y * sn, x * sn + y * cs);
}

Vec2D Vec2D::rotateAround(Vec2D const& other, float const& amount) const
{
  return subtract(other).rotatei(amount).addi(other);
}

Vec2D& Vec2D::uniti()
{
  float l = length();
  if(l > 0)
  {
    x /= l;
    y /= l;
  }
  return *this;
}
Vec2D& Vec2D::normali()
{
  float temp = x;
  x = y;
  y = -temp;
  return *this;
}
Vec2D& Vec2D::projectioni(Vec2D const& other)
{
  Vec2D direction = other.unit();
  return assign(direction.scalei(dot(direction)));
}
Vec2D& Vec2D::rotatei(float const& amount)
{
  float sn = sin(amount * TAU);
  float cs = cos(amount * TAU);
  float tx = x;
  x = x * cs - y * sn;
  y = tx * sn + y * cs;  
  return *this;
}
Vec2D& Vec2D::rotateAroundi(Vec2D const& other, float const& amount)
{
  return subtracti(other).rotatei(amount).addi(other);
}

float Vec2D::lengthSquared() const
{
  return x * x + y * y;
}
float Vec2D::length() const
{
  return sqrt(lengthSquared());
}
float Vec2D::dot(Vec2D const& other) const
{
  return x * other.x + y * other.y;
}
float Vec2D::cross(Vec2D const& other) const
{
  return x * other.y - y * other.x;
}

float Vec2D::angle() const
{
  if(y >= 0)
  {
    return acos(unit().x) / TAU;
  }
  else
  {
    return -acos(unit().x) / TAU;
  }
}

float Vec2D::rotationTo(Vec2D const& other) const
{
  float myAngle = angle();
  float otherAngle = other.angle();  
  if(myAngle > otherAngle)
  {
    return myAngle - (otherAngle + 1.0);
  }
  else
  {
    return myAngle - otherAngle;
  }
}

Vec2D& Vec2D::assign(Vec2D const& other)
{
  x = other.x;
  y = other.y;
  return *this;
}

Vec2D Vec2D::operator+(Vec2D const& other) const
{
  return add(other);
}
Vec2D Vec2D::operator-(Vec2D const& other) const
{
  return subtract(other);
}
Vec2D Vec2D::operator*(float const& scalar) const
{
  return scale(scalar);
}
Vec2D& Vec2D::operator+=(Vec2D const& other)
{
  return addi(other);
}
Vec2D& Vec2D::operator-=(Vec2D const& other)
{
  return subtracti(other);
}
Vec2D& Vec2D::operator*=(float const& scalar)
{
  return scalei(scalar);
}
Vec2D& Vec2D::operator=(Vec2D const& other)
{
  return assign(other);
}