#ifndef EXPLOSION_HH
#define EXPLOSION_HH

#include "gameworld.h"
#include "ew/updatable.h"
#include "ew/renderable.h"

#include "glhck/glhck.h"
#include "util/vec2d.h"
#include "textureatlas.h"

#include <string>
#include <vector>

class GameWorld;

class Explosion : public ew::Renderable, public ew::Updatable
{
public:
  static void init();
  static void term();

  Explosion(GameWorld* world, Vec2D const& position);
  ~Explosion();

  static ew::UID const ID;
  ew::UID getEntityId() const { return ID; }

  void render(ew::RenderContext* context);
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
