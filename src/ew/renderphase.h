#ifndef RENDERPHASE_HH
#define RENDERPHASE_HH

#include "phase.h"
#include "renderableworld.h"

class RenderPhase : public Phase
{
public:
  RenderPhase(RenderableWorld* world) : world(world) {}
  void execute(float const delta);

private:
  RenderableWorld* world;
};

#endif
