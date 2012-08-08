#ifndef RENDERABLE_HH
#define RENDERABLE_HH

#include "entity.h"

class Renderable : public virtual Entity
{
public:
  Renderable(World* world, int zIndex = 0, int layer = 0) : Entity(world), layer(layer), zIndex(zIndex) { world->addRenderable(this); }
  virtual ~Renderable() { world->removeRenderable(this); }
  virtual void render() = 0;
  virtual int getLayer() const { return layer; }
  virtual int getZIndex() const { return zIndex; }

private:
  int const layer;
  int const zIndex;
};

#endif
