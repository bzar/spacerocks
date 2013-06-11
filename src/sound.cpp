#include "sound.h"

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
    Mix_SetPosition(channel, position, static_cast<int>(distance * 255));
    return channel;
}

int Sound::stop()
{
  Mix_HaltChannel(channel);
}

int Sound::fadeOut(int durationMs)
{
  return Mix_FadeOutChannel(channel, durationMs);
}
