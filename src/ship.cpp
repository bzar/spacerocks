#include "ship.h"
#include "asteroid.h"
#include "explosion.h"
#include "ufo.h"
#include "ufolaser.h"
#include "powerup.h"
#include "gamenotification.h"
#include "particleengine.h"
#include "util/util.h"

#include "GLFW/glfw3.h"

std::vector<std::string> const Ship::IMAGES = {
  "img/ship-rapid.png", "img/ship-rapid_accelerating.png",
  "img/ship-rapid_left.png", "img/ship-rapid_left_accelerating.png",
  "img/ship-rapid_right.png", "img/ship-rapid_right_accelerating.png",
  "img/ship-spread.png", "img/ship-spread_accelerating.png",
  "img/ship-spread_left.png", "img/ship-spread_left_accelerating.png",
  "img/ship-spread_right.png", "img/ship-spread_right_accelerating.png",
  "img/ship-beam.png", "img/ship-beam_accelerating.png",
  "img/ship-beam_left.png", "img/ship-beam_left_accelerating.png",
  "img/ship-beam_right.png", "img/ship-beam_right_accelerating.png",
  "img/ship-plasma.png", "img/ship-plasma_accelerating.png",
  "img/ship-plasma_left.png", "img/ship-plasma_left_accelerating.png",
  "img/ship-plasma_right.png", "img/ship-plasma_right_accelerating.png",
  "img/shield.png"
};

TextureAtlas Ship::atlas = TextureAtlas();
Sound Ship::destroySound = Sound();
Sound Ship::engineSound = Sound();

namespace
{
  float const POWERUP_TEXT_SIZE = 14;
}

void Ship::init()
{
  atlas = TextureAtlas(IMAGES);
  destroySound.load("snd/sfx/explosion2.wav");
  engineSound.load("snd/sfx/enginehum3.ogg");
}

void Ship::term()
{
  atlas = TextureAtlas();
}

Ship::Ship(GameWorld* world, Vec2D const& position, Vec2D const& velocity) :
  ew::Entity(world), ew::Renderable(world), ew::Updatable(world), ew::Collidable(world), ew::Controllable(world),
  gameWorld(world), o(nullptr), shield(nullptr), v(velocity),
  turningLeft(false), turningRight(false), accelerating(false), shooting(false),
  immortalityLeft(4), shields(0),
  weapon(nullptr), laser(world), spread(world), beam(world), plasma(world),
  weapons({&laser, &spread, &beam, &plasma}),
  dead(false), shape(position, RADIUS)
{
  o = glhckSpriteNew(atlas.getTexture(), 32, 32);
  shield = glhckSpriteNew(atlas.getTexture(), 48, 48);

  glhckMaterialTextureTransform(glhckObjectGetMaterial(o), &atlas.getTransform(DEFAULT).transform, atlas.getTransform(DEFAULT).degree);
  glhckMaterialTextureTransform(glhckObjectGetMaterial(shield), &atlas.getTransform(SHIELD).transform, atlas.getTransform(SHIELD).degree);

  glhckObjectPositionf(o, position.x, position.y, 0);

  weapon = &laser;
  laser.setLevel(1);
}

Ship::~Ship()
{
  glhckObjectFree(o);
  glhckObjectFree(shield);
}


void Ship::render(ew::RenderContext* context)
{
  if(dead)
    return;

  glhckMaterial *m = glhckObjectGetMaterial(o);
  if (m) glhckMaterialDiffuseb(m, 255, 255, 255, immortalityLeft > 0 ? 128 : 255);
  glhckObjectRender(o);

  if(shields > 0)
  {
    glhckObjectRender(shield);
  }
}

void Ship::update(float const delta)
{
  if(gameWorld->getPaused())
    return;

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

  if(v.lengthSquared() > MAX_SPEED * MAX_SPEED)
  {
    v.uniti().scalei(MAX_SPEED);
  }

  ActionType at = DEFAULT;
  if(accelerating) {
    at = turningLeft && !turningRight ? LEFT_ACCELERATING :
        turningRight && !turningLeft ? RIGHT_ACCELERATING :
        ACCELERATING;
  } else {
    at = turningLeft && !turningRight ? LEFT :
        turningRight && !turningLeft ? RIGHT :
        DEFAULT;
  }

  WeaponType wt;
  if(weapon->getWeaponId() == LaserWeapon::ID)
    wt = RAPID;
  else if(weapon->getWeaponId() == SpreadWeapon::ID)
    wt = SPREAD;
  else if(weapon->getWeaponId() == BeamWeapon::ID)
    wt = BEAM;
  else
    wt = PLASMA;

  ImageType it = static_cast<ImageType>(wt * NUM_ACTIONS + at);

  glhckMaterialTextureTransform(glhckObjectGetMaterial(o), &atlas.getTransform(it).transform, atlas.getTransform(it).degree);
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
    weapon->shoot(getPosition() + direction.scale(RADIUS), direction);
  }

  glhckObjectPosition(shield, glhckObjectGetPosition(o));
  if(immortalityLeft > 0)
  {
    immortalityLeft -= delta;
  }
}

void Ship::control(ew::ControlContext* context)
{
  if(gameWorld->getPaused())
    return;

  turnLeft(context->keyDown(GLFW_KEY_LEFT));
  turnRight(context->keyDown(GLFW_KEY_RIGHT));
  accelerate(context->keyDown(GLFW_KEY_UP));
  shoot(context->keyDown(GLFW_KEY_SPACE)||context->keyDown(GLFW_KEY_END));

  if(context->keyPush(GLFW_KEY_Z) || context->keyPush(GLFW_KEY_RIGHT_SHIFT))
    prevWeapon();

  if(context->keyPush(GLFW_KEY_X) || context->keyPush(GLFW_KEY_RIGHT_CONTROL))
    nextWeapon();

  if(context->keyPush(GLFW_KEY_F1))
    increaseLaserLevel();
  if(context->keyPush(GLFW_KEY_F2))
    increaseSpreadLevel();
  if(context->keyPush(GLFW_KEY_F3))
    increaseBeamLevel();
  if(context->keyPush(GLFW_KEY_F4))
    increasePlasmaLevel();
  if(context->keyPush(GLFW_KEY_F5))
    shields += 1;
}

