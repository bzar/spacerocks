#ifndef TITLEWORLD_H
#define TITLEWORLD_H

#include "ew/renderableworld.h"
#include "ew/updatableworld.h"
#include "ew/controllableworld.h"
#include "ew/engine.h"

#include <vector>

class TitleSprite;
class TitleStart;

class TitleWorld : public ew::RenderableWorld, public ew::UpdatableWorld,
    public ew::ControllableWorld
{
public:
  TitleWorld(ew::Engine* engine);
  void reset();

private:
  ew::Engine* engine;
  std::vector<TitleSprite*> titleSprites;
  TitleStart* titleStart;
};

#endif // TITLEWORLD_H
