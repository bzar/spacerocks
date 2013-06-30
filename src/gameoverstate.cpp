#include "gameoverstate.h"
#include "GLFW/glfw3.h"
#include "states.h"
#include "gamestate.h"
#include "text.h"
#include "keys.h"

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
  world.setScore(gameWorld->player.cheater ? -1 : gameWorld->player.score);
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
  if(state->world.enteringHighScore())
  {
    if(actionKeyPush(ACTION_ENTER_LETTER, context))
    {
      state->world.enterHighScoreEntryLetter();
    }
    if(actionKeyPush(ACTION_ERASE_LETTER, context))
    {
      state->world.eraseHighScoreEntryLetter();
    }

    if(actionKeyPush(ACTION_NEXT_LETTER, context))
    {
      state->world.nextHighScoreEntryLetter();
    }

    if(actionKeyPush(ACTION_PREV_LETTER, context))
    {
      state->world.prevHighScoreEntryLetter();
    }
  }
  else
  {
    if(actionKeyPush(ACTION_START, context))
    {
      state->engine->setState(States::HIGHSCORES);
    }

  }
}
