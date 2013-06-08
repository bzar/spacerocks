#include "highscoreentryletter.h"

std::string const HighScoreEntryLetter::VALUES = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

HighScoreEntryLetter::HighScoreEntryLetter(ew::RenderableWorld *world, Vec2D const& centerIn, int zIndex, int layer) :
  index(0), text(nullptr)
{
  text = new Text(world, VALUES.substr(index, 1), centerIn, zIndex, layer);
}

HighScoreEntryLetter::~HighScoreEntryLetter()
{
  delete text;
}

void HighScoreEntryLetter::setCenterIn(const Vec2D &newCenterIn)
{
  text->setCenterIn(newCenterIn);
}

void HighScoreEntryLetter::next()
{
  index = (index + 1) % VALUES.size();
  text->setContent(VALUES.substr(index, 1));
  text->refresh();
}

void HighScoreEntryLetter::prev()
{
  index = index == 0 ? VALUES.size() - 1 : index - 1;
  text->setContent(VALUES.substr(index, 1));
  text->refresh();
}

char HighScoreEntryLetter::getValue() const
{
  return VALUES.at(index);
}
