#ifndef SHOT_HH
#define SHOT_HH

#include "sprite.h"
#include "GL/glhck.h"
#include "vec2d.h"
#include "circleshape.h"

#include <string>

class Shot : public Sprite
{
public:
  static void init();

  Shot(World* world, float const life, Vec2D const& position, Vec2D const& velocity);
  ~Shot();

  static int const ID;
  int getEntityId() const { return ID; }

  void render();
  void update(float delta);
  virtual CircleShape const* getShape() const;

  virtual void collide(Sprite const* other);

  Vec2D getPosition() const;

private:
  static std::string const IMAGE;
  static glhckTexture* TEXTURE;
  static int const RADIUS = 4;

  glhckObject* o;
  float life;
  Vec2D v;
  CircleShape shape;
};

#endif
