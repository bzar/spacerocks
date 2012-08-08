#include "weapon.h"
#include "util/util.h"

#include "laser.h"
#include "shot.h"
#include "plasma.h"

int Weapon::nextId = 0;

int const LaserWeapon::ID = Weapon::newWeaponId();
int const SpreadWeapon::ID = Weapon::newWeaponId();
int const BeamWeapon::ID = Weapon::newWeaponId();
int const PlasmaWeapon::ID = Weapon::newWeaponId();

LaserWeapon::LaserWeapon(GameWorld* world) : Weapon(world), cooldown(0)
{
}

void LaserWeapon::shoot(Vec2D const& position, Vec2D const& direction)
{
  if(cooldown > 0)
    return;

  Vec2D velocity = direction.scale(1200);
  Vec2D p = velocity.normal().uniti().scalei(8);
  Laser* laser1 = new Laser(world, 0.25, position + p, velocity);
  Laser* laser2 = new Laser(world, 0.25, position - p, velocity);
  cooldown = lerp(0.3, 0.05, (level - 1)/8.0);
}

void LaserWeapon::update(float const delta)
{
  cooldown = cooldown > 0 ? cooldown - delta : 0.0f;
}

SpreadWeapon::SpreadWeapon(GameWorld* world) : Weapon(world), cooldown(0)
{
}

void SpreadWeapon::shoot(Vec2D const& position, Vec2D const& direction)
{
  if(cooldown > 0)
    return;

  float spreadAngle = lerp(0.05, 0.45, (level - 1)/8.0);
  int shots = 2 * level + 1;
  for(int i = 0; i < shots; ++i)
  {
    Vec2D velocity = direction.scale(1200);
    velocity.rotatei(spreadAngle * i / (shots - 1) - spreadAngle / 2);
    Shot* shot = new Shot(world, 0.20, position, velocity);
  }
  cooldown = cooldown = lerp(0.8, 0.3, (level - 1)/8.0);
}

void SpreadWeapon::update(float const delta)
{
  cooldown = cooldown > 0 ? cooldown - delta : 0.0f;
}



BeamWeapon::BeamWeapon(GameWorld* world) : Weapon(world), beam(nullptr), time(0)
{
}

void BeamWeapon::shoot(Vec2D const& position, Vec2D const& direction)
{
  Vec2D beamVector = direction.scale(level * 32 * lerp(0, 1, time*8));
  if(beam == nullptr)
  {
    beam = new Beam(world, position, beamVector);
  }
  else
  {
    beam->setBasePosition(position);
    beam->setPositionDelta(beamVector);
  }
}

void BeamWeapon::stopShooting()
{
  world->removeEntity(beam);
  beam = nullptr;
  time = 0;
}

void BeamWeapon::update(float const delta)
{
  if(beam != nullptr)
    time += delta;
}

PlasmaWeapon::PlasmaWeapon(GameWorld* world) : Weapon(world), cooldown(0)
{
}

void PlasmaWeapon::shoot(Vec2D const& position, Vec2D const& direction)
{
  if(cooldown > 0)
    return;

  Vec2D velocity = direction.scale(1000);
  float power = lerp(4, 16, (level - 1)/8.0);
  Plasma* plasma = new Plasma(world, 0.5, power, position, velocity);
  cooldown = lerp(1.2, 0.8, (level - 1)/8.0);
}

void PlasmaWeapon::update(float const delta)
{
  cooldown = cooldown > 0 ? cooldown - delta : 0.0f;
}
