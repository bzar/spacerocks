#include "titlestart.h"

TitleStart::TitleStart(TitleWorld *world, const std::string &image, const Vec2D &position, const float start, const float interval, int const zIndex, int const layer) :
  ew::Entity(world), ew::Renderable(world, zIndex, layer), ew::Updatable(world),
  world(world), o(glhckSpriteNewFromFile(image.data(), 0, 0, nullptr, nullptr)), position(position), start(start), interval(interval), now(0)
{
  glhckObjectPositionf(o, position.x, position.y, 0);
}

TitleStart::~TitleStart()
{
  glhckObjectFree(o);
}

void TitleStart::render(ew::RenderContext *context)
{
  glhckObjectRender(o);
}

void TitleStart::update(const float delta)
{
  now += delta;
  bool visible = false;
  if(now >= start)
  {
    visible = static_cast<int>((now - start) / interval) % 2 == 0;
  }

  glhckMaterialDiffuseb(glhckObjectGetMaterial(o), 255, 255, 255, visible ? 255 : 0);
}

