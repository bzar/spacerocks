#include "updatableworld.h"

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

}


void UpdatableWorld::registerUpdatable(Updatable* updatable)
{
  updatablesToInsert.insert(updatable);
}

void UpdatableWorld::unregisterUpdatable(Updatable* updatable)
{
  updatablesToInsert.erase(updatable);
  updatables.erase(updatable);
}

std::set<Updatable*> const& UpdatableWorld::getUpdatables()
{
  return updatables;
}

