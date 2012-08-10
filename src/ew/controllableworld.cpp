#include "controllableworld.h"

ControllableWorld::ControllableWorld() :
  World(), controllables(), controllablesToInsert()
{

}

ControllableWorld::~ControllableWorld()
{

}


void ControllableWorld::registerControllable(Controllable* controllable)
{
  controllablesToInsert.insert(controllable);
}

void ControllableWorld::unregisterControllable(Controllable* controllable)
{
  controllables.erase(controllable);
}

std::set<Controllable*> const& ControllableWorld::getControllables()
{
  if(!controllablesToInsert.empty())
  {
    controllables.insert(controllablesToInsert.begin(), controllablesToInsert.end());
    controllablesToInsert.clear();
  }
  return controllables;
}

