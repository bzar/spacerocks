#ifndef RENDERPHASE_HH
#define RENDERPHASE_HH

#include "phase.h"
#include "renderableworld.h"

class RenderPhase : public Phase
{
public:
  void execute(RenderableWorld* world, float const delta);
};

#endif
