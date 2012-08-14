#include "gamestate.h"

#include "asteroid.h"
#include "ship.h"
#include "laser.h"
#include "shot.h"
#include "plasma.h"
#include "beam.h"
#include "explosion.h"
#include "ufo.h"
#include "ufolaser.h"
#include "powerup.h"
#include "particleengine.h"

void GameState::init()
{
  Ship::init();
  Asteroid::init();
  Laser::init();
  Shot::init();
  Plasma::init();
  Beam::init();
  Explosion::init();
  Ufo::init();
  UfoLaser::init();
  Powerup::init();
  ParticleEngine::init();
}

void GameState::term()
{
  Ship::term();
  Asteroid::term();
  Laser::term();
  Shot::term();
  Plasma::term();
  Beam::term();
  Explosion::term();
  Ufo::term();
  UfoLaser::term();
  Powerup::term();
  ParticleEngine::term();
}

GameState::GameState(Engine* engine) :
  State(engine, &world),
  world(engine), game(&world, engine), update(&world), collide(&world),
  render(&world), control(&world, engine->getControlContext())
{
  setPhases({&control, &update, &collide, &render, &game});
}
