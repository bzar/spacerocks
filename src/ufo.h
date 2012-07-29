#ifndef UFO_HH
#define UFO_HH

#include "sprite.h"
#include "GL/glhck.h"
#include "vec2d.h"

#include <string>
#include <vector>

class Ufo : public Sprite
{
public:
  static void init();

  Ufo(World* world, Vec2D const& position);
  ~Ufo();

  static int const ID;
  int getEntityId() const { return ID; }

  void render();
  void update(float delta);
  bool alive() const;

  virtual void collide(Sprite const* other);

  Vec2D getPosition() const;
  float getRadius() const;

private:
  static int const NUM_IMAGES = 4;
  static int const FPS = 15;
  static int const NUM_FRAMES = 10;
  static int const ANIMATION_FRAMES[NUM_FRAMES];
  static std::string const IMAGES[NUM_IMAGES];
  static std::vector<TransformData> TRANSFORM;
  static glhckTexture *TEXTURE;

  static int const RADIUS = 24;

  float time;
  glhckObject* o;
  float life;
};

#endif
