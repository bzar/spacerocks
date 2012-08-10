#include "controlphase.h"
#include "controllable.h"

ControlPhase::ControlPhase(ControlContext* context) :
  context(context)
{
}

void ControlPhase::execute(ControllableWorld* world, float const delta)
{
  for(Controllable* c : world->getControllables())
  {
    c->control(context);
  }
}
