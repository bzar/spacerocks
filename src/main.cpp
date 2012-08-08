#include "GL/glfw3.h"
#include "GL/glhck.h"

#include "engine.h"
#include "gamestate.h"

#include <cstdlib>
#include <random>

int const WIDTH = 800;
int const HEIGHT = 480;

int windowCloseCallback(GLFWwindow window);
void windowResizeCallback(GLFWwindow window, int width, int height);
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
  Engine* engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));
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

  Engine engine(&window);

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
