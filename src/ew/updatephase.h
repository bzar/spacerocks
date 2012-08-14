#ifndef UPDATEPHASE_HH
#define UPDATEPHASE_HH

#include "phase.h"
#include "updatableworld.h"

class UpdatePhase : public Phase
{
public:
  UpdatePhase(UpdatableWorld* world) : world(world) {}
  void execute(float const delta);

private:
  UpdatableWorld* world;
};

#endif
