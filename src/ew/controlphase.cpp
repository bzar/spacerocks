#include "controlphase.h"
#include "controllable.h"

ControlPhase::ControlPhase(ControllableWorld* world, ControlContext* context) :
  world(world), context(context)
{
}

void ControlPhase::execute(float const delta)
{
  for(Controllable* c : world->getControllables())
  {
    c->control(context);
  }
}
