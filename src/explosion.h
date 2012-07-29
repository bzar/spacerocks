#ifndef EXPLOSION_HH
#define EXPLOSION_HH

#include "sprite.h"
#include "GL/glhck.h"
#include "vec2d.h"

#include <string>
#include <vector>

class Explosion : public Sprite
{
public:
  Explosion(World* world, Vec2D const& position);
  ~Explosion();

  static int const ID;
  int getEntityId() const { return ID; }

  void render();
  void update(float delta);
  bool alive() const;

private:
  static int const NUM_IMAGES = 90;
  static int const FPS = 60;
  static std::string const IMAGES[NUM_IMAGES];
  static std::vector<TransformData> TRANSFORM;
  static glhckTexture *TEXTURE;

  float time;
  glhckObject* o;
};

#endif
