#ifndef SPARK_HH
#define SPARK_HH

#include "sprite.h"
#include "GL/glhck.h"
#include "vec2d.h"

#include <map>
#include <string>

class Spark : public Sprite
{
public:
  Spark(World* world, float const life, Vec2D const& position, Vec2D const& velocity);
  ~Spark();

  static int const ID;
  int getEntityId() const { return ID; }

  void render();
  void update(float delta);
  bool alive() const;

private:
  static std::string const IMAGE;
  static glhckTexture* TEXTURE;
  glhckObject* o;
  float life;
  Vec2D v;
};

#endif
