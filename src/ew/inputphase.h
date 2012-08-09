#ifndef INPUTPHASE_HH
#define INPUTPHASE_HH

#include "phase.h"

class InputPhase : public Phase
{
public:
  void execute(World* world, float const delta);
};

#endif
