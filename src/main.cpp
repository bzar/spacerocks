#include "GL/glfw3.h"
#include "GL/glhck.h"

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <forward_list>

#include "timer.h"
#include "world.h"
#include "vec2d.h"
#include "util.h"

#include "asteroid.h"
#include "ship.h"
#include "laser.h"
#include "spark.h"
#include "explosion.h"
#include "ufo.h"
#include "ufolaser.h"

int const WIDTH = 800;
int const HEIGHT = 480;

int const UFO_SCORE_INTERVAL_MIN = 400;
int const UFO_SCORE_INTERVAL_MAX = 800;
float const DEATH_DELAY = 3.0f;
float const UFO_DELAY = 2.0f;

struct Runtime {
  bool running;
};

int getUfoInterval()
{
  return UFO_SCORE_INTERVAL_MIN + (rand() % (UFO_SCORE_INTERVAL_MAX - UFO_SCORE_INTERVAL_MIN));
}

static int windowCloseCallback(GLFWwindow window);
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


static int windowCloseCallback(GLFWwindow window)
{
  Runtime* runtime = static_cast<Runtime*>(glfwGetWindowUserPointer(window));
  runtime->running = false;
  return GL_FALSE;
}

static void windowResizeCallback(GLFWwindow window, int width, int height)
{
  glhckDisplayResize(width, height);

  kmMat4 proj;
  kmMat4Scaling(&proj, 2.0f/WIDTH, 2.0f/HEIGHT, 0);
  glhckRenderSetProjection(&proj);
}

int gameloop(GLFWwindow& window)
{
  kmMat4 proj;
  kmMat4Scaling(&proj, 2.0f/WIDTH, 2.0f/HEIGHT, 0);
  glhckRenderSetProjection(&proj);

  Ship::init();
  Asteroid::init();
  Laser::init();
  Explosion::init();
  Spark::init();
  Ufo::init();
  UfoLaser::init();

  struct { Asteroid::Size s; Vec2D p; Vec2D v; } asteroids[] = {
    {Asteroid::LARGE,  {-200, 0}, {10, 0}},
    {Asteroid::MEDIUM, {-100, 0}, {5, 5}},
    {Asteroid::SMALL,  { -50, 0}, {0, 0}},
    {Asteroid::TINY,   {  0, 0}, {50, 20}},
    {Asteroid::SMALL,  {  50, 0}, {0, 0}},
    {Asteroid::MEDIUM, { 100, 0}, {-5, -5}},
    {Asteroid::LARGE,  { 200, 0}, {0, 10}},
    {Asteroid::LARGE,  {-200, 50}, {10, 0}},
    {Asteroid::MEDIUM, {-100, 50}, {5, 5}},
    {Asteroid::SMALL,  { -50, 50}, {0, 0}},
    {Asteroid::TINY,   {  0, 50}, {0, 0}},
    {Asteroid::SMALL,  {  50, 50}, {0, 0}},
    {Asteroid::MEDIUM, { 100, 50}, {-5, -5}},
    {Asteroid::LARGE,  { 200, 50}, {0, 1}}
  };

  World world = {nullptr, 0, SpriteSet()};

  for(auto d : asteroids)
  {
    std::shared_ptr<Asteroid> asteroid(new Asteroid(&world, d.s, d.p, d.v));
    world.sprites.insert(asteroid);
  }

  world.ship = new Ship(&world, {0, -200}, {0, 0});
  world.sprites.insert(std::shared_ptr<Ship>(world.ship));

  glhckObject* background = glhckSpriteNewFromFile("img/background.png", 0, 0, GLHCK_TEXTURE_DEFAULTS);
  glhckObjectScalef(background, 0.5f, 0.5f, 0.5f);
  glhckObjectPositionf(background, 0, 0, -0.01);

  glhckText *text = glhckTextNew(800, 40);
  unsigned int font = glhckTextNewFont(text, "fonts/DejaVuSans.ttf");

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
    glfwPollEvents();

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
      runtime.running = false;
    }

    double delta = timer.getDeltaTime();

    if(world.ship != nullptr)
    {
      world.ship->turnLeft(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS);
      world.ship->turnRight(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS);
      world.ship->accelerate(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS);
      world.ship->shoot(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS);
    }
    else
    {
      deathDelay -= delta;
      if(deathDelay <= 0)
      {
        world.ship = new Ship(&world, {0, 0}, {0, 0});
        world.sprites.insert(std::shared_ptr<Ship>(world.ship));
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
                                         randFloat(0, 5), randFloat(10, 100), 15.0f));
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

    std::forward_list<std::shared_ptr<Sprite>> deadParticles;
    for(auto i : world.sprites)
    {
      if(!i->alive())
      {
        deadParticles.push_front(i);
      }
    }

    for(auto i : deadParticles)
    {
      world.sprites.erase(i);
    }

    glhckObjectRender(background);

    for(auto i : world.sprites)
    {
      i->render();
    }

    std::ostringstream ss;
    ss << std::setprecision(2) << std::fixed
       << "Score: " << world.score
       << " | FPS: " << timer.getFPS()
       << " | total: " << timer.getTotalTime()
       << "s | frame: " << timer.getTicks();
    glhckTextDraw(text, font, 20, 5, 25, ss.str().data(), NULL);
    glhckTextRender(text);


    glfwSwapBuffers();
    glhckClear();

    timer.tick();
  }

  return EXIT_SUCCESS;
}
