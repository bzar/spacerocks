#include "powerup.h"
#include "ship.h"
#include "util/util.h"
#include <cstdlib>

float const Powerup::RADIUS = 16;
std::vector<std::string> const Powerup::IMAGES = {
  "img/powerup_laser.png",
  "img/powerup_spread.png",
  "img/powerup_continuous.png",
  "img/powerup_plasma.png",
  "img/powerup_extralife.png",
  "img/powerup_loselife.png",
  "img/powerup_shield.png"
};

TextureAtlas Powerup::atlas = TextureAtlas();
Sound Powerup::weaponSound = Sound();
Sound Powerup::extralifeSound = Sound();
Sound Powerup::loselifeSound = Sound();
Sound Powerup::shieldSound = Sound();

void Powerup::init()
{
  atlas = TextureAtlas(IMAGES);

  weaponSound.load("snd/sfx/weapon1probl.wav");
  extralifeSound.load("snd/sfx/game_showmenu.wav");
  loselifeSound.load("snd/sfx/game_hidemenu.wav");
  shieldSound.load("snd/sfx/antimaterhit.wav");
}

void Powerup::term()
{
  atlas = TextureAtlas();
}

Powerup::Powerup(GameWorld* world, Type const type, Vec2D const& position, Vec2D const& velocity) :
  ew::Entity(world), ew::Renderable(world), ew::Updatable(world), ew::Collidable(world),
  gameWorld(world), o(0), type(type), v(velocity), life(10), fadeOutTimer(0), shape(position, RADIUS)
{
  o = glhckSpriteNew(atlas.getTexture(), 32, 32);
  glhckMaterialTextureTransform(glhckObjectGetMaterial(o), &atlas.getTransform(type).transform, atlas.getTransform(type).degree);
  glhckObjectPositionf(o, position.x, position.y, 0);
}

Powerup::~Powerup()
{
  glhckObjectFree(o);
}

void Powerup::render(ew::RenderContext* context)
{
  glhckObjectRender(o);
}

void Powerup::update(float const delta)
{
  if(gameWorld->getPaused())
    return;

  life -= delta;

  if(life <= 0)
  {
    fadeOutTimer -= delta;
    if(fadeOutTimer <= 0)
    {
      world->removeEntity(this);
    }
    else
    {
      glhckMaterialDiffuseb(glhckObjectGetMaterial(o), 255, 255, 255, 255*max(0, fadeOutTimer)/FADEOUT_TIME);
      float const scale = lerp(2, 1, fadeOutTimer/FADEOUT_TIME);
      glhckObjectScalef(o, scale, scale, 1);
    }
  }
  else
  {
    glhckMaterialDiffuseb(glhckObjectGetMaterial(o), 255, 255, 255, lerp(0, 255, life));
  }


  glhckObjectMovef(o, v.x * delta, v.y * delta, 0);

  // FIXME: Do proper wrapping
  kmVec3 const* pos = glhckObjectGetPosition(o);
  if(pos->x < -400) {
     glhckObjectMovef(o, 800, 0, 0);
  } else if(pos->x > 400) {
     glhckObjectMovef(o, -800, 0, 0);
  }

  if(pos->y < -240) {
     glhckObjectMovef(o, 0, 480, 0);
  } else if(pos->y > 240) {
     glhckObjectMovef(o, 0, -480, 0);
  }

  shape.center = getPosition();
}

CircleShape const* Powerup::getShape() const
{
  return &shape;
}

void Powerup::collide(ew::Collidable const* other) {
  if(typeid(*other) == typeid(Ship)) {
    Ship const* ship = static_cast<Ship const*>(other);
    if(alive() && ship->alive() && shape.collidesWith(ship->getShape()))
    {
      life = 0;
      fadeOutTimer = FADEOUT_TIME;
      v = {0, 0};
      glhckMaterialBlendFunc(glhckObjectGetMaterial(o), GLHCK_SRC_ALPHA, GLHCK_ONE);

      if(type == LASER || type == SPREAD || type == BEAM || type == PLASMA) {
        weaponSound.play();
      } else if(type == EXTRALIFE) {
        extralifeSound.play();
      } else if(type == LOSELIFE) {
        loselifeSound.play();
      } else if(type == SHIELD) {
        shieldSound.play();
      }
    }
    return;
  }
}

Vec2D Powerup::getPosition() const
{
  kmVec3 const* pos = glhckObjectGetPosition(o);
  return {pos->x, pos->y};
}

Powerup::Type Powerup::getType() const
{
  return type;
}

bool Powerup::alive() const
{
  return life > 0;
}
