#ifndef HIGHSCORESTATE_H
#define HIGHSCORESTATE_H

#include "ew/state.h"
#include "ew/updatephase.h"
#include "ew/renderphase.h"
#include "ew/controlphase.h"

#include "ew/controllable.h"
#include "ew/updatable.h"
#include "highscoreworld.h"

class HighScoreState : public ew::State
{
public:
  static void init();
  static void term();

  HighScoreState(ew::Engine* engine);

private:
  class Controller : public ew::Controllable
  {
  public:
    Controller(HighScoreState *state);
    void control(ew::ControlContext* context);

  private:
    HighScoreState* state;
  };

  class Timer : public ew::Updatable
  {
  public:
    Timer(HighScoreState *state);
    void update(float const delta);

  private:
    HighScoreState* state;
    float stateTime;
  };

  friend class Controller;
  friend class Timer;

  ew::Engine* engine;
  HighScoreWorld world;
  ew::UpdatePhase update;
  ew::RenderPhase render;
  ew::ControlPhase control;


};

#endif // HighScoreSTATE_H
