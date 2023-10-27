#include "SoundDevice.hpp"
#include "SoundBuffer.hpp"
#include <iostream>
#include "SoundSource.hpp"
#include "MusicSource.hpp"
#include <thread>
#include <chrono>
#include "audioManager.hpp"

// Tutorials thanks to https://www.youtube.com/watch?v=pYK8XZHV74s (Code, Tech and Tutorials)

int main()
{
    // First, we need to create an audio manager
    EbonyAudio::AudioManager audioManager = EbonyAudio::AudioManager::Init();

    audioManager.LoadSound("wall", "SoundEffects/wall.wav");
    audioManager.LoadSound("magnet", "SoundEffects/magnet_action.wav");


    auto sound = audioManager.GetSound("wall");
    auto sound1 = audioManager.GetSound("magnet");

    std::shared_ptr<EbonyAudio::SoundStream> stream = audioManager.PlaySound(sound, EbonyAudio::UI);

    audioManager.LoadMusic("song18", "Music/song18.wav");
    std::shared_ptr<EbonyAudio::MusicSource> source = audioManager.GetMusic("song18");

    audioManager.PlayMusic(source);

    while (true)
    {
        audioManager.Update();
    }

    return 0;
}