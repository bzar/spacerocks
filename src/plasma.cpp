#include "plasma.h"
#include "asteroid.h"
#include "ufo.h"
#include "util/util.h"

UID const Plasma::ID = getUID();
std::string const Plasma::IMAGE = "img/plasma.png";
glhckTexture* Plasma::TEXTURE = nullptr;

void Plasma::init()
{
  TEXTURE = glhckTextureNew(IMAGE.data(), GLHCK_TEXTURE_DEFAULTS);
}

void Plasma::term()
{
  glhckTextureFree(TEXTURE);
}

Plasma::Plasma(GameWorld* world, float const life, float const power,
               Vec2D const& position, Vec2D const& velocity) :
  Entity(world), Renderable(world), Updatable(world), Collidable(world),
  o(0), power(power), nextPower(power), life(life), v(velocity), shape(position, getRadius())
{
  o = glhckSpriteNew(TEXTURE, 1, 1);
  glhckObjectScalef(o, getRadius(), getRadius(), 1);
  glhckObjectSetMaterialFlags(o, GLHCK_MATERIAL_ALPHA);
  glhckObjectPositionf(o, position.x, position.y, 0);
  glhckObjectRotationf(o, 0, 0, (v.angle()) * 360);
}

Plasma::~Plasma()
{
  glhckObjectFree(o);
}

void Plasma::render()
{
  glhckObjectRender(o);
}

void Plasma::update(float delta)
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

  shape.center = getPosition();
  if(power != nextPower)
  {
    glhckObjectScalef(o, getRadius(), getRadius(), 1);
    shape.radius = getRadius();
    power = nextPower;
  }

  if(life <= 0 || power <= 0)
    world->removeEntity(this);
}

CircleShape const* Plasma::getShape() const
{
  return &shape;
}


void Plasma::collide(Collidable const* other) {
  if(life <= 0 || power <= 0)
    return;

  if(other->getEntityId() == Asteroid::ID) {
    Asteroid const* asteroid = static_cast<Asteroid const*>(other);
    if(shape.collidesWith(asteroid->getShape()))
    {
      nextPower -= 1.0;
      v.scalei(0.9);
    }
    return;
  }

  if(other->getEntityId() == Ufo::ID) {
    Ufo const* ufo = static_cast<Ufo const*>(other);
    if(shape.collidesWith(ufo->getShape()))
    {
      nextPower -= 1.0;
      v.scalei(0.9);
    }
    return;
  }

  if(life <= 0 || power <= 0)
    world->removeEntity(this);

}

Vec2D Plasma::getPosition() const
{
  kmVec3 const* pos = glhckObjectGetPosition(o);
  return {pos->x, pos->y};
}

float Plasma::getPower() const
{
  return power;
}

float Plasma::getRadius() const
{
  return power;
}
