#ifndef PARTICLEENGINE_HH
#define PARTICLEENGINE_HH

#include "ew/updatable.h"
#include "ew/renderable.h"
#include "glhck/glhck.h"
#include "util/vec2d.h"

#include "textureatlas.h"
#include "gameworld.h"

#include <vector>
#include <array>
#include <string>

class ParticleEngine : public ew::Renderable, public ew::Updatable
{
public:
  enum ParticleType { SPARK, CORONA, TUNEL_RING, WAVE, NUM_PARTICLE_TYPES };

  static void init();
  static void term();

  ParticleEngine(GameWorld* world);
  ~ParticleEngine();

  void render(ew::RenderContext* context);
  void update(float const delta);

  void addParticle(ParticleType const type, Vec2D const& position, Vec2D const& velocity, float const life,
                   float const angle = 0, float const angleVelocity = 0,
                   float const scale = 1, float const scalingSpeed = 0,
                   float const opacity = 1, float const opacitySpeed = 0);

private:
  struct Particle
  {
    Particle() :
      o(nullptr), type(NUM_PARTICLE_TYPES), velocity({0, 0}), life(0.0f)
    {
    }
    glhckObject* o;
    ParticleType type;
    Vec2D velocity;
    float angle;
    float angularVelocity;
    float scale;
    float scalingSpeed;
    float opacity;
    float opacitySpeed;
    float life;
  };

  static std::vector<std::string> const IMAGES;
  static std::vector<float> const PARTICLE_SIZES;
  static TextureAtlas atlas;

  static int const NUM_PARTICLES = 64;
  typedef std::array<Particle, NUM_PARTICLES> Particles;

  GameWorld* gameWorld;
  Particles particles;
  int live;
};

#endif
