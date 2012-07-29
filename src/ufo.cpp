#include "ufo.h"
#include "laser.h"
#include "util.h"
#include "explosion.h"
#include "world.h"
#include "spark.h"

int const Ufo::ID = Entity::newEntityId();

int const Ufo::ANIMATION_FRAMES[NUM_FRAMES] = {1, 1, 1, 1, 1, 1, 1, 1, 2, 3};

std::string const Ufo::IMAGES[NUM_IMAGES] = {
  "img/ufo_1.png",
  "img/ufo_2.png",
  "img/ufo_3.png",
  "img/ufo_4.png",
};

std::vector<Sprite::TransformData> Ufo::TRANSFORM;
glhckTexture *Ufo::TEXTURE = NULL;

void Ufo::init()
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

Ufo::Ufo(World* world, Vec2D const& position) :
  Sprite(world, 1), o(0), time(0), life(3)
{
  o = glhckSpriteNew(TEXTURE, 24, 24);
  glhckObjectTransformCoordinates(o, &TRANSFORM[0].transform, TRANSFORM[0].degree);

  glhckObjectSetMaterialFlags(o, GLHCK_MATERIAL_ALPHA);
  glhckObjectPositionf(o, position.x, position.y, 0);
}

Ufo::~Ufo()
{
  glhckObjectFree(o);
}


void Ufo::render()
{
  glhckObjectRender(o);
}

void Ufo::update(float delta)
{
  time += delta;
  int frame = ANIMATION_FRAMES[static_cast<int>(time * FPS) % NUM_FRAMES];
  glhckObjectTransformCoordinates(o, &TRANSFORM[frame].transform, TRANSFORM[frame].degree);
  glhckObjectRotatef(o, 0, 0, delta * 180);
}

bool Ufo::alive() const
{
  return life > 0;
}

void Ufo::collide(Sprite const* other) {
  kmVec3 const* pos = glhckObjectGetPosition(o);
  Vec2D position{pos->x, pos->y};

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
          Spark* spark = new Spark(world, pLife, hitPosition, hitDirection.scale(speed) + dev);
          world->sprites.insert(std::shared_ptr<Spark>(spark));
        }
      }
      else
      {
        Explosion* explosion = new Explosion(world, position);
        world->sprites.insert(std::shared_ptr<Explosion>(explosion));
      }
    }
    return;
  }
}

Vec2D Ufo::getPosition() const
{
  kmVec3 const* pos = glhckObjectGetPosition(o);
  return {pos->x, pos->y};
}

float Ufo::getRadius() const
{
  return RADIUS;
}

