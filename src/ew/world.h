#ifndef WORLD_HH
#define WORLD_HH

#include <set>
#include <map>
#include <forward_list>

class Entity;
class Renderable;
class Updatable;
class Collidable;

class World
{
public:

  World();
  virtual ~World();

  virtual void update(float const delta);
  virtual void render();

  void addEntity(Entity* entity);
  void removeEntity(Entity* entity);

  void addRenderable(Renderable* const renderable);
  void removeRenderable(Renderable* const renderable);

  void addUpdatable(Updatable* const updatable);
  void removeUpdatable(Updatable* const updatable);

  void addCollidable(Collidable* const collidable);
  void removeCollidable(Collidable* const collidable);

protected:
  std::set<Entity*> entities;

private:
  class ZComparator
  {
  public:
    bool operator()(Renderable const* a, Renderable const* b);
  };

  std::set<Entity*> entitiesToInsert;
  std::set<Entity*> entitiesToRemove;

  std::set<Renderable*, ZComparator> renderables;
  std::set<Renderable*> renderablesToInsert;

  std::set<Updatable*> updatables;
  std::set<Updatable*> updatablesToInsert;

  std::set<Collidable*> collidables;
  std::set<Collidable*> collidablesToInsert;

};

#endif
