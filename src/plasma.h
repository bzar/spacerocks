#ifndef PLASMA_HH
#define PLASMA_HH

#include "gameworld.h"
#include "renderable.h"
#include "updatable.h"
#include "collidable.h"

#include "GL/glhck.h"
#include "vec2d.h"
#include "circleshape.h"

#include <string>

class Plasma : public Renderable, public Updatable, public Collidable
{
public:
  static void init();
  static void term();

  Plasma(GameWorld* world, float const life, float const power, Vec2D const& position, Vec2D const& velocity);
  ~Plasma();

  static Entity::Id const ID;
  Entity::Id getEntityId() const { return ID; }

  void render();
  void update(float const delta);
  void collide(Collidable const* other);

  virtual CircleShape const* getShape() const;

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
