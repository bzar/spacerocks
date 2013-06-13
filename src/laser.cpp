#include "laser.h"
#include "asteroid.h"
#include "ufo.h"
#include "util/util.h"

std::string const Laser::IMAGE = "img/laser2.png";
glhckTexture* Laser::TEXTURE = nullptr;

void Laser::init()
{
  TEXTURE = glhckTextureNewFromFile(IMAGE.data(), nullptr, nullptr);
}

void Laser::term()
{
  glhckTextureFree(TEXTURE);
}

Laser::Laser(GameWorld* world, float const life, Vec2D const& position, Vec2D const& velocity) :
  Entity(world), ew::Renderable(world), ew::Updatable(world), ew::Collidable(world),
  gameWorld(world), o(0), oGlow(nullptr), life(life), glowPhase(0), v(velocity), shape({0, 0}, {0, 0}, RADIUS)
{
  o = glhckSpriteNew(TEXTURE, 4, 16);
  glhckObjectPositionf(o, position.x, position.y, 0);
  glhckObjectRotationf(o, 0, 0, (v.angle() - 0.25) * 360);

  oGlow = glhckSpriteNew(TEXTURE, 4, 16);
  glhckObjectAddChild(o, oGlow);
  glhckObjectParentAffection(oGlow, GLHCK_AFFECT_TRANSLATION|GLHCK_AFFECT_ROTATION|GLHCK_AFFECT_SCALING);
  glhckMaterialBlendFunc(glhckObjectGetMaterial(oGlow), GLHCK_SRC_ALPHA, GLHCK_ONE);

  Vec2D r = v.unit().scale(LENGTH/2.0f - RADIUS);
  shape.p1 = position + r;
  shape.p2 = position - r;
}

Laser::~Laser()
{
  glhckObjectFree(o);
  glhckObjectFree(oGlow);
}

void Laser::render(ew::RenderContext* context)
{
  glhckObjectRender(o);
  glhckObjectRender(oGlow);
}

void Laser::update(float const delta)
{
  glowPhase += delta;
  float const phaseSpeed = 15;
  float const phase = (sin(glowPhase*phaseSpeed) + 1)/2;
  float const glowScale = lerp(0.1, 0.8, phase);
  float const glowOpacity = lerp(0.1, 0.5, phase);
  glhckObjectScalef(oGlow, glowScale + 1, glowScale * 0.25 + 1, 1);
  glhckMaterialDiffuseb(glhckObjectGetMaterial(oGlow), 255, 255, 255, 255 * glowOpacity);

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

Vec2D Laser::getPosition() const
{
  kmVec3 const* pos = glhckObjectGetPosition(o);
  return {pos->x, pos->y};
}
