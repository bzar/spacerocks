#ifndef COLLIDABLE_HH
#define COLLIDABLE_HH

#include "entity.h"

class Collidable : public virtual Entity
{
public:
  Collidable(World* world) : Entity(world) { world->registerRole(this, ID); }
  virtual ~Collidable() { world->unregisterRole(this, ID); }
  virtual void collide(Collidable const* other) = 0;
  static UID const ID;
};

#endif
