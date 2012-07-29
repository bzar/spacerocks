#include "ship.h"
#include "asteroid.h"
#include "explosion.h"
#include "laser.h"
#include "world.h"

int const Ship::ID = Entity::newEntityId();

std::string const Ship::IMAGES[NUM_IMAGES] = {
  "img/ship.png", "img/ship_accelerating.png",
  "img/ship_left.png", "img/ship_left_accelerating.png",
  "img/ship_right.png", "img/ship_right_accelerating.png",
};

std::vector<Sprite::TransformData> Ship::TRANSFORM;
glhckTexture *Ship::TEXTURE = NULL;

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
}

Ship::Ship(World* world, Vec2D const& position, Vec2D const& velocity) :
  Sprite(world), o(0), v(velocity),
  turningLeft(false), turningRight(false), accelerating(false), shooting(false), laserCooldown(0), dead(false)
{
  o = glhckSpriteNew(TEXTURE, 32, 32);
  glhckObjectScalef(o, 0.75, 0.75, 0.75);
  glhckObjectTransformCoordinates(o, &TRANSFORM[DEFAULT].transform, TRANSFORM[DEFAULT].degree);

  glhckObjectSetMaterialFlags(o, GLHCK_MATERIAL_ALPHA);
  glhckObjectPositionf(o, position.x, position.y, 0);
}

Ship::~Ship()
{
  glhckObjectFree(o);
}


void Ship::render()
{
  glhckObjectRender(o);
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
    Vec2D v(0, 1200);
    v.rotatei(getAngle());
    Vec2D p = v.normal().uniti().scalei(12);
    std::shared_ptr<Laser> laser1(new Laser(world, 0.25, getPosition() + p, v));
    std::shared_ptr<Laser> laser2(new Laser(world, 0.25, getPosition() - p, v));
    world->sprites.insert(laser1);
    world->sprites.insert(laser2);
    laserCooldown = 0.15;
  }

}

bool Ship::alive() const
{
  return !dead;
}


void Ship::collide(Sprite const* other) {
  kmVec3 const* pos = glhckObjectGetPosition(o);
  Vec2D position{pos->x, pos->y};

  if(other->getEntityId() == Asteroid::ID) {
    Asteroid const* asteroid = static_cast<Asteroid const*>(other);
    float d1 = (asteroid->getPosition() - position).lengthSquared();
    float d2 = (RADIUS + asteroid->getRadius()) * (RADIUS + asteroid->getRadius());
    if(d1 < d2)
    {
      Explosion* explosion = new Explosion(world, position);
      world->sprites.insert(std::shared_ptr<Explosion>(explosion));
      dead = true;
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

Vec2D Ship::getVelocity() const
{
  return v;
}

Vec2D Ship::getPosition() const
{
  kmVec3 const* pos = glhckObjectGetPosition(o);
  return {pos->x, pos->y};
}

float Ship::getAngle() const
{
  kmVec3 const* rot = glhckObjectGetRotation(o);
  return rot->z / 360;
}


