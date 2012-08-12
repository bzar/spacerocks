#include "collidableworld.h"

CollidableWorld::CollidableWorld() :
  World(), collidables(), collidablesToInsert()
{

}

CollidableWorld::~CollidableWorld()
{

}


void CollidableWorld::registerCollidable(Collidable* collidable)
{
  collidablesToInsert.insert(collidable);
}

void CollidableWorld::unregisterCollidable(Collidable* collidable)
{
  collidablesToInsert.erase(collidable);
  collidables.erase(collidable);
}

std::set<Collidable*> const& CollidableWorld::getCollidables()
{
  if(!collidablesToInsert.empty())
  {
    collidables.insert(collidablesToInsert.begin(), collidablesToInsert.end());
    collidablesToInsert.clear();
  }
  return collidables;
}

