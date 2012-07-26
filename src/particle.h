#ifndef PARTICLE_HH
#define PARTICLE_HH

#include "sprite.h"

class Particle : public Sprite
{
public:
  virtual bool alive() const = 0;
};
#endif