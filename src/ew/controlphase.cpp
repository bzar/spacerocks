#include "controlphase.h"
#include "controllable.h"

void ControlPhase::execute(ControllableWorld* world, float const delta)
{
  for(Controllable* c : world->getControllables())
  {
    c->control();
  }
}
