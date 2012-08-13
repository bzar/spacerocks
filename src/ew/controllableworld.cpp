#include "controllableworld.h"
#include "controllable.h"

ControllableWorld::ControllableWorld() :
  World(), controllables(), controllablesToInsert()
{
  onMaintenance([&]() {
    if(!controllablesToInsert.empty())
    {
      controllables.insert(controllablesToInsert.begin(), controllablesToInsert.end());
      controllablesToInsert.clear();
    }
  });

}

ControllableWorld::~ControllableWorld()
{
  for(Controllable* c : controllables)
  {
    c->controllableUnregistered();
  }
}


void ControllableWorld::registerControllable(Controllable* controllable)
{
  controllablesToInsert.insert(controllable);
}

void ControllableWorld::unregisterControllable(Controllable* controllable)
{
  controllablesToInsert.erase(controllable);
  controllables.erase(controllable);
  controllable->controllableUnregistered();
}

std::set<Controllable*> const& ControllableWorld::getControllables()
{
  return controllables;
}

