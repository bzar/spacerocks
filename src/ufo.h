#ifndef UFO_HH
#define UFO_HH

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

class Ufo : public ew::Renderable, public ew::Updatable, public ew::Collidable
{
public:
  static void init();
  static void term();

  Ufo(GameWorld* world, Vec2D const& startPosition, Vec2D const& endPosition,
      float freq, float amplitude);
  ~Ufo();

  void render(ew::RenderContext* context);
  void update(float const delta);
  void collide(ew::Collidable const* other);

  CircleShape const* getShape() const;

  Vec2D getPosition() const;
  float getLife() const;

private:
  static int const NUM_IMAGES = 4;
  static int const FPS = 15;
  static int const NUM_FRAMES = 10;
  static int const ANIMATION_FRAMES[NUM_FRAMES];
  static std::vector<std::string> const IMAGES;
  static TextureAtlas atlas;
  static int const RADIUS = 16;

  GameWorld* gameWorld;
  glhckObject* o;

  Vec2D startPosition;
  Vec2D endPosition;
  float freq;
  float amplitude;
  float duration;
  float accuracy;
  float shootInterval;
  float time;
  float life;
  CircleShape shape;
  
  static Sound shootSound;
  static Sound hitSound;
  static Sound destroySound;
};

#endif
