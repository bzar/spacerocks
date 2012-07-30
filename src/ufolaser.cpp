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
  Sprite(world), o(0), life(life), v(velocity), shape({0, 0}, {0, 0}, RADIUS)
{
  o = glhckSpriteNew(TEXTURE, 2, 8);
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
  shape.p1 = getPosition() + v.unit().scale(LENGTH/2.0f - RADIUS);
  shape.p2 = getPosition() - v.unit().scale(LENGTH/2.0f - RADIUS);
}

bool UfoLaser::alive() const
{
  return life > 0;
}

LineShape const* UfoLaser::getShape() const
{
  return &shape;
}


void UfoLaser::collide(Sprite const* other) {
  Vec2D position = getPosition();

  if(other->getEntityId() == Asteroid::ID) {
    Asteroid const* asteroid = static_cast<Asteroid const*>(other);
    if(shape.collidesWith(asteroid->getShape()))
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
