#pragma once

#include "SoundDevice.hpp"
#include "MusicSource.hpp"
#include "SoundSource.hpp"
#include "SourcePool.hpp"
#include "AudioType.hpp"
#include <unordered_map>


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


		static AudioManager Init();
		
		static SoundSource createSoundSourceObject(SoundSource sound, AudioType type);


		static std::unique_ptr<SoundSource> GetSource(AudioType type);
		static void ReturnSource(std::unique_ptr<SoundSource> source, AudioType type);

		// This may need to be changed for the float volume, but this can do to start
		static void SetVolume(std::uint8_t newVolume, AudioType type);

		static void Mute(AudioType type);

		static void SetPlaylist();
		static void PlayPlaylist();

		static void PlaySound();

		static ALuint LoadSound(const std::string& name, const char* filename);


		~AudioManager()
		{
			UISourcePool.CleanUp();
			EntitySourcePool.CleanUp();
			MusicSourcePool.CleanUp();

			delete device;

		}

	private:
		// SoundDevice, Volume Levels, Sound Loader, Streaming, Source Pools, sounds
		AudioManager() {}




		static SourcePool UISourcePool;
		static SourcePool EntitySourcePool;
		static SourcePool MusicSourcePool;
		//static std::unordered_map<std::string, ALuint> SoundEffectBuffers;
		static std::vector<ALuint> SoundEffectBuffers;

		static SoundDevice* device;

		static std::vector<std::uint8_t> volumes;
		



	};
}