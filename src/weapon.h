#ifndef WEAPON_HH
#define WEAPON_HH

#include "util/vec2d.h"
#include "beam.h"

class GameWorld;

class Weapon
{
public:
  Weapon(GameWorld* world) : world(world), level(0) {}

  virtual int getWeaponId() const = 0;

  virtual void shoot(Vec2D const& position, Vec2D const& direction) = 0;
  virtual void startShooting() {};
  virtual void stopShooting() {};
  virtual void update(float const delta) {};

  void setLevel(int const value) { level = value; }
  int getLevel() const { return level; }
  void increaseLevel() { level += level < MAX_LEVEL ? 1 : 0; }
  void decreaseLevel() { level -= level > MIN_LEVEL ? 1 : 0; }

protected:
  static int newWeaponId() { return nextId++; }

  static int const MIN_LEVEL = 1;
  static int const MAX_LEVEL = 8;
  GameWorld* world;
  int level;

private:
  static int nextId;
};


class LaserWeapon : public Weapon
{
public:
  LaserWeapon(GameWorld* world);
  static int const ID;
  int getWeaponId() const { return ID; }
  void shoot(Vec2D const& position, Vec2D const& direction);
  void update(float const delta);

private:
  float cooldown;
};


class SpreadWeapon : public Weapon
{
public:
  SpreadWeapon(GameWorld* world);
  static int const ID;
  int getWeaponId() const { return ID; }
  void shoot(Vec2D const& position, Vec2D const& direction);
  void update(float const delta);

private:
  float cooldown;
};


class BeamWeapon : public Weapon
{
public:
  BeamWeapon(GameWorld* world);
  static int const ID;
  int getWeaponId() const { return ID; }
  void shoot(Vec2D const& position, Vec2D const& direction);
  void stopShooting();
  void update(float const delta);

private:
  Beam* beam;
  float time;
};


class PlasmaWeapon : public Weapon
{
public:
  PlasmaWeapon(GameWorld* world);
  static int const ID;
  int getWeaponId() const { return ID; }
  void shoot(Vec2D const& position, Vec2D const& direction);
  void update(float const delta);

private:
  float cooldown;
};


#endif