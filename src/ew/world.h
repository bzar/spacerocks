#ifndef WORLD_HH
#define WORLD_HH

#include <set>
#include <map>
#include <forward_list>
#include <functional>

namespace ew
{
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
    void onMaintenance(std::function<void ()> callback);
    std::set<Entity*> entities;

  private:
    std::set<Entity*> entitiesToInsert;
    std::set<Entity*> entitiesToRemove;
    std::forward_list<std::function<void ()>> maintenanceCallbacks;
  };
};
#endif
