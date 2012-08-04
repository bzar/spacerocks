#ifndef ENTITY_HH
#define ENTITY_HH

class Entity
{
public:
  virtual int getEntityId() const = 0;

protected:
  static int newEntityId() { return nextId++; }

private:
  static int nextId;
};


#endif