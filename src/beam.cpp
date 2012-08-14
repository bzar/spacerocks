#include "beam.h"
#include "asteroid.h"
#include "ufo.h"
#include "util/util.h"

UID const Beam::ID = getUID();

float const Beam::RADIUS = 4;
std::string const Beam::IMAGE = "img/continuous_beam.png";
std::string const Beam::TIP_IMAGE = "img/continuous_tip.png";
glhckTexture* Beam::TEXTURE = nullptr;
glhckTexture* Beam::TIP_TEXTURE = nullptr;

void Beam::init()
{
  TEXTURE = glhckTextureNew(IMAGE.data(), GLHCK_TEXTURE_DEFAULTS);
  TIP_TEXTURE = glhckTextureNew(TIP_IMAGE.data(), GLHCK_TEXTURE_DEFAULTS);
}

void Beam::term()
{
  glhckTextureFree(TEXTURE);
  glhckTextureFree(TIP_TEXTURE);
}

Beam::Beam(GameWorld* world, Vec2D const& basePosition, Vec2D const& positionDelta) :
  Entity(world), Renderable(world, -1), Updatable(world), Collidable(world),
  o(nullptr), tip(nullptr), hitDelay(0), recovering(false), shape({0, 0}, {0, 0}, 4)
{
  o = glhckSpriteNew(TEXTURE, 4, 128);
  tip = glhckSpriteNew(TIP_TEXTURE, 4, 4);
  glhckObjectSetMaterialFlags(o, GLHCK_MATERIAL_ALPHA);
  glhckObjectSetMaterialFlags(tip, GLHCK_MATERIAL_ALPHA);

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

void Beam::render(RenderContext* context)
{
  glhckObjectRender(o);
  glhckObjectRender(tip);
}

void Beam::update(float const delta)
{
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
}

LineShape const* Beam::getShape() const
{
  return &shape;
}


void Beam::collide(Collidable const* other) {
  if(other->getEntityId() == Asteroid::ID) {
    Asteroid const* asteroid = static_cast<Asteroid const*>(other);
    if(shape.collidesWith(asteroid->getShape()))
    {
      if(!recovering)
        hitDelay = 0.10;
      shape.p2 = circleLineIntersectionPoint(asteroid->getShape()->center, asteroid->getShape()->radius, shape.p1, shape.p2, shape.radius);
    }
    return;
  }

  if(other->getEntityId() == Ufo::ID) {
    Ufo const* ufo = static_cast<Ufo const*>(other);
    if(shape.collidesWith(ufo->getShape()))
    {
      if(!recovering)
        hitDelay = 0.10;
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
  shape.p2 = shape.p1 + positionDelta;
}

Vec2D Beam::getBasePosition() const
{
  return shape.p1;
}

bool Beam::canHit() const
{
  return !recovering;
}