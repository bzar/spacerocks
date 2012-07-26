#ifndef LASER_HH
#define LASER_HH

#include "particle.h"
#include "GL/glhck.h"
#include "vec2d.h"

#include <map>
#include <string>

class Laser : public Particle
{
public:
  Laser(float const life, Vec2D const& position, Vec2D const& velocity);
  ~Laser();
  void render();
  void update(float delta);
  bool alive() const;
  
private:
  static std::string const IMAGE;
  static glhckTexture* TEXTURE;
  glhckObject* o;
  float life;
  Vec2D v;
};
#endif
