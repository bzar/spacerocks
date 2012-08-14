#ifndef GLFWRENDERCONTEXT_HH
#define GLFWRENDERCONTEXT_HH

#include "rendercontext.h"
#include "GL/glfw3.h"

class GLFWRenderContext : public ew::RenderContext
{
public:
  virtual void postRender()
  {
    glfwSwapBuffers();
  }
};
#endif
