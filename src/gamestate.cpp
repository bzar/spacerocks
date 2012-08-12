#include "gamestate.h"

#include "GL/glfw3.h"

#include "util/vec2d.h"
#include "util/util.h"

#include "ew/engine.h"

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
  State(engine), world(engine),
  game(engine), update(), collide(), render(),
  control(engine->getControlContext())
{
}

World* GameState::getWorld()
{
  return &world;
}

void GameState::process(float const delta)
{
  control.execute(&world, delta);
  update.execute(&world, delta);
  collide.execute(&world, delta);
  render.execute(&world, delta);
  game.execute(&world, delta);
}
