#ifndef PHASE_HH
#define PHASE_HH

#include "world.h"

class Phase
{
public:
  virtual void execute(World* world, float const delta) = 0;

};

#endif
