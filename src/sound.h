#ifndef SOUND_H
#define SOUND_H

#include <string>

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

class Sound
{
public:
    Sound();
    Sound(std::string const& filename);
    ~Sound();

    static void init();
    static void exit();

    void load(const std::string& filename);
    bool loaded() const;
    int play(float distance = 0.0f, int position = 0, int loops = 0);
    int stop();
    int fadeOut(int durationMs);
private:
    int channel;
    Mix_Chunk *buffer;
};

#endif // SOUND_H
