#include "music.h"
#include <iostream>

Music::Music() :
    buffers(), index(-1)
{

}

Music::Music(std::string const& filename) :
    buffers(), index(-1)
{
  Mix_Music* buffer = Mix_LoadMUS(filename.data());
  if(buffer)
  {
    buffers.push_back(buffer);
  }
  else
  {
    std::cerr << "Error loading music: " << Mix_GetError() << std::endl;
  }
}

Music::Music(const std::vector< std::string >& filenames) :
    buffers(), index(-1)
{
  for(std::string const& filename : filenames)
  {
    Mix_Music* buffer = Mix_LoadMUS(filename.data());
    if(buffer)
    {
      buffers.push_back(buffer);
    }
    else
    {
      std::cerr << "Error loading music: " << Mix_GetError() << std::endl;
    }
  }
}


Music::~Music()
{
  for(auto buffer : buffers)
  {
    Mix_FreeMusic(buffer);
  }
}

void Music::load(const std::vector< std::string >& filenames)
{
  for(std::string const& filename : filenames)
  {
    Mix_Music* buffer = Mix_LoadMUS(filename.data());
    if(buffer)
    {
      buffers.push_back(buffer);
    }
    else
    {
      std::cerr << "Error loading music: " << Mix_GetError() << std::endl;
    }
  }
}

void Music::play()
{
  if(index < 0)
  {
    index = 0;
  }
  
  if(index >= buffers.size())
  {
    return;
  }
  
  std::cout << "Playing music index " << index << std::endl;
  if(Mix_PlayMusic(buffers.at(index), 1) != 0)
  {
    std::cerr << "Error playing music: " << Mix_GetError() << std::endl;
  }
}

bool Music::playing()
{
  return Mix_PlayingMusic();
}

void Music::next()
{
  if(buffers.empty())
  {
    index = 0;
  }
  else
  {
    index = (index + 1) % buffers.size();
  }
}

void Music::stop()
{
  Mix_HaltMusic();
}

