#ifndef UPDATABLE_HH
#define UPDATABLE_HH

#include "entity.h"

class Updatable : public virtual Entity
{
public:
  Updatable(World* world) : Entity(world) { world->registerRole(this, ID); }
  virtual ~Updatable() { world->unregisterRole(this, ID); }
  virtual void update(float const delta) = 0;
  static UID const ID;
};

#endif
