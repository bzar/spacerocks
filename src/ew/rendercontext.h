#ifndef RENDERCONTEXT_HH
#define RENDERCONTEXT_HH

class RenderContext
{
public:
  virtual void preRender() {}
  virtual void postRender() {}
  virtual void waitUntilNextFrame() {}
};
#endif
