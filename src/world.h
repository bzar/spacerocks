#ifndef WORLD_HH
#define WORLD_HH

#include <set>
#include <map>
#include <forward_list>
#include <memory>
#include "sprite.h"
#include "ship.h"

class World
{
public:
  class ZComparator
  {
  public:
    bool operator()(std::shared_ptr<Sprite> const& a, std::shared_ptr<Sprite> const& b);
  };

  typedef std::set<std::shared_ptr<Sprite>, ZComparator> Sprites;

  struct Level
  {
    Level() : n(0), minAsteroidSpeed(0), maxAsteroidSpeed(0), ufoDuration(0), ufoAccuracy(0), ufoShootInterval(0) {}

    int n;
    float minAsteroidSpeed;
    float maxAsteroidSpeed;
    float ufoDuration;
    float ufoAccuracy;
    float ufoShootInterval;
  };

  struct Player
  {
    Player() : ship(nullptr), lives(0), score(0) {}
    Ship* ship;
    int lives;
    int score;
  };

  World(Player const& player = Player(), Level const& level = Level());

  void update(float const delta);
  void nextLevel();
  void initLevel(int const n);
  void addSprite(Sprite* sprite);
  void removeSprite(Sprite* sprite);
  Sprites const& getSprites();

  Level level;
  Player player;

private:
  static int const UFO_SCORE_INTERVAL_MIN;
  static int const UFO_SCORE_INTERVAL_MAX;
  static float const DEATH_DELAY;
  static float const UFO_DELAY;

  static int getUfoInterval();

  float deathDelay;
  float ufoDelay;
  int nextUfoScore;

  Sprites sprites;
  std::forward_list<Sprite*> spritesToInsert;
  std::forward_list<Sprite*> spritesToRemove;
};

#endif
