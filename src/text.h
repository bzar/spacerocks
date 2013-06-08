#ifndef TEXT_H
#define TEXT_H

#include "ew/renderableworld.h"
#include "ew/renderable.h"
#include "util/vec2d.h"
#include "glhck/glhck.h"

#include <string>

class Text : public ew::Renderable
{
public:
  Text(ew::RenderableWorld* world, std::string const& content, Vec2D const& centerIn, int zIndex = 0, int layer = 0);
  ~Text();

  void render(ew::RenderContext* context);

  void setContent(std::string const& newContent);
  void setCenterIn(Vec2D const& newCenterIn);
  void refresh();

private:
  glhckObject* o;
  glhckText* text;
  std::string content;
  Vec2D centerIn;
  int fontId;
  int fontSize;
};

#endif // TEXT_H
