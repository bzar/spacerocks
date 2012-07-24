#include "GL/glfw3.h"
#include "GL/glhck.h"
#include <cstdlib>

int const WIDTH = 800;
int const HEIGHT = 480;

int main(int argc, char** argv)
{
  if (!glfwInit())
    return EXIT_FAILURE;

  GLFWwindow window = 0;
  
  glfwOpenWindowHint(GLFW_DEPTH_BITS, 24);
  if (!(window = glfwOpenWindow(WIDTH, HEIGHT, GLFW_WINDOWED, "display test", NULL)))
    return EXIT_FAILURE;

  glfwSwapInterval(0);

  if (!glhckInit(argc, argv))
    return EXIT_FAILURE;

  if (!glhckDisplayCreate(WIDTH, HEIGHT, GLHCK_RENDER_OPENGL))
    return EXIT_FAILURE;
  
  while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
  {
    glfwPollEvents();
    glfwSwapBuffers();
    glhckClear();
  }
  
  return EXIT_SUCCESS;
}