#include "titleworld.h"
#include "prop.h"
#include "titlesprite.h"
#include "titlestart.h"

TitleWorld::TitleWorld(ew::Engine *engine) :
  ew::World(), ew::RenderableWorld(), ew::UpdatableWorld(),
  ew::ControllableWorld(),
  engine(engine)
{
  new Prop(this, "img/title-background.png", 800, 480, 0, -1);
  titleSprites = {
    new TitleSprite(this, "img/title-space.png", "snd/sfx/weaponfire2.wav", {-800, -50}, {-150, 50}, 1.0, 1.5, 1),
    new TitleSprite(this, "img/title-rocks.png", "snd/sfx/weaponfire2.wav", {700, 50}, {0, -50}, 1.5, 2, 2),
    new TitleSprite(this, "img/title-exclamation.png", "snd/sfx/explosion1.wav", {450, 370}, {320, 80}, 2.2, 2.5, 3)
  };
  titleStart = new TitleStart(this, "img/title-start.png", {0, -200}, 2.8, 0.2f, 4);
}

void TitleWorld::reset()
{
  for(TitleSprite* t : titleSprites)
  {
    t->reset();
  }
  titleStart->reset();
}
