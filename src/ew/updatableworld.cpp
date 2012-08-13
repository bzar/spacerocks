#include "updatableworld.h"
#include "updatable.h"

UpdatableWorld::UpdatableWorld() :
  World(), updatables(), updatablesToInsert()
{
  onMaintenance([&]() {
    if(!updatablesToInsert.empty())
    {
      updatables.insert(updatablesToInsert.begin(), updatablesToInsert.end());
      updatablesToInsert.clear();
    }
  });
}

UpdatableWorld::~UpdatableWorld()
{
  for(Updatable* u : updatables)
  {
    u->updatableUnregistered();
  }
}


void UpdatableWorld::registerUpdatable(Updatable* updatable)
{
  updatablesToInsert.insert(updatable);
}

void UpdatableWorld::unregisterUpdatable(Updatable* updatable)
{
  updatablesToInsert.erase(updatable);
  updatables.erase(updatable);
  updatable->updatableUnregistered();
}

std::set<Updatable*> const& UpdatableWorld::getUpdatables()
{
  return updatables;
}

