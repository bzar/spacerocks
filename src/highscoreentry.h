#ifndef HIGHSCOREENTRY_H
#define HIGHSCOREENTRY_H

#include "glhck/glhck.h"
#include "util/vec2d.h"
#include "text.h"
#include <string>

class HighScoreWorld;

class HighScoreEntry
{
public:
  HighScoreEntry(HighScoreWorld* world, Vec2D const& centerIn, int const rank, std::string const& name, int const score, int zIndex = 0, int layer = 0);
  ~HighScoreEntry();
private:
  class Updater : public ew::Updatable
  {
  public:
    Updater(HighScoreEntry* parent);
    void update(const float delta);
  private:
    HighScoreEntry* parent;
    float t = 0;
  };

  friend class Updater;

  HighScoreWorld* world;
  Text* text;
  Updater* updater;

  int rank;
  std::string name;
  int score;
};

#endif // HIGHSCOREENTRY_H
