#include "highscoreentry.h"
#include "highscoreworld.h"
#include <sstream>

HighScoreEntry::HighScoreEntry(HighScoreWorld *world, const Vec2D &centerIn, const int rank, const std::string &name, const int score, int zIndex, int layer) :
  world(world), text(nullptr), updater(new Updater(this)), rank(rank), name(name), score(score)
{
  std::ostringstream oss;
  oss << rank << ". " << name << " - " << score;
  text = new Text(world, oss.str(), centerIn, zIndex, layer);
}

HighScoreEntry::~HighScoreEntry()
{
  delete updater;
  delete text;
}

HighScoreEntry::Updater::Updater(HighScoreEntry* parent) :
  ew::Entity(parent->world), ew::Updatable(parent->world), parent(parent), t(0)
{

}

void HighScoreEntry::Updater::update(const float delta)
{
  t += delta;
  float const progress = max(0, min(1.0f, t - parent->rank * 0.2f));
  parent->text->setOpacity(progress);
}
