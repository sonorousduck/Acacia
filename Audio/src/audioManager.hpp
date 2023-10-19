#pragma once

#include "SoundDevice.hpp"
#include "MusicSource.hpp"
#include "SoundSource.hpp"
#include "SourcePool.hpp"


// ==============================================================
// 
// Temporary file to help me write the audio in the same way my game will work
//
// ==============================================================

namespace Ebony
{
	enum AudioType
	{
		ENTITY = 1,
		UI = 2,
		MUSIC = 4,
	};


	class AudioManager
	{
	public:


		static AudioManager Init();
		
		SoundSource createSoundSourceObject(SoundSource sound, AudioType type);



	private:
		// SoundDevice, Volume Levels, Sound Loader, Streaming, Source Pools, sounds
		AudioManager(SoundDevice* device)
		{

		}


	};
}