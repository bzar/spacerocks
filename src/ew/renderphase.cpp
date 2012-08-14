#include "renderphase.h"
#include "renderable.h"

void ew::RenderPhase::execute(float const delta)
{
  for(Renderable* r : world->getRenderables())
  {
    r->render(context);
  }
}
