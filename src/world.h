#ifndef WORLD_HH
#define WORLD_HH

#include <set>
#include <memory>
#include "sprite.h"
#include "ship.h"

class ZComparator
{
public:
  bool operator()(std::shared_ptr<Sprite> const& a, std::shared_ptr<Sprite> const& b)
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
};

typedef std::set<std::shared_ptr<Sprite>, ZComparator> SpriteSet;

struct Level
{
  int n;
  float minAsteroidSpeed;
  float maxAsteroidSpeed;
  float ufoDuration;
  float ufoAccuracy;
  float ufoShootInterval;
};

struct World
{
  Ship* ship;
  int score;
  SpriteSet sprites;
  Level level;
};

#endif
