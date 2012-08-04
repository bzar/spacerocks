#ifndef WEAPON_HH
#define WEAPON_HH

#include "vec2d.h"
#include "beam.h"

class World;

class Weapon
{
public:
  Weapon(World* world) : world(world), level(0) {}

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
  World* world;
  int level;

private:
  static int nextId;
};


class LaserWeapon : public Weapon
{
public:
  LaserWeapon(World* world);
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
  SpreadWeapon(World* world);
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
  BeamWeapon(World* world);
  static int const ID;
  int getWeaponId() const { return ID; }
  void shoot(Vec2D const& position, Vec2D const& direction);
  void stopShooting();

private:
  Beam* beam;
};


class PlasmaWeapon : public Weapon
{
public:
  PlasmaWeapon(World* world);
  static int const ID;
  int getWeaponId() const { return ID; }
  void shoot(Vec2D const& position, Vec2D const& direction);
  void update(float const delta);

private:
  float cooldown;
};


#endif