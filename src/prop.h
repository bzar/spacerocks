#ifndef PROP_HH
#define PROP_HH

#include "gameworld.h"
#include "ew/renderable.h"

#include "GL/glhck.h"

#include <map>
#include <string>

class Prop : public Renderable
{
public:
  static void init();
  static void term();

  Prop(GameWorld* world, std::string const& image, float const width, float const height, int const zIndex = 0, int const layer = 0);
  ~Prop();

  static UID const ID;
  UID getEntityId() const { return ID; }

  void render();

private:
  glhckTexture* texture;
  glhckObject* o;
};

#endif
