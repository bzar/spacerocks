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

#include "asteroid.h"
#include "ship.h"
#include "laser.h"
#include "spark.h"
#include "explosion.h"
#include "ufo.h"
#include "ufolaser.h"

int const WIDTH = 800;
int const HEIGHT = 480;

struct Runtime {
  bool running;
};

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

  World world = {nullptr, SpriteSet()};

  for(auto d : asteroids)
  {
    std::shared_ptr<Asteroid> asteroid(new Asteroid(&world, d.s, d.p, d.v));
    world.sprites.insert(asteroid);
  }

  world.ship = new Ship(&world, {0, -200}, {0, 0});
  world.sprites.insert(std::shared_ptr<Ship>(world.ship));

  std::shared_ptr<Ufo> ufo(new Ufo(&world, {0, 0}));
  world.sprites.insert(ufo);

  glhckObject* background = glhckSpriteNewFromFile("img/background.png", 0, 0, GLHCK_TEXTURE_DEFAULTS);
  glhckObjectScalef(background, 0.5f, 0.5f, 0.5f);
  glhckObjectPositionf(background, 0, 0, -0.01);

  glhckText *text = glhckTextNew(800, 40);
  unsigned int font = glhckTextNewFont(text, "/usr/share/fonts/truetype/freefont/FreeSans.ttf");

  glhckMemoryGraph();

  Runtime runtime = { true };
  glfwSetWindowUserPointer(window, &runtime);
  glfwSetWindowSizeCallback(windowResizeCallback);

  Timer timer;
  float deathDelay = 0;

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
      deathDelay = 3.0;
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
       << "FPS: " << timer.getFPS()
       << ", total: " << timer.getTotalTime()
       << "s, frame: " << timer.getTicks();
    glhckTextDraw(text, font, 20, 5, 25, ss.str().data(), NULL);
    glhckTextRender(text);


    glfwSwapBuffers();
    glhckClear();

    timer.tick();
  }

  return EXIT_SUCCESS;
}
