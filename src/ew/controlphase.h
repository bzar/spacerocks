#ifndef CONTROLPHASE_HH
#define CONTROLPHASE_HH

#include "phase.h"
#include "controllableworld.h"
#include "controlcontext.h"

class ControlPhase : public Phase
{
public:
  ControlPhase(ControlContext* context);
  void execute(ControllableWorld* world, float const delta);

private:
  ControlContext* context;
};

#endif
