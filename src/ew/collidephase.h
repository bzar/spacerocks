#ifndef COLLIDEPHASE_HH
#define COLLIDEPHASE_HH

#include "phase.h"

class CollidePhase : public Phase
{
public:
  void execute(World* world, float const delta);
};

#endif
