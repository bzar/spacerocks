#ifndef TITLESTART_H
#define TITLESTART_H

#include "titleworld.h"
#include "ew/updatable.h"
#include "ew/renderable.h"
#include "glhck/glhck.h"
#include "util/vec2d.h"
#include <string>

class TitleStart : public ew::Renderable, public ew::Updatable
{
public:
  TitleStart(TitleWorld* world, std::string const& image, Vec2D const& position, float const start, float const interval, int const zIndex = 0, int const layer = 0);
  ~TitleStart();
  void render(ew::RenderContext* context);
  void update(float const delta);

  void reset();

private:
  TitleWorld* world;
  glhckObject* o;
  Vec2D position;
  float start;
  float interval;
  float now;
};

#endif // TITLESPRITE_H
