#include "ufolaser.h"
#include "asteroid.h"
#include "util.h"

int const UfoLaser::ID = Entity::newEntityId();
std::string const UfoLaser::IMAGE = "img/ufolaser.png";
glhckTexture* UfoLaser::TEXTURE = nullptr;

void UfoLaser::init()
{
  TEXTURE = glhckTextureNew(IMAGE.data(), GLHCK_TEXTURE_DEFAULTS);
}

UfoLaser::UfoLaser(World* world, float const life, Vec2D const& position, Vec2D const& velocity) :
  Sprite(world), o(0), life(life), v(velocity)
{
  o = glhckSpriteNew(TEXTURE, 0, 0);
  glhckObjectScalef(o, 0.5f, 0.5f, 0.5f);
  glhckObjectSetMaterialFlags(o, GLHCK_MATERIAL_ALPHA);
  glhckObjectPositionf(o, position.x, position.y, 0);
  glhckObjectRotationf(o, 0, 0, (v.angle() - 0.25) * 360);
}

UfoLaser::~UfoLaser()
{
  glhckObjectFree(o);
}

void UfoLaser::render()
{
  glhckObjectRender(o);
}

void UfoLaser::update(float delta)
{
  life -= delta;
  glhckObjectMovef(o, v.x * delta, v.y * delta, 0);
}

bool UfoLaser::alive() const
{
  return life > 0;
}

void UfoLaser::collide(Sprite const* other) {
  kmVec3 const* pos = glhckObjectGetPosition(o);
  Vec2D position{pos->x, pos->y};

  if(other->getEntityId() == Asteroid::ID) {
    Asteroid const* asteroid = static_cast<Asteroid const*>(other);
    if(circlesIntersect(position, getRadius(), asteroid->getPosition(), asteroid->getRadius()))
    {
      life = 0;
    }
    return;
  }
}

Vec2D UfoLaser::getPosition() const
{
  kmVec3 const* pos = glhckObjectGetPosition(o);
  return {pos->x, pos->y};
}

float UfoLaser::getRadius() const
{
  return RADIUS;
}
