#ifndef GAMEOVERWORLD_H
#define GAMEOVERWORLD_H

#include "ew/renderableworld.h"
#include "ew/updatableworld.h"
#include "ew/controllableworld.h"
#include "ew/engine.h"
#include "text.h"
#include "highscoreentryletter.h"
#include "highscoremanager.h"

#include <vector>
#include <memory>

class GameOverWorld : public ew::RenderableWorld, public ew::UpdatableWorld,
    public ew::ControllableWorld
{
public:
  GameOverWorld(ew::Engine* engine);
  void setScore(int const newScore);

  void nextHighScoreEntryLetter();
  void prevHighScoreEntryLetter();
  bool setHighScoreEntryLetter(char letter);
  void enterHighScoreEntryLetter();
  void eraseHighScoreEntryLetter();
  bool enteringHighScore() const;

private:
  static int const NUM_HIGHSCORE_LETTERS = 3;
  void saveScore();
  bool isHighScore();

  ew::Engine* engine;

  int score;
  Text* scoreText;

  std::vector<HighScoreEntryLetter*> highScoreEntryLetters;
  int focusedHighScoreEntryLetter = -1;

  HighScoreManager highScoreManager;
};

#endif // GAMEOVERWORLD_H
