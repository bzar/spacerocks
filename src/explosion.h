#ifndef EXPLOSION_HH
#define EXPLOSION_HH

#include "gameworld.h"
#include "updatable.h"
#include "renderable.h"

#include "GL/glhck.h"
#include "vec2d.h"
#include "textureatlas.h"

#include <string>
#include <vector>

class GameWorld;

class Explosion : public Renderable, public Updatable
{
public:
  static void init();
  static void term();

  Explosion(GameWorld* world, Vec2D const& position);
  ~Explosion();

  static Entity::Id const ID;
  Entity::Id getEntityId() const { return ID; }

  void render();
  void update(float const delta);

private:
  static int const NUM_IMAGES = 90;
  static int const FPS = 60;
  static std::vector<std::string> const IMAGES;
  static TextureAtlas atlas;

  glhckObject* o;
  float time;
};

#endif
