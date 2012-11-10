#ifndef MUSIC_H
#define MUSIC_H

#include <string>
#include <vector>

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

class Music
{
public:
  Music();
  Music(std::string const& filename);
  Music(std::vector<std::string> const& filenames);
  ~Music();

  void load(std::vector<std::string> const& filenames);
  
  void play();
  bool playing();
  void next();
  void stop();

private:
  std::vector<Mix_Music*> buffers;
  int index;
};

#endif // MUSIC_H
