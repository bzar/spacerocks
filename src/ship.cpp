#include "ship.h"
#include "asteroid.h"
#include "explosion.h"
#include "laser.h"
#include "shot.h"
#include "plasma.h"
#include "world.h"
#include "util.h"
#include "ufo.h"
#include "ufolaser.h"
#include "powerup.h"

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
  shieldLeft(4), weaponCooldown(0), weapon(RAPID), dead(false), shape(position, RADIUS)
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

  weaponCooldown = weaponCooldown > 0 ? weaponCooldown - delta : 0.0f;

  if(shooting && weaponCooldown <= 0)
  {
    Vec2D direction = Vec2D(0, 1).rotatei(glhckObjectGetRotation(o)->z / 360);

    if(weapon == RAPID && world->player.weapon[RAPID])
    {
      Vec2D velocity = direction.scale(1200);
      Vec2D p = velocity.normal().uniti().scalei(8);
      std::shared_ptr<Laser> laser1(new Laser(world, 0.25, getPosition() + p, velocity));
      std::shared_ptr<Laser> laser2(new Laser(world, 0.25, getPosition() - p, velocity));
      world->sprites.insert(laser1);
      world->sprites.insert(laser2);
      weaponCooldown = lerp(0.3, 0.05, (world->player.weapon[RAPID] - 1)/8.0);
    }
    else if(weapon == SPREAD && world->player.weapon[SPREAD])
    {
      float spreadAngle = lerp(0.05, 0.45, (world->player.weapon[SPREAD] - 1)/8.0);
      int shots = 2 * world->player.weapon[SPREAD] + 1;
      for(int i = 0; i < shots; ++i)
      {
        Vec2D velocity = direction.scale(1200);
        velocity.rotatei(spreadAngle * i / (shots - 1) - spreadAngle / 2);
        std::shared_ptr<Shot> shot(new Shot(world, 0.25, getPosition(), velocity));
        world->sprites.insert(shot);
      }
      weaponCooldown = weaponCooldown = lerp(0.8, 0.3, (world->player.weapon[SPREAD] - 1)/8.0);
    }
    else if(weapon == CONTINUOUS)
    {

    }
    else if(weapon == PLASMA && world->player.weapon[PLASMA])
    {
      Vec2D velocity = direction.scale(1000);
      float power = lerp(4, 16, (world->player.weapon[PLASMA] - 1)/8.0);
      std::shared_ptr<Plasma> plasma(new Plasma(world, 0.5, power, getPosition(), velocity));
      world->sprites.insert(plasma);
      weaponCooldown = lerp(1.2, 0.8, (world->player.weapon[PLASMA] - 1)/8.0);
    }
  }

  glhckObjectPosition(shield, glhckObjectGetPosition(o));
  if(shieldLeft > 0)
  {
    shieldLeft -= delta;
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

  if(other->getEntityId() == Powerup::ID) {
    if(shieldLeft > 0)
      return;

    Powerup const* powerup = static_cast<Powerup const*>(other);
    if(shape.collidesWith(powerup->getShape()))
    {
      if(powerup->getType() == Powerup::RAPID)
      {
        world->player.weapon[RAPID] += world->player.weapon[RAPID] < 8 ? 1 : 0;
      }
      else if(powerup->getType() == Powerup::SPREAD)
      {
        world->player.weapon[SPREAD] += world->player.weapon[SPREAD] < 8 ? 1 : 0;
      }
      else if(powerup->getType() == Powerup::CONTINUOUS)
      {
        world->player.weapon[CONTINUOUS] += world->player.weapon[CONTINUOUS] < 8 ? 1 : 0;
      }
      else if(powerup->getType() == Powerup::PLASMA)
      {
        world->player.weapon[PLASMA] += world->player.weapon[PLASMA] < 8 ? 1 : 0;
      }
      else if(powerup->getType() == Powerup::EXTRALIFE)
      {
        world->player.lives += 1;
      }
      else if(powerup->getType() == Powerup::LOSELIFE)
      {
        world->player.lives -= 1;
      }
      else if(powerup->getType() == Powerup::SHIELD)
      {
        shieldLeft = 5.0;
      }
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

void Ship::setWeapon(Weapon const value)
{
  weapon = value;
}

void Ship::die()
{
  Explosion* explosion = new Explosion(world, getPosition());
  world->sprites.insert(std::shared_ptr<Explosion>(explosion));
  dead = true;
}
