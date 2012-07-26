#ifndef VEC2D_H
#define VEC2D_H

class Vec2D {
public:
  Vec2D(float const& x = 0.0, float const& y = 0.0);
  Vec2D(Vec2D const& other);
    
  Vec2D add(Vec2D const& other) const;
  Vec2D subtract(Vec2D const& other) const;
  Vec2D scale(float const& scalar) const;
  Vec2D neg() const;
  Vec2D& addi(Vec2D const& other);
  Vec2D& subtracti(Vec2D const& other);
  Vec2D& scalei(float const& scalar);
  Vec2D& negi();
  
  Vec2D unit() const;
  Vec2D normal() const;
  Vec2D projection(Vec2D const& other) const;
  Vec2D rotate(float const& amount) const;
  Vec2D rotateAround(Vec2D const& other, float const& amount) const;
  
  Vec2D& uniti();
  Vec2D& normali();
  Vec2D& projectioni(Vec2D const& other);
  Vec2D& rotatei(float const& amount);
  Vec2D& rotateAroundi(Vec2D const& other, float const& amount);
  
  float lengthSquared() const;
  float length() const;
  float dot(Vec2D const& other) const;
  float cross(Vec2D const& other) const;
  float angle() const;
  float rotationTo(Vec2D const& other) const;
  
  Vec2D& assign(Vec2D const& other);

  Vec2D operator+(Vec2D const& other) const;
  Vec2D operator-(Vec2D const& other) const;
  Vec2D operator*(float const& scalar) const;
  Vec2D& operator+=(Vec2D const& other);
  Vec2D& operator-=(Vec2D const& other);
  Vec2D& operator*=(float const& scalar);
  Vec2D& operator=(Vec2D const& other);

  float x;
  float y;
};
#endif