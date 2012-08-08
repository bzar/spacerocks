#include "world.h"
#include "entity.h"
#include "renderable.h"
#include "updatable.h"
#include "collidable.h"

bool World::ZComparator::operator()(Renderable const* a, Renderable const* b)
{
  if(a->getLayer() != b->getLayer())
  {
    return a->getLayer() < b->getLayer();
  }
  if(a->getZIndex() != b->getZIndex())
  {
    return a->getZIndex() < b->getZIndex();
  }
  else
  {
    return a < b;
  }
}

World::World() :
  entities(), entitiesToInsert(), entitiesToRemove(),
  renderables(), renderablesToInsert(),
  updatables(), updatablesToInsert(),
  collidables(), collidablesToInsert()
{
}

World::~World()
{
  for(Entity* e : entities)
  {
    delete e;
  }
}

void World::update(float const delta)
{
  for(Updatable* u : updatables)
  {
    u->update(delta);
  }

  for(Collidable* c1 : collidables)
  {
    for(Collidable* c2 : collidables)
    {
      if(c1 != c2)
      {
        c1->collide(c2);
      }
    }
  }

  for(Entity* entity : entitiesToRemove)
  {
    entities.erase(entity);
    delete entity;
  }

  entities.insert(entitiesToInsert.begin(), entitiesToInsert.end());
  entitiesToInsert.clear();
  entitiesToRemove.clear();

  renderables.insert(renderablesToInsert.begin(), renderablesToInsert.end());
  renderablesToInsert.clear();

  updatables.insert(updatablesToInsert.begin(), updatablesToInsert.end());
  updatablesToInsert.clear();

  collidables.insert(collidablesToInsert.begin(), collidablesToInsert.end());
  collidablesToInsert.clear();
}

void World::render()
{
  for(Renderable* r : renderables)
  {
    r->render();
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

void World::addRenderable(Renderable* const renderable)
{
  renderablesToInsert.insert(renderable);
}

void World::removeRenderable(Renderable* const renderable)
{
  renderables.erase(renderable);
}

void World::addUpdatable(Updatable* const updatable)
{
  updatablesToInsert.insert(updatable);
}

void World::removeUpdatable(Updatable* const updatable)
{
  updatables.erase(updatable);
}

void World::addCollidable(Collidable* const collidable)
{
  collidablesToInsert.insert(collidable);
}

void World::removeCollidable(Collidable* const collidable)
{
  collidables.erase(collidable);
}
