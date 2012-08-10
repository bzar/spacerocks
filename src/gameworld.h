#ifndef GAMEWORLD_HH
#define GAMEWORLD_HH

#include "ew/renderableworld.h"
#include "ew/updatableworld.h"
#include "ew/collidableworld.h"
#include "ew/controllableworld.h"

class Ship;
class ParticleEngine;
class Engine;
class Hud;

class GameWorld : public RenderableWorld, public UpdatableWorld,
                  public CollidableWorld, public ControllableWorld
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

  GameWorld(Engine* engine);

  void update(float const delta);

  void nextLevel();
  void initLevel(int const n);

  Level level;
  Player player;
  ParticleEngine* const particleEngine;
  Engine* const engine;
  Hud const* hud;

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
