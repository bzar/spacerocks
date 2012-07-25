#include "GL/glfw3.h"
#include "GL/glhck.h"

#include <cstdlib>
#include <iostream>
#include <set>
#include <memory>
#include <sstream>

#include "asteroid.h"

int const WIDTH = 800;
int const HEIGHT = 480;

struct Runtime {
  bool running;
  double now;
  double start;
  unsigned int frame;
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

  if(!glhckDisplayCreate(WIDTH, HEIGHT, GLHCK_RENDER_OPENGL))
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
  kmVec3 cameraPos = { 0, 0, 50 };
  kmVec3 cameraRot = { 0, 180, 0 };
  glhckCameraPosition(camera, &cameraPos);
  glhckCameraTargetf(camera, cameraPos.x, cameraPos.y, cameraPos.z + 1);
  glhckCameraRotate(camera, &cameraRot);
  glhckCameraUpdate(camera);
  
  Runtime runtime = { true, glfwGetTime(), glfwGetTime(), 0 };
  glfwSetWindowUserPointer(window, &runtime);
  
  std::set<std::shared_ptr<Sprite>> sprites;
  
  
  struct { Asteroid::Size s; km::vec2 p; km::vec2 v; } asteroids[] = {
    {Asteroid::LARGE,  {-20, 0}, {1, 0}},
    {Asteroid::MEDIUM, {-10, 0}, {0, 0}},
    {Asteroid::SMALL,  { -5, 0}, {0, 0}},
    {Asteroid::TINY,   {  0, 0}, {0, 0}},
    {Asteroid::SMALL,  {  5, 0}, {0, 0}},
    {Asteroid::MEDIUM, { 10, 0}, {0, 0}},
    {Asteroid::LARGE,  { 20, 0}, {0, 1}}
  };
  
  for(auto d : asteroids)
  {
    sprites.insert(std::shared_ptr<Sprite>(new Asteroid(d.s, d.p, d.v)));
  }
  
  glhckObject* background = glhckSpriteNew("img/background.png", 2, GLHCK_TEXTURE_DEFAULTS);
  glhckObjectPositionf(background, 0, 0, -42);

  glhckText *text = glhckTextNew(800, 40);
  unsigned int font = glhckTextNewFont(text, "/usr/share/fonts/truetype/freefont/FreeSans.ttf");
  
  glhckMemoryGraph();

  while(runtime.running)
  {
    
    float delta = glfwGetTime() - runtime.now;
    runtime.now += delta;
    
    
    glfwPollEvents();
    
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) 
    {
      runtime.running = false;
    }
    
    for(auto i : sprites)
    {
      i->update(delta);
    }
    
    glhckObjectDraw(background);
    
    for(auto i : sprites)
    {
      i->render();
    }
    
    glhckRender();
    
    std::ostringstream ss;
    ss << "total: " << static_cast<int>((runtime.now - runtime.start) * 1000) 
       << "ms, frame: " << runtime.frame;
    glhckTextDraw(text, font, 20, 5, 465, ss.str().data(), NULL);
    glhckTextRender(text);
    
    
    glfwSwapBuffers();
    glhckClear();
    
    ++runtime.frame;
  }
 
  return EXIT_SUCCESS;
}
