#ifndef RENDERCONTEXT_HH
#define RENDERCONTEXT_HH

namespace ew
{
  class RenderContext
  {
  public:
    virtual void preRender() {}
    virtual void postRender() {}
    virtual void waitUntilNextFrame() {}
  };
};
#endif
