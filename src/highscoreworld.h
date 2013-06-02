#ifndef HIGHSCOREWORLD_H
#define HIGHSCOREWORLD_H

#include "ew/renderableworld.h"
#include "ew/updatableworld.h"
#include "ew/controllableworld.h"
#include "ew/engine.h"

class HighScoreWorld : public ew::RenderableWorld, public ew::UpdatableWorld,
    public ew::ControllableWorld
{
public:
  HighScoreWorld(ew::Engine* engine);

private:
  ew::Engine* engine;
};

#endif // HIGHSCOREWORLD_H
