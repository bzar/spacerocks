#include "ufo.h"
#include "laser.h"
#include "shot.h"
#include "plasma.h"
#include "beam.h"
#include "explosion.h"
#include "ufolaser.h"
#include "powerup.h"
#include "ship.h"
#include "particleengine.h"
#include "util/util.h"

float const TAU = 2 * 3.14159265;

int const Ufo::ANIMATION_FRAMES[NUM_FRAMES] = {1, 1, 1, 1, 1, 1, 1, 1, 2, 3};

std::vector<std::string> const Ufo::IMAGES = {
  "img/ufo_1.png",
  "img/ufo_2.png",
  "img/ufo_3.png",
  "img/ufo_4.png",
};

TextureAtlas Ufo::atlas = TextureAtlas();
Sound Ufo::shootSound = Sound();
Sound Ufo::hitSound = Sound();
Sound Ufo::destroySound = Sound();

void Ufo::init()
{
  atlas = TextureAtlas(IMAGES);
  shootSound.load("snd/sfx/weaponfire9.wav");
  hitSound.load("snd/sfx/weaponfire3.wav");
  destroySound.load("snd/sfx/explosion1.wav");
}

void Ufo::term()
{
  atlas = TextureAtlas();
}

Ufo::Ufo(GameWorld* world, Vec2D const& startPosition, Vec2D const& endPosition,
         float freq, float amplitude) :
  ew::Entity(world), ew::Renderable(world), ew::Updatable(world), ew::Collidable(world),
  gameWorld(world), o(0), startPosition(startPosition), endPosition(endPosition),
  freq(freq), amplitude(amplitude),
  duration(gameWorld->level.ufoDuration),
  accuracy(gameWorld->level.ufoAccuracy),
  shootInterval(gameWorld->level.ufoShootInterval),
  time(0), life(3), shape(startPosition, RADIUS)
{
  o = glhckSpriteNew(atlas.getTexture(), 32, 32);
  glhckMaterialTextureTransform(glhckObjectGetMaterial(o), &atlas.getTransform(0).transform, atlas.getTransform(0).degree);
  glhckObjectPositionf(o, startPosition.x, startPosition.y, 0);
}

Ufo::~Ufo()
{
  glhckObjectFree(o);
}


void Ufo::render(ew::RenderContext* context)
{
  glhckObjectRender(o);
}

void Ufo::update(float const delta)
{
  if(gameWorld->getPaused())
    return;

  time += delta;
  int const frame = ANIMATION_FRAMES[static_cast<int>(time * FPS) % NUM_FRAMES];
  glhckMaterialTextureTransform(glhckObjectGetMaterial(o), &atlas.getTransform(frame).transform, atlas.getTransform(frame).degree);
  glhckObjectRotatef(o, 0, 0, delta * 180);

  Vec2D journey = endPosition - startPosition;
  float const deviation = amplitude * sin(freq * TAU * time / duration);
  Vec2D position = startPosition
    + journey.scale(time / duration)
    + journey.normal().uniti().scale(deviation);
  glhckObjectPositionf(o, position.x, position.y, 0);

  bool timeToShoot = static_cast<int>((time - delta) / shootInterval) !=
    static_cast<int>(time / shootInterval);

  if(timeToShoot && gameWorld->player.ship != nullptr)
  {
    Vec2D direction = (gameWorld->player.ship->getPosition() - getPosition()).uniti();
    float const spread = (1 - accuracy)/2;
    direction.rotatei(randFloat(-spread, spread));
    Vec2D velocity = direction.scale(600);
    UfoLaser* laser = new UfoLaser(gameWorld, 5.0f, getPosition(), velocity);
    shootSound.stopAndPlay();
  }

  shape.center = getPosition();

  if(life <= 0 || time > duration)
    gameWorld->removeEntity(this);
}

CircleShape const* Ufo::getShape() const
{
  return &shape;
}

void Ufo::collide(ew::Collidable const* other) {
  if(life <= 0)
    return;

  Vec2D position = getPosition();

  bool collide = false;
  Vec2D p;

  if(typeid(*other) == typeid(Laser)) {
    Laser const* laser = static_cast<Laser const*>(other);
    if(shape.collidesWith(laser->getShape()))
    {
      life -= 0.5;
      p = laser->getPosition();
      collide = true;
    }
  }

  else if(typeid(*other) == typeid(Shot)) {
    Shot const* shot = static_cast<Shot const*>(other);
    if(shape.collidesWith(shot->getShape()))
    {
      life -= 0.5;
      p = shot->getPosition();
      collide = true;
    }
  }

  else if(typeid(*other) == typeid(Plasma)) {
    Plasma const* plasma = static_cast<Plasma const*>(other);
    if(shape.collidesWith(plasma->getShape()))
    {
      life -= 1.0;
      p = plasma->getPosition();
      collide = true;
    }
  }

  else if(typeid(*other) == typeid(Beam)) {
    Beam const* beam = static_cast<Beam const*>(other);
    if(beam->canHit() && shape.collidesWith(beam->getShape()))
    {
      life -= 0.5;
      p = beam->getBasePosition();
      collide = true;
    }
  }

  else if(typeid(*other) == typeid(Ship)) {
    Ship const* ship = static_cast<Ship const*>(other);
    if(ship->alive() && shape.collidesWith(ship->getShape()))
    {
      life = 0;
      p = ship->getPosition();
      collide = true;
    }
  }

  if(collide)
  {
    if(life > 0)
    {
      Vec2D hitDirection = (p - position).uniti();
      Vec2D hitPosition = position + hitDirection.scale(RADIUS);
      Vec2D hitNormal = hitDirection.normal();

      for(int i = 0; i < 10; ++i)
      {
        float pLife = 0.1 + randFloat(0, 0.1);
        float speed = 120 + (rand() % 40);
        float scale = randFloat(1, 4);
        Vec2D dev = hitNormal.scale(randFloat(-100, 100));
        Vec2D startPos = hitPosition + Vec2D((rand() % 9) - 4, (rand() % 9) - 4);
        gameWorld->particleEngine->addParticle(ParticleEngine::SPARK, startPos, hitDirection.scale(speed) + dev, pLife, 0, 1, 1, scale, 1, -1);
      }

      hitSound.stopAndPlay();
    }
    else
    {
      gameWorld->removeEntity(this);
      gameWorld->addScore(100, position);

      Powerup::Type powerupType = static_cast<Powerup::Type>(randInt(0, Powerup::NUM_TYPES - 1));
      Vec2D velocity = Vec2D(0, 1).rotatei(randFloat(0, 1)).scalei(randFloat(30, 80));
      Powerup* powerup = new Powerup(gameWorld, powerupType, position, velocity);
      Explosion* explosion = new Explosion(gameWorld, position);
      gameWorld->particleEngine->addParticle(ParticleEngine::WAVE, position, Vec2D(), 1, 0, 0, 0.1, 3, 0.1, -0.1);

      destroySound.stopAndPlay();
    }
  }
}

Vec2D Ufo::getPosition() const
{
  kmVec3 const* pos = glhckObjectGetPosition(o);
  return {pos->x, pos->y};
}

float Ufo::getLife() const
{
  return life;
}
