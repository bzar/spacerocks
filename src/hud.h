#ifndef HUD_HH
#define HUD_HH

#include "gameworld.h"
#include "ew/renderable.h"

#include "GL/glhck.h"

class Hud : public ew::Renderable
{
public:
  Hud(GameWorld* world);
  ~Hud();

  static ew::UID const ID;
  ew::UID getEntityId() const { return ID; }

  void render(ew::RenderContext* context);

private:
  GameWorld* gameWorld;

  glhckText *gameText;
  glhckText *fpsText;
  unsigned int gameFont;
  unsigned int fpsFont;
};

#endif
