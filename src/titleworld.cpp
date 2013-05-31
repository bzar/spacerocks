#include "titleworld.h"
#include "prop.h"

TitleWorld::TitleWorld(ew::Engine *engine) :
  ew::World(), ew::RenderableWorld(), ew::UpdatableWorld(),
  ew::ControllableWorld(),
  engine(engine)
{
  new Prop(this, "img/title-background.png", 800, 480, 0, -1);
}
