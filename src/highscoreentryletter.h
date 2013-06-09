#ifndef HIGHSCOREENTRYLETTER_H
#define HIGHSCOREENTRYLETTER_H

#include "glhck/glhck.h"
#include "util/vec2d.h"
#include "text.h"
#include <string>

class GameOverWorld;

class HighScoreEntryLetter
{
public:
  HighScoreEntryLetter(GameOverWorld* world, Vec2D const& centerIn, int zIndex = 0, int layer = 0);
  ~HighScoreEntryLetter();

  void setCenterIn(Vec2D const& newCenterIn);
  void next();
  void prev();
  char getValue() const;

  bool getBlinking() const;
  void setBlinking(bool value);

private:
  class Updater : public ew::Updatable
  {
  public:
    Updater(HighScoreEntryLetter* parent);
    void update(const float delta);
  private:
    HighScoreEntryLetter* parent;
    float t = 0;
    constexpr static float const BLINK_INTERVAL = 0.2f;
  };

  friend class Updater;

  GameOverWorld* world;
  Updater* updater;
  int index;
  Text* text;
  bool blinking;
  static std::string const VALUES;
};

#endif // HIGHSCOREENTRYLETTER_H
