#ifndef GAMEOVERSTATE_H
#define GAMEOVERSTATE_H

#include "ew/state.h"
#include "ew/updatephase.h"
#include "ew/renderphase.h"
#include "ew/controlphase.h"

#include "ew/controllable.h"
#include "ew/updatable.h"
#include "gameoverworld.h"

class GameOverState : public ew::State
{
public:
  static void init();
  static void term();

  GameOverState(ew::Engine* engine);

private:
  class Controller : public ew::Controllable
  {
  public:
    Controller(GameOverState *state);
    void control(ew::ControlContext* context);

  private:
    GameOverState* state;
  };

  friend class Controller;

  ew::Engine* engine;
  GameOverWorld world;
  ew::UpdatePhase update;
  ew::RenderPhase render;
  ew::ControlPhase control;
};

#endif // GameOverSTATE_H
