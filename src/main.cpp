#include "GL/glfw3.h"
#include "GL/glhck.h"

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <forward_list>
#include <random>

#include "timer.h"
#include "world.h"
#include "vec2d.h"
#include "util.h"

#include "asteroid.h"
#include "ship.h"
#include "laser.h"
#include "shot.h"
#include "plasma.h"
#include "spark.h"
#include "explosion.h"
#include "ufo.h"
#include "ufolaser.h"
#include "powerup.h"

int const WIDTH = 800;
int const HEIGHT = 480;

int const UPDATE_ITERATIONS = 10;

int const UFO_SCORE_INTERVAL_MIN = 400;
int const UFO_SCORE_INTERVAL_MAX = 800;
float const DEATH_DELAY = 3.0f;
float const UFO_DELAY = 2.0f;

struct Runtime {
  bool running;
};

int windowCloseCallback(GLFWwindow window);
void windowResizeCallback(GLFWwindow window, int width, int height);
int getUfoInterval();
void initLevel(World* world, int n);
int gameloop(GLFWwindow& window);

int main(int argc, char** argv)
{
  if (!glfwInit())
    return EXIT_FAILURE;

  glfwOpenWindowHint(GLFW_DEPTH_BITS, 24);
  GLFWwindow window = glfwOpenWindow(WIDTH, HEIGHT, GLFW_WINDOWED, "Space Rocks!", NULL);

  if(!window)
    return EXIT_FAILURE;

  glfwSwapInterval(1);
  glfwSetWindowCloseCallback(windowCloseCallback);

  if(!glhckInit(argc, argv))
    return EXIT_FAILURE;

  if(!glhckDisplayCreate(WIDTH, HEIGHT, GLHCK_RENDER_AUTO))
    return EXIT_FAILURE;

  int retval = gameloop(window);

  glhckTerminate();
  glfwTerminate();

  return retval;
}


int windowCloseCallback(GLFWwindow window)
{
  Runtime* runtime = static_cast<Runtime*>(glfwGetWindowUserPointer(window));
  runtime->running = false;
  return GL_FALSE;
}

void windowResizeCallback(GLFWwindow window, int width, int height)
{
  glhckDisplayResize(width, height);

  kmMat4 proj;
  kmMat4Scaling(&proj, 2.0f/WIDTH, 2.0f/HEIGHT, 0);
  glhckRenderSetProjection(&proj);
}

int getUfoInterval()
{
  return UFO_SCORE_INTERVAL_MIN + (rand() % (UFO_SCORE_INTERVAL_MAX - UFO_SCORE_INTERVAL_MIN));
}

void initLevel(World& world)
{
  int const ASTEROID_VALUES[Asteroid::NUM_SIZES] = {
    1,
    2 + 2*1,
    3 + 2*2 + 4*1,
    4 + 2*3 + 4*2 + 8*1,
  };

  world.level.minAsteroidSpeed = lerp(10, 20, world.level.n/40.0);
  world.level.maxAsteroidSpeed = lerp(20, 60, world.level.n/40.0);
  world.level.ufoDuration = lerp(20, 10, world.level.n/40.0);
  world.level.ufoAccuracy = lerp(0.6, 0.9, world.level.n/60.0);
  world.level.ufoShootInterval = lerp(3.0, 1.5, world.level.n/60.0);

  int levelsPerNextSize = 4;
  float minAsteroidDistance = 100;
  float maxAsteroidDistance = 200;
  int maxAsteroidSize = Asteroid::NUM_SIZES - 1;
  int minAsteroidSize = maxAsteroidSize - world.level.n / levelsPerNextSize;
  minAsteroidSize = minAsteroidSize < 0 ? 0 : minAsteroidSize;
  int asteroidValue = (world.level.n + 2) * ASTEROID_VALUES[maxAsteroidSize];

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
      .scalei(randFloat(world.level.minAsteroidSpeed, world.level.maxAsteroidSpeed));
    Asteroid* asteroid = new Asteroid(&world, size, position, velocity);
    world.sprites.insert(std::shared_ptr<Asteroid>(asteroid));
  }
}

