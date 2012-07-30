#include "asteroid.h"
#include "laser.h"
#include "spark.h"
#include "world.h"
#include "util.h"
#include <cstdlib>

int const Asteroid::ID = Entity::newEntityId();
float const Asteroid::RADII[NUM_SIZES] = {4, 8, 16, 24};
std::string const Asteroid::IMAGES[NUM_SIZES] = {
  "img/asteroid_1.png",
  "img/asteroid_2.png",
  "img/asteroid_3.png",
  "img/asteroid_4.png"
};

float Asteroid::IMAGE_SIZES[NUM_SIZES] = {8, 8, 16, 24 };

std::vector<Sprite::TransformData> Asteroid::TRANSFORM;
glhckTexture *Asteroid::TEXTURE = NULL;

void Asteroid::init()
{
  glhckAtlas *TEXTURES = glhckAtlasNew();
  for(int i = 0; i < NUM_SIZES; ++i)
  {
    glhckTexture* texture = glhckTextureNew(IMAGES[i].data(), GLHCK_TEXTURE_DEFAULTS);
    glhckAtlasInsertTexture(TEXTURES, texture);
    glhckTextureFree(texture);
  }
  glhckAtlasPack(TEXTURES, true, true);

  for(int i = 0; i < NUM_SIZES; ++i)
  {
    TransformData t;
    glhckAtlasGetTransform(TEXTURES, glhckAtlasGetTextureByIndex(TEXTURES, i), &t.transform, &t.degree);
    TRANSFORM.push_back(t);
  }

  TEXTURE = glhckTextureRef(glhckAtlasGetTexture(TEXTURES));
  glhckAtlasFree(TEXTURES);
}

Asteroid::Asteroid(World* world, Size const size, Vec2D const& position, Vec2D const& velocity) :
  Sprite(world), o(0), size(size), v(velocity), life(size + 1), shape(position, RADII[size])
{
  o = glhckSpriteNew(TEXTURE, IMAGE_SIZES[size], IMAGE_SIZES[size]);
  glhckObjectTransformCoordinates(o, &TRANSFORM[size].transform, TRANSFORM[size].degree);
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

void Asteroid::update(float delta)
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

bool Asteroid::alive() const
{
  return life > 0;
}

CircleShape const* Asteroid::getShape() const
{
  return &shape;
}

void Asteroid::collide(Sprite const* other) {
  if(!alive())
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

  if(other->getEntityId() == Laser::ID) {
    Laser const* laser = static_cast<Laser const*>(other);
    if(shape.collidesWith(laser->getShape()))
    {
      life -= 0.5;

      if(alive())
      {
        Vec2D hitDirection = (laser->getPosition() - position).uniti();
        Vec2D hitPosition = position + hitDirection.scale(RADII[size]);
        Vec2D hitNormal = hitDirection.normal();

        for(int i = 0; i < 10; ++i)
        {
          float pLife = randFloat(0.1, 0.2);
          float speed = randFloat(120, 160);
          Vec2D dev = hitNormal.scale(randFloat(-100, 100));
          Vec2D startPos = hitPosition + Vec2D((rand() % 9) - 4, (rand() % 9) - 4);
          Spark* spark = new Spark(world, pLife, startPos, hitDirection.scale(speed) + dev);
          world->sprites.insert(std::shared_ptr<Spark>(spark));
        }
      }
      else
      {
        world->score += (size + 1) * 10;

        int r = static_cast<int>(RADII[size]);
        for(int i = 0; i < 2 * r; ++i)
        {
          float pLife = randFloat(0.25, 0.5);
          float speed = randFloat(120, 220);
          Vec2D direction = Vec2D(randFloat(-1, 1), randFloat(-1, 1)).uniti();
          Vec2D velocity = direction.scale(speed);
          Vec2D startPos = position + direction.scale(rand() % r);
          Spark* spark = new Spark(world, pLife, startPos, velocity);
          world->sprites.insert(std::shared_ptr<Spark>(spark));
        }

        if(size > TINY)
        {
          for(int i = 0; i < 2; ++i)
          {
            float speed = randFloat(10, 60);
            Vec2D direction = Vec2D(randFloat(-1, 1), randFloat(-1, 1)).uniti();
            Vec2D velocity = direction.scale(speed);
            Vec2D startPos = position + direction.scale(rand() % r);
            Asteroid* asteroid = new Asteroid(world, static_cast<Size>(size - 1), startPos, velocity);
            world->sprites.insert(std::shared_ptr<Asteroid>(asteroid));
          }
        }
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
