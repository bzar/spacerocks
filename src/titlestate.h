#ifndef TITLESTATE_H
#define TITLESTATE_H

#include "ew/state.h"
#include "ew/updatephase.h"
#include "ew/renderphase.h"
#include "ew/controlphase.h"

#include "ew/controllable.h"
#include "titleworld.h"

class TitleState : public ew::State
{
public:
  static void init();
  static void term();

  TitleState(ew::Engine* engine);

private:
  class Controller : public ew::Controllable
  {
  public:
    Controller(TitleState *state);
    void control(ew::ControlContext* context);

  private:
    TitleState* state;
  };

  friend class Controller;

  ew::Engine* engine;
  TitleWorld world;
  ew::UpdatePhase update;
  ew::RenderPhase render;
  ew::ControlPhase control;


};

#endif // TITLESTATE_H
