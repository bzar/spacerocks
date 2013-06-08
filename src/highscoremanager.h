#ifndef HIGHSCOREMANAGER_H
#define HIGHSCOREMANAGER_H

#include <string>
#include <vector>

class HighScoreManager
{
public:
  struct Entry {
    std::string name;
    int score;
  };

  HighScoreManager(std::string const& filename);
  void load();
  void save();
  bool isHighScore(int const score) const;
  void addEntry(std::string const& name, int const score);
  std::vector<Entry> const& getEntries() const;

private:
  static int const MAX_ENTRIES = 10;
  static void awesomeScoreEncryptionSystem(std::istream& from, std::ostream& to);

  void sort();

  std::string filename;
  std::vector<Entry> entries;
};

#endif // HIGHSCOREMANAGER_H
