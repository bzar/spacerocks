#ifndef SPRITE_HH
#define SPRITE_HH

class Sprite
{
public:
  virtual void render() = 0;
  virtual void update(float delta) = 0;
};
#endif