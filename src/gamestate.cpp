#include "gamestate.h"

#include "GL/glfw3.h"

#include <iomanip>
#include <sstream>
#include "vec2d.h"
#include "util.h"

#include "engine.h"

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

GameState::GameState(Engine* engine) :
  State(engine), world(), background(nullptr),
  gameText(nullptr), fpsText(nullptr), gameFont(0), fpsFont(0)
{
  world.player.lives = 3;
  world.initLevel(0);

  world.player.ship = new Ship(&world, {0, 0}, {0, 0});
  world.addSprite(world.player.ship);

  background = glhckSpriteNewFromFile("img/background.png", 400, 240, GLHCK_TEXTURE_DEFAULTS);

  gameText = glhckTextNew(200, 200);
  fpsText = glhckTextNew(800, 40);

  gameFont = glhckTextNewFont(gameText, "fonts/DejaVuSans.ttf");
  fpsFont = glhckTextNewFont(fpsText, "fonts/DejaVuSans.ttf");
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
  glhckObjectRender(background);

  for(auto i : world.getSprites())
  {
    i->render();
  }

  world.getParticleEngine().render();

  std::ostringstream ss;
  ss << "Level: " << (world.level.n + 1)
      << " | Score: " << world.player.score
      << " | Lives: " << world.player.lives
      << " | Weapons:"
      << " L" << world.player.ship->getLaserLevel()
      << " S" << world.player.ship->getSpreadLevel()
      << " B" << world.player.ship->getBeamLevel()
      << " P" << world.player.ship->getPlasmaLevel();

  glhckTextDraw(gameText, gameFont, 20, 5, 20, ss.str().data(), NULL);

  ss.str("");
  ss << std::setprecision(2) << std::fixed
      << "FPS: " << engine->getTimer().getFPS()
      << " | total: " << engine->getTimer().getTotalTime()
      << "s | frame: " << engine->getTimer().getTicks();

  glhckTextDraw(fpsText, fpsFont, 14, 5, 35, ss.str().data(), NULL);

  glhckTextRender(gameText);
  glhckTextRender(fpsText);

}
