#include "asteroid.h"
#include "laser.h"
#include "spark.h"
#include "world.h"
#include "util.h"
#include <cstdlib>

int const Asteroid::ID = Entity::newEntityId();
float const Asteroid::RADII[NUM_SIZES] = {8, 16, 32, 48};
std::string const Asteroid::IMAGES[NUM_SIZES] = {
  "img/asteroid_1.png",
  "img/asteroid_2.png",
  "img/asteroid_3.png",
  "img/asteroid_4.png"
};

float Asteroid::IMAGE_SIZES[NUM_SIZES] = {16, 16, 32, 48 };

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
  Sprite(world), o(0), size(size), v(velocity), life(size + 1)
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
}

bool Asteroid::alive() const
{
  return life > 0;
}

void Asteroid::collide(Sprite const* other) {
  if(!alive())
    return;

  Vec2D position = getPosition();

  if(other->getEntityId() == Asteroid::ID) {
    Asteroid const* asteroid = static_cast<Asteroid const*>(other);
    if(circlesIntersect(position, getRadius(), asteroid->getPosition(), asteroid->getRadius()))
    {
      v = (position - asteroid->getPosition()).uniti().scalei(v.length());
    }
    return;
  }

  if(other->getEntityId() == Laser::ID) {
    Laser const* laser = static_cast<Laser const*>(other);
    if(circlesIntersect(position, getRadius(), laser->getPosition(), laser->getRadius()))
    {
      life -= 0.5;

      if(alive())
      {
        Vec2D hitDirection = (laser->getPosition() - position).uniti();
        Vec2D hitPosition = position + hitDirection.scale(getRadius());
        Vec2D hitNormal = hitDirection.normal();

        for(int i = 0; i < 10; ++i)
        {
          float pLife = 0.1 + (rand() % 1000) / 10000.0f;
          float speed = 120 + (rand() % 40);
          Vec2D dev = hitNormal.scale(((rand() % 2000) - 1000) / 10.0f);
          Vec2D startPos = hitPosition + Vec2D((rand() % 9) - 4, (rand() % 9) - 4);
          Spark* spark = new Spark(world, pLife, startPos, hitDirection.scale(speed) + dev);
          world->sprites.insert(std::shared_ptr<Spark>(spark));
        }
      }
      else
      {
        world->score += size * 10;

        int r = static_cast<int>(getRadius());
        for(int i = 0; i < 2 * r; ++i)
        {
          float pLife = 0.25 + (rand() % 2500) / 10000.0f;
          float speed = 120 + (rand() % 100);
          Vec2D direction(((rand() % 2000) - 1000) / 1000.0f, ((rand() % 2000) - 1000) / 1000.0f);
          Vec2D velocity = direction.scale(speed);
          Vec2D startPos = position + direction.scale(rand() % r);
          Spark* spark = new Spark(world, pLife, startPos, velocity);
          world->sprites.insert(std::shared_ptr<Spark>(spark));
        }

        if(size > TINY)
        {
          for(int i = 0; i < 2; ++i)
          {
            float speed = 10 + (rand() % 50);
            Vec2D direction(((rand() % 2000) - 1000) / 1000.0f, ((rand() % 2000) - 1000) / 1000.0f);
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

float Asteroid::getRadius() const
{
  return RADII[size];
}
