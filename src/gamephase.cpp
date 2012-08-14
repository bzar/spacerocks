#include "gamephase.h"
#include "GL/glfw3.h"

GamePhase::GamePhase(GameWorld* world, Engine* engine) :
  world(world), engine(engine)
{

}

void GamePhase::execute(float const delta)
{
  if(engine->getControlContext()->keyDown(GLFW_KEY_ESCAPE))
  {
    engine->quit();
  }

  world->update(delta);
}
