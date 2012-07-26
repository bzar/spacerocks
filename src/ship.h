#ifndef SHIP_HH
#define SHIP_HH

#include "sprite.h"
#include "GL/glhck.h"
#include "vec2d.h"

#include <map>
#include <string>

class Ship : public Sprite
{
public:
  Ship(Vec2D const& position, Vec2D const& velocity);
  void render();
  void update(float delta);
  
  void turningLeft(bool const value);
  void turningRight(bool const value);
  void accelerating(bool const value);
  
private:
  
  static std::string const IMAGE;
  
  glhckObject* o;
  Vec2D v;
  
  bool turnLeft;
  bool turnRight;
  bool accelerate;
};
#endif
