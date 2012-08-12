#include "gameworld.h"
#include "util/util.h"
#include "ship.h"
#include "asteroid.h"
#include "ufo.h"
#include "ufolaser.h"
#include "particleengine.h"
#include "prop.h"
#include "hud.h"
#include "ew/engine.h"

int const GameWorld::UFO_SCORE_INTERVAL_MIN = 400;
int const GameWorld::UFO_SCORE_INTERVAL_MAX = 800;
float const GameWorld::DEATH_DELAY = 3.0f;
float const GameWorld::UFO_DELAY = 2.0f;

GameWorld::GameWorld(Engine* engine) :
  World(), RenderableWorld(), UpdatableWorld(), CollidableWorld(),
  ControllableWorld(),
  player(), level(),
  deathDelay(0), ufoDelay(0), nextUfoScore(getUfoInterval()),
  particleEngine(new ParticleEngine(this)), engine(engine),
  hud(new Hud(this))
{
  this->player.lives = 3;
  initLevel(0);
  this->player.ship = new Ship(this, {0, 0}, {0, 0});
  new Prop(this, "img/background.png", 400, 240, 0, -1);
}

void GameWorld::update(float const delta)
{
  if(!player.ship->alive())
  {
    if(deathDelay == 0)
    {
      deathDelay = DEATH_DELAY;
    }
    else
    {
      deathDelay -= delta;
    }

    if(deathDelay <= 0)
    {
      if(player.lives > 0)
      {
        deathDelay = 0;
        player.lives -= 1;
        player.ship->reset();
      }
    }
  }

  if(ufoDelay > 0)
  {
    ufoDelay -= delta;

    if(ufoDelay <= 0)
    {
      bool horizontal = rand() % 2;
      bool direction = rand() % 2;
      int d = rand() % (horizontal ? 480 : 800);
      Vec2D position(!horizontal ? d : direction ? 0 : 800,
                      horizontal ? d : direction ? 0 : 480);
      position -= Vec2D(400, 240);
      Ufo* ufo = new Ufo(this, position, position.neg(), randFloat(0, 5), randFloat(10, 100));
    }
  }

  if(player.score >= nextUfoScore)
  {
    nextUfoScore += getUfoInterval();
    ufoDelay = UFO_DELAY;
  }

  bool victory = ufoDelay <= 0 && player.ship != nullptr;

  for(Entity* e : entities)
  {
    victory = victory
      && e->getEntityId() != Asteroid::ID
      && e->getEntityId() != Ufo::ID
      && e->getEntityId() != UfoLaser::ID;
  }

  if(victory)
  {
    nextLevel();
  }
}

void GameWorld::nextLevel()
{
  initLevel(level.n + 1);
  player.ship->reset();
}

void GameWorld::initLevel(int const n)
{
  int const ASTEROID_VALUES[Asteroid::NUM_SIZES] = {
    1,
    2 + 2*1,
    3 + 2*2 + 4*1,
    4 + 2*3 + 4*2 + 8*1,
  };

  level.n = n;
  level.minAsteroidSpeed = lerp(10, 20, n/40.0);
  level.maxAsteroidSpeed = lerp(20, 60, n/40.0);
  level.ufoDuration = lerp(20, 10, n/40.0);
  level.ufoAccuracy = lerp(0.6, 0.9, n/60.0);
  level.ufoShootInterval = lerp(3.0, 1.5, n/60.0);

  int levelsPerNextSize = 4;
  float minAsteroidDistance = 100;
  float maxAsteroidDistance = 200;
  int maxAsteroidSize = Asteroid::NUM_SIZES - 1;
  int minAsteroidSize = maxAsteroidSize - n / levelsPerNextSize;
  minAsteroidSize = minAsteroidSize < 0 ? 0 : minAsteroidSize;
  int asteroidValue = (n + 2) * ASTEROID_VALUES[maxAsteroidSize];

  while(asteroidValue > 0)
  {
    Asteroid::Size size = Asteroid::TINY;
    if(asteroidValue >= ASTEROID_VALUES[minAsteroidSize])
    {
      size = static_cast<Asteroid::Size>(randInt(minAsteroidSize, maxAsteroidSize));
    }
    else
    {
      for(int i = Asteroid::NUM_SIZES - 1; i >= 0; --i)
      {
        if(asteroidValue >= ASTEROID_VALUES[i])
        {
          size = static_cast<Asteroid::Size>(i);
        }
      }
    }

    asteroidValue -= ASTEROID_VALUES[size];
    Vec2D position = Vec2D(0, 1)
      .rotatei(randFloat(0, 1))
      .scalei(randFloat(minAsteroidDistance, maxAsteroidDistance));
    Vec2D velocity = Vec2D(0, 1)
      .rotatei(randFloat(0, 1))
      .scalei(randFloat(level.minAsteroidSpeed, level.maxAsteroidSpeed));
    Asteroid* asteroid = new Asteroid(this, size, position, velocity);
  }
}

int GameWorld::getUfoInterval()
{
  return UFO_SCORE_INTERVAL_MIN + (rand() % (UFO_SCORE_INTERVAL_MAX - UFO_SCORE_INTERVAL_MIN));
}

