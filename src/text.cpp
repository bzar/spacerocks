#include "text.h"

Text::Text(ew::RenderableWorld *world, const std::string &content, Vec2D const& centerIn, int zIndex, int layer) :
  ew::Entity(world), ew::Renderable(world, zIndex, layer),
  o(nullptr), text(nullptr), content(content), centerIn(centerIn), fontId(-1), fontSize(40),
  visible(true)
{
  text = glhckTextNew(512, 512);
  glhckTextColorb(text, 255, 255, 255, 255);
  fontId = glhckTextFontNew(text, "fonts/DejaVuSans.ttf");
  refresh();
}

Text::~Text()
{
  glhckObjectFree(o);
  glhckTextFree(text);
}

void Text::render(ew::RenderContext *context)
{
  if(visible)
    glhckObjectRender(o);
}

void Text::setContent(const std::string &newContent)
{
  content = newContent;
}

void Text::setCenterIn(const Vec2D &newCenterIn)
{
  centerIn = newCenterIn;
}

void Text::refresh()
{
  if(o != nullptr)
    glhckObjectFree(o);

  o = glhckTextPlane(text, fontId, fontSize, content.data(), nullptr);
  glhckTextClear(text);
  glhckObjectPositionf(o, centerIn.x, centerIn.y, 0);
}

bool Text::getVisible() const
{
  return visible;
}

void Text::setVisible(bool value)
{
  visible = value;
}
