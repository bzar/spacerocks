#include "highscoremanager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

HighScoreManager::HighScoreManager(const std::string &filename) :
  filename(filename)
{
  load();
}

void HighScoreManager::load()
{
  entries.clear();
  std::ifstream ifs(filename);
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
