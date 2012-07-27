#ifndef SHIP_HH
#define SHIP_HH

#include "sprite.h"
#include "GL/glhck.h"
#include "vec2d.h"

#include <map>
#include <string>

class Ship : public Sprite
{
public:
  Ship(Vec2D const& position, Vec2D const& velocity);
  ~Ship();

  void render();
  void update(float delta);

  void turningLeft(bool const value);
  void turningRight(bool const value);
  void accelerating(bool const value);

  Vec2D getVelocity() const;
  Vec2D getPosition() const;
  float getAngle() const;

private:
  enum ImageType { DEFAULT, ACCELERATING,
                   LEFT, LEFT_ACCELERATING,
                   RIGHT, RIGHT_ACCELERATING,
                   NUM_IMAGES };
  static std::string const IMAGES[NUM_IMAGES];
  static glhckAtlas* TEXTURES;

  glhckObject* o;
  Vec2D v;

  bool turnLeft;
  bool turnRight;
  bool accelerate;
};
#endif
