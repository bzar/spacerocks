#ifndef ENTITY_HH
#define ENTITY_HH

#include "world.h"
#include <iostream>

class Entity
{
public:
  typedef unsigned int Id;

  Entity(World* world) : world(world) { world->addEntity(this); }
  virtual ~Entity() {}
  virtual Id getEntityId() const = 0;

protected:
  static Id newEntityId() { return nextId++; }
  World* world;

private:
  static Id nextId;
};


#endif