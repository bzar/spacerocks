#ifndef LASER_HH
#define LASER_HH

#include "sprite.h"
#include "GL/glhck.h"
#include "vec2d.h"

#include <map>
#include <string>

class Asteroid;

class Laser : public Sprite
{
public:
  Laser(float const life, Vec2D const& position, Vec2D const& velocity);
  ~Laser();

  static int const ID;
  int getEntityId() const { return ID; }

  void render();
  void update(float delta);
  bool alive() const;

  virtual void collide(Sprite const* other);

private:
  static std::string const IMAGE;
  static glhckTexture* TEXTURE;
  glhckObject* o;
  float life;
  Vec2D v;
};

#endif
