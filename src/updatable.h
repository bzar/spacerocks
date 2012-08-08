#ifndef UPDATABLE_HH
#define UPDATABLE_HH

#include "entity.h"

class Updatable : public virtual Entity
{
public:
  Updatable(World* world) : Entity(world) { world->addUpdatable(this); }
  virtual ~Updatable() { world->removeUpdatable(this); }
  virtual void update(float const delta) = 0;
};

#endif
