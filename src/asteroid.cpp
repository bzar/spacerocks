#include "asteroid.h"

std::map<Asteroid::Size, std::string> const Asteroid::IMAGES = {
  {Asteroid::TINY, "img/asteroid_1.png"},
  {Asteroid::SMALL, "img/asteroid_2.png"},
  {Asteroid::MEDIUM, "img/asteroid_3.png"},
  {Asteroid::LARGE, "img/asteroid_4.png"}
};

Asteroid::Asteroid(Size const size, km::vec2 const& position, km::vec2 const& velocity) : 
  Sprite(), o(0), v(velocity.x, velocity.y)
{
  o = glhckSpriteNew(IMAGES.at(size).data(), 3, GLHCK_TEXTURE_DEFAULTS);
  glhckObjectSetMaterialFlags(o, GLHCK_MATERIAL_ALPHA);
  glhckObjectPositionf(o, position.x, position.y, 0);
}

void Asteroid::render()
{
  glhckObjectDraw(o);
}

void Asteroid::update(float delta)
{
  glhckObjectMovef(o, v.x * delta, v.y * delta, 0);
  glhckObjectRotatef(o, 0, 0, delta * 40);
  
  // FIXME: Do proper wrapping
  kmVec3 const* pos = glhckObjectGetPosition(o);
  if(pos->x < -27) {
     glhckObjectMovef(o, 54, 0, 0);
  } else if(pos->x > 27) {
     glhckObjectMovef(o, -54, 0, 0);
  }
  
  if(pos->y < -16) {
     glhckObjectMovef(o, 0, 32, 0);
  } else if(pos->y > 16) {
     glhckObjectMovef(o, 0, -32, 0);
  }
}
