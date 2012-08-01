#include "shot.h"
#include "asteroid.h"
#include "util.h"
#include "ufo.h"

int const Shot::ID = Entity::newEntityId();
std::string const Shot::IMAGE = "img/shot.png";
glhckTexture* Shot::TEXTURE = nullptr;

void Shot::init()
{
  TEXTURE = glhckTextureNew(IMAGE.data(), GLHCK_TEXTURE_DEFAULTS);
}

Shot::Shot(World* world, float const life, Vec2D const& position, Vec2D const& velocity) :
  Sprite(world), o(0), life(life), v(velocity), shape(position, RADIUS)
{
  o = glhckSpriteNew(TEXTURE, 4, 4);
  glhckObjectSetMaterialFlags(o, GLHCK_MATERIAL_ALPHA);
  glhckObjectPositionf(o, position.x, position.y, 0);
}

Shot::~Shot()
{
  glhckObjectFree(o);
}

void Shot::render()
{
  glhckObjectRender(o);
}

void Shot::update(float delta)
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
}

bool Shot::alive() const
{
  return life > 0;
}

CircleShape const* Shot::getShape() const
{
  return &shape;
}


void Shot::collide(Sprite const* other) {
  if(other->getEntityId() == Asteroid::ID) {
    Asteroid const* asteroid = static_cast<Asteroid const*>(other);
    if(shape.collidesWith(asteroid->getShape()))
    {
      life = 0;
    }
    return;
  }

  if(other->getEntityId() == Ufo::ID) {
    Ufo const* ufo = static_cast<Ufo const*>(other);
    if(shape.collidesWith(ufo->getShape()))
    {
      life = 0;
    }
    return;
  }
}

Vec2D Shot::getPosition() const
{
  kmVec3 const* pos = glhckObjectGetPosition(o);
  return {pos->x, pos->y};
}
