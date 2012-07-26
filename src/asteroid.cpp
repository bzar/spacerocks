#include "asteroid.h"

 std::string const Asteroid::IMAGES[NUM_SIZES] = {
  "img/asteroid_1.png",
  "img/asteroid_2.png",
  "img/asteroid_3.png",
  "img/asteroid_4.png"
};

Asteroid::Asteroid(Size const size, Vec2D const& position, Vec2D const& velocity) : 
  Sprite(), o(0), v(velocity)
{
  o = glhckSpriteNewFromFile(IMAGES[size].data(), 2, GLHCK_TEXTURE_DEFAULTS);
  glhckObjectSetMaterialFlags(o, GLHCK_MATERIAL_ALPHA);
  glhckObjectPositionf(o, position.x, position.y, 0);
}

Asteroid::~Asteroid()
{
  glhckObjectFree(o);
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
