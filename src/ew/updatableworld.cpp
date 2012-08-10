#include "updatableworld.h"

UpdatableWorld::UpdatableWorld() :
  World(), updatables(), updatablesToInsert()
{

}

UpdatableWorld::~UpdatableWorld()
{

}


void UpdatableWorld::registerUpdatable(Updatable* updatable)
{
  updatablesToInsert.insert(updatable);
}

void UpdatableWorld::unregisterUpdatable(Updatable* updatable)
{
  updatables.erase(updatable);
}

std::set<Updatable*> const& UpdatableWorld::getUpdatables()
{
  if(!updatablesToInsert.empty())
  {
    updatables.insert(updatablesToInsert.begin(), updatablesToInsert.end());
    updatablesToInsert.clear();
  }
  return updatables;
}

