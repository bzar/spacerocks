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

}

RenderableWorld::~RenderableWorld()
{

}


void RenderableWorld::registerRenderable(Renderable* renderable)
{
  renderablesToInsert.insert(renderable);
}

void RenderableWorld::unregisterRenderable(Renderable* renderable)
{
  renderablesToInsert.erase(renderable);
  renderables.erase(renderable);
}

std::set<Renderable*, RenderableWorld::ZComparator> const& RenderableWorld::getRenderables()
{
  if(!renderablesToInsert.empty())
  {
    renderables.insert(renderablesToInsert.begin(), renderablesToInsert.end());
    renderablesToInsert.clear();
  }
  return renderables;
}

