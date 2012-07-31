#include "ship.h"
#include "asteroid.h"
#include "explosion.h"
#include "laser.h"
#include "world.h"
#include "util.h"
#include "ufo.h"
#include "ufolaser.h"

int const Ship::ID = Entity::newEntityId();

std::string const Ship::IMAGES[NUM_IMAGES] = {
  "img/ship.png", "img/ship_accelerating.png",
  "img/ship_left.png", "img/ship_left_accelerating.png",
  "img/ship_right.png", "img/ship_right_accelerating.png",
};

std::string const Ship::SHIELD_IMAGE = "img/shield.png";

std::vector<Sprite::TransformData> Ship::TRANSFORM;
glhckTexture *Ship::TEXTURE = NULL;
glhckTexture *Ship::SHIELD_TEXTURE = NULL;

void Ship::init()
{
  glhckAtlas *TEXTURES = glhckAtlasNew();
  for(int i = 0; i < NUM_IMAGES; ++i)
  {
    glhckTexture* texture = glhckTextureNew(IMAGES[i].data(), GLHCK_TEXTURE_DEFAULTS);
    glhckAtlasInsertTexture(TEXTURES, texture);
    glhckTextureFree(texture);
  }
  glhckAtlasPack(TEXTURES, true, false);

  for(int i = 0; i < NUM_IMAGES; ++i)
  {
    TransformData t;
    glhckAtlasGetTransform(TEXTURES, glhckAtlasGetTextureByIndex(TEXTURES, i), &t.transform, &t.degree);
    TRANSFORM.push_back(t);
  }

  TEXTURE = glhckTextureRef(glhckAtlasGetTexture(TEXTURES));
  glhckAtlasFree(TEXTURES);

  SHIELD_TEXTURE = glhckTextureNew(SHIELD_IMAGE.data(), GLHCK_TEXTURE_DEFAULTS);
}

Ship::Ship(World* world, Vec2D const& position, Vec2D const& velocity) :
  Sprite(world), o(0), shield(0), v(velocity),
  turningLeft(false), turningRight(false), accelerating(false), shooting(false),
  shieldLeft(4), laserCooldown(0), dead(false), shape(position, RADIUS)
{
  o = glhckSpriteNew(TEXTURE, 16, 16);
  shield = glhckSpriteNew(SHIELD_TEXTURE, 24, 24);

  glhckObjectTransformCoordinates(o, &TRANSFORM[DEFAULT].transform, TRANSFORM[DEFAULT].degree);

  glhckObjectSetMaterialFlags(o, GLHCK_MATERIAL_ALPHA);
  glhckObjectSetMaterialFlags(shield, GLHCK_MATERIAL_ALPHA);
  glhckObjectPositionf(o, position.x, position.y, 0);
}

Ship::~Ship()
{
  glhckObjectFree(o);
}


void Ship::render()
{
  glhckObjectRender(o);

  if(shieldLeft > 0)
  {
    glhckObjectRender(shield);
  }
}

void Ship::update(float delta)
{
  if(turningLeft) glhckObjectRotatef(o, 0, 0, delta * 120);
  if(turningRight) glhckObjectRotatef(o, 0, 0, delta * -120);

  if(accelerating)
  {
    kmScalar angle = glhckObjectGetRotation(o)->z;
    Vec2D acceleration(0, 120 *  delta);
    acceleration.rotatei(angle / 360);
    v += acceleration;
  }

  ImageType t = DEFAULT;
  if(accelerating) {
    t = turningLeft && !turningRight ? LEFT_ACCELERATING :
        turningRight && !turningLeft ? RIGHT_ACCELERATING :
        ACCELERATING;
  } else {
    t = turningLeft && !turningRight ? LEFT :
        turningRight && !turningLeft ? RIGHT :
        DEFAULT;
  }

  glhckObjectTransformCoordinates(o, &TRANSFORM[t].transform, TRANSFORM[t].degree);
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

  laserCooldown = laserCooldown > 0 ? laserCooldown - delta : 0.0f;

  if(shooting && laserCooldown <= 0)
  {
    Vec2D velocity(0, 1200);
    velocity.rotatei(glhckObjectGetRotation(o)->z / 360);
    Vec2D p = velocity.normal().uniti().scalei(8);
    std::shared_ptr<Laser> laser1(new Laser(world, 0.25, getPosition() + p, velocity));
    std::shared_ptr<Laser> laser2(new Laser(world, 0.25, getPosition() - p, velocity));
    world->sprites.insert(laser1);
    world->sprites.insert(laser2);
    laserCooldown = 0.15;
  }

  if(shieldLeft > 0)
  {
    shieldLeft -= delta;
    glhckObjectPosition(shield, glhckObjectGetPosition(o));
  }

  shape.center = getPosition();
}

bool Ship::alive() const
{
  return !dead;
}

CircleShape const* Ship::getShape() const
{
  return &shape;
}


void Ship::collide(Sprite const* other) {
  Vec2D position = getPosition();

  if(other->getEntityId() == Asteroid::ID) {
    if(shieldLeft > 0)
      return;

    Asteroid const* asteroid = static_cast<Asteroid const*>(other);
    if(shape.collidesWith(asteroid->getShape()))
    {
      die();
    }
    return;
  }

  if(other->getEntityId() == UfoLaser::ID) {
    if(shieldLeft > 0)
      return;

    UfoLaser const* ufoLaser = static_cast<UfoLaser const*>(other);
    if(shape.collidesWith(ufoLaser->getShape()))
    {
      die();
    }
    return;
  }

  if(other->getEntityId() == Ufo::ID) {
    if(shieldLeft > 0)
      return;

    Ufo const* ufo = static_cast<Ufo const*>(other);
    if(shape.collidesWith(ufo->getShape()))
    {
      die();
    }
    return;
  }

}

void Ship::turnLeft(bool const value)
{
  turningLeft = value;
}

void Ship::turnRight(bool const value)
{
  turningRight = value;
}

void Ship::accelerate(bool const value)
{
  accelerating = value;
}

void Ship::shoot(bool const value)
{
  shooting = value;
}

void Ship::reset()
{
  glhckObjectPositionf(o, 0, 0, 0);
  glhckObjectRotationf(o, 0, 0, 0);
  v = {0, 0};
}

Vec2D Ship::getVelocity() const
{
  return v;
}

Vec2D Ship::getPosition() const
{
  kmVec3 const* pos = glhckObjectGetPosition(o);
  return {pos->x, pos->y};
}

void Ship::die()
{
  Explosion* explosion = new Explosion(world, getPosition());
  world->sprites.insert(std::shared_ptr<Explosion>(explosion));
  dead = true;
}
