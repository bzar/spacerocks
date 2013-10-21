#include "asteroid.h"
#include "laser.h"
#include "shot.h"
#include "plasma.h"
#include "beam.h"
#include "util/util.h"
#include "particleengine.h"
#include "ship.h"

float const Asteroid::RADII[NUM_SIZES] = {3, 6, 13, 20};
/*std::vector<std::string> const Asteroid::IMAGES  = {
  "img/asteroid_1.png",
  "img/asteroid_2.png",
  "img/asteroid_3.png",
  "img/asteroid_4.png"
};*/
std::string const Asteroid::IMAGE = "img/asteroids.png";


int const Asteroid::IMAGE_SIZES[NUM_SIZES] = {16, 16, 32, 48 };
int const Asteroid::IMAGE_BLOCK_WIDTH = 16 + 16 + 32 + 48;
int const Asteroid::IMAGE_BLOCK_HEIGHT = 48;

glhckTexture* Asteroid::texture = nullptr;
Sound Asteroid::hitSound = Sound();
Sound Asteroid::destroySoundBig = Sound();
Sound Asteroid::destroySoundSmall = Sound();

void Asteroid::init()
{
  texture = glhckTextureNewFromFile(IMAGE.data(), nullptr, glhckTextureDefaultSpriteParameters());
  glhckTextureRef(texture);
  hitSound.load("snd/sfx/explosion4.wav");
  destroySoundBig.load("snd/sfx/explosion2.wav");
  destroySoundSmall.load("snd/sfx/explosion4.wav");
}

void Asteroid::term()
{
  glhckTextureFree(texture);
}

Asteroid::Asteroid(GameWorld* world, Size const size, Vec2D const& position, Vec2D const& velocity, int theme) :
  ew::Entity(world), ew::Renderable(world), ew::Updatable(world), ew::Collidable(world),
  gameWorld(world), o(0), size(size), v(velocity), life((size + 1)/2.0f), shape(position, RADII[size]), theme(theme)
{
  int width, height;
  glhckTextureGetInformation(texture, nullptr, &width, &height, nullptr, nullptr, nullptr, nullptr);

  int blocksPerColumn = height / IMAGE_BLOCK_HEIGHT;
  int column = theme / blocksPerColumn;
  int row = theme % blocksPerColumn;

  float widthf = width;
  float heightf = height;
  o = glhckSpriteNew(texture, IMAGE_SIZES[size], IMAGE_SIZES[size]);

  float tx = column * IMAGE_BLOCK_WIDTH;
  for(int i = 0; i < size; ++i)
  {
    tx += IMAGE_SIZES[i];
  }
  float ty = row * IMAGE_BLOCK_HEIGHT;

  glhckRect rect = {tx / widthf, 1 - ty / heightf,
                    IMAGE_SIZES[size]/widthf,
                    -IMAGE_SIZES[size]/heightf};

  glhckMaterialTextureTransform(glhckObjectGetMaterial(o), &rect, 0);
  glhckObjectPositionf(o, position.x, position.y, 0);
}

Asteroid::~Asteroid()
{
  glhckObjectFree(o);
}

void Asteroid::render(ew::RenderContext* context)
{
  glhckObjectRender(o);
}

void Asteroid::update(float const delta)
{
  glhckObjectRotatef(o, 0, 0, delta * 40);

  if(gameWorld->getPaused())
    return;

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
}

CircleShape const* Asteroid::getShape() const
{
  return &shape;
}

void Asteroid::collide(ew::Collidable const* other) {
  if(life <= 0)
    return;

  Vec2D position = getPosition();

  if(typeid(*other) == typeid(Asteroid)) {
    Asteroid const* asteroid = static_cast<Asteroid const*>(other);
    if(shape.collidesWith(asteroid->getShape()))
    {
      v = (position - asteroid->getPosition()).uniti().scalei(v.length());
    }
    return;
  }

  if(typeid(*other) == typeid(Laser)
    || typeid(*other) == typeid(Shot)
    || typeid(*other) == typeid(Plasma)
    || typeid(*other) == typeid(Beam)
    || typeid(*other) == typeid(Ship))
  {
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

    if(typeid(*other) == typeid(Shot)) {
      Shot const* shot = static_cast<Shot const*>(other);
      if(shape.collidesWith(shot->getShape()))
      {
        life -= 0.5;
        p = shot->getPosition();
        collide = true;
      }
    }

    if(typeid(*other) == typeid(Plasma)) {
      Plasma const* plasma = static_cast<Plasma const*>(other);
      if(shape.collidesWith(plasma->getShape()))
      {
        life -= 1.0;
        p = plasma->getPosition();
        collide = true;
      }
    }

    if(typeid(*other) == typeid(Beam)) {
      Beam const* beam = static_cast<Beam const*>(other);
      if(beam->canHit() && shape.collidesWith(beam->getShape()))
      {
        life -= 0.5;
        p = beam->getBasePosition();
        collide = true;
      }
    }

    if(typeid(*other) == typeid(Ship)) {
      Ship const* ship = static_cast<Ship const*>(other);
      if(!ship->immortal() && shape.collidesWith(ship->getShape()))
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
        Vec2D hitPosition = position + hitDirection.scale(RADII[size]);
        Vec2D hitNormal = hitDirection.normal();

        for(int i = 0; i < 10; ++i)
        {
          float pLife = randFloat(0.1, 0.2);
          float speed = randFloat(120, 160);
          float scale = randFloat(1, 4);
          Vec2D dev = hitNormal.scale(randFloat(-100, 100));
          Vec2D startPos = hitPosition + Vec2D((rand() % 9) - 4, (rand() % 9) - 4);
          gameWorld->particleEngine->addParticle(ParticleEngine::SPARK, startPos, hitDirection.scale(speed) + dev, pLife, 0, 1, 1, scale, 1, -1);
        }
        hitSound.stopAndPlay();
      }
      else
      {
        gameWorld->particleEngine->addParticle(ParticleEngine::CORONA, position, {0, 0}, 1, 0, 0, (size+1)/4.0f, 0, 0.1, -0.1);
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

Vec2D Asteroid::getVelocity() const
{
  return v;
}

float Asteroid::getLife() const
{
  return life;
}

void Asteroid::die()
{
  gameWorld->removeEntity(this);

  Vec2D position = getPosition();
  gameWorld->addScore((size + 1) * 10, position);

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
    for(int i = 0; i < gameWorld->level.asteroidFragCount; ++i)
    {
      float speed = randFloat(gameWorld->level.minAsteroidSpeed, gameWorld->level.maxAsteroidSpeed);
      Vec2D direction = Vec2D(randFloat(-1, 1), randFloat(-1, 1)).uniti();
      Vec2D velocity = direction.scale(speed);
      Vec2D startPos = position + direction.scale(rand() % r);
      Asteroid* asteroid = new Asteroid(gameWorld, static_cast<Size>(size - 1), startPos, velocity, theme);
    }
  }

  if(size >= MEDIUM)
  {
    destroySoundBig.stopAndPlay();
  }
  else
  {
    destroySoundSmall.stopAndPlay();
  }
}
