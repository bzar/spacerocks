#ifndef BEAM_HH
#define BEAM_HH

#include "sprite.h"
#include "GL/glhck.h"
#include "vec2d.h"
#include "lineshape.h"

#include <string>

class Beam : public Sprite
{
public:
  static void init();

  Beam(World* world, Vec2D const& basePosition, Vec2D const& positionDelta);
  ~Beam();

  static int const ID;
  int getEntityId() const { return ID; }

  void render();
  void update(float delta);
  virtual LineShape const* getShape() const;

  virtual void collide(Sprite const* other);

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
