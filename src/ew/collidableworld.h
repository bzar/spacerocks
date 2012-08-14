#ifndef COLLIDABLEWORLD_HH
#define COLLIDABLEWORLD_HH

#include "world.h"
#include <set>

namespace ew
{
  class Collidable;

  class CollidableWorld : public virtual World
  {
  public:
    CollidableWorld();
    virtual ~CollidableWorld();

    void registerCollidable(Collidable* collidable);
    void unregisterCollidable(Collidable* collidable);
    std::set<Collidable*> const& getCollidables();

  private:
    std::set<Collidable*> collidablesToInsert;
    std::set<Collidable*> collidables;
  };
};
#endif
