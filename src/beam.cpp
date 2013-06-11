#include "beam.h"
#include "asteroid.h"
#include "ufo.h"
#include "util/util.h"

float const Beam::RADIUS = 4;
float const Beam::HIT_RATE = 4;
std::string const Beam::IMAGE = "img/continuous_beam.png";
std::string const Beam::TIP_IMAGE = "img/continuous_tip.png";
glhckTexture* Beam::TEXTURE = nullptr;
glhckTexture* Beam::TIP_TEXTURE = nullptr;

void Beam::init()
{
  TEXTURE = glhckTextureNewFromFile(IMAGE.data(), nullptr, nullptr);
  TIP_TEXTURE = glhckTextureNewFromFile(TIP_IMAGE.data(), nullptr, nullptr);
}

void Beam::term()
{
  glhckTextureFree(TEXTURE);
  glhckTextureFree(TIP_TEXTURE);
}

Beam::Beam(GameWorld* world, Vec2D const& basePosition, Vec2D const& positionDelta) :
  ew::Entity(world), ew::Renderable(world, -1), ew::Updatable(world), ew::Collidable(world),
  gameWorld(world), o(nullptr), tip(nullptr), oGlow(nullptr), tipGlow(nullptr), hitDelay(0),
  recovering(false), glowPhase(0), shape({0, 0}, {0, 0}, 4)
{
  o = glhckSpriteNew(TEXTURE, 8, 256);
  tip = glhckSpriteNew(TIP_TEXTURE, 8, 8);

  oGlow = glhckSpriteNew(TEXTURE, 8, 256);
  tipGlow = glhckSpriteNew(TIP_TEXTURE, 8, 8);
  glhckObjectAddChild(o, oGlow);
  glhckObjectAddChild(tip, tipGlow);
  glhckObjectParentAffection(oGlow, GLHCK_AFFECT_TRANSLATION|GLHCK_AFFECT_ROTATION|GLHCK_AFFECT_SCALING);
  glhckObjectParentAffection(tipGlow, GLHCK_AFFECT_TRANSLATION|GLHCK_AFFECT_ROTATION|GLHCK_AFFECT_SCALING);
  glhckMaterialBlendFunc(glhckObjectGetMaterial(oGlow), GLHCK_SRC_ALPHA, GLHCK_ONE);
  glhckMaterialBlendFunc(glhckObjectGetMaterial(tipGlow), GLHCK_SRC_ALPHA, GLHCK_ONE);

  shape.p1 = basePosition;
  shape.p2 = basePosition + positionDelta;
  Vec2D midPoint = basePosition + positionDelta.scale(0.5);
  glhckObjectPositionf(o, midPoint.x, midPoint.y, 0);
  glhckObjectPositionf(tip, shape.p2.x, shape.p2.y, 0);

  float angle = (positionDelta.angle() - 0.25) * 360;
  glhckObjectRotationf(o, 0, 0, angle);
  glhckObjectRotationf(tip, 0, 0, angle);
  glhckObjectScalef(o, 1, positionDelta.length()/256, 1);
}

Beam::~Beam()
{
  glhckObjectFree(o);
  glhckObjectFree(tip);
}

void Beam::render(ew::RenderContext* context)
{
  glhckObjectRender(o);
  glhckObjectRender(tip);
  glhckObjectRender(oGlow);
  glhckObjectRender(tipGlow);
}

void Beam::update(float const delta)
{
  glowPhase += delta;
  float const phaseSpeed = 5;
  float const phase = (sin(glowPhase*phaseSpeed) + 1)/2;
  float const glowScale = lerp(1.1, 1.8, phase);
  float const glowOpacity = lerp(0.1, 0.5, phase);
  glhckObjectScalef(oGlow, glowScale, 1, 1);
  glhckObjectScalef(tipGlow, glowScale, glowScale, 1);
  glhckMaterialDiffuseb(glhckObjectGetMaterial(oGlow), 255, 255, 255, 255 * glowOpacity);
  glhckMaterialDiffuseb(glhckObjectGetMaterial(tipGlow), 255, 255, 255, 255 * glowOpacity);

  if(gameWorld->getPaused())
    return;

  Vec2D positionDelta = shape.p2 - shape.p1;
  Vec2D midPoint = shape.p1 + positionDelta.scale(0.5);
  glhckObjectPositionf(o, midPoint.x, midPoint.y, 0);
  glhckObjectPositionf(tip, shape.p2.x, shape.p2.y, 0);

  float angle = (positionDelta.angle() - 0.25) * 360;
  glhckObjectRotationf(o, 0, 0, angle);
  glhckObjectRotationf(tip, 0, 0, angle);

  glhckObjectScalef(o, 1, positionDelta.length()/256, 1);

  hitDelay = hitDelay > 0 ? hitDelay - delta : 0;
  recovering = hitDelay > 0;

  shape.p2 = nextEndPoint;
}

LineShape const* Beam::getShape() const
{
  return &shape;
}


void Beam::collide(ew::Collidable const* other) {
  if(typeid(*other) == typeid(Asteroid)) {
    Asteroid const* asteroid = static_cast<Asteroid const*>(other);
    if(shape.collidesWith(asteroid->getShape()))
    {
      if(!recovering)
        hitDelay = 1.0f / HIT_RATE;
      shape.p2 = circleLineIntersectionPoint(asteroid->getShape()->center, asteroid->getShape()->radius, shape.p1, shape.p2, shape.radius);
    }
    return;
  }

  if(typeid(*other) == typeid(Ufo)) {
    Ufo const* ufo = static_cast<Ufo const*>(other);
    if(shape.collidesWith(ufo->getShape()))
    {
      if(!recovering)
        hitDelay = 1.0f / HIT_RATE;
      shape.p2 = circleLineIntersectionPoint(ufo->getShape()->center, ufo->getShape()->radius, shape.p1, shape.p2, shape.radius);
    }
    return;
  }
}

void Beam::setBasePosition(Vec2D const& basePosition)
{
  Vec2D diff = basePosition - shape.p1;
  shape.p1 = basePosition;
  shape.p2 += diff;
}

void Beam::setPositionDelta(Vec2D const& positionDelta)
{
  nextEndPoint = shape.p1 + positionDelta;
}

Vec2D Beam::getBasePosition() const
{
  return shape.p1;
}

bool Beam::canHit() const
{
  return !recovering;
}
