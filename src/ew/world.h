#ifndef WORLD_HH
#define WORLD_HH

#include <set>
#include <map>
#include <forward_list>
#include "uniqueid.h"

class Entity;
class Renderable;
class Updatable;
class Collidable;

class World
{
public:

  World();
  virtual ~World();

  void addEntity(Entity* entity);
  void removeEntity(Entity* entity);

  void maintenance();

protected:
  std::set<Entity*> entities;

private:
  std::set<Entity*> entitiesToInsert;
  std::set<Entity*> entitiesToRemove;
};

#endif
