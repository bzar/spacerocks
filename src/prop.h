#ifndef PROP_HH
#define PROP_HH

#include "ew/renderableworld.h"
#include "ew/renderable.h"

#include "glhck/glhck.h"

#include <map>
#include <string>

class Prop : public ew::Renderable
{
public:
  static void init();
  static void term();

  Prop(ew::RenderableWorld* world, std::string const& image, float const width, float const height, int const zIndex = 0, int const layer = 0);
  ~Prop();

  void render(ew::RenderContext* context);

private:
  glhckTexture* texture;
  glhckObject* o;
};

#endif
