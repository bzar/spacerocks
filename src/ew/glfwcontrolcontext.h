#ifndef GLFWCONTROLCONTEXT_HH
#define GLFWCONTROLCONTEXT_HH

#include "controlcontext.h"
#include "GL/glfw3.h"
#include <iostream>

class GLFWControlContext : public ControlContext
{
public:
  GLFWControlContext(GLFWwindow* window) :
    ControlContext(), window(window)
  {
  }

  virtual bool keyDown(int const key)
  {
    return glfwGetKey(*window, key) == GLFW_PRESS;
  }

  virtual void update()
  {
    glfwPollEvents();
  }
private:
  GLFWwindow* window;
};

#endif