#ifndef ASTEROID_HH
#define ASTEROID_HH

#include "gameworld.h"
#include "ew/renderable.h"
#include "ew/updatable.h"
#include "ew/collidable.h"

#include "glhck/glhck.h"
#include "util/vec2d.h"
#include "util/circleshape.h"
#include "textureatlas.h"
#include "sound.h"

#include <string>
#include <vector>

class Asteroid : public ew::Renderable, public ew::Updatable, public ew::Collidable
{
public:
  static void init();
  static void term();

  enum Size { TINY = 0, SMALL, MEDIUM, LARGE, NUM_SIZES };
  Asteroid(GameWorld* world, Size const size, Vec2D const& position, Vec2D const& velocity);
  ~Asteroid();

  static ew::UID const ID;
  ew::UID getEntityId() const { return ID; }

  void render(ew::RenderContext* context);
  void update(float const delta);

  CircleShape const* getShape() const;
  void collide(ew::Collidable const* other);

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

  static Sound hitSound;
  static Sound destroySoundBig;
  static Sound destroySoundSmall;
  
};
#endif
