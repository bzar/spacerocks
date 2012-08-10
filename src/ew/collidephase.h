#ifndef COLLIDEPHASE_HH
#define COLLIDEPHASE_HH

#include "phase.h"
#include "collidableworld.h"

class CollidePhase : public Phase
{
public:
  void execute(CollidableWorld* world, float const delta);
};

#endif
