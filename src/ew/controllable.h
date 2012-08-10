#ifndef CONTROLLABLE_HH
#define CONTROLLABLE_HH

#include "entity.h"
#include "controllableworld.h"

class Controllable : public virtual Entity
{
public:
  Controllable(ControllableWorld* world) : Entity(world), controllableWorld(world)
  {
    controllableWorld->registerControllable(this);
  }
  virtual ~Controllable() { controllableWorld->unregisterControllable(this); }
  virtual void control() = 0;
  static UID const ID;

private:
  ControllableWorld* controllableWorld;
};

#endif
