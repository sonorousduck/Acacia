#include "SoundDevice.hpp"
#include "SoundBuffer.hpp"
#include <iostream>
#include "SoundSource.hpp"
#include "MusicBuffer.hpp"

// Tutorials thanks to https://www.youtube.com/watch?v=pYK8XZHV74s (Code, Tech and Tutorials)

int main()
{
    SoundDevice::init();

    uint32_t sound1 = SoundBuffer::get()->addSoundEffect("SoundEffects/wall.wav");
    uint32_t sound2 = SoundBuffer::get()->addSoundEffect("SoundEffects/magnet_action.wav");

    SoundSource mySpeaker{};
    SoundSource anotherSpeaker{};


    mySpeaker.Play(sound1);
    anotherSpeaker.Play(sound2);

    MusicBuffer myMusic("Music/TownTheme.wav");
    myMusic.Play();

    ALint state = AL_PLAYING;

    while (state == AL_PLAYING && alGetError() == AL_NO_ERROR)
    {
        myMusic.UpdateBufferStream();
        alGetSourcei(myMusic.getSource(), AL_SOURCE_STATE, &state);
    }

    std::cout << "Got here" << std::endl;

    return 0;
}