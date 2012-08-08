#ifndef BEAM_HH
#define BEAM_HH

#include "gameworld.h"
#include "renderable.h"
#include "updatable.h"
#include "collidable.h"

#include "GL/glhck.h"
#include "vec2d.h"
#include "lineshape.h"

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
