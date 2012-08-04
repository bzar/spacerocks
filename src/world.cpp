#include "world.h"
#include "util.h"
#include "asteroid.h"
#include "ufo.h"
#include "ufolaser.h"

#include <iostream>

int const World::UFO_SCORE_INTERVAL_MIN = 400;
int const World::UFO_SCORE_INTERVAL_MAX = 800;
float const World::DEATH_DELAY = 3.0f;
float const World::UFO_DELAY = 2.0f;

bool World::ZComparator::operator()(std::shared_ptr<Sprite> const& a, std::shared_ptr<Sprite> const& b)
{
  if(a->getZIndex() != b->getZIndex())
  {
    return a->getZIndex() < b->getZIndex();
  }
  else if(a->getEntityId() != b->getEntityId())
  {
    return a->getEntityId() < b->getEntityId();
  }
  else
  {
    return a < b;
  }
}

World::World(Player const& player, Level const& level) :
  player(player), level(level),
  deathDelay(0), ufoDelay(0), nextUfoScore(getUfoInterval()),
  sprites(), spritesToInsert(), spritesToRemove()
{
}

void World::update(float const delta)
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
      addSprite(ufo);
    }
  }

  if(player.score >= nextUfoScore)
  {
    nextUfoScore += getUfoInterval();
    ufoDelay = UFO_DELAY;
  }

  for(auto i : sprites)
  {
    i->update(delta);
  }

  for(auto i : sprites)
  {
    for(auto j : sprites)
    {
      if(i != j)
      {
        i->collide(j.get());
      }
    }
  }

  for(Sprite* sprite : spritesToRemove)
  {
    for(auto i = sprites.begin(); i != sprites.end(); ++i)
    {
      if(i->get() == sprite)
      {
        sprites.erase(i);
        break;
      }
    }
  }

  for(Sprite* sprite : spritesToInsert)
  {
    sprites.insert(std::shared_ptr<Sprite>(sprite));
  }

  spritesToRemove.clear();
  spritesToInsert.clear();

  bool victory = ufoDelay <= 0 && player.ship != nullptr;

  for(auto i : sprites)
  {
    victory = victory
      && i->getEntityId() != Asteroid::ID
      && i->getEntityId() != Ufo::ID
      && i->getEntityId() != UfoLaser::ID;
  }

  if(victory)
  {
    nextLevel();
  }
}

void World::nextLevel()
{
  initLevel(level.n + 1);
  player.ship->reset();
}

void World::initLevel(int const n)
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
    sprites.insert(std::shared_ptr<Sprite>(asteroid));
  }
}

void World::addSprite(Sprite* sprite)
{
  spritesToInsert.push_front(sprite);
}

void World::removeSprite(Sprite* sprite)
{
  spritesToRemove.push_front(sprite);
}

World::Sprites const& World::getSprites()
{
  return sprites;
}

int World::getUfoInterval()
{
  return UFO_SCORE_INTERVAL_MIN + (rand() % (UFO_SCORE_INTERVAL_MAX - UFO_SCORE_INTERVAL_MIN));
}

