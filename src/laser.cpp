#include "laser.h"
#include "asteroid.h"
#include "util.h"
#include "ufo.h"

int const Laser::ID = Entity::newEntityId();
std::string const Laser::IMAGE = "img/laser.png";
glhckTexture* Laser::TEXTURE = nullptr;

void Laser::init()
{
  TEXTURE = glhckTextureNew(IMAGE.data(), GLHCK_TEXTURE_DEFAULTS);
}

Laser::Laser(World* world, float const life, Vec2D const& position, Vec2D const& velocity) :
  Sprite(world), o(0), life(life), v(velocity)
{
  o = glhckSpriteNew(TEXTURE, 0, 0);
  glhckObjectScalef(o, 0.5f, 0.5f, 0.5f);
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
  glhckObjectRender(o);
}

void Laser::update(float delta)
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

bool Laser::alive() const
{
  return life > 0;
}

void Laser::collide(Sprite const* other) {
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

  if(other->getEntityId() == Ufo::ID) {
    Ufo const* ufo = static_cast<Ufo const*>(other);
    if(circlesIntersect(position, getRadius(), ufo->getPosition(), ufo->getRadius()))
    {
      life = 0;
    }
    return;
  }
}

Vec2D Laser::getPosition() const
{
  kmVec3 const* pos = glhckObjectGetPosition(o);
  return {pos->x, pos->y};
}

float Laser::getRadius() const
{
  return RADIUS;
}
