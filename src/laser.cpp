#include "laser.h"

std::string const Laser::IMAGE = "img/laser.png";
glhckTexture* Laser::TEXTURE = nullptr;

Laser::Laser(float const life, Vec2D const& position, Vec2D const& velocity) : 
  Particle(), o(0), life(life), v(velocity)
{
  if(TEXTURE == nullptr)
  {
    TEXTURE = glhckTextureNew(IMAGE.data(), GLHCK_TEXTURE_DEFAULTS);
  }
  o = glhckSpriteNew(TEXTURE, 1);
  glhckObjectSetMaterialFlags(o, GLHCK_MATERIAL_ALPHA);
  glhckObjectPositionf(o, position.x, position.y, 0);
  glhckObjectRotationf(o, 0, 0, (v.angle() - 0.25) * 360);
}

Laser::~Laser()
{
  glhckObjectFree(o);
}

void Laser::render()
{
  glhckObjectDraw(o);
}

void Laser::update(float delta)
{
  life -= delta;
  glhckObjectMovef(o, v.x * delta, v.y * delta, 0);
  
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

bool Laser::alive() const
{
  return life > 0;
}