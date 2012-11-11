#ifndef SHIP_HH
#define SHIP_HH

#include "gameworld.h"
#include "ew/renderable.h"
#include "ew/updatable.h"
#include "ew/collidable.h"
#include "ew/controllable.h"

#include "glhck/glhck.h"
#include "util/vec2d.h"
#include "util/circleshape.h"
#include "weapon.h"
#include "textureatlas.h"
#include "sound.h"

#include <string>
#include <vector>

class Ship : public ew::Renderable, public ew::Updatable, public ew::Collidable, public ew::Controllable
{
public:
  static void init();
  static void term();

  Ship(GameWorld* world, Vec2D const& position, Vec2D const& velocity);
  ~Ship();

  static ew::UID const ID;
  ew::UID getEntityId() const { return ID; }

  void render(ew::RenderContext* context);
  void update(float const delta);
  virtual void collide(ew::Collidable const* other);
  void control(ew::ControlContext* context);

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
  static int const MAX_SPEED = 800;

  GameWorld* gameWorld;
  glhckObject* o;
  glhckObject* shield;
  Vec2D v;

  bool turningLeft;
  bool turningRight;
  bool accelerating;
  bool shooting;

  float immortalityLeft;
  int shields;

  Weapon* weapon;
  LaserWeapon laser;
  SpreadWeapon spread;
  BeamWeapon beam;
  PlasmaWeapon plasma;
  std::vector<Weapon*> weapons;

  bool dead;
  CircleShape shape;
  
  static Sound destroySound;

};
#endif
