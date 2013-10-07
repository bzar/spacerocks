#include "text.h"

glhckText* Text::text = nullptr;
int Text::fontId = -1;

Text::Text(ew::RenderableWorld *world, const std::string &content, Vec2D const& centerIn, int zIndex, int layer) :
  ew::Entity(world), ew::Renderable(world, zIndex, layer),
  o(nullptr), content(content), centerIn(centerIn), fontSize(40),
  visible(true), opacity(1)
{
  if(text == nullptr)
  {
    text = glhckTextNew(512, 512);
    glhckTextColorb(text, 255, 255, 255, 255);
    fontId = glhckTextFontNew(text, "fonts/DejaVuSans.ttf");
  }
  refresh();
}

Text::~Text()
{
  glhckObjectFree(o);
}

void Text::render(ew::RenderContext *context)
{
  if(visible)
  {
    glhckMaterialDiffuseb(glhckObjectGetMaterial(o), 255, 255, 255, 255 * opacity);
    glhckObjectRender(o);
  }
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
  glhckMaterialBlendFunc(glhckObjectGetMaterial(o), GLHCK_SRC_ALPHA, GLHCK_ONE_MINUS_SRC_ALPHA);
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

float Text::getOpacity() const
{
    return opacity;
}

void Text::setOpacity(float value)
{
    opacity = value;
}
