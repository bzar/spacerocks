#include "GL/glfw3.h"
#include "GL/glhck.h"

#include <cstdlib>
#include <iostream>
#include <set>
#include <memory>

#include "asteroid.h"

int const WIDTH = 800;
int const HEIGHT = 480;

struct Runtime {
  bool running;
  double now;
  double total;
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
  
  glhckCameraRange(camera, 0.1f, 20.0f);
  kmVec3 cameraPos = { 0, 0, -10 };
  kmVec3 cameraRot = { 0, 0, 0 };
  glhckCameraPosition(camera, &cameraPos);
  glhckCameraTargetf(camera, cameraPos.x, cameraPos.y, cameraPos.z + 1);
  glhckCameraRotate(camera, &cameraRot);
  glhckCameraUpdate(camera);
  
  Runtime runtime = { true, glfwGetTime(), 0, 0 };
  glfwSetWindowUserPointer(window, &runtime);
  
  std::set<std::shared_ptr<Sprite>> sprites;
  
  sprites.insert(std::shared_ptr<Sprite>(new Asteroid(4, -4, 0)));
  //sprites.insert(std::shared_ptr<Sprite>(new Asteroid(3, -2, 0)));
  //sprites.insert(std::shared_ptr<Sprite>(new Asteroid(2, -1, 0)));
  sprites.insert(std::shared_ptr<Sprite>(new Asteroid(1, 0, 0)));
  //sprites.insert(std::shared_ptr<Sprite>(new Asteroid(2, 1, 0)));
  //sprites.insert(std::shared_ptr<Sprite>(new Asteroid(3, 2, 0)));
  //sprites.insert(std::shared_ptr<Sprite>(new Asteroid(4, 4, 0)));
  
 // glhckObject* background = glhckSpriteNew("img/background.png", 800, GLHCK_TEXTURE_DEFAULTS);
 // glhckObjectPositionf(background, 0, 0, 10);
  
  glhckMemoryGraph();

  while(runtime.running)
  {
    float delta = glfwGetTime() - runtime.now;
    runtime.now += delta;
    runtime.total += delta;
    
    glfwPollEvents();
    
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) 
    {
      runtime.running = false;
    }
    
    for(auto i : sprites)
    {
      i->update(delta);
    }
    
    //glhckObjectDraw(background);
    
    for(auto i : sprites)
    {
      i->render();
    }
    
    glhckRender();
    glfwSwapBuffers();
    glhckClear();
    
    ++runtime.frame;
  }
 
  return EXIT_SUCCESS;
}
