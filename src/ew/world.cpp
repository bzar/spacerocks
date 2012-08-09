#include "world.h"
#include "entity.h"
#include "renderable.h"
#include "updatable.h"
#include "collidable.h"



World::World() :
  entities(), entitiesToInsert(), entitiesToRemove(),
  roles(), rolesToInsert()
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
  entitiesToRemove.insert(entity);
}

void World::registerRole(Entity* entity, UID const roleId)
{
  rolesToInsert[roleId].insert(entity);
}

void World::unregisterRole(Entity* entity, UID const roleId)
{
  roles[roleId].erase(entity);
}

std::set<Entity*>& World::entitiesWithRole(UID const roleId)
{
  return roles[roleId];
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

  for(auto i = rolesToInsert.begin(); i != rolesToInsert.end(); ++i)
  {
    roles[i->first].insert(i->second.begin(), i->second.end());
  }
  rolesToInsert.clear();
}

