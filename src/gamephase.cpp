#include "gamephase.h"
#include "GL/glfw3.h"
#include "hud.h"

GamePhase::GamePhase(GameWorld* world, ew::Engine* engine) :
  world(world), engine(engine)
{

}

void GamePhase::execute(float const delta)
{
  if(engine->getControlContext()->keyDown(GLFW_KEY_ESCAPE))
  {
    engine->quit();
  }
  
  if(engine->getControlContext()->keyPush(GLFW_KEY_P))
  {
    world->setPaused(!world->getPaused());
  }

  if(engine->getControlContext()->keyPush(GLFW_KEY_I))
  {
    world->hud->toggleShowFPS();
  }

  world->update(delta);
}
