#include "renderphase.h"
#include "renderable.h"

bool RenderPhase::ZComparator::operator()(Renderable const* a, Renderable const* b)
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

void RenderPhase::execute(World* world, float const delta)
{
  std::set<Renderable*, ZComparator> sorted;

  for(Entity* e : world->entitiesWithRole(Renderable::ID))
  {
    Renderable* r = static_cast<Renderable*>(e);
    sorted.insert(r);
  }

  for(Renderable* r : sorted)
  {
    r->render();
  }
}
