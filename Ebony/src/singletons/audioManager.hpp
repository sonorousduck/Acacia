#pragma once

#include "../Audio/src/SoundDevice.hpp"
#include "../Audio/src/MusicSource.hpp"
#include "../Audio/src/SoundSource.hpp"
#include "../Audio/src/SourcePool.hpp"


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


		static void Init();
		
		SoundSource createSoundSourceObject(SoundSource sound, AudioType type);



	private:

	};
}