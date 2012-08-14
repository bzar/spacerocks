#include "updatephase.h"
#include "updatable.h"

void UpdatePhase::execute(float const delta)
{
  for(Updatable* u : world->getUpdatables())
  {
    u->update(delta);
  }
}
