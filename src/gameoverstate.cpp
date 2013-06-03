#include "gameoverstate.h"
#include "GL/glfw3.h"
#include "states.h"
#include "gamestate.h"
#include "text.h"

GameOverState::GameOverState(ew::Engine *engine) :
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

void GameOverState::enter()
{
  GameState* gameState = static_cast<GameState*>(engine->getState(States::GAME));
  GameWorld* gameWorld = gameState->getGameWorld();
  world.setScore(gameWorld->player.score);
}


void GameOverState::init()
{
}

void GameOverState::term()
{
}


GameOverState::Controller::Controller(GameOverState *state) :
  ew::Entity(&state->world), ew::Controllable(&state->world), state(state)
{
}

void GameOverState::Controller::control(ew::ControlContext *context)
{
  if(context->keyPush(GLFW_KEY_SPACE) || context->keyPush(GLFW_KEY_ENTER))
  {
    state->engine->setState(States::TITLE);
  }

  if(context->keyPush(GLFW_KEY_ESC))
  {
    state->engine->quit();
  }
}
