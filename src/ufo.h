#ifndef UFO_HH
#define UFO_HH

#include "sprite.h"
#include "GL/glhck.h"
#include "vec2d.h"
#include "circleshape.h"
#include "textureatlas.h"

#include <string>
#include <vector>

class Ufo : public Sprite
{
public:
  static void init();

  Ufo(World* world, Vec2D const& startPosition, Vec2D const& endPosition,
      float freq, float amplitude);
  ~Ufo();

  static int const ID;
  int getEntityId() const { return ID; }

  void render();
  void update(float delta);
  virtual CircleShape const* getShape() const;

  virtual void collide(Sprite const* other);

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
};

#endif
