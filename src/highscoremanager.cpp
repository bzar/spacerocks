#include "highscoremanager.h"
#include "sparrowNet.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

std::vector<HighScoreManager::Entry> const HighScoreManager::DEFAULT_ENTRIES = {
  {"BZR", 1000, true},
  {"CDF", 900, true},
  {"PKB", 800, true},
  {"FOO", 700, true},
  {"BAR", 600, true},
  {"BAZ", 500, true},
  {"BAG", 400, true},
  {"ABC", 300, true},
  {"DEF", 200, true},
  {"GHI", 100, true}
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

    Entry e;
    while((data >> e.name >> e.score >> e.submitted) && entries.size() < MAX_ENTRIES)
    {
      entries.push_back(e);
    }
  }

  if(entries.empty())
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
    decrypted << e.name << std::endl << e.score << std::endl << e.submitted << std::endl;
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
  entries.push_back({name, score, false});
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

void HighScoreManager::submitToCompo4All()
{
  spNetC4AProfilePointer profile = nullptr;

  bool needed = false;
  for(Entry& e : entries)
  {
    if(!e.submitted)
    {
      needed = true;
      break;
    }
  }

  if(!needed)
    return;

  try
  {
    spInitNet();
    profile = spNetC4AGetProfile();
    if(!profile)
    {
      throw C4AException("No C4A profile found!");
    }

    for(Entry& e : entries)
    {
      if(e.submitted)
        continue;

      spNetC4ACommitScore(profile, "spacerocks", e.score, nullptr, 10000);
      while(spNetC4AGetStatus() == SP_C4A_PROGRESS);
      if(spNetC4AGetStatus() != SP_C4A_OK)
      {
        throw C4AException("Could not submit score to C4A!");
      }

      e.submitted = true;
    }
  }
  catch(C4AException e)
  {
    std::cerr << "ERROR: " << e.what() << std::endl;
  }


  if(profile)
  {
    spNetC4AFreeProfile(profile);
  }

  spQuitNet();
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
