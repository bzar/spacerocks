#include "collidableworld.h"
#include "collidable.h"

ew::CollidableWorld::CollidableWorld() :
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

ew::CollidableWorld::~CollidableWorld()
{
  for(Collidable* c : collidables)
  {
    c->collidableUnregistered();
  }
}


void ew::CollidableWorld::registerCollidable(Collidable* collidable)
{
  collidablesToInsert.insert(collidable);
}

void ew::CollidableWorld::unregisterCollidable(Collidable* collidable)
{
  collidablesToInsert.erase(collidable);
  collidables.erase(collidable);
  collidable->collidableUnregistered();
}

std::set<ew::Collidable*> const& ew::CollidableWorld::getCollidables()
{
  return collidables;
}

