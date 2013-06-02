#include "hud.h"
#include "ship.h"
#include "ew/engine.h"

#include <iomanip>
#include <sstream>

Hud::Hud(GameWorld* world) :
  ew::Entity(world), ew::Renderable(world, 0, 1),
  gameWorld(world), gameText(nullptr), fpsText(nullptr), gameFont(0), fpsFont(0)
{
  gameText = glhckTextNew(200, 200);
  fpsText = glhckTextNew(800, 40);

  gameFont = glhckTextFontNew(gameText, "fonts/DejaVuSans.ttf");
  fpsFont = glhckTextFontNew(fpsText, "fonts/DejaVuSans.ttf");
}

Hud::~Hud()
{
  glhckTextFree(gameText);
  glhckTextFree(fpsText);
}

void Hud::render(ew::RenderContext* context)
{
  std::ostringstream ss;
  ss << "Level: " << (gameWorld->level.n + 1)
     << " | Score: " << gameWorld->player.score
     << " | Lives: " << gameWorld->player.lives
     << " | Weapons:";

  if(gameWorld->player.ship->getLaserLevel() > 0)
  {
    bool selected = gameWorld->player.ship->getSelectedWeaponId() == LaserWeapon::ID;
    ss << (selected ? " [" : " ") << "L" << gameWorld->player.ship->getLaserLevel() << (selected ? "]" : "");
  }

  if(gameWorld->player.ship->getSpreadLevel() > 0)
  {
    bool selected = gameWorld->player.ship->getSelectedWeaponId() == SpreadWeapon::ID;
    ss << (selected ? " [" : " ") << "S" << gameWorld->player.ship->getSpreadLevel() << (selected ? "]" : "");
  }

  if(gameWorld->player.ship->getBeamLevel() > 0)
  {
    bool selected = gameWorld->player.ship->getSelectedWeaponId() == BeamWeapon::ID;
    ss << (selected ? " [" : " ") << "B" << gameWorld->player.ship->getBeamLevel() << (selected ? "]" : "");
  }

  if(gameWorld->player.ship->getPlasmaLevel() > 0)
  {
    bool selected = gameWorld->player.ship->getSelectedWeaponId() == PlasmaWeapon::ID;
    ss << (selected ? " [" : " ") << "P" << gameWorld->player.ship->getPlasmaLevel() << (selected ? "]" : "");
  }

  glhckTextColorb(gameText, 0, 0, 0, 255);
  glhckTextStash(gameText, gameFont, 20, 6, 21, ss.str().data(), nullptr);
  glhckTextRender(gameText);
  glhckTextClear(gameText);

  glhckTextColorb(gameText, 255, 255, 255, 255);
  glhckTextStash(gameText, gameFont, 20, 5, 20, ss.str().data(), nullptr);
  glhckTextRender(gameText);
  glhckTextClear(gameText);

  if(showFPS)
  {
    ss.str("");
    ss << std::setprecision(2) << std::fixed
        << "FPS: " << gameWorld->engine->getTimer().getFPS()
        << " | total: " << gameWorld->engine->getTimer().getTotalTime()
        << "s | frame: " << gameWorld->engine->getTimer().getTicks();
    glhckTextStash(fpsText, fpsFont, 14, 5, 35, ss.str().data(), nullptr);
    glhckTextRender(fpsText);
    glhckTextClear(fpsText);
  }

}

void Hud::toggleShowFPS()
{
  showFPS = !showFPS;
}
