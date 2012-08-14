#ifndef PROP_HH
#define PROP_HH

#include "gameworld.h"
#include "ew/renderable.h"

#include "GL/glhck.h"

#include <map>
#include <string>

class Prop : public ew::Renderable
{
public:
  static void init();
  static void term();

  Prop(GameWorld* world, std::string const& image, float const width, float const height, int const zIndex = 0, int const layer = 0);
  ~Prop();

  static ew::UID const ID;
  ew::UID getEntityId() const { return ID; }

  void render(ew::RenderContext* context);

private:
  glhckTexture* texture;
  glhckObject* o;
};

#endif
