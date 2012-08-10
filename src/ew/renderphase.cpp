#include "renderphase.h"
#include "renderable.h"

void RenderPhase::execute(RenderableWorld* world, float const delta)
{
  for(Renderable* r : world->getRenderables())
  {
    r->render();
  }
}
