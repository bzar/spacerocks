#include "../src/highscoremanager.h"
#include <iostream>

int main(int argc, char** argv)
{
  if(argc != 2)
  {
    std::cerr << "Usage: " << argv[0] << " <scores.enc>" << std::endl;
    return 1;
  }

  HighScoreManager manager(argv[1]);
  manager.load();
  const std::vector<HighScoreManager::Entry>& entries = manager.getEntries();

  if(entries.empty())
  {
    std::cout << 0;
  }
  else
  {
    //std::cout << entries.at(0).score << std::endl;
    for(HighScoreManager::Entry const& e : entries)
    {
      std::cout << e.name << " " << e.score << " " << e.submitted << std::endl;
    }
  }
}

