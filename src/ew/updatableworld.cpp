#include "updatableworld.h"
#include "updatable.h"

ew::UpdatableWorld::UpdatableWorld() :
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

ew::UpdatableWorld::~UpdatableWorld()
{
  for(Updatable* u : updatables)
  {
    u->updatableUnregistered();
  }
}


void ew::UpdatableWorld::registerUpdatable(Updatable* updatable)
{
  updatablesToInsert.insert(updatable);
}

void ew::UpdatableWorld::unregisterUpdatable(Updatable* updatable)
{
  updatablesToInsert.erase(updatable);
  updatables.erase(updatable);
  updatable->updatableUnregistered();
}

std::set<ew::Updatable*> const& ew::UpdatableWorld::getUpdatables()
{
  return updatables;
}

