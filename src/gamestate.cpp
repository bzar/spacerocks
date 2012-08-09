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
  State(engine), world(engine)
{
}

void GameState::input()
{
  GLFWwindow& window = *engine->getWindow();

  bool lastPrevWeaponButtonState = glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS;
  bool lastNextWeaponButtonState = glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS;
  bool lastF1 = glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS;
  bool lastF2 = glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS;
  bool lastF3 = glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS;
  bool lastF4 = glfwGetKey(window, GLFW_KEY_F4) == GLFW_PRESS;

  glfwPollEvents();

  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    engine->quit();
  }

  if(world.player.ship != nullptr)
  {
    world.player.ship->turnLeft(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS);
    world.player.ship->turnRight(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS);
    world.player.ship->accelerate(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS);
    world.player.ship->shoot(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS);

    if(glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS && !lastPrevWeaponButtonState)
      world.player.ship->prevWeapon();

    if(glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS && !lastNextWeaponButtonState)
      world.player.ship->nextWeapon();

    if(glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS && !lastF1)
      world.player.ship->increaseLaserLevel();
    if(glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS && !lastF2)
      world.player.ship->increaseSpreadLevel();
    if(glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS && !lastF3)
      world.player.ship->increaseBeamLevel();
    if(glfwGetKey(window, GLFW_KEY_F4) == GLFW_PRESS && !lastF4)
      world.player.ship->increasePlasmaLevel();
  }
}

void GameState::update(float const delta)
{
  world.update(delta);
}

void GameState::render()
{
  world.render();
}
