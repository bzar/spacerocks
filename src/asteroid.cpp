#include "asteroid.h"
#include "laser.h"
#include "shot.h"
#include "plasma.h"
#include "beam.h"
#include "world.h"
#include "util.h"
#include "particleengine.h"

Entity::Id const Asteroid::ID = Entity::newEntityId();
float const Asteroid::RADII[NUM_SIZES] = {3, 6, 13, 20};
std::vector<std::string> const Asteroid::IMAGES  = {
  "img/asteroid_1.png",
  "img/asteroid_2.png",
  "img/asteroid_3.png",
  "img/asteroid_4.png"
};

float const Asteroid::IMAGE_SIZES[NUM_SIZES] = {8, 8, 16, 24 };

TextureAtlas Asteroid::atlas = TextureAtlas();

void Asteroid::init()
{
  atlas = TextureAtlas(IMAGES);
}

void Asteroid::term()
{
  atlas = TextureAtlas();
}

Asteroid::Asteroid(GameWorld* world, Size const size, Vec2D const& position, Vec2D const& velocity) :
  Entity(world), Renderable(world), Updatable(world), Collidable(world),
  gameWorld(world), o(0), size(size), v(velocity), life((size + 1)/2.0f), shape(position, RADII[size])
{
  o = glhckSpriteNew(atlas.getTexture(), IMAGE_SIZES[size], IMAGE_SIZES[size]);
  glhckObjectTransformCoordinates(o, &atlas.getTransform(size).transform, atlas.getTransform(size).degree);
  glhckObjectSetMaterialFlags(o, GLHCK_MATERIAL_ALPHA);
  glhckObjectPositionf(o, position.x, position.y, 0);
}

Asteroid::~Asteroid()
{
  glhckObjectFree(o);
}

void Asteroid::render()
{
  glhckObjectRender(o);
}

void Asteroid::update(float const delta)
{
  glhckObjectMovef(o, v.x * delta, v.y * delta, 0);
  glhckObjectRotatef(o, 0, 0, delta * 40);

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

CircleShape const* Asteroid::getShape() const
{
  return &shape;
}

void Asteroid::collide(Collidable const* other) {
  if(!life > 0)
    return;

  Vec2D position = getPosition();

  if(other->getEntityId() == Asteroid::ID) {
    Asteroid const* asteroid = static_cast<Asteroid const*>(other);
    if(shape.collidesWith(asteroid->getShape()))
    {
      v = (position - asteroid->getPosition()).uniti().scalei(v.length());
    }
    return;
  }

  if(other->getEntityId() == Laser::ID
    || other->getEntityId() == Shot::ID
    || other->getEntityId() == Plasma::ID
    || other->getEntityId() == Beam::ID)
  {
    bool collide = false;
    Vec2D p;

    if(other->getEntityId() == Laser::ID) {
      Laser const* laser = static_cast<Laser const*>(other);
      if(shape.collidesWith(laser->getShape()))
      {
        life -= 0.5;
        p = laser->getPosition();
        collide = true;
      }
    }

    if(other->getEntityId() == Shot::ID) {
      Shot const* shot = static_cast<Shot const*>(other);
      if(shape.collidesWith(shot->getShape()))
      {
        life -= 0.5;
        p = shot->getPosition();
        collide = true;
      }
    }

    if(other->getEntityId() == Plasma::ID) {
      Plasma const* plasma = static_cast<Plasma const*>(other);
      if(shape.collidesWith(plasma->getShape()))
      {
        life -= 1.0;
        p = plasma->getPosition();
        collide = true;
      }
    }

    if(other->getEntityId() == Beam::ID) {
      Beam const* beam = static_cast<Beam const*>(other);
      if(beam->canHit() && shape.collidesWith(beam->getShape()))
      {
        life -= 0.5;
        p = beam->getBasePosition();
        collide = true;
      }
    }

    if(collide)
    {
      if(life > 0)
      {
        Vec2D hitDirection = (p - position).uniti();
        Vec2D hitPosition = position + hitDirection.scale(RADII[size]);
        Vec2D hitNormal = hitDirection.normal();

        for(int i = 0; i < 10; ++i)
        {
          float pLife = randFloat(0.1, 0.2);
          float speed = randFloat(120, 160);
          Vec2D dev = hitNormal.scale(randFloat(-100, 100));
          Vec2D startPos = hitPosition + Vec2D((rand() % 9) - 4, (rand() % 9) - 4);
          gameWorld->particleEngine->addParticle(ParticleEngine::SPARK, startPos, hitDirection.scale(speed) + dev, pLife);
        }
      }
      else
      {
        die();
      }

    }

    return;
  }
}

Vec2D Asteroid::getPosition() const
{
  kmVec3 const* pos = glhckObjectGetPosition(o);
  return {pos->x, pos->y};
}

float Asteroid::getLife() const
{
  return life;
}

void Asteroid::die()
{
  gameWorld->removeEntity(this);

  Vec2D position = getPosition();
  gameWorld->player.score += (size + 1) * 10;

  int r = static_cast<int>(RADII[size]);
  for(int i = 0; i < 2 * r; ++i)
  {
    float pLife = randFloat(0.25, 0.5);
    float speed = randFloat(120, 220);
    Vec2D direction = Vec2D(randFloat(-1, 1), randFloat(-1, 1)).uniti();
    Vec2D velocity = direction.scale(speed);
    Vec2D startPos = position + direction.scale(rand() % r);
    gameWorld->particleEngine->addParticle(ParticleEngine::SPARK, startPos, velocity, pLife);
  }

  if(size > TINY)
  {
    for(int i = 0; i < 2; ++i)
    {
      float speed = randFloat(gameWorld->level.minAsteroidSpeed, gameWorld->level.maxAsteroidSpeed);
      Vec2D direction = Vec2D(randFloat(-1, 1), randFloat(-1, 1)).uniti();
      Vec2D velocity = direction.scale(speed);
      Vec2D startPos = position + direction.scale(rand() % r);
      Asteroid* asteroid = new Asteroid(gameWorld, static_cast<Size>(size - 1), startPos, velocity);
    }
  }
}