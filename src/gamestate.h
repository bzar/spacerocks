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
  World* getWorld();

private:
  GameWorld world;
};
#endif
