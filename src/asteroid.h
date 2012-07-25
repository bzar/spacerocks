#ifndef ASTEROID_HH
#define ASTEROID_HH

#include "sprite.h"
#include "GL/glhck.h"
#include "vec2.h"

class Asteroid : public Sprite
{
public:
  Asteroid(int const size, km::vec2 const& position, km::vec2 const& velocity);
  void render();
  void update(float delta);
  
private:
  glhckObject* o;
  float angle;
  km::vec2 v;
};
#endif