#ifndef LASER_HH
#define LASER_HH

#include "gameworld.h"
#include "ew/renderable.h"
#include "ew/updatable.h"
#include "ew/collidable.h"

#include "glhck/glhck.h"
#include "util/vec2d.h"
#include "util/lineshape.h"

#include <string>

class Laser : public ew::Renderable, public ew::Updatable, public ew::Collidable
{
public:
  static void init();
  static void term();

  Laser(GameWorld* world, float const life, Vec2D const& position, Vec2D const& velocity);
  ~Laser();

  void render(ew::RenderContext* context);
  void update(float const delta);
  void collide(ew::Collidable const* other);

  LineShape const* getShape() const;

  Vec2D getPosition() const;

private:
  static std::string const IMAGE;
  static glhckTexture* TEXTURE;
  static int const LENGTH = 8;
  static int const RADIUS = 1;

  GameWorld* gameWorld;
  glhckObject* o;
  glhckObject* oGlow;
  float life;
  float glowPhase;
  Vec2D v;
  LineShape shape;
};

#endif
