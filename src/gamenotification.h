#ifndef GAMENOTIFICATION_HH
#define GAMENOTIFICATION_HH

#include "ew/updatable.h"
#include "ew/renderable.h"
#include "glhck/glhck.h"
#include "util/vec2d.h"

#include "textureatlas.h"
#include "gameworld.h"

#include <vector>
#include <array>
#include <string>

class GameNotification : public ew::Renderable, public ew::Updatable
{
public:
  enum Size { TINY = 10, SMALL = 14, MEDIUM = 20, LARGE = 32 };
  static void init();
  static void term();

  GameNotification(GameWorld* world, std::string const& text, Size size);
  ~GameNotification();

  static ew::UID const ID;
  ew::UID getEntityId() const { return ID; }

  void render(ew::RenderContext* context);
  void update(float const delta);

private:
  Size size;
  glhckObject* o;
  
  float life;
};

#endif
