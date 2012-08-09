#ifndef RENDERPHASE_HH
#define RENDERPHASE_HH

#include "phase.h"

class RenderPhase : public Phase
{
public:
  void execute(World* world, float const delta);

private:
  class ZComparator
  {
  public:
    bool operator()(Renderable const* a, Renderable const* b);
  };
};

#endif
