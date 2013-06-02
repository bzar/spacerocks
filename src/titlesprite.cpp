#include "titlesprite.h"

TitleSprite::TitleSprite(TitleWorld *world, const std::string &image, const std::string &finishSound,
                         const Vec2D &from, const Vec2D &to, const float start, const float finish,
                         int const zIndex, int const layer) :
  ew::Entity(world), ew::Renderable(world, zIndex, layer), ew::Updatable(world),
  world(world), o(glhckSpriteNewFromFile(image.data(), 0, 0, nullptr, nullptr)), finishSound(finishSound),
  from(from), to(to), start(start), finish(finish), now(0)
{
}

TitleSprite::~TitleSprite()
{
  glhckObjectFree(o);
}

void TitleSprite::render(ew::RenderContext *context)
{
  glhckObjectRender(o);
}

void TitleSprite::update(const float delta)
{
  now += delta;
  float progress = max(0, min(1, (now - start) / (finish - start)));
  Vec2D position = from + (to - from) * progress;
  glhckObjectPositionf(o, position.x, position.y, 0);

  if(now - delta < finish && now >= finish)
  {
    finishSound.play();
  }
}

