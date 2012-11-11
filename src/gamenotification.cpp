#include "gamenotification.h"
#include "util/util.h"

ew::UID const GameNotification::ID = ew::getUID();

void GameNotification::init()
{
}

void GameNotification::term()
{

}

GameNotification::GameNotification(GameWorld* world, const std::string& text, 
                                   float size, float life, const Vec2D& position): 
  ew::Entity(world), ew::Renderable(world, 0, 2), ew::Updatable(world),
  size(size), life(life), o(nullptr), time(0)
{
  float width = 0;
  float height = size * 1.5f;
  
  glhckText* textObject = glhckTextNew(800, height);
  unsigned int font = glhckTextNewFont(textObject, "fonts/DejaVuSans.ttf");
  glhckTextColor(textObject, 255, 255, 255, 255);
  glhckTextDraw(textObject, font, size, 0, size, text.data(), &width);

  glhckText* shadowObject = glhckTextNew(800, height);
  unsigned int font2 = glhckTextNewFont(shadowObject, "fonts/DejaVuSans.ttf");
  glhckTextColor(shadowObject, 0, 0, 0, 255);
  glhckTextDraw(shadowObject, font2, size - 1, 1, size, text.data(), nullptr);
  
  glhckRtt* rtt = glhckRttNew(width, height, GLHCK_RTT_RGBA);
  glhckRttBegin(rtt);
  glhckClear();
  glhckTextRender(shadowObject);
  glhckTextRender(textObject);
  glhckRttFillData(rtt);
  glhckRttEnd(rtt);
  
  o = glhckSpriteNew(glhckRttGetTexture(rtt), width, height);
  glhckObjectMaterialFlags(o, GLHCK_MATERIAL_ALPHA);
  glhckObjectPositionf(o, position.x, position.y, 0);
  
  glhckRttFree(rtt);
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
    
    unsigned char opacity = lerp(255, 128, progress*progress);
    //glhckObjectColorb(o, 255,255,255, opacity);
  }
  else
  {
    world->removeEntity(this);
  }
}

