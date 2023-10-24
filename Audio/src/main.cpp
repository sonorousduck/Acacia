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

    uint32_t sound = audioManager.LoadSound("wall", "SoundEffects/wall.wav");

    std::unique_ptr<SoundSource> speaker = audioManager.GetSource(EbonyAudio::UI);
    //speaker->Play(sound);

    //std::cout << "Beep Beep" << std::endl;
    //std::unique_ptr<SoundSource> soundSource = audioManager.GetSource(EbonyAudio::AudioType::UI);
    //soundSource->Play(sound1);




   // SoundDevice::init();
   // EbonyAudio::Music::Init();

   // auto source = EbonyAudio::MusicSource::LoadFromFile("Music/song18.mp3");
   // auto source1 = EbonyAudio::MusicSource::LoadFromFile("Music/TownTheme.wav");

   // source.SetLoop(true);

   // EbonyAudio::Music::Play(source);
   ////EbonyAudio::Music::Play(source1);

   uint32_t sound1 = SoundBuffer::get()->addSoundEffect("SoundEffects/wall.wav");
   // uint32_t sound2 = SoundBuffer::get()->addSoundEffect("SoundEffects/magnet_action.wav");

   SoundSource mySpeaker{};
   // SoundSource anotherSpeaker{};


   mySpeaker.Play(sound1);

   while (true)
   {

   }
   // anotherSpeaker.Play(sound2);

    //MusicSource myMusic("Music/song18.mp3");
    //MusicBuffer myMusic("Music/TownTheme.wav");

    //myMusic.Play();

   /* ALint state = AL_PLAYING;

    while (state == AL_PLAYING && alGetError() == AL_NO_ERROR)
    {
        myMusic.UpdateBufferStream();
        alGetSourcei(myMusic.getSource(), AL_SOURCE_STATE, &state);
    }

    std::cout << "Got here" << std::endl;*/

	/*while (true)
	{
        source.UpdateBufferStream();
        source1.UpdateBufferStream();
	}*/


    return 0;
}