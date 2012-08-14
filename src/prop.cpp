#include "prop.h"

ew::UID const Prop::ID = ew::getUID();

Prop::Prop(GameWorld* world, std::string const& image, float const width, float const height, int const zIndex, int const layer) :
  ew::Entity(world), ew::Renderable(world, zIndex, layer), o(0)
{
  o = glhckSpriteNewFromFile(image.data(), width, height, GLHCK_TEXTURE_DEFAULTS);
}

Prop::~Prop()
{
  glhckObjectFree(o);
}

void Prop::render(ew::RenderContext* context)
{
  glhckObjectRender(o);
}
