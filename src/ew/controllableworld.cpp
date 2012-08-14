#include "controllableworld.h"
#include "controllable.h"

ew::ControllableWorld::ControllableWorld() :
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

ew::ControllableWorld::~ControllableWorld()
{
  for(Controllable* c : controllables)
  {
    c->controllableUnregistered();
  }
}


void ew::ControllableWorld::registerControllable(Controllable* controllable)
{
  controllablesToInsert.insert(controllable);
}

void ew::ControllableWorld::unregisterControllable(Controllable* controllable)
{
  controllablesToInsert.erase(controllable);
  controllables.erase(controllable);
  controllable->controllableUnregistered();
}

std::set<ew::Controllable*> const& ew::ControllableWorld::getControllables()
{
  return controllables;
}

