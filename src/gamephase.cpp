#include "gamephase.h"

GamePhase::GamePhase(Engine* engine) :
  engine(engine)
{

}

void GamePhase::execute(GameWorld* world, float const delta)
{
  if(engine->getControlContext()->keyDown(GLFW_KEY_ESCAPE))
  {
    engine->quit();
  }

  world->update(delta);
}
