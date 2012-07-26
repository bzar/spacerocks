#ifndef ASTEROID_HH
#define ASTEROID_HH

#include "sprite.h"
#include "GL/glhck.h"
#include "vec2d.h"

#include <string>

class Asteroid : public Sprite
{
public:
  enum Size { TINY, SMALL, MEDIUM, LARGE, NUM_SIZES };
  Asteroid(Size const size, Vec2D const& position, Vec2D const& velocity);
  ~Asteroid();
  
  void render();
  void update(float delta);
  
private:
  
  static std::string const IMAGES[NUM_SIZES];
  
  glhckObject* o;
  Vec2D v;
};
#endif
