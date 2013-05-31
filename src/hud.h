#ifndef HUD_HH
#define HUD_HH

#include "gameworld.h"
#include "ew/renderable.h"

#include "glhck/glhck.h"

class Hud : public ew::Renderable
{
public:
  Hud(GameWorld* world);
  ~Hud();

  void render(ew::RenderContext* context);

private:
  GameWorld* gameWorld;

  glhckText *gameText;
  glhckText *fpsText;
  unsigned int gameFont;
  unsigned int fpsFont;
};

#endif
