#include "hud.h"
#include "ship.h"
#include "ew/engine.h"

#include <iomanip>
#include <sstream>

UID const Hud::ID = getUID();

Hud::Hud(GameWorld* world) :
  Entity(world), Renderable(world, 0, 1),
  gameWorld(world), gameText(nullptr), fpsText(nullptr), gameFont(0), fpsFont(0)
{
  gameText = glhckTextNew(200, 200);
  fpsText = glhckTextNew(800, 40);

  gameFont = glhckTextNewFont(gameText, "fonts/DejaVuSans.ttf");
  fpsFont = glhckTextNewFont(fpsText, "fonts/DejaVuSans.ttf");
}

Hud::~Hud()
{
  glhckTextFree(gameText);
  glhckTextFree(fpsText);
}

void Hud::render(RenderContext* context)
{
  std::ostringstream ss;
  ss << "Level: " << (gameWorld->level.n + 1)
      << " | Score: " << gameWorld->player.score
      << " | Lives: " << gameWorld->player.lives
      << " | Weapons:"
      << " L" << gameWorld->player.ship->getLaserLevel()
      << " S" << gameWorld->player.ship->getSpreadLevel()
      << " B" << gameWorld->player.ship->getBeamLevel()
      << " P" << gameWorld->player.ship->getPlasmaLevel();

  glhckTextColor(gameText, 0, 0, 0, 255);
  glhckTextDraw(gameText, gameFont, 20, 6, 21, ss.str().data(), NULL);
  glhckTextRender(gameText);

  glhckTextColor(gameText, 255, 255, 255, 255);
  glhckTextDraw(gameText, gameFont, 20, 5, 20, ss.str().data(), NULL);
  glhckTextRender(gameText);


  ss.str("");
  ss << std::setprecision(2) << std::fixed
      << "FPS: " << gameWorld->engine->getTimer().getFPS()
      << " | total: " << gameWorld->engine->getTimer().getTotalTime()
      << "s | frame: " << gameWorld->engine->getTimer().getTicks();
  glhckTextDraw(fpsText, fpsFont, 14, 5, 35, ss.str().data(), NULL);
  glhckTextRender(fpsText);


}
