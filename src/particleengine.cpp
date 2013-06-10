#include "particleengine.h"

std::vector<std::string> const ParticleEngine::IMAGES = {
  "img/spark.png",
  "img/flares/corona.png"
};

std::vector<float> const ParticleEngine::PARTICLE_SIZES = {
  3, 128
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
  gameWorld(world), particles(), live(0)
{
  for(Particle& p : particles)
  {
    p.o = glhckSpriteNew(atlas.getTexture(), 2, 2);
    glhckMaterialBlendFunc(glhckObjectGetMaterial(p.o), GLHCK_SRC_ALPHA, GLHCK_ONE);
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
  if(gameWorld->getPaused())
    return;

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

    p.angle += p.angularVelocity * delta;
    p.scale += p.scalingSpeed * delta;
    p.opacity += p.opacitySpeed * delta;

    glhckObjectMovef(p.o, p.velocity.x * delta, p.velocity.y * delta, 0);
    glhckObjectRotationf(p.o, 0, 0, p.angle);
    glhckObjectScalef(p.o, PARTICLE_SIZES[p.type] * p.scale, PARTICLE_SIZES[p.type] * p.scale, 1);
    glhckMaterialDiffuseb(glhckObjectGetMaterial(p.o), 255, 255, 255, min(1, max(0, p.opacity)) * 255);
  }
}

void ParticleEngine::addParticle(ParticleType const type, Vec2D const& position, Vec2D const& velocity, float const life,
                                 float const angle, float const angularVelocity,
                                 float const scale, float const scalingSpeed,
                                 float const opacity, float const opacitySpeed)
{
  if(live >= NUM_PARTICLES)
    return;

  Particle& p = particles[live];
  ++live;

  if(p.type != type)
  {
    p.type = type;
    glhckMaterialTextureTransform(glhckObjectGetMaterial(p.o), &atlas.getTransform(p.type).transform, atlas.getTransform(p.type).degree);
  }

  glhckObjectPositionf(p.o, position.x, position.y, 0);
  glhckObjectRotationf(p.o, 0, 0, angle);
  glhckObjectScalef(p.o, PARTICLE_SIZES[p.type] * scale, PARTICLE_SIZES[p.type] * scale, 1);
  glhckMaterialDiffuseb(glhckObjectGetMaterial(p.o), 255, 255, 255, opacity * 255);

  p.life = life;
  p.velocity = velocity;
  p.angle = angle;
  p.angularVelocity = angularVelocity;
  p.scale = scale;
  p.scalingSpeed = scalingSpeed;
  p.opacity = opacity;
  p.opacitySpeed = opacitySpeed;
}
