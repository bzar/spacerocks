#ifndef GAMEWORLD_HH
#define GAMEWORLD_HH

#include "world.h"

class Ship;
class ParticleEngine;

class GameWorld : public World
{
public:
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

  GameWorld(Player const& player = Player(), Level const& level = Level());

  void update(float const delta);
  void render();

  void nextLevel();
  void initLevel(int const n);

  Level level;
  Player player;
  ParticleEngine* particleEngine;

private:
  static int const UFO_SCORE_INTERVAL_MIN;
  static int const UFO_SCORE_INTERVAL_MAX;
  static float const DEATH_DELAY;
  static float const UFO_DELAY;

  static int getUfoInterval();

  float deathDelay;
  float ufoDelay;
  int nextUfoScore;

};

#endif
