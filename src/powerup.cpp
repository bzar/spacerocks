#include "powerup.h"
#include "ship.h"
#include "world.h"
#include "util.h"
#include <cstdlib>

int const Powerup::ID = Entity::newEntityId();
float const Powerup::RADIUS = 16;
std::string const Powerup::IMAGES[NUM_TYPES] = {
  "img/powerup_rapidfire.png",
  "img/powerup_spread.png",
  "img/powerup_continuous.png",
  "img/powerup_plasma.png",
  "img/powerup_extralife.png",
  "img/powerup_loselife.png",
  "img/powerup_shield.png"
};

std::vector<Sprite::TransformData> Powerup::TRANSFORM;
glhckTexture *Powerup::TEXTURE = NULL;

void Powerup::init()
{
  glhckAtlas *TEXTURES = glhckAtlasNew();
  for(int i = 0; i < NUM_TYPES; ++i)
  {
    glhckTexture* texture = glhckTextureNew(IMAGES[i].data(), GLHCK_TEXTURE_DEFAULTS);
    glhckAtlasInsertTexture(TEXTURES, texture);
    glhckTextureFree(texture);
  }
  glhckAtlasPack(TEXTURES, true, true);

  for(int i = 0; i < NUM_TYPES; ++i)
  {
    TransformData t;
    glhckAtlasGetTransform(TEXTURES, glhckAtlasGetTextureByIndex(TEXTURES, i), &t.transform, &t.degree);
    TRANSFORM.push_back(t);
  }

  TEXTURE = glhckTextureRef(glhckAtlasGetTexture(TEXTURES));
  glhckAtlasFree(TEXTURES);
}

Powerup::Powerup(World* world, Type const type, Vec2D const& position, Vec2D const& velocity) :
  Sprite(world), o(0), type(type), v(velocity), life(10), shape(position, RADIUS)
{
  o = glhckSpriteNew(TEXTURE, 16, 16);
  glhckObjectTransformCoordinates(o, &TRANSFORM[type].transform, TRANSFORM[type].degree);
  glhckObjectSetMaterialFlags(o, GLHCK_MATERIAL_ALPHA);
  glhckObjectPositionf(o, position.x, position.y, 0);
}

Powerup::~Powerup()
{
  glhckObjectFree(o);
}

void Powerup::render()
{
  glhckObjectRender(o);
}

void Powerup::update(float delta)
{
  life -= delta;
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

bool Powerup::alive() const
{
  return life > 0;
}

CircleShape const* Powerup::getShape() const
{
  return &shape;
}

void Powerup::collide(Sprite const* other) {
  if(!alive())
    return;

  if(other->getEntityId() == Ship::ID) {
    Ship const* ship = static_cast<Ship const*>(other);
    if(shape.collidesWith(ship->getShape()))
    {
      life = 0;
    }
    return;
  }
}

Vec2D Powerup::getPosition() const
{
  kmVec3 const* pos = glhckObjectGetPosition(o);
  return {pos->x, pos->y};
}

Powerup::Type Powerup::getType() const
{
  return type;
}