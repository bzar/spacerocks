#include "ufolaser.h"
#include "asteroid.h"
#include "util/util.h"

ew::UID const UfoLaser::ID = ew::getUID();
std::string const UfoLaser::IMAGE = "img/ufolaser.png";
glhckTexture* UfoLaser::TEXTURE = nullptr;

void UfoLaser::init()
{
  TEXTURE = glhckTextureNew(IMAGE.data(), GLHCK_TEXTURE_DEFAULTS);
}

void UfoLaser::term()
{
  glhckTextureFree(TEXTURE);
}

UfoLaser::UfoLaser(GameWorld* world, float const life, Vec2D const& position, Vec2D const& velocity) :
  ew::Entity(world), ew::Renderable(world), ew::Updatable(world), ew::Collidable(world),
  o(0), life(life), v(velocity), shape({0, 0}, {0, 0}, RADIUS)
{
  o = glhckSpriteNew(TEXTURE, 2, 8);
  glhckObjectMaterialFlags(o, GLHCK_MATERIAL_ALPHA);
  glhckObjectPositionf(o, position.x, position.y, 0);
  glhckObjectRotationf(o, 0, 0, (v.angle() - 0.25) * 360);

  Vec2D r = v.unit().scale(LENGTH/2.0f - RADIUS);
  shape.p1 = position + r;
  shape.p2 = position - r;
}

UfoLaser::~UfoLaser()
{
  glhckObjectFree(o);
}

void UfoLaser::render(ew::RenderContext* context)
{
  glhckObjectRender(o);
}

void UfoLaser::update(float const delta)
{
  life -= delta;
  if(life <= 0)
    world->removeEntity(this);

  glhckObjectMovef(o, v.x * delta, v.y * delta, 0);
  shape.p1 = getPosition() + v.unit().scale(LENGTH/2.0f - RADIUS);
  shape.p2 = getPosition() - v.unit().scale(LENGTH/2.0f - RADIUS);
}

LineShape const* UfoLaser::getShape() const
{
  return &shape;
}


void UfoLaser::collide(ew::Collidable const* other) {
  Vec2D position = getPosition();

  if(other->getEntityId() == Asteroid::ID) {
    Asteroid const* asteroid = static_cast<Asteroid const*>(other);
    if(shape.collidesWith(asteroid->getShape()))
    {
      life = 0;
      world->removeEntity(this);
    }
    return;
  }
}

Vec2D UfoLaser::getPosition() const
{
  kmVec3 const* pos = glhckObjectGetPosition(o);
  return {pos->x, pos->y};
}
