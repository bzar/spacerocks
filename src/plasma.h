#ifndef PLASMA_HH
#define PLASMA_HH

#include "sprite.h"
#include "GL/glhck.h"
#include "vec2d.h"
#include "circleshape.h"

#include <string>

class Plasma : public Sprite
{
public:
  static void init();

  Plasma(World* world, float const life, float const power, Vec2D const& position, Vec2D const& velocity);
  ~Plasma();

  static int const ID;
  int getEntityId() const { return ID; }

  void render();
  void update(float delta);
  bool alive() const;
  virtual CircleShape const* getShape() const;

  virtual void collide(Sprite const* other);

  Vec2D getPosition() const;
  float getPower() const;
  float getRadius() const;

private:
  static std::string const IMAGE;
  static glhckTexture* TEXTURE;

  glhckObject* o;
  float power;
  float nextPower;
  float life;
  Vec2D v;
  CircleShape shape;
};

#endif
