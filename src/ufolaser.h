#ifndef UFOLASER_HH
#define UFOLASER_HH

#include "sprite.h"
#include "GL/glhck.h"
#include "vec2d.h"

#include <map>
#include <string>

class Asteroid;

class UfoLaser : public Sprite
{
public:
  static void init();

  UfoLaser(World* world, float const life, Vec2D const& position, Vec2D const& velocity);
  ~UfoLaser();

  static int const ID;
  int getEntityId() const { return ID; }

  void render();
  void update(float delta);
  bool alive() const;

  virtual void collide(Sprite const* other);

  Vec2D getPosition() const;
  float getRadius() const;

private:
  static std::string const IMAGE;
  static glhckTexture* TEXTURE;
  static int const RADIUS = 3;

  glhckObject* o;
  float life;
  Vec2D v;
};

#endif
