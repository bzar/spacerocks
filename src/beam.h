#ifndef BEAM_HH
#define BEAM_HH

#include "gameworld.h"
#include "ew/renderable.h"
#include "ew/updatable.h"
#include "ew/collidable.h"

#include "glhck/glhck.h"
#include "util/vec2d.h"
#include "util/lineshape.h"

#include <string>

class Beam : public ew::Renderable, public ew::Updatable, public ew::Collidable
{
public:
  static void init();
  static void term();

  Beam(GameWorld* world, Vec2D const& basePosition, Vec2D const& positionDelta);
  ~Beam();

  static ew::UID const ID;
  ew::UID getEntityId() const { return ID; }

  void render(ew::RenderContext* context);
  void update(float const delta);
  LineShape const* getShape() const;

  void collide(ew::Collidable const* other);

  void setBasePosition(Vec2D const& basePosition);
  void setPositionDelta(Vec2D const& positionDelta);
  Vec2D getBasePosition() const;
  bool canHit() const;

private:
  static float const RADIUS;
  static float const HIT_RATE;
  static std::string const IMAGE;
  static std::string const TIP_IMAGE;
  static glhckTexture* TEXTURE;
  static glhckTexture* TIP_TEXTURE;

  GameWorld* gameWorld;
  glhckObject* o;
  glhckObject* tip;
  float hitDelay;
  bool recovering;
  LineShape shape;
};

#endif
