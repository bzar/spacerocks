#ifndef GAMESTATE_HH
#define GAMESTATE_HH

#include "ew/state.h"
#include "ew/updatephase.h"
#include "ew/collidephase.h"
#include "ew/renderphase.h"
#include "ew/controlphase.h"
#include "gamephase.h"
#include "gameworld.h"
#include "GL/glhck.h"

class GameState : public State
{
public:
  static void init();
  static void term();

  GameState(Engine* engine);
  World* getWorld();
  void process(float const delta);

private:
  GameWorld world;
  GamePhase game;
  UpdatePhase update;
  CollidePhase collide;
  RenderPhase render;
  ControlPhase control;
};
#endif
