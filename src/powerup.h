#ifndef POWERUP_HH
#define POWERUP_HH

#include "sprite.h"
#include "GL/glhck.h"
#include "vec2d.h"
#include "circleshape.h"

#include <string>
#include <vector>

class Powerup : public Sprite
{
public:
  static void init();

  enum Type { RAPID, SPREAD, CONTINUOUS, PLASMA, NUM_TYPES };
  Powerup(World* world, Type const type, Vec2D const& position, Vec2D const& velocity);
  ~Powerup();

  static int const ID;
  int getEntityId() const { return ID; }

  void render();
  void update(float delta);

  bool alive() const;
  virtual CircleShape const* getShape() const;
  virtual void collide(Sprite const* other);

  Vec2D getPosition() const;
  Type getType() const;

private:
  static float const RADIUS;
  static std::string const IMAGES[NUM_TYPES];
  static std::vector<TransformData> TRANSFORM;
  static glhckTexture *TEXTURE;

  glhckObject* o;
  Type type;
  Vec2D v;
  float life;
  CircleShape shape;
};

#endif