int gameloop(GLFWwindow& window)
{
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<unsigned> seedGenerator;
  srand(seedGenerator(gen));

  kmMat4 proj;
  kmMat4Scaling(&proj, 2.0f/WIDTH, 2.0f/HEIGHT, 0);
  glhckRenderSetProjection(&proj);

  Ship::init();
  Asteroid::init();
  Laser::init();
  Shot::init();
  Plasma::init();
  Explosion::init();
  Spark::init();
  Ufo::init();
  UfoLaser::init();
  Powerup::init();

  World world = newWorld();
  world.player.lives = 3;
  world.player.weapon[Ship::RAPID] = 1;
  initLevel(world);

  world.ship = new Ship(&world, {0, 0}, {0, 0});
  world.sprites.insert(std::shared_ptr<Ship>(world.ship));

  glhckObject* background = glhckSpriteNewFromFile("img/background.png", 0, 0, GLHCK_TEXTURE_DEFAULTS);
  glhckObjectScalef(background, 0.5f, 0.5f, 0.5f);
  glhckObjectPositionf(background, 0, 0, -0.01);

  glhckText *gameText = glhckTextNew(200, 200);
  glhckText *fpsText = glhckTextNew(800, 40);
  unsigned int font = glhckTextNewFont(gameText, "fonts/DejaVuSans.ttf");
  unsigned int font2 = glhckTextNewFont(fpsText, "fonts/DejaVuSans.ttf");

  glhckMemoryGraph();

  Runtime runtime = { true };
  glfwSetWindowUserPointer(window, &runtime);
  glfwSetWindowSizeCallback(windowResizeCallback);

  Timer timer;
  float deathDelay = 0;
  float ufoDelay = 0;
  int nextUfoScore = getUfoInterval();

  while(runtime.running)
  {
    double totalDelta = timer.getDeltaTime();

    for(int iteration = 0; iteration < UPDATE_ITERATIONS; ++iteration)
    {
      double delta = totalDelta / UPDATE_ITERATIONS;

      bool lastPrevWeaponButtonState = glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS;
      bool lastNextWeaponButtonState = glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS;
      bool lastF1 = glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS;
      bool lastF2 = glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS;
      bool lastF3 = glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS;
      bool lastF4 = glfwGetKey(window, GLFW_KEY_F4) == GLFW_PRESS;

      glfwPollEvents();

      if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      {
        runtime.running = false;
      }

      if(world.ship != nullptr)
      {
        world.ship->turnLeft(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS);
        world.ship->turnRight(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS);
        world.ship->accelerate(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS);
        world.ship->shoot(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS);

        if(glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS && !lastPrevWeaponButtonState)
          world.ship->prevWeapon();

        if(glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS && !lastNextWeaponButtonState)
          world.ship->nextWeapon();

        if(glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS && !lastF1)
          world.player.weapon[Ship::RAPID] += world.player.weapon[Ship::RAPID] < 8 ? 1 : 0;
        if(glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS && !lastF2)
          world.player.weapon[Ship::SPREAD] += world.player.weapon[Ship::SPREAD] < 8 ? 1 : 0;
        if(glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS && !lastF3)
          world.player.weapon[Ship::CONTINUOUS] += world.player.weapon[Ship::CONTINUOUS] < 8 ? 1 : 0;
        if(glfwGetKey(window, GLFW_KEY_F4) == GLFW_PRESS && !lastF4)
          world.player.weapon[Ship::PLASMA] += world.player.weapon[Ship::PLASMA] < 8 ? 1 : 0;

      }
      else
      {
        deathDelay -= delta;
        if(deathDelay <= 0)
        {
          if(world.player.lives > 0)
          {
            world.player.lives -= 1;

            world.ship = new Ship(&world, {0, 0}, {0, 0});
            world.sprites.insert(std::shared_ptr<Ship>(world.ship));

            for(int i = 0; i < Ship::NUM_WEAPONS; ++i)
            {
              world.player.weapon[i] -= world.player.weapon[i] > 1 ? 1 : 0;
            }
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
          std::shared_ptr<Ufo> ufo(new Ufo(&world, position, position.neg(),
                                          randFloat(0, 5), randFloat(10, 100)));
          world.sprites.insert(ufo);
        }
      }

      if(world.score >= nextUfoScore)
      {
        nextUfoScore += getUfoInterval();
        ufoDelay = UFO_DELAY;
      }

      for(auto i : world.sprites)
      {
        i->update(delta);
      }

      for(auto i : world.sprites)
      {
        for(auto j : world.sprites)
        {
          if(i != j)
          {
            i->collide(j.get());
          }
        }
      }

      if(world.ship != nullptr && !world.ship->alive())
      {
        world.ship = nullptr;
        deathDelay = DEATH_DELAY;
      }

      bool victory = ufoDelay <= 0 && world.ship != nullptr;

      std::forward_list<std::shared_ptr<Sprite>> deadSprites;
      for(auto i : world.sprites)
      {
        if(!i->alive())
        {
          deadSprites.push_front(i);
        }

        victory = victory
          && i->getEntityId() != Asteroid::ID
          && i->getEntityId() != Ufo::ID
          && i->getEntityId() != UfoLaser::ID;
      }

      for(auto i : deadSprites)
      {
        world.sprites.erase(i);
      }

      if(victory)
      {
        world.level.n += 1;
        world.ship->reset();
        initLevel(world);
      }
    }

    glhckObjectRender(background);

    for(auto i : world.sprites)
    {
      i->render();
    }

    Ship::Weapon w = world.ship != nullptr ? world.ship->getWeapon() : Ship::NUM_WEAPONS;
    std::ostringstream ss;
    ss << "Level: " << (world.level.n + 1)
       << " | Score: " << world.score
       << " | Lives: " << world.player.lives
       << " | Weapons: "
       << (w == Ship::RAPID ? "[R" : "R") << world.player.weapon[Ship::RAPID] << (w == Ship::RAPID ? "] " : " ")
       << (w == Ship::SPREAD ? "[S" : "S") << world.player.weapon[Ship::SPREAD] << (w == Ship::SPREAD ? "] " : " ")
       << (w == Ship::CONTINUOUS ? "[C" : "C") << world.player.weapon[Ship::CONTINUOUS] << (w == Ship::CONTINUOUS ? "] " : " ")
       << (w == Ship::PLASMA ? "[P" : "P") << world.player.weapon[Ship::PLASMA] << (w == Ship::PLASMA ? "] " : " ");

    glhckTextDraw(gameText, font, 20, 5, 20, ss.str().data(), NULL);

    ss.str("");
    ss << std::setprecision(2) << std::fixed
       << "FPS: " << timer.getFPS()
       << " | total: " << timer.getTotalTime()
       << "s | frame: " << timer.getTicks();

    glhckTextDraw(fpsText, font, 14, 5, 35, ss.str().data(), NULL);

    glhckTextRender(gameText);
    glhckTextRender(fpsText);

    glfwSwapBuffers();
    glhckClear();

    timer.tick();
  }

  return EXIT_SUCCESS;
}
