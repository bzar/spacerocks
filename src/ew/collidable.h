#ifndef COLLIDABLE_HH
#define COLLIDABLE_HH

#include "entity.h"
#include "collidableworld.h"

namespace ew
{
  class Collidable : public virtual Entity
  {
  public:
    Collidable(CollidableWorld* world) : Entity(world), collidableWorld(world)
    {
      collidableWorld->registerCollidable(this);
    }
    virtual ~Collidable() { if(collidableWorld != nullptr) collidableWorld->unregisterCollidable(this); }
    virtual void collide(Collidable const* other) = 0;
    void collidableUnregistered() { collidableWorld = nullptr; }

  private:
    CollidableWorld* collidableWorld;
  };
};
#endif
