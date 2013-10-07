#include "highscoreworld.h"
#include "prop.h"

HighScoreWorld::HighScoreWorld(ew::Engine *engine) :
  ew::World(), ew::RenderableWorld(), ew::UpdatableWorld(),
  ew::ControllableWorld(),
  engine(engine),
  highScoreManager("scores.enc"),
  texts()
{
  new Prop(this, "img/highscores.png", 800, 480, 0, -1);
}

void HighScoreWorld::enter()
{
  for(HighScoreEntry* t : texts)
  {
    delete t;
  }
  texts.clear();

  highScoreManager.load();

  int const ROWS_PER_COLUMN = 5;
  int const COLUMNS = highScoreManager.getEntries().size() / (ROWS_PER_COLUMN + 1) + 1;
  int const COLUMN_SIZE = highScoreManager.getEntries().size() / COLUMNS + (highScoreManager.getEntries().size() % COLUMNS == 0 ? 0 : 1);

  int i = 0;
  for(HighScoreManager::Entry const& entry : highScoreManager.getEntries())
  {
    int const COLUMN = i / COLUMN_SIZE;
    int const ROW = i % COLUMN_SIZE;

    texts.push_back(new HighScoreEntry(this, Vec2D{-400 + 800/COLUMNS * (COLUMN + 0.5) , 0 - (ROW + 0.5f) * 40}, i + 1, entry.name, entry.score));
    ++i;
  }
}

void HighScoreWorld::exit()
{
  for(HighScoreEntry* t : texts)
  {
    delete t;
  }
  texts.clear();
  glhckMemoryGraph();
}
