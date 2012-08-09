#include "updatephase.h"
#include "updatable.h"

void UpdatePhase::execute(World* world, float const delta)
{
  for(Entity* e : world->entitiesWithRole(Updatable::ID))
  {
    Updatable* u = static_cast<Updatable*>(e);
    u->update(delta);
  }
}
