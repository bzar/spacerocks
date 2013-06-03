#include "titlestate.h"
#include "GL/glfw3.h"
#include "states.h"

TitleState::TitleState(ew::Engine *engine) :
  ew::State(engine, &world),
  engine(engine), world(engine),
  bgSound("snd/sfx/weaponfire17.wav"),
  music("snd/music/title.ogg"),
  update(&world),
  render(&world, engine->getRenderContext()),
  control(&world, engine->getControlContext())
{
  setPhases({&control, &update, &render});
  new Controller(this);
  new Timer(this);
}

void TitleState::enter()
{
  if(!music.playing())
    music.play();
  bgSound.play();
  world.reset();
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
  if(context->keyPush(GLFW_KEY_SPACE) || context->keyPush(GLFW_KEY_ENTER))
  {
    state->engine->setState(States::GAME);
  }

  if(context->keyPush(GLFW_KEY_ESC))
  {
    state->engine->quit();
  }
}

TitleState::Timer::Timer(TitleState *state) :
  ew::Entity(&state->world), ew::Updatable(&state->world),
  state(state), stateTime(0)
{
}

void TitleState::Timer::update(const float delta)
{
  stateTime += delta;

  if(stateTime > 10)
  {
    state->engine->setState(States::HIGHSCORES);
    stateTime = 0;
  }
}

