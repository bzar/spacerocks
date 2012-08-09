#include "powerup.h"
#include "ship.h"
#include "util/util.h"
#include <cstdlib>

UID const Powerup::ID = getUID();
float const Powerup::RADIUS = 16;
std::vector<std::string> const Powerup::IMAGES = {
  "img/powerup_laser.png",
  "img/powerup_spread.png",
  "img/powerup_continuous.png",
  "img/powerup_plasma.png",
  "img/powerup_extralife.png",
  "img/powerup_loselife.png",
  "img/powerup_shield.png"
};

TextureAtlas Powerup::atlas = TextureAtlas();

void Powerup::init()
{
  atlas = TextureAtlas(IMAGES);
}

void Powerup::term()
{
  atlas = TextureAtlas();
}

Powerup::Powerup(GameWorld* world, Type const type, Vec2D const& position, Vec2D const& velocity) :
  Entity(world), Renderable(world), Updatable(world), Collidable(world),
  o(0), type(type), v(velocity), life(10), shape(position, RADIUS)
{
  o = glhckSpriteNew(atlas.getTexture(), 16, 16);
  glhckObjectTransformCoordinates(o, &atlas.getTransform(type).transform, atlas.getTransform(type).degree);
  glhckObjectSetMaterialFlags(o, GLHCK_MATERIAL_ALPHA);
  glhckObjectPositionf(o, position.x, position.y, 0);
}

Powerup::~Powerup()
{
  glhckObjectFree(o);
}

void Powerup::render()
{
  glhckObjectRender(o);
}

void Powerup::update(float const delta)
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

  shape.center = getPosition();
}

CircleShape const* Powerup::getShape() const
{
  return &shape;
}

void Powerup::collide(Collidable const* other) {
  if(other->getEntityId() == Ship::ID) {
    Ship const* ship = static_cast<Ship const*>(other);
    if(shape.collidesWith(ship->getShape()))
    {
      world->removeEntity(this);
    }
    return;
  }
}

Vec2D Powerup::getPosition() const
{
  kmVec3 const* pos = glhckObjectGetPosition(o);
  return {pos->x, pos->y};
}

Powerup::Type Powerup::getType() const
{
  return type;
}
