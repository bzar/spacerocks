#include "shot.h"
#include "asteroid.h"
#include "ufo.h"
#include "util/util.h"

std::string const Shot::IMAGE = "img/shot.png";
glhckTexture* Shot::TEXTURE = nullptr;

void Shot::init()
{
  TEXTURE = glhckTextureNewFromFile(IMAGE.data(), nullptr, glhckTextureDefaultSpriteParameters());
}

void Shot::term()
{
  glhckTextureFree(TEXTURE);
}

Shot::Shot(GameWorld* world, float const life, Vec2D const& position, Vec2D const& velocity) :
  ew::Entity(world), ew::Renderable(world), ew::Updatable(world), ew::Collidable(world),
  gameWorld(world), o(0), life(life), v(velocity), shape(position, RADIUS)
{
  o = glhckSpriteNew(TEXTURE, 8, 8);
  glhckObjectPositionf(o, position.x, position.y, 0);
}

Shot::~Shot()
{
  glhckObjectFree(o);
}

void Shot::render(ew::RenderContext* context)
{
  glhckObjectRender(o);
}

void Shot::update(float const delta)
{
  if(gameWorld->getPaused())
    return;

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


void Shot::collide(ew::Collidable const* other) {
  if(life <= 0)
    return;

  if(typeid(*other) == typeid(Asteroid)) {
    Asteroid const* asteroid = static_cast<Asteroid const*>(other);
    if(shape.collidesWith(asteroid->getShape()))
    {
      life = 0;
      world->removeEntity(this);
    }
    return;
  }

  if(typeid(*other) == typeid(Ufo)) {
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
