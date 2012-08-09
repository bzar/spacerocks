#include "collidephase.h"
#include "collidable.h"

void CollidePhase::execute(World* world, float const delta)
{
  for(Entity* e1 : world->entitiesWithRole(Collidable::ID))
  {
    Collidable* c1 = static_cast<Collidable*>(e1);
    for(Entity* e2 : world->entitiesWithRole(Collidable::ID))
    {
      Collidable* c2 = static_cast<Collidable*>(e2);
      if(c1 != c2)
      {
        c1->collide(c2);
      }
    }
  }
}
