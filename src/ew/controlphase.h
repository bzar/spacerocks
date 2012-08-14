#ifndef CONTROLPHASE_HH
#define CONTROLPHASE_HH

#include "phase.h"
#include "controllableworld.h"
#include "controlcontext.h"

class ControlPhase : public Phase
{
public:
  ControlPhase(ControllableWorld* world, ControlContext* context);
  void execute(float const delta);

private:
  ControllableWorld* world;
  ControlContext* context;
};

#endif
