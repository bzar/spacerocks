#include "sound.h"
#include <iostream>

std::unordered_map<int, Sound*> Sound::channelMap = std::unordered_map<int, Sound*>();

Sound::Sound() :
  buffer(nullptr), channel(-1)
{

}

Sound::Sound(std::string const& filename) :
    buffer(nullptr), channel(-1)
{
    buffer = Mix_LoadWAV(filename.data());
}

Sound::~Sound()
{
  Mix_FreeChunk(buffer);
}

void Sound::init()
{
  SDL_Init(SDL_INIT_AUDIO);

  int audio_rate = 22050;
  Uint16 audio_format = AUDIO_S16SYS;
  int audio_channels = 2;
  int audio_buffers = 2048;

  Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers);
  Mix_ChannelFinished(Sound::unsetChannel);
}

void Sound::exit()
{
  Mix_CloseAudio();
  SDL_Quit();
}

void Sound::load(const std::string& filename)
{
  if(buffer != nullptr)
  {
    Mix_FreeChunk(buffer);
  }
  buffer = Mix_LoadWAV(filename.data());
}

bool Sound::loaded() const
{
  return buffer != nullptr;
}

int Sound::play(float distance, int position, int loops)
{
    channel = Mix_PlayChannel(-1, buffer, loops);
    if(channel != -1)
    {
      std::cout << "reserved channel " << channel << std::endl;
      Mix_SetPosition(channel, position, static_cast<int>(distance * 255));
      channelMap[channel] = this;
    }
    return channel;
}

int Sound::stop()
{
  if(channel != -1 && channelMap[channel] == this)
  {
    Mix_HaltChannel(channel);
  }
}

int Sound::stopAndPlay(float distance, int position, int loops)
{
  if(channelMap[channel] == this)
  {
    std::cout << "rewinding channel " << channel << std::endl;
    Mix_SetPosition(channel, position, static_cast<int>(distance * 255));
    Mix_PlayChannel(channel, buffer, loops);
    std::cout << "re-reserving channel " << channel << std::endl;
    channelMap[channel] = this;
    return channel;
  }
  else
  {
    return play(distance, position, loops);
  }
}

int Sound::fadeOut(int durationMs)
{
  return Mix_FadeOutChannel(channel, durationMs);
}

void Sound::unsetChannel(int channel)
{
  std::cout << "unsetchannel " << channel << std::endl;
  auto i = channelMap.find(channel);
  if(i != channelMap.end() && i->second != nullptr)
  {
    i->second = nullptr;
  }
}
