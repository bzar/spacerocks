#include "gamenotification.h"
#include "util/util.h"

void GameNotification::init()
{
}

void GameNotification::term()
{

}

GameNotification::GameNotification(GameWorld* world, const std::string& text,
                                   float size, float life, const Vec2D& position, const float endOpacity):
  ew::Entity(world), ew::Renderable(world, 0, 2), ew::Updatable(world),
  gameWorld(world), size(size), life(life), o(nullptr), time(0), endOpacity(endOpacity)
{
  float const renderMultiplier = 3;

  glhckText* textObject = glhckTextNew(512, 512);
  unsigned int font = glhckTextFontNew(textObject, "fonts/DejaVuSans.ttf");
  glhckTextColorb(textObject, 255, 255, 255, 255);
  o = glhckTextPlane(textObject, font, size*renderMultiplier, text.data(), nullptr);
  glhckMaterialBlendFunc(glhckObjectGetMaterial(o), GLHCK_SRC_ALPHA, GLHCK_ONE_MINUS_SRC_ALPHA);
  glhckObjectPositionf(o, position.x, position.y, 0);
  glhckTextFree(textObject);
}

GameNotification::~GameNotification()
{
  glhckObjectFree(o);
}

void GameNotification::render(ew::RenderContext* context)
{
  glhckObjectDraw(o);
}

void GameNotification::update(const float delta)
{
  time += delta;

  if(life > time)
  {
    float progress = time / life;
    float scale = lerp(1, 2, progress);
    glhckObjectScalef(o, scale, scale, 1);

    unsigned char opacity = lerp(255, static_cast<unsigned char>(endOpacity * 255), progress);
    glhckMaterialDiffuseb(glhckObjectGetMaterial(o), 255,255,255, opacity);
  }
  else
  {
    world->removeEntity(this);
  }
}

