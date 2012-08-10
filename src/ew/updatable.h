#ifndef UPDATABLE_HH
#define UPDATABLE_HH

#include "entity.h"
#include "updatableworld.h"

class Updatable : public virtual Entity
{
public:
  Updatable(UpdatableWorld* world) : Entity(world), updatableWorld(world)
  {
    updatableWorld->registerUpdatable(this);
  }
  virtual ~Updatable() { updatableWorld->registerUpdatable(this); }
  virtual void update(float const delta) = 0;
  static UID const ID;

private:
  UpdatableWorld* updatableWorld;
};

#endif
