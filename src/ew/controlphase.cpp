#include "controlphase.h"
#include "controllable.h"

ew::ControlPhase::ControlPhase(ControllableWorld* world, ControlContext* context) :
  world(world), context(context)
{
}

void ew::ControlPhase::execute(float const delta)
{
  for(Controllable* c : world->getControllables())
  {
    c->control(context);
  }
}
