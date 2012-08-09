#include "prop.h"

Entity::Id const Prop::ID = Entity::newEntityId();

Prop::Prop(GameWorld* world, std::string const& image, float const width, float const height, int const zIndex, int const layer) :
  Entity(world), Renderable(world, zIndex, layer), o(0)
{
  o = glhckSpriteNewFromFile(image.data(), width, height, GLHCK_TEXTURE_DEFAULTS);
}

Prop::~Prop()
{
  glhckObjectFree(o);
}

void Prop::render()
{
  glhckObjectRender(o);
}
