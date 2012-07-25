#include "asteroid.h"

std::map<int, std::string> const Asteroid::IMAGES = {
  {1, "img/asteroid_1.png"},
  {2, "img/asteroid_2.png"},
  {3, "img/asteroid_3.png"},
  {4, "img/asteroid_4.png"}
};

Asteroid::Asteroid(Size const size, km::vec2 const& position, km::vec2 const& velocity) : 
  Sprite(), o(0), angle(0), v(velocity.x, velocity.y)
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
  angle += delta * 40;
  angle = angle >= 360.0 ? angle - 360.0 : angle;
  glhckObjectMovef(o, v.x * delta, v.y * delta, 0);
  glhckObjectRotatef(o, 0, 0, angle);
}
