#include "GL/glfw3.h"
#include "glhck/glhck.h"

#include "ew/engine.h"
#include "ew/integration/glfwcontrolcontext.h"
#include "ew/integration/glfwtimecontext.h"
#include "ew/integration/glhckglfwrendercontext.h"
#include "gamestate.h"
#include "sound.h"

#include <cstdlib>
#include <random>
#include <iostream>

int const WIDTH = 800;
int const HEIGHT = 480;

int windowCloseCallback(GLFWwindow window);
void windowResizeCallback(GLFWwindow window, int width, int height);
int gameloop(GLFWwindow& window);

int main(int argc, char** argv)
{
  if (!glfwInit())
    return EXIT_FAILURE;

  glfwWindowHint(GLFW_DEPTH_BITS, 24);
  GLFWwindow window = glfwCreateWindow(WIDTH, HEIGHT, GLFW_WINDOWED, "Space Rocks!", NULL);
  glfwMakeContextCurrent(window);

  if(!window)
  {
    std::cerr << "GLFW error: " << glfwErrorString(glfwGetError()) << std::endl;
    return EXIT_FAILURE;
  }

  glfwSwapInterval(1);
  glfwSetWindowCloseCallback(windowCloseCallback);

  if(!glhckInit(argc, argv))
  {
    std::cerr << "GLHCK initialization error" << std::endl;
    return EXIT_FAILURE;
  }

  glhckSetGlobalPrecision(GLHCK_INDEX_BYTE, GLHCK_VERTEX_V2F);
  
  if(!glhckDisplayCreate(WIDTH, HEIGHT, GLHCK_RENDER_OPENGL))
  {
    std::cerr << "GLHCK display create error" << std::endl;
    return EXIT_FAILURE;
  }

  Sound::init();
  
  int retval = gameloop(window);

  Sound::exit();
  
  glhckTerminate();
  glfwTerminate();

  return retval;
}


int windowCloseCallback(GLFWwindow window)
{
  ew::Engine* engine = static_cast<ew::Engine*>(glfwGetWindowUserPointer(window));
  engine->quit();
  return GL_FALSE;
}

void windowResizeCallback(GLFWwindow window, int width, int height)
{
  glhckDisplayResize(width, height);

  kmMat4 proj;
  kmMat4Scaling(&proj, 2.0f/WIDTH, 2.0f/HEIGHT, 0);
  glhckRenderSetProjection(&proj);
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

  GLFWControlContext controlContext(&window);
  GlhckGLFWRenderContext renderContext(window);
  GLFWTimeContext timeContext;
  ew::Engine engine(&controlContext, &renderContext, &timeContext);

  glfwSetWindowUserPointer(window, &engine);
  glfwSetWindowSizeCallback(windowResizeCallback);

  GameState::init();
  GameState game(&engine);
  engine.addState(0, &game);
  engine.setState(0);

  glhckMemoryGraph();

  engine.run();

  GameState::term();

  return EXIT_SUCCESS;
}
