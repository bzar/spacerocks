#include "spark.h"
#include <iostream>

int const Spark::ID = Entity::newEntityId();
std::string const Spark::IMAGE = "img/spark.png";
glhckTexture* Spark::TEXTURE = nullptr;

Spark::Spark(World* world, float const life, Vec2D const& position, Vec2D const& velocity) :
  Sprite(world), o(0), life(life), v(velocity)
{
  if(TEXTURE == nullptr)
  {
    TEXTURE = glhckTextureNew(IMAGE.data(), GLHCK_TEXTURE_DEFAULTS);
  }
  o = glhckSpriteNew(TEXTURE, 0, 0);
  glhckObjectScalef(o, 0.5f, 0.5f, 0.5f);
  glhckObjectSetMaterialFlags(o, GLHCK_MATERIAL_ALPHA);
  glhckObjectPositionf(o, position.x, position.y, 0);
  glhckObjectRotationf(o, 0, 0, (v.angle() - 0.25) * 360);
}

Spark::~Spark()
{
  glhckObjectFree(o);
}

void Spark::render()
{
  glhckObjectRender(o);
}

void Spark::update(float delta)
{
  life -= delta;
  glhckObjectMovef(o, v.x * delta, v.y * delta, 0);

  // FIXME: Do proper wrapping
  kmVec3 const* pos = glhckObjectGetPosition(o);
  if(pos->x < -400) {
     glhckObjectMovef(o, 800, 0, 0);
  } else if(pos->x > 400) {
     glhckObjectMovef(o, -800, 0, 0);
  }

  if(pos->y < -240) {
     glhckObjectMovef(o, 0, 480, 0);
  } else if(pos->y > 240) {
     glhckObjectMovef(o, 0, -480, 0);
  }
}

bool Spark::alive() const
{
  return life > 0;
}

void Spark::collide(Sprite const* other) {

}
