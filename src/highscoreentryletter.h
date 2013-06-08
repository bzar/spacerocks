#ifndef HIGHSCOREENTRYLETTER_H
#define HIGHSCOREENTRYLETTER_H

#include "glhck/glhck.h"
#include "util/vec2d.h"
#include "text.h"
#include <string>

class HighScoreEntryLetter
{
public:
  HighScoreEntryLetter(ew::RenderableWorld* world, Vec2D const& centerIn, int zIndex = 0, int layer = 0);
  ~HighScoreEntryLetter();

  void setCenterIn(Vec2D const& newCenterIn);
  void next();
  void prev();
  char getValue() const;

private:
  int index;
  Text* text;

  static std::string const VALUES;
};

#endif // HIGHSCOREENTRYLETTER_H
