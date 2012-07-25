#ifndef ASTEROID_HH
#define ASTEROID_HH

#include "sprite.h"
#include "GL/glhck.h"
#include "vec2.h"

#include <map>
#include <string>

class Asteroid : public Sprite
{
public:
  enum Size { TINY = 1, SMALL, MEDIUM, LARGE };
  Asteroid(Size const size, km::vec2 const& position, km::vec2 const& velocity);
  void render();
  void update(float delta);
  
private:
  
  static std::map<Size, std::string> const IMAGES;
  
  glhckObject* o;
  km::vec2 v;
};
#endif
