#ifndef COLLIDABLE_HH
#define COLLIDABLE_HH

#include "entity.h"

class Collidable : public virtual Entity
{
public:
  Collidable(World* world) : Entity(world) { world->addCollidable(this); }
  virtual ~Collidable() { world->removeCollidable(this); }
  virtual void collide(Collidable const* other) = 0;
};

#endif
