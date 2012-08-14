#ifndef SHIP_HH
#define SHIP_HH

#include "gameworld.h"
#include "ew/renderable.h"
#include "ew/updatable.h"
#include "ew/collidable.h"
#include "ew/controllable.h"

#include "GL/glhck.h"
#include "util/vec2d.h"
#include "util/circleshape.h"
#include "weapon.h"
#include "textureatlas.h"

#include <string>
#include <vector>

class Ship : public Renderable, public Updatable, public Collidable, public Controllable
{
public:
  static void init();
  static void term();

  Ship(GameWorld* world, Vec2D const& position, Vec2D const& velocity);
  ~Ship();

  static UID const ID;
  UID getEntityId() const { return ID; }

  void render(RenderContext* context);
  void update(float const delta);
  virtual void collide(Collidable const* other);
  void control(ControlContext* context);

  bool alive() const;
  virtual CircleShape const* getShape() const;

  void turnLeft(bool const value);
  void turnRight(bool const value);
  void accelerate(bool const value);
  void shoot(bool const value);
  void reset();

  Vec2D getVelocity() const;
  Vec2D getPosition() const;

  void nextWeapon();
  void prevWeapon();

  void increaseLaserLevel() { laser.increaseLevel(); }
  void increaseSpreadLevel() { spread.increaseLevel(); }
  void increaseBeamLevel() { beam.increaseLevel(); }
  void increasePlasmaLevel() { plasma.increaseLevel(); }

  int getLaserLevel() { return laser.getLevel(); }
  int getSpreadLevel() { return spread.getLevel(); }
  int getBeamLevel() { return beam.getLevel(); }
  int getPlasmaLevel() { return plasma.getLevel(); }

  void die();

private:
  enum ImageType { DEFAULT, ACCELERATING,
                   LEFT, LEFT_ACCELERATING,
                   RIGHT, RIGHT_ACCELERATING,
                   SHIELD, NUM_IMAGES };
  static std::vector<std::string> const IMAGES;
  static TextureAtlas atlas;

  static int const RADIUS = 10;

  GameWorld* gameWorld;
  glhckObject* o;
  glhckObject* shield;
  Vec2D v;

  bool turningLeft;
  bool turningRight;
  bool accelerating;
  bool shooting;

  float shieldLeft;

  Weapon* weapon;
  LaserWeapon laser;
  SpreadWeapon spread;
  BeamWeapon beam;
  PlasmaWeapon plasma;
  std::vector<Weapon*> weapons;

  bool dead;
  CircleShape shape;

};
#endif
