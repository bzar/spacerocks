#ifndef UPDATEPHASE_HH
#define UPDATEPHASE_HH

#include "phase.h"
#include "updatableworld.h"

class UpdatePhase : public Phase
{
public:
  void execute(UpdatableWorld* world, float const delta);
};

#endif
