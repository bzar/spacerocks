#ifndef RENDERABLE_HH
#define RENDERABLE_HH

#include "entity.h"
#include "renderableworld.h"

class Renderable : public virtual Entity
{
public:
  Renderable(RenderableWorld* world, int zIndex = 0, int layer = 0) :
    Entity(world), renderableWorld(world), layer(layer), zIndex(zIndex)
  {
    renderableWorld->registerRenderable(this);
  }
  virtual ~Renderable() { if(renderableWorld != nullptr) renderableWorld->unregisterRenderable(this); }
  virtual void render() = 0;
  virtual int getLayer() const { return layer; }
  virtual int getZIndex() const { return zIndex; }
  void renderableUnregistered() { renderableWorld = nullptr; }
  static UID const ID;

private:
  RenderableWorld* renderableWorld;
  int const layer;
  int const zIndex;
};

#endif
