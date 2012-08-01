#ifndef WORLD_HH
#define WORLD_HH

#include <set>
#include <memory>
#include "sprite.h"
#include "ship.h"

class ZComparator
{
public:
  bool operator()(std::shared_ptr<Sprite> const& a, std::shared_ptr<Sprite> const& b);
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

struct Player
{
  int lives;
  int weapon[Ship::NUM_WEAPONS];
};

struct World
{
  Ship* ship;
  int score;
  SpriteSet sprites;
  Level level;
  Player player;
};

World newWorld();

#endif
