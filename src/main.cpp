#include "GL/glfw3.h"
#include "GL/glhck.h"

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <set>
#include <memory>
#include <sstream>

#include "timer.h"
#include "asteroid.h"
#include "ship.h"
#include "laser.h"
#include "vec2d.h"

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

int gameloop(GLFWwindow& window)
{
  glhckCamera *camera = glhckCameraNew();
  
  if(!camera)
    return EXIT_FAILURE;
  
  glhckCameraRange(camera, 0.1f, 200.0f);
  glhckCameraPositionf(camera, 0, 0, 50);
  glhckCameraTargetf(camera, 0, 0, 0);
  glhckCameraRotatef(camera, 0, 180, 0);
  glhckCameraUpdate(camera);
  
  std::set<std::shared_ptr<Sprite>> sprites;
  std::set<std::shared_ptr<Particle>> particles;
  
  struct { Asteroid::Size s; Vec2D p; Vec2D v; } asteroids[] = {
    {Asteroid::LARGE,  {-20, 0}, {1, 0}},
    {Asteroid::MEDIUM, {-10, 0}, {0.5, 0.5}},
    {Asteroid::SMALL,  { -5, 0}, {0, 0}},
    {Asteroid::TINY,   {  0, 0}, {5, 2}},
    {Asteroid::SMALL,  {  5, 0}, {0, 0}},
    {Asteroid::MEDIUM, { 10, 0}, {-0.5, -0.5}},
    {Asteroid::LARGE,  { 20, 0}, {0, 1}},
    {Asteroid::LARGE,  {-20, 5}, {1, 0}},
    {Asteroid::MEDIUM, {-10, 5}, {0.5, 0.5}},
    {Asteroid::SMALL,  { -5, 5}, {0, 0}},
    {Asteroid::TINY,   {  0, 5}, {0, 0}},
    {Asteroid::SMALL,  {  5, 5}, {0, 0}},
    {Asteroid::MEDIUM, { 10, 5}, {-0.5, -0.5}},
    {Asteroid::LARGE,  { 20, 5}, {0, 1}}
  };
  
  for(auto d : asteroids)
  {
    sprites.insert(std::shared_ptr<Sprite>(new Asteroid(d.s, d.p, d.v)));
  }
  
  std::shared_ptr<Ship> ship(new Ship({0, 0}, {0, 0}));
  sprites.insert(ship);
  
  glhckObject* background = glhckSpriteNewFromFile("img/background.png", 2, GLHCK_TEXTURE_DEFAULTS);
  glhckObjectPositionf(background, 0, 0, -42);

  glhckText *text = glhckTextNew(800, 40);
  unsigned int font = glhckTextNewFont(text, "/usr/share/fonts/truetype/freefont/FreeSans.ttf");
  
  glhckMemoryGraph();

  Runtime runtime = { true };
  glfwSetWindowUserPointer(window, &runtime);

  double laserCooldown = 0;
  Timer timer;
  
  while(runtime.running)
  {
    glfwPollEvents();
    
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) 
    {
      runtime.running = false;
    }
    
    ship->turningLeft(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS);
    ship->turningRight(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS);
    ship->accelerating(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS);
    
    if(laserCooldown <= 0 && glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
      Vec2D v(0, 120);
      v.rotatei(ship->getAngle());
      Vec2D p = v.normal().uniti().scalei(0.7);
      std::shared_ptr<Laser> laser1(new Laser(0.25, ship->getPosition() + p, v));
      std::shared_ptr<Laser> laser2(new Laser(0.25, ship->getPosition() - p, v));
      sprites.insert(laser1);
      sprites.insert(laser2);
      particles.insert(laser1);
      particles.insert(laser2);
      laserCooldown = 0.15;
    }

    double delta = timer.getDeltaTime();
    laserCooldown = laserCooldown ? laserCooldown - delta : laserCooldown;
    
    for(auto i : sprites)
    {
      i->update(delta);
    }
    
    glhckObjectDraw(background);
    
    for(auto i : sprites)
    {
      i->render();
    }
    
    for(auto i : particles)
    {
      if(!i->alive())
      {
        sprites.erase(i);
        particles.erase(i);
      }
    }
    glhckRender();
    
    std::ostringstream ss;
    ss << std::setprecision(2) << std::fixed 
       << "FPS: " << timer.getFPS()
       << ", total: " << timer.getTotalTime()
       << "s, frame: " << timer.getTicks();
    glhckTextDraw(text, font, 20, 5, 465, ss.str().data(), NULL);
    glhckTextRender(text);
    
    
    glfwSwapBuffers();
    glhckClear();
    
    timer.tick();
  }
 
  return EXIT_SUCCESS;
}
