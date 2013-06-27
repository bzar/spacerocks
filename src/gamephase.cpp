#include "gamephase.h"
#include "GLFW/glfw3.h"
#include "glhck/glhck.h"
#include "hud.h"
#include "states.h"

GamePhase::GamePhase(GameWorld* world, ew::Engine* engine) :
  world(world), engine(engine)
{

}

void GamePhase::execute(float const delta)
{
  if(engine->getControlContext()->keyPush(GLFW_KEY_ESCAPE))
  {
    engine->setState(States::TITLE);
  }

  if(engine->getControlContext()->keyPush(GLFW_KEY_P) || engine->getControlContext()->keyPush(GLFW_KEY_LEFT_ALT))
  {
    world->setPaused(!world->getPaused());
  }

  if(engine->getControlContext()->keyPush(GLFW_KEY_I))
  {
    world->hud->toggleShowFPS();
  }

  if(engine->getControlContext()->keyPush(GLFW_KEY_O)) {
    if(glhckRenderGetPass() & GLHCK_PASS_OVERDRAW) {
      glhckRenderPass(glhckRenderPassDefaults() & ~GLHCK_PASS_DEPTH);
    } else {
      glhckRenderPass(GLHCK_PASS_OVERDRAW);
    }
  }

  world->update(delta);
}
