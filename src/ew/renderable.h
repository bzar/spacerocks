#ifndef RENDERABLE_HH
#define RENDERABLE_HH

#include "entity.h"

class Renderable : public virtual Entity
{
public:
  Renderable(World* world, int zIndex = 0, int layer = 0) : Entity(world), layer(layer), zIndex(zIndex) { world->registerRole(this, ID); }
  virtual ~Renderable() { world->unregisterRole(this, ID); }
  virtual void render() = 0;
  virtual int getLayer() const { return layer; }
  virtual int getZIndex() const { return zIndex; }
  static UID const ID;

private:
  int const layer;
  int const zIndex;
};

#endif
