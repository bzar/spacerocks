#include "text.h"

Text::Text(ew::RenderableWorld *world, const std::string &content, Vec2D const& centerIn, int zIndex, int layer) :
  ew::Entity(world), ew::Renderable(world, zIndex, layer), text(nullptr), content(content), centerIn(centerIn), fontId(-1), fontSize(40)
{
  text = glhckTextNew(512, 512);
  glhckTextColorb(text, 255, 255, 255, 255);
  fontId = glhckTextFontNew(text, "fonts/DejaVuSans.ttf");
  refresh();
}

void Text::render(ew::RenderContext *context)
{
  glhckTextRender(text);
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
  glhckTextClear(text);
  kmVec2 minv, maxv;
  glhckTextGetMinMax(text, fontId, fontSize, content.data(), &minv, &maxv);
  glhckTextStash(text, fontId, fontSize, centerIn.x - maxv.x/2, centerIn.y - maxv.y/2, content.data(), nullptr);
}
