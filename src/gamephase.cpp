#include "gamephase.h"
#include "keys.h"
#include "glhck/glhck.h"
#include "hud.h"
#include "states.h"

GamePhase::GamePhase(GameWorld* world, ew::Engine* engine) :
  world(world), engine(engine)
{

}

void GamePhase::execute(float const delta)
{
  ew::ControlContext* ctx = engine->getControlContext();
  if(actionKeyPush(ACTION_EXIT, ctx))
  {
    engine->setState(States::TITLE);
  }

  if(actionKeyPush(ACTION_PAUSE, ctx))
  {
    world->setPaused(!world->getPaused());
  }

  if(actionKeyPush(ACTION_SHOW_INFO, ctx))
  {
    world->hud->toggleShowFPS();
  }

  if(actionKeyPush(ACTION_SHOW_OVERDRAW, ctx)) {
    if(glhckRenderGetPass() & GLHCK_PASS_OVERDRAW) {
      glhckRenderPass(glhckRenderPassDefaults() & ~GLHCK_PASS_DEPTH);
    } else {
      glhckRenderPass(GLHCK_PASS_OVERDRAW);
    }
  }

  if(actionKeyPush(ACTION_SKIP_LEVEL, ctx)) {
    world->reset(world->level.n + 1);
  }
  world->update(delta);
}
