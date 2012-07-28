#ifndef ENTITY_HH
#define ENTITY_HH

namespace
{
  static int nextId = 0;
}

class Entity
{
public:
  virtual int getEntityId() const = 0;

protected:
  static int newEntityId() { return nextId++; }
};


#endif