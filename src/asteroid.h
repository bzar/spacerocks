#ifndef ASTEROID_HH
#define ASTEROID_HH

#include "sprite.h"
#include "GL/glhck.h"
#include "vec2d.h"

#include <string>
#include <vector>

class Asteroid : public Sprite
{
public:
  static void init();

  enum Size { TINY, SMALL, MEDIUM, LARGE, NUM_SIZES };
  Asteroid(World* world, Size const size, Vec2D const& position, Vec2D const& velocity);
  ~Asteroid();

  static int const ID;
  int getEntityId() const { return ID; }

  void render();
  void update(float delta);

  bool alive() const;
  virtual void collide(Sprite const* other);

  Vec2D getPosition() const;
  float getRadius() const;

private:
  static float const RADII[NUM_SIZES];
  static std::string const IMAGES[NUM_SIZES];
  static std::vector<TransformData> TRANSFORM;
  static glhckTexture *TEXTURE;

  glhckObject* o;
  Size size;
  Vec2D v;
  float life;
};
#endif
