#include "gamenotification.h"

ew::UID const GameNotification::ID = ew::getUID();

void GameNotification::init()
{
}

void GameNotification::term()
{

}

GameNotification::GameNotification(GameWorld* world, const std::string& text, GameNotification::Size size): 
  ew::Entity(world), ew::Renderable(world), ew::Updatable(world),
  size(size), o(nullptr), life(0)
{
  glhckText* textObject = glhckTextNew(200, size);
  unsigned int font = glhckTextNewFont(textObject, "fonts/DejaVuSans.ttf");
  glhckTextColor(textObject, 255, 255, 255, 255);
  glhckTextDraw(textObject, font, size, 0, 0, text.data(), NULL);
  
  glhckRtt* rtt = glhckRttNew(200, size, GLHCK_RTT_RGBA);
  glhckRttBegin(rtt);
  glhckTextRender(textObject);
  glhckRttFillData(rtt);
  glhckRttEnd(rtt);
  
  o = glhckSpriteNew(glhckRttGetTexture(rtt), 200, size);
  
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

}

