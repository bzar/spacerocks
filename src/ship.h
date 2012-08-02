#ifndef SHIP_HH
#define SHIP_HH

#include "sprite.h"
#include "GL/glhck.h"
#include "vec2d.h"
#include "circleshape.h"

#include <string>
#include <vector>

class Ship : public Sprite
{
public:
  static void init();

  enum Weapon { RAPID, SPREAD, CONTINUOUS, PLASMA, NUM_WEAPONS };

  Ship(World* world, Vec2D const& position, Vec2D const& velocity);
  ~Ship();

  static int const ID;
  int getEntityId() const { return ID; }

  void render();
  void update(float delta);
  bool alive() const;
  virtual CircleShape const* getShape() const;

  virtual void collide(Sprite const* other);

  void turnLeft(bool const value);
  void turnRight(bool const value);
  void accelerate(bool const value);
  void shoot(bool const value);
  void reset();

  Vec2D getVelocity() const;
  Vec2D getPosition() const;

  void setWeapon(Weapon const value);
  Weapon getWeapon() const;
  void nextWeapon();
  void prevWeapon();

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

  static int const RADIUS = 10;

  glhckObject* o;
  glhckObject* shield;
  Vec2D v;

  bool turningLeft;
  bool turningRight;
  bool accelerating;
  bool shooting;

  float shieldLeft;
  float weaponCooldown;
  Weapon weapon;
  bool dead;
  CircleShape shape;

};
#endif
