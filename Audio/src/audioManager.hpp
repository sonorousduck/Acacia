#pragma once

#include "SoundDevice.hpp"
#include "MusicSource.hpp"
#include "SoundSource.hpp"
#include "SourcePool.hpp"
#include "AudioType.hpp"
#include <unordered_map>
#include "SoundStream.hpp"

// ==============================================================
// 
// Temporary file to help me write the audio in the same way my game will work
//
// ==============================================================

namespace EbonyAudio
{

	class AudioManager
	{
	public:


		static void Init();
		
		static void Update();


		static std::unique_ptr<SoundSource> GetSource(AudioType type);
		static void ReturnSource(std::unique_ptr<SoundSource> source, AudioType type);

		// This may need to be changed for the float volume, but this can do to start
		static void SetVolume(std::uint8_t newVolume, AudioType type);

		static void Mute(AudioType type);

		static void SetPlaylist();
		static void PlayPlaylist();

		static std::shared_ptr<SoundStream> PlaySound(ALuint sound, AudioType type);

		static ALuint LoadSound(const std::string& name, const char* filename);
		static std::shared_ptr<MusicSource> LoadMusic(const std::string& name, const char* filename);

		static void PlayMusic(std::shared_ptr<MusicSource> source);


		static ALuint GetSound(const std::string& name);
		static std::shared_ptr<EbonyAudio::MusicSource> GetMusic(const std::string& name);

		//static void PauseAll();
		//static void UnpauseAll();


		~AudioManager()
		{
			UISourcePool.CleanUp();
			EntitySourcePool.CleanUp();
			MusicSourcePool.CleanUp();

			for (auto it = AudioManager::SoundEffectBuffers.begin(); it != AudioManager::SoundEffectBuffers.end(); it++)
			{
				alDeleteBuffers(1, &it->second);
			}


			delete device;



		}

		
	private:
		
		AudioManager() {}

		static SourcePool UISourcePool;
		static SourcePool EntitySourcePool;
		static SourcePool MusicSourcePool;
		static std::unordered_map<std::string, ALuint> SoundEffectBuffers;
		static std::unordered_map<std::string, std::shared_ptr<MusicSource>> MusicSources;
		//static std::vector<ALuint> SoundEffectBuffers;
		static ALCdevice* s_AudioDevice;

		static std::vector<std::shared_ptr<MusicSource>> musicPlaying;
		static std::vector<std::shared_ptr<SoundStream>> sourcesPlaying;

		static SoundDevice* device;

		static std::vector<std::uint8_t> volumes;
		



	};
}