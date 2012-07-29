#ifndef SPRITE_HH
#define SPRITE_HH

#include "entity.h"
#include "GL/glhck.h"

struct World;

class Sprite : public Entity
{
public:
  Sprite(World* world, int zIndex = 0) : world(world), zIndex(zIndex) {};
  virtual void render() = 0;
  virtual void update(float delta) = 0;
  virtual void collide(Sprite const* other) {};
  virtual bool alive() const { return true; };

  int getZIndex() const { return zIndex; }

protected:
  struct TransformData
  {
    kmVec4 transform;
    short degree;
  };

  World* world;

private:
  int zIndex;
};
#endif