#ifndef ASTEROID_HH
#define ASTEROID_HH

#include "gameworld.h"
#include "ew/renderable.h"
#include "ew/updatable.h"
#include "ew/collidable.h"

#include "glhck/glhck.h"
#include "util/vec2d.h"
#include "util/circleshape.h"
#include "sound.h"

#include <string>
#include <vector>

class Asteroid : public ew::Renderable, public ew::Updatable, public ew::Collidable
{
public:
  static void init();
  static void term();

  static int const NUM_THEMES = 12;

  enum Size { TINY = 0, SMALL, MEDIUM, LARGE, NUM_SIZES };
  Asteroid(GameWorld* world, Size const size, Vec2D const& position, Vec2D const& velocity, int theme = 0);
  ~Asteroid();

  void render(ew::RenderContext* context);
  void update(float const delta);

  CircleShape const* getShape() const;
  bool collidesWith(Shape const* other) const;
  void collide(ew::Collidable const* other);

  Vec2D getPosition() const;
  Vec2D getVelocity() const;
  float getLife() const;
  void die();

private:
  static float const RADII[NUM_SIZES];
  static int const IMAGE_SIZES[NUM_SIZES];
  static int const IMAGE_BLOCK_WIDTH;
  static int const IMAGE_BLOCK_HEIGHT;
  static std::string const IMAGE;
  static glhckTexture* texture;

  GameWorld* gameWorld;
  glhckObject* o;
  Size size;
  Vec2D v;
  float life;
  CircleShape shape;
  int theme;

  static Sound hitSound;
  static Sound destroySoundBig;
  static Sound destroySoundSmall;
  
};
#endif
