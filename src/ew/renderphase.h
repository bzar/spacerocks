#ifndef RENDERPHASE_HH
#define RENDERPHASE_HH

#include "phase.h"
#include "renderableworld.h"
#include "rendercontext.h"

class RenderPhase : public Phase
{
public:
  RenderPhase(RenderableWorld* world, RenderContext* context) : world(world), context(context) {}
  void execute(float const delta);

private:
  RenderableWorld* world;
  RenderContext* context;
};

#endif
