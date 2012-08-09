#ifndef GAMESTATE_HH
#define GAMESTATE_HH

#include "ew/state.h"
#include "gameworld.h"
#include "GL/glhck.h"

class GameState : public State
{
public:
  static void init();
  static void term();

  GameState(Engine* engine);

  virtual void input();
  virtual void update(float const delta);
  virtual void render();

private:
  GameWorld world;
};
#endif
