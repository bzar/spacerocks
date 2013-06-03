#include "gameoverworld.h"
#include "prop.h"
#include "text.h"
#include <sstream>

GameOverWorld::GameOverWorld(ew::Engine *engine) :
  ew::World(), ew::RenderableWorld(), ew::UpdatableWorld(),
  ew::ControllableWorld(),
  engine(engine), score(0), scoreText(nullptr)
{
  new Prop(this, "img/gameover.png", 800, 480, 0, -1);
  scoreText = new Text(this, "Score: -", {400, 300});
}

void GameOverWorld::setScore(const int newScore)
{
  score = newScore;

  std::ostringstream oss;
  oss << "Score: " << score;
  scoreText->setContent(oss.str());
  scoreText->refresh();
}
