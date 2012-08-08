#include "shot.h"
#include "asteroid.h"
#include "ufo.h"
#include "util/util.h"

Entity::Id const Shot::ID = Entity::newEntityId();
std::string const Shot::IMAGE = "img/shot.png";
glhckTexture* Shot::TEXTURE = nullptr;

void Shot::init()
{
  TEXTURE = glhckTextureNew(IMAGE.data(), GLHCK_TEXTURE_DEFAULTS);
}

void Shot::term()
{
  glhckTextureFree(TEXTURE);
}

Shot::Shot(GameWorld* world, float const life, Vec2D const& position, Vec2D const& velocity) :
  Entity(world), Renderable(world), Updatable(world), Collidable(world),
  o(0), life(life), v(velocity), shape(position, RADIUS)
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

void Shot::update(float const delta)
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

CircleShape const* Shot::getShape() const
{
  return &shape;
}


void Shot::collide(Collidable const* other) {
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

Vec2D Shot::getPosition() const
{
  kmVec3 const* pos = glhckObjectGetPosition(o);
  return {pos->x, pos->y};
}
