#ifndef COLLIDABLE_HH
#define COLLIDABLE_HH

class Collidable
{
public:
  virtual void collide(Collidable const* other) {};
};
#endif