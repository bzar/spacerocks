#ifndef GAMESTATE_HH
#define GAMESTATE_HH

#include "ew/state.h"
#include "ew/updatephase.h"
#include "ew/collidephase.h"
#include "ew/renderphase.h"
#include "ew/controlphase.h"

#include "gamephase.h"
#include "gameworld.h"
#include "music.h"

class GameState : public ew::State
{
public:
  static void init();
  static void term();

  GameState(ew::Engine* engine);
  
  void enter();
  void exit();

  virtual void process(float const delta);

private:
  GameWorld world;
  GamePhase game;
  ew::UpdatePhase update;
  ew::CollidePhase collide;
  ew::RenderPhase render;
  ew::ControlPhase control;
  
  static Music music;
};
#endif
