#ifndef GAMEWORLD_HH
#define GAMEWORLD_HH

#include "ew/renderableworld.h"
#include "ew/updatableworld.h"
#include "ew/collidableworld.h"
#include "ew/controllableworld.h"
#include "ew/engine.h"

#include "util/vec2d.h"

class Ship;
class ParticleEngine;
class Hud;

class GameWorld : public ew::RenderableWorld, public ew::UpdatableWorld,
                  public ew::CollidableWorld, public ew::ControllableWorld
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

  GameWorld(ew::Engine* engine);

  void update(float const delta);

  void nextLevel();
  void initLevel(int const n);
  
  void addScore(int amount, Vec2D const& position);

  void setPaused(bool value);
  bool getPaused() const;
  
  Level level;
  Player player;
  ParticleEngine* const particleEngine;
  ew::Engine* const engine;
  Hud* hud;

private:
  static int const UFO_SCORE_INTERVAL_MIN;
  static int const UFO_SCORE_INTERVAL_MAX;
  static float const DEATH_DELAY;
  static float const UFO_DELAY;

  static int getUfoInterval();

  float levelStartDelay;
  float deathDelay;
  float ufoDelay;
  int nextUfoScore;
  
  bool paused;
};

#endif
