#include "titlestate.h"
#include "GL/glfw3.h"
#include "states.h"

TitleState::TitleState(ew::Engine *engine) :
  ew::State(engine, &world),
  engine(engine),
  world(engine),
  update(&world),
  render(&world, engine->getRenderContext()),
  control(&world, engine->getControlContext())
{
  setPhases({&control, &update, &render});
  new Controller(this);
}


void TitleState::init()
{
}

void TitleState::term()
{
}


TitleState::Controller::Controller(TitleState *state) :
  ew::Entity(&state->world), ew::Controllable(&state->world), state(state)
{
}

void TitleState::Controller::control(ew::ControlContext *context)
{
  if(context->keyDown(GLFW_KEY_SPACE) || context->keyDown(GLFW_KEY_ENTER))
  {
    state->engine->setState(States::GAME);
  }

  if(context->keyDown(GLFW_KEY_ESC))
  {
    state->engine->quit();
  }
}
