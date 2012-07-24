#ifndef ASTEROID_HH
#define ASTEROID_HH

#include "sprite.h"
#include "GL/glhck.h"

class Asteroid : public Sprite
{
public:
  Asteroid(int const size, float x, float y);
  void render();
  void update(float delta);
  
private:
  glhckObject* o;
  float angle;
};
#endif