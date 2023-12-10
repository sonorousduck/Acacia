#pragma once

#include "../Audio/src/SoundDevice.hpp"
#include "../Audio/src/MusicSource.hpp"
#include "../Audio/src/SoundSource.hpp"
#include "../Audio/src/SourcePool.hpp"

#include "SDL_mixer.h"
#include <cassert>
#include <deque>
#include <cstdint>


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

		static void Init(std::uint8_t channelCount = 32, std::uint8_t uiChannels = 6, std::uint8_t entityChannels = 26)
		{
			assert(uiChannels + entityChannels == channelCount);

			Mix_AllocateChannels(channelCount);

			sourceCount = channelCount;
		}
		
		static void Cleanup()
		{

		}




	private:
		AudioManager() {};
		static std::deque<std::uint8_t> UIChannelPool;
		static std::deque<std::uint8_t> EntityChannelPool;
		static int MusicSourcePool;

		static std::uint8_t sourceCount;


	};
}