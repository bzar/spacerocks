#ifndef EXPLOSION_HH
#define EXPLOSION_HH

#include "sprite.h"
#include "GL/glhck.h"
#include "vec2d.h"
#include "textureatlas.h"

#include <string>
#include <vector>

class Explosion : public Sprite
{
public:
  static void init();

  Explosion(World* world, Vec2D const& position);
  ~Explosion();

  static int const ID;
  int getEntityId() const { return ID; }

  void render();
  void update(float delta);

private:
  static int const NUM_IMAGES = 90;
  static int const FPS = 60;
  static std::vector<std::string> const IMAGES;
  static TextureAtlas atlas;

  float time;
  glhckObject* o;
};

#endif
