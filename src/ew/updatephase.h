#ifndef UPDATEPHASE_HH
#define UPDATEPHASE_HH

#include "phase.h"

class UpdatePhase : public Phase
{
public:
  void execute(World* world, float const delta);
};

#endif
