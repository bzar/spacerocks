#ifndef WORLD_HH
#define WORLD_HH

#include <set>
#include <memory>
#include "sprite.h"

struct World {
  std::set<std::shared_ptr<Sprite>> sprites;
};

#endif
