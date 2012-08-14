#include "particleengine.h"

ew::UID const ParticleEngine::ID = ew::getUID();

std::vector<std::string> const ParticleEngine::IMAGES = {
  "img/spark.png",
};

std::vector<float> const ParticleEngine::PARTICLE_SIZES = {
  3,
};

TextureAtlas ParticleEngine::atlas = TextureAtlas();

void ParticleEngine::init()
{
  atlas = TextureAtlas(IMAGES);
}

void ParticleEngine::term()
{
  atlas = TextureAtlas();
}

ParticleEngine::ParticleEngine(GameWorld* world) :
  ew::Entity(world), ew::Renderable(world, 0, 1), ew::Updatable(world),
  particles(), live(0)
{
  for(Particle& p : particles)
  {
    p.o = glhckSpriteNew(atlas.getTexture(), 1, 1);
    glhckObjectSetMaterialFlags(p.o, GLHCK_MATERIAL_ALPHA);
  }
}

ParticleEngine::~ParticleEngine()
{
  for(Particle& p : particles)
  {
    glhckObjectFree(p.o);
  }
}

void ParticleEngine::render(ew::RenderContext* context)
{
  for(int i = 0; i < live; ++i)
  {
    glhckObjectRender(particles[i].o);
  }
}

void ParticleEngine::update(float const delta)
{

  for(int i = 0; i < live; ++i)
  {
    Particle& p = particles[i];

    p.life -= delta;

    if(p.life <= 0)
    {
      --live;
      if(i < live)
      {
        Particle tmp = p;
        p = particles[live];
        particles[live] = tmp;
      }
      continue;
    }

    glhckObjectMovef(p.o, p.velocity.x * delta, p.velocity.y * delta, 0);
  }
}

void ParticleEngine::addParticle(ParticleType const type, Vec2D const& position, Vec2D const& velocity, float const life)
{
  if(live >= NUM_PARTICLES)
    return;

  Particle& p = particles[live];
  ++live;

  if(p.type != type)
  {
    p.type = type;
    glhckObjectTransformCoordinates(p.o, &atlas.getTransform(p.type).transform, atlas.getTransform(p.type).degree);
    glhckObjectScalef(p.o, PARTICLE_SIZES[p.type], PARTICLE_SIZES[p.type], 1);
  }

  glhckObjectPositionf(p.o, position.x, position.y, 0);
  p.life = life;
  p.velocity = velocity;
}