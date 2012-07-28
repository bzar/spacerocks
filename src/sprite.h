#ifndef SPRITE_HH
#define SPRITE_HH

#include "entity.h"

class Sprite : public Entity
{
public:
  virtual void render() = 0;
  virtual void update(float delta) = 0;
  virtual void collide(Sprite const* other) {};
  virtual bool alive() const { return true; };
};
#endif