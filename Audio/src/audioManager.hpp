#pragma once

#include "SoundDevice.hpp"
#include "MusicSource.hpp"
#include "SoundSource.hpp"
#include "SourcePool.hpp"
#include "AudioType.hpp"


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


		static SoundSource GetSource(AudioType type);
		static void ReturnSource(AudioType type);

		// This may need to be changed for the float volume, but this can do to start
		static void SetVolume(float volume, AudioType type);

		static void Mute(AudioType type);

		static void SetPlaylist();
		static void PlayPlaylist();

		static void PlaySound();


	private:
		// SoundDevice, Volume Levels, Sound Loader, Streaming, Source Pools, sounds
		AudioManager(SoundDevice* device)
		{

		}

		static SourcePool UISourcePool;
		static SourcePool EntitySourcePool;
		static SourcePool MusicSourcePool;
		



	};
}