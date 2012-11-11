#ifndef UFOLASER_HH
#define UFOLASER_HH

#include "gameworld.h"
#include "ew/renderable.h"
#include "ew/updatable.h"
#include "ew/collidable.h"

#include "glhck/glhck.h"
#include "util/vec2d.h"
#include "util/lineshape.h"

#include <map>
#include <string>

class Asteroid;

class UfoLaser : public ew::Renderable, public ew::Updatable, public ew::Collidable
{
public:
  static void init();
  static void term();

  UfoLaser(GameWorld* world, float const life, Vec2D const& position, Vec2D const& velocity);
  ~UfoLaser();

  static ew::UID const ID;
  ew::UID getEntityId() const { return ID; }

  void render(ew::RenderContext* context);
  void update(float const delta);
  void collide(ew::Collidable const* other);

  LineShape const* getShape() const;

  Vec2D getPosition() const;
  Vec2D getVelocity() const;

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
