#ifndef PLASMA_HH
#define PLASMA_HH

#include "gameworld.h"
#include "ew/renderable.h"
#include "ew/updatable.h"
#include "ew/collidable.h"

#include "GL/glhck.h"
#include "util/vec2d.h"
#include "util/circleshape.h"

#include <string>

class Plasma : public Renderable, public Updatable, public Collidable
{
public:
  static void init();
  static void term();

  Plasma(GameWorld* world, float const life, float const power, Vec2D const& position, Vec2D const& velocity);
  ~Plasma();

  static UID const ID;
  UID getEntityId() const { return ID; }

  void render(RenderContext* context);
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
