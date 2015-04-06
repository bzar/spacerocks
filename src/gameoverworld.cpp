#include "gameoverworld.h"
#include "prop.h"
#include "text.h"
#include <sstream>

GameOverWorld::GameOverWorld(ew::Engine *engine) :
  ew::World(), ew::RenderableWorld(), ew::UpdatableWorld(),
  ew::ControllableWorld(),
  engine(engine), score(0), scoreText(nullptr),
  highScoreEntryLetters(), highScoreManager("scores.enc")
{
  new Prop(this, "img/gameover.png", 800, 480, 0, -1);
  scoreText = new Text(this, "Score: ", {0, -30});

  for(int i = 0; i < NUM_HIGHSCORE_LETTERS; ++i)
  {
    float pos = 0 - (NUM_HIGHSCORE_LETTERS - 1) * 40.0f/2.0f + i * 40.0f;
    highScoreEntryLetters.push_back(new HighScoreEntryLetter(this, Vec2D{pos, -70}));
  }

}

void GameOverWorld::setScore(const int newScore)
{
  // No high scores for cheaters
  if(newScore < 0)
  {
    scoreText->setContent("Cheater!");
    scoreText->refresh();
    for(HighScoreEntryLetter* h : highScoreEntryLetters)
    {
      h->setVisible(false);
    }
    focusedHighScoreEntryLetter = -1;
    return;
  }

  score = newScore;

  std::ostringstream oss;
  oss << "Score: " << score;
  scoreText->setContent(oss.str());
  scoreText->refresh();

  if(isHighScore())
  {
    focusedHighScoreEntryLetter = 0;
    for(HighScoreEntryLetter* h : highScoreEntryLetters)
    {
      h->setVisible(true);
      h->setBlinking(false);
    }
    highScoreEntryLetters.at(focusedHighScoreEntryLetter)->setBlinking(true);
  }
  else
  {
    for(HighScoreEntryLetter* h : highScoreEntryLetters)
    {
      h->setVisible(false);
    }
    focusedHighScoreEntryLetter = -1;
    #ifdef C4A_ENABLED
      highScoreManager.submitToCompo4All();
    #endif
  }
}

void GameOverWorld::nextHighScoreEntryLetter()
{
  if(focusedHighScoreEntryLetter < 0)
    return;

  highScoreEntryLetters.at(focusedHighScoreEntryLetter)->next();
}

void GameOverWorld::prevHighScoreEntryLetter()
{
  if(focusedHighScoreEntryLetter < 0)
    return;

  highScoreEntryLetters.at(focusedHighScoreEntryLetter)->prev();
}

bool GameOverWorld::setHighScoreEntryLetter(char letter)
{
  if(focusedHighScoreEntryLetter < 0)
    return false;

  return highScoreEntryLetters.at(focusedHighScoreEntryLetter)->set(letter);
}

void GameOverWorld::enterHighScoreEntryLetter()
{
  if(focusedHighScoreEntryLetter < 0)
    return;

  highScoreEntryLetters.at(focusedHighScoreEntryLetter)->setBlinking(false);
  focusedHighScoreEntryLetter += 1;

  if(focusedHighScoreEntryLetter == NUM_HIGHSCORE_LETTERS)
  {
    focusedHighScoreEntryLetter = -1;
    saveScore();
  }
  else
  {
    highScoreEntryLetters.at(focusedHighScoreEntryLetter)->setBlinking(true);
  }
}

void GameOverWorld::eraseHighScoreEntryLetter()
{
  if(focusedHighScoreEntryLetter <= 0 )
    return;

  highScoreEntryLetters.at(focusedHighScoreEntryLetter)->setBlinking(false);
  focusedHighScoreEntryLetter -= 1;
  highScoreEntryLetters.at(focusedHighScoreEntryLetter)->setBlinking(true);
}

bool GameOverWorld::enteringHighScore() const
{
  return focusedHighScoreEntryLetter >= 0;
}

void GameOverWorld::saveScore()
{
  std::ostringstream oss;
  for(HighScoreEntryLetter* entryLetter : highScoreEntryLetters)
  {
    oss << entryLetter->getValue();
  }
  highScoreManager.addEntry(oss.str(), score);
#ifdef C4A_ENABLED
  highScoreManager.submitToCompo4All();
#endif
  highScoreManager.save();
}

bool GameOverWorld::isHighScore()
{
  return highScoreManager.isHighScore(score);
}
