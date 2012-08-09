#ifndef HUD_HH
#define HUD_HH

#include "gameworld.h"
#include "ew/renderable.h"

#include "GL/glhck.h"

class Hud : public Renderable
{
public:
  Hud(GameWorld* world);
  ~Hud();

  static UID const ID;
  UID getEntityId() const { return ID; }

  void render();

private:
  GameWorld* gameWorld;

  glhckText *gameText;
  glhckText *fpsText;
  unsigned int gameFont;
  unsigned int fpsFont;
};

#endif
