#include "laser.h"
#include "asteroid.h"
#include "ufo.h"
#include "util/util.h"

ew::UID const Laser::ID = ew::getUID();
std::string const Laser::IMAGE = "img/laser2.png";
glhckTexture* Laser::TEXTURE = nullptr;

void Laser::init()
{
  TEXTURE = glhckTextureNew(IMAGE.data(), GLHCK_TEXTURE_DEFAULTS);
}

void Laser::term()
{
  glhckTextureFree(TEXTURE);
}

Laser::Laser(GameWorld* world, float const life, Vec2D const& position, Vec2D const& velocity) :
  Entity(world), ew::Renderable(world), ew::Updatable(world), ew::Collidable(world),
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

Laser::~Laser()
{
  glhckObjectFree(o);
}

void Laser::render(ew::RenderContext* context)
{
  glhckObjectRender(o);
}

void Laser::update(float const delta)
{
  life -= delta;
  if(life <= 0)
    world->removeEntity(this);

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

  Vec2D position(pos->x, pos->y);
  Vec2D r = v.unit().scale(LENGTH/2.0f - RADIUS);
  shape.p1 = position + r;
  shape.p2 = position - r;
}

LineShape const* Laser::getShape() const
{
  return &shape;
}


void Laser::collide(ew::Collidable const* other) {
  if(life <= 0)
    return;

  if(other->getEntityId() == Asteroid::ID) {
    Asteroid const* asteroid = static_cast<Asteroid const*>(other);
    if(shape.collidesWith(asteroid->getShape()))
    {
      life = 0;
      world->removeEntity(this);
    }
    return;
  }

  if(other->getEntityId() == Ufo::ID) {
    Ufo const* ufo = static_cast<Ufo const*>(other);
    if(shape.collidesWith(ufo->getShape()))
    {
      life = 0;
      world->removeEntity(this);
    }
    return;
  }
}

Vec2D Laser::getPosition() const
{
  kmVec3 const* pos = glhckObjectGetPosition(o);
  return {pos->x, pos->y};
}
