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
  score = newScore;

  std::ostringstream oss;
  oss << "Score: " << score;
  scoreText->setContent(oss.str());
  scoreText->refresh();

  if(isHighScore())
  {
    focusedHighScoreEntryLetter = 0;
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

void GameOverWorld::enterHighScoreEntryLetter()
{
  if(focusedHighScoreEntryLetter < 0)
    return;

  focusedHighScoreEntryLetter += 1;

  if(focusedHighScoreEntryLetter == NUM_HIGHSCORE_LETTERS)
  {
    focusedHighScoreEntryLetter = -1;
    saveScore();
  }
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
  highScoreManager.save();
}

bool GameOverWorld::isHighScore()
{
  return highScoreManager.isHighScore(score);
}
