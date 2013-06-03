#include "gameoverworld.h"
#include "prop.h"

GameOverWorld::GameOverWorld(ew::Engine *engine) :
  ew::World(), ew::RenderableWorld(), ew::UpdatableWorld(),
  ew::ControllableWorld(),
  engine(engine)
{

}
