#ifndef GAMEOVERWORLD_H
#define GAMEOVERWORLD_H

#include "ew/renderableworld.h"
#include "ew/updatableworld.h"
#include "ew/controllableworld.h"
#include "ew/engine.h"
#include "text.h"

class GameOverWorld : public ew::RenderableWorld, public ew::UpdatableWorld,
    public ew::ControllableWorld
{
public:
  GameOverWorld(ew::Engine* engine);
  void setScore(int const newScore);

private:
  ew::Engine* engine;

  Text* scoreText;
  int score;
};

#endif // GAMEOVERWORLD_H
