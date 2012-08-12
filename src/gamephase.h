#ifndef GAMEPHASE_HH
#define GAMEPHASE_HH

#include "ew/phase.h"
#include "ew/engine.h"
#include "gameworld.h"

class GamePhase : public Phase
{
public:
  GamePhase(Engine* engine);
  void execute(GameWorld* world, float const delta);

private:
  Engine* engine;
};

#endif
