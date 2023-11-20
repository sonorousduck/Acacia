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
    EbonyAudio::AudioManager::Init();

    EbonyAudio::AudioManager::LoadSound("wall", "SoundEffects/wall.wav");
    EbonyAudio::AudioManager::LoadSound("magnet", "SoundEffects/magnet_action.wav");


    auto sound = EbonyAudio::AudioManager::GetSound("wall");
    auto sound1 = EbonyAudio::AudioManager::GetSound("magnet");

    std::shared_ptr<EbonyAudio::SoundStream> stream = EbonyAudio::AudioManager::PlaySound(sound, EbonyAudio::UI);

    EbonyAudio::AudioManager::LoadMusic("song18", "Music/cavethemeb4.ogg");
    std::shared_ptr<EbonyAudio::MusicSource> source = EbonyAudio::AudioManager::GetMusic("song18");

    EbonyAudio::AudioManager::PlayMusic(source);

    while (true)
    {
        EbonyAudio::AudioManager::Update();
    }

    return 0;
}