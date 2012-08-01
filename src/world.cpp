#include "world.h"

bool ZComparator::operator()(std::shared_ptr<Sprite> const& a, std::shared_ptr<Sprite> const& b)
{
  if(a->getZIndex() == b->getZIndex() && a->getEntityId() == b->getEntityId())
  {
    return a < b;
  }
  else if(a->getZIndex() == b->getZIndex())
  {
    return a->getEntityId() < b->getEntityId();
  }
  else
  {
    a->getZIndex() < b->getZIndex();
  }
}

World newWorld()
{
  return {nullptr, 0, SpriteSet(), {0, 0, 0, 0, 0, 0}, {0, {0}}};
}
