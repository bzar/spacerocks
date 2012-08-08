#ifndef BEAM_HH
#define BEAM_HH

#include "gameworld.h"
#include "ew/renderable.h"
#include "ew/updatable.h"
#include "ew/collidable.h"

#include "GL/glhck.h"
#include "util/vec2d.h"
#include "util/lineshape.h"

#include <string>

class Beam : public Renderable, public Updatable, public Collidable
{
public:
  static void init();
  static void term();

  Beam(GameWorld* world, Vec2D const& basePosition, Vec2D const& positionDelta);
  ~Beam();

  static Entity::Id const ID;
  Entity::Id getEntityId() const { return ID; }

  void render();
  void update(float const delta);
  LineShape const* getShape() const;

  void collide(Collidable const* other);

  void setBasePosition(Vec2D const& basePosition);
  void setPositionDelta(Vec2D const& positionDelta);
  Vec2D getBasePosition() const;
  bool canHit() const;

private:
  static float const RADIUS;
  static std::string const IMAGE;
  static std::string const TIP_IMAGE;
  static glhckTexture* TEXTURE;
  static glhckTexture* TIP_TEXTURE;

  glhckObject* o;
  glhckObject* tip;
  float hitDelay;
  bool recovering;
  LineShape shape;
};

#endif
