#ifndef CONTROLLABLEWORLD_HH
#define CONTROLLABLEWORLD_HH

#include "world.h"
#include <set>

namespace ew
{
  class Controllable;

  class ControllableWorld : public virtual World
  {
  public:
    ControllableWorld();
    virtual ~ControllableWorld();

    void registerControllable(Controllable* controllable);
    void unregisterControllable(Controllable* controllable);
    std::set<Controllable*> const& getControllables();

  private:
    std::set<Controllable*> controllablesToInsert;
    std::set<Controllable*> controllables;
  };
};
#endif
