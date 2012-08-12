#include "world.h"
#include "entity.h"
#include "renderable.h"
#include "updatable.h"
#include "collidable.h"



World::World() :
  entities(), entitiesToInsert(), entitiesToRemove()
{
}

World::~World()
{
  for(Entity* e : entities)
  {
    delete e;
  }
}

void World::addEntity(Entity* entity)
{
  entitiesToInsert.insert(entity);
}

void World::removeEntity(Entity* entity)
{
  auto toBeInserted = entitiesToInsert.find(entity);
  if(toBeInserted != entitiesToInsert.end())
  {
    delete entity;
    entitiesToInsert.erase(toBeInserted);
  }
  else
  {
    entitiesToRemove.insert(entity);
  }
}

void World::maintenance()
{
  for(Entity* entity : entitiesToRemove)
  {
    entities.erase(entity);
    delete entity;
  }

  entities.insert(entitiesToInsert.begin(), entitiesToInsert.end());
  entitiesToInsert.clear();
  entitiesToRemove.clear();
}

