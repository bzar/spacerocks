#ifndef ASTEROID_HH
#define ASTEROID_HH

#include "sprite.h"
#include "GL/glhck.h"
#include "vec2d.h"

#include <map>
#include <string>

class Asteroid : public Sprite
{
public:
  enum Size { TINY = 1, SMALL, MEDIUM, LARGE };
  Asteroid(Size const size, Vec2D const& position, Vec2D const& velocity);
  ~Asteroid();
  
  void render();
  void update(float delta);
  
private:
  
  static std::map<Size, std::string> const IMAGES;
  
  glhckObject* o;
  Vec2D v;
};
#endif
