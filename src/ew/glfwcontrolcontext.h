#ifndef GLFWCONTROLCONTEXT_HH
#define GLFWCONTROLCONTEXT_HH

#include "controlcontext.h"
#include "GL/glfw3.h"

class GLFWControlContext : public ControlContext
{
public:
  GLFWControlContext(GLFWwindow* window) :
    ControlContext(), window(window)
  {
  }

  virtual bool key(int const key)
  {
    return glfwGetKey(window, key) == GLFW_PRESS;
  };

private:
  GLFWwindow* window;
};

#endif