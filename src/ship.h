#ifndef SHIP_HH
#define SHIP_HH

#include "sprite.h"
#include "GL/glhck.h"
#include "vec2d.h"

#include <string>
#include <vector>

class Ship : public Sprite
{
public:
  static void init();

  Ship(World* world, Vec2D const& position, Vec2D const& velocity);
  ~Ship();

  static int const ID;
  int getEntityId() const { return ID; }

  void render();
  void update(float delta);
  bool alive() const;

  virtual void collide(Sprite const* other);

  void turnLeft(bool const value);
  void turnRight(bool const value);
  void accelerate(bool const value);
  void shoot(bool const value);

  Vec2D getVelocity() const;
  Vec2D getPosition() const;
  float getRadius() const;
  float getAngle() const;

  void die();

private:
  enum ImageType { DEFAULT, ACCELERATING,
                   LEFT, LEFT_ACCELERATING,
                   RIGHT, RIGHT_ACCELERATING,
                   NUM_IMAGES };
  static std::string const IMAGES[NUM_IMAGES];
  static std::string const SHIELD_IMAGE;
  static std::vector<TransformData> TRANSFORM;
  static glhckTexture *TEXTURE;
  static glhckTexture *SHIELD_TEXTURE;

  static int const RADIUS = 24;

  glhckObject* o;
  glhckObject* shield;
  Vec2D v;

  bool turningLeft;
  bool turningRight;
  bool accelerating;
  bool shooting;

  float shieldLeft;
  float laserCooldown;
  bool dead;
};
#endif
