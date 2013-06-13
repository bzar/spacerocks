#include "plasma.h"
#include "asteroid.h"
#include "ufo.h"
#include "util/util.h"

std::string const Plasma::IMAGE = "img/plasma.png";
glhckTexture* Plasma::TEXTURE = nullptr;

void Plasma::init()
{
  TEXTURE = glhckTextureNewFromFile(IMAGE.data(), nullptr, nullptr);
}

void Plasma::term()
{
  glhckTextureFree(TEXTURE);
}

Plasma::Plasma(GameWorld* world, float const life, float const power,
               Vec2D const& position, Vec2D const& velocity) :
  ew::Entity(world), ew::Renderable(world), ew::Updatable(world), ew::Collidable(world),
  gameWorld(world), o(nullptr), oGlow(nullptr), power(power), nextPower(power), life(life), glowPhase(0), v(velocity), shape(position, getRadius())
{
  o = glhckSpriteNew(TEXTURE, 2, 2);

  oGlow = glhckSpriteNew(TEXTURE, 2, 2);
  glhckObjectAddChild(o, oGlow);
  glhckObjectParentAffection(oGlow, GLHCK_AFFECT_TRANSLATION|GLHCK_AFFECT_ROTATION|GLHCK_AFFECT_SCALING);
  glhckMaterialBlendFunc(glhckObjectGetMaterial(oGlow), GLHCK_SRC_ALPHA, GLHCK_ONE);

  glhckObjectScalef(o, getRadius(), getRadius(), 1);
  glhckObjectPositionf(o, position.x, position.y, 0);
  glhckObjectRotationf(o, 0, 0, (v.angle()) * 360);
}

Plasma::~Plasma()
{
  glhckObjectFree(o);
  glhckObjectFree(oGlow);
}

void Plasma::render(ew::RenderContext* context)
{
  glhckObjectRender(o);
  glhckObjectRender(oGlow);
}

void Plasma::update(float delta)
{
  glowPhase += delta;
  float const phaseSpeed = 10;
  float const phase = (sin(glowPhase*phaseSpeed) + 1)/2;
  float const glowScale = lerp(1.2, 1.3, phase);
  float const glowOpacity = lerp(0.1, 0.5, phase);
  glhckObjectScalef(oGlow, glowScale, glowScale, 1);
  glhckMaterialDiffuseb(glhckObjectGetMaterial(oGlow), 255, 255, 255, 255 * glowOpacity);

  if(gameWorld->getPaused())
    return;

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


void Plasma::collide(ew::Collidable const* other) {
  if(life <= 0 || power <= 0)
    return;

  if(typeid(*other) == typeid(Asteroid)) {
    Asteroid const* asteroid = static_cast<Asteroid const*>(other);
    if(shape.collidesWith(asteroid->getShape()))
    {
      nextPower -= 1.0;
      v.scalei(0.9);
    }
    return;
  }

  if(typeid(*other) == typeid(Ufo)) {
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
