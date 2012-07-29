#ifndef WORLD_HH
#define WORLD_HH

#include <set>
#include <memory>
#include "sprite.h"

class ZComparator
{
public:
  bool operator()(std::shared_ptr<Sprite> const& a, std::shared_ptr<Sprite> const& b)
  {
    return a->getZIndex() == b->getZIndex() ? a < b : a->getZIndex() < b->getZIndex();
  }
};

struct World {
  std::set<std::shared_ptr<Sprite>, ZComparator> sprites;
};

#endif
