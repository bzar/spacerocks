#include "ship.h"

int const Ship::ID = Entity::newEntityId();

std::string const Ship::IMAGES[NUM_IMAGES] = {
  "img/ship.png", "img/ship_accelerating.png",
  "img/ship_left.png", "img/ship_left_accelerating.png",
  "img/ship_right.png", "img/ship_right_accelerating.png",
};

std::vector<TransformData> Ship::TRANSFORM;
glhckTexture *Ship::ATLAS_TEXTURE = NULL;

Ship::Ship(World* world, Vec2D const& position, Vec2D const& velocity) :
  Sprite(world), o(0), v(velocity),
  turnLeft(false), turnRight(false), accelerate(false)
{
  if (!ATLAS_TEXTURE) {
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

    ATLAS_TEXTURE = glhckTextureRef(glhckAtlasGetTexture(TEXTURES));
    glhckAtlasFree(TEXTURES);
  }

  o = glhckSpriteNew(ATLAS_TEXTURE, 32, 32);
  glhckObjectScalef(o, 0.75, 0.75, 0.75);
  glhckObjectTransformCoordinates(o, &TRANSFORM[0].transform, TRANSFORM[0].degree);

  glhckObjectSetMaterialFlags(o, GLHCK_MATERIAL_ALPHA);
  glhckObjectPositionf(o, position.x, position.y, 0);
}

Ship::~Ship()
{
  glhckObjectFree(o);
}


void Ship::render()
{
  glhckObjectRender(o);
}

void Ship::update(float delta)
{
  if(turnLeft) glhckObjectRotatef(o, 0, 0, delta * 120);
  if(turnRight) glhckObjectRotatef(o, 0, 0, delta * -120);

  if(accelerate)
  {
    kmScalar angle = glhckObjectGetRotation(o)->z;
    Vec2D acceleration(0, 120 *  delta);
    acceleration.rotatei(angle / 360);
    v += acceleration;
  }

  ImageType t = DEFAULT;
  if(accelerate) {
    t = turnLeft && !turnRight ? LEFT_ACCELERATING :
        turnRight && !turnLeft ? RIGHT_ACCELERATING :
        ACCELERATING;
  } else {
    t = turnLeft && !turnRight ? LEFT :
        turnRight && !turnLeft ? RIGHT :
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
}

void Ship::turningLeft(bool const value)
{
  turnLeft = value;
}

void Ship::turningRight(bool const value)
{
  turnRight = value;
}

void Ship::accelerating(bool const value)
{
  accelerate = value;
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

float Ship::getAngle() const
{
  kmVec3 const* rot = glhckObjectGetRotation(o);
  return rot->z / 360;
}


