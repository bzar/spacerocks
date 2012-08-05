#include "ship.h"
#include "asteroid.h"
#include "explosion.h"
#include "world.h"
#include "util.h"
#include "ufo.h"
#include "ufolaser.h"
#include "powerup.h"

int const Ship::ID = Entity::newEntityId();

std::vector<std::string> const Ship::IMAGES = {
  "img/ship.png", "img/ship_accelerating.png",
  "img/ship_left.png", "img/ship_left_accelerating.png",
  "img/ship_right.png", "img/ship_right_accelerating.png",
  "img/shield.png"
};

TextureAtlas Ship::atlas = TextureAtlas();

void Ship::init()
{
  atlas = TextureAtlas(IMAGES);
}

Ship::Ship(World* world, Vec2D const& position, Vec2D const& velocity) :
  Sprite(world, 1), o(nullptr), shield(nullptr), v(velocity),
  turningLeft(false), turningRight(false), accelerating(false), shooting(false),
  shieldLeft(4),
  weapon(nullptr), laser(world), spread(world), beam(world), plasma(world),
  weapons({&laser, &spread, &beam, &plasma}),
  dead(false), shape(position, RADIUS)
{
  o = glhckSpriteNew(atlas.getTexture(), 16, 16);
  shield = glhckSpriteNew(atlas.getTexture(), 24, 24);

  glhckObjectTransformCoordinates(o, &atlas.getTransform(DEFAULT).transform, atlas.getTransform(DEFAULT).degree);
  glhckObjectTransformCoordinates(shield, &atlas.getTransform(SHIELD).transform, atlas.getTransform(SHIELD).degree);

  glhckObjectSetMaterialFlags(o, GLHCK_MATERIAL_ALPHA);
  glhckObjectSetMaterialFlags(shield, GLHCK_MATERIAL_ALPHA);
  glhckObjectPositionf(o, position.x, position.y, 0);

  weapon = &laser;
  laser.setLevel(1);
}

Ship::~Ship()
{
  glhckObjectFree(o);
}


void Ship::render()
{
  if(dead)
    return;

  glhckObjectRender(o);

  if(shieldLeft > 0)
  {
    glhckObjectRender(shield);
  }
}

void Ship::update(float delta)
{
  if(dead)
    return;

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

  glhckObjectTransformCoordinates(o, &atlas.getTransform(t).transform, atlas.getTransform(t).degree);
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

  for(Weapon* weapon : weapons)
  {
    weapon->update(delta);
  }

  if(shooting)
  {
    Vec2D direction = Vec2D(0, 1).rotatei(glhckObjectGetRotation(o)->z / 360);
    weapon->shoot(getPosition(), direction);
  }

  glhckObjectPosition(shield, glhckObjectGetPosition(o));
  if(shieldLeft > 0)
  {
    shieldLeft -= delta;
  }
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
  if(dead)
    return;

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
      if(powerup->getType() == Powerup::LASER)
      {
        laser.increaseLevel();
      }
      else if(powerup->getType() == Powerup::SPREAD)
      {
        spread.increaseLevel();
      }
      else if(powerup->getType() == Powerup::BEAM)
      {
        beam.increaseLevel();
      }
      else if(powerup->getType() == Powerup::PLASMA)
      {
        plasma.increaseLevel();
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
  if(shooting != value)
  {
    if(value)
      weapon->startShooting();
    else
      weapon->stopShooting();
    shooting = value;
  }
}

void Ship::reset()
{
  dead = false;
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

void Ship::nextWeapon()
{
  int current = 0;
  for(int i = 0; i < weapons.size(); ++i)
  {
    if(weapons.at(i) == weapon)
    {
      current = i;
      break;
    }
  }

  for(int i = 1; i < weapons.size(); ++i)
  {
    Weapon* w = weapons.at((current + i) % weapons.size());
    if(w->getLevel() > 0)
    {
      weapon = w;
      break;
    }
  }
}

void Ship::prevWeapon()
{
  int current = 0;
  for(int i = 0; i < weapons.size(); ++i)
  {
    if(weapons.at(i) == weapon)
    {
      current = i;
      break;
    }
  }

  for(int i = weapons.size() - 1; i > 0 ; --i)
  {
    Weapon* w = weapons.at((current + i) % weapons.size());
    if(w->getLevel() > 0)
    {
      weapon = w;
      break;
    }
  }
}

void Ship::die()
{
  Explosion* explosion = new Explosion(world, getPosition());
  world->addSprite(explosion);
  dead = true;

  for(Weapon* w : weapons)
  {
    w->decreaseLevel();
  }
}
