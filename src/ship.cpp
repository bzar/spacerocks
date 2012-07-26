#include "ship.h"

std::string const Ship::IMAGES[NUM_IMAGES] = {
  "img/ship.png", "img/ship_accelerating.png",
  "img/ship_left.png", "img/ship_left_accelerating.png",
  "img/ship_right.png", "img/ship_right_accelerating.png",
};

glhckTexture* Ship::TEXTURES[NUM_IMAGES] = {nullptr};

Ship::Ship(Vec2D const& position, Vec2D const& velocity) : 
  Sprite(), o(0), v(velocity), 
  turnLeft(false), turnRight(false), accelerate(false)
{
  o = glhckSpriteNew(IMAGES[DEFAULT].data(), 2, GLHCK_TEXTURE_DEFAULTS);
  
  if(TEXTURES[DEFAULT] == nullptr)
  {
    for(int i = 0; i < NUM_IMAGES; ++i)
    {
      TEXTURES[i] = glhckTextureNew(IMAGES[i].data(), GLHCK_TEXTURE_DEFAULTS);
    }
  }
  
  glhckObjectSetMaterialFlags(o, GLHCK_MATERIAL_ALPHA);
  glhckObjectPositionf(o, position.x, position.y, 0);
}

void Ship::render()
{
  glhckObjectDraw(o);
}

void Ship::update(float delta)
{
  if(turnLeft) glhckObjectRotatef(o, 0, 0, delta * 120);
  if(turnRight) glhckObjectRotatef(o, 0, 0, delta * -120);
  
  if(accelerate)
  {
    kmScalar angle = glhckObjectGetRotation(o)->z;
    Vec2D acceleration(0, 8 *  delta);
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
  
  if(TEXTURES[t] != glhckObjectGetTexture(o))
  {
    glhckObjectSetTexture(o, TEXTURES[t]);
  }
  
  glhckObjectMovef(o, v.x * delta, v.y * delta, 0);
  
  
  // FIXME: Do proper wrapping
  kmVec3 const* pos = glhckObjectGetPosition(o);
  if(pos->x < -27) {
     glhckObjectMovef(o, 54, 0, 0);
  } else if(pos->x > 27) {
     glhckObjectMovef(o, -54, 0, 0);
  }
  
  if(pos->y < -16) {
     glhckObjectMovef(o, 0, 32, 0);
  } else if(pos->y > 16) {
     glhckObjectMovef(o, 0, -32, 0);
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

