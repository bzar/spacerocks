#ifndef GAMESTATE_HH
#define GAMESTATE_HH

#include "state.h"
#include "world.h"
#include "GL/glhck.h"

class GameState : public State
{
public:
  static void init();

  GameState(Engine* engine);

  virtual void input();
  virtual void update(float const delta);
  virtual void render();

private:
  World world;
  glhckObject* background;
  glhckText *gameText;
  glhckText *gameTextShadow;

  glhckText *fpsText;
  unsigned int gameFont;
  unsigned int fpsFont;
};
#endif
