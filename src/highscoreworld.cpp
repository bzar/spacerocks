#include "highscoreworld.h"
#include "prop.h"

HighScoreWorld::HighScoreWorld(ew::Engine *engine) :
  ew::World(), ew::RenderableWorld(), ew::UpdatableWorld(),
  ew::ControllableWorld(),
  engine(engine)
{
  new Prop(this, "img/highscores.png", 800, 480, 0, -1);
}
