#ifndef GAMEOVERWORLD_H
#define GAMEOVERWORLD_H

#include "ew/renderableworld.h"
#include "ew/updatableworld.h"
#include "ew/controllableworld.h"
#include "ew/engine.h"

class GameOverWorld : public ew::RenderableWorld, public ew::UpdatableWorld,
    public ew::ControllableWorld
{
public:
  GameOverWorld(ew::Engine* engine);

private:
  ew::Engine* engine;
};

#endif // GAMEOVERWORLD_H
