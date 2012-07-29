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
  float getAngle() const;

private:
  enum ImageType { DEFAULT, ACCELERATING,
                   LEFT, LEFT_ACCELERATING,
                   RIGHT, RIGHT_ACCELERATING,
                   NUM_IMAGES };
  static std::string const IMAGES[NUM_IMAGES];
  static std::vector<TransformData> TRANSFORM;
  static glhckTexture *TEXTURE;
  static const int RADIUS = 16;

  glhckObject* o;
  Vec2D v;

  bool turningLeft;
  bool turningRight;
  bool accelerating;
  bool shooting;

  float laserCooldown;
  bool dead;
};
#endif
