#include "gamenotification.h"
#include "util/util.h"

void GameNotification::init()
{
}

void GameNotification::term()
{

}

GameNotification::GameNotification(GameWorld* world, const std::string& text,
                                   float size, float life, const Vec2D& position):
  ew::Entity(world), ew::Renderable(world, 0, 2), ew::Updatable(world),
  gameWorld(world), size(size), life(life), o(nullptr), time(0)
{
  float const renderMultiplier = 3;

  glhckText* textObject = glhckTextNew(512, 512);
  unsigned int font = glhckTextFontNew(textObject, "fonts/DejaVuSans.ttf");
  glhckTextColorb(textObject, 255, 255, 255, 255);
  o = glhckTextPlane(textObject, font, size*renderMultiplier, text.data(), nullptr);
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
  if(gameWorld->getPaused())
    return;

  time += delta;

  if(life > time)
  {
    float progress = time / life;
    float scale = lerp(1, 2, progress);
    glhckObjectScalef(o, scale, scale, 1);

    unsigned char opacity = lerp(255, 128, progress);
    glhckMaterial* m = glhckObjectGetMaterial(o);
    if (m) glhckMaterialDiffuseb(m, 255,255,255, opacity);
  }
  else
  {
    world->removeEntity(this);
  }
}

