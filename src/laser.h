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
  static void init();

  Laser(World* world, float const life, Vec2D const& position, Vec2D const& velocity);
  ~Laser();

  static int const ID;
  int getEntityId() const { return ID; }

  void render();
  void update(float delta);
  bool alive() const;

  virtual void collide(Sprite const* other);

  Vec2D getPosition() const;
  Vec2D getFront() const;
  Vec2D getBack() const;
  float getRadius() const;

private:
  static std::string const IMAGE;
  static glhckTexture* TEXTURE;
  static int const LENGTH = 8;
  static int const RADIUS = 1;

  glhckObject* o;
  float life;
  Vec2D v;
};

#endif
