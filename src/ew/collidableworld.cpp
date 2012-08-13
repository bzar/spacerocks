#include "collidableworld.h"
#include "collidable.h"

CollidableWorld::CollidableWorld() :
  World(), collidables(), collidablesToInsert()
{
  onMaintenance([&]() {
    if(!collidablesToInsert.empty())
    {
      collidables.insert(collidablesToInsert.begin(), collidablesToInsert.end());
      collidablesToInsert.clear();
    }
  });
}

CollidableWorld::~CollidableWorld()
{
  for(Collidable* c : collidables)
  {
    c->collidableUnregistered();
  }
}


void CollidableWorld::registerCollidable(Collidable* collidable)
{
  collidablesToInsert.insert(collidable);
}

void CollidableWorld::unregisterCollidable(Collidable* collidable)
{
  collidablesToInsert.erase(collidable);
  collidables.erase(collidable);
  collidable->collidableUnregistered();
}

std::set<Collidable*> const& CollidableWorld::getCollidables()
{
  return collidables;
}

