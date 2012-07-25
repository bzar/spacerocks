#include "asteroid.h"

Asteroid::Asteroid(int const size, float x, float y) : Sprite(), o(0), angle(0)
{
  if(size <= 1) {
    o = glhckSpriteNew("img/asteroid_1.png", 3, GLHCK_TEXTURE_DEFAULTS);
  } else if(size == 2) {
    o = glhckSpriteNew("img/asteroid_2.png", 3, GLHCK_TEXTURE_DEFAULTS);
  } else if(size == 3) {
    o = glhckSpriteNew("img/asteroid_3.png", 3, GLHCK_TEXTURE_DEFAULTS);
  } else {
    o = glhckSpriteNew("img/asteroid_4.png", 3, GLHCK_TEXTURE_DEFAULTS);
  }

  glhckObjectSetMaterialFlags(o, GLHCK_MATERIAL_ALPHA);
  glhckObjectPositionf(o, x, y, 0);
}

void Asteroid::render()
{
  glhckObjectDraw(o);
}

void Asteroid::update(float delta)
{
  angle += delta * 40;
  glhckObjectRotatef(o, 0, 0, angle);
}
