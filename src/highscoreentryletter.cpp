#include "highscoreentryletter.h"
#include "gameoverworld.h"

std::string const HighScoreEntryLetter::VALUES = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

HighScoreEntryLetter::HighScoreEntryLetter(GameOverWorld *world, Vec2D const& centerIn, int zIndex, int layer) :
  world(world), updater(new Updater(this)), index(0), text(nullptr)
{
  text = new Text(world, VALUES.substr(index, 1), centerIn, zIndex, layer);
}

HighScoreEntryLetter::~HighScoreEntryLetter()
{
  delete updater;
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

HighScoreEntryLetter::Updater::Updater(HighScoreEntryLetter* parent) :
  ew::Entity(parent->world), ew::Updatable(parent->world), parent(parent), t(0)
{

}

void HighScoreEntryLetter::Updater::update(const float delta)
{
  t += delta;
  parent->text->setVisible(parent->visible && (!parent->blinking || static_cast<int>(t / BLINK_INTERVAL) % 2 == 0));
}

bool HighScoreEntryLetter::getBlinking() const
{
  return blinking;
}

void HighScoreEntryLetter::setBlinking(bool value)
{
  blinking = value;
}

bool HighScoreEntryLetter::getVisible() const
{
  return visible;
}

void HighScoreEntryLetter::setVisible(bool value)
{
  visible = value;
}
