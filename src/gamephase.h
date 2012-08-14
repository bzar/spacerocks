#ifndef GAMEPHASE_HH
#define GAMEPHASE_HH

#include "ew/phase.h"
#include "ew/engine.h"
#include "gameworld.h"

class GamePhase : public ew::Phase
{
public:
  GamePhase(GameWorld* world, ew::Engine* engine);
  void execute(float const delta);

private:
  GameWorld* world;
  ew::Engine* engine;
};

#endif
