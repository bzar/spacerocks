#ifndef GLFWTIMECONTEXT_HH
#define GLFWTIMECONTEXT_HH

#include "timecontext.h"
#include "GL/glfw3.h"

class GLFWTimeContext : public ew::TimeContext
{
public:
  virtual double timeInSeconds()
  {
    return glfwGetTime();
  }
};
#endif
