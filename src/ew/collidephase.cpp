#include "collidephase.h"
#include "collidable.h"

void CollidePhase::execute(CollidableWorld* world, float const delta)
{
  for(Collidable* c1 : world->getCollidables())
  {
    for(Collidable* c2 : world->getCollidables())
    {
      if(c1 != c2)
      {
        c1->collide(c2);
      }
    }
  }
}
