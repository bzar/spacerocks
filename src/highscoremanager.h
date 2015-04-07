#ifndef HIGHSCOREMANAGER_H
#define HIGHSCOREMANAGER_H

#include <string>
#include <vector>
#include <exception>

class HighScoreManager
{
public:
  struct Entry {
    std::string name;
    int score;
    bool submitted;
  };

  HighScoreManager(std::string const& filename);
  void load();
  void save();
  bool isHighScore(int const score) const;
  void addEntry(std::string const& name, int const score);
  std::vector<Entry> const& getEntries() const;
#ifdef C4A_ENABLED
  void submitToCompo4All();
#endif

private:
  class C4AException : public std::exception
  {
  public:
    C4AException(std::string const& reason) : _reason(reason) {}
    const char* what() { return _reason.data(); }
  private:
    std::string const _reason;
  };


  static int const MAX_ENTRIES = 10;
  static std::vector<Entry> const DEFAULT_ENTRIES;
  static void awesomeScoreEncryptionSystem(std::istream& from, std::ostream& to);

  void sort();

  std::string filename;
  std::vector<Entry> entries;
};

#endif // HIGHSCOREMANAGER_H
