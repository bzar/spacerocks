#ifndef ASTEROID_HH
#define ASTEROID_HH

#include "gameworld.h"
#include "renderable.h"
#include "updatable.h"
#include "collidable.h"

#include "GL/glhck.h"
#include "vec2d.h"
#include "circleshape.h"
#include "textureatlas.h"

#include <string>
#include <vector>

class Asteroid : public Renderable, public Updatable, public Collidable
{
public:
  static void init();
  static void term();

  enum Size { TINY, SMALL, MEDIUM, LARGE, NUM_SIZES };
  Asteroid(GameWorld* world, Size const size, Vec2D const& position, Vec2D const& velocity);
  ~Asteroid();

  static Entity::Id const ID;
  Entity::Id getEntityId() const { return ID; }

  void render();
  void update(float const delta);

  CircleShape const* getShape() const;
  void collide(Collidable const* other);

  Vec2D getPosition() const;
  float getLife() const;
  void die();

private:
  static float const RADII[NUM_SIZES];
  static std::vector<std::string> const IMAGES;
  static float const IMAGE_SIZES[NUM_SIZES];
  static TextureAtlas atlas;

  GameWorld* gameWorld;
  glhckObject* o;
  Size size;
  Vec2D v;
  float life;
  CircleShape shape;
};
#endif