bool Ship::alive() const
{
  return !dead;
}

bool Ship::immortal() const
{
  return immortalityLeft > 0;
}

CircleShape const* Ship::getShape() const
{
  return &shape;
}


void Ship::collide(ew::Collidable const* other) {
  if(dead)
    return;

  Vec2D position = getPosition();

  if(typeid(*other) == typeid(Asteroid)) {
    if(immortalityLeft > 0)
      return;

    Asteroid const* asteroid = static_cast<Asteroid const*>(other);
    if(shape.collidesWith(asteroid->getShape()))
    {
      if(shields <= 0)
      {
        die();
      }
      else
      {
        shields -= 1;
        float speed = 2 * asteroid->getVelocity().subtract(v).length();
        v = position.subtract(asteroid->getPosition()).uniti().scale(speed);
      }
    }
    return;
  }

  if(typeid(*other) == typeid(UfoLaser)) {
    if(immortalityLeft > 0)
      return;

    UfoLaser const* ufoLaser = static_cast<UfoLaser const*>(other);
    if(shape.collidesWith(ufoLaser->getShape()))
    {
      if(shields <= 0)
      {
        die();
      }
      else
      {
        shields -= 1;
        v += ufoLaser->getVelocity().scale(0.1);
      }
    }
    return;
  }

  if(typeid(*other) == typeid(Ufo)) {
    if(immortalityLeft > 0)
      return;

    Ufo const* ufo = static_cast<Ufo const*>(other);
    if(shape.collidesWith(ufo->getShape()))
    {
      if(shields <= 0)
      {
        die();
      }
      else
      {
        shields -= 1;
      }
    }
    return;
  }

  if(typeid(*other) == typeid(Powerup)) {
    Powerup const* powerup = static_cast<Powerup const*>(other);
    if(powerup->alive() && shape.collidesWith(powerup->getShape()))
    {
      if(powerup->getType() == Powerup::LASER)
      {
        increaseLaserLevel();
      }
      else if(powerup->getType() == Powerup::SPREAD)
      {
        increaseSpreadLevel();
      }
      else if(powerup->getType() == Powerup::BEAM)
      {
        increaseBeamLevel();
      }
      else if(powerup->getType() == Powerup::PLASMA)
      {
        increasePlasmaLevel();
      }
      else if(powerup->getType() == Powerup::EXTRALIFE)
      {
        gameWorld->player.lives += 1;
        new GameNotification(gameWorld, "1up", POWERUP_TEXT_SIZE, 1.0, getPosition());

      }
      else if(powerup->getType() == Powerup::LOSELIFE)
      {
        gameWorld->player.lives -= 1;
        new GameNotification(gameWorld, "-1up", POWERUP_TEXT_SIZE, 1.0, getPosition());

      }
      else if(powerup->getType() == Powerup::SHIELD)
      {
        shields += 1;
        new GameNotification(gameWorld, "Shield +1", POWERUP_TEXT_SIZE, 1.0, getPosition());
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
  if(value != accelerating)
  {
    accelerating = value;
    if(value)
    {
      engineSound.play(0, 0, -1);
    }
    else
    {
      engineSound.fadeOut(500);
    }
  }
}

void Ship::shoot(bool const value)
{
  if(shooting != value)
  {
    shooting = value && alive();
    if(shooting)
      weapon->startShooting();
    else
      weapon->stopShooting();
  }
}

void Ship::reset()
{
  dead = false;
  glhckObjectPositionf(o, 0, 0, 0);
  glhckObjectRotationf(o, 0, 0, 0);
  glhckObjectPositionf(shield, 0, 0, 0);
  glhckObjectRotationf(shield, 0, 0, 0);
  v = {0, 0};
  immortalityLeft = 4.0f;
  weapon->stopShooting();
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
  weapon->stopShooting();
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
  weapon->stopShooting();
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

void Ship::increaseLaserLevel() {
  laser.increaseLevel();
  new GameNotification(gameWorld, "Laser +1", POWERUP_TEXT_SIZE, 1.0, getPosition());
}
void Ship::increaseSpreadLevel() {
  spread.increaseLevel();
  new GameNotification(gameWorld, "Spread +1", POWERUP_TEXT_SIZE, 1.0, getPosition());
}
void Ship::increaseBeamLevel() {
  beam.increaseLevel();
  new GameNotification(gameWorld, "Beam +1", POWERUP_TEXT_SIZE, 1.0, getPosition());
}
void Ship::increasePlasmaLevel() {
  plasma.increaseLevel();
  new GameNotification(gameWorld, "Plasma +1", POWERUP_TEXT_SIZE, 1.0, getPosition());
}

int Ship::getSelectedWeaponId() const
{
  return weapon->getWeaponId();
}

void Ship::die()
{
  weapon->stopShooting();
  new Explosion(gameWorld, getPosition());
  gameWorld->particleEngine->addParticle(ParticleEngine::WAVE, getPosition(), Vec2D(), 1, 0, 0, 0.1, 3, 0.1, -0.1);
  dead = true;

  for(Weapon* w : weapons)
  {
    w->decreaseLevel();
  }

  destroySound.play();
}

