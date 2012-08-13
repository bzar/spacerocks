#include "renderableworld.h"
#include "renderable.h"

bool RenderableWorld::ZComparator::operator()(Renderable const* a, Renderable const* b)
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


RenderableWorld::RenderableWorld() :
  World(), renderables(), renderablesToInsert()
{
  onMaintenance([&]() {
    if(!renderablesToInsert.empty())
    {
      renderables.insert(renderablesToInsert.begin(), renderablesToInsert.end());
      renderablesToInsert.clear();
    }
  });
}

RenderableWorld::~RenderableWorld()
{
  for(Renderable* r : renderables)
  {
    r->renderableUnregistered();
  }
}


void RenderableWorld::registerRenderable(Renderable* renderable)
{
  renderablesToInsert.insert(renderable);
}

void RenderableWorld::unregisterRenderable(Renderable* renderable)
{
  renderablesToInsert.erase(renderable);
  renderables.erase(renderable);
  renderable->renderableUnregistered();
}

std::set<Renderable*, RenderableWorld::ZComparator> const& RenderableWorld::getRenderables()
{
  return renderables;
}

