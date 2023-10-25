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
    //SoundSource mySpeaker{};


    // First, we need to create an audio manager
    EbonyAudio::AudioManager audioManager = EbonyAudio::AudioManager::Init();

    audioManager.LoadSound("wall", "SoundEffects/wall.wav");
    audioManager.LoadSound("magnet", "SoundEffects/magnet_action.wav");

    //std::unique_ptr<SoundSource> speaker = audioManager.GetSource(EbonyAudio::UI);
    //std::unique_ptr<SoundSource> speaker1 = audioManager.GetSource(EbonyAudio::UI);

    auto sound = audioManager.GetSound("wall");
    auto sound1 = audioManager.GetSound("magnet");

    audioManager.PlaySound(sound, EbonyAudio::UI); 

    //speaker->Play(sound);

    //std::cout << "Beep Beep" << std::endl;
    //std::unique_ptr<SoundSource> soundSource = audioManager.GetSource(EbonyAudio::AudioType::UI);
    //soundSource->Play(sound1);




   // SoundDevice::init();
   // EbonyAudio::Music::Init();

   //auto source = EbonyAudio::MusicSource::LoadFromFile("Music/song18.mp3");
   // auto source1 = EbonyAudio::MusicSource::LoadFromFile("Music/TownTheme.wav");

   //source.SetLoop(true);

   // EbonyAudio::Music::Play(source);
   //EbonyAudio::Music::Play(source1);

   //uint32_t sound1 = SoundBuffer::get()->addSoundEffect("SoundEffects/wall.wav");

   // SoundSource anotherSpeaker{};


   //mySpeaker.Play(sound1);

    //speaker->Play(sound);
    //speaker1->Play(sound1);
    //mySpeaker.Play(sound1);
    //mySpeaker.Play(sound1);


    while (true)
    {
        audioManager.Update();
    }

    /*ALint state = AL_PLAYING;
    while (state == AL_PLAYING && alGetError() == AL_NO_ERROR)
    {
    	std::cout << "Currently playing sound" << std::endl;
    	alGetSourcei(speaker1->source, AL_SOURCE_STATE, &state);
    }*/

    //audioManager.ReturnSource(std::move(speaker), EbonyAudio::UI);
    //audioManager.ReturnSource(std::move(speaker1), EbonyAudio::UI);


   /*while (true)
   {

   }*/
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