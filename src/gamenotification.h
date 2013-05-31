#ifndef GAMENOTIFICATION_HH
#define GAMENOTIFICATION_HH

#include "ew/updatable.h"
#include "ew/renderable.h"
#include "glhck/glhck.h"
#include "util/vec2d.h"

#include "textureatlas.h"
#include "gameworld.h"

#include <vector>
#include <array>
#include <string>

class GameNotification : public ew::Renderable, public ew::Updatable
{
public:
  static void init();
  static void term();

  GameNotification(GameWorld* world, std::string const& text, float size, float life, Vec2D const& position);
  ~GameNotification();

  void render(ew::RenderContext* context);
  void update(float const delta);

private:
  GameWorld* gameWorld;
  float size;
  float life;
  
  glhckObject* o;  
  float time;
};

#endif

