#ifndef TITLESTATE_H
#define TITLESTATE_H

#include "ew/state.h"
#include "ew/updatephase.h"
#include "ew/renderphase.h"
#include "ew/controlphase.h"

#include "ew/controllable.h"
#include "ew/updatable.h"

#include "titleworld.h"
#include "sound.h"

class TitleState : public ew::State
{
public:
  static void init();
  static void term();

  TitleState(ew::Engine* engine);

  void enter();

private:
  class Controller : public ew::Controllable
  {
  public:
    Controller(TitleState *state);
    void control(ew::ControlContext* context);

  private:
    TitleState* state;
  };

  class Timer : public ew::Updatable
  {
  public:
    Timer(TitleState *state);
    void update(float const delta);

  private:
    TitleState* state;
    float stateTime;
  };

  friend class Controller;
  friend class Timer;

  ew::Engine* engine;
  TitleWorld world;
  Sound bgSound;
  ew::UpdatePhase update;
  ew::RenderPhase render;
  ew::ControlPhase control;


};

#endif // TITLESTATE_H
