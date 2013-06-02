#ifndef TITLESPRITE_H
#define TITLESPRITE_H

#include "titleworld.h"
#include "ew/updatable.h"
#include "ew/renderable.h"
#include "glhck/glhck.h"
#include "util/vec2d.h"
#include "sound.h"

#include <string>

class TitleSprite : public ew::Renderable, public ew::Updatable
{
public:
  TitleSprite(TitleWorld* world, std::string const& image, std::string const& finishSound,
              Vec2D const& from, Vec2D const& to, float const start, float const finish,
              int const zIndex = 0, int const layer = 0);
  ~TitleSprite();
  void render(ew::RenderContext* context);
  void update(float const delta);

private:
  TitleWorld* world;
  glhckObject* o;
  Sound finishSound;
  Vec2D from;
  Vec2D to;
  float start;
  float finish;
  float now;
};

#endif // TITLESPRITE_H
