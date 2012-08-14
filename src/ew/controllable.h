#ifndef CONTROLLABLE_HH
#define CONTROLLABLE_HH

#include "entity.h"
#include "controllableworld.h"
#include "controlcontext.h"

namespace ew
{
  class Controllable : public virtual Entity
  {
  public:
    Controllable(ControllableWorld* world) : Entity(world), controllableWorld(world)
    {
      controllableWorld->registerControllable(this);
    }
    virtual ~Controllable() { if(controllableWorld != nullptr) controllableWorld->unregisterControllable(this); }
    virtual void control(ControlContext* context) = 0;
    void controllableUnregistered() { controllableWorld = nullptr; }

  private:
    ControllableWorld* controllableWorld;
  };
};
#endif
