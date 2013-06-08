#include "highscoremanager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

std::vector<HighScoreManager::Entry> const HighScoreManager::DEFAULT_ENTRIES = {
  {"BZR", 1000},
  {"CDF", 900},
  {"PKB", 800},
  {"FOO", 700},
  {"BAR", 600},
  {"BAZ", 500},
  {"BAG", 400},
  {"ABC", 300},
  {"DEF", 200},
  {"GHI", 100}
};

HighScoreManager::HighScoreManager(const std::string &filename) :
  filename(filename)
{
  load();
}

void HighScoreManager::load()
{
  entries.clear();
  std::ifstream ifs(filename);

  if(ifs)
  {
    std::ostringstream decrypted;
    awesomeScoreEncryptionSystem(ifs, decrypted);
    std::istringstream data(decrypted.str());

    std::cout << "Loading scores: " << decrypted.str();

    Entry e;
    while((data >> e.name >> e.score) && entries.size() < MAX_ENTRIES)
    {
      std::cout << "Loaded score: " << e.name << " - " << e.score << std::endl;
      entries.push_back(e);
    }
  }
  else
  {
    entries = DEFAULT_ENTRIES;
  }
  sort();
}

void HighScoreManager::save()
{
  std::ostringstream decrypted;
  for(Entry const& e : entries)
  {
    decrypted << e.name << std::endl << e.score << std::endl;
  }

  std::ofstream ofs(filename);
  std::istringstream iss(decrypted.str());
  awesomeScoreEncryptionSystem(iss, ofs);
}

bool HighScoreManager::isHighScore(const int score) const
{
  return entries.size() < MAX_ENTRIES || entries.at(entries.size() - 1).score < score;
}

void HighScoreManager::addEntry(const std::string &name, const int score)
{
  entries.push_back({name, score});
  sort();
  while(entries.size() > MAX_ENTRIES)
  {
    entries.pop_back();
  }
}

const std::vector<HighScoreManager::Entry> &HighScoreManager::getEntries() const
{
  return entries;
}

void HighScoreManager::awesomeScoreEncryptionSystem(std::istream &from, std::ostream &to)
{
  char c;
  while(from.get(c))
  {
    unsigned char ec[2] = {static_cast<char>(c + 128), '\0'};
    to << ec;
  }
}

void HighScoreManager::sort()
{
  std::stable_sort(entries.begin(), entries.end(), [](Entry const& a, Entry const& b) {return a.score > b.score;});
}
