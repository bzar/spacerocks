#ifndef CONTROLPHASE_HH
#define CONTROLPHASE_HH

#include "phase.h"
#include "controllableworld.h"

class ControlPhase : public Phase
{
public:
  void execute(ControllableWorld* world, float const delta);
};

#endif
