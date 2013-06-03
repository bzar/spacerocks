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

Music GameState::music = Music();

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
  
  music.load({
    "snd/music/01.ogg",
    "snd/music/02.ogg",
    "snd/music/03.ogg",
    "snd/music/04.ogg",
    "snd/music/05.ogg",
    "snd/music/06.ogg",
    "snd/music/07.ogg",
    "snd/music/08.ogg",
    "snd/music/09.ogg"
  });
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

GameState::GameState(ew::Engine* engine) :
  ew::State(engine, &world), world(engine),
  game(&world, engine), update(&world), collide(&world),
  render(&world, engine->getRenderContext()),
  control(&world, engine->getControlContext())
{
  setPhases({&control, &update, &collide, &render, &game});
}

void GameState::enter()
{
  world.reset();
}

void GameState::exit()
{
  music.stop();
}

void GameState::process(const float delta)
{
    ew::State::process(delta);

    if(!music.playing())
    {
      music.next();
      music.play();
    }
}

GameWorld *GameState::getGameWorld()
{
  return &world;
}
