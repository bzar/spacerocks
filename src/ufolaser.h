#ifndef UFOLASER_HH
#define UFOLASER_HH

#include "sprite.h"
#include "GL/glhck.h"
#include "vec2d.h"
#include "lineshape.h"

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
  virtual LineShape const* getShape() const;

  virtual void collide(Sprite const* other);

  Vec2D getPosition() const;

private:
  static std::string const IMAGE;
  static glhckTexture* TEXTURE;
  static int const LENGTH = 8;
  static int const RADIUS = 1;

  glhckObject* o;
  float life;
  Vec2D v;
  LineShape shape;
};

#endif
