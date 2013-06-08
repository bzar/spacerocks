#ifndef HIGHSCOREWORLD_H
#define HIGHSCOREWORLD_H

#include "ew/renderableworld.h"
#include "ew/updatableworld.h"
#include "ew/controllableworld.h"
#include "ew/engine.h"
#include "highscoremanager.h"
#include "text.h"
#include <vector>

class HighScoreWorld : public ew::RenderableWorld, public ew::UpdatableWorld,
    public ew::ControllableWorld
{
public:
  HighScoreWorld(ew::Engine* engine);
  void enter();

private:
  ew::Engine* engine;
  HighScoreManager highScoreManager;
  std::vector<Text*> texts;
};

#endif // HIGHSCOREWORLD_H
