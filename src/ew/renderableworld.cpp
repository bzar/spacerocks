#include "renderableworld.h"
#include "renderable.h"

bool ew::RenderableWorld::ZComparator::operator()(Renderable const* a, Renderable const* b)
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


ew::RenderableWorld::RenderableWorld() :
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

ew::RenderableWorld::~RenderableWorld()
{
  for(Renderable* r : renderables)
  {
    r->renderableUnregistered();
  }
}


void ew::RenderableWorld::registerRenderable(Renderable* renderable)
{
  renderablesToInsert.insert(renderable);
}

void ew::RenderableWorld::unregisterRenderable(Renderable* renderable)
{
  renderablesToInsert.erase(renderable);
  renderables.erase(renderable);
  renderable->renderableUnregistered();
}

std::set<ew::Renderable*, ew::RenderableWorld::ZComparator> const& ew::RenderableWorld::getRenderables()
{
  return renderables;
}

