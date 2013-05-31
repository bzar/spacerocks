#ifndef TITLEWORLD_H
#define TITLEWORLD_H

#include "ew/renderableworld.h"
#include "ew/updatableworld.h"
#include "ew/controllableworld.h"
#include "ew/engine.h"

class TitleWorld : public ew::RenderableWorld, public ew::UpdatableWorld,
    public ew::ControllableWorld
{
public:
  TitleWorld(ew::Engine* engine);

private:
  ew::Engine* engine;
};

#endif // TITLEWORLD_H
